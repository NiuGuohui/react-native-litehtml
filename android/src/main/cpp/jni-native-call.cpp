#include <android/rect.h>
#include "jni-native-call.h"
#include "android-util.h"

#define NATIVE_CALL_JAVA_CLASS "com/ollie/litehtml/LiteHtmlNativeCall"
#define FONT_INFO_JAVA_CLASS "com/ollie/litehtml/FontInfo"


JniNativeCall::JniNativeCall(JNIEnv *env, jobject nativeCall) {
    this->env = env;
    this->nativeCall = env->NewGlobalRef(nativeCall);
    this->nativeCallClass = (jclass) env->NewGlobalRef(env->FindClass(NATIVE_CALL_JAVA_CLASS));
}

JniNativeCall::~JniNativeCall() {
    if (nativeCall) env->DeleteGlobalRef(nativeCall);
}

void
JniNativeCall::createFont(const char *fontName, const litehtml::font_description &descr, int result[]) const {
    jclass jInfoClass = env->FindClass(FONT_INFO_JAVA_CLASS);
    jmethodID jInfoConstructor = env->GetMethodID(jInfoClass, "<init>",
                                                  "(IIZIILjava/lang/String;Ljava/lang/String;)V");
    jobject jInfo = env->NewObject(jInfoClass, jInfoConstructor, descr.size, descr.weight,
                                   static_cast<bool>(descr.style == litehtml::font_style::font_style_italic),
                                   static_cast<int>(descr.decoration_line),
                                   static_cast<int>(descr.decoration_style),
                                   env->NewStringUTF(descr.decoration_color.to_string().c_str()),
                                   descr.emphasis_style.empty() ? nullptr :
                                   env->NewStringUTF(descr.emphasis_style.c_str()));

    auto methodName = std::string("(Ljava/lang/String;L") + FONT_INFO_JAVA_CLASS + ";)[I";

    auto jResult = (jintArray) env->CallObjectMethod(
            nativeCall,
            env->GetMethodID(nativeCallClass, "createFont", methodName.c_str()),
            fontName ? env->NewStringUTF(fontName) : nullptr, jInfo
    );
    auto l = env->GetIntArrayElements(jResult, nullptr);
    for (int i = 0; i < 5; ++i) result[i] = l[i];
    env->ReleaseIntArrayElements(jResult, l, 0);
    env->DeleteLocalRef(jResult);
}

void JniNativeCall::deleteFont(int hFont) const {
    env->CallVoidMethod(nativeCall, env->GetMethodID(nativeCallClass, "deleteFont", "(I)V"),
                        hFont);
}

int JniNativeCall::textWidth(const char *text, int hFont) const {
    auto jW = env->CallFloatMethod(nativeCall,
                                   env->GetMethodID(nativeCallClass, "measureTextWidth",
                                                    "(ILjava/lang/String;)F"),
                                   hFont,
                                   env->NewStringUTF(text)
    );
    return static_cast<int>(std::round(jW));
}

void
JniNativeCall::drawBackground(jobject canvas, jobject box, jfloatArray borderRadiuses, jstring argb) const {
    env->CallVoidMethod(nativeCall,
                        env->GetMethodID(nativeCallClass, "drawBackgroundColor",
                                         "(Landroid/graphics/Canvas;Landroid/graphics/Rect;Ljava/lang/String;[F)V"),
                        canvas, box, argb, borderRadiuses);
}


void
JniNativeCall::drawText(jobject canvas, jobject box, jstring argb, int font, const char *text) const {
    env->CallVoidMethod(nativeCall,
                        env->GetMethodID(nativeCallClass, "drawText",
                                         "(Landroid/graphics/Canvas;Landroid/graphics/RectF;Ljava/lang/String;ILjava/lang/String;)V"),
                        canvas, box, argb, font, env->NewStringUTF(text));
}


void JniNativeCall::drawListMarker(jobject canvas, jobject box, jstring argb, int type) const {
    env->CallVoidMethod(nativeCall,
                        env->GetMethodID(nativeCallClass, "drawListMarker",
                                         "(Landroid/graphics/Canvas;Landroid/graphics/Rect;Ljava/lang/String;I)V"),
                        canvas, box, argb, type);
}

void JniNativeCall::loadImage(const char *url) const {
    env->CallVoidMethod(nativeCall, env->GetMethodID(nativeCallClass, "loadImage", "(Ljava/lang/String;)V"),
                        env->NewStringUTF(url));
}

void JniNativeCall::getImageSize(const char *url, int result[2]) const {
    auto jResult = (jintArray) env->CallObjectMethod(nativeCall,
                                                     env->GetMethodID(nativeCallClass, "getImageSize",
                                                                      "(Ljava/lang/String;)[I"),
                                                     env->NewStringUTF(url));
    auto l = env->GetIntArrayElements(jResult, nullptr);
    for (int i = 0; i < 2; ++i) result[i] = l[i];
    env->ReleaseIntArrayElements(jResult, l, 0);
    env->DeleteLocalRef(jResult);
}

void JniNativeCall::drawImage(jobject canvas, const char *url, jobject box, jfloatArray borderRadiuses) const {
    env->CallVoidMethod(
            nativeCall,
            env->GetMethodID(nativeCallClass, "drawImage",
                             "(Landroid/graphics/Canvas;Landroid/graphics/Rect;Ljava/lang/String;[F)V"),
            canvas, box, env->NewStringUTF(url), borderRadiuses
    );
}

void JniNativeCall::drawBorder(jobject canvas, jobject box, jfloatArray borderRadiuses, jfloatArray borderWidths,
                               jobjectArray borderStyles, jobjectArray borderColors) const {
    env->CallVoidMethod(
            nativeCall,
            env->GetMethodID(nativeCallClass, "drawBorder",
                             "(Landroid/graphics/Canvas;Landroid/graphics/RectF;[F[F[Ljava/lang/String;[Ljava/lang/String;)V"),
            canvas,
            box,
            borderRadiuses,
            borderWidths,
            borderStyles,
            borderColors
    );
}

void JniNativeCall::setClip(jobject box, jfloatArray borderRadiuses) const {
    env->CallVoidMethod(
            nativeCall,
            env->GetMethodID(nativeCallClass, "setClip", "(Landroid/graphics/RectF;[F)V"),
            box,
            borderRadiuses
    );
}

void JniNativeCall::delClip() const {
    env->CallVoidMethod(nativeCall, env->GetMethodID(nativeCallClass, "clearClip", "()V"));
}

void JniNativeCall::callImageClick(const char *src, const int width, const int height) const {
    env->CallVoidMethod(
            nativeCall,
            env->GetMethodID(nativeCallClass, "callImageClick", "(Ljava/lang/String;II)V"),
            env->NewStringUTF(src),
            width,
            height
    );
}

void JniNativeCall::callAnchorClick(const char *href, const char *content) const {
    env->CallVoidMethod(
            nativeCall,
            env->GetMethodID(nativeCallClass, "callAnchorClick", "(Ljava/lang/String;Ljava/lang/String;)V"),
            env->NewStringUTF(href),
            env->NewStringUTF(content)
    );

}
