#ifndef LITEHTML_JNI_NATIVE_CALL_H
#define LITEHTML_JNI_NATIVE_CALL_H

#include <jni.h>
#include <string>
#include "litehtml.h"

class JniNativeCall {
public:
    JNIEnv *env;
    jobject nativeCall;
    jclass nativeCallClass;

public:
    JniNativeCall(JNIEnv *env, jobject nativeCall);

    ~JniNativeCall();

    void
    createFont(const char *fontName, const litehtml::font_description &descr, float result[]) const;

    void deleteFont(int hFont) const;

    float textWidth(const char *text, int hFont) const;

    void
    drawBackground(jobject canvas, jobject box, jfloatArray borderRadiuses, jstring argb) const;

    void drawText(jobject canvas, jobject box, jstring argb, int font, const char *text) const;

    void drawListMarker(jobject canvas, jobject box, jstring argb, int type) const;

    void loadImage(const char *url) const;

    void getImageSize(const char *url, int result[2]) const;

    void drawImage(jobject canvas, const char *url, jobject box, jfloatArray borderRadiuses) const;

    void drawBorder(jobject canvas, jobject box, jfloatArray borderRadiuses, jfloatArray borderWidths,
                    jobjectArray borderStyles, jobjectArray borderColors) const;

    void setClip(jobject box, jfloatArray borderRadiuses) const;

    void delClip() const;

    void callImageClick(const char *src, const int width, const int height) const;

    void callAnchorClick(const char *href, const char *content) const;
};

#endif //LITEHTML_JNI_NATIVE_CALL_H
