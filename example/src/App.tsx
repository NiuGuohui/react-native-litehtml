import { View, StyleSheet, ScrollView, StatusBar } from 'react-native';
import { LitehtmlView } from 'react-native-litehtml';

const richText = `
  <div style="width: 100px;height: 100px;background-color: green"></div>
  <p style="text-align: start">
    <span style="font-size: 24px; text-decoration: line-through">This is <u>test</u>.</span>
  </p>
  <a href="https://google.com">Google</a>
  <div>
    <p style="text-align: left">Bold text:</p>
    <p style="text-align: left">
      <strong>Example text for bold formatting.</strong>
      <img src="https://dev.w3.org/SVG/tools/svgweb/samples/svg-files/pencil.svg" style="height: 1000px;width: 50px;" />
    </p>
    <p style="text-align: left">Italic text:</p>
    <p style="text-align: left">
      <em>Example text for italic formatting.</em>
    </p>
    <p style="text-align: left">Underline text:</p>
    <p style="text-align: left">
      <u>Example text for underline formatting.</u>
    </p>
    <p style="text-align: left">Strikethrough text:</p>
    <p style="text-align: left">
      <s>Example text for strikethrough formatting.</s>
    </p>
    <p style="text-align: left">Emphasis text:</p>
    <p style="text-align: left">
      <span style="text-emphasis: filled;text-emphasis-position: under;">
        Example text for text-emphasis formatting.
      </span>
    </p>
    <p style="text-align: left">
      Superscript:
      <span style="font-size: 10.5pt">
        <span style="font-family: 'Times New Roman', serif">SP<sup>3&nbsp;&nbsp;</sup>&nbsp;</span></span
      >
      Subscript:
      <span style="font-size: 10.5pt"
        ><span style="font-family: 'Times New Roman', serif"
          ><span style="color: black">HNO<sub>3</sub></span></span
        ></span
      >
    </p>
    <p style="text-align: left">Ordered list:</p>
    <ol>
      <li style="text-align: left">Example list item one.</li>
      <li style="text-align: left">Example list item two.</li>
      <li style="text-align: left">Example list item three.</li>
    </ol>
    <p style="text-align: left">Unordered list:</p>
    <ul>
      <li style="text-align: left">Example unordered item one.</li>
      <li style="text-align: left">Example unordered item two.</li>
      <li style="text-align: left">Example unordered item three.</li>
    </ul>
    <p style="text-align: center">
      The above formats are all left-aligned, and this line is
      <span style="color: #e74c3c"><strong>centered</strong></span> text.
    </p>
    <p style="text-align: right">This line is right-aligned.</p>
    <p style="text-align: justify">This text is justified with equal spacing on both sides.</p>
    <p style="text-align: justify">Images:</p>
    <p style="text-align: justify">
      <img src="https://picsum.photos/200/300" style="width: 100px" />
    </p>
    <p style="text-align: justify">
      <img src="https://picsum.photos/900" />
    </p>
    <p>Table</p>
    <table border="1" cellpadding="1" cellspacing="1" style="width: 500px">
      <tbody>
        <tr>
          <td>
            <span style="font-size: 10.5pt">Option</span>
          </td>
          <td>
            <span style="font-size: 10.5pt">Experiment Operation and Phenomenon</span>
          </td>
          <td>
            <span style="font-size: 10.5pt">Experiment Conclusion</span>
          </td>
        </tr>
        <tr>
          <td>A</td>
          <td>Example experiment details here.</td>
          <td>Example conclusion here.</td>
        </tr>
        <tr>
          <td>B</td>
          <td>Example experiment details here.</td>
          <td>Example conclusion here.</td>
        </tr>
      </tbody>
    </table>
    <p style="text-align: left">Below is a horizontal line:</p>
    <hr />
    <p style="text-align: left">Next, text with different font settings:</p>
    <p style="text-align: left">1. Default font text example.</p>
    <p style="text-align: left">
      <span style="font-size: 10.5pt"><span style="font-size: 11pt">2. Song font text example.</span></span>
    </p>
    <p style="text-align: left">
      <span style="font-family: 'SimHei'"
        ><span style="font-size: 10.5pt"><span style="font-size: 11pt">3. Hei Ti font example.</span></span></span
      >
    </p>
    <p style="text-align: left">
      <span style="font-size: 12pt"
        ><span style="font-family: 'Microsoft YaHei'">4. Microsoft YaHei font example.</span></span
      >
    </p>
    <p style="text-align: left">&nbsp;</p>
    <p style="text-align: left">Different font sizes:</p>
    <p>1. Default text example.</p>
    <p>
      <span style="font-size: 8px">8px text example.</span>
    </p>
    <p>
      <span style="font-size: 10px">10px text example.</span>
    </p>
    <p>
      <span style="font-size: 24px">24px text example.</span>
    </p>
    <p>
      <span style="font-size: 48px">48px text example.</span>
    </p>
    <p>
      <span style="font-size: 72px">72px text example.</span>
    </p>
    <p>
      <span style="font-size: 18px">More styles: Wavy underline, solid underline, emphasis:</span>
    </p>
    <p><span style="font-size: 18px">Set different heading formats</span></p>
    <h1>This is Heading 1</h1>
    <h2>This is Heading 2</h2>
    <h3>This is Heading 3</h3>
    <pre>Formatted text example</pre>
    <p><span style="font-size: 18px">Font Colors</span></p>
    <p>
      <span style="color: #e74c3c"><span style="font-size: 18px">Red</span></span>
    </p>
    <p>
      <span style="color: #f39c12"><span style="font-size: 18px">Yellow</span></span>
    </p>
    <p>
      <span style="color: #27ae60"><span style="font-size: 18px">Green</span></span>
    </p>
    <p><span style="font-size: 18px">Background Colors</span></p>
    <p>
      <span style="background-color: #e74c3c">Red background text</span>
    </p>
    <p>
      <span style="background-color: #f39c12">Yellow background text</span>
    </p>
    <p>
      <span style="background-color: #2ecc71">Green background text</span>
    </p>
  </div>`;

const css = (str: TemplateStringsArray) => str.join('');

const style = css`
  :root {
    box-sizing: border-box;
    line-height: 1.5;
    font-size: 16px;
  }
  a:active {
    background-color: #f00;
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
    height: auto !important;
    display: inline-block;
    vertical-align: middle;
  }
`;

export default function App() {
  return (
    <View style={styles.container}>
      <View style={{ height: StatusBar.currentHeight }} />
      <ScrollView style={styles.container}>
        <View style={{ borderRadius: 100, overflow: 'hidden', borderWidth: 2, borderColor: '#F00' }}>
          <LitehtmlView
            css={style}
            html={richText}
            style={{ height: 3000 }}
            onLoadSVG={(src) => {
              console.log(src);
              return fetch(src).then((res) =>
                res.text().then((text) =>
                  text.replaceAll(/(width|height)=['"](.+?)(ex|em|px)['"]/gi, (substring) => {
                    const s = substring.replaceAll(/width|height|=|'|"/g, '');
                    const unit = s.match(/[a-z]+/)?.[0] ?? 'px';
                    let value = parseFloat(s);
                    if (unit === 'ex') value = value * (16 / 2);
                    else if (unit === 'em') value = value * 16;
                    if (substring.includes('height')) value *= 0.95;
                    return substring.replace(s, value.toString());
                  })
                )
              );
            }}
            onContentLayout={(width, height) => {}}
            onImageClick={(e) => {
              console.log(e.nativeEvent);
            }}
            onAnchorClick={(e) => {
              console.log(e.nativeEvent);
            }}
          />
        </View>
      </ScrollView>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
});
