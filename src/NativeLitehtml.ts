import { type TurboModule, TurboModuleRegistry } from 'react-native';

interface Spec extends TurboModule {
  /**
   * Register fonts in rendering engine.
   * @param fontName font name
   * @param fontPath font path
   */
  registerFont: (fontName: string[], fontPath: string) => void;
  /**
   * Register fonts in rendering engine from asset.
   */
  registerFontFromAsset: (fontName: string[], inAssetsPath: string) => void;
}

export const NativeLitehtml = TurboModuleRegistry.getEnforcing<Spec>('NativeLitehtml');
