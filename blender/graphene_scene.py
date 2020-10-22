#
# Copyright (c) 2013 Pavlo Lavrenenko
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

bl_info = {
    "name": "Graphene scene format",
    "author": "Pavlo Lavrenenko",
    "version": (0, 2, 0),
    "blender": (2, 80, 0),
    "location": "File > Import-Export",
    "description": "Export scene for Graphene engine",
    "category": "Import-Export"
}

import struct
import os
import bpy
import math
from bpy.types import Operator
from bpy.props import StringProperty
from bpy_extras.io_utils import ExportHelper
from bpy_extras.node_shader_utils import rgba_to_rgb


class ExportEntity(Operator, ExportHelper):

    bl_idname = "export.graphene_scene"
    bl_label = "Export Graphene scene"

    filename_ext = ".scene"
    filter_glob: StringProperty(default="*.scene", options={'HIDDEN'})

    def __make_relative(self, target_filepath):
        # Per Blender Manual: Files & Data System - Blender File - Blend Files
        # When relative paths are supported, the File Browser provides a Relative Path checkbox,
        # when entering the path into a text field, use a double slash prefix (//) to make it so.
        if target_filepath[0:2] == "//":
            blend_filedir = os.path.dirname(self.context.blend_data.filepath)
            target_filepath = os.path.join(blend_filedir, target_filepath[2:])

        # Use forward slash separator (works well on Win32/Linux platforms)
        scene_filedir = os.path.dirname(self.filepath)
        return os.path.relpath(target_filepath, scene_filedir).replace('\\', '/')

    def __write_scene(self):
        previous_mode = None
        if bpy.ops.object.mode_set.poll():
            previous_mode = self.context.object.mode
            bpy.ops.object.mode_set(mode='OBJECT')

        export_world = self.context.scene.world
        assert export_world.use_nodes, "Scene requires Use Nodes enabled for the world"

        # Blender light names to Graphene LightType enum (src/Light.h)
        light_types = {
            'POINT': 0,
            'SPOT' : 1,
            'SUN'  : 2
        }

        export_entities = []
        export_lights = []
        export_player = None

        depsgraph = self.context.evaluated_depsgraph_get()
        for depsgraph_object in depsgraph.object_instances:
            # Evaluated objects have modifiers applied
            if depsgraph_object.is_instance:
                continue

            evaluated_object = depsgraph_object.object

            if evaluated_object.type == 'EMPTY':
                if evaluated_object.name == 'Player':
                    assert export_player is None, "Scene supports single Player placeholder only"
                    export_player = evaluated_object
                    continue

                if evaluated_object.instance_collection is not None:
                    assert evaluated_object.instance_collection.library is not None, "Scene supports linked objects only"
                    export_entities.append(evaluated_object)
                    continue

            if evaluated_object.type == 'LIGHT':
                assert evaluated_object.data.type in light_types, "Scene supports %s lights only" % list(light_types.keys())
                export_lights.append(evaluated_object)
                continue

        assert export_player is not None, "Scene requires Player placeholder defined"

        with open(self.filepath, "wb") as f:
            f.write(struct.pack("<4s", bytearray("GPNW", 'ASCII')))
            f.write(struct.pack("<3b1b", *bl_info["version"], 0))

            export_name = self.context.scene.name
            export_name += "\0" * (255 - len(export_name))
            f.write(struct.pack("<255s1b", bytearray(export_name, 'ASCII'), 0))

            world_inputs = export_world.node_tree.nodes['Background'].inputs
            ambient_color = rgba_to_rgb(world_inputs['Color'].default_value)
            ambient_energy = world_inputs['Strength'].default_value

            f.write(struct.pack("<3f", *ambient_color))
            f.write(struct.pack("<1f", ambient_energy))

            # Translation from Blender X(right), Y(forward), Z(up) to Graphene X(right), Y(up), Z(forward)
            # coordinate system is straightforward: swap Y and Z scale factors, rotation angles and positions.
            # The rotation angles should also be reversed, rotation around X amended with 90 degree turn:
            # default Blender lamp direction is down, default Graphene light direction is forward.

            f.write(struct.pack("<3f", -export_player.rotation_euler.x, -export_player.rotation_euler.z, -export_player.rotation_euler.y))
            f.write(struct.pack("<3f", export_player.location.x, export_player.location.z, export_player.location.y))

            export_skybox = export_world.get("skybox_texture", "")
            export_skybox = self.__make_relative(export_skybox)
            export_skybox += "\0" * (255 - len(export_skybox))
            f.write(struct.pack("<255s1b", bytearray(export_skybox, 'ASCII'), 0))

            f.write(struct.pack("<1i1i", len(export_entities), len(export_lights)))

            for entity in export_entities:
                export_name = entity.name
                export_name += "\0" * (255 - len(export_name))
                f.write(struct.pack("<255s1b", bytearray(export_name, 'ASCII'), 0))

                export_filepath = entity.instance_collection.library.filepath.rsplit('.', 1)[0] + '.entity'
                export_filepath = self.__make_relative(export_filepath)
                export_filepath += "\0" * (255 - len(export_filepath))
                f.write(struct.pack("<255s1b", bytearray(export_filepath, 'ASCII'), 0))

                f.write(struct.pack("<3f", entity.scale.x, entity.scale.z, entity.scale.y))
                f.write(struct.pack("<3f", -entity.rotation_euler.x, -entity.rotation_euler.z, -entity.rotation_euler.y))
                f.write(struct.pack("<3f", entity.location.x, entity.location.z, entity.location.y))

            for light in export_lights:
                export_name = light.name
                export_name += "\0" * (255 - len(export_name))
                f.write(struct.pack("<255s1b", bytearray(export_name, 'ASCII'), 0))

                f.write(struct.pack("<1i", light_types[light.data.type]))
                f.write(struct.pack("<1f", light.data.energy / 100.0)) # Empirical watts to ratio

                if light.data.type != 'SUN':
                    f.write(struct.pack("<1f", light.data.distance))
                else:
                    f.write(struct.pack("<1f", 0.0))

                if light.data.type == 'SPOT':
                    f.write(struct.pack("<1f1f", light.data.spot_size, light.data.spot_blend))
                else:
                    f.write(struct.pack("<1f1f", 0.0, 0.0))

                rotation_x = math.pi / 2.0
                f.write(struct.pack("<3f", light.data.color.r, light.data.color.g, light.data.color.b))
                f.write(struct.pack("<3f", -light.rotation_euler.x + rotation_x, -light.rotation_euler.z, -light.rotation_euler.y))
                f.write(struct.pack("<3f", light.location.x, light.location.z, light.location.y))

        if previous_mode is not None and bpy.ops.object.mode_set.poll():
            bpy.ops.object.mode_set(mode=previous_mode)

        return {'FINISHED'}

    def execute(self, context):
        self.context = context
        return self.__write_scene()


def menu_func_export(self, context):
    self.layout.operator(ExportEntity.bl_idname, text="Graphene (.scene)")


def register():
    bpy.utils.register_class(ExportEntity)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportEntity)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
