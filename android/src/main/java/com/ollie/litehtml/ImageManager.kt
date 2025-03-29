package com.ollie.litehtml

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Path
import android.graphics.PorterDuff
import android.graphics.PorterDuffXfermode
import android.graphics.Rect
import android.util.Log
import androidx.core.graphics.toRectF
import androidx.core.graphics.withClip
import androidx.core.net.toUri
import com.caverock.androidsvg.PreserveAspectRatio
import com.caverock.androidsvg.RenderOptions
import com.caverock.androidsvg.SVG
import com.facebook.common.executors.UiThreadImmediateExecutorService
import com.facebook.common.references.CloseableReference
import com.facebook.datasource.BaseDataSubscriber
import com.facebook.datasource.DataSource
import com.facebook.drawee.backends.pipeline.Fresco
import com.facebook.imagepipeline.image.CloseableBitmap
import com.facebook.imagepipeline.image.CloseableImage
import com.facebook.imagepipeline.request.ImageRequestBuilder
import com.facebook.react.modules.network.OkHttpClientProvider
import okhttp3.Call
import okhttp3.Callback
import okhttp3.Request
import okhttp3.Response
import java.io.IOException
import kotlin.math.roundToInt


class ImageManager(private val context: Context) {
  private val cacheSvgs = HashMap<String, SVG>()
  private val cache = HashMap<String, CloseableReference<CloseableImage>>()

  fun loadImage(url: String, relayout: () -> Unit) {
    if (isSvgImage(url)) {
      val request: Request = Request.Builder().url(url).build()
      OkHttpClientProvider.createClient(context).newCall(request).enqueue(object : Callback {
        override fun onResponse(call: Call, response: Response) {
          if (response.isSuccessful) {
            response.body?.byteStream()?.let {
              try {
                val svg = SVG.getFromInputStream(it)
                cacheSvgs[url] = svg
              } catch (e: Exception) {
                Log.e("ImageManager", "Error loading SVG: $url", e)
              } finally {
                relayout()
              }
            }
          }
        }

        override fun onFailure(call: Call, e: IOException) {
        }
      })
    } else {
      Fresco.getImagePipeline().fetchDecodedImage(ImageRequestBuilder.newBuilderWithSource(url.toUri()).build(), null)
        .subscribe(object : BaseDataSubscriber<CloseableReference<CloseableImage>>() {
          override fun onNewResultImpl(dataSource: DataSource<CloseableReference<CloseableImage>>) {
            val result = dataSource.result
            if (dataSource.isFinished && dataSource.hasResult() && result != null) {
              relayout()
              cache[url] = result
            }
          }

          override fun onFailureImpl(dataSource: DataSource<CloseableReference<CloseableImage>>) {
            relayout()
          }
        }, UiThreadImmediateExecutorService.getInstance())
    }
  }


  fun getImageSize(url: String): IntArray {
    if (isSvgImage(url)) {
      return cacheSvgs[url]?.documentViewBox?.let { intArrayOf(it.width().roundToInt(), it.height().roundToInt()) }
        ?: intArrayOf()
    }
    return cache[url]?.get()?.let { intArrayOf(it.width, it.height) } ?: intArrayOf(0, 0)
  }

  fun drawImage(canvas: Canvas, rect: Rect, url: String, borderRadius: FloatArray) {
    if (isSvgImage(url)) {
      cacheSvgs[url]?.let { svg ->
        canvas.withClip(Path().apply {
          if (borderRadius.max() > 0) addRoundRect(rect.toRectF(), borderRadius, Path.Direction.CW)
          else addRect(rect.toRectF(), Path.Direction.CW)
        }) {
          val options = RenderOptions().preserveAspectRatio(PreserveAspectRatio.FULLSCREEN_START)
          rect.toRectF().let { options.viewPort(it.left, it.top, it.width(), it.height()) }
          svg.setDocumentWidth("100%")
          svg.setDocumentHeight("100%")
          svg.renderToCanvas(canvas, options)
        }
      }
    } else {
      cache[url]?.get()?.let {
        if (it is CloseableBitmap) {
          if (borderRadius.max() > 0) {
            canvas.withClip(Path().apply { addRoundRect(rect.toRectF(), borderRadius, Path.Direction.CW) }) {
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

  fun release() {
    cache.forEach { it.value.close() }
    cache.clear()
    cacheSvgs.clear()
  }


  private fun isSvgImage(url: String) = url.toUri().path?.contains(".svg") ?: false
}
