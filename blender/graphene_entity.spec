0        7       15       24       31
+--------+--------+--------+--------+  <-- Header
|           magic number            |
+--------+--------+--------+--------+
| major  | minor  | patch  |objects |
+--------+--------+--------+--------+  <-- Object material
|         ambient intensity         |
+--------+--------+--------+--------+
|         diffuse intensity         |
+--------+--------+--------+--------+
|                                   |
|           diffuse color           |
|                                   |
+--------+--------+--------+--------+
|        specular intensity         |
+--------+--------+--------+--------+
|         specular hardness         |
+--------+--------+--------+--------+
|                                   |
|          specular color           |
|                                   |
+--------+--------+--------+--------+
~       diffuse texture path        ~
+--------+--------+--------+--------+  <-- Object geometry
|          vertices number          |
+--------+--------+--------+--------+
|           faces number            |
+--------+--------+--------+--------+  <-- Object data
~      vertex coordinates data      ~
+--------+--------+--------+--------+
~        vertex normals data        ~
+--------+--------+--------+--------+
~        uv coordinates data        ~
+--------+--------+--------+--------+
~             faces data            ~
+--------+--------+--------+--------+

File structure:
 1. magic number           (4 bytes) - 47 50 48 4E bytes sequence ("GPHN")
 2. version major number   (1 bytes) - format major version
 3. version minor number   (1 bytes) - format minor version
 4. version patch number   (1 bytes) - format patch version
 5. objects                (1 bytes) - objects number
 6. ambient intensity      (1 float) - amount of ambient light the mesh receives
 7. diffuse intensity      (1 float) - amount of diffuse light the mesh reflects
 8. diffuse color          (3 floats) - diffuse color in normalized rgb
 9. specular intensity     (1 float) - the intensity of mesh specular reflection
10. specular hardness      (1 int) - the hardness of mesh specular reflection
11. specular color         (3 floats) - specular color in normalized rgb
12. diffuse texture path   (256 bytes) - diffuse texture path
13. vertices number        (4 bytes) - number of vertices (single object)
14. faces number           (4 bytes) - number of faces (single object)
15. vertex coordinates     (variable length) - float tuples (x, y, z)
16. vertex normals         (variable length) - float tuples (x, y, z)
17. uv coordinates         (variable length) - float tuples (u, v)
18. faces                  (variable length) - integer tuples (v1, v3, v3)
