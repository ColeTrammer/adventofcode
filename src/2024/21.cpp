#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

AOC_SOLUTION(2024, 21, a, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto keypad = Vec<Vec<char>> {};
    keypad.push_back(Array { '7', '8', '9' } | to<Vector>());
    keypad.push_back(Array { '4', '5', '6' } | to<Vector>());
    keypad.push_back(Array { '1', '2', '3' } | to<Vector>());
    keypad.push_back(Array { '\0', '0', 'A' } | to<Vector>());

    auto remote = Vec<Vec<char>> {};
    remote.push_back(Array { '\0', '^', 'A' } | to<Vector>());
    remote.push_back(Array { '<', 'v', '>' } | to<Vector>());

    auto shortest_length = [&](Vec<Vec<char>> const& grid, usize rs, usize cs, Tsv target) -> i64 {
        struct State {
            usize row = 0;
            usize col = 0;
            usize chars = 0;

            bool operator==(State const& other) const {
                return Tuple { row, col, chars } == Tuple { other.row, other.col, other.chars };
            }

            auto operator<=>(State const& other) const {
                return Tuple { row, col, chars } <=> Tuple { other.row, other.col, other.chars };
            }
        };

        auto queue = Vec<State> {};
        auto vs = Set<State> {};

        auto add = [&](State&& s) {
            if (vs.contains(s)) {
                return;
            }
            vs.insert({ s.row, s.col, s.chars });
            queue.push_back(di::move(s));
        };

        auto next = [&](State const& s, char nch) {
            if (nch == 'A') {
                add({ s.row, s.col, s.chars + 1 });
                return;
            }

            int dr = 0;
            int dc = 0;
            switch (nch) {
                case 'v':
                    dr = 1;
                    break;
                case '^':
                    dr = -1;
                    break;
                case '>':
                    dc = 1;
                    break;
                case '<':
                    dc = -1;
                    break;
                default:
                    ASSERT(false);
                    break;
            }

            auto nr = s.row + dr;
            auto nc = s.col + dc;
            if (nr >= grid.size() || nc >= grid[nr].size() || grid[nr][nc] == '\0') {
                return;
            }

            add({ nr, nc, s.chars });
        };

        add({ rs, cs, 0 });

        auto res = Vec<Ts> {};
        auto length = 0_i64;
        while (!queue.empty() && res.empty()) {
            length++;
            auto tqueue = di::move(queue);

            for (auto s : tqueue | as_rvalue) {
                if (s.chars == target.size()) {
                    return length;
                }

                auto t = target[s.chars];
                if (grid[s.row][s.col] == t) {
                    next(s, 'A');
                    continue;
                }

                next(s, '<');
                next(s, '^');
                next(s, '>');
                next(s, 'v');
            }
        }
        return -1;
    };

    auto all_combos = [&](Vec<Vec<char>> const& grid, usize rs, usize cs, Tsv target) -> Vec<Ts> {
        auto length = shortest_length(grid, rs, cs, target);

        // DP to tell us if we can reach the end in the target number of moves.
        auto dpt = Vec<Vec<Vec<Vec<i64>>>> {};
        dpt.resize(grid.size());
        for (auto& x : dpt) {
            x.resize(grid[0].size());
            for (auto& y : x) {
                y.resize(target.size() + 1);
                for (auto& z : y) {
                    z.resize(length + 1);
                }
            }
        }

        // Base case: reachable cause we're at the end. Start and end at A.
        dpt[rs][cs][0][0] = true;

        for (auto len : range(1, length + 1)) {
            for (auto r : range(grid.size())) {
                for (auto c : range(grid[0].size())) {
                    if (grid[r][c] == '\0') {
                        continue;
                    }
                    if (len == length && grid[r][c] != 'A') {
                        continue;
                    }

                    for (auto t : range(1zu, target.size() + 1)) {
                        auto& ss = dpt[r][c][t][len];

                        // We're good if and only if all neighbors with 1 less step are good.

                        // We need to press A
                        if (grid[r][c] == target[target.size() - t]) {
                            ss += dpt[r][c][t - 1][len - 1];
                            continue;
                        }

                        // We need to try moving.
                        for (auto ch : Array { '<', '>', 'v', '^' }) {
                            int dr = 0;
                            int dc = 0;
                            switch (ch) {
                                case 'v':
                                    dr = 1;
                                    break;
                                case '^':
                                    dr = -1;
                                    break;
                                case '>':
                                    dc = 1;
                                    break;
                                case '<':
                                    dc = -1;
                                    break;
                                default:
                                    ASSERT(false);
                                    break;
                            }

                            auto nr = r + dr;
                            auto nc = c + dc;
                            if (nr >= grid.size() || nc >= grid[nr].size() || grid[nr][nc] == '\0') {
                                continue;
                            }

                            ss += dpt[nr][nc][t][len - 1];
                        }
                    }
                }
            }
        }

        auto memo = Map<Tuple<usize, usize, usize, i64>, Vec<Ts>> {};

        auto dfs = [&](this auto&& dfs, usize row, usize col, usize m, i64 len) -> Vec<Ts> {
            if (len < 0) {
                return {};
            }
            if (!dpt[row][col][m][len]) {
                return {};
            }
            if (memo.contains({ row, col, m, len })) {
                return clone(memo[{ row, col, m, len }]);
            }

            auto& r = memo[{ row, col, m, len }];
            if (m == 0 && len == 0) {
                r.push_back(""_ts);
                return clone(r);
            }

            if (grid[row][col] == target[target.size() - m]) {
                for (auto& s : dfs(row, col, m - 1, len - 1)) {
                    auto& ss = r.emplace_back();
                    ss.push_back('A');
                    ss.append(di::move(s));
                }
                return clone(r);
            }
            for (auto ch : Array { '<', '>', 'v', '^' }) {
                int dr = 0;
                int dc = 0;
                switch (ch) {
                    case 'v':
                        dr = 1;
                        break;
                    case '^':
                        dr = -1;
                        break;
                    case '>':
                        dc = 1;
                        break;
                    case '<':
                        dc = -1;
                        break;
                    default:
                        ASSERT(false);
                        break;
                }

                auto nr = row + dr;
                auto nc = col + dc;
                if (nr >= grid.size() || nc >= grid[nr].size() || grid[nr][nc] == '\0') {
                    continue;
                }

                for (auto& s : dfs(nr, nc, m, len - 1)) {
                    auto& ss = r.emplace_back();
                    ss.push_back(ch);
                    ss.append(di::move(s));
                }
            }
            return clone(r);
        };

        auto res = dfs(rs, cs, target.size(), length - 1);
        return res;
    };

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto numeric_part = uparse_int(line.substr(0, 3));

        auto complexity = NumericLimits<i64>::max;
        auto robot_moves = all_combos(keypad, 3, 2, line);
        for (auto const& move1 : robot_moves) {
            for (auto const& move2 : all_combos(remote, 0, 2, move1)) {
                complexity = min(complexity, shortest_length(remote, 0, 2, move2));
            }
        }

        println("{} * {}"_sv, numeric_part, complexity - 1);
        s += numeric_part * (complexity - 1);
    }
    return s;
}

AOC_SOLUTION(2024, 21, b, i64) {
    auto lines = input | splitv("\n"_tsv);

    auto keypad = Vec<Vec<char>> {};
    keypad.push_back(Array { '7', '8', '9' } | to<Vector>());
    keypad.push_back(Array { '4', '5', '6' } | to<Vector>());
    keypad.push_back(Array { '1', '2', '3' } | to<Vector>());
    keypad.push_back(Array { '\0', '0', 'A' } | to<Vector>());

    constexpr auto N = 25zu;

    // DP[S][E][N] counts the number of characters needed to encode moving from 'S' to 'E' when using 'N' remotes.
    // Since we higher level remotes always start and end on 'A', this is very useful.
    auto dp = Map<char, Map<char, Map<usize, i64>>> {};

    for (auto i : range(N + 1)) {
        for (auto curr : "A<>^v"_tsv) {
            for (auto ch : "A<>^v"_tsv) {
                if (i == 0) {
                    dp[curr][ch][i] = 1;
                    continue;
                }

                // This holds all shortest paths to move from curr => ch. The code
                // selects which one is cheapest by looking at the DP grid, but
                // in practice the same path is chosen at all depths.
                auto res = Vec<Tsv> {};
                switch (curr) {
                    case 'A':
                        switch (ch) {
                            case 'A':
                                res.push_back("A"_tsv);
                                break;
                            case '>':
                                res.push_back("vA"_tsv);
                                break;
                            case 'v':
                                res.push_back("<vA"_tsv);
                                res.push_back("v<A"_tsv);
                                break;
                            case '<':
                                res.push_back("v<<A"_tsv);
                                res.push_back("<v<A"_tsv);
                                break;
                            case '^':
                                res.push_back("<A"_tsv);
                                break;
                            default:
                                ASSERT(false);
                        }
                        break;
                    case '>':
                        switch (ch) {
                            case 'A':
                                res.push_back("^A"_tsv);
                                break;
                            case '>':
                                res.push_back("A"_tsv);
                                break;
                            case 'v':
                                res.push_back("<A"_tsv);
                                break;
                            case '<':
                                res.push_back("<<A"_tsv);
                                break;
                            case '^':
                                res.push_back("<^A"_tsv);
                                res.push_back("^<A"_tsv);
                                break;
                            default:
                                ASSERT(false);
                        }
                        break;
                    case 'v':
                        switch (ch) {
                            case 'A':
                                res.push_back(">^A"_tsv);
                                res.push_back("^>A"_tsv);
                                break;
                            case '>':
                                res.push_back(">A"_tsv);
                                break;
                            case 'v':
                                res.push_back("A"_tsv);
                                break;
                            case '<':
                                res.push_back("<A"_tsv);
                                break;
                            case '^':
                                res.push_back("^A"_tsv);
                                break;
                            default:
                                ASSERT(false);
                        }
                        break;
                    case '<':
                        switch (ch) {
                            case 'A':
                                res.push_back(">>^A"_tsv);
                                res.push_back(">^>A"_tsv);
                                break;
                            case '>':
                                res.push_back(">>A"_tsv);
                                break;
                            case 'v':
                                res.push_back(">A"_tsv);
                                break;
                            case '<':
                                res.push_back("A"_tsv);
                                break;
                            case '^':
                                res.push_back(">^A"_tsv);
                                break;
                            default:
                                ASSERT(false);
                        }
                        break;
                    case '^':
                        switch (ch) {
                            case 'A':
                                res.push_back(">A"_tsv);
                                break;
                            case '>':
                                res.push_back("v>A"_tsv);
                                res.push_back(">vA"_tsv);
                                break;
                            case 'v':
                                res.push_back("vA"_tsv);
                                break;
                            case '<':
                                res.push_back("v<A"_tsv);
                                break;
                            case '^':
                                res.push_back("A"_tsv);
                                break;
                            default:
                                ASSERT(false);
                        }
                        break;
                    default:
                        ASSERT(false);
                }

                auto score = NumericLimits<i64>::max;
                for (auto ss : res) {
                    ASSERT(ss.ends_with('A'));
                    auto s = 0_i64;
                    auto c = 'A';
                    for (auto nc : ss) {
                        s += dp[c][nc][i - 1];
                        c = nc;
                    }

                    score = min(score, s);
                }
                dp[curr][ch][i] = score;
            }
        }
    }

    // Shortest path algorithm, where the weights of each edge are determined by the
    // precomputed costs above. It would also be sufficent to generate all shortest
    // paths (like in part 1), and find the one with min complexity, but this is probably
    // faster.
    auto dkj = [&](Tsv code) -> i64 {
        struct State {
            i64 score = 0;
            char prev = 'A';
            usize matched = 0;
            usize row = 3;
            usize col = 2;

            bool operator==(State const& other) const = default;
            auto operator<=>(State const& other) const = default;
        };

        auto queue = Set<State> {};

        auto add = [&](State const& state) {
            queue.insert(state);
        };

        add({});
        while (!queue.empty()) {
            auto state = *queue.begin();
            queue.erase(queue.begin());

            if (state.matched == code.size()) {
                return state.score;
            }

            // Need to press A.
            if (keypad[state.row][state.col] == code[state.matched]) {
                add({ state.score + dp[state.prev]['A'][N], 'A', state.matched + 1, state.row, state.col });
                continue;
            }

            for (auto nch : "<>^v"_tsv) {
                int dr = 0;
                int dc = 0;
                switch (nch) {
                    case 'v':
                        dr = 1;
                        break;
                    case '^':
                        dr = -1;
                        break;
                    case '>':
                        dc = 1;
                        break;
                    case '<':
                        dc = -1;
                        break;
                    default:
                        ASSERT(false);
                        break;
                }

                auto nr = state.row + dr;
                auto nc = state.col + dc;
                if (nr >= keypad.size() || nc >= keypad[nr].size() || keypad[nr][nc] == '\0') {
                    continue;
                }

                add({ state.score + dp[state.prev][nch][N], nch, state.matched, nr, nc });
            }
        }
        return -1;
    };

    auto s = 0_i64;
    for (auto [row, line] : enumerate(lines)) {
        auto numeric_part = uparse_int(line.substr(0, 3));
        auto length = dkj(line);
        auto complexity = numeric_part * length;

        if (verbose) {
            println("{} * {} = {}"_sv, numeric_part, length, complexity);
        }
        s += complexity;
    }
    return s;
}
