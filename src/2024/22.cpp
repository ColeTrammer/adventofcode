#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 22, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto mix = [](u64 a, u64 b) -> u64 {
        return a ^ b;
    };

    auto prune = [](u64 a) -> u64 {
        return a % 16777216;
    };

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto n = uparse_i<u64>(line);
        for (auto _ : range(2000)) {
            auto z = n * 64;
            n = prune(mix(n, z));
            auto y = n / 32;
            n = prune(mix(n, y));
            auto x = n * 2048;
            n = prune(mix(n, x));
        }

        s += n;
    }
    return s;
}

AOC_SOLUTION(2024, 22, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto mix = [](u64 a, u64 b) -> u64 {
        return a ^ b;
    };

    auto prune = [](u64 a) -> u64 {
        return a % 16777216;
    };

    auto s = 0_i64;
    auto inits = Vec<i8> {};
    auto seqs = Vec<Vec<i8>> {};
    constexpr auto N = 2000;
    for (auto [row, line] : enumerate(lines)) {
        auto& seq = seqs.emplace_back();
        auto n = uparse_i<u64>(line);
        inits.push_back(i8(n % 10));
        for (auto _ : range(N)) {
            auto prev = n;
            auto z = n * 64;
            n = prune(mix(n, z));
            auto y = n / 32;
            n = prune(mix(n, y));
            auto x = n * 2048;
            n = prune(mix(n, x));
            seq.push_back(i8(n % 10) - i8(prev % 10));
        }

        ASSERT(seq.size() == 2000);
    }

    for (auto a : range(i8(-9), i8(9))) {
        for (auto b : range(i8(-9), i8(9))) {
            for (auto c : range(i8(-9), i8(9))) {
                for (auto d : range(i8(-9), i8(9))) {
                    if (a + b + c + d <= -10 || a + b + c + d >= 10) {
                        continue;
                    }
                    auto r = 0_i64;

                    for (auto const& [n, seq] : enumerate(seqs)) {
                        auto price = inits[n] + seq[0] + seq[1] + seq[2];
                        for (auto i : range(3zu, 2000zu)) {
                            price += seq[i];
                            if (a == seq[i - 3] && b == seq[i - 2] && c == seq[i - 1] && d == seq[i]) {
                                r += price;
                                break;
                            }
                        }
                    }

                    s = max(s, r);
                }
            }
        }
    }
    return s;
}
