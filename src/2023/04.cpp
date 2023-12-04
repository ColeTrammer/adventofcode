#include <di/container/view/pairwise.h>
#include <runner/aoc_problem_registry.h>

#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 4, a, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto sum = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [_, nums] : split(line, ':') | pairwise) {
            for (auto [winning, mine] : split(nums, '|') | pairwise) {
                auto w = split(trim(winning), ' ') | filter(not_fn(empty)) | transform(uparse_int) | to<TreeSet>();
                auto m = split(trim(mine), ' ') | filter(not_fn(empty)) | transform(uparse_int) | to<TreeSet>();

                w.intersect(m);
                auto matches = w.size();
                if (matches > 0) {
                    sum += 1 << (matches - 1);
                }
            }
        }
    }
    return sum;
}

AOC_SOLUTION(2023, 4, b, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto counts = Vector<i32> {};
    counts.resize(lines.size(), 1);
    for (auto [row, line] : enumerate(lines)) {
        for (auto [_, nums] : split(line, ':') | pairwise) {
            for (auto [winning, mine] : split(nums, '|') | pairwise) {
                auto w = split(trim(winning), ' ') | filter(not_fn(empty)) | transform(uparse_int) | to<TreeSet>();
                auto m = split(trim(mine), ' ') | filter(not_fn(empty)) | transform(uparse_int) | to<TreeSet>();

                w.intersect(m);
                auto matches = w.size();
                for (auto x = row + 1; x < lines.size() && x < row + 1 + matches; x++) {
                    counts[x] += counts[row];
                }
            }
        }
    }
    return sum(counts);
}
