# muteki

EABI code execution on Besta® RTOS devices.

This repo contains `libmuteki-shims`, an unofficial, free as in freedom syscall shims and headers for working with Besta® RTOS syscalls, and `libmuteki-osdep`, an EABI bridge library for Besta® RTOS applets and modules.

## Integrating muteki into a C compiler

> [!TIP]
> When bootstrapping muteki toolchains (e.g. `arm-none-bestaeabi`), it's advised to build and install the shims using a bootstrap C compiler before building the libc and the final compiler, since libc depends on it.

```sh
meson setup --cross-file meson-cross/<your desired cross file here> build
meson compile -C build
sudo meson install -C build
```

## Integrating muteki syscall definitions into Ghidra

Run `python scripts/gen_ghidra_prf.py <path-to-your-ghidra-user-dir>/parserprofiles/muteki-shims.prf` to generate a parser profile named `muteki-shims.prf`, and use `File -> Parse C Source...` to import the header files using that generated  parser profile.

> [!NOTE]
> The parser profile needs to be regenerated when the file names or layout under `include/` changes, or the project root directory is moved or renamed. Failure to do so may cause unexpected behaviors during import.

## Developing muteki using clangd

Generate a fresh build directory named `builddir/` and specify `--query-driver=/path/to/arm-none-bestaeabi-gcc` in the clangd command line to get started.

clangd [does not support non-stand-alone (headers-only) files](https://github.com/clangd/clangd/issues/45) at this moment. To make the language server work properly, open the test file under `src/test.c` first before opening any of the header files. Fail to do so may cause the language server to output bogus errors on how it cannot find system headers.
