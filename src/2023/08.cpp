#include <di/container/algorithm/fold_left_first.h>
#include <di/container/algorithm/sort.h>
#include <di/function/compare_backwards.h>
#include <di/math/abs_unsigned.h>
#include <di/math/gcd.h>
#include <di/math/lcm.h>
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

AOC_SOLUTION(2023, 8, a, isize) {
    auto lines = input | splitv('\n');

    auto sum = 0;

    auto ins = lines[0].view();

    using Map = TreeMap<Tsv, Tuple<Tsv, Tsv>>;
    auto map = Map {};
    for (auto& line : lines | drop(2)) {
        auto node = line.substr(line.begin() + 0, line.begin() + 3);
        auto l = line.substr(line.begin() + 7, line.begin() + 10);
        auto r = line.substr(line.begin() + 12, line.begin() + 15);

        map[node] = { l, r };
    }

    auto node = "AAA"_tsv;
    auto i = 0zu;
    while (node != "ZZZ"_tsv) {
        sum++;

        ASSERT(map.contains(node));

        auto dir = ins[i++ % ins.size()];
        if (dir == 'L') {
            node = tget<0>(map[node]);
        } else {
            node = tget<1>(map[node]);
        }
    }

    return sum;
}

AOC_SOLUTION(2023, 8, b, isize) {
    auto lines = input | splitv('\n');

    auto ins = lines[0].view();

    using Map = TreeMap<Tsv, Tuple<Tsv, Tsv>>;
    auto map = Map {};
    for (auto& line : lines | drop(2)) {
        auto node = line.substr(line.begin() + 0, line.begin() + 3);
        auto l = line.substr(line.begin() + 7, line.begin() + 10);
        auto r = line.substr(line.begin() + 12, line.begin() + 15);

        map[node] = { l, r };
    }

    auto nodes = keys(map) | filter([](auto const& string) {
                     return string.ends_with('A');
                 }) |
                 to<Vector>();
    auto results = Vector<usize> {};
    for (auto& node : nodes) {
        auto sum = 0zu;
        auto i = 0zu;
        while (!node.ends_with('Z')) {
            sum++;

            ASSERT(map.contains(node));

            auto dir = ins[i % ins.size()];
            if (dir == 'L') {
                node = tget<0>(map[node]);
            } else {
                node = tget<1>(map[node]);
            }
            i++;
        }
        results.push_back(sum);
    }

    return *fold_left_first(results, lcm);
}
