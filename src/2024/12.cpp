#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 12, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto vs = Set<Coord> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, c] : enumerate(line)) {
            if (vs.contains({ row, col })) {
                continue;
            }

            auto dfs = [&](this auto&& dfs, usize row, usize col, c8 ch) -> Tuple<i64, i64> {
                if (vs.contains({ row, col })) {
                    return { 0, 0 };
                }
                vs.insert({ row, col });

                auto ar = 1_i64;
                auto p = 0_i64;

                for (auto [dr, dc] :
                     Array<Tuple<int, int>, 4> { Tuple { -1, 0 }, Tuple { 1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
                    auto nr = row + dr;
                    auto nc = col + dc;

                    if (nr >= lines.size() || nc >= lines[0].size()) {
                        p++;
                        continue;
                    }
                    if (lines[nr][nc] != ch) {
                        p++;
                        continue;
                    }

                    auto [na, np] = dfs(nr, nc, ch);
                    ar += na;
                    p += np;
                }

                return { ar, p };
            };

            auto [a, p] = dfs(row, col, c);
            s += a * p;
        }
    }
    return s;
}

AOC_SOLUTION(2024, 12, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto vs = Set<Coord> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, plant] : enumerate(line)) {
            if (vs.contains({ row, col })) {
                continue;
            }

            auto g = Map<Coord, Set<Coord>> {};
            auto dfs = [&](this auto&& dfs, usize row, usize col, c8 ch) -> i64 {
                if (vs.contains({ row, col })) {
                    return 0;
                }
                vs.insert({ row, col });

                auto ar = 1_i64;

                for (auto [dr, dc] :
                     Array<Tuple<int, int>, 4> { Tuple { -1, 0 }, Tuple { 1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
                    auto nr = row + dr;
                    auto nc = col + dc;

                    auto [pr, pc, pr2, pc2] = [&] -> Tuple<usize, usize, usize, usize> {
                        // Up
                        if (dr == -1) {
                            return { row, col, row, col + 1 };
                        }
                        // Down
                        else if (dr == 1) {
                            return { row + 1, col, row + 1, col + 1 };
                        }
                        // Left
                        else if (dc == -1) {
                            return { row, col, row + 1, col };
                        }
                        // Right
                        else {
                            return { row, col + 1, row + 1, col + 1 };
                        }
                    }();

                    if (nr >= lines.size() || nc >= lines[0].size() || lines[nr][nc] != ch) {
                        g[{ pr, pc }].insert({ pr2, pc2 });
                        g[{ pr2, pc2 }].insert({ pr, pc });
                        continue;
                    }

                    ar += dfs(nr, nc, ch);
                }

                return ar;
            };

            auto a = dfs(row, col, plant);

            auto edges = Set<Tuple<Coord, Coord>> {};
            auto nodes = Set<Coord> {};
            auto dfs2 = [&](this auto&& dfs2, usize r0, usize c0, usize r, usize c, int odr, int odc) -> i64 {
                // Base
                if (r == r0 && c == c0 && !edges.empty()) {
                    return 0;
                }
                nodes.insert({ r, c });

                // Ensure we're always making right turns.
                auto dirs = [&] {
                    // Up
                    if (odr == -1) {
                        return Array { Tuple { 0, 1 }, Tuple { 1, 0 }, Tuple { 0, -1 }, Tuple { -1, 0 } };
                    }
                    // Down
                    else if (odr == 1) {
                        return Array { Tuple { 0, -1 }, Tuple { -1, 0 }, Tuple { 0, 1 }, Tuple { 1, 0 } };
                    }
                    // Left
                    else if (odc == -1) {
                        return Array { Tuple { -1, 0 }, Tuple { 0, 1 }, Tuple { 1, 0 }, Tuple { 0, -1 } };
                    }
                    // Right
                    else {
                        return Array { Tuple { 1, 0 }, Tuple { 0, -1 }, Tuple { -1, 0 }, Tuple { 0, 1 } };
                    }
                }();
                for (auto [dr, dc] : dirs) {
                    if (edges.contains({ { r, c }, { r + dr, c + dc } })) {
                        continue;
                    }
                    if (g[{ r, c }].contains({ r + dr, c + dc })) {
                        edges.insert({ { r, c }, { r + dr, c + dc } });
                        edges.insert({ { r + dr, c + dc }, { r, c } });
                        auto res = dfs2(r0, c0, r + dr, c + dc, dr, dc);
                        if (dr != odr || dc != odc) {
                            return 1 + res;
                        }
                        return res;
                    }
                }

                return 1;
            };

            auto p = 0_i64;
            for (auto [r0, c0] : keys(g)) {
                if (!nodes.contains({ r0, c0 })) {
                    edges.clear();
                    p += dfs2(r0, c0, r0, c0, -1, 0);
                }
            }

            s += a * p;
        }
    }
    return s;
}
