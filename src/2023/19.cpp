#include <di/bit/bitset/bit_set.h>
#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/container/function.h>
#include <di/function/not_fn.h>
#include <di/function/ycombinator.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 19, a, i64) {
    auto [rules, parts] = split_two(input, "\n\n"_tsv);

    using Input = TreeMap<char, i64>;
    using Step = Function<Optional<Tsv>(Input const&)>;
    using Workflow = Vector<Step>;

    auto workflows = TreeMap<Tsv, Workflow> {};

    for (auto rule : rules | split('\n')) {
        auto [name, wfs] = split_two(rule, '{');
        wfs = wfs.substr(0, wfs.size() - 1);

        auto steps = split(wfs, ',') | transform([](Tsv step) -> Step {
                         if (auto colon = step.find(':')) {
                             return [step](Input const& input) -> Optional<Tsv> {
                                 auto colon = step.find(':');
                                 auto cond = step.substr(step.begin(), colon.begin());
                                 auto dest = step.substr(colon.end());

                                 auto v = cond[0];
                                 auto s = cond[1];
                                 auto n = uparse_i<i64>(cond.substr(2, {}));
                                 if (s == '<' && *input.at(v) < n) {
                                     return dest;
                                 }
                                 if (s == '>' && *input.at(v) > n) {
                                     return dest;
                                 }
                                 return nullopt;
                             };
                         } else {
                             return [step](Input const&) -> Optional<Tsv> {
                                 return step;
                             };
                         }
                     }) |
                     to<Vector>();
        workflows.insert({ name, di::move(steps) });
    }

    auto s = 0_i64;
    for (auto part : split(parts, '\n')) {
        part = part.substr(1, part.size() - 2);
        auto input = Input {};
        for (auto var : split(part, ',')) {
            auto v = var[0];
            auto x = uparse_i<i64>(var.substr(2, {}));
            input[v] = x;
        }

        auto name = "in"_tsv;
        while (name != "A"_tsv && name != "R"_tsv) {
            for (auto& s : *workflows.at(name)) {
                if (auto next = s(input)) {
                    name = *next;
                    break;
                }
            }
        }

        if (name == "A"_tsv) {
            s += sum(values(input));
        }
    }
    return s;
}

struct Constraint {
    BitSet<4000> x;
    BitSet<4000> m;
    BitSet<4000> a;
    BitSet<4000> s;
};

AOC_SOLUTION(2023, 19, b, i64) {
    auto [rules, parts] = split_two(input, "\n\n"_tsv);

    using Step = Function<Tsv(Constraint&, Constraint&)>;
    using Workflow = Vector<Step>;

    auto workflows = TreeMap<Tsv, Workflow> {};

    for (auto rule : rules | split('\n')) {
        auto [name, wfs] = split_two(rule, '{');
        wfs = wfs.substr(0, wfs.size() - 1);

        auto steps = split(wfs, ',') | transform([](Tsv step) -> Step {
                         if (auto colon = step.find(':')) {
                             return [step](Constraint& input, Constraint& inv_input) -> Tsv {
                                 auto colon = step.find(':');
                                 auto cond = step.substr(step.begin(), colon.begin());
                                 auto dest = step.substr(colon.end());

                                 auto v = cond[0];
                                 auto s = cond[1];
                                 auto n = uparse_i<i64>(cond.substr(2, {}));

                                 auto bitset = [&] {
                                     if (v == 'x') {
                                         return &Constraint::x;
                                     } else if (v == 'm') {
                                         return &Constraint::m;
                                     } else if (v == 'a') {
                                         return &Constraint::a;
                                     } else {
                                         return &Constraint::s;
                                     }
                                 }();

                                 if (s == '<') {
                                     for (auto i : range(1_i64, 4001_i64)) {
                                         if (i >= n) {
                                             (input.*bitset)[i - 1] = false;
                                         } else {
                                             (inv_input.*bitset)[i - 1] = false;
                                         }
                                     }
                                     return dest;
                                 } else {
                                     for (auto i : range(1_i64, 4001_i64)) {
                                         if (i <= n) {
                                             (input.*bitset)[i - 1] = false;
                                         } else {
                                             (inv_input.*bitset)[i - 1] = false;
                                         }
                                     }
                                     return dest;
                                 }
                             };
                         } else {
                             return [step](Constraint&, Constraint&) -> Tsv {
                                 return step;
                             };
                         }
                     }) |
                     to<Vector>();
        workflows.insert({ name, di::move(steps) });
    }

    auto s = 0_i64;

    auto bs_size = [](auto const& bitset) {
        auto r = 0_i64;
        for (auto i : range(bitset.size())) {
            if (bitset[i]) {
                r++;
            }
        }
        return r;
    };

    auto dfs = ycombinator([&](auto&& dfs, Constraint& input, Tsv name) -> void {
        if (name == "R"_tsv) {
            return;
        }
        if (name == "A"_tsv) {
            s += bs_size(input.x) * bs_size(input.m) * bs_size(input.a) * bs_size(input.s);
            return;
        }
        auto next_constraint = input;
        for (auto& step : workflows[name]) {
            auto next = step(input, next_constraint);
            dfs(input, next);
            input = next_constraint;
        }
    });

    auto constraint = Constraint {};
    for (auto i : range(1_i64, 4001_i64)) {
        constraint.x[i - 1] = true;
        constraint.m[i - 1] = true;
        constraint.a[i - 1] = true;
        constraint.s[i - 1] = true;
    }

    dfs(constraint, "in"_tsv);

    return s;
}
