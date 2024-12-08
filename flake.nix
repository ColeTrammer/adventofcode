{
  description = "Advent of Code solutions";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-root.url = "github:srid/flake-root";
    treefmt-nix.url = "github:numtide/treefmt-nix";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [
        inputs.treefmt-nix.flakeModule
        inputs.flake-root.flakeModule
      ];
      systems = [ "x86_64-linux" ];
      perSystem =
        {
          config,
          pkgs,
          ...
        }:
        {
          treefmt = {
            inherit (config.flake-root) projectRootFile;

            programs = {
              nixfmt.enable = true;
              clang-format = {
                enable = true;
                package = pkgs.clang-tools_19;
              };
              prettier.enable = true;
              shfmt = {
                enable = true;
                indent_size = 4;
              };
              just.enable = true;
            };

            settings.excludes = [
              "deps/**"
              "adventofcode2020/**"
              "adventofcode2021"
              "adventofcode2022"
              ".gitignore"
              ".gitmodules"
              ".prettierignore"
              "LICENSE"
            ];

            settings.formatter.prettier.includes = [
              ".clang-format"
              ".clangd"
              ".prettierrc"
              "flake.lock"
            ];

            settings.formatter.cmake-format = {
              command = "${pkgs.cmake-format}/bin/cmake-format";
              options = [ "-i" ];
              includes = [
                "CMakeLists.txt"
                "CMakeToolchain.*.txt"
                "*.cmake"
              ];
            };
          };

          devShells.default =
            let
              gccVersion = "14";
              llvmVersion = "19";
            in
            pkgs.mkShell.override { stdenv = pkgs."gcc${gccVersion}Stdenv"; } {
              packages =
                [
                  config.treefmt.build.wrapper
                  pkgs.cmake-format
                ]
                ++ builtins.attrValues config.treefmt.build.programs
                ++ [
                  pkgs."clang-tools_${llvmVersion}"
                  pkgs.jq
                  pkgs.just
                  pkgs.html-xml-utils
                ]
                ++ [
                  pkgs."clang_${llvmVersion}"
                  pkgs.cmake
                  pkgs.ninja
                  pkgs.gdb
                  pkgs."lldb_${llvmVersion}"
                  pkgs.ccache
                  pkgs.valgrind
                ];

              hardeningDisable = [ "format" ];
            };
        };
    };
}
