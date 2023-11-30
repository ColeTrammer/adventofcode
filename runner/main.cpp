#include <di/cli/parser.h>
#include <di/container/path/path_view.h>
#include <di/container/string/string.h>
#include <di/container/string/transparent_encoding.h>
#include <di/format/present.h>
#include <dius/main.h>
#include <runner/aoc_problem_registry.h>

namespace aoc {
struct Args {
    di::Optional<di::PathView> input;
    int year { 2023 };
    int day;
    bool part_b { false };
    bool test { false };

    constexpr static auto get_cli_parser() {
        return di::cli_parser<Args>("aoc"_sv, "Advent of Code Solver"_sv)
            .flag<&Args::input>('i', "input"_tsv, "Input file path"_sv)
            .flag<&Args::year>('y', "year"_tsv, "Year to solve"_sv)
            .flag<&Args::day>('d', "day"_tsv, "Day to solve"_sv, true)
            .flag<&Args::part_b>('b', "part-b"_tsv, "Run part b"_sv)
            .flag<&Args::test>('t', "test"_tsv, "Run with test input"_sv);
    }
};

di::Result<void> main(Args& args) {
    auto prefix = TRY(di::present("{}/{:02}"_sv, args.year, args.day));
    auto prefix_as_transparent_string = prefix | di::transform([](c32 code_point) {
                                            return static_cast<char>(code_point);
                                        }) |
                                        di::to<di::Vector>() | di::to<di::TransparentString>();
    auto prefix_as_path = di::Path { di::move(prefix_as_transparent_string) };

    auto default_path = di::move(prefix_as_path) / (args.test ? "test.txt"_pv : "input.txt"_pv);
    auto path = args.input.value_or(default_path);
    auto string = TRY(dius::read_to_string(path) | di::if_error([&](auto&& error) {
                          dius::eprintln("Failed to read input file '{}': {}"_sv, path.data(), error.message());
                      }));

    auto solver = AocProblemRegistry::the().lookup({ args.year, args.day, args.part_b });
    if (!solver) {
        dius::eprintln("No solver found for {} day {} part {}"_sv, args.year, args.day, args.part_b ? "b"_sv : "a"_sv);
        return di::Unexpected(di::BasicError::InvalidArgument);
    }

    (*solver)(string);
    return {};
}
}

DIUS_MAIN(aoc::Args, aoc)
