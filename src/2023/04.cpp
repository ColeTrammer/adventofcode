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
        auto [_, nums] = split_two(line, ':');
        auto [winning, mine] = split_two(nums, '|');

        auto w = all_nums(winning, ' ') | to<TreeSet>();
        auto m = all_nums(mine, ' ') | to<TreeSet>();

        w &= m;

        auto matches = w.size();
        if (matches > 0) {
            sum += 1 << (matches - 1);
        }
    }
    return sum;
}

AOC_SOLUTION(2023, 4, b, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto counts = repeat(1, lines.size()) | to<Vector>();
    for (auto [row, line] : enumerate(lines)) {
        auto [_, nums] = split_two(line, ':');
        auto [winning, mine] = split_two(nums, '|');

        auto w = all_nums(winning, ' ') | to<TreeSet>();
        auto m = all_nums(mine, ' ') | to<TreeSet>();

        w &= m;

        auto matches = w.size();
        for (usize i = 1; i <= matches; i++) {
            counts[row + i] += counts[row];
        }
    }
    return sum(counts);
}
