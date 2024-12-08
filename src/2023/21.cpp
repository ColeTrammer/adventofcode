#include "di/assert/prelude.h"
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

AOC_SOLUTION(2023, 21, a, i64) {
    auto lines = input | splitv('\n');

    auto srow = 0zu;
    auto scol = 0zu;
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch == 'S') {
                srow = row;
                scol = col;
            }
        }
    }

    auto spots = TreeSet<Tuple<usize, usize>> {};
    spots.insert({ srow, scol });

    for (auto i : range(64)) {
        (void) i;

        auto curr = di::move(spots);

        for (auto [row, col] : curr) {
            for (auto [r, c, v] : neighbors(lines, row, col, false)) {
                if (v != '#') {
                    spots.insert({ r, c });
                }
            }
        }
    }

    return spots.size();
}

AOC_SOLUTION(2023, 21, b, i64) {
    auto lines = input | splitv('\n');

    auto const width = ssize(lines[0]);
    auto const height = ssize(lines);

    auto dfs = [&](usize srow, usize scol, usize iterations) -> usize {
        auto spots = TreeSet<Tuple<usize, usize>> {};
        spots.insert({ srow, scol });

        for (auto i : range(iterations)) {
            (void) i;

            auto curr = di::move(spots);

            for (auto [row, col] : curr) {
                for (auto [r, c, v] : neighbors(lines, row, col, false)) {
                    if (v != '#') {
                        spots.insert({ r, c });
                    }
                }
            }
        }

        return spots.size();
    };

    DI_ASSERT_EQ(width, height);

    auto const n = 26501365;
    auto const srow = height / 2;
    auto const scol = width / 2;

    auto const steps_to_edge_from_center = scol + 1;
    auto const steps_to_edge_from_edge = width;

    auto const full_grids_directionally = (n - steps_to_edge_from_center) / steps_to_edge_from_edge;
    auto const partial_directional_grids_iterations =
        n - (steps_to_edge_from_center + full_grids_directionally * steps_to_edge_from_edge);
    auto const partial_diagonal_grids_iterations = partial_directional_grids_iterations - steps_to_edge_from_center;
    auto const partial_diagonal_large_iterations = partial_diagonal_grids_iterations + steps_to_edge_from_edge;

    auto const diagonal_grids_small = full_grids_directionally + 1;
    auto const diagonal_grids_large = full_grids_directionally;

    auto odd_full_grids = 1z;
    auto even_full_grids = 0z;
    {
        auto b = true;
        auto c = 0z;
        for (auto i : range(full_grids_directionally)) {
            (void) i;

            c += 4;

            if (b) {
                even_full_grids += c;
            } else {
                odd_full_grids += c;
            }

            b = !b;
        }
    }

    auto const reachable_from_even_full_grid = dfs(srow, scol, 2 * width);
    auto const reachable_from_odd_full_grid = dfs(srow, scol, 2 * width + 1);

    auto const reachable_from_pure_right = dfs(srow, 0, partial_directional_grids_iterations);
    auto const reachable_from_pure_left = dfs(srow, width - 1, partial_directional_grids_iterations);
    auto const reachable_from_pure_up = dfs(height - 1, scol, partial_directional_grids_iterations);
    auto const reachable_from_pure_down = dfs(0, scol, partial_directional_grids_iterations);

    auto const reachable_from_partial_bottom_right = dfs(height - 1, 0, partial_diagonal_grids_iterations);
    auto const reachable_from_partial_bottom_left = dfs(height - 1, width - 1, partial_diagonal_grids_iterations);
    auto const reachable_from_partial_top_right = dfs(0, 0, partial_diagonal_grids_iterations);
    auto const reachable_from_partial_top_left = dfs(0, width - 1, partial_diagonal_grids_iterations);

    auto const reachable_from_partial_bottom_right_large = dfs(height - 1, 0, partial_diagonal_large_iterations);
    auto const reachable_from_partial_bottom_left_large = dfs(height - 1, width - 1, partial_diagonal_large_iterations);
    auto const reachable_from_partial_top_right_large = dfs(0, 0, partial_diagonal_large_iterations);
    auto const reachable_from_partial_top_left_large = dfs(0, width - 1, partial_diagonal_large_iterations);

    auto s = 0z;

    s += reachable_from_even_full_grid * even_full_grids;
    s += reachable_from_odd_full_grid * odd_full_grids;

    s += reachable_from_pure_right;
    s += reachable_from_pure_left;
    s += reachable_from_pure_up;
    s += reachable_from_pure_down;

    s += diagonal_grids_small * reachable_from_partial_bottom_right;
    s += diagonal_grids_small * reachable_from_partial_bottom_left;
    s += diagonal_grids_small * reachable_from_partial_top_right;
    s += diagonal_grids_small * reachable_from_partial_top_left;

    s += diagonal_grids_large * reachable_from_partial_bottom_right_large;
    s += diagonal_grids_large * reachable_from_partial_bottom_left_large;
    s += diagonal_grids_large * reachable_from_partial_top_right_large;
    s += diagonal_grids_large * reachable_from_partial_top_left_large;

    return s;
}
