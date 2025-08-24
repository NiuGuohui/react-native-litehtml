import codegenNativeComponent from 'react-native/Libraries/Utilities/codegenNativeComponent';
import type { HostComponent, ViewProps } from 'react-native';
import type { DirectEventHandler, Int32 } from 'react-native/Libraries/Types/CodegenTypes';
import codegenNativeCommands from 'react-native/Libraries/Utilities/codegenNativeCommands';

export interface NativeLitehtmlViewProps extends ViewProps {
  content: { html: string; css?: string };
  /**
   * Triggered when the html layout changes
   */
  onHtmlLayout?: DirectEventHandler<{ width: Int32; height: Int32 }>;
  /**
   * Triggered when the image is clicked
   */
  onImageClick?: DirectEventHandler<{ src: string; width: Int32; height: Int32 }>;
  /**
   * Triggered when the anchor is clicked
   */
  onAnchorClick?: DirectEventHandler<{ href: string; content?: string }>;
  /**
   * Load svg content.
   */
  onLoadSVG?: DirectEventHandler<{ src: string }>;
}

export default codegenNativeComponent<NativeLitehtmlViewProps>('LitehtmlView');

export interface NativeImageCropViewCommands {
  svgLoaded: (ref: React.ElementRef<HostComponent<NativeLitehtmlViewProps>>, src: string, svgContent: string) => void;
}

export const Commands = codegenNativeCommands<NativeImageCropViewCommands>({
  supportedCommands: ['svgLoaded'],
});
