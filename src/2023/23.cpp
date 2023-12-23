#include <di/assert/assert_binary.h>
#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/vector/static_vector.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/container/prelude.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 23, a, i64) {
    auto lines = input | splitv('\n');

    // auto const width = lines[0].size();
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

    auto const width = lines[0].size();
    auto const height = lines.size();

    auto visited = di::Vector<bool> {};
    visited.resize(width * height);

    auto s = 0;

    auto dfs = ycombinator([&](auto&& dfs, i32 length, usize row, usize col) -> void {
        if (row == height - 1) {
            if (length > s) {
                eprintln("New best: {}"_sv, length);
            }
            s = max(s, length);
            return;
        }

        visited[row * height + col] = true;
        auto orow = row;
        auto ocol = col;
        auto to_remove = di::StaticVector<Tuple<usize, usize>, di::Constexpr<1000zu>> {};
        for (;;) {
            auto candidates = di::StaticVector<Tuple<usize, usize>, di::Constexpr<4zu>> {};
            for (auto [r, c, v] : neighbors(lines, row, col, false)) {
                if (visited[r * height + c]) {
                    continue;
                }
                if (v != '#') {
                    ASSERT(candidates.push_back({ r, c }));
                }
            }

            if (candidates.size() == 1) {
                tie(row, col) = candidates[0];
                ++length;

                if (row == height - 1) {
                    dfs(length, row, col);
                    break;
                }

                visited[row * height + col] = true;
                ASSERT(to_remove.push_back({ row, col }));
                continue;
            } else {
                for (auto [r, c] : candidates) {
                    dfs(length + 1, r, c);
                }
                break;
            }
        }

        for (auto [r, c] : to_remove) {
            visited[r * height + c] = false;
        }
        visited[orow * height + ocol] = false;
    });

    dfs(0, 0, 1);
    return s;
}
