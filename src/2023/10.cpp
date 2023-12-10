#include <di/container/algorithm/any_of.h>
#include <di/container/view/cartesian_product.h>
#include <runner/aoc_problem_registry.h>

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

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 10, a, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<usize, usize>;

    auto nodes = Vector<Point> {};
    auto visited = TreeSet<Point> {};

    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                nodes.push_back({ row, col });
                visited.insert({ row, col });
            }
        }
    }

    auto s = 0;
    while (!nodes.empty()) {
        auto curr = di::move(nodes);

        for (auto [row, col] : curr) {
            for (auto [r, c, ch] : neighbors(lines, row, col, false)) {
                if (visited.contains({ r, c })) {
                    continue;
                }

                switch (lines[row][col]) {
                    case '|':
                        if (c != col) {
                            continue;
                        }
                        break;
                    case '-':
                        if (r != row) {
                            continue;
                        }
                        break;
                    case 'L':
                        if (r < row || c > col) {
                            break;
                        }
                        continue;
                    case 'J':
                        if (r < row || c < col) {
                            break;
                        }
                        continue;
                    case '7':
                        if (r > row || c < col) {
                            break;
                        }
                        continue;
                    case 'F':
                        if (r > row || c > col) {
                            break;
                        }
                        continue;
                    case '.':
                        continue;
                }

                auto good = false;
                switch (ch) {
                    case '|':
                        good = c == col;
                        break;
                    case '-':
                        good = r == row;
                        break;
                    case 'L':
                        good = r > row || c < col;
                        break;
                    case 'J':
                        good = r > row || c > col;
                        break;
                    case '7':
                        good = r < row || c > col;
                        break;
                    case 'F':
                        good = r < row || c < col;
                        break;
                }
                if (good) {
                    nodes.push_back({ r, c });
                    visited.insert({ r, c });
                }
            }
        }

        s++;
    }

    return s - 1;
}

AOC_SOLUTION(2023, 10, b, i64) {
    auto lines = input | splitv('\n');

    using Point = Tuple<usize, usize>;

    auto nodes = Vector<Point> {};
    auto visited = TreeSet<Point> {};

    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                nodes.push_back({ row, col });
                visited.insert({ row, col });
            }
        }
    }

    auto [sr, sc] = nodes[0];

    auto s = 0;
    while (!nodes.empty()) {
        auto curr = di::move(nodes);

        for (auto [row, col] : curr) {
            for (auto [r, c, ch] : neighbors(lines, row, col, false)) {
                if (visited.contains({ r, c })) {
                    continue;
                }

                switch (lines[row][col]) {
                    case '|':
                        if (c != col) {
                            continue;
                        }
                        break;
                    case '-':
                        if (r != row) {
                            continue;
                        }
                        break;
                    case 'L':
                        if (r < row || c > col) {
                            break;
                        }
                        continue;
                    case 'J':
                        if (r < row || c < col) {
                            break;
                        }
                        continue;
                    case '7':
                        if (r > row || c < col) {
                            break;
                        }
                        continue;
                    case 'F':
                        if (r > row || c > col) {
                            break;
                        }
                        continue;
                    case '.':
                        continue;
                }

                auto good = false;
                switch (ch) {
                    case '|':
                        good = c == col;
                        break;
                    case '-':
                        good = r == row;
                        break;
                    case 'L':
                        good = r > row || c < col;
                        break;
                    case 'J':
                        good = r > row || c > col;
                        break;
                    case '7':
                        good = r < row || c > col;
                        break;
                    case 'F':
                        good = r < row || c < col;
                        break;
                }
                if (good) {
                    nodes.push_back({ r, c });
                    visited.insert({ r, c });
                }
            }
        }
    }

    for (auto [row, col] : cartesian_product(range(lines.size()), range(lines[0].size()))) {
        if (!visited.contains({ row, col })) {
            const_cast<char*>(lines[row].data())[col] = '.';
        }
    }

    if (visited.contains({ sr + 1, sc }) && any_of(Array { '|', 'J', 'L' }, equal(lines[sr + 1][sc])) &&
        visited.contains({ sr - 1, sc }) && any_of(Array { '|', 'F', '7' }, equal(lines[sr - 1][sc]))) {
        const_cast<char*>(lines[sr].data())[sc] = '|';
    } else if (visited.contains({ sr, sc - 1 }) && any_of(Array { '-', 'F', 'L' }, equal(lines[sr][sc - 1])) &&
               visited.contains({ sr, sc + 1 }) && any_of(Array { '-', 'J', '7' }, equal(lines[sr][sc + 1]))) {
        const_cast<char*>(lines[sr].data())[sc] = '-';
    } else if (visited.contains({ sr + 1, sc }) && any_of(Array { '|', 'J', 'L' }, equal(lines[sr + 1][sc])) &&
               visited.contains({ sr, sc + 1 }) && any_of(Array { '-', 'J', '7' }, equal(lines[sr][sc + 1]))) {
        const_cast<char*>(lines[sr].data())[sc] = 'F';
    } else if (visited.contains({ sr + 1, sc }) && any_of(Array { '|', 'J', 'L' }, equal(lines[sr + 1][sc])) &&
               visited.contains({ sr, sc - 1 }) && any_of(Array { '-', 'F', 'L' }, equal(lines[sr][sc - 1]))) {
        const_cast<char*>(lines[sr].data())[sc] = '7';
    } else if (visited.contains({ sr - 1, sc }) && any_of(Array { '|', 'F', '7' }, equal(lines[sr - 1][sc])) &&
               visited.contains({ sr, sc + 1 }) && any_of(Array { '-', 'J', '7' }, equal(lines[sr][sc + 1]))) {
        const_cast<char*>(lines[sr].data())[sc] = 'L';
    } else if (visited.contains({ sr - 1, sc }) && any_of(Array { '|', 'F', '7' }, equal(lines[sr - 1][sc])) &&
               visited.contains({ sr, sc - 1 }) && any_of(Array { '-', 'F', 'L' }, equal(lines[sr][sc - 1]))) {
        const_cast<char*>(lines[sr].data())[sc] = 'J';
    }

    for (auto [row, col] : cartesian_product(range(lines.size()), range(lines[0].size()))) {
        if (visited.contains({ row, col })) {
            continue;
        }

        auto nodes = Vector<Point> {};
        auto vs = TreeSet<Point> {};
        nodes.push_back({ row, col });
        vs.insert({ row, col });

        auto contained = true;
        while (!nodes.empty() && contained) {
            auto curr = di::move(nodes);
            for (auto [row, col] : curr) {
                if (row == 0 || col == 0 || row == lines.size() || col == lines[0].size()) {
                    contained = false;
                    break;
                }

                for (auto [dr, dc] : Array { Point { -1, 0 }, Point { 1, 0 }, Point { 0, 1 }, Point { 0, -1 } }) {
                    auto r = row + dr;
                    auto c = col + dc;
                    if (r > lines.size() || c > lines[0].size()) {
                        continue;
                    }
                    if (vs.contains({ r, c })) {
                        continue;
                    }

                    // Down
                    if (dr == 1) {
                        auto c1 = lines[row][col - 1];
                        auto c2 = lines[row][col];

                        if ((c1 == 'F' || c1 == '-' || c1 == 'L') && (c2 == 'J' || c2 == '-' || c2 == '7')) {
                            continue;
                        }
                    }
                    // Up
                    if (dr == usize(-1)) {
                        auto c1 = lines[row - 1][col - 1];
                        auto c2 = lines[row - 1][col];
                        if ((c1 == 'F' || c1 == '-' || c1 == 'L') && (c2 == 'J' || c2 == '-' || c2 == '7')) {
                            continue;
                        }
                    }

                    // Left
                    if (dc == usize(-1)) {
                        auto c1 = lines[row - 1][col - 1];
                        auto c2 = lines[row][col - 1];
                        if ((c1 == 'F' || c1 == '|' || c1 == '7') && (c2 == 'J' || c2 == '|' || c2 == 'L')) {
                            continue;
                        }
                    }

                    // Right
                    if (dc == usize(1)) {
                        auto c1 = lines[row - 1][col];
                        auto c2 = lines[row][col];
                        if ((c1 == 'F' || c1 == '|' || c1 == '7') && (c2 == 'J' || c2 == '|' || c2 == 'L')) {
                            continue;
                        }
                    }

                    // println("A: {} {}"_sv, r, c);
                    vs.insert({ r, c });
                    nodes.push_back({ r, c });
                }
            }
        }

        if (!contained) {
            continue;
        }

        s++;
    }

    return s;
}
