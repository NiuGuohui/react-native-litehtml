#include <jni.h>
#include <string>
#include <cassert>
#include "android-util.h"
#include "jni-native-call.h"
#include "android-container.h"

// Get document object
document::ptr getDocument(jlong address) {
    assert(address != 0x0);
    auto ptr = reinterpret_cast<document::ptr *>(address);
    assert(ptr != nullptr);
    return *ptr;
}

/**
 * Create a document object
 */
extern "C" JNIEXPORT jlong JNICALL
Java_com_ollie_litehtml_LitehtmlRenderer_createDocument(JNIEnv *env, jobject renderer, jstring html,
                                                        jstring user_styles) {
    auto htmlStr = env->GetStringUTFChars(html, nullptr);
    auto userStylesStr = env->GetStringUTFChars(user_styles, nullptr);
    auto container = new android_container(new JniNativeCall(env, renderer));
    auto doc = document::createFromString(htmlStr, container, master_css, userStylesStr);
    return reinterpret_cast<jlong>(new document::ptr(doc));
}

/**
 * Layout document object
 */
extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_ollie_litehtml_LitehtmlRenderer_layoutDocument(JNIEnv *env, jobject, jlong document, jfloat width) {
    auto doc = getDocument(document);
    doc->render(width);
    auto result = createJFloatArray(env, {doc->width(), doc->height()});
    return result;
}

/**
 * Draw document object
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ollie_litehtml_LitehtmlRenderer_renderDocument(JNIEnv *env, jobject, jlong document, jobject canvas,
                                                        jfloat width, jfloat height) {
    auto doc = getDocument(document);
    doc->draw(reinterpret_cast<uint_ptr>(canvas), 0, 0, new position(0, 0, width, height));
}

/**
 * Delete document object
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ollie_litehtml_LitehtmlRenderer_deleteDocument(JNIEnv *env, jobject, jlong address) {
    assert(address != 0x0);
    delete reinterpret_cast<document::ptr *>(address);
}

/**
 * Document response touch events
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_ollie_litehtml_LitehtmlRenderer_touchDocument(JNIEnv *env, jobject renderer, jlong document,
                                                       jint type, jfloat x, jfloat y) {
    auto s = std::vector<position>();
    auto doc = getDocument(document);
    auto a = false;
    switch (reinterpret_cast<int>(type)) {
        case 1:
            a = doc->on_lbutton_down(x, y, 0, 0, s);
            break;
        case 2:
            if (doc->on_lbutton_up(x, y, 0, 0, s)) a = true;
            if (doc->on_mouse_leave(s)) a = true;
            break;
        case 3:
            a = doc->on_button_cancel(s);
            break;
    }
    if (a && !s.empty()) {
        auto method = env->GetMethodID(env->GetObjectClass(renderer), "callRender", "()V");
        env->CallVoidMethod(renderer, method);
    }
}
