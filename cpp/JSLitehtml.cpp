#include "JSLitehtml.h"
#include <android/log_macros.h>
#include <cmath>

namespace RNLitehtml {
    JSLitehtml::JSLitehtml(JSDocumentContainer *container, const std::string &html, const std::string &css)
            : document(litehtml::document::createFromString(html, container, litehtml::master_css, css)) {
    }

    JSLitehtml::~JSLitehtml() {
        document.reset();
        document = nullptr;
    }

    jsi::Value JSLitehtml::get(jsi::Runtime &rt, const jsi::PropNameID &name) {
        auto key = name.utf8(rt);
        if (key == "layout") {
            return jsi::Function::createFromHostFunction(
                    rt,
                    name,
                    2,
                    [this](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *arguments,
                           size_t count) -> jsi::Value {
                        if (count < 2) throw jsi::JSError(rt, "layout arguments error");
                        if (!arguments[0].isNumber()) throw jsi::JSError(rt, "first argument must be a number");
                        if (!arguments[1].isString()) throw jsi::JSError(rt, "second argument must be a string");
                        auto second = arguments[1].asString(rt).utf8(rt);
                        document->render(
                                static_cast<int>(arguments[0].asNumber()),
                                second == "render_fixed_only" ? litehtml::render_type::render_fixed_only :
                                second == "render_no_fixed" ? litehtml::render_type::render_no_fixed :
                                litehtml::render_type::render_all
                        );
                        auto result = jsi::Object(rt);
                        result.setProperty(rt, "width", document->width());
                        result.setProperty(rt, "height", document->height());
                        return result;
                    });
        } else if (key == "draw") {
            return jsi::Function::createFromHostFunction(
                    rt,
                    jsi::PropNameID::forAscii(rt, "draw"),
                    2,
                    [this](jsi::Runtime &rt,
                           const jsi::Value &thisValue,
                           const jsi::Value *arguments,
                           size_t count) -> jsi::Value {
                        if (count < 2) throw jsi::JSError(rt, "draw arguments error");
                        if (!arguments[0].isObject()) throw jsi::JSError(rt, "first argument must be a rect object");
                        if (!arguments[1].isNumber()) throw jsi::JSError(rt, "second argument must be a number");
                        auto pos = arguments[0].asObject(rt);
                        document->draw(
                                static_cast<int>(arguments[1].asNumber()),
                                0,
                                0,
                                new litehtml::position(
                                        static_cast<int>(pos.getProperty(rt, "x").asNumber()),
                                        static_cast<int>(pos.getProperty(rt, "y").asNumber()),
                                        static_cast<int>(pos.getProperty(rt, "width").asNumber()),
                                        static_cast<int>(pos.getProperty(rt, "height").asNumber())
                                )
                        );
                        return jsi::Value::undefined();
                    });
        }

        return jsi::Value::undefined();
    }

    void JSLitehtml::set(jsi::Runtime &rt, const jsi::PropNameID &name, const jsi::Value &value) {
    }

    std::vector<jsi::PropNameID> JSLitehtml::getPropertyNames(jsi::Runtime &rt) {
        auto result = std::vector<jsi::PropNameID>();
        result.push_back(jsi::PropNameID::forAscii(rt, "layout"));
        result.push_back(jsi::PropNameID::forAscii(rt, "draw"));
        result.push_back(jsi::PropNameID::forAscii(rt, "onTouchDown"));
        result.push_back(jsi::PropNameID::forAscii(rt, "onTouchUp"));
        result.push_back(jsi::PropNameID::forAscii(rt, "onTouchCancel"));
        return result;
    }
}