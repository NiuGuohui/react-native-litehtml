package com.ollie.litehtml.events

import com.facebook.react.bridge.Arguments
import com.facebook.react.bridge.WritableMap
import com.facebook.react.uimanager.events.Event


class OnAnchorClickEvent(private val href: String, private val content: String?, surfaceId: Int, viewId: Int) :
  Event<OnAnchorClickEvent>(surfaceId, viewId) {
  override fun getEventName() = NAME

  override fun getEventData(): WritableMap? = Arguments.createMap().apply {
    putString("href", href)
    if (content == null) putNull("content")
    else putString("content", content)
  }

  companion object {
    const val NAME = "onAnchorClick"
  }
}
