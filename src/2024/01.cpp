#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 1, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    auto x = MultiSet<i32> {};
    auto y = MultiSet<i32> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [a, b] : all_nums(line) | pairwise) {
            x.insert(a);
            y.insert(b);
        }
    }

    s = zip_transform(minus, x, y) | transform(abs) | sum;
    return s;
}

AOC_SOLUTION(2024, 1, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    auto x = Vec<i32> {};
    auto y = Map<i32, i32> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [a, b] : all_nums(line) | pairwise) {
            x.push_back(a);
            y[b]++;
        }
    }

    for (auto a : x) {
        s += a * y[a];
    }

    return s;
}
