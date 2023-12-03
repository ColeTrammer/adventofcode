#include <di/container/algorithm/product.h>
#include <runner/aoc_problem_registry.h>

#include <di/container/interface/empty.h>
#include <di/container/tree/tree_set.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 3, a, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto sum = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (usize col = 0; col < line.size(); col++) {
            usize num_end = col;
            for (; num_end < line.size(); num_end++) {
                if (!('0'_mc - '9'_mc)(line[num_end])) {
                    break;
                }
            }
            if (num_end != col) {
                auto num = parse_unchecked<i32>(Tsv { line.begin() + col, line.begin() + num_end });

                bool valid = false;
                for (usize c : range(col, num_end)) {
                    for (auto dr : Array { -1, 0, 1 }) {
                        for (auto dc : Array { -1, 0, 1 }) {
                            auto ch = lines.at(row + dr).value_or(line).span().at(dc + c).value_or('.');
                            if (!('0'_mc - '9'_mc || '.'_mc)(ch)) {
                                valid = true;
                            }
                        }
                    }
                }

                if (valid) {
                    sum += num;
                }
            }
            col = num_end;
        }
    }
    return sum;
}

AOC_SOLUTION(2023, 3, b, i32) {
    auto lines = input | split('\n') | to<Vector>();

    auto sum = 0;
    auto map = TreeMap<Tuple<usize, usize>, TreeSet<Tuple<usize, usize, i32>>> {};
    for (auto [row, line] : enumerate(lines)) {
        for (usize col = 0; col < line.size(); col++) {
            usize num_end = col;
            for (; num_end < line.size(); num_end++) {
                if (!('0'_mc - '9'_mc)(line[num_end])) {
                    break;
                }
            }
            if (num_end != col) {
                auto num = parse_unchecked<i32>(Tsv { line.begin() + col, line.begin() + num_end });

                for (usize c : range(col, num_end)) {
                    for (auto dr : Array { -1, 0, 1 }) {
                        for (auto dc : Array { -1, 0, 1 }) {
                            auto ch = lines.at(row + dr).value_or(line).span().at(dc + c).value_or('.');
                            if (ch == '*') {
                                map[{ row + dr, dc + c }].insert({ row, col, num });
                            }
                        }
                    }
                }
            }
            col = num_end;
        }
    }

    for (auto const& [_, set] : map) {
        if (set.size() > 1) {
            sum += product(set | transform([](auto const& tuple) {
                               return get<2>(tuple);
                           }));
        }
    }

    return sum;
}
