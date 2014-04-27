0        7       15       24       31
+--------+--------+--------+--------+  <-- Header
|           magic number            |
+--------+--------+--------+--------+
| major  | minor  | patch  |objects |
+--------+--------+--------+--------+  <-- Object material
|       diffuse texture path        |
+--------+--------+--------+--------+  <-- Object geometry
|          vertices number          |
+--------+--------+--------+--------+
|           faces number            |
+--------+--------+--------+--------+  <-- Object data
|      vertex coordinates data      |
+--------+--------+--------+--------+
|        vertex normals data        |
+--------+--------+--------+--------+
|        uv coordinates data        |
+--------+--------+--------+--------+
|             faces data            |
+--------+--------+--------+--------+

File structure:
 1. magic number           (4 bytes) - 47 50 48 4E bytes sequence ("GPHN")
 2. version major number   (1 bytes) - format major version
 3. version minor number   (1 bytes) - format minor version
 4. version patch number   (1 bytes) - format patch version
 5. objects                (1 bytes) - objects number
 6. diffuse texture path   (256 bytes) - texture path
 7. vertices number        (4 bytes) - number of vertices (single object)
 8. faces number           (4 bytes) - number of faces (single object)
 9. vertex coordinates     (variable length) - float tuples (x, y, z)
10. vertex normals         (variable length) - float tuples (x, y, z)
11. uv coordinates         (variable length) - float tuples (u, v)
12. faces                  (variable length) - integer tuples (v1, v3, v3)
