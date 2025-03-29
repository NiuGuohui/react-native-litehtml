package com.ollie.litehtml

import android.content.Context
import android.graphics.Canvas
import android.view.MotionEvent
import android.view.View
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch


class LitehtmlView(context: Context) : View(context) {
  private var document: Long? = null
  private val density = context.resources.displayMetrics.density
  private var listener: ((width: Int, height: Int) -> Unit)? = null
  private val nativeCall = LiteHtmlNativeCall(context) {
    post { requestLayout() }
    onSizeChanged(width, height, width, height)
  }

  var imageClickListener: ((src: String, width: Int, height: Int) -> Unit)? = null
    set(value) {
      field = value
      nativeCall.imageClickListener = value
    }
  var anchorClickListener: ((href: String, content: String?) -> Unit)? = null
    set(value) {
      field = value
      nativeCall.anchorClickListener = value
    }

  fun setHTML(html: String, css: String = "") {
    if (document != null) deleteDocument(document!!)
    document = createDocument(nativeCall, html, css)
    onSizeChanged(width, height, width, height)
  }

  fun setOnLayoutListener(listener: (width: Int, height: Int) -> Unit) {
    this.listener = listener
  }

  fun destroy() {
    nativeCall.release()
    scope.launch {
      synchronized(this) {
        document?.let { deleteDocument(it) }
        document = null
        listener = null
      }
    }
  }

  override fun onTouchEvent(event: MotionEvent): Boolean {
    document?.let {
      val tx = transform(event.x.toInt())
      val ty = transform(event.y.toInt())
      when (event.action) {
        MotionEvent.ACTION_DOWN -> touchDocument(it, 1, tx, ty)
        MotionEvent.ACTION_UP -> touchDocument(it, 2, tx, ty)
        MotionEvent.ACTION_CANCEL -> touchDocument(it, 3, tx, ty)
      }
      postInvalidateOnAnimation()
    }
    return true
  }

  override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
    scope.launch {
      synchronized(this) {
        document?.let {
          val size = layoutDocument(it, transform(width))
          listener?.invoke(size[0], size[1])
          postInvalidateOnAnimation()
        }
      }
    }
  }

  override fun onDraw(canvas: Canvas) {
    if (width > 0 && height > 0 && document != null) {
      canvas.scale(density, density)
      synchronized(this) { renderDocument(document!!, canvas, transform(width), transform(height)) }
    }
  }

  private fun transform(size: Int): Int = (size / density).toInt()

  private external fun deleteDocument(document: Long)
  private external fun layoutDocument(document: Long, maxWith: Int): IntArray
  private external fun touchDocument(document: Long, type: Int, x: Int, y: Int)
  private external fun renderDocument(document: Long, canvas: Canvas, width: Int, height: Int)
  private external fun createDocument(nativeCall: LiteHtmlNativeCall, html: String, userStyles: String): Long

  override fun requestLayout() {
    super.requestLayout()
    post {
      measure(
        MeasureSpec.makeMeasureSpec(width, MeasureSpec.EXACTLY),
        MeasureSpec.makeMeasureSpec(height, MeasureSpec.EXACTLY)
      )
      layout(left, top, right, bottom)
    }
  }

  companion object {
    val scope = CoroutineScope(Dispatchers.Main.limitedParallelism(3))

    init {
      System.loadLibrary("react-native-litehtml")
    }
  }
}
