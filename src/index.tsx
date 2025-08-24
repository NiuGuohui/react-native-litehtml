import { default as NativeLitehtmlView, Commands, type NativeLitehtmlViewProps } from './LitehtmlViewNativeComponent';
import { type NativeSyntheticEvent, StyleSheet, type ViewProps } from 'react-native';
import { useCallback, useMemo, useRef } from 'react';
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
  /**
   * Triggered when the html layout
   */
  onContentLayout?: (width: number, height: number) => void;
  /**
   * Load svg content.
   */
  onLoadSVG?: (src: string) => Promise<string>;
}

/**
 * Render HTML by litehtml.
 * It will be auto height and full width if you don't set width/height style.
 */
export function LitehtmlView(props: LitehtmlViewProps) {
  const ref = useRef<any>(null);
  const { html, css, onContentLayout, onLoadSVG } = props;

  return (
    <NativeLitehtmlView
      {...props}
      ref={ref as any}
      collapsable={false}
      content={useMemo(() => ({ html, css }), [html, css])}
      style={StyleSheet.compose({ width: '100%' }, props.style)}
      onHtmlLayout={useCallback(
        (e: NativeSyntheticEvent<{ width: number; height: number }>) =>
          onContentLayout?.(e.nativeEvent.width, e.nativeEvent.height),
        [onContentLayout]
      )}
      onLoadSVG={useCallback(
        (e: NativeSyntheticEvent<{ src: string }>) => {
          const src = e.nativeEvent.src;
          onLoadSVG?.(src)?.then((content) => Commands.svgLoaded(ref.current, src, content));
        },
        [onLoadSVG]
      )}
    />
  );
}
