# Coursework for Games Programming 2

This project is going to be a 3d renderer built in OpenGL, SDL and GLEW with as few libraries as possible

Copyright Isaac Marrs 2025

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
