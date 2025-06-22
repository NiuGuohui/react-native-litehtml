#include "Utils.h"
#include "JSDocumentContainer.h"
#include <android/log_macros.h>

namespace RNLitehtml {
    litehtml::uint_ptr
    JSDocumentContainer::create_font(const litehtml::font_description &descr, const litehtml::document *doc,
                                     litehtml::font_metrics *fm) {
        auto jsCreateFont = jsLayer.getProperty(*rt, "createFont").asObject(*rt);
        if (jsCreateFont.isFunction(*rt)) {
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
            auto jsFm = jsCreateFont.asFunction(*rt).call(*rt, jsFD);
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
    }

    const char *JSDocumentContainer::get_default_font_name() const {
        ALOGE("Niu get_default_font_name");
        return "m";
    }

    int JSDocumentContainer::text_width(const char *text, litehtml::uint_ptr hFont) {
        return 0;
    }

    void JSDocumentContainer::draw_text(litehtml::uint_ptr hdc, const char *text, litehtml::uint_ptr hFont,
                                        litehtml::web_color color,
                                        const litehtml::position &pos) {}

    int JSDocumentContainer::pt_to_px(int pt) const {
        ALOGE("Niu pt_to_px");
        // 1pt = 1.333px
        // At this stage, most browsers will simulate the DPI of web pages to 96, so 1pt is basically a constant here.
        return static_cast<int>(roundf(1.333f * (float) pt));
    }

    int JSDocumentContainer::get_default_font_size() const {
        ALOGE("Niu pt_to_px");
        return 16;
    }

    void JSDocumentContainer::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker &marker) {}

    void JSDocumentContainer::load_image(const char *src, const char *baseurl, bool redraw_on_ready) {}

    void JSDocumentContainer::get_image_size(const char *src, const char *baseurl, litehtml::size &sz) {}

    void JSDocumentContainer::draw_image(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                         const std::string &url,
                                         const std::string &base_url) {}

    void JSDocumentContainer::draw_solid_fill(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                              const litehtml::web_color &color) {}

    void JSDocumentContainer::draw_linear_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                                   const litehtml::background_layer::linear_gradient &gradient) {}

    void JSDocumentContainer::draw_radial_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                                   const litehtml::background_layer::radial_gradient &gradient) {}

    void JSDocumentContainer::draw_conic_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer &layer,
                                                  const litehtml::background_layer::conic_gradient &gradient) {}

    void JSDocumentContainer::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders &borders,
                                           const litehtml::position &draw_pos,
                                           bool root) {}

    void JSDocumentContainer::set_caption(const char *caption) {}

    void JSDocumentContainer::set_base_url(const char *base_url) {}

    void JSDocumentContainer::link(const std::shared_ptr<litehtml::document> &doc, const litehtml::element::ptr &el) {}

    void JSDocumentContainer::on_anchor_click(const char *url, const litehtml::element::ptr &el) {}

    bool JSDocumentContainer::on_element_click(const litehtml::element::ptr &el) {
        return false;
    }

    void JSDocumentContainer::set_cursor(const char *cursor) {}

    void JSDocumentContainer::transform_text(litehtml::string &text, litehtml::text_transform tt) {}

    void
    JSDocumentContainer::import_css(litehtml::string &text, const litehtml::string &url, litehtml::string &baseurl) {
    }

    void JSDocumentContainer::set_clip(const litehtml::position &pos, const litehtml::border_radiuses &bdr_radius) {}

    void JSDocumentContainer::del_clip() {}

    void JSDocumentContainer::get_viewport(litehtml::position &viewport) const {}

    litehtml::element::ptr
    JSDocumentContainer::create_element(const char *tag_name, const litehtml::string_map &attributes,
                                        const std::shared_ptr<litehtml::document> &doc) {
        return 0;
    }

    void JSDocumentContainer::get_media_features(litehtml::media_features &media) const {

    }

    void JSDocumentContainer::get_language(litehtml::string &language, litehtml::string &culture) const {}

    litehtml::string JSDocumentContainer::resolve_color(const litehtml::string &color) const {
        return litehtml::document_container::resolve_color(color);
    }

    void JSDocumentContainer::split_text(const char *text, const std::function<void(const char *)> &on_word,
                                         const std::function<void(const char *)> &on_space) {}

    void JSDocumentContainer::on_mouse_event(const litehtml::element::ptr &el, litehtml::mouse_event event) {}
}