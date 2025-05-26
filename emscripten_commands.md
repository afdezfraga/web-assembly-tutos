# emscripten useful commands

## Init Emscripten

```bash
../emsdk/emsdk activate latest
source ../emsdk/emsdk_env.sh
```

## Run Emscripten html from local file

You can't just run the html file directly in the browser, you need to run a local server.
That is because the browser has security restrictions that prevent loading files directly from the file system.
We will use the `emrun` command to run a local server.

```bash
emrun <path_to_html_file>
```



