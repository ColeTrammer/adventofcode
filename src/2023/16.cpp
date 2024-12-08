#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 16, a, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<usize, usize>;
    enum Direction { Right, Up, Left, Down };

    auto m = TreeSet<Point> {};

    auto q = Vector<Tuple<Point, Direction>> {};
    auto v = TreeSet<Tuple<Point, Direction>> {};

    auto add = [&](Point p, int dd) {
        auto d = static_cast<Direction>(dd);
        if (!v.contains({ p, d })) {
            q.push_back({ p, d });
            v.insert({ p, d });
            m.insert(p);
        }
    };

    add(Point { -1, 0 }, Direction::Right);

    while (!q.empty()) {
        auto items = di::move(q);
        for (auto [p, d] : items) {
            auto [x, y] = p;
            auto dx = Array { 1, 0, -1, 0 };
            auto dy = Array { 0, -1, 0, 1 };
            auto dslash = Array { Direction::Up, Direction::Right, Direction::Down, Direction::Left };
            auto dbslash = Array { Direction::Down, Direction::Left, Direction::Up, Direction::Right };

            auto nx = x + dx[d];
            auto ny = y + dy[d];
            if (nx >= lines[0].size() || ny >= lines.size()) {
                continue;
            }

            auto ch = lines[ny][nx];
            switch (ch) {
                case '.':
                    add({ nx, ny }, d);
                    break;
                case '/':
                    add({ nx, ny }, dslash[d]);
                    break;
                case '\\':
                    add({ nx, ny }, dbslash[d]);
                    break;
                case '|':
                    if (d % 2 == 0) {
                        add({ nx, ny }, Direction::Up);
                        add({ nx, ny }, Direction::Down);
                    } else {
                        add({ nx, ny }, d);
                    }
                    break;
                case '-':
                    if (d % 2 == 0) {
                        add({ nx, ny }, d);
                    } else {
                        add({ nx, ny }, Direction::Left);
                        add({ nx, ny }, Direction::Right);
                    }
                    break;
            }
        }
    }

    return m.size() - 1;
}

AOC_SOLUTION(2023, 16, b, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<usize, usize>;
    enum Direction { Right, Up, Left, Down };

    auto compute = [&](Point init, Direction dinit) {
        auto m = TreeSet<Point> {};

        auto q = Vector<Tuple<Point, Direction>> {};
        auto v = TreeSet<Tuple<Point, Direction>> {};

        auto add = [&](Point p, int dd) {
            auto d = static_cast<Direction>(dd);
            if (!v.contains({ p, d })) {
                q.push_back({ p, d });
                v.insert({ p, d });
                m.insert(p);
            }
        };

        add(init, dinit);

        while (!q.empty()) {
            auto items = di::move(q);
            for (auto [p, d] : items) {
                auto [x, y] = p;
                auto dx = Array { 1, 0, -1, 0 };
                auto dy = Array { 0, -1, 0, 1 };
                auto dslash = Array { Direction::Up, Direction::Right, Direction::Down, Direction::Left };
                auto dbslash = Array { Direction::Down, Direction::Left, Direction::Up, Direction::Right };

                auto nx = x + dx[d];
                auto ny = y + dy[d];
                if (nx >= lines[0].size() || ny >= lines.size()) {
                    continue;
                }

                auto ch = lines[ny][nx];
                switch (ch) {
                    case '.':
                        add({ nx, ny }, d);
                        break;
                    case '/':
                        add({ nx, ny }, dslash[d]);
                        break;
                    case '\\':
                        add({ nx, ny }, dbslash[d]);
                        break;
                    case '|':
                        if (d % 2 == 0) {
                            add({ nx, ny }, Direction::Up);
                            add({ nx, ny }, Direction::Down);
                        } else {
                            add({ nx, ny }, d);
                        }
                        break;
                    case '-':
                        if (d % 2 == 0) {
                            add({ nx, ny }, d);
                        } else {
                            add({ nx, ny }, Direction::Left);
                            add({ nx, ny }, Direction::Right);
                        }
                        break;
                }
            }
        }

        return m.size() - 1;
    };

    auto m = 0_usize;
    for (auto x : range(lines[0].size())) {
        m = max(m, compute({ x, -1 }, Direction::Down));
        m = max(m, compute({ x, lines.size() }, Direction::Up));
    }
    for (auto y : range(lines.size())) {
        m = max(m, compute({ -1, y }, Direction::Right));
        m = max(m, compute({ lines[0].size(), y }, Direction::Left));
    }
    return m;
}
