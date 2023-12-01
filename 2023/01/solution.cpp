#include <di/container/interface/reconstruct.h>
#include <di/container/view/drop.h>
#include <di/parser/parse_unchecked.h>
#include <runner/aoc_problem_registry.h>

#include <di/container/interface/empty.h>
#include <di/function/not_fn.h>
#include <dius/print.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 1, a, i32) {
    auto lines = input | split(U'\n') | filter(not_fn(empty));
    auto nums = lines | transform([](StringView string) {
                    auto filtered = string | filter(U'0'_m - U'9'_m);
                    return (*filtered.front() - U'0') * 10 + (*filtered.back() - U'0');
                });
    return sum(nums);
}

AOC_SOLUTION(2023, 1, b, i32) {
    auto lines = input | split(U'\n') | filter(not_fn(empty));

    auto get_num = [](StringView line) {
        auto nums = Array {
            "one"_sv, "two"_sv, "three"_sv, "four"_sv, "five"_sv, "six"_sv, "seven"_sv, "eight"_sv, "nine"_sv,
        };

        auto digits = Vector<i32> {};
        for (auto it = line.begin(); it != line.end(); ++it) {
            auto part = reconstruct(it, line.end());
            for (auto [i, num] : enumerate(nums)) {
                if (starts_with(part, num)) {
                    digits.push_back(i + 1);
                }
            }
            if (*part.front() >= U'0' && *part.front() <= U'9') {
                digits.push_back(*part.front() - U'0');
            }
        }

        return *digits.front() * 10 + *digits.back();
    };

    return sum(transform(lines, get_num));
}
