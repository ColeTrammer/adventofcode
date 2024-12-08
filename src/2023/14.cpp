#include "di/container/algorithm/count.h"
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

AOC_SOLUTION(2023, 14, a, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != 'O') {
                continue;
            }

            auto r = row - 1;
            while (r < row && lines[r][col] == '.') {
                swap(lines[r][col], lines[r + 1][col]);
                r--;
            }
        }
    }

    for (auto [row, line] : enumerate(lines)) {
        s += (lines.size() - row) * count(line, 'O');
    }
    return s;
}

AOC_SOLUTION(2023, 14, b, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;

    auto spin = [&]<usize dr, usize dc>(Constexpr<dr>, Constexpr<dc>) {
        auto l = [&] {
            if constexpr (dr == 1) {
                return reverse(enumerate(lines));
            } else {
                return enumerate(lines);
            }
        }();
        for (auto [row, line] : l) {
            auto r = [&] {
                if constexpr (dc == 1) {
                    return reverse(enumerate(line));
                } else {
                    return enumerate(line);
                }
            }();

            for (auto [col, ch] : r) {
                if (ch != 'O') {
                    continue;
                }

                auto r = row + dr;
                auto c = col + dc;
                while (r < lines.size() && c < line.size() && lines[r][c] == '.') {
                    swap(lines[r][c], lines[r - dr][c - dc]);
                    r += dr;
                    c += dc;
                }
            }
        }
    };

    auto map = TreeMap<Ts, usize> {};
    auto rmap = TreeMap<usize, Vector<Ts>> {};
    auto add_to_map = [&](usize i) -> Optional<usize> {
        auto key = ""_ts;
        for (auto& line : lines) {
            key += line;
        }
        auto r = map.try_emplace(di::clone(key), i);
        if (!tget<1>(r)) {
            return tget<1>(*tget<0>(r));
        }
        rmap[i] = di::clone(lines);
        return nullopt;
    };

    for (usize i = 0;; i++) {
        if (auto r = add_to_map(i)) {
            auto l = i - *r;
            auto d = 1000000000zu - *r;
            d %= l;

            lines = di::clone(rmap[*r + d]);

            break;
        }
        spin(c_<-1zu>, c_<0zu>);
        spin(c_<0zu>, c_<-1zu>);
        spin(c_<1zu>, c_<0zu>);
        spin(c_<0zu>, c_<1zu>);
    }

    for (auto [row, line] : enumerate(lines)) {
        s += (lines.size() - row) * count(line, 'O');
    }
    return s;
}
