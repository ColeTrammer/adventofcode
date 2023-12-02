#include <di/container/algorithm/product.h>
#include <di/container/view/pairwise.h>
#include <runner/aoc_problem_registry.h>

#include <di/container/interface/empty.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 2, a, i32) {
    auto lines = input | split('\n') | filter(not_fn(empty));

    auto sum = 0;
    for (auto [i, line] : enumerate(lines)) {
        auto colon = line.find(':');
        auto info = line.substr(colon.end());
        auto maxes = Array { 0, 0, 0 };

        for (auto round : split(info, ';')) {
            for (auto object : split(round, ',')) {
                object = object.substr(object.begin() + 1);
                for (auto [num, color] : split(object, ' ') | pairwise) {
                    auto x = parse_unchecked<i32>(num);

                    if (color == "red"_tsv) {
                        maxes[0] = max(maxes[0], x);
                    }
                    if (color == "green"_tsv) {
                        maxes[1] = max(maxes[1], x);
                    }
                    if (color == "blue"_tsv) {
                        maxes[2] = max(maxes[2], x);
                    }
                }
            }
        }

        if (maxes[0] <= 12 && maxes[1] <= 13 && maxes[2] <= 14) {
            sum += i + 1;
        }
    }
    return sum;
}

AOC_SOLUTION(2023, 2, b, i32) {
    auto lines = input | split('\n') | filter(not_fn(empty));

    auto sum = 0;
    for (auto [i, line] : enumerate(lines)) {
        auto colon = line.find(':');
        auto info = line.substr(colon.end());
        auto maxes = Array { 0, 0, 0 };

        for (auto round : split(info, ';')) {
            for (auto object : split(round, ',')) {
                object = object.substr(object.begin() + 1);
                for (auto [num, color] : split(object, ' ') | pairwise) {
                    auto x = parse_unchecked<i32>(num);

                    if (color == "red"_tsv) {
                        maxes[0] = max(maxes[0], x);
                    }
                    if (color == "green"_tsv) {
                        maxes[1] = max(maxes[1], x);
                    }
                    if (color == "blue"_tsv) {
                        maxes[2] = max(maxes[2], x);
                    }
                }
            }
        }

        sum += product(maxes);
    }
    return sum;
}
