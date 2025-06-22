import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';
import type { UnsafeObject } from 'react-native/Libraries/Types/CodegenTypes';

export interface LitehtmlInstance {
  render: (maxWidth: number, renderType: 'render_all' | 'render_fixed_only' | 'render_no_fixed') => void;
  draw: (width: number, height: number) => void;
}

interface Spec extends TurboModule {
  createInstance: (html: string, css: string, container: UnsafeObject) => UnsafeObject;
}

export default TurboModuleRegistry.getEnforcing<Spec>('Litehtml');
