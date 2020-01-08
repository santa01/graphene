Scene definition:

0        7       15       24       31
+--------+--------+--------+--------+  <-- Header
|           magic number            |
+--------+--------+--------+--------+
| major  | minor  | patch  | unused |
+--------+--------+--------+--------+
~          scene name [256]         ~  <-- NULL terminated
+--------+--------+--------+--------+
|                                   |
|           ambient color           |
|                                   |
+--------+--------+--------+--------+
|           ambient energy          |
+--------+--------+--------+--------+
|                                   |
|       player rotation angles      |
|                                   |
+--------+--------+--------+--------+
|                                   |
|        player scene position      |
|                                   |
+--------+--------+--------+--------+
~         skybox texture [256]      ~  <-- NULL terminated
+--------+--------+--------+--------+
|              entities             |
+--------+--------+--------+--------+
|               lights              |
+--------+--------+--------+--------+
~         entity definition         ~
+--------+--------+--------+--------+
~               . . .               ~
+--------+--------+--------+--------+
~          light definition         ~
+--------+--------+--------+--------+
~               . . .               ~
+--------+--------+--------+--------+


Entity definition:

0        7       15       24       31
+--------+--------+--------+--------+
~         entity name [256]         ~  <-- NULL terminated
+--------+--------+--------+--------+
~       entity file path [256]      ~  <-- NULL terminated
+--------+--------+--------+--------+
|                                   |
|          scaling factors          |
|                                   |
+--------+--------+--------+--------+
|                                   |
|          rotation angles          |
|                                   |
+--------+--------+--------+--------+
|                                   |
|           scene position          |
|                                   |
+--------+--------+--------+--------+


Light definition:

0        7       15       24       31
+--------+--------+--------+--------+
~          light name [256]         ~  <-- NULL terminated
+--------+--------+--------+--------+
|                type               |
+--------+--------+--------+--------+
|               energy              |
+--------+--------+--------+--------+
|          falloff distance         |
+--------+--------+--------+--------+
|             spot angle            |
+--------+--------+--------+--------+
|             spot blend            |
+--------+--------+--------+--------+
|                                   |
|            light color            |
|                                   |
+--------+--------+--------+--------+
|                                   |
|          rotation angles          |
|                                   |
+--------+--------+--------+--------+
|                                   |
|           scene position          |
|                                   |
+--------+--------+--------+--------+


File structure:
 1. magic number           (4 bytes) - 47 50 4E 57 bytes sequence ("GPNW")
 2. version major number   (1 bytes) - format major version
 3. version minor number   (1 bytes) - format minor version
 4. version patch number   (1 bytes) - format patch version
 5. scene name             (256 bytes) - scene name
 6. ambient color          (3 floats) - scene ambient light color in normalized rgb
 7. ambient energy         (1 float) - amount of ambient light projected onto the scene
 8. player rotation angles (3 floats) - player rotation angles
 9. player scene position  (3 floats) - player spawn position
10. skybox texture         (256 bytes) - skybox texture path without actual filename
11. entities               (1 int) - number of exported entities
12. lights                 (1 int) - number of exported lights
13. entity name            (256 bytes) - entity name
14. entity file path       (256 bytes) - entity source file
15. scaling factors        (3 floats) - entity scaling factors
16. rotation angles        (3 floats) - entity rotation angles
17. scene position         (3 floats) - entity world position
18. light name             (256 bytes) - light name
19. type                   (1 int) - light type
20. energy                 (1 float) - amount of light projected onto the scene
21. falloff distance       (1 float) - light falloff distance
22. spot angle             (1 float) - spot light cone angle
23. spot blend             (1 float) - spot light cone border blend size
24. light color            (3 floats) - light color in normalized rgb
25. rotation angles        (3 floats) - light rotation angles
26. scene position         (3 floats) - light world position
