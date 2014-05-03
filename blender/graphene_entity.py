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
    "version": (0, 0, 1),
    "blender": (2, 6, 0),
    "location": "File > Import-Export",
    "description": "Export entity for Graphene library",
    "category": "Import-Export"
}

import struct
import bmesh
import bpy
from bpy.types import Operator
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
        if obj.parent and obj.parent.dupli_type in {'VERTS', 'FACES'}:
            continue

        if obj.dupli_type != 'NONE':
            obj.dupli_list_create(context.scene)
            derived_objects = [(dupli.object, dupli.matrix) for dupli in obj.dupli_list]
        else:
            derived_objects = [(obj, obj.matrix_world)]

        for object_derived, matrix in derived_objects:
            try:
                mesh = object_derived.to_mesh(context.scene, True, 'PREVIEW')
            except RuntimeError:
                pass
            else:
                triangulate(mesh)
                mesh.transform(global_matrix * matrix)
                meshes.append(mesh)

        if obj.dupli_type != 'NONE':
            obj.dupli_list_clear()

    with open(filepath, "wb") as f:
        f.write(struct.pack("<4s", bytearray("GPHN", 'ASCII')))
        f.write(struct.pack("<4b", *(bl_info["version"] + (len(meshes),))))

        for mesh in meshes:
            loops = mesh.loops[:]
            loops_number = len(loops)

            vertices = [(0.0, 0.0, 0.0)] * loops_number
            normals = [(0.0, 0.0, 0.0)] * loops_number
            uvs = [(0.0, 0.0)] * loops_number

            polygons = mesh.polygons[:]
            faces = [range(p.loop_start, p.loop_start + p.loop_total) for p in polygons]

            vertex_attrs = mesh.vertices[:]
            materials = mesh.materials[:]  # TODO: Write more than one material

            try:
                uv_loops = mesh.uv_layers.active.data[:]
            except AttributeError:
                uv_loops = None

            for face_index in [i for face in faces for i in face]:
                vertex = vertex_attrs[loops[face_index].vertex_index]
                vertices[face_index] = vertex.co
                normals[face_index] = vertex.normal

                if uv_loops is not None:  # Also flip vertical coordinate
                    uv = uv_loops[face_index].uv
                    uvs[face_index] = (uv[0], 1.0 - uv[1])

            f.write(struct.pack("<2f", materials[0].ambient, materials[0].diffuse_intensity))
            f.write(struct.pack("<3f", *materials[0].diffuse_color[:]))
            f.write(struct.pack("<1f1i", materials[0].specular_intensity, materials[0].specular_hardness))
            f.write(struct.pack("<3f", *materials[0].specular_color[:]))

            try:
                diffuse_texture = materials[0].active_texture.image.filepath[2:257]
            except AttributeError:
                diffuse_texture = ''

            diffuse_texture += '\0' * (255 - len(diffuse_texture))
            f.write(struct.pack("<255s1b", bytearray(diffuse_texture, 'ASCII'), 0))

            f.write(struct.pack("<1i1i", len(vertices), len(faces)))
            for vertex in vertices:
                f.write(struct.pack("<3f", vertex[0], vertex[1], vertex[2]))
            for normal in normals:
                f.write(struct.pack("<3f", normal[0], normal[1], normal[2]))
            for uv in uvs:
                f.write(struct.pack("<2f", uv[0], uv[1]))
            for face in faces:
                f.write(struct.pack("<3i", face[0], face[1], face[2]))

            bpy.data.meshes.remove(mesh)

    return {'FINISHED'}


class ExportEntity(Operator, ExportHelper):

    bl_idname = "export.graphene_entity"
    bl_label = "Export Graphene entity"

    filename_ext = ".entity"
    filter_glob = StringProperty(default="*.entity", options={'HIDDEN'})

    axis_forward = EnumProperty(
        name="Forward", default='-Z',
        items=(
            ('X', "X Forward", ""),
            ('Y', "Y Forward", ""),
            ('Z', "Z Forward", ""),
            ('-X', "-X Forward", ""),
            ('-Y', "-Y Forward", ""),
            ('-Z', "-Z Forward", "")
        ))

    axis_up = EnumProperty(
        name="Up", default='Y',
        items=(
            ('X', "X Up", ""),
            ('Y', "Y Up", ""),
            ('Z', "Z Up", ""),
            ('-X', "-X Up", ""),
            ('-Y', "-Y Up", ""),
            ('-Z', "-Z Up", "")
        ))

    def execute(self, context):
        global_matrix = axis_conversion(to_forward=self.axis_forward, to_up=self.axis_up).to_4x4()
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
