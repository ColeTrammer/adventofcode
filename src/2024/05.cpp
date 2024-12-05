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

AOC_SOLUTION(2024, 5, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    auto g = Map<int, Set<int>> {};
    for (auto [row, line] : enumerate(lines)) {
        if (line.empty()) {
            continue;
        }

        if (line.contains('|')) {
            for (auto [l, r] : all_nums(line, '|') | pairwise) {
                g[l].insert(r);
            }
            continue;
        }

        auto vs = all_nums(line, ',');
        auto good = true;
        for (auto [i, x] : enumerate(vs)) {
            for (auto y : *vs.subspan(i + 1)) {
                if (g[y].contains(x)) {
                    good = false;
                    break;
                }
            }
        }

        if (good) {
            s += vs[vs.size() / 2];
        }
    }
    return s;
}

AOC_SOLUTION(2024, 5, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    auto g = Map<int, Set<int>> {};
    for (auto [row, line] : enumerate(lines)) {
        if (line.empty()) {
            continue;
        }

        if (line.contains('|')) {
            for (auto [l, r] : all_nums(line, '|') | pairwise) {
                g[l].insert(r);
            }
            continue;
        }

        auto vs = all_nums(line, ',');
        auto good = true;
        for (auto [i, x] : enumerate(vs)) {
            for (auto y : *vs.subspan(i + 1)) {
                if (g[y].contains(x)) {
                    good = false;
                    break;
                }
            }
        }

        if (good) {
            continue;
        }

        auto ns = Vec<i32> {};
        auto vst = Set<i32> {};
        auto gst = clone(vs) | to<Set<i32>>();
        auto dfs = [&](this auto&& dfs, i32 n) -> void {
            if (vst.contains(n) || !gst.contains(n)) {
                return;
            }
            vst.insert(n);

            for (auto y : g[n]) {
                dfs(y);
            }

            ns.push_back(n);
        };

        for (auto x : vs) {
            dfs(x);
        }
        s += ns[ns.size() / 2];
    }
    return s;
}
