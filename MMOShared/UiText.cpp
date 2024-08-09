//
// Created by Keegan Beaulieu on 2024-07-21.
//

#include "UiText.h"

#include <Material.h>
#include <UiStyle.h>
#include <Window.h>
#include <GL/glew.h>
#include <__format/format_functions.h>

std::unordered_map<std::string, UiText::FontFace> UiText::font_faces;
FT_Library UiText::freetype;

UiText::TextStyle UiText::get_text_style(UiText *text)
{
    UiStyle style = UiStyle::get_style<UiText>(text->style_tags);

    TextStyle text_style = {
        .font = "Times.ttc",
        .font_size = 40,
        .colour = Colour(0.0, 0.0, 0.0, 1.0)
    };

    try { text_style.font = style.get<std::string>("font"); } catch (...) { }
    try { text_style.font_size = style.get<int>("font-size"); } catch (...) { }
    try { text_style.colour = style.get<Colour>("text-colour"); } catch (...) { }

    return text_style;
}

void UiText::handle_update(UiContainer *container, void *data)
{
    UiText *ui_text = (UiText *)container;
    ui_text->verify_glyphs();

    ui_text->text_display.clear();

    Vector2i cursor;
    Bounds bounds = ui_text->calculate_bounds();

    switch (ui_text->alignment) {
        case UI_TOP_LEFT:
            cursor = ui_text->calculated_position;
            cursor.y += bounds.origin_offset;
            break;
        case UI_CENTER_LEFT:
            cursor.x = ui_text->calculated_position.x;
            cursor.y = ui_text->calculated_position.y + ui_text->calculated_size.y/2 - bounds.size.y/2 + bounds.origin_offset;
            break;
        case UI_CENTER:
            cursor.x = ui_text->calculated_position.x + ui_text->calculated_size.x/2 - bounds.size.x/2;
            cursor.y = ui_text->calculated_position.y + ui_text->calculated_size.y/2 - bounds.size.y/2 + bounds.origin_offset;
            break;
        default:
            throw std::runtime_error("Not handled.");
    }

    for (char c : ui_text->text) {
        Glyph &glyph = ui_text->glyphs->glyphs[c];

        ui_text->text_display.push_back({
            .texture = glyph.texture,
            .position = cursor + glyph.bearing,
            .size = glyph.size,
            .bearing = glyph.bearing
        });

        cursor.x += glyph.advance;
    }
}

void UiText::handle_render(UiContainer *container, void *data)
{
    UiText *ui_text = (UiText *)container;
    TextStyle text_style = get_text_style(ui_text);

    for (CharacterDisplay c : ui_text->text_display) {
        if (c.texture) {
            Material::defaults.ui_text->set("pixel_scale", PIXEL_SCALE);
            Material::defaults.ui_text->set("colour", text_style.colour);
            Material::defaults.ui_text->set("pos", c.position);
            Material::defaults.ui_text->set("depth", (float)ui_text->calculated_depth + 1);
            Material::defaults.ui_text->set("max_depth", (float)MAX_DEPTH);
            Material::defaults.ui_text->set("size", c.size);
            Material::defaults.ui_text->set("window_size", Window::true_size);
            Material::defaults.ui_text->set("use_texture", (int)(c.texture != nullptr));
            Material::defaults.ui_text->texture = c.texture;
            Material::defaults.ui_text->bind();

            Mesh::defaults.ui_quad->draw();
        }
    }
}

void UiText::init()
{
    FT_Error error = FT_Init_FreeType(&freetype);
    if (error)
        throw std::runtime_error("Failed to initialize freetype.");
}

void UiText::cleanup()
{
    for (auto ft_pair : font_faces) {
        for (auto glyph_pair : ft_pair.second.glyphs) {
            cleanup_glyphs(&glyph_pair.second);
        }

        FT_Done_Face(ft_pair.second.ft_face);
    }

    FT_Done_FreeType(freetype);
}

UiText *UiText::create(UiContainer *parent)
{
    UiText *ui_text = alloc<UiText>(parent);
    ui_text->on_update.subscribe(&handle_update, nullptr);
    ui_text->on_render.subscribe(&handle_render, nullptr);
    ui_text->on_destroy.subscribe(&handle_destroy, nullptr);
    ui_text->text = "";

    ui_text->calculate_fit_size_override = calculate_text_fit_size;

    return ui_text;
}

UiText::FontFace *UiText::load_font_face(const TextStyle &text_style)
{
    std::string path = "/System/Library/Fonts/" + text_style.font;

    FT_Face ft_face;
    FT_Error error = FT_New_Face(freetype, path.c_str(), 0, &ft_face);
    if (error)
        throw std::runtime_error("Failed to load font face: " + std::to_string(error) + ".");

    font_faces[text_style.font] = {
        .ft_face = ft_face,
        .name = text_style.font
    };

    return &font_faces[text_style.font];
}

void UiText::load_glyphs(const TextStyle &text_style)
{
    FontFace *font_face = nullptr;

    try {
        font_face = &font_faces.at(text_style.font);
        glyphs = &font_face->glyphs.at(text_style.font_size);

        ++glyphs->users;
    } catch (...) {
        // ReSharper disable once CppDFAUnreachableCode
        if (!font_face)
            font_face = load_font_face(text_style);

        font_face->glyphs[text_style.font_size] = {
            .font_face = font_face,
            .size = text_style.font_size,
            .users = 1
        };

        glyphs = &font_face->glyphs[text_style.font_size];

        FT_Set_Pixel_Sizes(font_face->ft_face, 0, text_style.font_size);

        for (unsigned char c = 0; c < 128; ++c) {
            if (FT_Load_Char(font_face->ft_face, c, FT_LOAD_RENDER))
                continue;

            Vector2i size_vec = Vector2i(
                font_face->ft_face->glyph->bitmap.width,
                font_face->ft_face->glyph->bitmap.rows
            );

            Texture *texture = nullptr;

            if (size_vec.x != 0 && size_vec.y != 0) {
                texture = Texture::create({
                    .filtering = Texture::TEX_LINEAR,
                    .format = Texture::TEX_RED,
                    .width = size_vec.x,
                    .height = size_vec.y,
                    .pixels = font_face->ft_face->glyph->bitmap.buffer,
                    .invert = true,
                    .stride = 1,
                    .name = "Glyph "+std::to_string((int)c)
                });
            }

            glyphs->glyphs[c] = {
                .texture = texture,
                .size = size_vec,
                .bearing = Vector2i(
                    font_face->ft_face->glyph->bitmap_left,
                    -font_face->ft_face->glyph->bitmap_top
                ),
                .advance = (int)font_face->ft_face->glyph->advance.x/64
            };
        }

        glyphs->descent = -font_face->ft_face->size->metrics.descender/64;
        glyphs->ascent = -font_face->ft_face->size->metrics.ascender/64;
    }

    ascent = glyphs->ascent;
    descent = glyphs->descent;
}

void UiText::calculate_text_fit_size(UiContainer *container)
{
    UiText *text = (UiText *)container;
    text->verify_glyphs();

    Bounds bounds = text->calculate_bounds();
    if (text->size.x == UI_SIZE_FIT)
        text->calculated_size.x = bounds.size.x;
    if (text->size.y == UI_SIZE_FIT)
        text->calculated_size.y = bounds.size.y;
}

void UiText::handle_destroy(UiContainer *container, void *)
{
    UiText *text = (UiText *)container;
    TextStyle text_style = get_text_style(text);

    if (!text->glyphs)
        return;

    --text->glyphs->users;

    if (text->glyphs->users == 0) {
        FontFace *face = text->glyphs->font_face;
        text->glyphs->font_face->glyphs.erase(text_style.font_size);

        if (face->glyphs.empty() && text_style.font != face->name)
            font_faces.erase(text_style.font);
    }
}

UiText::Bounds UiText::calculate_bounds()
{
    Bounds bounds;

    for (int i = 0; i < text.length(); ++i) {
        Glyph &glyph = glyphs->glyphs[text[i]];
        if (i < text.length() - 1)
            bounds.size.x += glyph.advance;
        else
            bounds.size.x += glyph.size.x + glyph.bearing.x;
    }

    bounds.size.y = -ascent + descent;
    bounds.origin_offset = -ascent;

    return bounds;
}

void UiText::verify_glyphs()
{
    TextStyle text_style = get_text_style(this);

    if (!glyphs) {
        load_glyphs(text_style);
    } else if (glyphs && (text_style.font != glyphs->font_face->name || text_style.font_size != glyphs->size)) {
        --glyphs->users;

        if (glyphs->users == 0) {
            glyphs->font_face->glyphs.erase(text_style.font_size);

            if (glyphs->font_face->glyphs.size() == 0 && text_style.font != glyphs->font_face->name)
                font_faces.erase(text_style.font);
        }

        load_glyphs(text_style);
    }
}

void UiText::destroy_font_face(FontFace *font_face)
{
    FT_Done_Face(font_face->ft_face);
    font_faces.erase(font_face->name);
}

void UiText::cleanup_glyphs(GlyphSet *glyphs)
{
    for (auto pair : glyphs->glyphs) {
        Glyph *glyph = &pair.second;
        if (glyph->texture)
            Texture::destroy(glyph->texture);
    }
}

void UiText::destroy_glyphs(GlyphSet *glyphs)
{
    cleanup_glyphs(glyphs);
    glyphs->font_face->glyphs.erase(glyphs->size);
}
