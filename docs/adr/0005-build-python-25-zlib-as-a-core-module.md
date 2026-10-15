# Build Python 2.5 zlib as a core module

Status: accepted

## Context

Blender 2.45 requires bundled Python 2.5 and uses zlib. The macOS dependency script wrote a zlib entry to `Modules/Setup.local`, which contributes linker flags after Python's static-module boundary. It did not add zlib to the interpreter's built-in module table. The first configure/build pass also generated `Modules/config.c` while `Modules/Setup` changed in the same second, so Make could miss the timestamp update.

## Decision

Enable the existing zlib entry in Python 2.5's `Modules/Setup`. Remove stale `Modules/config.c`, run `make Modules/config.c` to regenerate module configuration, then build and install Python. Keep the pinned Python 2.5.6 source, patch and dependency hashes unchanged.

## Consequences

The dependency build's Python smoke check now proves zlib import and round-trip compression. Python's optional `_ctypes` and Tk modules remain outside Blender's required dependency set; their unavailable libffi/X11 prerequisites do not replace or weaken the zlib check.

## Evidence

The prior dependency run ended with `ImportError: No module named zlib`. Enabling the `Modules/Setup` entry, forcing configuration regeneration and rebuilding made this check pass:

```sh
python2.5 -c 'import sys, math, zlib, xml.parsers.expat; assert sys.version_info[:2] == (2, 5); assert zlib.decompress(zlib.compress("native")) == "native"; print sys.version'
```
