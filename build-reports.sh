#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$repo_dir/reports"

all_labs=(lab1 lab2 lab3 lab4 lab5 lab6 lab7 lab8 lab9 lab10)

usage() {
  echo "Usage: $(basename "$0") [all | <lab> ...]"
  echo "Examples:"
  echo "  $(basename "$0") 9 10       # build lab9 and lab10"
  echo "  $(basename "$0") lab9       # build lab9"
  echo "  $(basename "$0") all        # build all labs (lab1..lab10)"
  echo "  $(basename "$0")            # interactive prompt"
}

resolve_lab() {
  local arg="${1%/}"
  arg="${arg%.tex}"
  arg="${arg#*/}"
  if [[ "$arg" =~ ^[0-9]+$ ]]; then
    echo "lab$arg"
  elif [[ "$arg" =~ ^lab[0-9]+$ ]]; then
    echo "$arg"
  else
    echo ""
  fi
}

selected=()

if [[ $# -eq 0 ]]; then
  if [[ -t 0 ]]; then
    echo "Available reports: 1 2 3 4 5 6 7 8 9 10 (or 'all')"
    read -r -p "Select report(s) to build [all]: " input
    if [[ -z "${input// }" || "$input" == "all" || "$input" == "a" ]]; then
      selected=("${all_labs[@]}")
    else
      read -r -a args <<< "$input"
      for arg in "${args[@]}"; do
        lab="$(resolve_lab "$arg")"
        if [[ -z "$lab" ]]; then
          echo "Error: unknown lab '$arg'" >&2
          usage >&2
          exit 1
        fi
        selected+=("$lab")
      done
    fi
  else
    selected=("${all_labs[@]}")
  fi
else
  for arg in "$@"; do
    case "$arg" in
      -h|--help)
        usage
        exit 0
        ;;
      all|a)
        selected=("${all_labs[@]}")
        ;;
      *)
        lab="$(resolve_lab "$arg")"
        if [[ -z "$lab" ]]; then
          echo "Error: unknown lab '$arg'" >&2
          usage >&2
          exit 1
        fi
        selected+=("$lab")
        ;;
    esac
  done
fi

for lab in "${selected[@]}"; do
  source_file="$repo_dir/$lab/$lab.tex"
  if [[ ! -f "$source_file" ]]; then
    echo "Error: $source_file not found" >&2
    exit 1
  fi
  echo "Building $lab -> reports/$lab.pdf ..."
  (cd "$repo_dir/$lab" && tectonic --outdir "$repo_dir/reports" "$lab.tex")
done
