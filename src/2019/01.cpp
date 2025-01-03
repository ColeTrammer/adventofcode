#include "di/container/interface/empty.h"
#include "di/function/not_fn.h"
#include "dius/print.h"
#include "runner/aoc_problem_registry.h"

AOC_SOLUTION(2019, 01, a, i32) {
    auto lines = input | di::split('\n') | di::filter(di::not_fn(di::empty));
    auto nums = lines | di::transform(di::parse_unchecked<i32>);
    return nums | di::transform([](i32 x) {
               return x / 3 - 2;
           }) |
           di::sum;
}

AOC_SOLUTION(2019, 01, b, i32) {
    auto lines = input | di::split('\n') | di::filter(di::not_fn(di::empty));
    auto nums = lines | di::transform(di::parse_unchecked<i32>);

    return nums | di::transform([](i32 x) {
               auto sum = -x;
               while (x > 0) {
                   sum += x;
                   x = x / 3 - 2;
               }
               return sum;
           }) |
           di::sum;
}
