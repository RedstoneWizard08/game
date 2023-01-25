#pragma once

#ifndef __FONT_CPP
#define __FONT_CPP

#include "font.hpp"
#include "common.hpp"

#include <SDL2/SDL.h>

#include <fstream>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "../external/stb/stb_truetype.h"

const int LOW_CHAR = 32;
const int HIGH_CHAR = 126 + 1;

struct FontCharacter {
    SDL_Rect region;
    int xadvance;
    int ybaseline;
};

struct FontImpl {
    SDL_Surface* surface;
    int baseline;
    int height;
    vector<unsigned char> rendered_font;
    vector<FontCharacter> mapping;
};

Font::Font(const char* filename, float ptsize, float xadvance_adjust)
    : self(new FontImpl) {
    vector<char> font_buffer;
    ifstream in(filename, ifstream::binary);

    in.seekg(0, ios_base::end);

    font_buffer.resize(in.tellg());

    in.seekg(0, ios_base::beg);
    in.read(font_buffer.data(), font_buffer.size());

    int width = 0, height = int(ceil(ptsize));

    self->height = int(ceil(height));

    stbtt_fontinfo font;

    stbtt_InitFont(&font, reinterpret_cast<unsigned char*>(font_buffer.data()),
                   0);

    for (char c = LOW_CHAR; c < HIGH_CHAR; c++) {
        int ix0, iy0, ix1, iy1;

        stbtt_GetCodepointBitmapBox(&font, c, 1, 1, &ix0, &iy0, &ix1, &iy1);

        width +=
            1 + int(ceil(ix1 * ptsize / 1000.0) - floor(ix0 * ptsize / 1000.0));
    }

    height += 5;

    vector<unsigned char> rendered_font_grayscale;
    rendered_font_grayscale.resize(width * height);

    constexpr int N = HIGH_CHAR - LOW_CHAR;
    stbtt_bakedchar chardata[N];

    int r = stbtt_BakeFontBitmap(
        reinterpret_cast<unsigned char*>(font_buffer.data()), 0, ptsize,
        rendered_font_grayscale.data(), width, height, LOW_CHAR, N, chardata);

    if (r < 0) {
        FAIL("BakeFontBitmap not enough space");
    }

    self->mapping.resize(HIGH_CHAR);
    self->baseline = 0;

    for (int c = LOW_CHAR; c < HIGH_CHAR; c++) {
        auto& M = self->mapping[c];
        float x = 0.0, y = 0.0;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(chardata, width, height, c - LOW_CHAR, &x, &y, &q,
                           true);

        M.region.x = int(floor(q.s0 * width));
        M.region.y = int(floor(q.t0 * height));
        M.region.w = int(ceil((q.s1 - q.s0) * width));
        M.region.h = int(ceil((q.t1 - q.t0) * height));
        M.xadvance = int(round(x + xadvance_adjust));
        M.ybaseline = int(-q.y0);
        self->baseline = max(self->baseline, M.ybaseline);
    }

    self->rendered_font.resize(width * height * 4);

    for (unsigned i = 0; i < rendered_font_grayscale.size(); i++) {
        self->rendered_font[i * 4] = 255;
        self->rendered_font[i * 4 + 1] = 255;
        self->rendered_font[i * 4 + 2] = 255;
        self->rendered_font[i * 4 + 3] = rendered_font_grayscale[i];
    }

    self->surface = SDL_CreateRGBSurfaceFrom(
        self->rendered_font.data(), width, height, 32, width * 4,

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
    );
}

Font::~Font() {}

void Font::Draw(SDL_Surface* surface, int x, int y, const char* text) const {
    SDL_Rect dest;
    dest.x = x;
    for (const char* s = text; *s != '\0'; s++) {
        FontCharacter loc = self->mapping[*s];
        dest.y = y - loc.ybaseline;
        SDL_BlitSurface(self->surface, &loc.region, surface, &dest);
        dest.x += loc.xadvance;
    }
}

int Font::Height() const { return self->height; }

int Font::Baseline() const { return self->baseline; }

int Font::Width(const char* text) const {
    int width = 0;
    for (const char* s = text; *s != '\0'; s++) {
        width += self->mapping[*s].xadvance;
    }
    return width;
}

#endif
