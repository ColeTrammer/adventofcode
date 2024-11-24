#pragma once

#include <di/container/string/string_view.h>
#include <di/container/tree/tree_map.h>
#include <dius/print.h>

namespace aoc {
class AocProblemRegistry {
private:
    using Function = void (*)(di::TransparentStringView, bool);
    using Key = di::Tuple<int, int, bool>;

public:
    static inline AocProblemRegistry& the() {
        static AocProblemRegistry s_the {};
        return s_the;
    }

    constexpr void register_solver(Key key, Function function) { m_map[key] = function; }

    constexpr auto lookup(Key key) const { return m_map.at(key); }

    constexpr auto list() const { return di::keys(m_map); }

private:
    AocProblemRegistry() = default;

    di::TreeMap<Key, Function> m_map;
};

#define AOC_SOLUTION(year, day, part, Ret)                                                                             \
    static Ret solve_##year##_##day##_##part(di::TransparentStringView, bool);                                         \
    static __attribute__((constructor)) void __registersolve_##year##_##day##_##part() {                               \
        aoc::AocProblemRegistry::the().register_solver(                                                                \
            { year, day, "" #part ""_tsv == "a"_tsv ? false : true },                                                  \
            [](di::TransparentStringView view, bool is_test) {                                                         \
                auto result = solve_##year##_##day##_##part(view, is_test);                                            \
                dius::println("" #year " day {} part " #part ": {}"_sv, di::parse_unchecked<i32>(""_sv #day), result); \
            });                                                                                                        \
    }                                                                                                                  \
    static Ret solve_##year##_##day##_##part([[maybe_unused]] di::TransparentStringView input,                         \
                                             [[maybe_unused]] bool is_test)
}
