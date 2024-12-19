#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 19, a, i64) {
    auto groups = input | splitv("\n\n"_tsv);

    auto avail = groups[0] | split(", "_tsv) | to<Vec<Tsv>>();
    auto patterns = groups[1] | split("\n"_tsv);

    auto ll = Map<char, Vec<Tsv>> {};
    for (auto const& s : avail) {
        ll[s[0]].push_back(s);
    }

    auto s = 0_i64;
    for (auto [row, line] : enumerate(patterns)) {
        auto possible = Vec<bool> {};
        possible.resize(line.size() + 1);
        *possible.back() = true;

        for (auto i : range(line.size()) | reverse) {
            for (auto s : ll[line[i]]) {
                if (line.substr(i).starts_with(s) && possible[i + s.size()]) {
                    possible[i] = true;
                    break;
                }
            }
        }
        if (possible[0]) {
            s++;
        }
    }
    return s;
}

AOC_SOLUTION(2024, 19, b, i64) {
    auto groups = input | splitv("\n\n"_tsv);

    auto avail = groups[0] | split(", "_tsv) | to<Vec<Tsv>>();
    auto patterns = groups[1] | split("\n"_tsv);

    auto s = 0_i64;
    for (auto [row, line] : enumerate(patterns)) {
        auto possible = Vec<i64> {};
        possible.resize(line.size() + 1);
        *possible.back() = 1;

        for (auto i : range(line.size()) | reverse) {
            for (auto s : avail) {
                if (line.substr(i).starts_with(s)) {
                    possible[i] += possible[i + s.size()];
                }
            }
        }
        s += possible[0];
    }
    return s;
}
