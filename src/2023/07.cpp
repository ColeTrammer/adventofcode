#include "di/container/algorithm/compare.h"
#include "di/container/algorithm/count_if.h"
#include "di/container/algorithm/equal.h"
#include "di/container/algorithm/sort.h"
#include "di/container/interface/empty.h"
#include "di/container/tree/tree_map.h"
#include "di/container/tree/tree_multimap.h"
#include "di/container/tree/tree_multiset.h"
#include "di/container/tree/tree_set.h"
#include "di/container/view/keys.h"
#include "di/container/view/zip.h"
#include "di/container/view/zip_transform.h"
#include "di/function/compare_backwards.h"
#include "di/function/not_fn.h"
#include "di/function/ycombinator.h"
#include "di/math/numeric_limits.h"
#include "dius/print.h"
#include "runner/aliases.h"
#include "runner/aoc_problem_registry.h"

using namespace di;
using namespace dius;

struct Card {
    auto operator<=>(Card const&) const = default;
    bool operator==(Card const&) const = default;

    Card() = default;

    Card(char ch) {
        value = [&] {
            switch (ch) {
                case 'A':
                    return 14;
                case 'K':
                    return 13;
                case 'Q':
                    return 12;
                case 'J':
                    return 11;
                case 'j':
                    return 1;
                case 'T':
                    return 10;
                default:
                    return ch - '0';
            }
        }();
    }

    bool joker() const { return value == 1; }

    u8 value { 0 };
};

enum class Type {
    Five = 15,
    Four = 14,
    Full = 13,
    Three = 12,
    Two = 11,
    One = 10,
    High = 0,
};

struct Rank {
    bool operator==(Rank const& other) const {
        return this->type == other.type && di::container::equal(this->cards, other.cards);
    }
    auto operator<=>(Rank const& other) const {
        if (auto result = this->type <=> other.type; result != 0) {
            return result;
        }
        return di::container::compare(this->cards, other.cards);
    }

    Rank(Type type, auto cards) : type(type), cards { cards } {}
    Rank() = default;

    Type type { Type::High };
    Array<Card, 5> cards {};
};

struct Hand {
    Hand(Vector<Card> cards) {
        copy(cards, this->cards.begin());
        rank = get_rank();
    }

    bool operator==(Hand const& other) const { return this->rank == other.rank; }

    auto operator<=>(Hand const& other) const { return this->rank <=> other.rank; }

    Rank get_rank() const {
        auto compute = [&](Array<Card, 5> cards_in, Array<Card, 5> og_cards) {
            auto cards = TreeMap<Card, usize> {};
            for (auto card : cards_in) {
                cards[card]++;
            }

            auto max_count = max(values(cards));
            auto sorted = keys(cards) | to<Vector>();
            sort(sorted, compare_backwards, [&](auto card) {
                return Tuple { cards[card], card };
            });

            if (max_count == 5) {
                return Rank { Type::Five, og_cards };
            }
            if (max_count == 4) {
                return Rank { Type::Four, og_cards };
            }
            if (cards.size() == 2) {
                return Rank { Type::Full, og_cards };
            }
            if (max_count == 3) {
                return Rank { Type::Three, og_cards };
            }
            if (cards.size() == 3) {
                return Rank { Type::Two, og_cards };
            }
            if (max_count == 2) {
                return Rank { Type::One, og_cards };
            }
            return Rank { Type::High, og_cards };
        };

        auto bt = ycombinator([&](auto& bt, Array<Card, 5> cards, usize i) -> Rank {
            if (i == 5) {
                return compute(cards, this->cards);
            }
            if (!cards[i].joker()) {
                return bt(cards, i + 1);
            }
            auto max = Rank {};
            for (usize val = 1; val < 15; val++) {
                if (val == 11) {
                    continue;
                }
                auto v = cards[i].value;
                cards[i].value = val;
                max = di::max(max, bt(cards, i + 1));
                cards[i].value = v;
            }
            return max;
        });

        return bt(this->cards, 0);
    }

    Array<Card, 5> cards {};
    Rank rank {};
};

AOC_SOLUTION(2023, 7, a, i32) {
    auto lines = input | splitv('\n');

    auto sum = 0;

    auto hands = Vector<Hand> {};
    auto bids = Vector<usize> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [cards_s, bid_s] = split_two(line.view(), ' ');
        auto bid = uparse_i<usize>(bid_s);
        auto hand = Hand { cards_s | transform([](auto ch) {
                               return Card(ch);
                           }) |
                           to<Vector>() };

        hands.push_back(hand);
        bids.push_back(bid);
    }

    sort(zip(hands, bids), compare_backwards);

    for (auto [i, bid] : bids | enumerate) {
        sum += (hands.size() - i) * bid;
    }
    return sum;
}

AOC_SOLUTION(2023, 7, b, i32) {
    auto lines = input | splitv('\n');

    auto sum = 0;

    auto hands = Vector<Hand> {};
    auto bids = Vector<usize> {};
    for (auto [row, line] : enumerate(lines)) {
        auto [cards_s, bid_s] = split_two(line.view(), ' ');
        auto bid = uparse_i<usize>(bid_s);
        auto hand = Hand { cards_s | transform([](auto ch) {
                               if (ch == 'J') {
                                   ch = 'j';
                               }
                               return Card(ch);
                           }) |
                           to<Vector>() };

        hands.push_back(hand);
        bids.push_back(bid);
    }

    sort(zip(hands, bids), compare_backwards);

    for (auto [i, bid] : bids | enumerate) {
        sum += (hands.size() - i) * bid;
    }
    return sum;
}
