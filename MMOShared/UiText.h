//
// Created by Keegan Beaulieu on 2024-07-21.
//

#ifndef UITEXT_H
#define UITEXT_H

#include <unordered_map>

#include "UiContainer.h"

#include <freetype2/freetype/freetype.h>

class UiText : public UiContainer
{
public:
    struct CharacterDisplay
    {
        Vector2i position;
        Vector2i size;
        Vector2i bearing;
        Texture *texture;
        int row;
    };

    std::string text;
    std::vector<CharacterDisplay> text_display;
    int descent;
    int ascent;

    static void init();
    static void cleanup();
    static UiText *create(UiContainer *parent);
    static void handle_update(UiContainer *container, void *data);
    static void handle_render(UiContainer *container, void *data);

private:
    struct Glyph
    {
        Texture *texture;
        Vector2i size;
        Vector2i bearing;
        int advance;
    };

    struct FontFace;

    struct GlyphSet
    {
        std::unordered_map<char, Glyph> glyphs;
        int size;
        FontFace *font_face;
        int users;
        int ascent;
        int descent;
    };

    struct FontFace
    {
        FT_Face ft_face;
        std::string name;
        std::unordered_map<int, GlyphSet> glyphs;
    };

    struct TextStyle
    {
        std::string font;
        int font_size;
        Colour colour;
    };

    struct Bounds
    {
        Vector2i size;
        int origin_offset;
    };

    GlyphSet *glyphs;

    FontFace *load_font_face(const TextStyle &text_style);
    void load_glyphs(const TextStyle &text_style);
    Bounds calculate_bounds();
    void verify_glyphs();

    static std::unordered_map<std::string, FontFace> font_faces;
    static FT_Library freetype;

    static void destroy_font_face(FontFace *font_face);
    static void cleanup_glyphs(GlyphSet *glyphs);
    static void destroy_glyphs(GlyphSet *glyphs);
    static TextStyle get_text_style(UiText *text);
    static void calculate_text_fit_size(UiContainer *container);
    static void handle_destroy(UiContainer *container, void *);
};

#endif //UITEXT_H
