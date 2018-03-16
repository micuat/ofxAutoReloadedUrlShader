# ofxAutoReloadedUrlShader

ofShader that loads from URL every 2 seconds. Based on [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader/). Useful for mobile development.

## Usage

Assuming that files are hosted as:

* http://192.168.1.3/shader/noise.vert
* http://192.168.1.3/shader/noise.frag

then,

```
shader.load("http://192.168.1.3/shader/noise");
```
