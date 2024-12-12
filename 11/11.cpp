#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

static inline uint32_t get_num_digits(uint64_t n) {
    if (n == 0) {
        return 1;
    }
    uint32_t count = 0;
    while (n != 0) {
        n /= 10;
        count++;
    }
    return count;
}

static inline uint64_t pow10(uint32_t n) {
    uint64_t result = 1;
    for (uint32_t i = 0; i < n; i++) {
        result *= 10;
    }
    return result;
}

static uint64_t count_stones(uint64_t starting_stone, uint64_t depth) {
    if (depth == 0) {
        return 1;
    }

    static std::map<std::pair<uint64_t, uint64_t>, uint64_t> cache;

    if (auto it = cache.find({starting_stone, depth}); it != cache.end()) {
        return it->second;
    }

    uint64_t result = 0;
    if (starting_stone == 0) {
        result = count_stones(1, depth - 1);
    } else {
        uint32_t num_digits = get_num_digits(starting_stone);
        if (num_digits % 2 == 0) {
            // even number of digits
            uint64_t divisor = pow10(num_digits / 2);
            uint64_t left_half = starting_stone / divisor;
            uint64_t right_half = starting_stone % divisor;
            result = count_stones(left_half, depth - 1) +
                     count_stones(right_half, depth - 1);
        } else {
            result = count_stones(2024 * starting_stone, depth - 1);
        }
    }

    cache[{starting_stone, depth}] = result;

    return result;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_11(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::string input_line;
    std::getline(in, input_line);

    std::istringstream iss(input_line);
    std::vector<uint64_t> input{std::istream_iterator<uint64_t>(iss), {}};

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    for (uint64_t i : input) {
        count1 += count_stones(i, 25);
    }

    for (uint64_t i : input) {
        count2 += count_stones(i, 75);
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
