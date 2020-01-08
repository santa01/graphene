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
    "blender": (2, 7, 0),
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


def make_relative(context, scene_filepath, target_filepath):
    # Per Blender Manual: Files & Data System - Blender File - Blend Files
    # When relative paths are supported, the File Browser provides a Relative Path checkbox,
    # when entering the path into a text field, use a double slash prefix (//) to make it so.
    if target_filepath[0:2] == "//":
        blend_filedir = os.path.dirname(context.blend_data.filepath)
        target_filepath = os.path.join(blend_filedir, target_filepath[2:])

    # Use forward slash separator (works well on Win32/Linux platforms)
    scene_filedir = os.path.dirname(scene_filepath)
    return os.path.relpath(target_filepath, scene_filedir).replace('\\', '/')


def write_scene(context, scene_filepath):
    previous_mode = None
    if bpy.ops.object.mode_set.poll():
        previous_mode = context.object.mode
        bpy.ops.object.mode_set(mode='OBJECT')

    export_entities = []
    export_lights = []
    export_player = None

    # Blender light names to Graphene LightType enum (src/Light.h)
    light_types = {
        'POINT': 0,
        'SPOT':  1,
        'SUN':   2
    }

    for scene_object in context.scene.objects:
        if scene_object.type == 'EMPTY':
            if scene_object.dupli_type == 'NONE' and scene_object.name == 'Player':
                assert export_player is None, "Scene can have single Player EMPTY only"
                export_player = scene_object
                continue

            if scene_object.dupli_type == 'GROUP' and scene_object.dupli_group.library is not None:
                export_entities.append(scene_object)
                continue

        if scene_object.type == 'LAMP' and scene_object.data.type in light_types.keys():
            export_lights.append(scene_object)
            continue

    assert export_player is not None, "Scene requires Player EMPTY defined"

    with open(scene_filepath, "wb") as f:
        f.write(struct.pack("<4s", bytearray("GPNW", 'ASCII')))
        f.write(struct.pack("<3b1b", *bl_info["version"], 0))

        export_name = context.scene.name
        export_name += "\0" * (255 - len(export_name))
        f.write(struct.pack("<255s1b", bytearray(export_name, 'ASCII'), 0))

        ambient_color = context.scene.world.ambient_color
        f.write(struct.pack("<3f", ambient_color.r, ambient_color.g, ambient_color.b))
        f.write(struct.pack("<1f", context.scene.world.get("ambient_energy", 1.0)))

        # Translation from Blender X(right), Y(forward), Z(up) to Graphene X(right), Y(up), Z(forward)
        # coordinate system is straightforward: swap Y and Z scale factors, rotation angles and positions.
        # The rotation angles should also be reversed, rotation around X amended with 90 degree turn:
        # default Blender lamp direction is down, default Graphene light direction is forward.

        f.write(struct.pack("<3f", -export_player.rotation_euler.x, -export_player.rotation_euler.z, -export_player.rotation_euler.y))
        f.write(struct.pack("<3f", export_player.location.x, export_player.location.z, export_player.location.y))

        export_skybox = context.scene.world.get("skybox_texture", "")
        export_skybox = make_relative(context, scene_filepath, export_skybox)
        export_skybox += "\0" * (255 - len(export_skybox))
        f.write(struct.pack("<255s1b", bytearray(export_skybox, 'ASCII'), 0))

        f.write(struct.pack("<1i1i", len(export_entities), len(export_lights)))

        for entity in export_entities:
            export_name = entity.name
            export_name += "\0" * (255 - len(export_name))
            f.write(struct.pack("<255s1b", bytearray(export_name, 'ASCII'), 0))

            export_filepath = entity.dupli_group.library.filepath.rsplit('.', 1)[0] + '.entity'
            export_filepath = make_relative(context, scene_filepath, export_filepath)
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
            f.write(struct.pack("<1f", light.data.energy))

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


class ExportEntity(Operator, ExportHelper):

    bl_idname = "export.graphene_scene"
    bl_label = "Export Graphene scene"

    filename_ext = ".scene"
    filter_glob = StringProperty(default="*.scene", options={'HIDDEN'})

    def execute(self, context):
        return write_scene(context, self.filepath)


def menu_func_export(self, context):
    self.layout.operator(ExportEntity.bl_idname, text="Graphene (.scene)")


def register():
    bpy.utils.register_class(ExportEntity)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportEntity)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
