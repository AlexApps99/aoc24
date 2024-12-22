#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

static void mix_n_prune(uint64_t val, uint64_t &secret) {
    secret ^= val;
    secret &= 0xFF'FF'FF;
}

// hashing of array of 4 elements
namespace std {
template <> struct hash<std::array<int64_t, 4>> {
    std::size_t operator()(const std::array<int64_t, 4> &arr) const {
        std::size_t result = 0;
        for (const int64_t elem : arr) {
            result ^= std::hash<int64_t>{}(elem);
        }
        return result;
    }
};
} // namespace std

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_22(std::istream &in, std::string &out1, std::string &out2) {
    std::string input_line{};

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    std::unordered_map<std::array<int64_t, 4>, uint64_t> profits{};
    while (std::getline(in, input_line)) {
        uint64_t secret = std::stoull(input_line);

        std::unordered_set<std::array<int64_t, 4>> seen_deltas{};
        std::vector<int64_t> deltas{};
        uint64_t price = secret % 10;
        for (uint64_t i = 0; i < 2000; i++) {
            mix_n_prune(secret << 6, secret);
            mix_n_prune(secret >> 5, secret);
            mix_n_prune(secret << 11, secret);

            int64_t prev_price = price;
            price = secret % 10;
            int64_t delta =
                static_cast<int64_t>(price) - static_cast<int64_t>(prev_price);
            deltas.push_back(delta);
            if (deltas.size() >= 4) {
                std::array<int64_t, 4> delta_arr{
                    deltas[deltas.size() - 4], deltas[deltas.size() - 3],
                    deltas[deltas.size() - 2], deltas[deltas.size() - 1]};

                if (seen_deltas.find(delta_arr) == seen_deltas.end()) {
                    seen_deltas.insert(delta_arr);
                    profits[delta_arr] += price;
                }
            }
        }
        count1 += secret;
    }

    std::array<int64_t, 4> target{0, 0, 0, 0};
    for (const auto &[delta, profit] : profits) {
        if (profit > count2) {
            count2 = profit;
            target = delta;
        }
    }
    for (const auto &elem : target) {
        std::cerr << elem << ',';
    }
    std::cerr << '\n';

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
