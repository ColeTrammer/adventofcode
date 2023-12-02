#pragma once

#include <di/container/string/string_view.h>
#include <di/container/tree/tree_map.h>
#include <dius/print.h>
#include <runner/helpful_includes.h>

namespace aoc {
class AocProblemRegistry {
private:
    using Function = void (*)(di::TransparentStringView);
    using Key = di::Tuple<int, int, bool>;

public:
    static inline AocProblemRegistry& the() {
        static AocProblemRegistry s_the {};
        return s_the;
    }

    constexpr void register_solver(Key key, Function function) { m_map[key] = function; }

    constexpr auto lookup(Key key) const { return m_map.at(key); }

private:
    AocProblemRegistry() = default;

    di::TreeMap<Key, Function> m_map;
};

#define AOC_SOLUTION(year, day, part, Ret)                                                                             \
    static Ret solve_##year##_##day##_##part(di::TransparentStringView);                                               \
    static __attribute__((constructor)) void __registersolve_##year##_##day##_##part() {                               \
        aoc::AocProblemRegistry::the().register_solver(                                                                \
            { year, day, "" #part ""_tsv == "a"_tsv ? false : true }, [](di::TransparentStringView view) {             \
                auto result = solve_##year##_##day##_##part(view);                                                     \
                dius::println("" #year " day {} part " #part ": {}"_sv, di::parse_unchecked<i32>(""_sv #day), result); \
            });                                                                                                        \
    }                                                                                                                  \
    static Ret solve_##year##_##day##_##part(di::TransparentStringView input)
}