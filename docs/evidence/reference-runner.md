# Original macOS reference runner investigation

Date: 2026-10-13. Scope: a bounded primary-source investigation for ticket 01. Local governing inputs are `.scratch/macos-native-build/issues/01-native-build-runs-locally.md`, `.scratch/macos-native-build/spec.md`, and `docs/adr/0001-reference-release.md`. These pre-existing local planning files are untracked and are not included in this evidence commit. The binding requirement is a runnable original Mac reference, complete preservation matrix, hashed runtime fixtures, and justified fixed comparison limits before application porting.

## Result and evidence boundary

The original macOS reference has not executed. No compatible Intel machine or guest has been demonstrated. The reference/corpus gate remains open; application porting and numerical parity claims must wait for that gate. A compatible Intel Mac is the preferred candidate. Emulating an Intel macOS guest on Apple Silicon is a possible investigation path, not an established runner.

The selected archive and all regular-file hashes are recorded in [reference-inventory.json](reference-inventory.json); the [static receipt](ticket-01-reference-inventory.md) identifies the official archive and checksum sources. This investigation independently ran `file` and `otool -L` against the extracted original application and player. Both are i386 Mach-O executables. Both require `/Library/Frameworks/Python.framework/Versions/2.5/Python`, along with Carbon, AGL, OpenGL/GLU, QuickTime, and the other libraries listed by `otool`. An arbitrary Intel macOS installation is therefore insufficient: the precise dependencies, their i386 slices, and working drawing/input paths must be demonstrated.

## Host and runner constraints

- **Direct host execution:** Apple states that Mojave 10.14 was the last macOS release supporting 32-bit applications, and Catalina 10.15 removed that support. The current Apple Silicon/macOS 27.0.1 host is not a supported direct runner for this i386 release. This conclusion combines Apple's OS rule with the inspected binary architecture; a separate [host attempt](ticket-01-reference-inventory.md) returned `bad CPU type in executable` before Blender started. [Apple compatibility statement](https://support.apple.com/en-us/103076).
- **Native macOS virtualization:** UTM documents its macOS guest path as Apple Silicon/macOS 12+ virtualization. That path does not supply a legacy Intel system with 32-bit application support. [UTM macOS guest documentation](https://docs.getutm.app/guest-support/macos/).
- **Intel emulation:** UTM requires matching guest/host CPU architectures for virtualization; otherwise it uses emulation. QEMU documents TCG as its CPU emulation engine. An Intel guest on this ARM host consequently needs a cross-architecture emulation path, rather than Intel hardware acceleration. These capabilities do not prove that a particular old macOS installer, kernel, bootloader, or display driver works. [UTM architecture documentation](https://docs.getutm.app/settings-qemu/system/), [QEMU system introduction](https://www.qemu.org/docs/master/system/introduction.html).
- **Guest graphics:** UTM's QEMU display documentation says its VirGL acceleration requires Linux guest drivers. That documented capability does not establish legacy macOS AGL/OpenGL support. Software OpenGL might provide a viable path if the chosen macOS guest exposes it, but guest boot, context creation, editor drawing, input, and actual renders must all be observed. No such proof was obtained. [UTM display documentation](https://docs.getutm.app/settings-qemu/devices/display/).
- **Emulated concurrency:** UTM warns that forced multicore x86/i386-on-ARM emulation has correctness limitations. Preserve default single-vCPU operation for initial feasibility measurements; obtain independent thread-count evidence before using an emulated system as a multithreaded rendering oracle. [UTM CPU documentation](https://docs.getutm.app/settings-qemu/system/).

Host discovery during this task found no `qemu-system-x86_64`, `qemu-system-i386`, `qemu-system-ppc`, `utmctl`, `VBoxManage`, `prlctl`, `python2.5`, or `python2` on `PATH`. The standard `/Applications` locations for UTM, VirtualBox, and Parallels Desktop were absent. Common user VM directories were absent; the checked Documents directory supplied no `.utm`, `.pvm`, `.vdi`, `.vmdk`, or `.iso` candidates, and the inspected Downloads images were Windows 2000 and Omarchy images. These checks do not establish global absence of an emulator or VM. The UTM container Documents directory exists but enumeration failed with `PermissionError: [Errno 1] Operation not permitted`; its contents remain unknown. No software was installed and no guest was provisioned.

## Missing external inputs

1. Access to an Intel Mac or an already functioning Intel macOS guest that can execute i386 programs, with the exact OS/build, CPU, graphics implementation, interactive desktop access, and a clean test account identified. Mojave-or-earlier support is a necessary OS condition, not sufficient application compatibility evidence.
2. A compatible i386 Python 2.5 framework at the executable's recorded install name, with version, provenance, and hashes. It must load the original Blender Python API and extension modules; merely finding a `python2.5` command does not meet this requirement.
3. For a new emulator investigation: maintainer-supplied compatible macOS installation media or a known working guest image, plus its provenance and exact emulator/boot/display configuration. No working image or supported legacy macOS graphics configuration was identified here.
4. Reference-created `.blend` fixtures, retained outputs, interaction screenshots, and dependency-conditioned feature receipts. These must precede the complete hashed corpus and justified numerical limits required by the render-parity ADR.

The maintainer confirmed that no compatible Intel Mac or configured legacy macOS VM is available and authorized downloads from `*.blender.org`. The original Blender archive is already available and verified. That authorization does not supply a legacy macOS guest or its external Python framework. The missing input is a runnable compatible reference system, or compatible installation media plus the runtime needed for a bounded emulation experiment. Selecting a candidate starts verification; it does not close the reference gate.

## Verification commands for a supplied candidate

Run these commands **inside the candidate reference system**, in a clean account, using the complete unmodified extracted release. Replace the generic absolute paths with that system's paths. They are a proposed procedure, not executed receipts. Shell redirection records the application's own exit status without a pipeline masking it.

```sh
reference_root=/absolute/path/blender-2.45-darwin-8.10.1-i386
reference_blender="$reference_root/Blender.app/Contents/MacOS/blender"
reference_player="$reference_root/blenderplayer"
reference_receipts=/absolute/path/reference-receipts
reference_scene=/absolute/path/reference-fixtures/scene.blend
mkdir -p "$reference_receipts"
sw_vers > "$reference_receipts/os.txt"
uname -a > "$reference_receipts/kernel.txt"
file "$reference_blender" "$reference_player" > "$reference_receipts/architecture.txt"
otool -L "$reference_blender" > "$reference_receipts/blender-libraries.txt"
otool -L "$reference_player" > "$reference_receipts/player-libraries.txt"
file /Library/Frameworks/Python.framework/Versions/2.5/Python > "$reference_receipts/python-architecture.txt"
"$reference_blender" -v > "$reference_receipts/version.log" 2>&1
printf '%s\n' "$?" > "$reference_receipts/version.exit"
"$reference_blender" -h > "$reference_receipts/help.log" 2>&1
printf '%s\n' "$?" > "$reference_receipts/help.exit"
```

Run a Python API check against the reference-created scene. Blender's legacy CLI and API are documented by [creator.c](../../source/creator/creator.c) and [Blender.py](../../source/blender/python/api2_2x/doc/Blender.py). Require both the success marker and the recorded exit status: script errors can otherwise be missed.

```sh
cat > "$reference_receipts/python-smoke.py" <<'PY'
import sys
import Blender
assert sys.version_info[:2] == (2, 5), sys.version
assert Blender.Get('version') == 245
print 'REFERENCE_PYTHON_OK', sys.version, Blender.Get('version')
PY
"$reference_blender" -b "$reference_scene" -P "$reference_receipts/python-smoke.py" > "$reference_receipts/python.log" 2>&1
printf '%s\n' "$?" > "$reference_receipts/python.exit"
"$reference_blender" -b "$reference_scene" -o "$reference_receipts/still_" -F PNG -x 1 -t 1 -f 1 > "$reference_receipts/still.log" 2>&1
printf '%s\n' "$?" > "$reference_receipts/still.exit"
"$reference_blender" -b "$reference_scene" -o "$reference_receipts/animation_" -F PNG -x 1 -t 1 -s 1 -e 3 -a > "$reference_receipts/animation.log" 2>&1
printf '%s\n' "$?" > "$reference_receipts/animation.exit"
"$reference_blender" -p 100 100 1024 768 "$reference_scene" > "$reference_receipts/gui.log" 2>&1
printf '%s\n' "$?" > "$reference_receipts/gui.exit"
```

During the GUI run, record screenshots and actual input; perform F12 rendering, cancel and render again, edit/save/reopen, and normal exit. Separately launch the original player with a reference-created game fixture and record input, graphics, audio, and exit. Validate output dimensions/channels, hash inputs and outputs with an available SHA256 tool, and retain original logs plus any crash reports. `-v`, Python loading, or a background still render alone cannot establish Mac UX or full feature parity.

Repeat reference renders before locking the corpus. Following the local `docs/adr/0002-render-parity.md` requirement, document exact decoded-pixel behavior and justified cross-architecture limits per case; do not invent a tolerance from these proposed commands. Follow the local `docs/adr/0003-ux-boundary.md` requirement for application-content geometry and interaction measurements, separating OS chrome from Blender-owned controls.

## Investigation limitations

This investigation read official Apple, UTM, and QEMU documentation and inspected the original binary headers/dependencies and repository CLI/API documentation. It did not execute original Blender, boot an Intel guest, test graphics drivers, validate installation media, or benchmark emulation. No measured corpus, render tolerance, or runnable Mac reference was produced. The architecture and driver constraints narrow the next experiment; they do not prove that legacy macOS emulation is impossible.
