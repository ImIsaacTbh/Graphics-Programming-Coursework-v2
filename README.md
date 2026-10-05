# Coursework for Games Programming 2

This project is going to be a 3d renderer built in OpenGL, SDL and GLEW with as few libraries as possible

Copyright Isaac Marrs 2025

## Building

**CLion / CMake:** open the repository root in CLion and run the `GP2` target. From a terminal:

```
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/Lab1/GP2
```

- **Linux:** needs SDL2 and GLEW from your package manager (Arch: `pacman -S sdl2 glew`; Debian/Ubuntu: `apt install libsdl2-dev libglew-dev`). glm comes from `deps/`.
- **Windows:** uses the prebuilt 32-bit libraries in `deps/`, so pick an **x86** Visual Studio toolchain. `SDL2.dll` and `glew32.dll` are copied next to the exe after each build.

CMake builds load assets from `Lab1/Resources/` by absolute path, so they run from any working directory.

**Visual Studio:** open `GP2.sln`, pick `Debug | Win32` and run.

## Engine layout

All engine code lives in `Lab1/`. Includes are written relative to that folder (e.g. `#include "Renderer/Shader.h"`).

| Folder | Responsibility |
| --- | --- |
| `Core/` | `Application` (owns every system and runs the main loop), `Window` (SDL window + GL context), `Input` (event polling, keyboard/mouse state), `FrameTimer` (delta time, FPS), `FileSystem` (resource discovery), `Paths` (resource directory). |
| `Renderer/` | GPU resources (`Shader`, `Texture`, `Mesh`), `Material`, `Light`, `ShadowMaps` (depth pass) and `Renderer` (shadow pass + lit pass for a `Scene`). |
| `Scene/` | World data with no rendering or UI logic: `Scene`, `GameObject`, `Transform`, `Camera`, `Primitives`. |
| `Game/` | Gameplay built on top of the engine: `DemoScene` (default objects and animation) and `CameraController` (mouse look + WASD). |
| `Editor/` | `EditorUI`: the Dear ImGui hierarchy, stats and inspector windows. |
| `ThirdParty/` | Vendored libraries: Dear ImGui, stb_image, OBJ loader. |
| `Resources/` | Models, textures and shaders, loaded at runtime from `../Lab1/Resources/`. |

Each frame `Application` polls input, builds the editor UI, updates the demo scene, renders the scene, draws the UI on top, and swaps buffers.

Press **Escape** to switch between controlling the camera and using the editor.
