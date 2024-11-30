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

AOC_SOLUTION(2020, 6, a, i64) {
    auto lines = input | splitv("\n\n"_sv);

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        s += line | filter(~'\n'_mc) | to<TreeSet>() | size;
    }
    return s;
}

AOC_SOLUTION(2020, 6, b, i64) {
    auto lines = input | splitv("\n\n"_sv);

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto v = line | filter(~'\n'_mc) | to<TreeSet>();
        for (auto set : line | splitv("\n"_sv) | transform(to<TreeSet>())) {
            v.intersect(set);
        }
        s += v.size();
    }
    return s;
}
