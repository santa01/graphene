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
    "blender": (2, 7, 0),
    "location": "File > Import-Export",
    "description": "Export entity for Graphene engine",
    "category": "Import-Export"
}

import struct
import os
from collections import namedtuple
import bmesh
import bpy
from bpy.types import Operator, Material
from bpy.props import StringProperty, EnumProperty
from bpy_extras.io_utils import ExportHelper, axis_conversion


def triangulate(mesh):
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()


def write_entity(context, filepath, global_matrix):
    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')

    meshes = []

    for obj in context.scene.objects:
        object_parent = obj.parent
        if object_parent is not None and object_parent.dupli_type in {'VERTS', 'FACES'}:
            continue

        if obj.dupli_type != 'NONE':
            obj.dupli_list_create(context.scene)
            derived_objects = [(dupli.object, dupli.matrix) for dupli in obj.dupli_list]
        else:
            derived_objects = [(obj, obj.matrix_world)]

        for derived_object, local_matrix in derived_objects:
            mesh = derived_object.to_mesh(context.scene, True, 'PREVIEW')

            # Renderer requires 3 vertex faces
            triangulate(mesh)
            meshes.append(mesh)

            # Modified global_matrix keeps X vector pointing right (see below), face winding
            # becomes Counter Clock Wise with normals pointing the opposite direction.
            # flip_normals() changes the face winding back to Clock Wise and fix normals direction.
            mesh.transform(global_matrix * local_matrix)
            mesh.flip_normals()

            # Calculate split vertex normals, which preserve sharp edges.
            mesh.calc_normals_split()

        if obj.dupli_type != 'NONE':
            obj.dupli_list_clear()

    with open(filepath, "wb") as f:
        f.write(struct.pack("<4s", bytearray("GPHN", 'ASCII')))
        f.write(struct.pack("<4b", *(bl_info["version"] + (len(meshes),))))

        for mesh in meshes:
            loops = mesh.loops[:]
            polygons = mesh.polygons[:]
            vertices = mesh.vertices[:]
            materials = mesh.materials[:]

            if mesh.uv_layers.active is not None:
                uv_loop = mesh.uv_layers.active.data[:]
            else:
                uv_loop = None

            export_vertices = [(0.0, 0.0, 0.0)] * len(loops)
            export_normals = [(0.0, 0.0, 0.0)] * len(loops)
            export_uvs = [(0.0, 0.0)] * len(loops)
            export_faces = [p.loop_indices for p in polygons]

            for face_index in [index for face in export_faces for index in face]:
                vertex_index = loops[face_index].vertex_index
                vertex = vertices[vertex_index]

                export_vertices[face_index] = vertex.co
                export_normals[face_index] = vertex.normal
                if uv_loop is not None:
                    export_uvs[face_index] = uv_loop[face_index].uv

            # Materials list remains non empty after materials are removed
            if materials and materials[0] is not None:
                # TODO: Write more than one material
                material = materials[0]
            else:
                material = bpy.data.materials.new(name="default")
                material.ambient = 1.0
                material.diffuse_intensity = 1.0
                material.diffuse_color = (1.0, 0.0, 0.0)
                material.specular_intensity = 0.5
                material.specular_hardness = 50
                material.specular_color = (1.0, 1.0, 1.0)

            f.write(struct.pack("<2f", material.ambient, material.diffuse_intensity))
            f.write(struct.pack("<3f", *material.diffuse_color))
            f.write(struct.pack("<1f1i", material.specular_intensity, material.specular_hardness))
            f.write(struct.pack("<3f", *material.specular_color))

            diffuse_texture = material.active_texture
            if diffuse_texture is not None and diffuse_texture.type == 'IMAGE':
                # Per Blender Manual: Files & Data System - Blender File - Blend Files
                # When relative paths are supported, the File Browser provides a Relative Path checkbox,
                # when entering the path into a text field, use a double slash prefix (//) to make it so.
                texture_filepath = diffuse_texture.image.filepath
                if texture_filepath[0:2] == "//":
                    blend_filedir = os.path.dirname(context.blend_data.filepath)
                    texture_filepath = os.path.join(blend_filedir, texture_filepath[2:])

                export_filedir = os.path.dirname(filepath)
                export_filename = os.path.relpath(texture_filepath, export_filedir)
            else:
                export_filename = ""

            export_filename += "\0" * (255 - len(export_filename))
            f.write(struct.pack("<255s1b", bytearray(export_filename, 'ASCII'), 0))

            f.write(struct.pack("<1i1i", len(export_vertices), len(export_faces)))
            for vertex in export_vertices:
                f.write(struct.pack("<3f", vertex[0], vertex[1], vertex[2]))
            for normal in export_normals:
                f.write(struct.pack("<3f", normal[0], normal[1], normal[2]))
            for uv in export_uvs:
                f.write(struct.pack("<2f", uv[0], uv[1]))
            for face in export_faces:
                f.write(struct.pack("<3i", face[0], face[1], face[2]))

            bpy.data.meshes.remove(mesh)

    return {'FINISHED'}


class ExportEntity(Operator, ExportHelper):

    bl_idname = "export.graphene_entity"
    bl_label = "Export Graphene entity"

    filename_ext = ".entity"
    filter_glob = StringProperty(default="*.entity", options={'HIDDEN'})

    def execute(self, context):
        # Translate from Blender X(right), Y(forward), Z(up)
        # to Graphene X(right), Y(up), Z(forward) coordinate system
        global_matrix = axis_conversion(to_forward='Z', to_up='Y').to_4x4()

        # axis_conversion() issued matrix also flips X(right) to X(left),
        # flip back manually
        global_matrix[0][0] = 1.0

        return write_entity(context, self.filepath, global_matrix)


def menu_func_export(self, context):
    self.layout.operator(ExportEntity.bl_idname, text="Graphene (.entity)")


def register():
    bpy.utils.register_class(ExportEntity)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportEntity)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
