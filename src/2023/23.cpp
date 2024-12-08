#include "di/assert/assert_binary.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/vector/static_vector.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/container/prelude.h"
#include "di/function/not_fn.h"
#include "di/function/ycombinator.h"
#include "di/math/abs_diff.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 23, a, i64) {
    auto lines = input | splitv('\n');

    auto const height = lines.size();

    auto visited = di::TreeSet<Tuple<usize, usize>> {};

    auto s = 0;
    auto dfs = ycombinator([&](auto&& dfs, i32 length, usize row, usize col) -> void {
        if (row == height - 1) {
            s = max(s, length);
            return;
        }

        visited.insert({ row, col });
        for (auto [r, c, v] : neighbors(lines, row, col, false)) {
            if (visited.contains({ r, c })) {
                continue;
            }
            if (v == '.' || (v == '>' && c > col) || (v == '<' && c < col) || (v == 'v' && r > row) ||
                (v == '^' && r < row)) {
                dfs(length + 1, r, c);
            }
        }
        visited.erase({ row, col });
    });

    dfs(0, 0, 1);
    return s;
}

AOC_SOLUTION(2023, 23, b, i64) {
    auto lines = input | splitv('\n');

    auto const height = lines.size();

    auto visited = TreeSet<Tuple<usize, usize>> {};
    auto edges = TreeMap<Tuple<usize, usize>, StaticVector<Tuple<usize, usize, i32>, Constexpr<4zu>>> {};

    auto build_graph = ycombinator([&](auto&& build_graph, usize row, usize col) -> void {
        auto [orow, ocol] = Tuple { row, col };
        visited.insert({ row, col });

        auto length = 0;
        for (;;) {
            auto candidates = di::StaticVector<Tuple<usize, usize>, di::Constexpr<4zu>> {};
            for (auto [r, c, v] : neighbors(lines, row, col, false)) {
                if (visited.contains({ r, c })) {
                    continue;
                }
                if (v != '#') {
                    ASSERT(candidates.push_back({ r, c }));
                }
            }

            if (candidates.size() == 1) {
                tie(row, col) = candidates[0];
                visited.insert({ row, col });
                ++length;
                continue;
            }

            if (orow != row || ocol != col) {
                ASSERT(edges[{ orow, ocol }].push_back({ row, col, length }));
                ASSERT(edges[{ row, col }].push_back({ orow, ocol, length }));
                build_graph(row, col);
                break;
            }

            for (auto [r, c] : candidates) {
                ASSERT(edges[{ row, col }].push_back({ r, c, 1 }));
                ASSERT(edges[{ r, c }].push_back({ row, col, 1 }));
                build_graph(r, c);
            }
            break;
        }
    });

    build_graph(0, 1);
    visited.clear();

    auto s = 0;

    auto dfs = ycombinator([&](auto&& dfs, i32 length, usize row, usize col) -> void {
        if (row == height - 1) {
            s = max(s, length);
            return;
        }

        visited.insert({ row, col });
        for (auto [r, c, l] : edges[{ row, col }]) {
            if (visited.contains({ r, c })) {
                continue;
            }
            dfs(length + l, r, c);
        }
        visited.erase({ row, col });
    });

    dfs(0, 0, 1);
    return s;
}
