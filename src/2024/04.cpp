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
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 4, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    for (auto [rw, line] : enumerate(lines)) {
        for (auto [cl, ch] : enumerate(line)) {
            for (auto dr : range(-1, 2)) {
                for (auto dc : range(-1, 2)) {
                    auto row = (int) rw;
                    auto col = (int) cl;
                    auto str = Ts {};
                    while ((usize) row < lines.size() && row >= 0 && (usize) col < lines[row].size() && col >= 0 &&
                           str.size() < 4) {
                        str.push_back(lines[row][col]);
                        row += dr;
                        col += dc;
                    }

                    if (str == "XMAS"_tsv) {
                        s++;
                    }
                }
            }
        }
    }
    return s;
}

AOC_SOLUTION(2024, 4, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0;
    for (auto [rw, line] : enumerate(lines)) {
        for (auto [cl, ch] : enumerate(line)) {
            if (rw == 0 || rw == lines.size() - 1 || cl == 0 || cl == lines[rw].size() - 1) {
                continue;
            }

            auto a = Ts {};
            a.push_back(lines[rw - 1][cl - 1]);
            a.push_back(lines[rw][cl]);
            a.push_back(lines[rw + 1][cl + 1]);

            auto b = Ts {};
            b.push_back(lines[rw - 1][cl + 1]);
            b.push_back(lines[rw][cl]);
            b.push_back(lines[rw + 1][cl - 1]);

            if (a == "MAS"_tsv || a == "SAM"_tsv) {
                if (b == "MAS"_tsv || b == "SAM"_tsv) {
                    s++;
                }
            }
        }
    }
    return s;
}
