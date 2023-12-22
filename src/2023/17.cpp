#include <di/container/interface/empty.h>
#include <di/container/queue/priority_queue.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/compare_backwards.h>
#include <di/function/not_fn.h>
#include <di/math/abs_diff.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 17, a, i64) {
    auto lines = input | splitv('\n');

    using State = Tuple<i32, i32, i32, i32, i32>;
    using Scored = Tuple<i32, State>;

    auto q = PriorityQueue<Scored, Vector<Scored>, CompareBackwards> {};
    auto v = TreeSet<State> {};

    auto add = [&](Scored s) {
        if (!v.contains(tget<1>(s))) {
            q.push(s);
            v.insert(tget<1>(s));
        }
    };
    add({ 0, State { 0, 0, 1, 0, 0 } });
    add({ 0, State { 0, 0, 0, 1, 0 } });

    auto b = NumericLimits<i32>::max;
    while (!q.empty()) {
        auto [s, st] = *q.pop();
        auto [x, y, dx, dy, c] = st;
        if (x == ssize(lines[0]) - 1 && y == ssize(lines) - 1) {
            return s;
        }

        for (auto [ndx, ndy] : Array { Tuple { -1, 0 }, Tuple { 1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
            auto nx = x + ndx;
            auto ny = y + ndy;
            if (nx < 0 || nx >= ssize(lines[0]) || ny < 0 || ny >= ssize(lines)) {
                continue;
            }

            if (ndx == dx && ndy == dy) {
                if (c == 3) {
                    continue;
                }
                add({ s + lines[ny][nx] - '0', State { nx, ny, ndx, ndy, c + 1 } });
                continue;
            }
            if (abs_diff(ndx, dx) < 2 && abs_diff(ndy, dy) < 2) {
                add({ s + lines[ny][nx] - '0', State { nx, ny, ndx, ndy, 1 } });
            }
        }
    }
    return b;
}

AOC_SOLUTION(2023, 17, b, i64) {
    auto lines = input | splitv('\n');

    using State = Tuple<i32, i32, i32, i32, i32>;
    using Scored = Tuple<i32, State>;

    auto q = PriorityQueue<Scored, Vector<Scored>, CompareBackwards> {};
    auto v = TreeSet<State> {};

    auto add = [&](Scored s) {
        if (!v.contains(tget<1>(s))) {
            q.push(s);
            v.insert(tget<1>(s));
        }
    };
    add({ 0, State { 0, 0, 1, 0, 0 } });
    add({ 0, State { 0, 0, 0, 1, 0 } });

    auto b = NumericLimits<i32>::max;
    while (!q.empty()) {
        auto [s, st] = *q.pop();
        auto [x, y, dx, dy, c] = st;
        if (x == ssize(lines[0]) - 1 && y == ssize(lines) - 1) {
            if (c < 4) {
                continue;
            }
            return s;
        }

        for (auto [ndx, ndy] : Array { Tuple { -1, 0 }, Tuple { 1, 0 }, Tuple { 0, 1 }, Tuple { 0, -1 } }) {
            auto nx = x + ndx;
            auto ny = y + ndy;
            if (nx < 0 || nx >= ssize(lines[0]) || ny < 0 || ny >= ssize(lines)) {
                continue;
            }

            if (ndx == dx && ndy == dy) {
                if (c == 10) {
                    continue;
                }
                add({ s + lines[ny][nx] - '0', State { nx, ny, ndx, ndy, c + 1 } });
                continue;
            }
            if (c >= 4 && abs_diff(ndx, dx) < 2 && abs_diff(ndy, dy) < 2) {
                add({ s + lines[ny][nx] - '0', State { nx, ny, ndx, ndy, 1 } });
            }
        }
    }
    return b;
}
