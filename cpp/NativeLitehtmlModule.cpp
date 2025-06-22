#include <utility>
#include "NativeLitehtmlModule.h"
#include "JSLitehtml.h"
#include "JSDocumentContainer.h"

namespace facebook::react {
    NativeLitehtmlModule::NativeLitehtmlModule(std::shared_ptr<CallInvoker> jsInvoker)
            : NativeLitehtmlCxxSpec(std::move(jsInvoker)) {
    }

    jsi::Object
    NativeLitehtmlModule::createInstance(jsi::Runtime &rt, jsi::String html,
                                         std::optional<jsi::String> css, jsi::Object container) {
        return rt.global().createFromHostObject(
                rt,
                std::make_shared<RNLitehtml::JSLitehtml>(
                        new RNLitehtml::JSDocumentContainer(&rt, std::move(container)),
                        html.utf8(rt),
                        css.has_value() ? css->utf8(rt) : ""
                )
        );
    }

    NativeLitehtmlModule::~NativeLitehtmlModule() = default;
}
