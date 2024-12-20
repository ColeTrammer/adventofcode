#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 20, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto srow = 0_usize;
    auto scol = 0_usize;
    auto erow = 0_usize;
    auto ecol = 0_usize;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                srow = row;
                scol = col;
            } else if (ch == 'E') {
                erow = row;
                ecol = col;
            }
        }
    }

    auto base_costs = Vec<Vec<i64>> {};
    base_costs.resize(lines.size());
    for (auto& x : base_costs) {
        x.resize(lines[0].size(), -1);
    }

    auto vis = Set<Coord> {};
    auto dfs = [&](this auto&& dfs, usize row, usize col) -> i64 {
        if (row == erow && col == ecol) {
            base_costs[row][col] = 0;
            return 0;
        }
        if (vis.contains({ row, col })) {
            return base_costs[row][col];
        }

        vis.insert({ row, col });
        for (auto const& [r, c, v] : neighbors(lines, row, col, false)) {
            if (v != '#') {
                auto cost = 1 + dfs(r, c);
                if (cost == 0) {
                    continue;
                }
                if (base_costs[row][col] == -1) {
                    base_costs[row][col] = cost;
                } else {
                    base_costs[row][col] = min(base_costs[row][col], cost);
                }
            }
        }
        return base_costs[row][col];
    };
    dfs(srow, scol);

    auto hist = Map<i64, usize> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '#') {
                for (auto [r, c] :
                     Array { Tuple { row + 2, col }, Tuple { row - 2, col }, Tuple { row, col + 2 },
                             Tuple { row, col - 2 }, Tuple { row + 1, col + 1 }, Tuple { row - 1, col - 1 },
                             Tuple { row + 1, col - 1 }, Tuple { row - 1, col + 1 } }) {
                    if (r < lines.size() && c < lines[0].size()) {
                        if (lines[r][c] != '#') {
                            auto new_cost = 2 + base_costs[r][c];
                            auto save = base_costs[row][col] - new_cost;
                            if (save > 0) {
                                hist[save]++;
                            }
                            if (save >= 100) {
                                s++;
                            }
                        }
                    }
                }
            }
        }
    }

    return s;
}

AOC_SOLUTION(2024, 20, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto srow = 0_usize;
    auto scol = 0_usize;
    auto erow = 0_usize;
    auto ecol = 0_usize;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                srow = row;
                scol = col;
            } else if (ch == 'E') {
                erow = row;
                ecol = col;
            }
        }
    }

    auto base_costs = Vec<Vec<i64>> {};
    base_costs.resize(lines.size());
    for (auto& x : base_costs) {
        x.resize(lines[0].size(), -1);
    }

    auto vis = Set<Coord> {};
    auto dfs = [&](this auto&& dfs, usize row, usize col) -> i64 {
        if (row == erow && col == ecol) {
            base_costs[row][col] = 0;
            return 0;
        }
        if (vis.contains({ row, col })) {
            return base_costs[row][col];
        }

        vis.insert({ row, col });
        for (auto const& [r, c, v] : neighbors(lines, row, col, false)) {
            if (v != '#') {
                auto cost = 1 + dfs(r, c);
                if (cost == 0) {
                    continue;
                }
                if (base_costs[row][col] == -1) {
                    base_costs[row][col] = cost;
                } else {
                    base_costs[row][col] = min(base_costs[row][col], cost);
                }
            }
        }
        return base_costs[row][col];
    };
    dfs(srow, scol);

    auto offsets = Set<Tuple<i64, i64>> {};
    auto vs = Set<Tuple<i64, i64, i64>> {};
    auto gen = [&](this auto&& gen, i64 dr, i64 dc, i64 rem) {
        offsets.insert({ dr, dc });
        if (rem == 0) {
            return;
        }
        if (vs.contains({ dr, dc, rem })) {
            return;
        }
        vs.insert({ dr, dc, rem });

        for (auto [r, c] : Array { Tuple { 1, 0 }, Tuple { -1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
            gen(dr + r, dc + c, rem - 1);
        }
    };
    gen(0, 0, 20);

    auto hist = Map<i64, usize> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '#') {
                for (auto [dr, dc] : offsets) {
                    auto r = row + dr;
                    auto c = col + dc;
                    if (r < lines.size() && c < lines[0].size()) {
                        if (lines[r][c] != '#') {
                            auto new_cost = abs(dr) + abs(dc) + base_costs[r][c];
                            auto save = base_costs[row][col] - new_cost;
                            if (save >= 50) {
                                hist[save]++;
                            }
                            if (save >= 100) {
                                s++;
                            }
                        }
                    }
                }
            }
        }
    }

    return s;
}
