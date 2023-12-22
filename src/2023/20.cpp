#include <di/container/algorithm/sort.h>
#include <di/container/interface/empty.h>
#include <di/container/queue/queue.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/compare_backwards.h>
#include <di/function/container/function.h>
#include <di/function/not_fn.h>
#include <di/math/lcm.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 20, a, i64) {
    auto lines = input | splitv('\n');
    sort(lines, compare_backwards);

    using Pulse = Tuple<Tsv, Tsv, bool>;
    auto pulses = Queue<Pulse> {};
    auto s1 = 0_i64;
    auto s2 = 0_i64;
    auto add_pulse = [&](Pulse pulse) {
        pulses.push(pulse);
        if (tget<2>(pulse)) {
            s1++;
        } else {
            s2++;
        }
    };

    auto button = [&] {
        s2++;
        auto [_, nodes] = split_two(lines[0].view(), " -> "_tsv);
        for (auto name : split(nodes, ", "_tsv)) {
            add_pulse({ "broadcaster"_tsv, name, false });
        }
    };

    using Gate = Function<void(Pulse)>;
    auto gates = TreeMap<Tsv, Gate> {};

    for (auto [row, line] : enumerate(lines | drop(1))) {
        auto [name, nodes] = split_two(line.view(), " -> "_tsv);
        auto type = name[0];
        name = name.substr(1, {});

        if (type == '%') {
            gates[name] = make_function<void(Pulse)>([name, nodes, state = false, &add_pulse](Pulse pulse) mutable {
                if (tget<2>(pulse) == false) {
                    state = !state;
                    for (auto node : split(nodes, ", "_tsv)) {
                        add_pulse({ name, node, state });
                    }
                }
            });
        } else {
            auto input_nodes = Vector<Tsv> {};
            for (auto const& line : lines) {
                auto [n, nodes] = split_two(line.view(), " -> "_tsv);
                if (n != "broadcaster"_tsv) {
                    n = n.substr(1, {});
                }
                for (auto node : nodes | split(", "_tsv)) {
                    if (node == name) {
                        input_nodes.push_back(n);
                    }
                }
            }
            gates[name] = make_function<void(Pulse)>([name, nodes, &add_pulse, memory = TreeMap<Tsv, bool> {},
                                                      input_nodes = di::move(input_nodes)](Pulse pulse) mutable {
                memory[tget<0>(pulse)] = tget<2>(pulse);
                auto result = !all_of(input_nodes | transform([&](Tsv input) {
                                          return memory[input];
                                      }));
                for (auto node : split(nodes, ", "_tsv)) {
                    add_pulse({ name, node, result });
                }
            });
        }
    }

    for (auto i : range(1000)) {
        (void) i;

        button();
        while (!pulses.empty()) {
            auto pulse = *pulses.pop();
            if (gates[tget<1>(pulse)]) {
                gates[tget<1>(pulse)](pulse);
            }
        }
    }

    return s1 * s2;
}

AOC_SOLUTION(2023, 20, b, i64) {
    auto lines = input | splitv('\n');
    sort(lines, compare_backwards);

    using Pulse = Tuple<Tsv, Tsv, bool>;
    auto pulses = Queue<Pulse> {};
    auto add_pulse = [&](Pulse pulse) {
        pulses.push(pulse);
    };

    auto button = [&] {
        auto [_, nodes] = split_two(lines[0].view(), " -> "_tsv);
        for (auto name : split(nodes, ", "_tsv)) {
            add_pulse({ "broadcaster"_tsv, name, false });
        }
    };

    using Gate = Function<void(Pulse)>;
    auto gates = TreeMap<Tsv, Gate> {};

    auto rx_values = Vector<Tsv> {};
    for (auto [row, line] : enumerate(lines | drop(1))) {
        auto [name, nodes] = split_two(line.view(), " -> "_tsv);
        auto type = name[0];
        name = name.substr(1, {});

        if (type == '%') {
            gates[name] = make_function<void(Pulse)>([name, nodes, state = false, &add_pulse](Pulse pulse) mutable {
                if (tget<2>(pulse) == false) {
                    state = !state;
                    for (auto node : split(nodes, ", "_tsv)) {
                        add_pulse({ name, node, state });
                    }
                }
            });
        } else {
            auto input_nodes = Vector<Tsv> {};
            for (auto const& line : lines) {
                auto [n, nodes] = split_two(line.view(), " -> "_tsv);

                if (n != "broadcaster"_tsv) {
                    n = n.substr(1, {});
                }
                for (auto node : nodes | split(", "_tsv)) {
                    if (node == name) {
                        input_nodes.push_back(n);
                    }
                }
            }
            if (nodes == "rx"_tsv) {
                rx_values = di::clone(input_nodes);
            }
            gates[name] = make_function<void(Pulse)>([name, nodes, &add_pulse, memory = TreeMap<Tsv, bool> {},
                                                      input_nodes = di::move(input_nodes)](Pulse pulse) mutable {
                memory[tget<0>(pulse)] = tget<2>(pulse);
                auto result = !all_of(input_nodes | transform([&](Tsv input) {
                                          return memory[input];
                                      }));
                for (auto node : split(nodes, ", "_tsv)) {
                    add_pulse({ name, node, result });
                }
            });
        }
    }

    auto rx_cycles = TreeMap<Tsv, i64> {};

    for (auto i = 1_i64; rx_cycles.size() < rx_values.size(); ++i) {
        button();

        while (!pulses.empty()) {
            auto pulse = *pulses.pop();
            if (gates[tget<1>(pulse)]) {
                gates[tget<1>(pulse)](pulse);
            }

            if (contains(rx_values, tget<1>(pulse)) && !tget<2>(pulse)) {
                rx_cycles.try_emplace(tget<1>(pulse), i);
            }
        }
    }

    auto s = 1_i64;
    for (auto x : values(rx_cycles)) {
        s = lcm(s, x);
    }
    return s;
}
