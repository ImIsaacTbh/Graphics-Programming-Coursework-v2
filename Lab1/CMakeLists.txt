cmake_minimum_required(VERSION 3.20)

project(Labs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(ENGINE_SOURCES
    main.cpp
    Core/Application.cpp
    Core/FileSystem.cpp
    Core/FrameTimer.cpp
    Core/Input.cpp
    Core/Window.cpp
    Editor/EditorUI.cpp
    Game/CameraController.cpp
    Game/DemoScene.cpp
    Renderer/Mesh.cpp
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
    Renderer/ShadowMaps.cpp
    Renderer/Texture.cpp
    Scene/Primitives.cpp
    Scene/Scene.cpp
)

set(THIRD_PARTY_SOURCES
    ThirdParty/imgui/imgui.cpp
    ThirdParty/imgui/imgui_demo.cpp
    ThirdParty/imgui/imgui_draw.cpp
    ThirdParty/imgui/imgui_impl_opengl3.cpp
    ThirdParty/imgui/imgui_impl_sdl2.cpp
    ThirdParty/imgui/imgui_tables.cpp
    ThirdParty/imgui/imgui_widgets.cpp
    ThirdParty/stb/STB.c
)

add_executable(Labs ${ENGINE_SOURCES} ${THIRD_PARTY_SOURCES})

# Include + library directories
target_include_directories(Labs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../deps/include
)

target_link_directories(Labs PRIVATE
    ${CMAKE_SOURCE_DIR}/../deps/lib
)

target_link_libraries(Labs PRIVATE
    SDL2
    SDL2main
    glew32
    opengl32
)

file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})

target_link_options(Labs PRIVATE /IGNORE:4099)

if (WIN32)
    target_link_options(Labs PRIVATE "/SUBSYSTEM:CONSOLE")
endif()