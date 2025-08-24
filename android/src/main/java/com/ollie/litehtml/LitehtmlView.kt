package com.ollie.litehtml

import android.content.Context
import android.graphics.Color
import android.graphics.PorterDuff
import android.graphics.SurfaceTexture
import android.view.MotionEvent
import android.view.TextureView
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch


class LitehtmlView(context: Context) : TextureView(context), TextureView.SurfaceTextureListener {
  private var document: Long? = null
  private val scope = CoroutineScope(Dispatchers.Main)
  private val density = context.resources.displayMetrics.density

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
    scope.launch {
      synchronized(this) {
        lockCanvas()?.let { canvas ->
          canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
          canvas.scale(density, density)
          if (document != null) {
            renderer.renderDocument(document!!, canvas, transform(width), transform(height))
          }
          unlockCanvasAndPost(canvas)
        }
      }
    }
  }

  init {
    isOpaque = false
    surfaceTextureListener = this
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

  override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
    layoutDocument()
  }

  override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
    layoutDocument()
  }

  override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
    return true
  }

  override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
  }

  companion object {
    init {
      System.loadLibrary("react-native-litehtml")
    }
  }
}
