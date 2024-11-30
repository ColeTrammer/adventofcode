preset := env("PRESET", "clang_release_default")

alias c := configure
alias b := build
alias cb := configure_build
alias bf := build_file
alias bonly := build_target
alias r := run
alias br := build_run
alias s := submit
alias bs := build_submit

# Default command: configure and build
default:
    @just preset={{ preset }} configure_build

# Configure
configure *args="":
    cmake --preset {{ preset }} {{ args }}

# Build
build *args="": ensure_configured
    cmake --build --preset {{ preset }} {{ args }}

# Configure and build
configure_build:
    @just preset={{ preset }} configure
    @just preset={{ preset }} build

# Compile a specific file (regex matching)
build_file name: ensure_configured
    #!/usr/bin/env bash
    set -euo pipefail

    targets=$(cmake --build --preset {{ preset }}_non_unity -- -t targets all \
        | cut -d ' ' -f 1 \
        | tr -d '[:]' \
        | grep -E "{{ name }}" \
    )

    echo -e '\e[1m'"cmake --build --preset {{ preset }}_non_unity -t "$targets'\e[m'
    cmake --build --preset {{ preset }}_non_unity -t ${targets}

# Build a specific target (regex matching)
build_target name: ensure_configured
    #!/usr/bin/env bash
    set -euo pipefail

    targets=$(cmake --build --preset {{ preset }} -- -t targets all \
        | cut -d ' ' -f 1 \
        | tr -d '[:]' \
        | grep -E "{{ name }}" \
        | grep -vF '.cxx.o' \
        | grep -vF 'cmake_object_order' \
        | grep -vF 'CMakeFiles' \
        | grep -vF 'CMakeLists.txt' \
        | grep -vF '/install' \
        | grep -vF 'verify_interface_header_sets' \
        | grep -vF '/edit_cache' \
        | grep -vF '/rebuild_cache' \
        | grep -vF '/list_install_components' \
        | grep -vE '/all$' \
        | grep -vE '/test$' \
        | grep -E '/' \
    )

    echo -e '\e[1m'"cmake --build --preset {{ preset }} -t "$targets'\e[m'
    cmake --build --preset {{ preset }} -t $targets

# Run aoc solver
run *args="": ensure_configured
    #!/usr/bin/env bash
    set -euo pipefail

    build_directory=$( \
        jq -rc '.configurePresets.[] | select(.name == "{{ preset }}") | .binaryDir' < CMakePresets.json | \
        sed s/\${sourceDir}/./g \
    )

    set +e
    echo -e '\e[1m'"$build_directory/aoc {{ args }}"'\e[m'
    $build_directory/aoc {{ args }}
    exit 0

# Build and run aoc solver
build_run *args="":
    @just preset={{ preset }} build
    @just preset={{ preset }} run {{ args }}

# Submit answer
submit *args="": ensure_configured
    @just preset={{ preset }} run {{ args }} | ./scripts/submit.sh

# Build and submit answer
build_submit *args="":
    @just preset={{ preset }} build
    @just preset={{ preset }} submit {{ args }}

# Run clang-tidy and perform fixes
tidy *args="": ensure_configured
    #!/usr/bin/env bash
    set -euo pipefail

    export IROS_TIDY_ARGS="{{ args }}"
    cmake --build --preset {{ preset }} -t tidy

# Run static analysis
analyze *args="": ensure_configured
    #!/usr/bin/env bash
    set -euo pipefail

    export IROS_TIDY_ARGS="{{ args }}"
    cmake --build --preset {{ preset }} -t analyze

# Run clang-tidy and output failures
check_tidy *args="": ensure_configured
    #!/usr/bin/env bash
    set -euo pipefail

    export IROS_TIDY_ARGS="{{ args }}"
    cmake --build --preset {{ preset }} -t check_tidy

# Clean
clean: ensure_configured
    @just preset={{ preset }} build --target clean

# Build docs
build_docs: ensure_configured
    @just preset={{ preset }} build --target docs

# Open docs
open_docs: ensure_configured
    @just preset={{ preset }} build --target open_docs

# Build and open docs
docs:
    @just preset={{ preset }} build_docs
    @just preset={{ preset }} open_docs

# Auto-format source code
format:
    nix fmt

# Validate format and lint code
check:
    nix flake check

# Select a CMake preset (meant to be run with eval, e.g. `eval $(just choose)`)
choose:
    @echo "export PRESET=\$(cmake --list-presets=configure | tail +2 | fzf | awk '{ print \$1 }' | tr -d '[\"]')"

# Clean all
cleanall:
    rm -rf build/

[private]
ensure_configured preset=preset:
    #!/usr/bin/env bash
    set -euo pipefail

    build_directory=$( \
        jq -rc '.configurePresets.[] | select(.name == "{{ preset }}") | .binaryDir' < CMakePresets.json | \
        sed s/\${sourceDir}/./g \
    )
    if [ ! -d "$build_directory" ]; then
        echo -e '\e[1m'"cmake --preset {{ preset }}"'\e[m'
        cmake --preset {{ preset }}
    fi

    build_directory="$(realpath $build_directory)"
    if [ "`readlink build/compile_commands.json`" != "$build_directory"/compile_commands.json ]; then
        rm -f build/compile_commands.json
        ln -s "$build_directory"/compile_commands.json build/compile_commands.json
    fi
