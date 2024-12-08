#include "di/container/algorithm/minmax.h"
#include "di/container/algorithm/remove_if.h"
#include "di/container/algorithm/sort.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/adjacent.h"
#include "di/container/view/cartesian_product.h"
#include "di/container/view/split.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/not_fn.h"
#include "di/function/uncurry.h"
#include "di/math/gcd.h"
#include "di/parser/parse_unchecked.h"
#include "di/types/integers.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 18, a, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<isize, isize>;
    auto cells = TreeSet<Point> {};

    auto x = 0z;
    auto y = 0z;

    auto xmin = NumericLimits<isize>::max;
    auto xmax = NumericLimits<isize>::min;
    auto ymin = NumericLimits<isize>::max;
    auto ymax = NumericLimits<isize>::min;

    auto add = [&] {
        cells.insert({ x, y });
        xmin = min(xmin, x);
        xmax = max(xmax, x);
        ymin = min(ymin, y);
        ymax = max(ymax, y);
    };
    add();

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [dir, dist, color] : line | split(' ') | adjacent<3zu>) {
            auto dis = uparse_i<isize>(dist);
            for (auto _ : range(dis)) {
                (void) _;
                switch (dir[0]) {
                    case 'U':
                        y--;
                        break;
                    case 'D':
                        y++;
                        break;
                    case 'L':
                        x--;
                        break;
                    case 'R':
                        x++;
                        break;
                }
                add();
            }
        }
    }

    for (auto [x, y] : cartesian_product(range(xmin, xmax + 1), range(ymin, ymax + 1))) {
        if (cells.contains({ x, y })) {
            s++;
            continue;
        }
        auto c = 0;
        auto b = false;
        while (++x <= xmax) {
            if (cells.contains({ x, y + 1 }) && cells.contains({ x, y })) {
                if (!b) {
                    c++;
                    b = true;
                }
            } else if (b) {
                b = false;
            }
        }
        if (c % 2 == 1) {
            s++;
        }
    }
    return s;
}

AOC_SOLUTION(2023, 18, b, i64) {
    auto lines = input | splitv('\n');
    using Point = Tuple<i64, i64>;
    using Line = Tuple<Point, Point>;

    auto ls = Vector<Line> {};

    auto s = 0_i64;
    {
        auto x = 0z;
        auto y = 0z;
        for (auto [row, line] : enumerate(lines)) {
            auto [_, color] = split_two(line.view(), '#');
            color = color.substr(0, color.size() - 1);

            auto dir = *color.back();
            auto hex_string = "0x"_ts;
            hex_string += color.substr(0, color.size() - 1);
            auto num = parse_unchecked<i64>(hex_string);

            auto ox = x;
            auto oy = y;

            switch (dir) {
                case '0':
                    x += num;
                    break;
                case '1':
                    y += num;
                    break;
                case '2':
                    x -= num;
                    break;
                case '3':
                    y -= num;
                    break;
            }

            auto d1 = Point { ox, oy };
            auto d2 = Point { x, y };
            auto [p1, p2] = minmax(d1, d2);

            ls.push_back(Line { p1, p2 });
        }
    }

    auto pxss = ls | transform([](Line line) {
                    return tget<0>(tget<0>(line));
                }) |
                to<TreeSet>();
    pxss.insert_container(ls | transform([](Line line) {
                              return tget<0>(tget<1>(line));
                          }));
    auto pxs = pxss | to<Vector>();

    auto vs = ls | filter([](auto line) {
                  auto [p1, p2] = line;
                  auto [x1, y1] = p1;
                  auto [x2, y2] = p2;
                  return x1 == x2;
              }) |
              to<Vector>();

    sort(ls);

    // Assume the shape starts like this:
    // ------
    // |
    // ------
    // The shapes area will only depend on the min and max y values of line segments.
    // We just need maintain a list of horizontal lines and update each time an x changes.
    // Also... This code doesn't count the vertical lines which are on the right of a rectange. As such, we maintain
    // a list of all vertical lines and remove the ones which are on the left of a box.

    auto x = pxs[0];
    for (auto nx : pxs | drop(1)) {
        auto ys = TreeSet<i64> {};
        for (auto [p1, p2] : ls | filter([&](auto line) {
                                 auto [p1, p2] = line;
                                 auto [x1, y1] = p1;
                                 auto [x2, y2] = p2;
                                 return (x1 <= x && x2 >= x) && (x1 <= nx && x2 >= nx);
                             })) {
            auto [x1, y1] = p1;
            auto [x2, y2] = p2;
            if (x1 != x2) {
                ys.insert(y1);
            }
        }

        auto dx = nx - x;
        for (auto [y1, y2] : pairwise(ys) | stride(2)) {
            auto dy = y2 - y1 + 1;
            s += dy * dx;

            auto [c, e] = di::container::remove_if(vs, [&](auto line) {
                auto [lp1, lp2] = line;
                auto [lx1, ly1] = lp1;
                auto [lx2, ly2] = lp2;
                if (lx1 < x || lx1 >= nx) {
                    return false;
                }
                return ly1 >= y1 && ly1 <= y2 && ly2 >= y1 && ly2 <= y2;
            });
            vs.erase(c, e);
        }
        x = nx;
    }

    for (auto [p1, p2] : vs) {
        auto [x1, y1] = p1;
        auto [x2, y2] = p2;
        auto dy = y2 - y1 + 1;
        // So... this assumes the uncounted vertical lines look like this:
        // ------------
        // |
        // |
        // The top most pixel of the vertical is already counted, since its a horizontal line.
        // This applies to all lines except for the rightmost.
        // Subtracting by 1 assumes that a vertical line cannot be part of 2 horizontal lines:
        // -----------
        // |
        // |
        // -----------
        // This is surprisingly true of both my input and the provided test input, but could be fixed by being checking
        // for horizontal line going rightward explicitly.
        if (x1 != pxs.back()) {
            dy -= 1;
        }
        s += dy;
    }
    return s;
}
