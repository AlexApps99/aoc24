#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void aoc_01(std::istream &in, std::ostream &out1, std::ostream &out2) {
    // generate heaps for each input,
    // then sort em
    std::vector<unsigned long> heap_a{};
    std::vector<unsigned long> heap_b{};

    // parse input by line
    std::string line;
    while (std::getline(in, line)) {
        // find position of whitespace
        std::size_t whitespace_pos = line.find(" ");

        // split by whitespace
        unsigned long a =
            std::strtoul(line.substr(0, whitespace_pos).c_str(), nullptr, 10);
        unsigned long b =
            std::strtoul(line.substr(whitespace_pos + 1).c_str(), nullptr, 10);

        // chuck onto heaps
        heap_a.push_back(a);
        std::push_heap(heap_a.begin(), heap_a.end());

        heap_b.push_back(b);
        std::push_heap(heap_b.begin(), heap_b.end());
    }

    // get difference of each pair in heaps
    unsigned long total_dist = 0;

    unsigned int idx = heap_a.size();
    while (idx > 0) {
        // pop the max value from each heap and stick it at the end
        // (when done, the heaps will be sorted in ascending order)
        std::pop_heap(heap_a.begin(), heap_a.begin() + idx);
        std::pop_heap(heap_b.begin(), heap_b.begin() + idx);

        idx -= 1;

        // get the difference between the two values
        unsigned long a = heap_a[idx];
        unsigned long b = heap_b[idx];

        total_dist += std::abs((long)a - (long)b);
    }

    // output the total distance
    out1 << total_dist << std::endl;

    // part 2
    unsigned long sum = 0;

    unsigned int idx_left = 0;
    unsigned int idx_right = 0;

    // this counts RHS occurences of LHS values.
    // as the heaps are sorted, we can just iterate through them
    // in similar vibes to merge sort's merge step
    while (idx_left < heap_a.size()) {
        unsigned long left_val = heap_a[idx_left];
        unsigned long occurences = 0;
        while (idx_right < heap_b.size() && heap_b[idx_right] <= left_val) {
            if (heap_b[idx_right] == left_val) {
                occurences += 1;
            }
            idx_right += 1;
        }

        unsigned long occurences_of_left_val = 0;
        while (idx_left < heap_a.size() && heap_a[idx_left] == left_val) {
            idx_left += 1;
            occurences_of_left_val += 1;
        }

        sum += occurences * left_val * occurences_of_left_val;
    }

    out2 << sum << std::endl;
}
