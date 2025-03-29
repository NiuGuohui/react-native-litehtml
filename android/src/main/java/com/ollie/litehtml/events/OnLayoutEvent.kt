package com.ollie.litehtml.events

import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.WritableMap
import com.facebook.react.uimanager.events.Event


class OnLayoutEvent(private val width: Int, private val height: Int, surfaceId: Int, viewId: Int) :
  Event<OnLayoutEvent>(surfaceId, viewId) {
  override fun getEventName() = NAME

  override fun getEventData(): WritableMap? {
    val data = Arguments.createMap()
    data.putInt("width", width)
    data.putInt("height", height)
    return data
  }

  companion object {
    const val NAME = "onHtmlLayout"
  }
}
