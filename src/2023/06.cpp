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
    auto [timess, distancess] = split_two(input, '\n');
    auto [_, timesl] = split_two(timess, ':');
    auto [__, distancesl] = split_two(distancess, ':');

    auto times = all_nums(timesl, ' ');
    auto distances = all_nums(distancesl, ' ');

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
    auto [timess, distancess] = split_two(input, '\n');
    auto [_, timesl] = split_two(timess, ':');
    auto [__, distancesl] = split_two(distancess, ':');

    auto time = timesl | filter(~' '_mc) | uparse_i<i64>;
    auto d = distancesl | filter(~' '_mc) | uparse_i<i64>;

    return count_if(range(time), [&](auto t) {
        return t * (time - t) > d;
    });
}
