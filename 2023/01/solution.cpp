#include <di/container/interface/reconstruct.h>
#include <di/container/view/drop.h>
#include <di/parser/parse_unchecked.h>
#include <runner/aoc_problem_registry.h>

#include <di/container/interface/empty.h>
#include <di/function/not_fn.h>
#include <dius/print.h>

AOC_SOLUTION(2023, 1, a, i32) {
    auto lines = input | di::split(U'\n') | di::filter(di::not_fn(di::empty));
    auto nums = lines | di::transform([](di::StringView string) {
                    auto filtered = string | di::filter([](auto ch) {
                                        return ch >= U'0' && ch <= U'9';
                                    }) |
                                    di::transform([](auto ch) {
                                        return c8(ch);
                                    }) |
                                    di::to<di::Vector>() | di::to<di::String>();
                    return (*filtered.front() - U'0') * 10 + (*filtered.back() - U'0');
                });
    return di::sum(nums);
}

AOC_SOLUTION(2023, 1, b, i32) {
    auto lines = input | di::split(U'\n') | di::filter(di::not_fn(di::empty));

    auto get_num = [](di::StringView line) {
        auto nums = di::Array {
            "one"_sv, "two"_sv, "three"_sv, "four"_sv, "five"_sv, "six"_sv, "seven"_sv, "eight"_sv, "nine"_sv,
        };

        auto digits = di::Vector<i32> {};
        for (auto it = line.begin(); it != line.end(); ++it) {
            auto part = di::reconstruct(it, line.end());
            for (auto [i, num] : di::enumerate(nums)) {
                if (di::starts_with(part, num)) {
                    digits.push_back(i + 1);
                }
            }
            if (*part.front() >= U'0' && *part.front() <= U'9') {
                digits.push_back(*part.front() - U'0');
            }
        }

        return *digits.front() * 10 + *digits.back();
    };

    return di::sum(di::transform(lines, get_num));
}
