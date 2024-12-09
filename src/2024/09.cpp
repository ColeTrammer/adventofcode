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

    auto s = 0_i64;
    auto v = Vec<Tuple<i64, i64, i64>> {};
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
            auto left = tget<1>(v[i - 1]) + tget<2>(v[i - 1]);
            auto right = tget<1>(v[i]);

            if (right - left >= tget<2>(v[fi])) {
                v.insert(v.begin() + i, { tget<0>(v[fi]), left, tget<2>(v[fi]) });
                v.erase(v.begin() + fi + 1);
                fi++;
                break;
            }
        }
    }

    for (auto [id, pos, l] : v) {
        while (l > 0) {
            s += id * pos;
            pos++;
            l--;
        }
    }

    return s;
}
