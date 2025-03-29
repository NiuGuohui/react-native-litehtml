#ifndef LITEHTML_ANDROID_UTIL_H
#define LITEHTML_ANDROID_UTIL_H

#include <jni.h>
#include <vector>
#include <litehtml.h>
#include <initializer_list>

namespace {

    /**
     * Create JAVA IntArray
     */
    jintArray createJIntArray(JNIEnv *env, const std::initializer_list<int> &args) {
        auto size = (int) args.size();
        auto result = env->NewIntArray(size);
        jint *arr = new jint[size];
        std::copy(args.begin(), args.end(), arr);
        env->SetIntArrayRegion(result, 0, size, arr);
        return result;
    }

    /**
     * Create JAVA FloatArray
     */
    jfloatArray createJFloatArray(JNIEnv *env, const std::initializer_list<float> &args) {
        auto size = (int) args.size();
        auto result = env->NewFloatArray(size);
        auto *arr = new jfloat[size];
        std::copy(args.begin(), args.end(), arr);
        env->SetFloatArrayRegion(result, 0, size, arr);
        return result;
    }

    /**
     * Create JAVA Array<String>
     */
    jobjectArray createJStringArray(JNIEnv *env, const std::initializer_list<jstring> &args) {
        auto size = (int) args.size();
        jclass stringClass = env->FindClass("java/lang/String");
        jobjectArray stringArray = env->NewObjectArray(size, stringClass, nullptr);
        auto i = 0;
        for (auto &arg: args) {
            env->SetObjectArrayElement(stringArray, i++, arg);
        }
        return stringArray;
    }

    /**
     * Create JAVA Rect对象
     */
    jobject createJRect(JNIEnv *env, int x, int y, int width, int height) {
        jclass rectClass = env->FindClass("android/graphics/Rect");
        jmethodID constructor = env->GetMethodID(rectClass, "<init>", "(IIII)V");
        return env->NewObject(rectClass, constructor, x, y, x + width, y + height);
    }

    /**
     * Create JAVA RectF对象
     */
    jobject createJRectF(JNIEnv *env, float x, float y, float width, float height) {
        jclass rectFClass = env->FindClass("android/graphics/RectF");
        jmethodID constructor = env->GetMethodID(rectFClass, "<init>", "(FFFF)V");
        return env->NewObject(rectFClass, constructor, x, y, x + width, y + height);
    }


    /**
     * Convert to parsable color values for JAVA layer
     */
    jstring createJColorString(JNIEnv *env, litehtml::web_color color) {
        char argb[9];
        t_snprintf(argb, 9, "%02X%02X%02X%02X", color.alpha, color.red, color.green, color.blue);
        return env->NewStringUTF(argb);
    }

    /**
     * Get border style
     */
    const char *getBorderStyle(litehtml::border_style style) {
        switch (style) {
            case litehtml::border_style_dotted:
                return "dotted";
            case litehtml::border_style_dashed:
                return "dashed";
            case litehtml::border_style_double:
                return "double";
            case litehtml::border_style_groove:
            case litehtml::border_style_solid:
            case litehtml::border_style_ridge:
            case litehtml::border_style_inset:
            case litehtml::border_style_outset:
                return "solid";
            default:
                return "none";
        }
    }

}

#endif //LITEHTML_ANDROID_UTIL_H
