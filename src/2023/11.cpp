#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/cartesian_product.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/not_fn.h>
#include <di/math/abs_diff.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 11, a, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    for (usize row = lines.size() - 1; row < lines.size(); row--) {
        if (all_of(lines[row], equal('.'))) {
            auto string = repeat('.', lines[0].size()) | to<Ts>();
            lines.insert(lines.begin() + row, di::move(string));
        }
    }
    for (usize col = lines[0].size() - 1; col < lines[0].size(); col--) {
        if (all_of(lines, [&](auto const& line) {
                return line.data()[col] == '.';
            })) {
            for (auto& line : lines) {
                line.insert(col, '.');
            }
        }
    }

    auto const rows = lines.size();
    auto const cols = lines[0].size();
    auto galaxies = cartesian_product(range(rows), range(cols)) | filter([&](auto point) {
                        auto [row, col] = point;
                        return lines[row][col] == '#';
                    }) |
                    to<Vector>();

    for (usize i = 0; i < galaxies.size() - 1; i++) {
        for (usize j = i + 1; j < galaxies.size(); j++) {
            auto [r1, c1] = galaxies[i];
            auto [r2, c2] = galaxies[j];
            s += abs_diff(r1, r2) + abs_diff(c1, c2);
        }
    }

    return s;
}

AOC_SOLUTION(2023, 11, b, i64) {
    auto lines = input | splitv('\n');

    auto s = 0_i64;
    auto expanded_rows = TreeSet<usize> {};
    auto expanded_cols = TreeSet<usize> {};
    for (usize row = lines.size() - 1; row < lines.size(); row--) {
        if (all_of(lines[row], equal('.'))) {
            expanded_rows.insert(row);
        }
    }
    for (usize col = lines[0].size() - 1; col < lines[0].size(); col--) {
        if (all_of(lines, [&](auto const& line) {
                return line.data()[col] == '.';
            })) {
            expanded_cols.insert(col);
        }
    }

    auto const rows = lines.size();
    auto const cols = lines[0].size();
    auto galaxies = cartesian_product(range(rows), range(cols)) | filter([&](auto point) {
                        auto [row, col] = point;
                        return lines[row][col] == '#';
                    }) |
                    to<Vector>();

    for (usize i = 0; i < galaxies.size() - 1; i++) {
        for (usize j = i + 1; j < galaxies.size(); j++) {
            auto [r1, c1] = galaxies[i];
            auto [r2, c2] = galaxies[j];

            if (r2 < r1) {
                swap(r1, r2);
            }
            if (c2 < c1) {
                swap(c1, c2);
            }

            for (auto r : range(r1, r2)) {
                if (expanded_rows.contains(r)) {
                    s += 1000000;
                } else {
                    s++;
                }
            }
            for (auto c : range(c1, c2)) {
                if (expanded_cols.contains(c)) {
                    s += 1000000;
                } else {
                    s++;
                }
            }
        }
    }

    return s;
}
