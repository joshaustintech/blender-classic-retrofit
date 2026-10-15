# Keep foreground proof separate from CLI verification

Status: accepted

## Context

Some post-2.45 tickets require opening a packaged Blender app and checking foreground UI or rendered output. The CUA runtime may lack native app controls even when the host can run Blender in a desktop session. Blender's packaged executable can create, reopen, and batch-render projects; those checks prove file and CLI behavior only.

## Decision

Record CLI save/reopen, batch rendering, native CTest, sanitizer, GUI-open screenshots, and interactive GUI render evidence as separate results. Do not treat a background render or a successful app build as proof of foreground rendering or visual parity. When CUA native controls are absent, use an explicitly authorized native input fallback only if it can launch and control the actual packaged GUI; otherwise leave the foreground-render acceptance open.

## Consequences

Tickets requiring foreground evidence remain incomplete until the required UI observation can be performed. This preserves the acceptance criteria while allowing independent native and CLI checks to proceed.

## Evidence

For Ticket 03, malformed and missing-font regression tests pass, the bundled valid font retains the glyph A outline and width, and the packaged command-line workflow saves/reopens text geometry and matches a pre-fix render byte-for-byte. On 2026-10-14, a fresh CUA session returned an empty native-app inventory and lacked its documented native app methods. Direct GUI startup inside the sandbox aborted in Cocoa initialization. Running the same compiled executable with host window access opened the saved project; native macOS input then activated Blender's Render control. The actual `Blender:Render` window displayed the text image and showed a 0.11-second preview render. Screenshot: `../../.scratch/post-245-fixes/evidence/font-text-foreground-render.png`. Full CTest passed 9/9 with host window access; the sandboxed run's `ghost_native` test failed on LaunchServices access.
