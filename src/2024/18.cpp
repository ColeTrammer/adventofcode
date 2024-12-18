#include "di/math/midpoint.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 18, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto mems = Set<Coord> {};

    u64 H = is_test ? 7 : 71;
    u64 W = is_test ? 7 : 71;
    u64 N = is_test ? 12 : 1024;

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        if (row >= N) {
            break;
        }

        for (auto [x, y] : all_nums(line, ',') | pairwise) {
            mems.insert({ y, x });
        }
    }

    auto queue = Vec<Coord> {};
    auto vs = Set<Coord> {};

    auto add = [&](Coord c) {
        auto [row, col] = c;
        if (row >= H || col >= W) {
            return;
        }

        if (vs.contains(c) || mems.contains(c)) {
            return;
        }

        vs.insert(c);
        queue.push_back(c);
    };

    add({ 0, 0 });

    while (!queue.empty()) {
        auto cqueue = di::move(queue);
        for (auto [row, col] : cqueue) {
            if (row == H - 1 && col == W - 1) {
                return s;
            }

            for (auto [dr, dc] : Array { Tuple { -1, 0 }, Tuple { 1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
                add({ row + dr, col + dc });
            }
        }

        s++;
    }

    return -1;
}

AOC_SOLUTION(2024, 18, b, String) {
    auto lines = input | splitv("\n"_tsv);

    auto mems = Set<Coord> {};
    auto mv = Vec<Coord> {};

    u64 H = is_test ? 7 : 71;
    u64 W = is_test ? 7 : 71;
    u64 N = is_test ? 12 : 1024;

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [x, y] : all_nums(line, ',') | pairwise) {
            mv.push_back({ y, x });
        }
    }

    auto bfs = [&] {
        auto queue = Vec<Coord> {};
        auto vs = Set<Coord> {};

        auto add = [&](Coord c) {
            auto [row, col] = c;
            if (row >= H || col >= W) {
                return;
            }

            if (vs.contains(c) || mems.contains(c)) {
                return;
            }

            vs.insert(c);
            queue.push_back(c);
        };

        add({ 0, 0 });

        while (!queue.empty()) {
            auto cqueue = di::move(queue);
            for (auto [row, col] : cqueue) {
                if (row == H - 1 && col == W - 1) {
                    return true;
                }

                for (auto [dr, dc] : Array { Tuple { -1, 0 }, Tuple { 1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
                    add({ row + dr, col + dc });
                }
            }

            s++;
        }
        return false;
    };

    u64 lo = 0;
    u64 hi = mv.size();

    while (lo < hi) {
        auto mid = midpoint(lo, hi);

        mems.clear();
        mems.insert_container(*mv.subspan(0, mid));
        auto good = bfs();

        if (good) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return *di::present("{},{}"_sv, tget<1>(mv[lo - 1]), tget<0>(mv[lo - 1]));
}
