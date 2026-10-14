# Native Apple Silicon alpha release

Category: enhancement
Status: ready-for-agent

## Scope

Deliver optimized native Blender 2.45, bundled Python 2.5.6, static dependencies where supported, stable GUI/fullscreen, and GitHub release `2026.10.14-alpha`. Game Engine/player excluded. Original-reference parity remains deferred.

## Implementation and validation

Fullscreen now redraws the existing context, keeps its exit control accessible, and restores frame/title. Dependency packaging resolves real dylib filenames, includes Python standard library/modules and licenses, moves runtime data into Resources with a legacy symlink, and ad-hoc signs modified code. The release ZIP preserves the app bundle and includes a portable checksum asset.

Four CTest checks pass, including five GHOST fullscreen cycles with pixel/frame/title assertions, legacy Python bundled-path/API checks, native dynamic loading, and TIFF write/read. Actual packaged Python/TIFF checks pass; 57 Mach-O files are arm64 with resolving bundle/system linkage. Real desktop startup, fullscreen entry/exit, and foreground render were exercised. Alpha is not notarized; SDL/TIFF stay bundled dynamic. Wider display/version and reference parity validation remain outside this release receipt.

## Review

Standards and spec reviews completed. Fixed Apple-only scope guards, portable checksum basename, and TIFF runtime-name mismatch. Final reviews found no further concrete defects.
