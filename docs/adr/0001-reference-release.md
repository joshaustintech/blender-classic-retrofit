# Use the official macOS Intel/Python 2.5 release as the reference

The parity baseline is the official Blender 2.45 macOS Intel release with Python 2.5, including every shipped feature. This was chosen over the Python 2.3 variant and the repository's CMake defaults so that preservation has one concrete comparison target; CMake currently disables features enabled in the legacy macOS configuration. The archived binary, scripts, resources, and observed behavior must establish the reference corpus before retrofit changes can claim parity.
