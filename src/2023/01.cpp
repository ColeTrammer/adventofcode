#include "di/container/interface/empty.h"
#include "di/container/interface/reconstruct.h"
#include "di/container/view/drop.h"
#include "di/function/not_fn.h"
#include "di/parser/parse_unchecked.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 1, a, i32) {
    auto lines = input | split('\n') | filter(not_fn(empty));
    auto nums = lines | transform([](Tsv string) {
                    auto filtered = string | filter('0'_mc - '9'_mc);
                    return (*filtered.front() - '0') * 10 + (*filtered.back() - '0');
                });
    return sum(nums);
}

AOC_SOLUTION(2023, 1, b, i32) {
    auto lines = input | split('\n') | filter(not_fn(empty));

    auto get_num = [](Tsv line) {
        auto nums = Array {
            "one"_tsv, "two"_tsv, "three"_tsv, "four"_tsv, "five"_tsv, "six"_tsv, "seven"_tsv, "eight"_tsv, "nine"_tsv,
        };

        auto digits = Vector<i32> {};
        for (auto it = line.begin(); it != line.end(); ++it) {
            auto part = Tsv(it, line.end());
            for (auto [i, num] : enumerate(nums)) {
                if (starts_with(part, num)) {
                    digits.push_back(i + 1);
                }
            }
            if (*part.front() >= '0' && *part.front() <= '9') {
                digits.push_back(*part.front() - '0');
            }
        }

        return *digits.front() * 10 + *digits.back();
    };

    return sum(transform(lines, get_num));
}
