package com.ollie.litehtml

import com.facebook.react.bridge.ReadableMap
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.uimanager.SimpleViewManager
import com.facebook.react.uimanager.ThemedReactContext
import com.facebook.react.uimanager.UIManagerHelper
import com.facebook.react.uimanager.ViewManagerDelegate
import com.facebook.react.uimanager.annotations.ReactProp
import com.facebook.react.uimanager.events.Event
import com.facebook.react.viewmanagers.LitehtmlViewManagerDelegate
import com.facebook.react.viewmanagers.LitehtmlViewManagerInterface
import com.ollie.litehtml.events.OnAnchorClickEvent
import com.ollie.litehtml.events.OnImageClickEvent
import com.ollie.litehtml.events.OnLayoutEvent
import com.ollie.litehtml.events.OnLoadSVGEvent

@ReactModule(name = LitehtmlViewManager.NAME)
class LitehtmlViewManager : SimpleViewManager<LitehtmlView>(), LitehtmlViewManagerInterface<LitehtmlView> {
  private val mDelegate: ViewManagerDelegate<LitehtmlView> = LitehtmlViewManagerDelegate(this)

  override fun getName() = NAME
  override fun getDelegate() = mDelegate

  public override fun createViewInstance(context: ThemedReactContext) = LitehtmlView(context)

  override fun addEventEmitters(reactContext: ThemedReactContext, view: LitehtmlView) {
    super.addEventEmitters(reactContext, view)
    val dispatcher = { e: Event<*> ->
      reactContext.runOnUiQueueThread {
        UIManagerHelper.getEventDispatcherForReactTag(reactContext, view.id)?.dispatchEvent(e)
      }
    }
    view.layoutListener = { width, height ->
      dispatcher(OnLayoutEvent(width, height, reactContext.surfaceId, view.id))
    }
    view.imageManager.loadSVGListener = { src ->
      dispatcher(OnLoadSVGEvent(src, reactContext.surfaceId, view.id))
    }
    view.renderer.imageClickListener = { src, width, height ->
      dispatcher(OnImageClickEvent(src, width, height, reactContext.surfaceId, view.id))
    }
    view.renderer.anchorClickListener = { href, content ->
      dispatcher(OnAnchorClickEvent(href, content, reactContext.surfaceId, view.id))
    }
  }

  @ReactProp(name = "content")
  override fun setContent(view: LitehtmlView, content: ReadableMap?) {
    content?.let {
      it.getString("html")?.let { html ->
        view.setHTML(html, it.getString("css") ?: "")
      }
    }
  }

  override fun svgLoaded(view: LitehtmlView, src: String, svgContent: String) {
    view.imageManager.svgLoaded(src, svgContent)
  }

  override fun getExportedCustomDirectEventTypeConstants() = mapOf(
    OnLayoutEvent.NAME to mapOf("registrationName" to OnLayoutEvent.NAME),
    OnLoadSVGEvent.NAME to mapOf("registrationName" to OnLoadSVGEvent.NAME),
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
