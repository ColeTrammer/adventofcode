# Advent of Code Solutions

This repository contains my solutions to the [Advent of Code](https://adventofcode.com) challenges.

## Table of Solution Completion

| Year | Language | Number of Problems Solved |
| ---- | -------- | ------------------------- |
| 2020 | C++      | 3/25                      |
| 2021 | Rust     | 8/25                      |
| 2022 | Rust     | 25/25                     |
| 2023 | C++      | 0/25                      |

## Running the Solutions

Since I created separate repositories for the years 2021 and 2022, they have been added as git submodules. Additionally,
the 2023 solutions depend on my own C++ libraries, which are also included as git submodules. To clone this repository
with the submodules, use the following command:

```bash
git clone --recurse-submodules https://github.com/ColeTrammer/adventofcode.git
```

### 2023

The 2023 solutions are written in C++ using my own custom libraries, which are part of my from own from-scratch
[OS](https://github.com/ColeTrammer/iros). In fact, the built solutions have no external (not even standard library)
dependencies. The library itself only works on Linux x86_64, as it implements the Linux ABI for system calls and program
startup.

The source code is located in the `2023` directory. Each day has a solution file `solution.cpp`, in a directory
of the form `01`, `02`, etc. Additionally, there are some C++ solution to 2022 problems in the `2022` directory. To
compile and run the solutions, use the following commands:

```bash
cmake --preset clang_release_default
cmake --build --preset clang_release_default

export PATH="$(realpath ./build/clang/release):$PATH"
export DAY=01
export YEAR=2023

aoc -y $YEAR -d $DAY
```

The input files are located in the `$YEAR/$DAY/input.txt` file. The solution by default reads a file named
`$YEAR/$DAY/input.txt` in the current working directory, but can be changed by passing the path to the input file as a
command line argument:

```bash
aoc -y $YEAR -d $DAY -i input.txt
```

To get the solution to part b, pass the `-b` flag:

```bash
aoc -y $YEAR -d $DAY -b
```

To run against the problem provided sample input (named `test.txt`), pass the `-t` flag:

```bash
aoc -y $YEAR -d $DAY -t
```

### 2022

The solutions for 2022 are located in the `adventofcode2022` directory. Each day has its own rust project, named
`day01`, `day02`, etc. To compile and run a solution, use the following commands:

```bash
DAY=01
cd adventofcode2022/day$DAY
cargo update # This may be required if the Cargo.lock file is out of date
cargo run --release
```

The input files are located in the `adventofcode2022/day$DAY/input.txt` file. The solution defaults to reading a file
named `input.txt` in the current working directory, but can be changed by passing the path to the input file as a
command line argument:

```bash
cargo run --release -- --input input.txt
```

To get the solution for part b, pass the `--part-b` flag:

```bash
cargo run --release -- --part-b
```

To run against the problem provided sample input (named `test.txt`), pass the `--test` flag:

```bash
cargo run --release -- --test
```

### 2021

The solutions for 2021 are located in the `adventofcode2021` directory. Each day has its own rust project, named `day1`,
`day2`, etc. To compile and run a solution, use the following commands:

```bash
DAY=1
cd adventofcode2021/day$DAY
cargo update # This may be required if the Cargo.lock file is out of date
cargo run --release
```

The input files are located in the `adventofcode2021/day$DAY/input.txt` file. The solutions are hard-coded to read a
file named `input.txt` in the current working directory.

### 2020

The solutions for 2020 are located in the `adventofcode2020` directory. The solutions files are named `1.cpp`, `2.cpp`,
etc. To compile and run a solution, use the following commands:

```bash
DAY=1
cmake -S . -B build -G Ninja
cmake --build build
./build/$DAY
```

The input files are hardcoded as strings in the source code.

## License

This repository is licensed under the MIT license. See the [LICENSE](LICENSE) file for more details.
