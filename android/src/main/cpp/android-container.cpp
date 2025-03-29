#include "android-container.h"
#include "android-util.h"

using namespace litehtml;

const char *DEFAULT_FONT_NAME = "_litehtml_font_";

android_container::android_container(JniNativeCall *jniNativeCall) {
    this->jniNativeCall = jniNativeCall;
}

android_container::~android_container() = default;

/**
 * This method is mainly used to create all font identifiers, so each font should return the ID identifier of the font (i.e. the hFont parameter that will be used in the later rendered functions)
 * Execute this function on all sizes and types of characters at the bottom layer, and then return the measurement information of the font.
 */
litehtml::uint_ptr
android_container::create_font(const font_description &descr, const document *doc, litehtml::font_metrics *fm) {
    auto result = new int[6]{0, 0, 0, 0, 0, 0};
    // Call jni to create font
    jniNativeCall->createFont(
            strcmp(descr.family.c_str(), DEFAULT_FONT_NAME) == 0 ? nullptr : descr.family.c_str(),
            descr,
            result
    );
    fm->height = result[1];
    fm->ascent = result[2];
    fm->descent = result[3];
    fm->x_height = result[4];
    fm->ch_width = result[5];
    fm->super_shift = descr.size / 3;
    fm->sub_shift = descr.size / 3;
    return litehtml::uint_ptr(result[0]);
}

/**
 * Delete the font created in the create_font function
 * @param hFont Font Identifier
 */
void android_container::delete_font(litehtml::uint_ptr hFont) {
    jniNativeCall->deleteFont(static_cast<int>(hFont));
}

/**
 * Returns the width of the specified text content
 * @param hFont Font Identifier
 */
int android_container::text_width(const char *text, litehtml::uint_ptr hFont) {
    return jniNativeCall->textWidth(text, static_cast<int>(hFont));
}

/**
 * Draw text
 * @param hdc
 * @param text text content
 * @param hFont Font Identifier
 * @param color Text color
 * @param pos Location
 */
void android_container::draw_text(litehtml::uint_ptr hdc, const char *text, litehtml::uint_ptr hFont,
                                  litehtml::web_color color, const litehtml::position &pos) {
    auto jCanvas = reinterpret_cast<jobject>(hdc);
    auto fontName = static_cast<int>(hFont);
    jniNativeCall->drawText(
            jCanvas,
            createJRectF(jniNativeCall->env, (float) pos.x, (float) pos.y, (float) pos.width, (float) pos.height),
            createJColorString(jniNativeCall->env, color),
            fontName,
            text
    );
}

/**
 * Text splitter, temporarily use default
 */
void android_container::split_text(const char *text, const std::function<void(const char *)> &on_word,
                                   const std::function<void(const char *)> &on_space) {
    litehtml::document_container::split_text(text, on_word, on_space);
}

int android_container::pt_to_px(int pt) const {
    // 1pt = 1.333px
    // At this stage, most browsers will simulate the DPI of web pages to 96, so 1pt is basically a constant here.
    return round_f(1.333f * (float) pt);
}

int android_container::get_default_font_size() const {
    return 16;
}

const char *android_container::get_default_font_name() const {
    return DEFAULT_FONT_NAME;
}

/**
 * Draw background color
 * @param hdc canvas jobject object
 * @param layer Background information. When drawing solid color background, origin_box, repeat, and attachment do not take effect. You only need to consider clip_box and border_radius.
 * @param color
 */
void
android_container::draw_solid_fill(litehtml::uint_ptr hdc, const background_layer &layer, const web_color &color) {
    auto jCanvas = reinterpret_cast<jobject>(hdc);
    auto jRect = createJRect(jniNativeCall->env,
                             layer.clip_box.x, layer.clip_box.y, layer.clip_box.width, layer.clip_box.height);
    auto jBorderRadius = createJFloatArray(jniNativeCall->env,
                                           {
                                                   (float) layer.border_radius.top_left_x,
                                                   (float) layer.border_radius.top_left_y,
                                                   (float) layer.border_radius.top_right_x,
                                                   (float) layer.border_radius.top_right_y,
                                                   (float) layer.border_radius.bottom_right_x,
                                                   (float) layer.border_radius.bottom_right_y,
                                                   (float) layer.border_radius.bottom_left_x,
                                                   (float) layer.border_radius.bottom_left_y
                                           }
    );

    jniNativeCall->drawBackground(jCanvas, jRect, jBorderRadius, createJColorString(jniNativeCall->env, color));
}

/**
 * Draw list markers
 */
void android_container::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker &marker) {
    auto jCanvas = reinterpret_cast<jobject>(hdc);
    int type;
    if (marker.marker_type == list_style_type::list_style_type_disc) type = 0;
    else if (marker.marker_type == list_style_type::list_style_type_square) type = 1;
    else if (marker.marker_type == list_style_type::list_style_type_circle) type = 2;
    else return;
    jniNativeCall->drawListMarker(
            jCanvas,
            createJRect(jniNativeCall->env, marker.pos.x, marker.pos.y, marker.pos.width, marker.pos.height),
            createJColorString(jniNativeCall->env, marker.color),
            type
    );
}

/**
 * Loading pictures
 */
void android_container::load_image(const char *src, const char *baseurl, bool redraw_on_ready) {
    jniNativeCall->loadImage(src);
}

/**
 * Get the size of the image
 */
void android_container::get_image_size(const char *src, const char *baseurl, litehtml::size &sz) {
    int size[2] = {0, 0};
    jniNativeCall->getImageSize(src, size);
    if (size[0] > 0 && size[1] > 0) {
        sz.width = size[0];
        sz.height = size[1];
    }
}

/**
 * Draw pictures
 */
void android_container::draw_image(litehtml::uint_ptr hdc, const background_layer &layer, const std::string &url,
                                   const std::string &base_url) {
    auto jCanvas = reinterpret_cast<jobject>(hdc);
    jniNativeCall->drawImage(
            jCanvas,
            url.c_str(),
            createJRect(jniNativeCall->env, layer.clip_box.x, layer.clip_box.y, layer.clip_box.width,
                        layer.clip_box.height),
            createJFloatArray(jniNativeCall->env,
                              {
                                      (float) layer.border_radius.top_left_x,
                                      (float) layer.border_radius.top_left_y,
                                      (float) layer.border_radius.top_right_x,
                                      (float) layer.border_radius.top_right_y,
                                      (float) layer.border_radius.bottom_right_x,
                                      (float) layer.border_radius.bottom_right_y,
                                      (float) layer.border_radius.bottom_left_x,
                                      (float) layer.border_radius.bottom_left_y
                              }
            )
    );
}

/**
 * Draw borders
 */
void android_container::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders &borders,
                                     const litehtml::position &draw_pos, bool root) {
    auto jCanvas = reinterpret_cast<jobject>(hdc);
    jniNativeCall->drawBorder(
            jCanvas,
            createJRectF(jniNativeCall->env, (float) draw_pos.x, (float) draw_pos.y, (float) draw_pos.width,
                         (float) draw_pos.height),
            createJFloatArray(jniNativeCall->env,
                              {
                                      (float) borders.radius.top_left_x,
                                      (float) borders.radius.top_left_y,
                                      (float) borders.radius.top_right_x,
                                      (float) borders.radius.top_right_y,
                                      (float) borders.radius.bottom_right_x,
                                      (float) borders.radius.bottom_right_y,
                                      (float) borders.radius.bottom_left_x,
                                      (float) borders.radius.bottom_left_y
                              }
            ),
            createJFloatArray(jniNativeCall->env, {
                    (float) borders.left.width, (float) borders.top.width,
                    (float) borders.right.width, (float) borders.bottom.width
            }),
            createJStringArray(jniNativeCall->env, {
                    jniNativeCall->env->NewStringUTF(getBorderStyle(borders.left.style)),
                    jniNativeCall->env->NewStringUTF(getBorderStyle(borders.top.style)),
                    jniNativeCall->env->NewStringUTF(getBorderStyle(borders.right.style)),
                    jniNativeCall->env->NewStringUTF(getBorderStyle(borders.bottom.style))
            }),
            createJStringArray(jniNativeCall->env, {
                    createJColorString(jniNativeCall->env, borders.left.color),
                    createJColorString(jniNativeCall->env, borders.top.color),
                    createJColorString(jniNativeCall->env, borders.right.color),
                    createJColorString(jniNativeCall->env, borders.bottom.color)
            })
    );
}


void android_container::transform_text(litehtml::string &text, litehtml::text_transform tt) {
    switch (tt) {
        case litehtml::text_transform::text_transform_capitalize: {
            auto a = split_string(text, " ");
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

void android_container::set_clip(const litehtml::position &pos, const litehtml::border_radiuses &radius) {
    jniNativeCall->setClip(
            createJRectF(jniNativeCall->env, (float) pos.x, (float) pos.y, (float) pos.width, (float) pos.height),
            createJFloatArray(jniNativeCall->env,
                              {
                                      (float) radius.top_left_x,
                                      (float) radius.top_left_y,
                                      (float) radius.top_right_x,
                                      (float) radius.top_right_y,
                                      (float) radius.bottom_right_x,
                                      (float) radius.bottom_right_y,
                                      (float) radius.bottom_left_x,
                                      (float) radius.bottom_left_y
                              }
            )
    );
}

void android_container::del_clip() {
    jniNativeCall->delClip();
}

/**
 * Support \@media query
 */
void android_container::get_media_features(litehtml::media_features &media) const {
    media.type = litehtml::media_type::media_type_screen;
    media.color = 8;
    media.monochrome = 0;
    media.color_index = 256;
    media.resolution = 96;
}

/**
 * Handle image click
 */
bool android_container::on_element_click(const litehtml::element::ptr &el) {
    if (el && string(el->get_tagName()) == "img" || string(el->get_tagName()) == "image") {
        auto sz = el->get_placement();
        jniNativeCall->callImageClick(el->get_attr("src", ""), sz.width, sz.height);
    }
}

/**
 * Handle anchor click
 */
void android_container::on_anchor_click(const char *url, const litehtml::element::ptr &el) {
    auto content = string();
    el->get_text(content);
    jniNativeCall->callAnchorClick(url, content.c_str());
}

void android_container::get_client_rect(litehtml::position &client) const {
//    client.x = client.y = 0;
//    client.width = 999;
//    client.height = 9999;
}

std::shared_ptr<litehtml::element>
android_container::create_element(const char *tag_name, const litehtml::string_map &attributes,
                                  const std::shared_ptr<litehtml::document> &doc) {
    return nullptr;
}

/**
 * Gradient color is not supported yet
 */
void android_container::draw_linear_gradient(litehtml::uint_ptr hdc, const background_layer &layer,
                                             const background_layer::linear_gradient &gradient) {
}


/**
 * Gradient color is not supported yet
 */
void android_container::draw_radial_gradient(litehtml::uint_ptr hdc, const background_layer &layer,
                                             const background_layer::radial_gradient &gradient) {
}


/**
 * Gradient color is not supported yet
 */
void android_container::draw_conic_gradient(litehtml::uint_ptr hdc, const background_layer &layer,
                                            const background_layer::conic_gradient &gradient) {
}

/**
 * Resolve name color. For example: background-color: window
 * @warning Not supported yet
 */
litehtml::string android_container::resolve_color(const litehtml::string &color) const {
    return litehtml::document_container::resolve_color(color);
}

/**
 * Support \:lang(en-US) query
 * @warning Not supported yet
 */
void android_container::get_language(litehtml::string &language, litehtml::string &culture) const {
    language = "en";
    culture = "";
}

/**
 * <title> Events after tag parsing
 * @warning Not supported yet
 */
void android_container::set_caption(const char *caption) {
}

/**
 * Processing <base> tags
 * @warning Not supported yet
 */
void android_container::set_base_url(const char *base_url) {
}

/**
 * Processing <link> tags
 * @warning Not supported yet
 */
void android_container::link(const std::shared_ptr<litehtml::document> &doc, const litehtml::element::ptr &el) {
}

/**
 * Set the cursor
 * @warning The mobile terminal does not support cursor
 */
void android_container::set_cursor(const char *cursor) {
}


/**
 * Import CSS
 * @warning Not supported yet
 */
void android_container::import_css(litehtml::string &text, const litehtml::string &url, litehtml::string &baseurl) {
}

/**
 * Handle mouse events
 */
void android_container::on_mouse_event(const litehtml::element::ptr &el, litehtml::mouse_event event) {
}
