AC_INIT([game], [0.1], [jacob@nosadnile.net])
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects])

LT_INIT
AC_SUBST([LIBTOOL_DEPS])

if test -z $CC; then
    CC=gcc
fi

if test -z $CXX; then
    CXX=g++
fi

EMSCRIPTEN="NO"

if test "$(basename "$CC")" = "emcc" || test "$(basename "$CXX")" = "em++"; then
    EMSCRIPTEN="YES"
fi

if test $EMSCRIPTEN = "YES"; then
    SDL2_VERSION=2.0.10
else
    SDL2_VERSION=2.0.20
fi

AM_PATH_SDL2(
    $SDL2_VERSION,
    :,
    AC_MSG_ERROR([*** SDL2 version $SDL2_VERSION not found!])
)

COMMON_FLAGS="-std=c++11 -MMD -MP -isystem . -O2 -lGL -lglfw3 -lSDL2_image -static -g -ggdb -g3 -ggdb3"
WASM_FLAGS="-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s FULL_ES2=1 -s WASM=1"

IMGUI_FLAGS='-I$(top_srcdir)/external/imgui/ -I$(top_srcdir)/external/imgui/backends/'
STB_FLAGS='-I$(top_srcdir)/external/stb/ -I$(top_srcdir)/external/'

if test $EMSCRIPTEN = "YES"; then
    CFLAGS="$CFLAGS $COMMON_FLAGS $WASM_FLAGS $IMGUI_FLAGS $STB_FLAGS"
    CXXFLAGS="$CXXFLAGS $COMMON_FLAGS $WASM_FLAGS $IMGUI_FLAGS $STB_FLAGS"
else
    CFLAGS="$CFLAGS $COMMON_FLAGS $SDL_CFLAGS $IMGUI_FLAGS $STB_FLAGS"
    CXXFLAGS="$CXXFLAGS $COMMON_FLAGS $SDL_CFLAGS $IMGUI_FLAGS $STB_FLAGS"
    LIBS="$LIBS $COMMON_FLAGS $SDL_LIBS $IMGUI_FLAGS $STB_FLAGS"
fi

AC_PROG_CC
AC_PROG_CXX

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
