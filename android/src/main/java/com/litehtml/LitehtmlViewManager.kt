package com.litehtml

import android.graphics.Color
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.uimanager.SimpleViewManager
import com.facebook.react.uimanager.ThemedReactContext
import com.facebook.react.uimanager.ViewManagerDelegate
import com.facebook.react.uimanager.annotations.ReactProp
import com.facebook.react.viewmanagers.LitehtmlViewManagerInterface
import com.facebook.react.viewmanagers.LitehtmlViewManagerDelegate

@ReactModule(name = LitehtmlViewManager.NAME)
class LitehtmlViewManager : SimpleViewManager<LitehtmlView>(),
  LitehtmlViewManagerInterface<LitehtmlView> {
  private val mDelegate: ViewManagerDelegate<LitehtmlView>

  init {
    mDelegate = LitehtmlViewManagerDelegate(this)
  }

  override fun getDelegate(): ViewManagerDelegate<LitehtmlView>? {
    return mDelegate
  }

  override fun getName(): String {
    return NAME
  }

  public override fun createViewInstance(context: ThemedReactContext): LitehtmlView {
    return LitehtmlView(context)
  }

  @ReactProp(name = "color")
  override fun setColor(view: LitehtmlView?, color: String?) {
    view?.setBackgroundColor(Color.parseColor(color))
  }

  companion object {
    const val NAME = "LitehtmlView"
  }
}
