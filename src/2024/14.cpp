#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 14, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    i64 W = is_test ? 11 : 101;
    i64 H = is_test ? 7 : 103;
    i64 T = 100;

    auto q = Array { 0_i64, 0_i64, 0_i64, 0_i64 };
    auto ps = Map<Coord, usize> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [axs, ays] = split_two(line.view(), " "_tsv);
        auto p = all_nums(axs.substr(2), ',');
        auto v = all_nums(ays.substr(2), ',');

        auto px = p[0];
        auto py = p[1];
        auto vx = v[0];
        auto vy = v[1];

        auto fx = (px + vx * T) % W;
        auto fy = (py + vy * T) % H;
        if (fx < 0) {
            fx += W;
        }
        if (fy < 0) {
            fy += H;
        }

        if (fx < W / 2 && fy < H / 2) {
            q[0]++;
        } else if (fx < W / 2 && fy >= H / 2 + 1) {
            q[1]++;
        } else if (fx >= W / 2 + 1 && fy < H / 2) {
            q[2]++;
        } else if (fx >= W / 2 + 1 && fy >= H / 2 + 1) {
            q[3]++;
        }
        ps[{ fy, fx }]++;
    }

    if (verbose) {
        for (auto r : range(H)) {
            for (auto c : range(W)) {
                if (ps[{ r, c }]) {
                    print("{}"_sv, ps[{ r, c }]);
                } else {
                    print("."_sv);
                }
            }
            println(""_sv);
        }

        println("{}"_sv, q);
    }
    return product(q);
}

AOC_SOLUTION(2024, 14, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    i64 W = is_test ? 11 : 101;
    i64 H = is_test ? 7 : 103;

    for (i64 T = 0; T < 20000; T++) {
        auto ps = Map<Coord, usize> {};
        for (auto [row, line] : enumerate(lines)) {
            auto [axs, ays] = split_two(line.view(), " "_tsv);
            auto p = all_nums(axs.substr(2), ',');
            auto v = all_nums(ays.substr(2), ',');

            auto px = p[0];
            auto py = p[1];
            auto vx = v[0];
            auto vy = v[1];

            auto fx = (px + vx * T) % W;
            auto fy = (py + vy * T) % H;
            if (fx < 0) {
                fx += W;
            }
            if (fy < 0) {
                fy += H;
            }

            ps[{ fy, fx }]++;
        }

        // Incidentally, the christmas tree appears when all robots have
        // a unique position.
        if (all_of(values(ps), equal(1))) {
            return T;
        }
    }

    return 0;
}
