package com.ollie.litehtml

import android.graphics.Paint

data class FontInfo(
  val size: Float,
  val weight: Int,
  val italic: Boolean,
  val decorationLine: Int,
  val decorationStyle: Int,
  val decorationColor: String,
  val emphasis: String?,
  val emphasisColor: String,
) {
  val paint = Paint()

  companion object {
    const val TextDecorationUnderline = 0x01
    const val TextDecorationOverline = 0x02
    const val TextDecorationLineThrough = 0x04

    const val TextEmphasisFilled = "filled"
    const val TextEmphasisDot = "dot"
    const val TextEmphasisTriangle = "triangle"

    const val TextDecorationStyleSolid = 0x00
    const val TextDecorationStyleDouble = 0x01
    const val TextDecorationStyleDotted = 0x02
    const val TextDecorationStyleDashed = 0x03
    const val TextDecorationStyleWavy = 0x04
    const val TextDecorationStyleMax = 0x05
  }
}
