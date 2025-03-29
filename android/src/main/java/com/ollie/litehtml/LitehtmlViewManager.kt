package com.ollie.litehtml

import com.facebook.react.bridge.ReadableMap
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.uimanager.SimpleViewManager
import com.facebook.react.uimanager.ThemedReactContext
import com.facebook.react.uimanager.UIManagerHelper
import com.facebook.react.uimanager.ViewManagerDelegate
import com.facebook.react.uimanager.annotations.ReactProp
import com.facebook.react.viewmanagers.LitehtmlViewManagerDelegate
import com.facebook.react.viewmanagers.LitehtmlViewManagerInterface
import com.ollie.litehtml.events.OnAnchorClickEvent
import com.ollie.litehtml.events.OnImageClickEvent
import com.ollie.litehtml.events.OnLayoutEvent

@ReactModule(name = LitehtmlViewManager.NAME)
class LitehtmlViewManager : SimpleViewManager<LitehtmlView>(), LitehtmlViewManagerInterface<LitehtmlView> {
  private val mDelegate: ViewManagerDelegate<LitehtmlView> = LitehtmlViewManagerDelegate(this)

  override fun getName() = NAME
  override fun getDelegate() = mDelegate

  public override fun createViewInstance(context: ThemedReactContext) = LitehtmlView(context)

  override fun addEventEmitters(reactContext: ThemedReactContext, view: LitehtmlView) {
    super.addEventEmitters(reactContext, view)
    val dispatcher = { UIManagerHelper.getEventDispatcherForReactTag(reactContext, view.id) }
    view.setOnLayoutListener { width, height ->
      dispatcher()?.dispatchEvent(OnLayoutEvent(width, height, reactContext.surfaceId, view.id))
    }
    view.imageClickListener = { src, width, height ->
      dispatcher()?.dispatchEvent(OnImageClickEvent(src, width, height, reactContext.surfaceId, view.id))
    }
    view.anchorClickListener = { href, content ->
      dispatcher()?.dispatchEvent(OnAnchorClickEvent(href, content, reactContext.surfaceId, view.id))
    }
  }

  @ReactProp(name = "content")
  override fun setContent(view: LitehtmlView, content: ReadableMap?) {
    content?.let {
      val css = it.getString("css") ?: ""
      it.getString("html")?.let { html -> view.setHTML(html, css) }
    }
  }

  override fun getExportedCustomDirectEventTypeConstants() = mapOf(
    OnLayoutEvent.NAME to mapOf("registrationName" to OnLayoutEvent.NAME),
    OnImageClickEvent.NAME to mapOf("registrationName" to OnImageClickEvent.NAME),
    OnAnchorClickEvent.NAME to mapOf("registrationName" to OnAnchorClickEvent.NAME),
  )

  override fun onDropViewInstance(view: LitehtmlView) {
    view.destroy()
    super.onDropViewInstance(view)
  }

  companion object {
    const val NAME = "LitehtmlView"
  }
}
