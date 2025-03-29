package com.ollie.litehtml

import android.graphics.Typeface
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReadableArray

class LitehtmlModule(context: ReactApplicationContext) : NativeLitehtmlSpec(context) {
  override fun registerFont(fontName: ReadableArray, fontPath: String) {
    val value = lazy { Typeface.createFromFile(fontPath.replace("file://", "")) }
    for (i in 0 until fontName.size()) {
      fontName.getString(i)?.let { LiteHtmlNativeCall.fontRegistry[it] = value }
    }
  }

  override fun registerFontFromAsset(fontName: ReadableArray, inAssetsPath: String) {
    val assets = reactApplicationContext.assets
    val value = lazy { Typeface.createFromAsset(assets, inAssetsPath) }
    for (i in 0 until fontName.size()) {
      fontName.getString(i)?.let { LiteHtmlNativeCall.fontRegistry[it] = value }
    }
  }

  override fun invalidate() {
    super.invalidate()
    LiteHtmlNativeCall.fontRegistry.clear()
  }
}
