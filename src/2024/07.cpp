#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 7, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto [as, bs] = split_two(line.view(), ':');
        auto target = uparse_int(as);
        auto ns = all_nums(bs);

        auto dfs = [&](this auto&& dfs, i64 p, usize i) -> bool {
            if (i == ns.size()) {
                return target == p;
            }

            return dfs(p + ns[i], i + 1) || dfs(p * ns[i], i + 1);
        };

        if (dfs(ns[0], 1)) {
            s += target;
        }
    }
    return s;
}

AOC_SOLUTION(2024, 7, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto [as, bs] = split_two(line.view(), ':');
        auto target = uparse_int(as);
        auto ns = all_nums(bs);

        auto dfs = [&](this auto&& dfs, i64 p, usize i) -> bool {
            if (i == ns.size()) {
                return target == p;
            }

            auto concat = to_string(p);
            concat += to_string(ns[i]);
            auto cn = uparse_int(concat);
            return dfs(p + ns[i], i + 1) || dfs(p * ns[i], i + 1) || dfs(cn, i + 1);
        };

        if (dfs(ns[0], 1)) {
            s += target;
        }
    }
    return s;
}
