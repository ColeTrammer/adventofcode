#include <di/container/algorithm/count_if.h>
#include <di/container/algorithm/reverse.h>
#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/not_fn.h>
#include <di/vocab/tuple/make_tuple.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 12, a, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    for (auto [row, line] : enumerate(lines)) {
        auto [map_s, extents_s] = split_two(line.view(), ' ');
        auto extents = all_nums_i<usize>(extents_s, ',');
        auto map = map_s | to<Ts>();

        auto valid = [&] {
            // println("V: {}, E: {}"_sv, map, extents);
            // println("S: {}"_sv, map | split('.') | filter(not_fn(empty)) | transform(size));
            return di::container::equal(map | split('.') | filter(not_fn(empty)) | transform(size), extents);
        };

        auto bt = ycombinator([&](auto&& bt, usize index) -> i32 {
            if (index == map.size()) {
                auto v = valid();
                // println("v={}"_sv, v);
                return v ? 1 : 0;
            }

            if (map[index] != '?') {
                return bt(index + 1);
            }

            map[index] = '#';
            auto r1 = bt(index + 1);
            map[index] = '.';
            auto r2 = bt(index + 1);
            map[index] = '?';
            return r1 + r2;
        });

        auto r = bt(0);
        // println("{}"_sv, r);
        s += r;
    }
    return s;
}

AOC_SOLUTION(2023, 12, b, i64) {
    auto lines = input | splitv('\n');

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto [map_s, extents_s] = split_two(line.view(), ' ');
        auto og_extents = all_nums_i<usize>(extents_s, ',');
        auto og_map = map_s | to<Ts>();

        auto extents = Vector<usize> {};
        auto map = Ts {};

        auto unfold = [&] {
            auto first = true;
            for (auto _ : range(5)) {
                (void) _;
                extents.append_container(og_extents);
                if (first) {
                    first = false;
                } else {
                    map.push_back('?');
                }
                map.append(og_map);
            }
        };

        unfold();

        // dp[E][I] = # of arrangements starting from position I which satify the last E constaints.
        // It is assumed the character at I-1 is a '.'.
        auto dp = Vector<Vector<i64>> {};
        dp.resize(extents.size() + 1);
        for (auto& row : dp) {
            row.resize(map.size() + 1);
        }

        // Base case: 0 arrangements:
        dp[0][map.size()] = 1;
        for (auto i : reverse(range(map.size()))) {
            if (map[i] == '#') {
                dp[0][i] = 0;
            } else {
                dp[0][i] = dp[0][i + 1];
            }
        }

        for (auto e : range(1_usize, extents.size() + 1)) {
            for (auto i : reverse(range(map.size()))) {
                auto if_dot = dp[e][i + 1];
                if (map[i] == '.' || map[i] == '?') {
                    dp[e][i] += if_dot;
                }

                if (map[i] == '.') {
                    continue;
                }

                auto length = extents[extents.size() - e];
                if (i + length > map.size()) {
                    continue;
                }

                auto impossible = false;
                for (auto u : range(length)) {
                    if (map[i + u] == '.') {
                        impossible = true;
                        break;
                    }
                }

                if (impossible) {
                    continue;
                }

                auto if_hash = 0_i64;
                if (e == 1) {
                    if (i + length == map.size()) {
                        if_hash++;
                    } else if (map[i + length] != '#') {
                        if_hash = dp[e - 1][i + length + 1];
                    }
                } else {
                    if (i + length < map.size() && map[i + length] != '#') {
                        if_hash = dp[e - 1][i + length + 1];
                    }
                }
                dp[e][i] += if_hash;
            }
        }

        auto r = dp[extents.size()][0];
        s += r;
    }
    return s;
}
