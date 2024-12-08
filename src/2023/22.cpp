#include "di/container/algorithm/minmax.h"
#include "di/container/algorithm/sort.h"
#include "di/container/hash/node/node_hash_map.h"
#include "di/container/hash/node/node_hash_set.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/cartesian_product.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/compare_backwards.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 22, a, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<isize, isize, isize>;
    using Line = Tuple<Point, Point>;

    auto blocks = Vector<Line> {};

    for (auto [row, line] : enumerate(lines)) {
        auto [a, b] = split_two(line.view(), '~');
        auto ax = uparse_i<isize>(splitv(a, ',')[0]);
        auto ay = uparse_i<isize>(splitv(a, ',')[1]);
        auto az = uparse_i<isize>(splitv(a, ',')[2]);

        auto bx = uparse_i<isize>(splitv(b, ',')[0]);
        auto by = uparse_i<isize>(splitv(b, ',')[1]);
        auto bz = uparse_i<isize>(splitv(b, ',')[2]);

        blocks.push_back({ Point { ax, ay, az }, Point { bx, by, bz } });
    }

    auto points_in_line = [&](Line const& l) -> Vector<Point> {
        auto [a, b] = l;
        auto [c, d] = minmax(a, b);
        auto result = Vector<Point> {};

        auto [ax, ay, az] = c;
        auto [bx, by, bz] = d;
        for (auto [x, y, z] : cartesian_product(range(ax, bx + 1), range(ay, by + 1), range(az, bz + 1))) {
            result.push_back({ x, y, z });
        }

        return result;
    };

    auto line_z_compare = [](Line const& l1, Line const& l2) {
        auto [a1, a2] = l1;
        auto [b1, b2] = l2;
        auto [ax1, ay1, az1] = a1;
        auto [ax2, ay2, az2] = a2;
        auto [bx1, by1, bz1] = b1;
        auto [bx2, by2, bz2] = b2;
        return Tuple { min(az1, az2), max(az1, az2), min(ax1, ax2), max(ax1, ax2), min(ay1, ay2), max(ay1, ay2) } <=>
               Tuple { min(bz1, bz2), max(bz1, bz2), min(bx1, bx2), max(bx1, bx2), min(by1, by2), max(by1, by2) };
    };

    sort(blocks, line_z_compare);

    auto lines_from_points = TreeMap<Point, Line*> {};

    auto make_map = [&] {
        lines_from_points = TreeMap<Point, Line*> {};
        for (auto& line : blocks) {
            for (auto [x, y, z] : points_in_line(line)) {
                lines_from_points[{ x, y, z }] = &line;
            }
        }
    };

    make_map();

    auto simulate = [&] {
        for (auto& line : blocks) {
            for (;;) {
                auto should_fall = true;
                for (auto [x, y, z] : points_in_line(line)) {
                    if (z <= 1 ||
                        (lines_from_points.contains({ x, y, z - 1 }) && lines_from_points[{ x, y, z - 1 }] != &line)) {
                        should_fall = false;
                        break;
                    }
                }

                if (!should_fall) {
                    break;
                }

                for (auto [x, y, z] : points_in_line(line)) {
                    lines_from_points.erase({ x, y, z });
                }
                for (auto [x, y, z] : points_in_line(line)) {
                    lines_from_points[{ x, y, z - 1 }] = &line;
                }

                tget<2>(tget<0>(line))--;
                tget<2>(tget<1>(line))--;
            }
        }
    };

    simulate();

    auto can_disintegrate = Vector<Line*> {};
    for (auto& line : blocks) {
        can_disintegrate.push_back(&line);
    }

    for (auto& line : blocks) {
        auto deps = TreeSet<Line*> {};
        for (auto [x, y, z] : points_in_line(line)) {
            if (lines_from_points.contains({ x, y, z - 1 }) && &line != lines_from_points[{ x, y, z - 1 }]) {
                deps.insert(lines_from_points[{ x, y, z - 1 }]);
            }
        }

        if (deps.size() == 1) {
            di::erase(can_disintegrate, first(deps));
        }
    }

    return can_disintegrate.size();
}

AOC_SOLUTION(2023, 22, b, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<isize, isize, isize>;
    using Line = Tuple<Point, Point>;

    auto blocks = Vector<Line> {};

    for (auto [row, line] : enumerate(lines)) {
        auto [a, b] = split_two(line.view(), '~');
        auto ax = uparse_i<isize>(splitv(a, ',')[0]);
        auto ay = uparse_i<isize>(splitv(a, ',')[1]);
        auto az = uparse_i<isize>(splitv(a, ',')[2]);

        auto bx = uparse_i<isize>(splitv(b, ',')[0]);
        auto by = uparse_i<isize>(splitv(b, ',')[1]);
        auto bz = uparse_i<isize>(splitv(b, ',')[2]);

        blocks.push_back({ Point { ax, ay, az }, Point { bx, by, bz } });
    }

    auto points_in_line = [&](Line const& l) -> Vector<Point> {
        auto [a, b] = l;
        auto [c, d] = minmax(a, b);
        auto result = Vector<Point> {};

        auto [ax, ay, az] = c;
        auto [bx, by, bz] = d;
        for (auto [x, y, z] : cartesian_product(range(ax, bx + 1), range(ay, by + 1), range(az, bz + 1))) {
            result.push_back({ x, y, z });
        }

        return result;
    };

    auto line_z_compare = [](Line const& l1, Line const& l2) {
        auto [a1, a2] = l1;
        auto [b1, b2] = l2;
        auto [ax1, ay1, az1] = a1;
        auto [ax2, ay2, az2] = a2;
        auto [bx1, by1, bz1] = b1;
        auto [bx2, by2, bz2] = b2;
        return Tuple { min(az1, az2), max(az1, az2), min(ax1, ax2), max(ax1, ax2), min(ay1, ay2), max(ay1, ay2) } <=>
               Tuple { min(bz1, bz2), max(bz1, bz2), min(bx1, bx2), max(bx1, bx2), min(by1, by2), max(by1, by2) };
    };

    sort(blocks, line_z_compare);

    auto lines_from_points = TreeMap<Point, Line*> {};

    auto make_map = [&] {
        lines_from_points = TreeMap<Point, Line*> {};
        for (auto& line : blocks) {
            for (auto [x, y, z] : points_in_line(line)) {
                lines_from_points[{ x, y, z }] = &line;
            }
        }
    };

    make_map();

    auto simulate = [&]() -> i64 {
        auto res = 0_i64;
        for (auto& line : blocks) {
            auto did_fall = false;
            for (;;) {
                auto should_fall = true;
                for (auto [x, y, z] : points_in_line(line)) {
                    if (z <= 1 ||
                        (lines_from_points.contains({ x, y, z - 1 }) && lines_from_points[{ x, y, z - 1 }] != &line)) {
                        should_fall = false;
                        break;
                    }
                }

                if (!should_fall) {
                    break;
                }
                did_fall = true;

                for (auto [x, y, z] : points_in_line(line)) {
                    lines_from_points.erase({ x, y, z });
                }
                for (auto [x, y, z] : points_in_line(line)) {
                    lines_from_points[{ x, y, z - 1 }] = &line;
                }

                tget<2>(tget<0>(line))--;
                tget<2>(tget<1>(line))--;
            }

            if (did_fall) {
                ++res;
            }
        }
        return res;
    };

    simulate();

    auto s = 0_i64;
    for (auto i : range(blocks.size())) {
        auto new_lines = di::clone(blocks);
        new_lines.erase(new_lines.begin() + i);

        auto old_blocks = exchange(blocks, di::move(new_lines));
        make_map();
        s += simulate();

        blocks = di::move(old_blocks);
    }

    return s;
}
