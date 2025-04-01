# react-native-litehtml

![NPM Version](https://img.shields.io/npm/v/react-native-litehtml)

A fast simple html renderer. The porting of [LiteHTML](https://github.com/litehtml/litehtml).

For supported features, please refer to the content in the [LiteHTML](https://github.com/litehtml/litehtml) repository.
The current version has not yet implemented all litehtml APIs (such as gradient backgrounds).
Additionally, support for svg images has been added (only `img` links are supported, `<svg>` is not supported yet).

**Currently, only supports the android platform and new architecture.**

## Installation

```sh
npm install react-native-litehtml
```

## Usage

`LitehtmlView` will auto fill the width of the parent and automatically raise the height.

```js
import { LitehtmlView } from 'react-native-litehtml';

// ...
const style = `
  :root {
    box-sizing: border-box;
    line-height: 1.5;
    font-size: 16px;
  }
  table {
    margin: 10px 0;
    border-collapse: collapse;
    display: table;
    text-indent: 0;
  }
  td {
    padding: 5px 10px;
    border: 1px solid #ddd;
  }
  sup,
  sub {
    font-size: 75%;
  }
  hr {
    border-color: #e5e7eb;
  }

  img {
    max-width: 100%;
    display: inline-block;
    vertical-align: middle;
  }
`;

const richText = `
<p style="text-align: start">
    <span style="font-size: 24px; text-decoration: line-through">This is <u>test</u>.</span>
</p>
`;

<LitehtmlView css={style} html={richText} />;
```

Look [example](example/src/App.tsx) for more details.

### LitehtmlView

The component supports all ViewProps. The specific Props are as follows:

| Prop          | Type                                                                             | Default   | Description                          |
| ------------- | -------------------------------------------------------------------------------- | --------- | ------------------------------------ |
| css           | string                                                                           | ''        | The css style.                       |
| html          | string                                                                           | ''        | The html content.                    |
| onImageClick  | (event: { nativeEvent: { src: string; width: number; height: number } }) => void | undefined | Callback when the image is clicked.  |
| onAnchorClick | (event: { nativeEvent: { href: string, content?: string } }) => void             | undefined | Callback when the anchor is clicked. |

### NativeLitehtml

`NativeLitehtml` can be used to register fonts before rendering.

```ts
import { NativeLitehtml } from 'react-native-litehtml';

// Register fonts in rendering engine.
NativeLitehtml.registerFont(["Microsoft YaHei"], '/data/.../fonts/YaHei.ttf');
// Register fonts in rendering engine from asset.
NativeLitehtml.registerFontFromAsset(["Microsoft YaHei"], 'fonts/YaHei.ttf');

...
```

## ToDo

- [ ] Migration to using pure JS + C++ + Skia rendering
- [ ] Add other implementations of LiteHTML
- [ ] iOS support

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
