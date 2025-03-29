import { default as NativeLitehtmlView, type NativeLitehtmlViewProps } from './LitehtmlViewNativeComponent';
import { StyleSheet, type ViewProps } from 'react-native';
import { useCallback, useMemo, useState } from 'react';
export { NativeLitehtml } from './NativeLitehtml';

export interface LitehtmlViewProps extends ViewProps, Pick<NativeLitehtmlViewProps, 'onAnchorClick' | 'onImageClick'> {
  /**
   * html string
   */
  html: string;
  /**
   * css string
   */
  css?: string;
}

/**
 * Render HTML by litehtml.
 * It will be auto height and full width if you don't set width/height style.
 */
export function LitehtmlView(props: LitehtmlViewProps) {
  const [height, setHeight] = useState(0);

  return (
    <NativeLitehtmlView
      {...props}
      collapsable={false}
      style={StyleSheet.compose({ width: '100%', height }, props.style)}
      onHtmlLayout={useCallback((e: any) => setHeight(e.nativeEvent.height), [])}
      content={useMemo(() => ({ html: props.html, css: props.css }), [props.html, props.css])}
    />
  );
}
