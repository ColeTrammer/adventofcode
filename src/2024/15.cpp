#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 15, a, i64) {
    auto parts = input | splitv("\n\n"_tsv);

    auto grid = parts[0].view() | splitv('\n');
    auto inputs = parts[1].view() | splitv('\n');

    usize row = 0;
    usize col = 0;
    for (auto [r, line] : enumerate(grid)) {
        for (auto [c, ch] : enumerate(line)) {
            if (ch == '@') {
                row = r;
                col = c;
            }
        }
    }

    for (auto& line : inputs) {
        for (auto ch : line) {
            i64 dr = 0;
            i64 dc = 0;
            switch (ch) {
                case '>':
                    dc = 1;
                    break;
                case '<':
                    dc = -1;
                    break;
                case 'v':
                    dr = 1;
                    break;
                case '^':
                    dr = -1;
                    break;
            }

            auto nch = grid[row + dr][col + dc];
            if (nch == '#') {
                continue;
            }

            if (nch == 'O') {
                i64 i = 2;
                auto nch2 = '\0';
                for (;;) {
                    nch2 = grid[row + dr * i][col + dc * i];
                    if (nch2 == '.' || nch2 == '#') {
                        break;
                    }
                    i++;
                }

                if (nch2 == '#') {
                    continue;
                }

                swap(grid[row + dr * i][col + dc * i], grid[row + dr][col + dc]);
            }
            row += dr;
            col += dc;
        }
    }

    auto s = 0_i64;
    for (auto r : range(grid.size())) {
        for (auto c : range(grid[r].size())) {
            if (grid[r][c] == 'O') {
                s += r * 100 + c;
            }
        }
    }
    return s;
}

AOC_SOLUTION(2024, 15, b, i64) {
    auto parts = input | splitv("\n\n"_tsv);

    auto grid_small = parts[0].view() | splitv('\n');
    auto inputs = parts[1].view() | splitv('\n');

    auto grid = Vec<Vec<char>> {};
    for (auto const& line : grid_small) {
        auto& new_line = grid.emplace_back();
        for (auto ch : line) {
            switch (ch) {
                case '#':
                    new_line.push_back('#');
                    new_line.push_back('#');
                    break;
                case 'O':
                    new_line.push_back('[');
                    new_line.push_back(']');
                    break;
                case '.':
                    new_line.push_back('.');
                    new_line.push_back('.');
                    break;
                case '@':
                    new_line.push_back('@');
                    new_line.push_back('.');
                    break;
            }
        }
    }

    usize row = 0;
    usize col = 0;
    for (auto [r, line] : enumerate(grid)) {
        for (auto [c, ch] : enumerate(line)) {
            if (ch == '@') {
                row = r;
                col = c;
                ch = '.';
            }
        }
    }

    for (auto& line : inputs) {
        for (auto ch : line) {
            i64 dr = 0;
            i64 dc = 0;
            switch (ch) {
                case '>':
                    dc = 1;
                    break;
                case '<':
                    dc = -1;
                    break;
                case 'v':
                    dr = 1;
                    break;
                case '^':
                    dr = -1;
                    break;
            }

            auto nch = grid[row + dr][col + dc];
            if (nch == '#') {
                continue;
            }

            if (nch == '[' || nch == ']') {
                if (dr == 0) {
                    i64 i = 2;
                    auto nch2 = '\0';
                    for (;;) {
                        nch2 = grid[row][col + dc * i];
                        if (nch2 == '.' || nch2 == '#') {
                            break;
                        }
                        i++;
                    }

                    if (nch2 == '#') {
                        continue;
                    }

                    while (i >= 2) {
                        swap(grid[row][col + dc * i], grid[row][col + dc * (i - 1)]);
                        i--;
                    }
                } else {
                    // Vertical. Can move 2.
                    auto left = col;
                    auto right = col;
                    if (nch == '[') {
                        right++;
                    } else {
                        left--;
                    }

                    i64 i = 2;
                    auto good = false;
                    auto remem = Vec<Vec<usize>> {};
                    auto& f = remem.emplace_back();
                    f.push_back(left);
                    f.push_back(right);
                    for (;;) {
                        auto above = remem.back()->span() | transform([&](usize c) {
                                         return grid[row + dr * i][c];
                                     }) |
                                     to<Vec<char>>();
                        if (all_of(above, equal('.'))) {
                            // Do move.
                            good = true;

                            for (auto const& v : remem | reverse) {
                                for (auto j : v) {
                                    swap(grid[row + dr * i][j], grid[row + dr * (i - 1)][j]);
                                }
                                i--;
                            }

                            break;
                        }

                        if (any_of(above, equal('#'))) {
                            break;
                        }

                        auto set = Set<usize> {};
                        for (auto c : *remem.back()) {
                            auto nc = grid[row + dr * i][c];
                            if (nc == '[') {
                                set.insert(c);
                                set.insert(c + 1);
                            } else if (nc == ']') {
                                set.insert(c);
                                set.insert(c - 1);
                            }
                        }
                        remem.push_back(set | to<Vector>());

                        i++;
                    }

                    if (!good) {
                        continue;
                    }
                }
            }
            row += dr;
            col += dc;
        }
    }
    if (verbose) {
        for (auto r : range(grid.size())) {
            for (auto c : range(grid[r].size())) {
                if (row == r && col == c) {
                    print("@"_sv);
                } else {
                    print("{}"_sv, grid[r][c]);
                }
            }
            println(""_sv);
        }
    }

    auto s = 0_i64;
    for (auto r : range(grid.size())) {
        for (auto c : range(grid[r].size())) {
            if (grid[r][c] == '[') {
                s += r * 100 + c;
            }
        }
    }
    return s;
}
