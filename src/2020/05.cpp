#include "di/container/interface/access.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/not_fn.h"
#include "di/math/midpoint.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2020, 5, a, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto row_chars = line.substr(0, 7);
        auto col_chars = line.substr(7);

        int rlo = 0;
        int rhi = 128;
        for (auto c : row_chars) {
            if (c == 'F') {
                rhi = midpoint(rlo, rhi);
            } else if (c == 'B') {
                rlo = midpoint(rlo, rhi);
            }
        }

        int clo = 0;
        int chi = 8;
        for (auto c : col_chars) {
            if (c == 'L') {
                chi = midpoint(clo, chi);
            } else if (c == 'R') {
                clo = midpoint(clo, chi);
            }
        }

        s = max(s, rlo * 8 + clo);
    }
    return s;
}

AOC_SOLUTION(2020, 5, b, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    auto ids = di::TreeSet<int> {};
    for (auto [row, line] : enumerate(lines)) {
        auto row_chars = line.substr(0, 7);
        auto col_chars = line.substr(7);

        int rlo = 0;
        int rhi = 128;
        for (auto c : row_chars) {
            if (c == 'F') {
                rhi = midpoint(rlo, rhi);
            } else if (c == 'B') {
                rlo = midpoint(rlo, rhi);
            }
        }

        int clo = 0;
        int chi = 8;
        for (auto c : col_chars) {
            if (c == 'L') {
                chi = midpoint(clo, chi);
            } else if (c == 'R') {
                clo = midpoint(clo, chi);
            }
        }

        ids.insert(rlo * 8 + clo);
    }

    auto all_ids = range(*front(ids), *back(ids) + 1) | di::to<di::TreeSet>();
    all_ids.subtract(ids);

    s = *front(all_ids);
    return s;
}
