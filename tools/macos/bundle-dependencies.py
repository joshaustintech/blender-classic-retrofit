#!/usr/bin/env python3
"""Copy non-system dylibs into a Blender app and rewrite their install names."""
import argparse
import os
import shutil
import stat
import subprocess
import sys


def dependencies(path):
    output = subprocess.check_output(["/usr/bin/otool", "-L", path], text=True)
    return [line.strip().split(" (compatibility version", 1)[0]
            for line in output.splitlines()[1:] if line.startswith("\t")]


def resolve(name, owner, search_dirs):
    if name.startswith("@loader_path/"):
        candidate = os.path.join(os.path.dirname(owner), name[len("@loader_path/"):])
        return candidate if os.path.exists(candidate) else None
    if name.startswith("@executable_path/"):
        candidate = os.path.join(os.path.dirname(executable), name[len("@executable_path/"):])
        return candidate if os.path.exists(candidate) else None
    if name.startswith("@rpath/"):
        name = os.path.basename(name)
    if os.path.isabs(name):
        return name if os.path.exists(name) else None
    for directory in search_dirs:
        candidate = os.path.join(directory, os.path.basename(name))
        if os.path.exists(candidate):
            return candidate
    return None


def is_system(name):
    return name.startswith(("/System/Library/", "/usr/lib/"))


parser = argparse.ArgumentParser()
parser.add_argument("executable")
parser.add_argument("search_dirs", nargs="*")
parser.add_argument("--extra", action="append", default=[])
parser.add_argument("--module-dir")
args = parser.parse_args()
executable = os.path.realpath(args.executable)
search_dirs = list(dict.fromkeys(os.path.abspath(path) for path in args.search_dirs if path))
target_dir = os.path.dirname(executable)
queued = list(args.extra)
visited = set()
rewrites = []
owners = [executable]
if args.module_dir:
    for directory, _, files in os.walk(args.module_dir):
        owners.extend(os.path.realpath(os.path.join(directory, name))
                      for name in files if name.endswith(".so"))
owner_targets = dict((owner, owner) for owner in owners)

for owner in owners:
    for dependency in dependencies(owner):
        if not is_system(dependency):
            queued.append((owner, dependency))

while queued:
    item = queued.pop()
    if isinstance(item, tuple):
        owner, dependency = item
    else:
        owner, dependency = executable, item
    source = resolve(dependency, owner, search_dirs)
    if source is None:
        if dependency.startswith(("@", "/")) and not is_system(dependency):
            raise SystemExit("Cannot resolve non-system dependency: " + dependency)
        continue
    source = os.path.realpath(source)
    target = os.path.join(target_dir, os.path.basename(source))
    if source == executable:
        continue
    if source not in visited:
        visited.add(source)
        owner_targets[source] = target
        if source != target:
            if os.path.exists(target):
                os.chmod(target, os.stat(target).st_mode | stat.S_IWUSR)
            shutil.copy2(source, target)
        os.chmod(target, os.stat(target).st_mode | stat.S_IWUSR)
        subprocess.check_call(["/usr/bin/install_name_tool", "-id", "@executable_path/" + os.path.basename(target), target])
        for child in dependencies(source):
            if not is_system(child):
                queued.append((source, child))
    rewrites.append((owner, dependency, "@executable_path/" + os.path.basename(target)))

for owner, old, new in rewrites:
    target_owner = owner_targets[os.path.realpath(owner)]
    if old != new:
        subprocess.check_call(["/usr/bin/install_name_tool", "-change", old, new, target_owner])

for target in sorted(set(owner_targets.values()) - {executable}) + [executable]:
    subprocess.check_call(["/usr/bin/codesign", "--force", "--sign", "-", target])

print("Bundled dylibs:", ", ".join(sorted(os.path.basename(path) for path in visited)))
