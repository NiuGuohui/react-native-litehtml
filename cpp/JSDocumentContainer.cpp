#include "Utils.h"
#include "JSDocumentContainer.h"
#include <android/log_macros.h>

namespace RNLitehtml {
    litehtml::uint_ptr
    JSDocumentContainer::create_font(const litehtml::font_description &descr, const litehtml::document *doc,
                                     litehtml::font_metrics *fm) {
        auto createFont = wrapper.getJSFunc("createFont");
        if (createFont) {
            auto jsFD = jsi::Object(*rt);
            jsFD.setProperty(*rt, "fontFamily", descr.family);
            jsFD.setProperty(*rt, "fontSize", descr.size);
            jsFD.setProperty(*rt, "fontWeight", descr.weight);
            jsFD.setProperty(*rt, "italic", descr.style == litehtml::font_style_italic);
            if (descr.decoration_line != litehtml::text_decoration_line_none) {
                auto decoration = jsi::Object(*rt);
                decoration.setProperty(*rt, "underline",
                                       !!(descr.decoration_line & litehtml::text_decoration_line_underline));
                decoration.setProperty(*rt, "overline",
                                       !!(descr.decoration_line & litehtml::text_decoration_line_overline));
                decoration.setProperty(*rt, "lineThrough",
                                       !!(descr.decoration_line & litehtml::text_decoration_line_line_through));
                decoration.setProperty(*rt, "color", formatColor(descr.decoration_color));
                decoration.setProperty(*rt, "style",
                                       descr.decoration_style ==
                                       litehtml::text_decoration_style::text_decoration_style_dashed
                                       ? "dashed" :
                                       descr.decoration_style ==
                                       litehtml::text_decoration_style::text_decoration_style_dotted
                                       ? "dotted" :
                                       descr.decoration_style ==
                                       litehtml::text_decoration_style::text_decoration_style_double
                                       ? "double" :
                                       descr.decoration_style ==
                                       litehtml::text_decoration_style::text_decoration_style_wavy
                                       ? "wavy" :
                                       descr.decoration_style ==
                                       litehtml::text_decoration_style::text_decoration_style_max
                                       ? "max" : "solid");
                jsFD.setProperty(*rt, "decoration", decoration);
            }
            if (!descr.emphasis_style.empty()) {
                auto emphasis = jsi::Object(*rt);
                emphasis.setProperty(*rt, "style", descr.emphasis_style);
                emphasis.setProperty(*rt, "color", formatColor(descr.emphasis_color));
                emphasis.setProperty(*rt, "over", !!(descr.emphasis_position &
                                                     litehtml::text_emphasis_position::text_emphasis_position_over));
                emphasis.setProperty(*rt, "under", !!(descr.emphasis_position &
                                                      litehtml::text_emphasis_position::text_emphasis_position_under));
                emphasis.setProperty(*rt, "left", !!(descr.emphasis_position &
                                                     litehtml::text_emphasis_position::text_emphasis_position_left));
                emphasis.setProperty(*rt, "right", !!(descr.emphasis_position &
                                                      litehtml::text_emphasis_position::text_emphasis_position_right));
                jsFD.setProperty(*rt, "emphasis", emphasis);
            }
            auto jsFm = createFont->call(*rt, jsFD);
            if (jsFm.isObject()) {
                auto result = jsFm.asObject(*rt);
                fm->font_size = static_cast<int>(result.getProperty(*rt, "fontSize").asNumber());
                fm->height = static_cast<int>(result.getProperty(*rt, "height").asNumber());
                fm->ascent = static_cast<int>(result.getProperty(*rt, "ascent").asNumber());
                fm->descent = static_cast<int>(result.getProperty(*rt, "descent").asNumber());
                fm->x_height = static_cast<int>(result.getProperty(*rt, "xHeight").asNumber());
                fm->ch_width = static_cast<int>(result.getProperty(*rt, "chWidth").asNumber());
                fm->super_shift = static_cast<int>(result.getProperty(*rt, "superShift").asNumber());
                fm->sub_shift = static_cast<int>(result.getProperty(*rt, "subShift").asNumber());
                return static_cast<int>(result.getProperty(*rt, "id").asNumber());
            }
        }
        return 0;
    }

    void JSDocumentContainer::delete_font(litehtml::uint_ptr hFont) {
        auto deleteFont = wrapper.getJSFunc("deleteFont");
        if (deleteFont) deleteFont->call(*rt, jsi::Value(static_cast<double>(hFont)));
    }

    const char *JSDocumentContainer::get_default_font_name() const {
        auto getDefaultFontFamily = wrapper.getJSFunc("getDefaultFontFamily");
        if (getDefaultFontFamily) {
            auto result = getDefaultFontFamily->call(*rt);
            if (result.isString()) {
                std::string temp = result.getString(*rt).utf8(*rt);
                char *resultStr = new char[temp.size() + 1];
                std::strcpy(resultStr, temp.c_str());
                return resultStr;
            }
        }
        return "sans-serif";
    }

    int JSDocumentContainer::text_width(const char *text, litehtml::uint_ptr hFont) {
        auto getTextWidth = wrapper.getJSFunc("getTextWidth");
        if (getTextWidth) {
            auto result = getTextWidth->call(*rt);
            if (result.isNumber()) return static_cast<int>(result.getNumber());
        }
        return 0;
    }

    int JSDocumentContainer::pt_to_px(int pt) const {
        auto ptToPx = wrapper.getJSFunc("ptToPx");
        if (ptToPx) {
            auto result = ptToPx->call(*rt, pt);
            if (result.isNumber()) return static_cast<int>(round(result.getNumber()));
        }
        // 1pt = 1.333px
        // At this stage, most browsers will simulate the DPI of web pages to 96, so 1pt is basically a constant here.
        return static_cast<int>(roundf(1.333f * (float) pt));
    }

    int JSDocumentContainer::get_default_font_size() const {
        auto getDefaultFontSize = wrapper.getJSFunc("getDefaultFontSize");
        if (getDefaultFontSize) {
            auto result = getDefaultFontSize->call(*rt);
            if (result.isNumber()) return static_cast<int>(result.getNumber());
        }
        return 16;
    }

    void JSDocumentContainer::load_image(const char *src, const char *baseurl, bool redraw_on_ready) {
        auto loadImage = wrapper.getJSFunc("loadImage");
        if (loadImage) loadImage->call(*rt, baseurl, redraw_on_ready);
    }

    void JSDocumentContainer::get_image_size(const char *src, const char *baseurl, litehtml::size &sz) {}

    void JSDocumentContainer::set_caption(const char *caption) {
        auto setTitle = wrapper.getJSFunc("setTitle");
        if (setTitle) setTitle->call(*rt, caption);
    }

    void JSDocumentContainer::set_base_url(const char *base_url) {
        auto setBaseUrl = wrapper.getJSFunc("setBaseUrl");
        if (setBaseUrl) setBaseUrl->call(*rt, base_url);
    }

    void JSDocumentContainer::on_anchor_click(const char *url, const litehtml::element::ptr &el) {
        auto onAnchorClick = wrapper.getJSFunc("onAnchorClick");
        if (onAnchorClick) onAnchorClick->call(*rt, url);
    }

    litehtml::string JSDocumentContainer::resolve_color(const litehtml::string &color) const {
        return litehtml::document_container::resolve_color(color);
    }

    void JSDocumentContainer::get_language(litehtml::string &language, litehtml::string &culture) const {
        auto getLanguage = wrapper.getJSFunc("getLanguage");
        if (getLanguage) getLanguage->call(*rt, language, culture);
    }

    bool JSDocumentContainer::on_element_click(const litehtml::element::ptr &el) {
        auto onElementClick = wrapper.getJSFunc("onElementClick");
        if (onElementClick) {
            auto elAttrs = el->dump_get_attrs();
            auto attrs = jsi::Object(*rt);
            for (const auto &[name, value]: elAttrs) {
                attrs.setProperty(*rt, name.c_str(), value);
            }
            auto result = onElementClick->call(*rt, attrs);
            return result.isBool() && result.asBool();
        }
        return false;
    }

    void JSDocumentContainer::set_cursor(const char *cursor) {
        auto setCursor = wrapper.getJSFunc("setCursor");
        if (setCursor) setCursor->call(*rt, cursor);
    }

    void JSDocumentContainer::transform_text(litehtml::string &text, litehtml::text_transform tt) {
        switch (tt) {
            case litehtml::text_transform::text_transform_capitalize: {
                auto a = litehtml::split_string(text, " ");
                text = "";
                for (auto c: a) {
                    std::transform(c.begin(), c.end(), c.begin(), ::toupper);
                    text.append(c + " ");
                }
                break;
            }
            case litehtml::text_transform::text_transform_uppercase:
                std::transform(text.begin(), text.end(), text.begin(), ::toupper);
                break;
            case litehtml::text_transform::text_transform_lowercase:
                std::transform(text.begin(), text.end(), text.begin(), ::tolower);
                break;
            case litehtml::text_transform::text_transform_none:
            default:
                break;
        }
    }

    void JSDocumentContainer::set_clip(const litehtml::position &pos, const litehtml::border_radiuses &bdr_radius) {
        auto setClip = wrapper.getJSFunc("setClip");
        if (setClip) {
            auto jsRadius = jsi::Array(*rt, 8);
            jsRadius.setValueAtIndex(*rt, 0, bdr_radius.top_left_x);
            jsRadius.setValueAtIndex(*rt, 1, bdr_radius.top_left_y);
            jsRadius.setValueAtIndex(*rt, 2, bdr_radius.top_right_x);
            jsRadius.setValueAtIndex(*rt, 3, bdr_radius.top_right_y);
            jsRadius.setValueAtIndex(*rt, 4, bdr_radius.bottom_right_x);
            jsRadius.setValueAtIndex(*rt, 5, bdr_radius.bottom_right_y);
            jsRadius.setValueAtIndex(*rt, 6, bdr_radius.bottom_left_x);
            jsRadius.setValueAtIndex(*rt, 7, bdr_radius.bottom_left_y);
            setClip->call(*rt, createRect(*rt, pos), jsRadius);
        }
    }

    void JSDocumentContainer::del_clip() {
        auto delClip = wrapper.getJSFunc("delClip");
        if (delClip) delClip->call(*rt);
    }

    void JSDocumentContainer::get_viewport(litehtml::position &viewport) const {
        auto getViewport = wrapper.getJSFunc("getViewport");
        if (getViewport) {
            auto result = getViewport->call(*rt);
            if (result.isObject()) {
                auto o = result.asObject(*rt);
                viewport.x = static_cast<int>(o.getProperty(*rt, "x").asNumber());
                viewport.y = static_cast<int>(o.getProperty(*rt, "y").asNumber());
                viewport.width = static_cast<int>(o.getProperty(*rt, "width").asNumber());
                viewport.height = static_cast<int>(o.getProperty(*rt, "height").asNumber());
            }
        }
    }

    void JSDocumentContainer::split_text(const char *text, const std::function<void(const char *)> &on_word,
                                         const std::function<void(const char *)> &on_space) {
        litehtml::document_container::split_text(text, on_word, on_space);
    }

    void JSDocumentContainer::draw_text(litehtml::uint_ptr hdc, const char *text, litehtml::uint_ptr hFont,
                                        litehtml::web_color color,
                                        const litehtml::position &pos) {}

    void JSDocumentContainer::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker &marker) {}

    void JSDocumentContainer::draw_image(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                         const std::string &url,
                                         const std::string &base_url) {}

    void JSDocumentContainer::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders &borders,
                                           const litehtml::position &draw_pos,
                                           bool root) {}

    void JSDocumentContainer::draw_solid_fill(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                              const litehtml::web_color &color) {}

    void JSDocumentContainer::draw_linear_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                                   const litehtml::background_layer::linear_gradient &gradient) {}

    void JSDocumentContainer::draw_radial_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                                   const litehtml::background_layer::radial_gradient &gradient) {}

    void JSDocumentContainer::draw_conic_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                                  const litehtml::background_layer::conic_gradient &gradient) {}


    void JSDocumentContainer::link(const std::shared_ptr<litehtml::document> &doc, const litehtml::element::ptr &el) {}

    void
    JSDocumentContainer::import_css(litehtml::string &text, const litehtml::string &url, litehtml::string &baseurl) {
    }

    void JSDocumentContainer::on_mouse_event(const litehtml::element::ptr &el, litehtml::mouse_event event) {
    }

    void JSDocumentContainer::get_media_features(litehtml::media_features &media) const {
    }

    litehtml::element::ptr
    JSDocumentContainer::create_element(const char *tag_name, const litehtml::string_map &attributes,
                                        const std::shared_ptr<litehtml::document> &doc) {
        return nullptr;
    }
}