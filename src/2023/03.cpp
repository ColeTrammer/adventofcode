#include "di/container/algorithm/product.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/chunk_by.h"
#include "di/function/equal.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 3, a, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto sum = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto groups = line | enumerate | chunk_by(proj(tget<1> | is_digit, equal));

        for (auto const& group : groups) {
            auto digits = group | transform(tget<1>);
            for (auto number : parse_int(digits)) {
                auto col = get<0>(*group.front());
                auto col_end = col + group.size();

                for (auto [r, c, value] : neighbors(lines, row, range(col, col_end))) {
                    if (value != '.' && !is_digit(value)) {
                        sum += number;
                        break;
                    }
                }
            }
        }
    }
    return sum;
}

AOC_SOLUTION(2023, 3, b, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto sum = 0;
    auto map = di::TreeMap<di::Tuple<usize, usize>, di::Vector<i32>> {};
    for (auto [row, line] : enumerate(lines)) {
        auto groups = line | enumerate | chunk_by(proj(tget<1> | is_digit, equal));

        for (auto const& group : groups) {
            auto digits = group | transform(tget<1>);
            for (auto number : parse_int(digits)) {
                auto col = get<0>(*group.front());
                auto col_end = col + group.size();

                for (auto [r, c, value] : neighbors(lines, row, range(col, col_end))) {
                    if (value == '*') {
                        map[{ r, c }].push_back(number);
                        break;
                    }
                }
            }
        }
    }

    for (auto const& [_, nums] : map) {
        if (nums.size() > 1) {
            sum += product(nums);
        }
    }
    return sum;
}
