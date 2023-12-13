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

AOC_SOLUTION(2023, 13, a, i64) {
    auto grids = input | splitv("\n\n"_tsv);

    auto s1 = 0;
    auto s2 = 0;
    for (auto& grid : grids) {
        auto lines = grid | splitv('\n');

        auto const rows = lines.size();
        auto const cols = lines[0].size();

        for (auto x : range(1zu, cols)) {
            auto reflection = true;
            for (auto c : range(x)) {
                auto cc = x + (x - c) - 1;
                // println("{} {} {}"_sv, x, c, cc);
                if (cc >= cols) {
                    continue;
                }
                for (auto r : range(rows)) {
                    if (lines[r][c] != lines[r][cc]) {
                        reflection = false;
                        break;
                    }
                }
            }

            if (reflection) {
                s1 += x;
                // println("S1+: {}"_sv, s1);
                break;
            }
        }

        for (auto y : range(1zu, rows)) {
            auto reflection = true;
            for (auto r : range(y)) {
                auto rr = y + (y - r) - 1;
                // println("{} {} {}"_sv, y, r, rr);
                if (rr >= rows) {
                    continue;
                }
                for (auto c : range(cols)) {
                    if (lines[r][c] != lines[rr][c]) {
                        reflection = false;
                        break;
                    }
                }
            }

            if (reflection) {
                s2 += y;
                // println("S1+: {}"_sv, s1);
                break;
            }
        }
    }
    return s1 + 100 * s2;
}

AOC_SOLUTION(2023, 13, b, i64) {
    auto grids = input | splitv("\n\n"_tsv);

    auto s1 = 0;
    auto s2 = 0;
    for (auto& grid : grids) {
        auto lines = grid | splitv('\n');

        auto const rows = lines.size();
        auto const cols = lines[0].size();

        for (auto x : range(1zu, cols)) {
            auto reflection = 0;
            for (auto c : range(x)) {
                auto cc = x + (x - c) - 1;
                // println("{} {} {}"_sv, x, c, cc);
                if (cc >= cols) {
                    continue;
                }
                for (auto r : range(rows)) {
                    if (lines[r][c] != lines[r][cc]) {
                        reflection++;
                    }
                }
            }

            if (reflection == 1) {
                s1 += x;
                // println("S1+: {}"_sv, s1);
                break;
            }
        }

        for (auto y : range(1zu, rows)) {
            auto reflection = 0;
            for (auto r : range(y)) {
                auto rr = y + (y - r) - 1;
                // println("{} {} {}"_sv, y, r, rr);
                if (rr >= rows) {
                    continue;
                }
                for (auto c : range(cols)) {
                    if (lines[r][c] != lines[rr][c]) {
                        reflection++;
                    }
                }
            }

            if (reflection == 1) {
                s2 += y;
                // println("S1+: {}"_sv, s1);
                break;
            }
        }
    }
    return s1 + 100 * s2;
}
