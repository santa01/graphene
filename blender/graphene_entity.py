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
    "name": "Graphene entity format",
    "author": "Pavlo Lavrenenko",
    "version": (0, 2, 0),
    "blender": (2, 80, 0),
    "location": "File > Import-Export",
    "description": "Export entity for Graphene engine",
    "category": "Import-Export"
}

import struct
import os
from collections import defaultdict
import bmesh
import bpy
from bpy.types import Operator
from bpy.props import StringProperty
from bpy_extras.io_utils import ExportHelper, axis_conversion
from bpy_extras.node_shader_utils import ShaderWrapper, rgba_to_rgb


class MaterialShaderWrapper(ShaderWrapper):

    def __init__(self, material=None):
        super(MaterialShaderWrapper, self).__init__(material, is_readonly=True, use_nodes=True)

    def __update_material(self, shader_node=None):
        if shader_node is None:
            self.ambient_intensity = 1.0
            self.diffuse_intensity = 1.0
            self.diffuse_color = (1.0, 0.0, 0.0)
            self.specular_intensity = 0.5
            self.specular_hardness = 50
            self.specular_color = (1.0, 1.0, 1.0)

            self.diffuse_texture = None
        else:
            base_color = shader_node.inputs["Base Color"]
            specular = shader_node.inputs["Specular"]
            roughness = shader_node.inputs["Roughness"]

            self.ambient_intensity = 1.0 # TODO
            self.diffuse_intensity = 1.0 # TODO
            self.diffuse_color = rgba_to_rgb(base_color.default_value)
            self.specular_intensity = 1.0 - roughness.default_value
            self.specular_hardness = 50 # TODO
            self.specular_color = rgba_to_rgb(specular.default_value)

            self.diffuse_texture = None
            if base_color.is_linked:
                from_node = base_color.links[0].from_node
                if from_node.bl_idname == 'ShaderNodeTexImage':
                    self.diffuse_texture = from_node.image

    def update(self):
        super(MaterialShaderWrapper, self).update()

        if self.material is None:
            self.__update_material()
            return

        shader_node = None
        for node in self.material.node_tree.nodes:
            if node.bl_idname == 'ShaderNodeOutputMaterial' and node.inputs[0].is_linked:
                from_node = node.inputs[0].links[0].from_node
                if from_node.bl_idname == 'ShaderNodeEeveeSpecular':
                    shader_node = from_node
                    break
            elif node.bl_idname == 'ShaderNodeEeveeSpecular' and node.outputs[0].is_linked:
                for output_link in node.outputs[0].links:
                    to_node = output_link.to_node
                    if to_node.bl_idname == 'ShaderNodeOutputMaterial':
                        shader_node = to_node
                        break

        assert shader_node != None
        self.__update_material(shader_node)


class ExportEntity(Operator, ExportHelper):

    bl_idname = "export.graphene_entity"
    bl_label = "Export Graphene entity"

    filename_ext = ".entity"
    filter_glob: StringProperty(default="*.entity", options={'HIDDEN'})

    @staticmethod
    def __triangulate(object_mesh):
        bm = bmesh.new()
        bm.from_mesh(object_mesh)
        bmesh.ops.triangulate(bm, faces=bm.faces)
        bm.to_mesh(object_mesh)
        bm.free()

    def __make_relative(self, target_filepath):
        # Per Blender Manual: Files & Data System - Blender File - Blend Files
        # When relative paths are supported, the File Browser provides a Relative Path checkbox,
        # when entering the path into a text field, use a double slash prefix (//) to make it so.
        if target_filepath[0:2] == "//":
            blend_filedir = os.path.dirname(self.context.blend_data.filepath)
            target_filepath = os.path.join(blend_filedir, target_filepath[2:])

        # Use forward slash separator (works well on Win32/Linux platforms)
        entity_filedir = os.path.dirname(self.filepath)
        return os.path.relpath(target_filepath, entity_filedir).replace('\\', '/')

    def __write_entity(self, global_matrix):
        default_material_shader = MaterialShaderWrapper()

        previous_mode = None
        if bpy.ops.object.mode_set.poll():
            previous_mode = self.context.object.mode
            bpy.ops.object.mode_set(mode='OBJECT')

        with open(self.filepath, "wb") as f:
            f.write(struct.pack("<4s", bytearray("GPNE", 'ASCII')))
            f.write(struct.pack("<3b1b", *bl_info["version"], 0))
            f.write(struct.pack("<1i", 0))  # Write meshes count later

            meshes_count = 0
            depsgraph = self.context.evaluated_depsgraph_get()

            for depsgraph_object in depsgraph.object_instances:
                # Evaluated objects have modifiers applied
                if depsgraph_object.is_instance:
                    evaluated_object = depsgraph_object.instance_object
                else:
                    evaluated_object = depsgraph_object.object

                object_mesh = evaluated_object.to_mesh()
                if object_mesh is None:
                    continue # Non-geometry object

                # Renderer requires 3 vertex faces.
                ExportEntity.__triangulate(object_mesh)

                # Modified global_matrix keeps X vector pointing right (see below), loop winding
                # becomes Counter Clock Wise with normals pointing the opposite direction.
                # flip_normals() changes the loop winding back to Clock Wise and fix normals direction.
                object_mesh.transform(global_matrix @ depsgraph_object.matrix_world)
                object_mesh.flip_normals()

                # Calculate split vertex normals, which preserve sharp edges.
                object_mesh.calc_normals_split()

                loops = object_mesh.loops[:]
                polygons = object_mesh.polygons[:]
                vertices = object_mesh.vertices[:]
                materials = object_mesh.materials[:]

                if object_mesh.uv_layers.active is not None:
                    uv_loop = object_mesh.uv_layers.active.data[:]
                else:
                    uv_loop = None

                material_polygons = defaultdict(list)
                for polygon in polygons:
                    material_polygons[polygon.material_index].append(polygon)

                meshes_count += len(material_polygons)
                for material_index, export_polygons in material_polygons.items():
                    material_loops = [polygon.loop_indices for polygon in export_polygons]
                    material_indices = [loop_index for loop in material_loops for loop_index in loop]

                    indices_count = len(material_indices)
                    export_vertices = [(0.0, 0.0, 0.0)] * indices_count
                    export_normals = [(0.0, 0.0, 0.0)] * indices_count
                    export_uvs = [(0.0, 0.0)] * indices_count

                    # Verticies are contiguous, generate [(0, 1, 2), (3, 4, 5), ...]
                    export_loops = [range(start, start + 3) for start in range(0, indices_count, 3)]

                    for mapped_index, loop_index in enumerate(material_indices):
                        vertex_index = loops[loop_index].vertex_index
                        vertex = vertices[vertex_index]

                        export_vertices[mapped_index] = vertex.co
                        export_normals[mapped_index] = vertex.normal

                        if uv_loop is not None:
                            export_uvs[mapped_index] = uv_loop[loop_index].uv

                    if materials and materials[material_index] is not None:
                        export_material = MaterialShaderWrapper(materials[material_index])
                    else:
                        export_material = default_material_shader

                    f.write(struct.pack("<2f", export_material.ambient_intensity, export_material.diffuse_intensity))
                    f.write(struct.pack("<3f", *export_material.diffuse_color))
                    f.write(struct.pack("<1f1i", export_material.specular_intensity, export_material.specular_hardness))
                    f.write(struct.pack("<3f", *export_material.specular_color))

                    export_filename = ""
                    if export_material.diffuse_texture is not None:
                        export_filename = self.__make_relative(export_material.diffuse_texture.filepath)

                    export_filename += "\0" * (255 - len(export_filename))
                    f.write(struct.pack("<255s1b", bytearray(export_filename, 'ASCII'), 0))

                    f.write(struct.pack("<1i1i", len(export_vertices), len(export_loops)))
                    for vertex in export_vertices:
                        f.write(struct.pack("<3f", vertex[0], vertex[1], vertex[2]))
                    for normal in export_normals:
                        f.write(struct.pack("<3f", normal[0], normal[1], normal[2]))
                    for uv in export_uvs:
                        f.write(struct.pack("<2f", uv[0], uv[1]))
                    for loop in export_loops:
                        f.write(struct.pack("<3i", loop[0], loop[1], loop[2]))

                f.seek(8)  # 4 bytes for "GPHN", 3 bytes for version, 1 byte unused
                f.write(struct.pack("<1i", meshes_count))

                evaluated_object.to_mesh_clear()

        if previous_mode is not None and bpy.ops.object.mode_set.poll():
            bpy.ops.object.mode_set(mode=previous_mode)

        return {'FINISHED'}

    def execute(self, context):
        self.context = context

        # Translate from Blender X(right), Y(forward), Z(up)
        # to Graphene X(right), Y(up), Z(forward) coordinate system
        global_matrix = axis_conversion(to_forward='Z', to_up='Y').to_4x4()

        # axis_conversion() issued matrix also flips X(right) to X(left),
        # flip back manually
        global_matrix[0][0] = 1.0

        return self.__write_entity(global_matrix)


def menu_func_export(self, context):
    self.layout.operator(ExportEntity.bl_idname, text="Graphene (.entity)")


def register():
    bpy.utils.register_class(ExportEntity)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportEntity)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
