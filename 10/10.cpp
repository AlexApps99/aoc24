#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

void search_trailhead(const std::vector<std::string> &input, uint64_t x,
                      uint64_t y, uint64_t &count1, uint64_t &count2) {
    // count number of "9" positions reachable from starting position,
    // given that we can move horizontally/vertically to the number
    // above the current one (starting at 0)
    uint64_t distinct_paths_that_end_in_peak = 0;
    std::set<std::pair<uint64_t, uint64_t>> peaks_set;
    std::vector<std::pair<uint64_t, uint64_t>> stack;
    stack.push_back(std::make_pair(x, y));
    while (!stack.empty()) {
        auto [x, y] = stack.back();
        stack.pop_back();

        if (input[y][x] == '9') {
            distinct_paths_that_end_in_peak++;
            peaks_set.insert(std::make_pair(x, y));
        }

        if (x > 0 && input[y][x - 1] == input[y][x] + 1) {
            stack.push_back(std::make_pair(x - 1, y));
        }
        if (x < input[y].size() - 1 && input[y][x + 1] == input[y][x] + 1) {
            stack.push_back(std::make_pair(x + 1, y));
        }
        if (y > 0 && input[y - 1][x] == input[y][x] + 1) {
            stack.push_back(std::make_pair(x, y - 1));
        }
        if (y < input.size() - 1 && input[y + 1][x] == input[y][x] + 1) {
            stack.push_back(std::make_pair(x, y + 1));
        }
    }

    count1 += peaks_set.size();
    count2 += distinct_paths_that_end_in_peak;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_10(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::vector<std::string> input;
    std::string input_line;
    while (std::getline(in, input_line)) {
        input.push_back(input_line);
    }

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    for (uint64_t y = 0; y < input.size(); y++) {
        for (uint64_t x = 0; x < input[y].size(); x++) {
            if (input[y][x] == '0') {
                search_trailhead(input, x, y, count1, count2);
            }
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
