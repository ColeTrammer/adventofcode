#include <di/container/algorithm/find_if.h>
#include <di/container/algorithm/remove_if.h>
#include <di/container/interface/empty.h>
#include <di/container/queue/queue.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip.h>
#include <di/container/view/zip_transform.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>
#include <runner/aoc_problem_registry.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 15, a, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    auto& line = lines[0];
    for (auto st : split(line, ',')) {
        auto m = 0;
        for (auto ch : st) {
            m += ch;
            m *= 17;
            m %= 256;
        }
        s += m;
    }
    return s;
}

AOC_SOLUTION(2023, 15, b, i64) {
    auto lines = input | splitv('\n');

    auto s = 0;
    auto lens = TreeMap<u8, Vector<Tuple<Tsv, i32>>> {};
    auto& line = lines[0];
    for (auto st : split(line, ',')) {
        auto label = ""_tsv;
        auto focal_length = 0;
        auto x = st.find('=');
        if (x) {
            label = st.substr(st.begin(), x.begin());
            focal_length = *st.back() - '0';
        } else {
            label = st.substr(st.begin(), st.find('-').begin());
        }

        auto m = 0;
        for (auto ch : label) {
            m += ch;
            m *= 17;
            m %= 256;
        }

        auto& ls = lens[m];
        if (focal_length == 0) {
            auto v = di::container::remove_if(ls, [&](auto tuple) {
                return tget<0>(tuple) == label;
            });
            if (v) {
                ls.erase(v.begin());
            }
        } else {
            auto it = find_if(ls, [&](auto tuple) {
                return tget<0>(tuple) == label;
            });
            if (it != ls.end()) {
                tget<1>(*it) = focal_length;
            } else {
                ls.push_back({ label, focal_length });
            }
        }
    }

    for (auto& [b, items] : lens) {
        for (auto [i, item] : enumerate(items)) {
            s += (b + 1) * (i + 1) * tget<1>(item);
        }
    }
    return s;
}
