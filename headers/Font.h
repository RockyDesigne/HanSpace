//
// Created by HORIA on 11.05.2024.
//

#ifndef HANSPACE_FONT_H
#define HANSPACE_FONT_H

#include <cstdint>
#include "Textures.h"
#include "Buffers.h"
#include "glad.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// Can only load one font at a time for now

struct FontData {
    GLuint textureId = 0;
    const char* path = nullptr;
};

namespace MyFont {

    constexpr uint64_t bitmapWidth = 512;
    constexpr uint64_t bitmapHeight = 512;

    unsigned char pixels[512 * 512];
    stbtt_bakedchar chardata[200];
    unsigned char ttf_buffer[1<<20];

    void my_stbtt_initfont_from_memory(FontData& fontData, const unsigned char* buffer) {

        stbtt_fontinfo font;

        auto offset = stbtt_GetFontOffsetForIndex(buffer, 0);

        stbtt_InitFont(&font, buffer, offset);

        stbtt_BakeFontBitmap(buffer, 0, 64, pixels, 512, 512, 0, font.numGlyphs, chardata);

        glGenTextures(1, &fontData.textureId);
        glBindTexture(GL_TEXTURE_2D, fontData.textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

    void my_stbtt_initfont_from_file(FontData& fontData) {
        if (!fontData.path) {
            throw std::runtime_error("No file path provided!");
        }
        auto file = fopen(fontData.path, "rb");
        if (!file) {
            throw std::runtime_error("could not open font file!");
        }

        fread(ttf_buffer, 1, 1<<20, file);

        fclose(file);

        my_stbtt_initfont_from_memory(fontData, ttf_buffer);

    }

    void bindFont(FontData font) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.textureId);
    }

    void drawGlyph(float x, float y, char c) {

        stbtt_aligned_quad q = {};

        stbtt_GetBakedQuad(reinterpret_cast<const stbtt_bakedchar *>(chardata),
                           512,
                           512,  // same data as above
                           c,             // character to display
                           &x,
                           &y,   // pointers to current position in screen pixel space
                           &q,      // output: quad to draw
                           1);

        auto windowHeight = (float) Window::height; // replace with actual function to get window height

        // invert y coordinates
        q.y0 = windowHeight - q.y0;
        q.y1 = windowHeight - q.y1;

        Buffers::push_vert(q.x0, q.y0, 1.0f, 0.5f, 0.75f, q.s0, q.t0);
        Buffers::push_vert(q.x1, q.y0, 1.0f, 0.5f, 0.75f, q.s1, q.t0);
        Buffers::push_vert(q.x0, q.y1, 1.0f, 0.5f, 0.75f, q.s0, q.t1);
        Buffers::push_vert(q.x1, q.y1, 1.0f, 0.5f, 0.75, q.s1, q.t1);
    }
}

#endif //HANSPACE_FONT_H
