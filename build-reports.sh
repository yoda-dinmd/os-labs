#!/usr/bin/env bash
set -euo pipefail
repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$repo_dir/build"
for source in 'lab1&2/lab1.tex' 'lab1&2/lab2.tex' 'lab5/lab5.tex' 'lab6/lab6.tex'; do
  (cd "$repo_dir/$(dirname -- "$source")" && tectonic --outdir "$repo_dir/build" "$(basename -- "$source")")
done
