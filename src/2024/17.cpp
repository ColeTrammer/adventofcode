#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 17, a, Ts) {
    auto lines = input | splitv("\n"_tsv);

    auto a = all_nums(lines[0])[0];
    auto b = all_nums(lines[1])[0];
    auto c = all_nums(lines[2])[0];

    auto program = all_nums(lines[4].substr(9), ',');

    auto ip = 0_usize;
    auto output = Vec<char> {};
    while (ip < program.size()) {
        auto op = program[ip++];

        auto get_literal = [&] -> u64 {
            return program[ip++];
        };

        auto get_combo = [&] -> u64 {
            auto operand = program[ip++];
            switch (operand) {
                case 0:
                case 1:
                case 2:
                case 3:
                    return operand;
                case 4:
                    return a;
                case 5:
                    return b;
                case 6:
                    return c;
                default:
                    ASSERT(false);
                    return 0;
            }
        };

        if (op != 3 && ip + 1 >= program.size()) {
            break;
        }

        switch (op) {
            case 0: {
                auto num = a;
                auto denom = 1 << get_combo();
                a = num / denom;
                break;
            }
            case 1: {
                b ^= get_literal();
                break;
            }
            case 2: {
                b = get_combo() % 8;
                break;
            }
            case 3: {
                if (a != 0) {
                    ip = get_literal();
                }
                break;
            }
            case 4: {
                b ^= c;
                ip++;
                break;
            }
            case 5: {
                auto out = get_combo() % 8;
                auto ch = out + '0';
                output.push_back(ch);
                break;
            }
            case 6: {
                auto num = a;
                auto denom = 1 << get_combo();
                b = num / denom;
                break;
            }
            case 7: {
                auto num = a;
                auto denom = 1 << get_combo();
                c = num / denom;
                break;
            }
        }
    }

    auto result = Ts {};
    auto first = true;
    for (auto ch : output) {
        if (!first) {
            result.push_back(',');
        }
        first = false;
        result.push_back(ch);
    }
    return result;
}

AOC_SOLUTION(2024, 17, b, u64) {
    auto lines = input | splitv("\n"_tsv);

    // Program:
    // 2,4, B = A%8
    // REDACTED C = ....
    // REDACTED
    // REDACTED
    // REDACTED B = .... (but is a function of *all* of A, not just A%8)
    // 5,5, out B
    // 0,3, A = A / 8
    // 3,0 <--- loop while a != 0, through whole program.
    //
    // However, the program is nicely structured to loop until A reaches 0. Additionally,
    // B and C are both initialized at the start of the code block. This means that left over
    // values for B and C don't matter.
    //
    // If B only depended on A%8, then the program's output for a single iteration would be fully
    // determined by the lowest 3 bits. However, becaise the output actually depends on the full
    // value of A, the solution isn't so trivial.
    //
    // However, it is true that on the final iteration, the output is fully determined by A%8.
    // So there are only 8 possible values of A we brute force over, and probably only 1 will work.
    //
    // The key insight is that the last value of A locks in the upper bits of A for the previous
    // iteration. So if A must be 1 for the last iteration, A must be [8, 15] on the second to last
    // iteration, and so on. Since there are multiple possible values of A which work, a full
    // DFS is needed.

    auto program = all_nums_i<u64>(lines[4].substr(9), ',');

    auto compute_candidates = [&](u64 a_high, u64 target, u64 max = 8) -> Vec<Tuple<u64, u64, u64>> {
        auto result = Vec<Tuple<u64, u64, u64>> {};

        for (auto candidate : range(max)) {
            auto ip = 0_usize;
            auto a = a_high << 3 | candidate;
            auto b = 0;
            auto c = 0;

            auto output = Vec<u64> {};
            while (ip < program.size()) {
                auto op = program[ip++];

                auto get_literal = [&] -> u64 {
                    return program[ip++];
                };

                auto get_combo = [&] -> u64 {
                    auto operand = program[ip++];
                    switch (operand) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                            return operand;
                        case 4:
                            return a;
                        case 5:
                            return b;
                        case 6:
                            return c;
                        default:
                            ASSERT(false);
                            return 0;
                    }
                };

                if (op != 3 && ip + 1 >= program.size()) {
                    break;
                }

                switch (op) {
                    case 0: {
                        auto num = a;
                        auto denom = 1_u64 << get_combo();
                        a = num / denom;
                        break;
                    }
                    case 1: {
                        b ^= get_literal();
                        break;
                    }
                    case 2: {
                        b = get_combo() % 8;
                        break;
                    }
                    case 3: {
                        if (a != 0) {
                            ip = get_literal();
                        }
                        break;
                    }
                    case 4: {
                        b ^= c;
                        ip++;
                        break;
                    }
                    case 5: {
                        auto out = get_combo() % 8;
                        if (out == target) {
                            result.push_back({ a, b, c });
                        }
                        ip = program.size();
                        break;
                    }
                    case 6: {
                        auto num = a;
                        auto denom = 1_u64 << get_combo();
                        b = num / denom;
                        break;
                    }
                    case 7: {
                        auto num = a;
                        auto denom = 1_u64 << get_combo();
                        c = num / denom;
                        break;
                    }
                }
            }
        }
        return result;
    };

    auto dfs = [&](this auto&& dfs, Span<u64> program, u64 a) -> Optional<u64> {
        if (program.empty()) {
            return a;
        }

        auto candidates = compute_candidates(a, *program.back());
        program = *program.subspan(0, program.size() - 1);

        auto res = NumericLimits<u64>::max;
        for (auto [a, b, c] : candidates) {
            if (auto r = dfs(program, a)) {
                res = min(res, *r);
            }
        }
        if (res == NumericLimits<u64>::max) {
            return nullopt;
        }
        return res;
    };

    auto res = dfs(program.span(), 0_u64);

    ASSERT(res.has_value());
    return res.value();
}
