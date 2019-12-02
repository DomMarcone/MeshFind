# MeshFind
A simple tool for extracting raw 3d mesh data from files

It works on a few assumptions about modern 3d meshes:
1. Data will be using IEEE754 32bit floating point values.
2. The data will be uncompressed.
3. It's either structured by a long array of face verticies (highly repetetive data), or it's indexed (accompanied with an index array).
4. If the data contains normals, they can be easily found (unit vectors) and used to determine the offset and stride
