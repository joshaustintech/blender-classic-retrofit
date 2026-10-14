# Ticket 01 reference inventory receipt

Date: 2026-10-13. Source: `758a5a10e3a088eed05ebacf4e527b6c626a532f` (`v2.45`).

## Result

The first acceptance gate remains open. No original Mac reference has executed, no reference-created fixture corpus exists, and no comparison limits have been justified or locked. Application porting must wait for these prerequisites in the accepted local plan, `.scratch/macos-native-build/spec.md`. The plan, ticket, and ADRs predate this work and remain untracked local inputs; this commit includes prerequisite evidence only. The [runner investigation](reference-runner.md) records the missing system/runtime inputs and the bounded emulator research.

Static work completed:

- Reverified the official archive's SHA256 and published MD5, plus both executable hashes against the existing local provenance receipt (`docs/evidence/reference-release.md`). The archive is the [official Intel/Python 2.5 release](https://mirror.blender.org/release/Blender2.45/blender-2.45-OSX-10.3-py2.5-intel.tar.bz2); its published MD5 `d8663ceabb3eae88dd2e2c86a632e232` was rechecked against a freshly downloaded copy of the [official checksum manifest](https://mirror.blender.org/release/Blender2.45/release245.md5). The measured SHA256 is `5cc3f1ea2481bcc191626196f06cd21a92952e835a57c854b5974a8523736b46`.
- Inventoried all 208 regular files in the archive: relative paths, sizes, and SHA256 hashes are in [reference-inventory.json](reference-inventory.json). The archive has 268 members, including 60 directories and no links or other special members. This is an asset inventory, not a hashed runtime fixture corpus or a complete feature preservation matrix.
- Compared all 147 archived script/helper files with the 135 repository files. The 106 identical files match byte for byte. Full unified diffs of the 29 differing files contain only expanded SVN `$Id: ... $` comment lines versus repository `$Id$` lines. The manifest retains both original hashes and classifies those files as `svn_id_comment_only`; no executable source difference was observed in those 29 comparisons. This does not demonstrate that the scripts run with a native Python runtime.
- Identified all twelve archive-only Collada files through the manifest's `archive_only` entries. They remain a packaging parity gap. No scripts or application code have been replaced during this prerequisite audit.

## Executed host checks

Host: arm64, macOS 27.0.1 (`26A434`). All commands below are shown with generic temporary paths; choose fresh paths when repeating them.

`file` identifies original application and player as i386 Mach-O. `otool -L` reports the original application's external Python dependency at `/Library/Frameworks/Python.framework/Versions/2.5/Python`, plus Carbon, AGL, QuickTime, and the other legacy dependencies. Binary headers and link names are not runtime resolution evidence.

An initial attempt to execute a previously extracted non-executable inspection copy returned exit 126, `permission denied`. A new copy made directly from the archive with its original executable mode `0755` was then invoked:

```sh
"$RETROFIT_REFERENCE_COPY" -b -v
```

The host rejected it with exit 127 and `bad CPU type in executable`. Blender did not start; this is an operating-system execution refusal, not an observed application crash. The copy's extracted bytes match the application hash in the manifest.

A fresh native configuration was attempted without changing source or bypassing policies:

```sh
cmake -S . -B "$RETROFIT_BUILD" -DCMAKE_OSX_ARCHITECTURES=arm64
```

Exit 1 at `CMakeLists.txt:48`: `Compatibility with CMake < 3.5 has been removed from CMake.` No executable was produced. This reconfirms the documented configuration blocker; it does not justify starting the application port before the reference gate.

## Reproduce the static inventory

Use Python 3 on the audit host; this utility does not run or convert the legacy Python 2.5 scripts. Provide the verified official archive and the repository root. Save the following code to a disposable `inventory.py`, then run:

```sh
python3 "$RETROFIT_INVENTORY_SCRIPT" "$RETROFIT_REFERENCE_ARCHIVE" . > "$RETROFIT_INVENTORY_OUTPUT"
cmp docs/evidence/reference-inventory.json "$RETROFIT_INVENTORY_OUTPUT"
```

Exit 0 from `cmp` means the static inventory reproduces for this source snapshot. The archive checksum is checked before inspection; no archive member is extracted to the filesystem. The script normalizes only complete SVN Id comment lines for classification and retains the unmodified member hash. That classification was separately checked through full unified diffs.

```python
import hashlib
import json
from pathlib import Path
import re
import sys
import tarfile

archive = Path(sys.argv[1])
source = Path(sys.argv[2])
sha256 = lambda data: hashlib.sha256(data).hexdigest()
archive_bytes = archive.read_bytes()
expected_sha256 = '5cc3f1ea2481bcc191626196f06cd21a92952e835a57c854b5974a8523736b46'
if sha256(archive_bytes) != expected_sha256:
    raise SystemExit('Reference archive SHA256 mismatch')
root = 'blender-2.45-darwin-8.10.1-i386/'
scripts = 'Blender.app/Contents/MacOS/.blender/scripts/'
# Normalize only full SVN Id comment lines; preserve all other bytes.
normalize = lambda data: re.sub(rb'(?m)^# \$Id:[^\r\n]*\$', b'# $Id$', data)
files = []
with tarfile.open(archive) as reference:
    for member in sorted(reference.getmembers(), key=lambda m: m.name):
        if not member.isfile():
            continue
        if not member.name.startswith(root):
            raise SystemExit('Unexpected archive root')
        path = member.name[len(root):]
        data = reference.extractfile(member).read()
        entry = dict(path=path, size=len(data), sha256=sha256(data))
        if path.startswith(scripts):
            local = source / 'release/scripts' / path[len(scripts):]
            if local.is_file():
                local_bytes = local.read_bytes()
                entry['source_sha256'] = sha256(local_bytes)
                entry['comparison'] = ('identical' if data == local_bytes else
                    'svn_id_comment_only' if normalize(data) == local_bytes else 'different')
            else:
                entry['comparison'] = 'archive_only'
        files.append(entry)
print(json.dumps(dict(
    archive_name=archive.name,
    archive_sha256=sha256(archive_bytes),
    archive_md5=hashlib.md5(archive_bytes).hexdigest(),
    source_commit='758a5a10e3a088eed05ebacf4e527b6c626a532f',
    coverage='Static regular-file inventory; no executed reference fixtures or parity evidence',
    files=files), indent=2))
```

## Acceptance coverage

All eight ticket acceptance criteria remain unchecked. The static inventory and runner investigation advance prerequisite discovery only. Still required: actual original execution; a complete executable preservation matrix; reference-created scenes/screenshots/outputs; independently justified fixed limits; pinned native dependencies; fresh application/player build; clean-account repeated launches; ten real editor cycles; foreground/background still and animation renders; cancellation; full scripting/format/resource/player/round-trip/UI checks; and retained runtime comparison receipts.

No application source changed. Application typechecking, module tests, the full preservation suite, screenshots, and render comparisons were not run because the prerequisite runtime is unavailable and no application build exists. Documentation and JSON consistency checks cannot substitute for those gates.
