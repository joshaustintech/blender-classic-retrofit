# Blender Classic Retrofit

This project preserves Blender 2.45's capabilities and user experience on Apple Silicon macOS.

## Language

**Reference release**:
The official Blender 2.45 macOS Intel release with Python 2.5. It is the comparison target for existing features, interactions, scripting behavior, and rendering.
_Avoid_: latest Blender, CMake defaults

**Feature parity**:
Availability and equivalent behavior of every feature shipped in the reference release.
_Avoid_: core features only, compiles successfully

**UX parity**:
Preservation of Blender-owned interface and interaction behavior, including layouts, fonts, icons, dialogs, logical-pixel geometry, defaults, shortcuts, and workflows. Current macOS window frames and Dock appearance are permitted.
_Avoid_: modernized UI, approximate workflow

**Blender Internal**:
The renderer shipped within Blender 2.45 that generates images from its scenes, materials, lights, and render settings.
_Avoid_: Cycles, replacement renderer

**Render parity**:
Equivalent rendered output for the same scene and settings, judged against the reference release using strict measured tolerances fixed before porting. Numerical differences do not permit changed shading, defaults, or visibly different results.
_Avoid_: looks close enough, arbitrary image tolerance
