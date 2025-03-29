package com.ollie.litehtml

import android.graphics.Canvas
import android.graphics.DashPathEffect
import android.graphics.Paint
import android.graphics.Path
import android.graphics.PathDashPathEffect
import android.graphics.RectF
import androidx.core.graphics.toColorInt
import androidx.core.graphics.withClip
import kotlin.math.max

object BorderUtil {

  /**
   * Get the x-coordinate of the extended line at a target y-coordinate.
   */
  fun getExtendedLineXAtY(x1: Float, y1: Float, x2: Float, y2: Float, targetY: Float): Float {
    // Calculate the slope k of the line segment
    val k = (y2 - y1) / (x2 - x1)
    // Check if the slope is infinite (i.e., vertical line), if so, the x-coordinate is fixed
    if (x1 == x2) return x1 // The line is vertical, return the fixed x value
    // Use the point-slope form to solve for the x value corresponding to the target y value
    // y = k(x - x1) + y1 => x = (y - y1) / k + x1
    val xAtTargetY = (targetY - y1) / k + x1
    return xAtTargetY
  }

  /**
   * Fill the border with the specified type.
   * @param type Border type. dotted: dotted line dashed: dashed line double: double line
   */
  fun fillBorder(canvas: Canvas, type: String, outer: Path, inner: Path, width: Float, paint: Paint) {
    when (type) {
      "dotted" -> {
        canvas.drawPath(inner, paint.apply {
          style = Paint.Style.STROKE
          strokeWidth = width
          pathEffect = PathDashPathEffect(
            Path().apply { addCircle(width / 2, width / 2, width / 2, Path.Direction.CW) },
            width * 2,
            0f,
            PathDashPathEffect.Style.TRANSLATE
          )
        })
      }

      "dashed" -> {
        // Dashed line
        canvas.drawPath(inner, paint.apply {
          style = Paint.Style.STROKE
          strokeWidth = width * 10f
          pathEffect = DashPathEffect(floatArrayOf(width * 2, width), 0f)
        })
      }

      "double" -> {
        // Double line
        canvas.drawPath(outer, paint.apply {
          style = Paint.Style.STROKE
          strokeWidth = max(width * 0.7f, 1f)
        })
        canvas.drawPath(inner, paint.apply {
          style = Paint.Style.STROKE
          strokeWidth = max(width * 0.7f, 1f)
        })
      }

      // Solid line
      else -> {
        canvas.drawPath(outer, Paint(paint))
      }
    }
  }


  fun drawBorder(
    canvas: Canvas,
    rect: RectF,
    borderRadius: FloatArray,
    borderWidths: FloatArray,
    borderStyles: Array<String>,
    borderColors: Array<String>
  ) {
    val paint = Paint().apply {
      strokeWidth = 0f
      isAntiAlias = true
      style = Paint.Style.FILL
    }
    // Outer rectangle
    val outer = Path().apply { addRoundRect(rect, borderRadius, Path.Direction.CW) }
    // Inner rectangle
    val inner = Path().apply {
      addRoundRect(RectF(rect).apply {
        left += borderWidths[0]
        top += borderWidths[1]
        right -= borderWidths[2]
        bottom -= borderWidths[3]
      }, FloatArray(8) { borderRadius[it] - borderWidths[it % 4] }, Path.Direction.CW)
    }
    // Clipped complete border shape
    val path = Path().apply {
      // Draw outer rounded rectangle
      addRoundRect(rect, borderRadius, Path.Direction.CW)
      op(Path().apply {
        // Inner rectangle
        addRoundRect(RectF(rect).apply {
          left += borderWidths[0]
          top += borderWidths[1]
          right -= borderWidths[2]
          bottom -= borderWidths[3]
        }, FloatArray(8) { borderRadius[it] - borderWidths[it % 4] }, Path.Direction.CW)
      }, Path.Op.DIFFERENCE)
    }

    // Right-angled triangle with vertex at bottom-left
    val p1 = Path().apply {
      moveTo(rect.left, rect.top)
      val x = BorderUtil.getExtendedLineXAtY(
        rect.left, rect.top, rect.left + borderWidths[0], rect.top + borderWidths[1], rect.bottom
      )
      lineTo(x, rect.bottom)
      lineTo(rect.left, rect.bottom)
      close()
    }
    // Right-angled triangle with vertex at top-left
    val p2 = Path().apply {
      moveTo(rect.left, rect.bottom)
      val x = BorderUtil.getExtendedLineXAtY(
        rect.left, rect.bottom, rect.left + borderWidths[0], rect.bottom - borderWidths[3], rect.top
      )
      lineTo(x, rect.top)
      lineTo(rect.left, rect.top)
      close()
    }
    // Right-angled triangle with vertex at bottom-right
    val p3 = Path().apply {
      moveTo(rect.right, rect.top)
      val x = BorderUtil.getExtendedLineXAtY(
        rect.right, rect.top, rect.right - borderWidths[2], rect.top + borderWidths[1], rect.bottom
      )
      lineTo(x, rect.bottom)
      lineTo(rect.right, rect.bottom)
      close()
    }
    // Right-angled triangle with vertex at top-right
    val p4 = Path().apply {
      moveTo(rect.right, rect.bottom)
      val x = BorderUtil.getExtendedLineXAtY(
        rect.right, rect.bottom, rect.right - borderWidths[2], rect.bottom - borderWidths[3], rect.top
      )
      lineTo(x, rect.top)
      lineTo(rect.right, rect.top)
      close()
    }
    // Left border
    if (borderWidths[0] > 0) {
      paint.color = "#${borderColors[0]}".toColorInt()
      canvas.withClip(Path(path).apply { op(Path(p1).apply { op(p2, Path.Op.INTERSECT) }, Path.Op.INTERSECT) }) {
        fillBorder(this, borderStyles[0], outer, inner, borderWidths[0], Paint(paint))
      }
    }
    // Top border
    if (borderWidths[1] > 0) {
      paint.color = "#${borderColors[1]}".toColorInt()
      canvas.withClip(Path(path).apply {
        op(p1, Path.Op.DIFFERENCE)
        op(p3, Path.Op.DIFFERENCE)
        op(
          Path().apply { addRect(rect.left, rect.top, rect.right, rect.centerY(), Path.Direction.CW) },
          Path.Op.INTERSECT
        )
      }) {
        fillBorder(this, borderStyles[1], outer, inner, borderWidths[1], Paint(paint))
      }
    }
    // Right border
    if (borderWidths[2] > 0) {
      paint.color = "#${borderColors[2]}".toColorInt()
      canvas.withClip(Path(path).apply { op(Path(p3).apply { op(p4, Path.Op.INTERSECT) }, Path.Op.INTERSECT) }) {
        fillBorder(this, borderStyles[2], outer, inner, borderWidths[2], Paint(paint))
      }
    }
    // Bottom border
    if (borderWidths[3] > 0) {
      paint.color = "#${borderColors[3]}".toColorInt()
      canvas.withClip(Path(path).apply {
        op(p4, Path.Op.DIFFERENCE)
        op(p2, Path.Op.DIFFERENCE)
        op(
          Path().apply { addRect(rect.left, rect.centerY(), rect.right, rect.bottom, Path.Direction.CW) },
          Path.Op.INTERSECT
        )
      }) {
        fillBorder(this, borderStyles[3], outer, inner, borderWidths[3], Paint(paint))
      }
    }
  }
}
