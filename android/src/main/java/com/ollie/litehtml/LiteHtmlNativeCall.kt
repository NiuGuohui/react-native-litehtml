package com.ollie.litehtml

import android.content.Context
import android.graphics.Canvas
import android.graphics.CornerPathEffect
import android.graphics.Paint
import android.graphics.Path
import android.graphics.Rect
import android.graphics.RectF
import android.graphics.Typeface
import androidx.core.graphics.toRectF
import kotlin.math.absoluteValue
import kotlin.math.max
import androidx.core.graphics.toColorInt
import androidx.core.graphics.withClip
import kotlin.math.ceil


class LiteHtmlNativeCall(context: Context, private val relayout: () -> Unit) {
  private var fontId = 1
  private var clips = mutableListOf<Path>()
  private val imageManager = ImageManager(context)
  private val fontMap = mutableMapOf<Int, FontInfo>()

  var imageClickListener: ((src: String, width: Int, height: Int) -> Unit)? = null
  var anchorClickListener: ((href: String, content: String?) -> Unit)? = null

  fun callImageClick(src: String, width: Int, height: Int) {
    imageClickListener?.invoke(src, width, height)
  }

  fun callAnchorClick(href: String, content: String?) {
    anchorClickListener?.invoke(href, content)
  }

  /**
   * This function is used by the C++ layer to obtain text measurement information.
   * @param fontName The font name, if null it means system font
   * @param info Font information
   * @return Returns an IntArray of 5 elements { id, height, ascent, descent, x_height, 0_height }
   */
  fun createFont(fontName: String?, info: FontInfo): IntArray {
    // Create Paint for the current font
    info.paint.apply {
      isAntiAlias = true
      textSize = info.size.toFloat()
      // Underline
      isUnderlineText =
        (info.decorationLine and FontInfo.TextDecorationUnderline) != 0 && info.decorationStyle == FontInfo.TextDecorationStyleSolid
      // Strikethrough
      isStrikeThruText = (info.decorationLine and FontInfo.TextDecorationLineThrough) != 0
      // Font support
      setTypeface(
        Typeface.create(
          fontRegistry[fontName]?.value, when (info.weight) {
            600, 700 -> if (info.italic) Typeface.BOLD_ITALIC else Typeface.BOLD
            else -> if (info.italic) Typeface.ITALIC else Typeface.NORMAL
          }
        )
      )
    }
    return IntArray(7).apply {
      info.paint.getFontMetricsInt().let {
        val emphasisSpace = emphasisDescent(info)
        this[0] = fontId++
        this[1] = it.ascent.absoluteValue + it.descent.absoluteValue + it.leading + emphasisSpace
        this[2] = it.ascent
        // Extra bottom space for emphasis characters
        this[3] = it.descent + emphasisSpace
        this[4] = Rect().apply { info.paint.getTextBounds("x", 0, 1, this) }.height()
        this[5] = Rect().apply { info.paint.getTextBounds("0", 0, 1, this) }.height()
      }
      fontMap[this[0]] = info
    }
  }

  /**
   * Remove a font object
   */
  fun deleteFont(fontId: Int) {
    fontMap.remove(fontId)
  }

  /**
   * Measure text width
   */
  fun measureTextWidth(fontId: Int, text: String): Float = fontMap[fontId]?.paint?.measureText(text) ?: 0f

  /**
   * Load an image
   */
  fun loadImage(url: String) = imageManager.loadImage(url, relayout)

  /**
   * Get image size
   */
  fun getImageSize(url: String) = imageManager.getImageSize(url)

  /**
   * Draw background color
   */
  fun drawBackgroundColor(canvas: Canvas, rect: Rect, color: String, borderRadius: FloatArray) = transform(canvas) {
    val paint = Paint().apply {
      strokeWidth = 0f
      isAntiAlias = true
      style = Paint.Style.FILL
      setColor("#$color".toColorInt())
    }
    // Check if there are rounded corners
    if (borderRadius.max() > 0) {
      canvas.drawPath(Path().apply { addRoundRect(rect.toRectF(), borderRadius, Path.Direction.CW) }, paint)
    } else {
      canvas.drawRect(rect, paint)
    }
  }

  /**
   * Draw text
   */
  fun drawText(canvas: Canvas, rect: RectF, color: String, fontId: Int, text: String) = transform(canvas) {
    fontMap[fontId]?.let {
      val emphasisSpace = emphasisDescent(it)
      val paint = Paint(it.paint).apply { setColor("#$color".toColorInt()) }
      val baseline = rect.bottom - paint.fontMetricsInt.descent - emphasisSpace
      canvas.drawText(text, rect.left, baseline, paint)

      // Draw underline or strikethrough
      if (it.decorationLine > 0 && it.decorationStyle != FontInfo.TextDecorationStyleSolid) {
        paint.style = Paint.Style.STROKE
        paint.strokeWidth = max(paint.textSize * 0.1f, 0.2f)
        val path = Path()
        when (it.decorationStyle) {
          FontInfo.TextDecorationStyleWavy -> {
            paint.pathEffect = CornerPathEffect(4f)
            val d = it.paint.fontMetrics.descent.toInt()
            val base = baseline + d
            path.moveTo(rect.left, base)
            for (i in 0..ceil(rect.width() / d).toInt()) {
              path.lineTo(rect.left + i * d, if (i % 2 == 0) base else baseline)
            }
          }

          FontInfo.TextDecorationStyleDashed -> {
          }

          FontInfo.TextDecorationStyleDotted -> {
          }

          FontInfo.TextDecorationStyleDouble -> {
          }
        }

        canvas.drawPath(path, paint)
        paint.pathEffect = null
      }
      // Draw emphasis dots
      if (it.emphasis == FontInfo.TextEmphasisFilled) {
        paint.strokeWidth = 0f
        paint.style = Paint.Style.FILL
        var x = rect.left
        text.forEach { c ->
          val dx = paint.measureText(c.toString())
          canvas.drawCircle(x + dx / 2, baseline + emphasisSpace * 0.75f, max(paint.textSize * 0.1f, 0.2f), paint)
          x += dx
        }
      }
    }
  }

  /**
   * Draw list marker (e.g., ul)
   * @param type Marker type. 0: Circle; 1: Square; 2: Hollow circle;
   */
  fun drawListMarker(canvas: Canvas, rect: Rect, color: String, type: Int) = transform(canvas) {
    val pColor = "#$color".toColorInt()
    when (type) {
      0 -> {
        canvas.drawCircle(rect.centerX().toFloat(), rect.centerY().toFloat(), rect.width() / 2f, Paint().apply {
          setColor(pColor)
          strokeWidth = 0f
          style = Paint.Style.FILL
        })
      }

      1 -> {
        canvas.drawRect(rect, Paint().apply {
          setColor(pColor)
          strokeWidth = 0f
          style = Paint.Style.FILL
        })
      }

      2 -> {
        canvas.drawCircle(rect.centerX().toFloat(), rect.centerY().toFloat(), rect.width() / 2f, Paint().apply {
          setColor(pColor)
          strokeWidth = 1f
          style = Paint.Style.STROKE
        })
      }
    }
  }

  /**
   * Draw an image
   */
  fun drawImage(canvas: Canvas, rect: Rect, url: String, borderRadius: FloatArray) = transform(canvas) {
    imageManager.drawImage(canvas, rect, url, borderRadius)
  }

  /**
   * Draw border
   * @param borderRadius Border radius, starting from top-left clockwise
   * @param borderWidths Border widths, left-top-right-bottom
   * @param borderStyles Border styles, left-top-right-bottom. solid: Solid line; dashed: Dashed line; dotted: Dotted line; double: Double line
   * @param borderColors Border colors, left-top-right-bottom
   */
  fun drawBorder(
    canvas: Canvas,
    rect: RectF,
    borderRadius: FloatArray,
    borderWidths: FloatArray,
    borderStyles: Array<String>,
    borderColors: Array<String>
  ) = transform(canvas) { BorderUtil.drawBorder(canvas, rect, borderRadius, borderWidths, borderStyles, borderColors) }

  /**
   * Set clipping region
   */
  fun setClip(rect: RectF, borderRadius: FloatArray) {
    clips.add(Path().apply { addRoundRect(rect, borderRadius, Path.Direction.CW) })
  }

  fun clearClip() {
    clips.removeLastOrNull()
  }

  fun release() {
    clips.clear()
    fontMap.clear()
    imageManager.release()
  }

  private fun transform(canvas: Canvas, runnable: () -> Unit) {
    clips.lastOrNull()?.let { canvas.withClip(it) { runnable.invoke() } } ?: runnable.invoke()
  }

  private fun emphasisDescent(info: FontInfo) = if (info.emphasis == "filled") (info.size * 0.5f).toInt() else 0

  companion object {
    val fontRegistry = mutableMapOf<String, Lazy<Typeface>>()
  }
}
