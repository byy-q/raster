# Raster

A small C++ software rasterizer that projects 3D coordinates onto a pixel canvas and writes the result as a 24-bit BMP image.

## Features

- BMP canvas creation and per-pixel color writes
- 3D perspective projection using a configurable camera
- Line and triangle rasterization, including interpolated color intensity
- Triangle and cube drawing helpers
- A cube mesh definition with eight vertices and twelve triangle indices
- 4x4 homogeneous transforms for cube instance scaling, translation, and rotation
- Camera movement and rotation apply inverse transforms to all registered instances

## Repository layout

```text
src/
  include/
    camera.hpp        Coordinates, camera
    canvas.hpp        BMP canvas and pixel operations
    color.hpp         RGB color type
    matrix.hpp        4x4 homogeneous transformations
    tool.hpp          Rasterization and projection declarations
    world_objects.hpp Triangle, cube, and cube mesh definitions
  tool.cpp            Rasterization and projection implementations
```

## Requirements

- A C++17 compiler, such as GCC or MinGW-w64

## Build

Build the rasterizer as a static library with CMake:

```powershell
cmake -G "MinGW Makefiles" -S . -B object/cmake
cmake --build object/cmake
ctest --test-dir object/cmake --output-on-failure
```

The resulting `raster` library can be linked into a program that uses the headers under `src/include`. The repository currently has no checked-in program entry point.

To compile the implementation to an object file directly instead:

```powershell
New-Item -ItemType Directory -Force object | Out-Null
g++ -std=c++17 -I src/include -Wall -Wextra -c src/tool.cpp -o object/tool.o
```

To build a runnable program, provide a `main` source file that uses the headers under `src/include`, then link it with `src/tool.cpp`. For example, if you create `src/test/main.cpp` locally:

```powershell
g++ -std=c++17 -Wall -Wextra src/test/main.cpp src/tool.cpp -o main.exe
```

The `main.cpp` path is ignored by Git in this repository, so a local entry point will not be included in commits unless the ignore rule is changed or the file is explicitly force-added.

## Output

Create the output directory before running a program that writes to it. The canvas constructor opens the specified path and truncates an existing file:

```cpp
canvas image(800, 600, "images/output.bmp", {255, 255, 255});
```

The `images/` directory is ignored by Git. The canvas writes a 24-bit BMP file.

## Notes

- `Project` rejects a vertex outside the camera's view. The current triangle and cube helpers skip drawing the entire shape if any of its vertices is rejected; clipping is not implemented.
- `model` and `instance` in `world_objects.hpp` are placeholders. The `cube_model` currently stores mesh vertices and triangle indices, but is not yet connected to a generic model-instance renderer.
- Build outputs, generated images, and `src/test/main.cpp` are excluded by `.gitignore`.
