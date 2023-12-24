#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/not_fn.h>
#include <di/io/writer_println.h>
#include <di/math/gcd.h>
#include <dius/print.h>
#include <dius/sync_file.h>
#include <dius/system/process.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 24, a, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<double, double, double>;
    auto ls = Vector<Tuple<Point, Point>> {};

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        erase_if(line, '@'_mc || ','_mc);
        auto nums = all_nums_i<i64>(line);

        auto x = nums[0];
        auto y = nums[1];
        auto z = nums[2];

        auto x2 = nums[3];
        auto y2 = nums[4];
        auto z2 = nums[5];

        ls.push_back({ Point { x, y, z }, Point { x2, y2, z2 } });
    }

    for (auto i : range(ls.size() - 1)) {
        for (auto j : range(i + 1, ls.size())) {
            auto [x1, y1, z1] = tget<0>(ls[i]);
            auto [dx1, dy1, dz1] = tget<1>(ls[i]);
            auto [x2, y2, z2] = tget<0>(ls[j]);
            auto [dx2, dy2, dz2] = tget<1>(ls[j]);

            auto t2 = ((y2 - y1) - (x2 - x1) * dy1 / dx1) / (dx2 / dx1 * dy1 - dy2);
            auto t1 = (x2 - x1 + t2 * dx2) / dx1;

            auto x = t1 * dx1 + x1;
            auto y = t1 * dy1 + y1;

            if (t1 < 0 || t2 < 0) {
                continue;
            }

            constexpr auto tlow = 200000000000000;
            constexpr auto thigh = 400000000000000;
            if (x >= tlow && x <= thigh && y >= tlow && y <= thigh) {
                s++;
            }
        }
    }
    return s;
}

AOC_SOLUTION(2023, 24, b, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<i64, i64, i64>;
    auto ls = Vector<Tuple<Point, Point>> {};

    auto vs = TreeSet<Point> {};
    for (auto [row, line] : enumerate(lines)) {
        erase_if(line, '@'_mc || ','_mc);
        auto nums = all_nums_i<i64>(line);

        auto x = nums[0];
        auto y = nums[1];
        auto z = nums[2];

        auto x2 = nums[3];
        auto y2 = nums[4];
        auto z2 = nums[5];

        ls.push_back({ Point { x, y, z }, Point { x2, y2, z2 } });
    }

    // L1: t1 v + p = t1 v1 + p1
    //  => (p - p1) = -t1 (v - v1)
    //  => (p - p1) x (v - v1) = 0
    // L2: t2 v + p = t2 v2 + p2
    //  => (p - p2) = -t2 (v - v2)
    //  => (p - p2) x (v - v2) = 0
    // L3: t3 v + p = t3 v3 + p3
    //  => (p - p3) = -t3 (v - v3)
    //  => (p - p3) x (v - v3) = 0

    // Combine to get 6 equations for 6 unknowns:
    // (p - p1) x (v - v1) = (p - p2) x (v - v2)
    // (p - p1) x (v - v1) = (p - p3) x (v - v3)

    auto matrix = Array<Array<double, 7>, 6> {};

    auto equation_for = [&](Point p1, Point v1, Point p2, Point v2) {
        auto [x1, y1, z1] = p1;
        auto [vx1, vy1, vz1] = v1;
        auto [x2, y2, z2] = p2;
        auto [vx2, vy2, vz2] = v2;

        return Array {
            Array { 0_i64, -(vz1 - vz2), vy1 - vy2, 0_i64, z1 - z2, -(y1 - y2),
                    y2 * vz2 - z2 * vy2 - (y1 * vz1 - z1 * vy1) },
            Array { vz1 - vz2, 0_u64, -(vx1 - vx2), -(z1 - z2), 0_i64, x1 - x2,
                    z2 * vx2 - x2 * vz2 - (z1 * vx1 - x1 * vz1) },
            Array { -(vy1 - vy2), vx1 - vx2, 0_i64, y1 - y2, -(x1 - x2), 0_i64,
                    x2 * vy2 - y2 * vx2 - (x1 * vy1 - y1 * vx1) },
        };
    };

    auto [p1, v1] = ls[0];
    auto [p2, v2] = ls[1];
    auto [p3, v3] = ls[2];

    auto a = equation_for(p1, v1, p2, v2);
    auto b = equation_for(p1, v1, p3, v3);

    for (auto [r, row] : enumerate(a)) {
        for (auto [c, v] : enumerate(row)) {
            matrix[r][c] = v;
        }
    }
    for (auto [r, row] : enumerate(b)) {
        for (auto [c, v] : enumerate(row)) {
            matrix[r + 3][c] = v;
        }
    }

    auto ffabs = [](double x) {
        if (x < 0) {
            return -x;
        }
        return x;
    };

    auto h = 0_usize;
    auto k = 0_usize;
    while (h < 6 && k < 7) {
        auto i_max = h;
        for (auto i = h + 1; i < 6; i++) {
            if (ffabs(matrix[i][k]) > ffabs(matrix[i_max][k])) {
                i_max = i;
            }
        }

        if (matrix[i_max][k] == 0) {
            ++k;
            continue;
        } else {
            swap(matrix[h], matrix[i_max]);

            for (auto i = h + 1; i < 6; i++) {
                auto f = matrix[i][k] / matrix[h][k];
                matrix[i][k] = 0;
                for (auto j = k + 1; j < 7; j++) {
                    matrix[i][j] -= f * matrix[h][j];
                }
            }

            ++h;
            ++k;
        }
    }

    for (auto row : range(6) | reverse) {
        for (auto c : range(row + 1, 6)) {
            auto v = matrix[row][c];
            matrix[row][c] = 0;
            matrix[row][6] -= v * matrix[c][6];
        }
        auto vv = matrix[row][row];
        for (auto& v : matrix[row]) {
            v /= vv;
        }
    }

    auto x = i64(matrix[0][6] + 0.4);
    auto y = i64(matrix[1][6] + 0.4);
    auto z = i64(matrix[2][6] + 0.4);

    return x + y + z;
}
