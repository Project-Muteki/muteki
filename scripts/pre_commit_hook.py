#!/usr/bin/env python

from typing import Final, LiteralString


import os
import subprocess
from argparse import ArgumentParser, Namespace
from collections.abc import Generator
from pathlib import Path, PurePosixPath
import sys


MUTEKI_HEADER_START: Final[LiteralString] = '''
/**
 * @file muteki.h
 * @brief Index header that includes everything available.
 */

#ifndef __MUTEKI_H__
#define __MUTEKI_H__
'''.strip()

MUTEKI_HEADER_END: Final[LiteralString] = '#endif  // __MUTEKI_H__'


def get_repo_root() -> Path:
    repo_root_str = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"],
        check=True,
        capture_output=True,
        text=True,
    ).stdout.strip()

    return Path(repo_root_str)


def matches_any(path: Path, root: Path, patterns: list[str]) -> bool:
    """Return True if path matches any glob pattern.

    Paths are normalized to POSIX-style separators so the same patterns
    work on both POSIX and Windows.
    """
    relative = PurePosixPath(*path.relative_to(root).parts)

    return any(
        relative.match(pattern.replace("\\", "/"))
        for pattern in patterns
    )


def list_recursive(root: Path, ignore_patterns: list[str]) -> Generator[Path]:
    root = root.resolve()

    for dirpath, dirnames, filenames in os.walk(root):
        current = Path(dirpath)

        # Prune ignored directories so os.walk() doesn't descend into them.
        dirnames[:] = [
            name
            for name in dirnames
            if not matches_any(current / name, root, ignore_patterns)
        ]

        dirnames.sort()
        filenames.sort()

        for name in filenames:
            path = current / name

            if not matches_any(path, root, ignore_patterns):
                yield path.relative_to(root)


def install_hook() -> None:
    repo_root = get_repo_root()

    commit_hook_path = repo_root / '.git' / 'hooks' / 'pre-commit'
    with commit_hook_path.open('w') as f:
        f.writelines([
            '#!/bin/sh\n',
            'set -e\n',
            'python "$(git rev-parse --show-toplevel)/scripts/pre_commit_hook.py"\n'
        ])
    commit_hook_path.chmod(0o755)


def parse_args() -> tuple[ArgumentParser, Namespace]:
    p = ArgumentParser()
    p.add_argument('--install', action='store_true', default=False, help='Install the hooks.')
    return p, p.parse_args()


if __name__ == '__main__':
    _, args = parse_args()
    if args.install:
        install_hook()
        sys.exit(0)

    repo_root = get_repo_root()
    search_root = repo_root / 'include' / 'muteki'
    headers = list(list_recursive(search_root, [
        '**/types/*.h',
        'common.h',
    ]))
    with (repo_root / 'include' / 'muteki.h').open('w') as f:
        f.write(MUTEKI_HEADER_START)
        f.write('\n\n')
        f.write('#include "muteki/common.h"\n\n')
        for h in headers:
            f.write(f'#include "{(PurePosixPath('muteki') / h).as_posix()}"\n')
        f.write('\n')
        f.write(MUTEKI_HEADER_END)
