<h1 align="center"><img alt="SoLoBoP" src="https://raw.githubusercontent.com/gist/v-atamanenko/e547f3f0667e4684ca665b11a7ce1e3c/raw/6dd7ce900576ae334cda1886095c09a784663507/solobop_logo.svg"></h1>
<p align="center">
  <a href="#what-is-this">What is this</a> •
  <a href="#prerequisites">Prerequisites</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#project-structure">Project Structure</a> •
  <a href="#notes-on-internals">Notes on Internals</a> •
  <a href="#build">Build</a> •
  <a href="#credits">Credits</a> •
  <a href="#license">License</a>
</p>

**solobop** (**so**-**lo**ader **bo**iler**p**late) – a starter kit for Android-to-PSVita ports based on [TheFloW's so_util](https://github.com/Rinnegatamante/so_util).

## How it works

The Vita runs on an ARMv7 CPU which is also the primary architecture of Android devices from around 2012 to 2019. This means native Android game binaries (`.so` files) can, with the right scaffolding, be executed directly on the Vita. This project provides such scaffolding.

The core idea is similar to that of [WINE](https://www.winehq.org/): instead of emulating Android, we intercept the game's OS-facing calls and redirect them to native equivalents and/or reimplementations. The `.so` itself runs as-is at the CPU level.

In practice this means:
- Loading the `.so` into memory, applying relocations, and resolving its imports against Vita-native implementations (via [so_util](lib/so_util))
- Providing a fake JVM/JNI environment so the game can do its Java-side initialization (via [FalsoJNI](lib/falso_jni))
- Reimplementing Android-specific interfaces (mostly NDK-adjacent things) in ways that work on the Vita

### Projects built on top of Solobop

* [Baldur's Gate: Dark Alliance Port by Nevak](https://github.com/Nevak/bgda-vita)
* [Modern Combat 3 Port by gl33ntwine](https://github.com/v-atamanenko/mc3-vita)
* [SoulCalibur Port by Rinnegatamante](https://github.com/Rinnegatamante/soulcalibur_vita)
* [Geometry Dash Port by hatoving](https://github.com/hatoving/gdash-vita)
* [Lego Star Wars: The Complete Saga Port by gm666q](https://github.com/gm666q/lswtcs-vita)
* [Hill Climb Racing Port by MemoryHunter](https://github.com/memory-hunter/hill-climb-racing-vita)
* [NBA Jam Port by elliencode](https://github.com/elliencode/nba-jam-psv)
* And more.


## Prerequisites

- [VitaSDK-softfp](https://github.com/vitasdk-softfp#how-to-install) installed and `VITASDK` env var pointing to it.
- [kubridge](https://github.com/bythos14/kubridge) - kernel plugin required for RWX memory; must be installed on the target Vita.
- Familiarity with C and basic reverse engineering (you'll need to figure out what the `.so` expects).
- The target game's `.so` extracted from its APK.

If you're new to Vita homebrew development in general, start with [Getting Started with PSVita Homebrew Development](https://web.archive.org/web/20250216192850/https://gl33ntwine.com/posts/develop-for-vita/) first.

## Getting Started

### 0. Use the template (optional)

Press the big green button "Use this template" to create your own repository, if you wish. In that case, replace the repo URL with yours in the steps below.

### 1. Clone with submodules

```sh
git clone --recurse-submodules https://github.com/v-atamanenko/soloader-boilerplate
```

Or if you already cloned without them:

```sh
git submodule update --init --recursive
```

### 2. Configure `CMakeLists.txt`

At the top of the file, set the basics for your port:

```cmake
set(VITA_APP_NAME "My Game Port")
set(VITA_TITLEID  "MYGM00001")      # Must be unique on the device
set(VITA_VPKNAME  "my_game")

set(DATA_PATH "ux0:data/mygame/" CACHE STRING "Path to data (with trailing /)")
set(SO_PATH "${DATA_PATH}libmain.so" CACHE STRING "Path to .so")
```

`DATA_PATH` is where users place the game's data files; `SO_PATH` is the Android `.so` within that folder.

There are also a few toggleable options:

| Option | Default | Description |
|---|---|---|
| `USE_SCELIBC_IO` | ON | Use SceLibcBridge for IO. Recommended for most ports. |
| `SHADER_FORMAT` | GLSL | Shader format: `GLSL`, `CG`, or `GXP`. |
| `DUMP_COMPILED_SHADERS` | ON | Cache compiled shaders on disk (GLSL/CG only). |

### 3. Fill in `dynlib.c`

This is the heart of the port - a symbol table that maps the `.so`'s imported symbols to their Vita-native equivalents. The boilerplate already includes a large set of common libc, math, OpenGL and other symbols. Your job is to handle whatever the specific game needs on top of that.

Run the loader, watch the logs for unresolved symbol warnings, and add them. The pattern is simple:

```c
// in the dynlib array:
{ "someFunction", (uintptr_t)someFunction_impl },
```

Watch out for Bionic/newlib struct size mismatches. The most common ones (pthread, stat, dirent, etc.) are already handled in `source/reimpl/` but there can be more.

### 4. Fill in `java.c`

This is the FalsoJNI implementation file - the fake JVM that the game talks to at startup (and sometimes during runtime). Start with empty arrays as they are now, run the loader, and look for `[GetMethodID]` / `[GetFieldID]` errors in the logs. For each one, implement the corresponding method or field.

See [FalsoJNI's README](lib/falso_jni/README.md) for the full details on how to implement methods and fields.

### 5. Fill in `patch.c`

Game-specific patches go here, if needed. These typically include:
- Skipping license or DRM checks
- Working around code that uses unsupported Android APIs
- Fixing crashes caused by platform differences

Use `hook_addr()` with either a named export or a raw address offset. See [so_util's README](lib/so_util/README.md) for the hooking API.

### 6. Wire up controls in `main.c`

The boilerplate sets up a controls loop that polls buttons, touch, and analog sticks and calls three handler stubs:

```c
void controls_handler_key(int32_t keycode, ControlsAction action);
void controls_handler_touch(int32_t id, float x, float y, ControlsAction action);
void controls_handler_analog(ControlsStickId which, float x, float y, ControlsAction action);
```

Implement these to call whatever input API the game exposes. `keycode` uses Android's `AKEYCODE_*` constants (defined in `controls.h`) so the game doesn't know the difference.

Also add your main rendering loop:

```c
int (* JNI_OnLoad)(void *jvm) = (void *)so_symbol(&so_mod, "JNI_OnLoad");
JNI_OnLoad(&jvm);

gl_init();

// ... any other init the game needs

while (1) {
    controls_poll();
    // ... call into the .so to render a frame
    gl_swap();
}
```

## Project Structure

```
source/
├── main.c              Entry point, main loop, controls handlers
├── dynlib.c            Symbol resolution table
├── java.c              FalsoJNI implementation (JNI methods and fields)
├── patch.c             Game-specific hooks and patches
├── reimpl/             Android-compat reimplementations
│   ├── controls.c/h    Input handling (buttons, touch, analog)
│   ├── egl.c/h         EGL surface/context stubs
│   ├── errno.c/h       Bionic errno translation
│   ├── io.c/h          File I/O wrappers
│   ├── log.c/h         Android log to Vita log bridge
│   ├── mem.c/h         Memory-related wrappers
│   ├── pthr.c/h        pthread struct size compatibility wrappers
│   ├── sys.c/h         Misc syscall wrappers
│   ├── time64.c/h      64-bit time functions
│   └── asset_manager   Android AssetManager reimplementation
└── utils/
    ├── dialog.c/h      Fatal error dialog (SceCommonDialog)
    ├── glutil.c/h      vitaGL init, gl_swap, shader compilation hooks
    ├── init.c/h        soloader_init_all() / the full init sequence
    ├── logger.c/h      Logging macros
    ├── settings.c/h    Persistent settings (load/save/reset)
    └── utils.c/h       A bunch of useful helpers

lib/
├── falso_jni/          Fake JVM/JNI interface (submodule)
├── so_util/            ELF loader and hooking (submodule)
├── libc_bridge/        SceLibcBridge stubs for IO compatibility
├── fios/               FIOS2 file I/O overlay
├── kubridge/           kubridge headers (submodule)
└── sha1/               SHA-1 implementation
```

## Notes on Internals

### `soloader_init_all()`

Everything before `main()` calls into the game is handled by `soloader_init_all()` in `source/utils/init.c`. In order, it:

1. Checks for a `-config` launch parameter and redirects to a separate configurator binary if present
2. Sets CPU/GPU/bus clock frequencies to max (444/222/222/166 MHz)
3. Initializes FIOS if `USE_SCELIBC_IO` is set
4. Checks that kubridge is loaded
5. Checks that the `.so` file exists
6. Loads and relocates the `.so`
7. Loads settings
8. Resolves imports (`dynlib.c`)
9. Applies patches (`patch.c`)
10. Flushes caches
11. Runs init arrays
12. Pre-initializes vitaGL
13. Initializes FalsoJNI
14. Starts the controls thread

### Shader support

The boilerplate hooks `glShaderSource` and `glCompileShader` to intercept GLSL shaders from the game. Depending on `SHADER_FORMAT`:

- **GLSL** - passed directly to VitaGL for runtime GLSL translation (or dumping).
- **CG** - loaded from disk as `.cg` sources if you wish to translate them manually.
- **GXP** - loaded from disk as pre-compiled binary `.gxp` files.

When `DUMP_COMPILED_SHADERS` is enabled (GLSL/CG), compiled shader binaries are written to `DATA_PATH` on first run and reused on subsequent launches, saving startup time.

### Settings

`source/utils/settings.c` provides a simple persistent settings system backed by a binary file at `DATA_PATH/settings.bin`. The boilerplate includes two sample settings (`setting_sampleSetting`, `setting_sampleSetting2`); replace these with whatever your port actually needs.

Settings are meant to be exposed through a separate configurator app launched from the Vita's LiveArea. The boilerplate handles the launch redirect automatically but does not provide the configurator app. You can see an example of such in Mass Effect Infiltartor or BackStab HD ports.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This produces `build/VpkNameYouChose.vpk`. Install it on the Vita like any other homebrew, then copy the game data to `DATA_PATH`.

### Useful CMake targets

| Target | What it does |
|---|---|
| `send` | Upload `eboot.bin` to the Vita over FTP and relaunch |
| `send_kvdb` | Same, but launches with KVDB debug enabled |
| `dump` | Pull a crash dump from the Vita and run through vita-parse-core |
| `reboot` | Reboot the Vita over network |

Set `PSVITAIP` to your Vita's IP for these to work.

### Debug build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Debug builds enable `DEBUG_SOLOADER` logging throughout the loader. You can also uncomment lines in `CMakeLists.txt` to enable verbose FalsoJNI logging (`FALSOJNI_DEBUGLEVEL=0`) or OpenGL call logging (`DEBUG_OPENGL`).

## Credits

- [TheFloW](https://github.com/TheOfficialFloW) - original so-loader, gtasa_vita
- [Rinnegatamante](https://github.com/Rinnegatamante) - VitaGL, numerous ports and contributions
- [fgsfds](https://github.com/fgsfds) - various contributions

## License

This software may be modified and distributed under the terms of the MIT license. See the [LICENSE](LICENSE) file for details.
