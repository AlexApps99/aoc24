#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

static inline uint64_t concat(uint64_t a, uint64_t b) {
    if (b != 0) {
        uint64_t temp_b = b;
        while (temp_b != 0) {
            temp_b /= 10;
            a *= 10;
        }
    }
    return a + b;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
static void handle_eqn(uint64_t goal, const std::vector<uint64_t> &nums,
                       uint64_t &count1, uint64_t &count2) {
    static std::vector<int64_t> results{};
    static std::vector<int64_t> inputs{};
    results.clear();
    inputs.clear();

    results.emplace_back(static_cast<int64_t>(nums[0]));

    for (uint64_t i = 1; i < nums.size(); i++) {
        std::swap(results, inputs);
        results.clear();

        for (int64_t input : inputs) {
            bool is_part_2_only = input < 0;
            uint64_t abs_input = std::abs(input);

            int64_t mul = static_cast<int64_t>(abs_input * nums[i]);
            results.emplace_back(is_part_2_only ? -mul : mul);

            int64_t sum = static_cast<int64_t>(abs_input + nums[i]);
            results.emplace_back(is_part_2_only ? -sum : sum);

            int64_t concat_res =
                static_cast<int64_t>(concat(abs_input, nums[i]));
            results.emplace_back(-concat_res);
        }
    }

    int64_t pos_goal = static_cast<int64_t>(goal);
    int64_t neg_goal = -static_cast<int64_t>(goal);

    bool incremented_count2 = false;
    for (uint32_t i = 0; i < results.size(); i++) {
        if (results[i] == pos_goal) {
            count1 += goal;
            if (!incremented_count2) {
                count2 += goal;
                incremented_count2 = true;
            }
            break;
        } else if (results[i] == neg_goal) {
            if (!incremented_count2) {
                count2 += goal;
                incremented_count2 = true;
            }
        }
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_07(std::istream &in, std::string &out1, std::string &out2) {
    uint64_t count1 = 0;
    uint64_t count2 = 0;

    std::string line;
    while (std::getline(in, line)) {
        size_t delim = line.find(": ");
        if (delim == std::string::npos) {
            continue;
        }
        uint64_t goal = std::stoul(line.substr(0, delim));

        // read from str
        std::stringstream ss(line.substr(delim + 2));
        const std::vector<uint64_t> nums{std::istream_iterator<uint64_t>(ss),
                                         {}};

        handle_eqn(goal, nums, count1, count2);
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
