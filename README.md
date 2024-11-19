# imApp

A simple wrapper around sokol_app, sokol_gfx, sokol_imgui, and sokol_log to make it easier to create simple applications with sokol libraries.

## Clone:

```bash
> git clone https://github.com/BenMcAvoy/imApp.git
> cd imApp
```

## Modify:
```
> edit src/app.cpp # modify DrawCallback and other code
```

## Build:

```bash
> cmake -B build
> cmake --build build
```

> [!NOTE]
> On Linux you'll also need to install the 'usual' dev-packages needed for X11+GL development.

## Build and Run WASM/HTML version via Emscripten (Linux, macOS)

Setup the emscripten SDK as described [here](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions).

Don't forget to run `source $HOME/emsdk/emsdk_env.sh` to set up the environment.
And then in the directory of your project:

```
emcmake cmake -DCMAKE_BUILD_TYPE=MinSizeRel -B build-wasm
cmake --build build-wasm
```

To run the compilation result in the system web browser:

```
> emrun demo.html
```

## IDE Integration:
- You must configure the project using CMake (`cmake -B build`) to get the LSP to function, this may be done automatically by some editors. The reason for this is CMake is configured to export commands to tell the LSP where the files are.
