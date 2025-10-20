package com.ollie.litehtml

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Path
import android.graphics.PorterDuff
import android.graphics.PorterDuffXfermode
import android.graphics.RectF
import android.os.Handler
import androidx.core.graphics.withClip
import androidx.core.net.toUri
import com.caverock.androidsvg.SVG
import com.facebook.common.executors.UiThreadImmediateExecutorService
import com.facebook.common.references.CloseableReference
import com.facebook.datasource.BaseDataSubscriber
import com.facebook.datasource.DataSource
import com.facebook.drawee.backends.pipeline.Fresco
import com.facebook.imagepipeline.image.CloseableBitmap
import com.facebook.imagepipeline.image.CloseableImage
import com.facebook.imagepipeline.request.ImageRequestBuilder
import kotlin.math.ceil


class ImageManager(private val context: Context, private val imgLoaded: () -> Unit) {
  private val cacheSvgs = HashMap<String, SVG>()
  private val cache = HashMap<String, CloseableReference<CloseableImage>>()

  var loadSVGListener: ((src: String) -> Unit)? = null

  fun loadImage(url: String) {
    if (isSvgImage(url)) {
      Handler(context.mainLooper).post {
        if (cacheSvgs.contains(url)) imgLoaded.invoke()
        else loadSVGListener?.invoke(url)
      }
    } else {
      Fresco.getImagePipeline().fetchDecodedImage(ImageRequestBuilder.newBuilderWithSource(url.toUri()).build(), null)
        .subscribe(object : BaseDataSubscriber<CloseableReference<CloseableImage>>() {
          override fun onNewResultImpl(dataSource: DataSource<CloseableReference<CloseableImage>>) {
            val result = dataSource.result
            if (dataSource.isFinished && dataSource.hasResult() && result != null) {
              cache[url] = result
              imgLoaded.invoke()
            }
          }

          override fun onFailureImpl(dataSource: DataSource<CloseableReference<CloseableImage>>) {
            imgLoaded.invoke()
          }
        }, UiThreadImmediateExecutorService.getInstance())
    }
  }

  fun getImageSize(url: String): IntArray {
    if (isSvgImage(url)) {
      if (!cacheSvgs.containsKey(url)) return intArrayOf(0, 0)
      val svg = cacheSvgs[url]!!
      return intArrayOf(ceil(svg.documentWidth).toInt(), ceil(svg.documentHeight).toInt())
    }
    return cache[url]?.get()?.let { intArrayOf(it.width, it.height) } ?: intArrayOf(0, 0)
  }

  fun drawImage(canvas: Canvas, rect: RectF, url: String, borderRadius: FloatArray) {
    if (isSvgImage(url)) {
      val clip = Path().apply {
        if (borderRadius.max() > 0) addRoundRect(rect, borderRadius, Path.Direction.CW)
        else addRect(rect, Path.Direction.CW)
      }
      canvas.withClip(clip) {
        cacheSvgs[url]?.let { drawPicture(it.renderToPicture(), rect) }
      }
    } else {
      cache[url]?.get()?.let {
        if (it is CloseableBitmap) {
          if (borderRadius.max() > 0) {
            canvas.withClip(Path().apply { addRoundRect(rect, borderRadius, Path.Direction.CW) }) {
              drawBitmap(it.underlyingBitmap, null, rect, Paint().apply {
                strokeWidth = 0f
                isAntiAlias = true
                xfermode = PorterDuffXfermode(PorterDuff.Mode.SRC_IN)
              })
            }
          } else {
            canvas.drawBitmap(it.underlyingBitmap, null, rect, Paint().apply { isAntiAlias = true })
          }
        }
      }
    }
  }

  fun svgLoaded(src: String, svgContent: String) {
    cacheSvgs[src] = SVG.getFromString(svgContent)
    imgLoaded.invoke()
  }

  fun release() {
    cache.forEach { it.value.close() }
    cache.clear()
    cacheSvgs.clear()
    loadSVGListener = null
  }

  private fun isSvgImage(url: String) = url.toUri().path?.contains(".svg") ?: false
}
