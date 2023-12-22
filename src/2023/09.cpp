#include <di/function/minus.h>
#include <runner/aoc_problem_registry.h>

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

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 9, a, i64) {
    auto lines = input | splitv('\n');

    auto sum = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto nums = all_nums(line);

        auto diffs = Vector<Vector<i32>> {};
        diffs.push_back(di::clone(nums));

        while (!all_of(last(diffs), equal(0))) {
            auto& ns = last(diffs);
            diffs.push_back(ns | pairwise_transform(flip(minus)) | to<Vector>());
        }

        sum += di::sum(diffs | transform(last));
    }
    return sum;
}

AOC_SOLUTION(2023, 9, b, i64) {
    auto lines = input | splitv('\n');

    auto sum = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto nums = all_nums(line);

        auto diffs = Vector<Vector<i32>> {};
        diffs.push_back(di::clone(nums));

        while (!all_of(last(diffs), equal(0))) {
            auto& ns = last(diffs);
            diffs.push_back(ns | pairwise_transform(flip(minus)) | to<Vector>());
        }

        auto res = 0;
        for (usize i = diffs.size() - 1; i < diffs.size(); i--) {
            auto n = diffs[i][0];

            res = n - res;
        }
        sum += res;
    }
    return sum;
}
