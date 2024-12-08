#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 3, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto r = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (auto i = 0zu; i < line.size();) {
            if (line.substr(i).starts_with("mul("_ts)) {
                i += 4;
                auto s = i;
                if (!is_digit(line[i])) {
                    continue;
                }
                while (i < line.size() - 2 && is_digit(line[i])) {
                    i++;
                }
                if (line[i] != ',') {
                    continue;
                }
                auto ss = ++i;
                while (i < line.size() - 1 && is_digit(line[i])) {
                    i++;
                }

                if (line[i] != ')') {
                    continue;
                }

                auto a = uparse_int(line.substr(s, ss - s - 1));
                auto b = uparse_int(line.substr(ss, i - ss));
                r += a * b;
            } else {
                i++;
            }
        }
    }
    return r;
}

AOC_SOLUTION(2024, 3, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto r = 0;
    auto y = true;
    for (auto [row, line] : enumerate(lines)) {
        for (auto i = 0zu; i < line.size();) {
            if (line.substr(i).starts_with("mul("_ts) && y) {
                i += 4;
                auto s = i;
                if (!is_digit(line[i])) {
                    continue;
                }
                while (i < line.size() - 2 && is_digit(line[i])) {
                    i++;
                }
                if (line[i] != ',') {
                    continue;
                }
                auto ss = ++i;
                while (i < line.size() - 1 && is_digit(line[i])) {
                    i++;
                }

                if (line[i] != ')') {
                    continue;
                }

                auto a = uparse_int(line.substr(s, ss - s - 1));
                auto b = uparse_int(line.substr(ss, i - ss));
                r += a * b;
            } else if (line.substr(i).starts_with("do()"_tsv)) {
                y = true;
                i += 4;
            } else if (line.substr(i).starts_with("don't()"_tsv)) {
                y = false;
                i += 7;
            } else {
                i++;
            }
        }
    }
    return r;
}
