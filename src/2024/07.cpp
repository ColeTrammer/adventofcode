#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 7, a, u64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_u64;
    for (auto [row, line] : enumerate(lines)) {
        auto [as, bs] = split_two(line.view(), ':');
        auto target = uparse_i<u64>(as);
        auto ns = all_nums_i<u64>(bs, ' ');

        auto dfs = [&](this auto&& dfs, u64 p, usize i) -> bool {
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

    auto s = 0_u64;
    for (auto [row, line] : enumerate(lines)) {
        auto [as, bs] = split_two(line.view(), ':');
        auto target = uparse_i<u64>(as);
        auto ns = all_nums_i<u64>(bs, ' ');

        auto dfs = [&](this auto&& dfs, u64 p, usize i) -> bool {
            if (i == ns.size()) {
                return target == p;
            }

            auto concat = to_string(p);
            concat += to_string(ns[i]);
            auto cn = *di::parse<u64>(concat.view());
            return dfs(p + ns[i], i + 1) || dfs(p * ns[i], i + 1) || dfs(cn, i + 1);
        };

        if (dfs(ns[0], 1)) {
            s += target;
        }
    }
    return s;
}
