# Blender 2.45 Reference Evidence

## Provenance

The selected reference is `blender-2.45-OSX-10.3-py2.5-intel.tar.bz2`, retrieved through Blender's [official mirror service](https://mirror.blender.org/release/Blender2.45/blender-2.45-OSX-10.3-py2.5-intel.tar.bz2). Its published checksum comes from the [official manifest](https://mirror.blender.org/release/Blender2.45/release245.md5).

| Artifact | Verified value |
| --- | --- |
| Archive size | 10,823,668 bytes |
| Published and measured archive MD5 | `d8663ceabb3eae88dd2e2c86a632e232` |
| Measured archive SHA256 | `5cc3f1ea2481bcc191626196f06cd21a92952e835a57c854b5974a8523736b46` |
| Blender executable SHA256 | `2c19298f4a62cbe29221f742f2e6f4c8992df5696b4e9042f81e03943352408e` |
| Player executable SHA256 | `2a466450804f0e9e05d63fc888d450a6d97d4f25d72bdc4c4b9fc5761631c86b` |

Archive root: `blender-2.45-darwin-8.10.1-i386/`. Blender is at `Blender.app/Contents/MacOS/blender`; player is at `blenderplayer`.

`file` and `otool -hv` identify both executables as 32-bit i386 Mach-O. `otool -L` identifies an external Python 2.5 framework dependency and legacy Carbon, AGL, QuickTime, OpenGL/GLU, and audio frameworks. The archive contains no Python framework.

Neither original executable has been run. Modern macOS dropped [32-bit application execution](https://support.apple.com/en-us/103076) and removed the [QuickTime framework](https://developer.apple.com/documentation/macos-release-notes/macos-catalina-10_15-release-notes). A compatible reference system with Python 2.5 is therefore a prerequisite for original macOS behavior measurements. No local reference VM or compatible machine has been verified.

## Resource inventory

The archive supplies 147 script/helper files; this repository supplies 135. Of their 135 common files, 106 are byte-identical and 29 differ. These 29 differences have not been reviewed for behavioral significance.

The twelve archive-only files are Collada import/export scripts and their helper package; see `legacy-opengl-probe.md` for the exact list. This is a concrete packaging parity gap requiring reconciliation.

Original scripts reside under `Contents/MacOS/.blender/scripts/`; locale data and `.Blanguages` reside under `Contents/Resources/`. Startup data and fallback font data are embedded in repository source; absence of corresponding standalone archive files is not evidence of missing behavior. Resource verification must cover both packaged and embedded data.

## Platform donor research

Later upstream Cocoa work is a candidate source for a GHOST adapter, not a verified drop-in replacement. The [first pure Cocoa history entry](https://projects.blender.org/archive/blender-archive/commits/commit/49e780d30a906c8d755e75a58b9e9ffab572546e/source/blender/windowmanager) records missing fullscreen, font problems, SDL dependencies on Carbon, and coordinate changes. [Later history](https://projects.blender.org/archive/blender-archive/commits/commit/485d1ef06ebd3d25945b14f7987ce84016b5d311/source/blender/windowmanager?page=4) adds document and input handling but also records focus, hang, and swap defects. Donor commits must be audited and their adaptation proved against the original interface and workflows.
