#include <di/container/algorithm/count_if.h>
#include <di/function/identity.h>
#include <runner/aoc_problem_registry.h>

#include <di/container/algorithm/product.h>
#include <di/container/interface/empty.h>
#include <di/container/tree/tree_map.h>
#include <di/container/tree/tree_multimap.h>
#include <di/container/tree/tree_multiset.h>
#include <di/container/tree/tree_set.h>
#include <di/container/view/zip_transform.h>
#include <di/function/not_fn.h>
#include <dius/print.h>
#include <runner/aliases.h>

using namespace di;
using namespace dius;

AOC_SOLUTION(2023, 6, a, i32) {
    auto lines = splitv(input, '\n');

    auto times = all_nums(lines[0]);
    auto distances = all_nums(lines[1]);

    return zip_transform(
               [&](auto time, auto d) {
                   return count_if(range(time), [&](auto t) {
                       return t * (time - t) > d;
                   });
               },
               times, distances) |
           product;
}

AOC_SOLUTION(2023, 6, b, i32) {
    auto lines = splitv(input, '\n');

    auto time = lines[0] | filter('0'_mc - '9'_mc) | uparse_i<i64>;
    auto d = lines[1] | filter('0'_mc - '9'_mc) | uparse_i<i64>;

    return count_if(range(time), [&](auto t) {
        return t * (time - t) > d;
    });
}
