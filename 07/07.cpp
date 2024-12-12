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

static inline std::optional<uint64_t> concat(uint64_t a, uint64_t b) {
    uint64_t digits = get_num_digits(b);
    uint64_t mul_fac = 1;
    for (uint64_t k = 0; k < digits; k++) {
        mul_fac *= 10;
    }
    uint64_t res = a * mul_fac;
    if (res / mul_fac != a) {
        // overflow
        return std::nullopt;
    }
    uint64_t res2 = res + b;
    if (res2 < res) {
        // underflow
        return std::nullopt;
    }
    return res2;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
static void handle_eqn(uint64_t goal, const std::vector<uint64_t> &nums,
                       uint64_t &count1, uint64_t &count2) {
    bool add_to_count1 = false;
    bool add_to_count2 = false;
    for (uint64_t i = 0; i < (1UL << (2 * (nums.size() - 1))); i++) {
        uint64_t sum = nums[0];
        bool used_funky = false;
        bool bogus = false;
        for (size_t j = 1; j < nums.size(); j++) {
            switch ((i >> (2 * (j - 1))) & 0b11UL) {
            case 0: {
                uint64_t res = sum * nums[j];
                if (nums[j] != 0 && res / nums[j] != sum) {
                    // overflow
                    bogus = true;
                    break;
                }
                sum = res;
                break;
            }
            case 1: {
                uint64_t res = sum + nums[j];
                if (res < sum) {
                    // overflow
                    bogus = true;
                    break;
                    break;
                }
                sum = res;
                break;
            }
            case 2: {
                used_funky = true;
                std::optional<uint64_t> res = concat(sum, nums[j]);
                if (!res.has_value()) {
                    bogus = true;
                    break;
                }

                sum = res.value();
                break;
            }
            default:
                bogus = true;
                break;
            }
            if (bogus) {
                break;
            }
        }
        if (sum == goal && !bogus) {
            add_to_count2 = true;
            if (!used_funky) {
                add_to_count1 = true;
            }
            if (add_to_count1 && add_to_count2) {
                break;
            }
        }
    }
    if (add_to_count1) {
        count1 += goal;
    }
    if (add_to_count2) {
        count2 += goal;
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
