#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 25, a, i64) {
    auto groups = input | splitv("\n\n"_tsv);

    auto s = 0_i64;
    auto keys = Vec<Vec<i64>> {};
    auto locks = Vec<Vec<i64>> {};
    for (auto [row, liness] : enumerate(groups)) {
        auto lines = splitv(liness, '\n');
        if (lines[0] == "#####"_tsv) {
            // lock
            auto& lock = locks.emplace_back();
            for (auto c : range(5)) {
                for (auto r : range(8)) {
                    if (r == 7 || lines[r][c] == '.') {
                        lock.push_back(r - 1);
                        break;
                    }
                }
            }
        } else {
            // key
            auto& key = keys.emplace_back();
            for (auto c : range(5)) {
                for (auto r : range(8)) {
                    if (r == 7 || lines[6 - r][c] == '.') {
                        key.push_back(r - 1);
                        break;
                    }
                }
            }
        }
    }

    for (auto const& [key, lock] : cartesian_product(keys, locks)) {
        auto good = true;
        for (auto [l, k] : zip(lock, key)) {
            if (l + k >= 6) {
                good = false;
            }
        }
        if (good) {
            s++;
        }
    }
    return s;
}
