#pragma once

#include <di/container/concepts/container.h>
#include <di/container/concepts/container_of.h>
#include <di/container/concepts/random_access_container.h>
#include <di/container/concepts/sized_container.h>
#include <di/container/interface/access.h>
#include <di/container/meta/container_reference.h>
#include <di/container/string/string.h>
#include <di/container/tree/tree_set.h>
#include <di/container/vector/vector.h>
#include <di/container/view/pairwise.h>
#include <di/container/view/single.h>
#include <di/function/curry_back.h>
#include <di/function/generator.h>
#include <di/function/invoke.h>
#include <di/function/pipeable.h>
#include <di/function/piped.h>
#include <di/meta/core.h>
#include <di/meta/language.h>
#include <di/meta/util.h>
#include <di/parser/integral_set.h>
#include <di/parser/parse_unchecked.h>
#include <di/util/to_owned.h>
#include <dius/print.h>

using Ts = di::TransparentString;
using Tsv = di::TransparentStringView;

constexpr inline auto is_digit = di::piped('0'_mc - '9'_mc);
constexpr inline auto is_alpha = di::piped('a'_mc - 'z'_mc || 'A'_mc - 'Z'_mc);
constexpr inline auto is_alnum = di::piped('0'_mc - '9'_mc || 'a'_mc - 'z'_mc || 'A'_mc - 'Z'_mc);
constexpr inline auto is_xdigit = di::piped('0'_mc - '9'_mc || 'a'_mc - 'f'_mc || 'A'_mc - 'F'_mc);
constexpr inline auto is_space = di::piped(' '_mc || '\t'_mc || '\n'_mc || '\r'_mc || '\v'_mc || '\f'_mc);
constexpr inline auto is_dot = di::piped('.'_mc);

namespace aoc::detail {
struct TrimFunction : di::function::pipeline::EnablePipeline {
    constexpr auto operator()(Tsv view) const -> Tsv {
        auto begin = view.begin();
        auto end = view.end();

        while (begin != end && is_space(*begin)) {
            begin++;
        }
        while (begin != end && is_space(*(end - 1))) {
            end--;
        }

        return Tsv { begin, end };
    }
};
}

constexpr inline auto trim = aoc::detail::TrimFunction {};

static_assert(trim("  \t\n\r\v\fabc  \t\n\r\v\f"_tsv) == "abc"_tsv);

namespace aoc::detail {
template<usize index>
struct TGetFunction : di::function::pipeline::EnablePipeline {
    template<typename T>
    constexpr auto operator()(T&& tuple) const -> decltype(auto) {
        return di::get<index>(di::forward<T>(tuple));
    }
};
}

template<usize index>
constexpr inline auto tget = aoc::detail::TGetFunction<index> {};

namespace aoc::detail {
template<typename T>
concept UsizeOrUsizeContainer = di::concepts::DecaysTo<T, usize> || di::concepts::ContainerOf<T, usize>;

template<UsizeOrUsizeContainer T>
constexpr auto maybe_single(T&& value) -> decltype(auto) {
    if constexpr (di::concepts::DecaysTo<T, usize>) {
        return di::single(value);
    } else {
        return di::forward<T>(value);
    }
}

struct NeighborsFunction {
    template<di::concepts::RandomAccessContainer T>
    requires(di::concepts::RandomAccessContainer<di::meta::ContainerReference<T>> && di::concepts::SizedContainer<T> &&
             di::concepts::SizedContainer<di::meta::ContainerReference<T>>)
    auto operator()(T&& grid, UsizeOrUsizeContainer auto&& rows, UsizeOrUsizeContainer auto&& cols,
                    bool diag = true) const {
        auto const row_count = grid.size();

        auto set = di::TreeSet<di::Tuple<usize, usize>> {};
        auto result = di::Vector<di::Tuple<usize, usize, di::meta::ContainerValue<di::meta::ContainerReference<T>>>> {};
        for (auto row : maybe_single(rows)) {
            for (auto col : maybe_single(cols)) {
                set.insert({ row, col });
            }
        }

        for (auto row : maybe_single(rows)) {
            for (auto col : maybe_single(cols)) {
                for (auto dr : di::Array { -1, 0, 1 }) {
                    for (auto dc : di::Array { -1, 0, 1 }) {
                        if (dr == 0 && dc == 0) {
                            continue;
                        }
                        if (!diag && dr != 0 && dc != 0) {
                            continue;
                        }
                        if (row + dr >= row_count) {
                            continue;
                        }
                        auto const col_count = grid[row + dr].size();
                        if (col + dc >= col_count) {
                            continue;
                        }
                        if (set.contains({ row + dr, col + dc })) {
                            continue;
                        }
                        set.insert({ row + dr, col + dc });
                        result.push_back(di::Tuple { row + dr, col + dc, grid[row + dr][col + dc] });
                    }
                }
            }
        }
        return result;
    }
};
}

constexpr inline auto neighbors = aoc::detail::NeighborsFunction {};

namespace aoc::detail {
template<typename P, typename F>
class ProjImpl : di::function::pipeline::EnablePipeline {
public:
    template<typename Pn, typename Fn>
    constexpr explicit ProjImpl(Pn&& p, Fn&& f) : m_proj(di::forward<Pn>(p)), m_f(di::forward<Fn>(f)) {}

    constexpr ProjImpl(ProjImpl const&) = default;
    constexpr ProjImpl(ProjImpl&&) = default;

    constexpr ProjImpl& operator=(ProjImpl const&) = delete;
    constexpr ProjImpl& operator=(ProjImpl&&) = delete;

    template<typename... Args>
    requires(di::concepts::Invocable<F&, di::meta::InvokeResult<P&, Args>...>)
    constexpr auto operator()(Args&&... args) & -> decltype(auto) {
        return di::invoke(m_f, di::invoke(m_proj, di::forward<Args>(args))...);
    }

    template<typename... Args>
    requires(di::concepts::Invocable<F const&, di::meta::InvokeResult<P const&, Args>...>)
    constexpr auto operator()(Args&&... args) const& -> decltype(auto) {
        return di::invoke(m_f, di::invoke(m_proj, di::forward<Args>(args))...);
    }

    template<typename... Args>
    requires(di::concepts::Invocable<F &&, di::meta::InvokeResult<P&, Args>...>)
    constexpr auto operator()(Args&&... args) && -> decltype(auto) {
        return di::invoke(di::move(m_f), di::invoke(m_proj, di::forward<Args>(args))...);
    }

    template<typename... Args>
    requires(di::concepts::Invocable<F const &&, di::meta::InvokeResult<P const&, Args>...>)
    constexpr auto operator()(Args&&... args) const&& -> decltype(auto) {
        return di::invoke(di::move(m_f), di::invoke(m_proj, di::forward<Args>(args))...);
    }

private:
    P m_proj;
    F m_f;
};

struct ProjFunction {
    template<di::concepts::DecayConstructible P, di::concepts::DecayConstructible F>
    constexpr auto operator()(P&& predicate, F&& function) const {
        return ProjImpl<di::meta::Decay<P>, di::meta::Decay<F>> { di::forward<P>(predicate), di::forward<F>(function) };
    }
};
}

constexpr inline auto proj = aoc::detail::ProjFunction {};

namespace aoc::detail {
template<di::concepts::Integral T>
struct ParseIFunction : di::function::pipeline::EnablePipeline {
    template<typename U>
    constexpr auto operator()(U&& input) const {
        auto string = di::forward<U>(input) | di::to<di::Vector>() | di::to<Ts>();
        return di::parser::parse<T>(string.view()).optional_value();
    }
};

template<di::concepts::Integral T>
struct ParseUncheckedIFunction : di::function::pipeline::EnablePipeline {
    template<typename U>
    constexpr auto operator()(U&& input) const {
        auto string = di::forward<U>(input) | di::to<di::Vector>() | di::to<Ts>();
        return di::parser::parse_unchecked<T>(string);
    }
};
}

template<di::concepts::Integral T>
constexpr inline auto parse_i = aoc::detail::ParseIFunction<T> {};

template<di::concepts::Integral T>
constexpr inline auto uparse_i = aoc::detail::ParseUncheckedIFunction<T> {};

constexpr inline auto parse_int = parse_i<i32>;
constexpr inline auto uparse_int = uparse_i<i32>;

namespace aoc::detail {
struct SplitTwo {
    auto operator()(auto string, auto split_on) const {
        auto pairs = di::split(string, split_on) | di::pairwise;
        return *pairs.front();
    }
};
}

constexpr inline auto split_two = di::curry_back(aoc::detail::SplitTwo {}, di::c_<2zu>);

namespace aoc::detail {
struct SplitV {
    template<typename T, typename U>
    constexpr auto operator()(T&& container, U&& split_on) const {
        return di::split(di::forward<T>(container), di::forward<U>(split_on)) | di::transform(di::to_owned) |
               di::to<di::Vector>();
    }
};
}

constexpr inline auto splitv = di::curry_back(aoc::detail::SplitV {}, di::c_<2zu>);

namespace aoc::detail {
template<typename I>
struct AllNums {
    template<typename T>
    auto operator()(T&& container) const {
        if constexpr (!di::concepts::DecaysTo<T, Tsv> && !di::concepts::DecaysTo<T, Ts>) {
            return di::forward<T>(container) | di::transform(parse_i<I>) | di::filter([](auto const& value) {
                       return value.has_value();
                   }) |
                   di::transform([](auto const& value) {
                       return *value;
                   }) |
                   di::to<di::Vector>();
        } else {
            return (*this)(di::forward<T>(container), ' ');
        }
    }

    template<typename T>
    auto operator()(T&& container, auto split_on) const {
        return (*this)(di::forward<T>(container) | di::split(split_on));
    }
};
}

constexpr inline auto all_nums = di::curry_back(aoc::detail::AllNums<int> {}, di::c_<2zu>);

template<typename I>
constexpr inline auto all_nums_i = di::curry_back(aoc::detail::AllNums<I> {}, di::c_<2zu>);

constexpr inline auto first = di::front_unchecked;
constexpr inline auto last = di::back_unchecked;
constexpr inline auto access = di::at_unchecked;
