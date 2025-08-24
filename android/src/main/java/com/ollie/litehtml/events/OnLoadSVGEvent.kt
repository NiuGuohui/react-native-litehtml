package com.ollie.litehtml.events

import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.WritableMap
import com.facebook.react.uimanager.events.Event


class OnLoadSVGEvent(private val src: String, surfaceId: Int, viewId: Int) : Event<OnLoadSVGEvent>(surfaceId, viewId) {
  override fun getEventName() = NAME

  override fun getEventData(): WritableMap? {
    val data = Arguments.createMap()
    data.putString("src", src)
    return data
  }

  companion object {
    const val NAME = "onLoadSVG"
  }
}
