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

    auto s = Vector<String> {};
    s.push_back("from sympy import solve, Symbol"_s);
    s.push_back("f = []"_s);
    s.push_back("x = Symbol('x')"_s);
    s.push_back("y = Symbol('y')"_s);
    s.push_back("z = Symbol('z')"_s);
    s.push_back("vx = Symbol('vx')"_s);
    s.push_back("vy = Symbol('vy')"_s);
    s.push_back("vz = Symbol('vz')"_s);

    auto vars = Vector<String> {};
    vars.push_back("x"_s);
    vars.push_back("y"_s);
    vars.push_back("z"_s);
    vars.push_back("vx"_s);
    vars.push_back("vy"_s);
    vars.push_back("vz"_s);

    for (auto [i, l] : enumerate(ls)) {
        auto [p, v] = l;
        auto [x, y, z] = p;
        auto [vx, vy, vz] = v;

        s.push_back(*present("f.append(({} - z)/(vz - {}) - ({} - x)/(vx - {}))"_sv, z, vz, x, vx));
        s.push_back(*present("f.append(({} - z)/(vz - {}) - ({} - y)/(vy - {}))"_sv, z, vz, y, vy));
        s.push_back(*present("f.append(({} - y)/(vy - {}) - ({} - x)/(vx - {}))"_sv, y, vy, x, vx));
    }

    s.push_back(*present("r = solve([*f], [{}], dict=True, check=False)"_sv, vars | join_with(U',') | to<String>()));
    s.push_back(*present("s = r[0][x] + r[0][y] + r[0][z]"_sv));

    s.push_back(*present("file = open('/tmp/2023-24.txt', 'w')"_sv));
    s.push_back(*present("file.write(f'{{s}}')"_sv));
    s.push_back(*present("file.close()"_sv));

    auto file = *open_sync("/tmp/2023-24.py"_pv, OpenMode::WriteClobber);

    for (auto const& line : s) {
        di::writer_println<di::container::string::Utf8Encoding>(file, "{}"_sv, line);
    }

    ASSERT(dius::system::Process(Array { "/usr/bin/python"_ts, "/tmp/2023-24.py"_ts } | to<Vector>()).spawn_and_wait());

    auto result = *aoc::detail::read_to_string("/tmp/2023-24.txt"_pv);

    return uparse_i<i64>(result);
}
