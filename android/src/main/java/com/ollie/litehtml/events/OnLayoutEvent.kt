package com.ollie.litehtml.events

import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.WritableMap
import com.facebook.react.uimanager.events.Event


class OnLayoutEvent(private val width: Float, private val height: Float, surfaceId: Int, viewId: Int) :
  Event<OnLayoutEvent>(surfaceId, viewId) {
  override fun getEventName() = NAME
  override fun canCoalesce() = false

  override fun getEventData(): WritableMap? {
    val data = Arguments.createMap()
    data.putDouble("width", width.toDouble())
    data.putDouble("height", height.toDouble())
    return data
  }

  companion object {
    const val NAME = "onHtmlLayout"
  }
}
