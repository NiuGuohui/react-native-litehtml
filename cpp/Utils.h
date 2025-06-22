#pragma once

#include "litehtml.h"
#include <jsi/jsi.h>

namespace RNLitehtml {

    using namespace facebook;

    std::string formatColor(litehtml::web_color color) {
        return "#" + color.to_string();
    }

    jsi::Value createRect(jsi::Runtime &runtime, const litehtml::position pos) {
        auto obj = jsi::Object(runtime);
        obj.setProperty(runtime, "x", pos.x);
        obj.setProperty(runtime, "y", pos.y);
        obj.setProperty(runtime, "width", pos.width);
        obj.setProperty(runtime, "height", pos.height);
        return obj;
    }
}