export interface FontDescription {
  fontFamily: string;
  fontSize: number;
  fontWeight: number;
  italic: boolean;
  decoration?: {
    color: string;
    overline: boolean;
    underline: boolean;
    lineThrough: boolean;
    style: 'dashed' | 'dotted' | 'double' | 'wavy' | 'max' | 'solid';
  };
  emphasis?: {
    style: string;
    color: string;
    over: boolean;
    under: boolean;
    left: boolean;
    right: boolean;
  };
}

export interface FontMetrics {
  id: number;
  // Font size in pixels. The same as size argument of the create_font function
  fontSize: number;
  // Font height in pixels.
  height: number;
  // The distance from the baseline to the top of a line of text.
  ascent: number;
  // The distance from the baseline to the bottom of a line of text.
  descent: number;
  // Height of the symbol x
  xHeight: number;
  // Height of the symbol 0
  chWidth: number;
  // True to call draw text function for spaces. If False, just use space width without draw.
  drawSpaces: boolean;
  // The baseline shift for subscripts.
  subShift: number;
  // The baseline shift for superscripts.
  superShift: number;
}
