import type { ViewProps } from 'react-native';
import { Canvas } from '@shopify/react-native-skia';

interface LitehtmlViewProps extends ViewProps {
  html: string;
  css?: string;
  // onHtmlLayout?: (e: any) => void;
  // onImageClick?: (e: any) => void;
  // onAnchorClick?: (e: any) => void;
}

export function LitehtmlView(props: LitehtmlViewProps) {
  return <Canvas style={props.style} />;
}
