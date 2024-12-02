#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

void aoc_01(std::istream &in, std::string &out1, std::string &out2) {
    // generate lists for each input,
    // then sort em
    std::vector<uint32_t> list_a{};
    std::vector<uint32_t> list_b{};

    // parse input by line
    std::string line;
    while (std::getline(in, line)) {
        // find position of whitespace
        const std::size_t whitespace_pos = line.find(' ');

        // split by whitespace
        const uint64_t a =
            std::strtoul(line.substr(0, whitespace_pos).c_str(), nullptr, 10);
        const uint64_t b =
            std::strtoul(line.substr(whitespace_pos + 1).c_str(), nullptr, 10);

        // chuck onto lists
        // no need to do a narrowing check lmao :)
        list_a.push_back(static_cast<uint32_t>(a));
        list_b.push_back(static_cast<uint32_t>(b));
    }

    // sort lists
    std::sort(list_a.begin(), list_a.end());
    std::sort(list_b.begin(), list_b.end());

    // get difference of each pair in lists
    uint64_t total_dist = 0;

    for (size_t idx = 0; idx < list_a.size(); idx++) {
        // convert to signed ints
        const int32_t a = static_cast<int32_t>(list_a[idx]);
        const int32_t b = static_cast<int32_t>(list_b[idx]);
        // get the difference between the two values
        total_dist += std::abs(a - b);
    }

    // output the total distance
    out1 = std::to_string(total_dist);

    // part 2
    uint64_t sum = 0;

    size_t idx_left = 0;
    size_t idx_right = 0;

    // this counts RHS occurences of LHS values.
    // as the lists are sorted, we can just iterate through them
    // in similar vibes to merge sort's merge step
    while (idx_left < list_a.size()) {
        const uint64_t left_val = list_a[idx_left];
        uint64_t occurences = 0;
        // count all the occurences of our value in list b
        // (skipping values less than it)
        while (idx_right < list_b.size() && list_b[idx_right] <= left_val) {
            if (list_b[idx_right] == left_val) {
                occurences += 1;
            }
            idx_right += 1;
        }

        // count all occurences of our value in list a
        // (we know for a fact there will be nothing less than it)
        uint64_t occurences_of_left_val = 0;
        while (idx_left < list_a.size() && list_a[idx_left] == left_val) {
            idx_left += 1;
            occurences_of_left_val += 1;
        }

        // add to the sum
        sum += occurences * left_val * occurences_of_left_val;
    }

    out2 = std::to_string(sum);
}
