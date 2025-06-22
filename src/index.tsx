import Litehtml, { type LitehtmlInstance } from './NativeLitehtml';
import { createWorkletRuntime, runOnRuntime } from 'react-native-reanimated';
import type { FontDescription, FontMetrics } from './Font';

const createInstance = Litehtml.createInstance;

const runtime = createWorkletRuntime('NativeLitehtml');

setTimeout(() => {
  runOnRuntime(runtime, () => {
    'worklet';
    const i = createInstance(`<body><p>Niu<u>Guohui</u></p></body>`, '', new DocumentContainer()) as LitehtmlInstance;
    i.render(1000, 'render_all');
    console.log(i);
  })();
}, 1000);

export function multiply(): number {
  return 2;
}

class DocumentContainer {
  __workletClass = true;
  private fontId = 0;
  createFont(descr: FontDescription): FontMetrics {
    return {
      id: this.fontId++,
      fontSize: descr.fontSize,
      height: 12,
      // height: it.ascent.absoluteValue + it.descent.absoluteValue + it.leading + emphasisSpace,
      ascent: 12,
      descent: 12,
      xHeight: 12,
      chWidth: 12,
      drawSpaces: true,
      subShift: 12,
      superShift: 12,
    };
  }
}
