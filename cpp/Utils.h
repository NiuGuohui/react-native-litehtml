#pragma once

#include "litehtml.h"
#include <jsi/jsi.h>

namespace RNLitehtml {

    std::string formatColor(litehtml::web_color color) {
        return "#" + color.to_string();
    }
}