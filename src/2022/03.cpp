#include "di/container/algorithm/fold_left_first.h"
#include "di/container/tree/tree_set.h"
#include "di/function/bit_and.h"
#include "runner/aoc_problem_registry.h"

static int prio(char value) {
    if (('a'_mc - 'z'_mc)(value)) {
        return 1 + value - 'a';
    } else {
        return 27 + value - 'A';
    }
}

AOC_SOLUTION(2022, 3, a, i32) {
    return input | di::split('\n') | di::transform([](di::TransparentStringView runsack) {
               auto halfway_point = *runsack.iterator_at_offset(runsack.size_bytes() / 2);
               auto left = runsack.substr(runsack.begin(), halfway_point) | di::to<di::TreeSet>();
               auto right = runsack.substr(halfway_point) | di::to<di::TreeSet>();

               left &= right;
               return *left.front();
           }) |
           di::transform(prio) | di::sum;
}

AOC_SOLUTION(2022, 3, b, i32) {
    return input | di::split('\n') | di::chunk(3) |
           di::transform(di::transform(di::to<di::TreeSet>()) | di::fold_left_first(di::bit_and)) |
           di::transform([](auto optional_set) {
               return *optional_set->front();
           }) |
           di::transform(prio) | di::sum;
}
