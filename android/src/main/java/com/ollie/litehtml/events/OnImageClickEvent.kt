package com.ollie.litehtml.events

import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.WritableMap
import com.facebook.react.uimanager.events.Event


class OnImageClickEvent(
  private val src: String, private val width: Int, private val height: Int, surfaceId: Int, viewId: Int
) : Event<OnImageClickEvent>(surfaceId, viewId) {
  override fun getEventName() = NAME

  override fun getEventData(): WritableMap? = Arguments.createMap().apply {
    putString("src", src)
    putInt("width", width)
    putInt("height", height)
  }

  companion object {
    const val NAME = "onImageClick"
  }
}
