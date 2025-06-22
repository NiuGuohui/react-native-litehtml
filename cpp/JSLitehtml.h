#pragma once

#include "litehtml.h"
#include "JSDocumentContainer.h"
#include <jsi/jsi.h>

using namespace facebook;

namespace RNLitehtml {

    class JSLitehtml : public jsi::HostObject {
    public:
        JSLitehtml(jsi::Runtime &runtime, JSDocumentContainer *container, const std::string& html,
                   const std::string &css);

        ~JSLitehtml() override;

        jsi::Value get(jsi::Runtime &rt, const jsi::PropNameID &name) override;

        void set(jsi::Runtime &rt, const jsi::PropNameID &name, const jsi::Value &value) override;

        std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt) override;

    private:
        JSDocumentContainer *container;
        litehtml::document::ptr document;
    };
}