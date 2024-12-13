#include "di/math/functions.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 13, a, i64) {
    auto lines = input | splitv("\n\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto ls = line | splitv('\n');

        auto as = ls[0].substr(ls[0].find(':').begin() + 1);
        auto [axs, ays] = split_two(as, ", "_tsv);
        auto ax = uparse_int(axs.substr(2));
        auto ay = uparse_int(ays.substr(2));

        auto bs = ls[1].substr(ls[1].find(':').begin() + 1);
        auto [bxs, bys] = split_two(bs, ", "_tsv);
        auto bx = uparse_int(bxs.substr(2));
        auto by = uparse_int(bys.substr(2));

        auto cs = ls[2].substr(ls[2].find(':').begin() + 1);
        auto [cxs, cys] = split_two(cs, ", "_tsv);
        auto cx = uparse_int(cxs.substr(3));
        auto cy = uparse_int(cys.substr(2));

        auto cost = Optional<i64> {};
        for (auto x : range(100)) {
            for (auto y : range(100)) {
                auto ar = x * ax + y * bx;
                auto br = x * ay + y * by;
                if (ar == cx && br == cy) {
                    auto c = 3 * x + y;
                    if (!cost) {
                        cost = c;
                    } else {
                        cost = min(cost.value(), (i64) c);
                    }
                }
            }
        }

        if (cost) {
            s += *cost;
        }
    }
    return s;
}

AOC_SOLUTION(2024, 13, b, i64) {
    auto lines = input | splitv("\n\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto ls = line | splitv('\n');

        auto as = ls[0].substr(ls[0].find(':').begin() + 1);
        auto [axs, ays] = split_two(as, ", "_tsv);
        auto ax = (double) uparse_int(axs.substr(2));
        auto ay = (double) uparse_int(ays.substr(2));

        auto bs = ls[1].substr(ls[1].find(':').begin() + 1);
        auto [bxs, bys] = split_two(bs, ", "_tsv);
        auto bx = (double) uparse_int(bxs.substr(2));
        auto by = (double) uparse_int(bys.substr(2));

        auto cs = ls[2].substr(ls[2].find(':').begin() + 1);
        auto [cxs, cys] = split_two(cs, ", "_tsv);
        auto cx = (double) uparse_int(cxs.substr(3)) + 10000000000000;
        auto cy = (double) uparse_int(cys.substr(2)) + 10000000000000;

        // ax*T + bx*Q = cx
        // ay*T + by*Q = cy
        //   => by*Q = cy - ay*T
        //   => Q = (cy - ay*T) / by
        // ax*T + bx* (cy - ay*T) / by = cx
        // => ax*T + bx*cy/by - ay*bx*T/by = cx
        // => (ax - ay*bx/by) * T = cx - bx*cy/by
        // => T = cx/ax - cx*by/ay/bx - bx*cy/by * ax + cx*cy/ay

        auto T = round((cx - bx * cy / by) / (ax - ay * bx / by));
        auto Q = round((cy - ay * T) / by);

        if (verbose) {
            println("T={} Q={} ax*T + bx*Q={}"_sv, (i64) T, (i64) Q, (i64) ax * (i64) T + (i64) bx * (i64) Q);
        }

        if ((i64) ax * (i64) T + (i64) bx * (i64) Q == (i64) cx &&
            (i64) ay * (i64) T + (i64) by * (i64) Q == (i64) cy) {
            s += 3 * (i64) T + (i64) Q;
        }

        if (verbose) {
            println("a: {} {}"_sv, (i64) ax, (i64) ay);
            println("b: {} {}"_sv, (i64) bx, (i64) by);
            println("c: {} {}"_sv, (i64) cx, (i64) cy);
        }
    }
    return s;
}
