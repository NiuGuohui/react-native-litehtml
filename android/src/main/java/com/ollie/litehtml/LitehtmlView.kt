package com.ollie.litehtml

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.view.MotionEvent
import android.view.View
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch


class LitehtmlView(context: Context) : View(context) {
  private var document: Long? = null
  private val scope = CoroutineScope(Dispatchers.Main)
  private val density = context.resources.displayMetrics.density
  private var needRedraw = true

  var layoutListener: ((width: Float, height: Float) -> Unit)? = null

  val imageManager = ImageManager { layoutDocument() }
  val renderer = LitehtmlRenderer(imageManager).apply {
    renderListener = { renderDocument() }
  }

  private fun transform(size: Int) = size / density

  private fun layoutDocument() {
    scope.launch {
      synchronized(this) {
        document?.let {
          val size = renderer.layoutDocument(it, transform(width))
          layoutListener?.invoke(size[0], size[1])
          renderDocument()
        }
      }
    }
  }

  private fun renderDocument() {
    needRedraw = true
    postInvalidateOnAnimation()
  }

  init {
    setBackgroundColor(Color.TRANSPARENT)
    setLayerType(LAYER_TYPE_HARDWARE, null)
  }

  fun setHTML(html: String, css: String = "") {
    if (document != null) renderer.deleteDocument(document!!)
    document = renderer.createDocument(html, css)
    layoutDocument()
  }

  fun destroy() {
    renderer.release()
    imageManager.release()
    synchronized(this) {
      document?.let { renderer.deleteDocument(it) }
      document = null
      layoutListener = null
    }
  }

  override fun onDraw(canvas: Canvas) {
    if (needRedraw) {
      canvas.drawColor(Color.TRANSPARENT)
      canvas.scale(density, density)
      if (document != null) {
        renderer.renderDocument(document!!, canvas, transform(width), transform(height))
        needRedraw = false
      }
    }
  }

  override fun onTouchEvent(event: MotionEvent): Boolean {
    document?.let {
      val tx = transform(event.x.toInt())
      val ty = transform(event.y.toInt())
      when (event.action) {
        MotionEvent.ACTION_DOWN -> renderer.touchDocument(it, 1, tx, ty)
        MotionEvent.ACTION_UP -> renderer.touchDocument(it, 2, tx, ty)
        MotionEvent.ACTION_CANCEL -> renderer.touchDocument(it, 3, tx, ty)
      }
    }
    return true
  }

  override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
    super.onSizeChanged(w, h, oldw, oldh)
    if (w > 0 && h > 0 && (w != oldw || h != oldh)) layoutDocument()
  }

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
    init {
      System.loadLibrary("react-native-litehtml")
    }
  }
}
