#pragma once

#if __has_include(<React-Codegen/LitehtmlSpecJSI.h>)
// CocoaPods include (iOS)
#include <React-Codegen/LitehtmlSpecJSI.h>
#elif __has_include(<LitehtmlSpecJSI.h>)
// CMake include on Android
#include <LitehtmlSpecJSI.h>
#else
#error Cannot find react-native-litehtml spec! Try cleaning your cache and re-running CodeGen!
#endif

#include "litehtml.h"

namespace facebook::react {

// The TurboModule itself
    class NativeLitehtmlModule : public NativeLitehtmlCxxSpec<NativeLitehtmlModule> {
    public:
        NativeLitehtmlModule(std::shared_ptr<CallInvoker> jsInvoker);

        ~NativeLitehtmlModule();

        jsi::Object
        createInstance(jsi::Runtime &rt, jsi::String html, std::optional<jsi::String> css,
                       jsi::Object container);
    };

} // namespace facebook::react
