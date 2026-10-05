#!/usr/bin/env bash
set -euo pipefail
repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$repo_dir/build"
cd "$repo_dir/lab1&2"
for report in lab1 lab2; do
  tectonic --outdir "$repo_dir/build" "$report.tex"
done
