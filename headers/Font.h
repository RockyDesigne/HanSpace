//
// Created by HORIA on 11.05.2024.
//

#ifndef HANSPACE_FONT_H
#define HANSPACE_FONT_H

#include <cstdint>
#include "Textures.h"
#include "Buffers.h"
#include "ttfBuffer.h"
#include "glad.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace MyFont {

    constexpr uint64_t bitmapWidth = 512;
    constexpr uint64_t bitmapHeight = 512;

//constexpr uint64_t fileSize = 50'000;
    unsigned char pixels[512 * 512];
    stbtt_bakedchar chardata[200];

    GLuint fontTexture;

    void my_stbtt_initfont() {

        stbtt_fontinfo font;

        auto offset = stbtt_GetFontOffsetForIndex(ttfBuffer, 0);

        stbtt_InitFont(&font, ttfBuffer, offset);

        stbtt_BakeFontBitmap(ttfBuffer, 0, 64, pixels, 512, 512, 0, font.numGlyphs, chardata);

        glGenTextures(1, &fontTexture);
        glBindTexture(GL_TEXTURE_2D, fontTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

    void bindFont() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
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
