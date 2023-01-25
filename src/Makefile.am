AUTOMAKE_OPTIONS = foreign

APP_SOURCE_FILES = main.cpp app.cpp window.cpp font.cpp render-imgui.cpp \
    render-sprites.cpp render-shapes.cpp render-surface.cpp glwrappers.cpp \
    atlas.cpp

bin_PROGRAMS = game game.html
lib_LTLIBRARIES = libimgui.la

libimgui_la_SOURCES = ../external/imgui/imgui.cpp ../external/imgui/imgui_draw.cpp \
    ../external/imgui/imgui_demo.cpp ../external/imgui/imgui_tables.cpp \
    ../external/imgui/imgui_widgets.cpp ../external/imgui/backends/imgui_impl_sdl.cpp \
    ../external/imgui/backends/imgui_impl_opengl3.cpp

libimgui_la_CFLAGS = -fPIC -static
libimgui_la_CXXFLAGS = -fPIC -static
libimgui_la_LDFLAGS = -version-info 1:89:0

game_LDADD = libimgui.la
game_html_LDADD = libimgui.la

game_SOURCES = $(APP_SOURCE_FILES)
game_html_SOURCES = $(APP_SOURCE_FILES)
