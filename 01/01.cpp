#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

void aoc_01(std::istream &in, std::ostream &out1, std::ostream &out2) {
    // generate heaps for each input,
    // then sort em
    std::vector<uint64_t> heap_a{};
    std::vector<uint64_t> heap_b{};

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

        // chuck onto heaps
        heap_a.push_back(a);
        std::push_heap(heap_a.begin(), heap_a.end());

        heap_b.push_back(b);
        std::push_heap(heap_b.begin(), heap_b.end());
    }

    // get difference of each pair in heaps
    uint64_t total_dist = 0;

    size_t idx = heap_a.size();
    while (idx > 0) {
        // pop the max value from each heap and stick it at the end
        // (when done, the heaps will be sorted in ascending order)
        // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
        std::pop_heap(heap_a.begin(), heap_a.begin() + idx);
        std::pop_heap(heap_b.begin(), heap_b.begin() + idx);
        // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)

        idx -= 1;

        // get the difference between the two values
        const uint64_t a = heap_a[idx];
        const uint64_t b = heap_b[idx];

        total_dist +=
            std::abs(static_cast<int64_t>(a) - static_cast<int64_t>(b));
    }

    // output the total distance
    out1 << total_dist << '\n';

    // part 2
    uint64_t sum = 0;

    size_t idx_left = 0;
    size_t idx_right = 0;

    // this counts RHS occurences of LHS values.
    // as the heaps are sorted, we can just iterate through them
    // in similar vibes to merge sort's merge step
    while (idx_left < heap_a.size()) {
        const uint64_t left_val = heap_a[idx_left];
        uint64_t occurences = 0;
        while (idx_right < heap_b.size() && heap_b[idx_right] <= left_val) {
            if (heap_b[idx_right] == left_val) {
                occurences += 1;
            }
            idx_right += 1;
        }

        uint64_t occurences_of_left_val = 0;
        while (idx_left < heap_a.size() && heap_a[idx_left] == left_val) {
            idx_left += 1;
            occurences_of_left_val += 1;
        }

        sum += occurences * left_val * occurences_of_left_val;
    }

    out2 << sum << '\n';
}
