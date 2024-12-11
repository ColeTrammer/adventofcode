#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 8, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto m = Map<char, Vector<Coord>> {};
    auto set = Set<Coord> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '.') {
                m[ch].push_back({ row, col });
            }
        }
    }

    for (auto const& [_, nodes] : m) {
        for (auto i : range(nodes.size())) {
            for (auto j : range(i + 1, nodes.size())) {
                auto [r1, c1] = nodes[i];
                auto [r2, c2] = nodes[j];

                auto dr = r1 - r2;
                auto dc = c1 - c2;

                auto r3 = r1 + dr;
                auto c3 = c1 + dc;
                auto r4 = r2 - dr;
                auto c4 = c2 - dc;

                set.insert({ r3, c3 });
                set.insert({ r4, c4 });
            }
        }
    }

    return count_if(set, [&](auto p) {
        auto [r, c] = p;
        return r < lines.size() && c < lines[0].size();
    });
}

AOC_SOLUTION(2024, 8, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto m = Map<char, Vector<Coord>> {};
    auto set = Set<Coord> {};
    for (auto [row, line] : enumerate(lines)) {
        for (auto [col, ch] : enumerate(line)) {
            if (ch != '.') {
                m[ch].push_back({ row, col });
                set.insert({ row, col });
            }
        }
    }

    for (auto const& [_, nodes] : m) {
        for (auto i : range(nodes.size())) {
            for (auto j : range(i + 1, nodes.size())) {
                auto [r1, c1] = nodes[i];
                auto [r2, c2] = nodes[j];

                auto dr = r1 - r2;
                auto dc = c1 - c2;

                auto r3 = r1 + dr;
                auto c3 = c1 + dc;
                auto r4 = r2 - dr;
                auto c4 = c2 - dc;

                while (r3 < lines.size() && c3 < lines[0].size()) {
                    set.insert({ r3, c3 });
                    r3 += dr;
                    c3 += dc;
                }
                while (r4 < lines.size() && c4 < lines[0].size()) {
                    set.insert({ r4, c4 });
                    r4 -= dr;
                    c4 -= dc;
                }
            }
        }
    }

    return set.size();
}
