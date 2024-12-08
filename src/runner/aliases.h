#pragma once

#include "di/container/concepts/container.h"
#include "di/container/concepts/container_of.h"
#include "di/container/concepts/random_access_container.h"
#include "di/container/concepts/sized_container.h"
#include "di/container/interface/access.h"
#include "di/container/interface/empty.h"
#include "di/container/meta/container_reference.h"
#include "di/container/prelude.h"
#include "di/container/string/string.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/vector/vector.h"
#include "di/container/view/pairwise.h"
#include "di/container/view/single.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/curry_back.h"
#include "di/function/equal_or_greater.h"
#include "di/function/equal_or_less.h"
#include "di/function/generator.h"
#include "di/function/invoke.h"
#include "di/function/not_fn.h"
#include "di/function/pipeable.h"
#include "di/function/piped.h"
#include "di/function/proj.h"
#include "di/meta/core.h"
#include "di/meta/language.h"
#include "di/meta/util.h"
#include "di/parser/integral_set.h"
#include "di/parser/parse_unchecked.h"
#include "di/util/to_owned.h"
#include "dius/print.h"

template<typename T>
using Vec = di::Vector<T>;

template<typename K, typename V>
using Map = di::TreeMap<K, V>;

template<typename K, typename V>
using MultiMap = di::TreeMultiSet<K, V>;

template<typename T>
using Set = di::TreeSet<T>;

template<typename T>
using MultiSet = di::TreeMultiSet<T>;

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
template<di::concepts::Integral T>
struct ParseIFunction : di::function::pipeline::EnablePipeline {
    template<typename U>
    requires(di::concepts::ContainerOf<U, char>)
    constexpr auto operator()(U&& input) const {
        auto string = di::forward<U>(input) | di::to<di::Vector>() | di::to<Ts>();
        return di::parser::parse<T>(string.view()).optional_value();
    }

    template<typename U>
    requires(di::concepts::ContainerOf<U, c32>)
    constexpr auto operator()(U&& input) const {
        auto string = di::forward<U>(input) | di::to<di::Vector>() | di::to<di::String>();
        return di::parser::parse<T>(string.view()).optional_value();
    }
};

template<di::concepts::Integral T>
struct ParseUncheckedIFunction : di::function::pipeline::EnablePipeline {
    template<typename U>
    requires(di::concepts::ContainerOf<U, char>)
    constexpr auto operator()(U&& input) const {
        auto string = di::forward<U>(input) | di::to<di::Vector>() | di::to<Ts>();
        return di::parser::parse_unchecked<T>(string);
    }

    template<typename U>
    requires(di::concepts::ContainerOf<U, c32>)
    constexpr auto operator()(U&& input) const {
        auto string = di::forward<U>(input) | di::to<di::Vector>() | di::to<di::String>();
        return di::parser::parse_unchecked<T>(string);
    }
};
}

template<di::concepts::Integral T>
constexpr inline auto parse_i = aoc::detail::ParseIFunction<T> {};

template<di::concepts::Integral T>
constexpr inline auto uparse_i = aoc::detail::ParseUncheckedIFunction<T> {};

constexpr inline auto parse_int = parse_i<i64>;
constexpr inline auto uparse_int = uparse_i<i64>;

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
namespace aoc {
constexpr inline auto access = di::at_unchecked;
}

namespace aoc::detail {
inline auto read_to_string(di::PathView path) -> di::Result<di::TransparentString> {
    auto file = TRY(dius::open_sync(path, dius::OpenMode::Readonly));
    auto bytes = TRY(di::read_all(file));

    return bytes | di::transform([](byte byte) {
               return di::to_integer<char>(byte);
           }) |
           di::to<di::Vector>() | di::to<di::TransparentString>();
}
}

namespace aoc::detail {
struct Between {
    template<typename T, typename U, typename V>
    constexpr static auto operator()(T const& value, U const& lower, V const& upper) -> bool
    requires(requires {
        { value >= lower } -> di::concepts::ImplicitlyConvertibleTo<bool>;
        { value <= upper } -> di::concepts::ImplicitlyConvertibleTo<bool>;
    })
    {
        return di::equal_or_greater(value, lower) && di::equal_or_less(value, upper);
    }
};
}

constexpr inline auto between = di::curry_back(aoc::detail::Between {}, di::meta::c_<3zu>);
