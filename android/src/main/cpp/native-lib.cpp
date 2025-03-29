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
Java_com_ollie_litehtml_LitehtmlView_createDocument(JNIEnv *env, jobject view, jobject nativeCall,
                                                    jstring html, jstring user_styles) {
    auto htmlStr = env->GetStringUTFChars(html, nullptr);
    auto userStylesStr = env->GetStringUTFChars(user_styles, nullptr);
    auto container = new android_container(new JniNativeCall(env, nativeCall));
    auto doc = document::createFromString(htmlStr, container, master_css, userStylesStr);
    return reinterpret_cast<jlong>(new document::ptr(doc));
}

/**
 * Layout document object
 */
extern "C" JNIEXPORT jintArray JNICALL
Java_com_ollie_litehtml_LitehtmlView_layoutDocument(JNIEnv *env, jobject view, jlong document, jint width) {
    auto max_width = reinterpret_cast<int>(width);
    auto doc = getDocument(document);
    doc->render(max_width);
    auto result = createJIntArray(env, {doc->width(), doc->height()});
    return result;
}

/**
 * Draw document object
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ollie_litehtml_LitehtmlView_renderDocument(JNIEnv *env, jobject view, jlong document, jobject canvas,
                                                    jint width, jint height) {
    auto doc = getDocument(document);
    doc->draw(reinterpret_cast<uint_ptr>(canvas), 0, 0,
              new position(0, 0, reinterpret_cast<int>(width), reinterpret_cast<int>(height)));
}

/**
 * Delete document object
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ollie_litehtml_LitehtmlView_deleteDocument(JNIEnv *env, jobject view, jlong address) {
    assert(address != 0x0);
    delete reinterpret_cast<document::ptr *>(address);
}

/**
 * Document response touch events
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_ollie_litehtml_LitehtmlView_touchDocument(JNIEnv *env, jobject view, jlong document,
                                                   jint type, jint x, jint y) {
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
        auto method = env->GetMethodID(env->GetObjectClass(view), "postInvalidateOnAnimation", "()V");
        env->CallVoidMethod(view, method);
    }
}
