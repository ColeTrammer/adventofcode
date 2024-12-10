#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 10, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto set = Set<Tuple<usize, usize>> {};
    auto dfs = [&](this auto&& dfs, usize row, usize col, char x) {
        if (row >= lines.size() || col >= lines[0].size()) {
            return 0;
        }

        if (lines[row][col] != x) {
            return 0;
        }

        if (x == '9') {
            set.insert({ row, col });
            return 1;
        }

        return dfs(row + 1, col, x + 1) + dfs(row - 1, col, x + 1) + dfs(row, col + 1, x + 1) +
               dfs(row, col - 1, x + 1);
    };

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, c] : enumerate(line)) {
            if (c == '0') {
                set.clear();
                dfs(row, col, c);
                s += set.size();
            }
        }
    }
    return s;
}

AOC_SOLUTION(2024, 10, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto set = Set<Tuple<usize, usize>> {};
    auto dfs = [&](this auto&& dfs, usize row, usize col, char x) {
        if (row >= lines.size() || col >= lines[0].size()) {
            return 0;
        }

        if (lines[row][col] != x) {
            return 0;
        }

        if (x == '9') {
            set.insert({ row, col });
            return 1;
        }

        return dfs(row + 1, col, x + 1) + dfs(row - 1, col, x + 1) + dfs(row, col + 1, x + 1) +
               dfs(row, col - 1, x + 1);
    };

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, c] : enumerate(line)) {
            if (c == '0') {
                s += dfs(row, col, c);
            }
        }
    }
    return s;
}
