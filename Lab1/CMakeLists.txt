cmake_minimum_required(VERSION 3.20)

project(GP2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps)

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

add_executable(GP2 ${ENGINE_SOURCES} ${THIRD_PARTY_SOURCES})

target_include_directories(GP2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Load assets from the source tree, so the game runs from any working directory
# (CLion runs it from the build folder). Visual Studio builds fall back to ../Lab1/Resources/.
target_compile_definitions(GP2 PRIVATE GP2_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources/")

if (WIN32)
    # Prebuilt 32-bit libraries + headers in deps/ (DLLs in deps/bin/ and Debug/)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(WARNING "deps/lib only contains 32-bit (x86) libraries. Use an x86 toolchain "
                        "(in CLion: Settings > Build > Toolchains > Visual Studio, Architecture: x86).")
    endif()

    target_include_directories(GP2 PRIVATE ${DEPS_DIR}/include)
    target_link_directories(GP2 PRIVATE ${DEPS_DIR}/lib)
    target_link_libraries(GP2 PRIVATE SDL2main SDL2 glew32 opengl32 assimp-vc143-mt)
    target_compile_definitions(GP2 PRIVATE WINDOWS_IGNORE_PACKING_MISMATCH _CRT_SECURE_NO_WARNINGS ASSIMP_DLL)

    if (MSVC)
        target_link_options(GP2 PRIVATE /IGNORE:4099 /SUBSYSTEM:CONSOLE)
    endif()

    # Runtime DLLs next to the executable
    foreach (dll SDL2.dll glew32.dll)
        add_custom_command(TARGET GP2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/../Debug/${dll} $<TARGET_FILE_DIR:GP2>)
    endforeach()
    add_custom_command(TARGET GP2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DEPS_DIR}/bin/assimp-vc143-mt.dll $<TARGET_FILE_DIR:GP2>)
else()
    # System packages (Arch: pacman -S sdl2 glew, Debian/Ubuntu: apt install libsdl2-dev libglew-dev)
    find_package(SDL2 REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(OpenGL REQUIRED)

    # The code includes <SDL/SDL.h> (the layout in deps/include), but system SDL2
    # installs to include/SDL2. Expose it under that name, plus the bundled glm,
    # without putting deps/include (Windows SDL config, old GLEW) on the include path.
    find_path(SDL2_HEADER_DIR SDL.h PATHS ${SDL2_INCLUDE_DIRS} PATH_SUFFIXES SDL2 REQUIRED NO_DEFAULT_PATH)
    set(SHIM_DIR ${CMAKE_CURRENT_BINARY_DIR}/include_shim)
    file(MAKE_DIRECTORY ${SHIM_DIR})
    file(CREATE_LINK ${SDL2_HEADER_DIR} ${SHIM_DIR}/SDL SYMBOLIC)
    file(CREATE_LINK ${DEPS_DIR}/include/glm ${SHIM_DIR}/glm SYMBOLIC)

    target_include_directories(GP2 PRIVATE ${SHIM_DIR})
    target_link_libraries(GP2 PRIVATE SDL2::SDL2 GLEW::GLEW OpenGL::GL)

    # Assimp: use an installed copy if there is one, otherwise build it from source (OBJ importer only)
    find_package(assimp CONFIG QUIET)
    if (NOT assimp_FOUND)
        include(FetchContent)
        set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
        set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
        set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
        set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
        set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(assimp
            GIT_REPOSITORY https://github.com/assimp/assimp.git
            GIT_TAG v6.0.5
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(assimp)
    endif()
    target_link_libraries(GP2 PRIVATE assimp::assimp)
endif()
