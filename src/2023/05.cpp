#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/function/not_fn.h"
#include "di/vocab/tuple/make_from_tuple.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 5, a, usize) {
    auto lines = input | split('\n') | to<Vector>();

    auto [_, seeds_s] = split_two(lines[0], ':');
    auto seeds = all_nums_i<usize>(seeds_s, ' ') | to<Vector>();

    for (usize row = 2; row < lines.size(); row++) {
        auto matched = TreeSet<usize> {};
        while (++row < lines.size() && !lines[row].empty()) {
            auto nums = all_nums_i<usize>(lines[row], ' ') | to<Vector>();
            usize dest = nums[0];
            usize source = nums[1];
            usize length = nums[2];

            for (auto [i, seed] : enumerate(seeds)) {
                if (seed >= source && seed < source + length) {
                    if (matched.contains(i)) {
                        continue;
                    }
                    seed = seed - source + dest;
                    matched.insert(i);
                }
            }
        }
    }

    return min(seeds);
}

struct Interval {
    usize start { 0 };
    usize end { 0 };

    bool operator==(Interval const&) const = default;
    auto operator<=>(Interval const&) const = default;

    usize length() const { return end - start; }
    usize last() const { return empty() ? start : end - 1; }

    bool empty() const { return start == end; }

    bool overlap(Interval other) {
        return contains(other.start) || contains(other.last()) || other.contains(start) || other.contains(last());
    }

    bool contains(usize point) { return point >= start && point < end; }
};

AOC_SOLUTION(2023, 5, b, usize) {
    auto lines = input | split('\n') | to<Vector>();

    auto [_, seeds_s] = split_two(lines[0], ':');
    auto seeds = all_nums_i<usize>(seeds_s, ' ') | pairwise_transform([](auto s, auto l) {
                     return Interval { s, s + l };
                 }) |
                 stride(2) | to<Vector>();

    for (usize row = 2; row < lines.size(); row++) {
        auto matched = TreeSet<usize> {};
        auto new_intervals = Vector<Interval> {};
        while (++row < lines.size() && !lines[row].empty()) {
            auto nums = all_nums_i<usize>(lines[row], ' ') | to<Vector>();
            usize dest = nums[0];
            usize source = nums[1];
            usize length = nums[2];

            auto source_interval = Interval { source, source + length };

            for (usize i = 0; i < seeds.size(); i++) {
                auto& seed = seeds[i];
                if (source_interval.overlap(seed)) {
                    // 4 cases:
                    // completely contained
                    if (source_interval.contains(seed.start) && source_interval.contains(seed.last())) {
                        new_intervals.push_back(Interval { seed.start - source + dest, seed.end - source + dest });
                        matched.insert(i);
                    }
                    // split right:
                    else if (source_interval.contains(seed.start)) {
                        new_intervals.push_back(
                            Interval { seed.start - source + dest, source_interval.end - source + dest });
                        seed = Interval { source_interval.end, seed.end };
                    }
                    // split left:
                    else if (source_interval.contains(seed.last())) {
                        new_intervals.push_back(
                            Interval { source_interval.start - source + dest, seed.end - source + dest });
                        seed = Interval { seed.start, source_interval.start };
                    }
                    // split both:
                    else {
                        new_intervals.push_back(
                            Interval { source_interval.start - source + dest, source_interval.end - source + dest });
                        auto seed_copy = seed;
                        seed = Interval { source_interval.end, seed.end };
                        seeds.push_back(Interval { seed_copy.start, source_interval.start });
                    }
                }
            }
        }
        for (usize i = 0; i < seeds.size(); i++) {
            if (!matched.contains(i)) {
                new_intervals.push_back(seeds[i]);
            }
        }
        seeds = di::move(new_intervals);
    }

    return min(seeds | transform(&Interval::start));
}
