CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1)

PROJECT(Game VERSION 0.1.0)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

CONFIGURE_FILE(src/config.h.in src/config.h)

INCLUDE_DIRECTORIES(external/imgui src ${SDL2_INCLUDE_DIRS})

ADD_LIBRARY(
    imgui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

ADD_LIBRARY(
    GameLib
    src/app.cpp
    src/window.cpp
    src/font.cpp
    src/render-imgui.cpp
    src/render-sprites.cpp
    src/render-shapes.cpp
    src/render-surface.cpp
    src/glwrappers.cpp
    src/atlas.cpp
)

ADD_EXECUTABLE(
    Game
    src/main.cpp
)

TARGET_LINK_LIBRARIES(
    Game
    PRIVATE
        imgui
        GameLib
        GL
        glfw
        ${SDL2_LIBRARIES}
)
