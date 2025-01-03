#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 11, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto stones = all_nums_i<i64>(line);

        for (auto _ : range(is_test ? 6 : 25)) {
            auto new_stones = Vec<i64> {};
            new_stones.reserve(2 * stones.size());
            for (auto it = stones.begin(); it != stones.end(); ++it) {
                auto& stone = *it;
                if (stone == 0) {
                    new_stones.push_back(1);
                    continue;
                }

                auto s = to_string(stone) | to_ts;
                if (s.size() % 2 == 0) {
                    auto as = s.substr(0, s.size() / 2);
                    auto bs = s.substr(s.size() / 2);

                    new_stones.push_back(uparse_int(as));
                    new_stones.push_back(uparse_int(bs));
                } else {
                    new_stones.push_back(stone * 2024);
                }
            }
            stones = di::move(new_stones);
        }

        s += stones.size();
    }
    return s;
}

AOC_SOLUTION(2024, 11, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto stones = all_nums_i<i64>(line) | to<LinkedList>();

        auto memo = Map<Tuple<i64, i64>, i64> {};

        auto compute = [&](this auto&& compute, i64 value, i64 iters) -> i64 {
            if (memo.contains({ value, iters })) {
                return memo[{ value, iters }];
            }

            if (iters == 0) {
                return 1;
            }

            auto r = [&] -> i64 {
                if (value == 0) {
                    return compute(1, iters - 1);
                }

                auto s = to_string(value);
                if (s.size_code_units() % 2 == 0) {
                    auto l = 1;
                    for (auto _ : range(s.size_code_units() / 2)) {
                        l *= 10;
                    }

                    auto a = value / l;
                    auto b = value % l;

                    return compute(a, iters - 1) + compute(b, iters - 1);
                } else {
                    return compute(value * 2024, iters - 1);
                }
            }();

            memo[{ value, iters }] = r;
            return r;
        };

        for (auto stone : stones) {
            s += compute(stone, 75);
        }
    }
    return s;
}
