#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 9, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto s = 0_i64;
    auto v = Vec<isize> {};
    auto id = 0_isize;
    for (auto [row, line] : enumerate(lines)) {
        if (line.size() % 2 == 1) {
            line.push_back('0');
        }
        for (auto [file, free] : line | pairwise | stride(2)) {
            auto nid = id++;
            for (auto _ : range(file - '0')) {
                v.push_back(nid);
            }
            for (auto _ : range(free - '0')) {
                v.push_back(-1);
            }
        }
    }

    auto lo = 0_usize;
    auto hi = v.size() - 1;
    while (lo < hi) {
        if (v[lo] != -1) {
            lo++;
            continue;
        }
        if (v[hi] == -1) {
            hi--;
            continue;
        }

        v[lo++] = exchange(v[hi--], -1);
    }

    if (verbose) {
        println("{}"_sv, v | transform([](isize id) {
                             if (id == -1) {
                                 return U'.';
                             } else {
                                 return *front(to_string(id));
                             }
                         }) | to<String>());
    }
    for (auto [i, c] : enumerate(v)) {
        if (c != -1) {
            s += i * c;
        }
    }
    return s;
}

AOC_SOLUTION(2024, 9, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    struct Interval {
        i64 id = 0;
        i64 pos = 0;
        i64 len = 0;
    };

    auto s = 0_i64;
    auto v = Vec<Interval> {};
    auto id = 0_i64;
    auto pos = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        if (line.size() % 2 == 1) {
            line.push_back('0');
        }
        for (auto [file, free] : line | pairwise | stride(2)) {
            v.push_back({ id++, pos, file - '0' });
            pos += file - '0';
            pos += free - '0';
        }
    }

    for (auto fi = v.size() - 1; fi > 0; fi--) {
        for (auto i : range(1lu, fi + 1)) {
            auto left = v[i - 1].pos + v[i - 1].len;
            auto right = v[i].pos;

            if (right - left >= v[fi].len) {
                v.insert(v.begin() + i, { v[fi].id, left, v[fi].len });
                v.erase(v.begin() + fi + 1);
                fi++;
                break;
            }
        }
    }

    for (auto [id, pos, l] : v) {
        s += id * (pos * l + ((l - 1) * l / 2));
    }

    return s;
}
