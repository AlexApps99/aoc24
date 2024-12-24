#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
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

using coord = std::pair<int32_t, int32_t>;

coord get_keypad_btn(const char c) {
    switch (c) {
    case '7':
        return {0, 0};
    case '8':
        return {1, 0};
    case '9':
        return {2, 0};
    case '4':
        return {0, 1};
    case '5':
        return {1, 1};
    case '6':
        return {2, 1};
    case '1':
        return {0, 2};
    case '2':
        return {1, 2};
    case '3':
        return {2, 2};
    case '0':
        return {1, 3};
    case 'A':
    default:
        return {2, 3};
    }
}

coord get_dirpad_btn(const char c) {
    switch (c) {
    case '^':
        return {1, 0};
    case 'A':
    default:
        return {2, 0};
    case '<':
        return {0, 1};
    case 'v':
        return {1, 1};
    case '>':
        return {2, 1};
    }
}

// we input a sequence into dirpad,
// which controls robot on dirpad,
// which controls robot on dirpad,
// which controls robot on keypad

uint64_t get_min_num_presses(const std::string &code) { return 0; }

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_21(std::istream &in, std::string &out1, std::string &out2) {
    std::vector<std::string> lines{};
    std::string input_line{};

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    std::vector<std::function<coord(const char)>> keypads{
        get_keypad_btn, get_dirpad_btn, get_dirpad_btn};

    while (std::getline(in, input_line)) {
        uint64_t min_num_presses = get_min_num_presses(input_line);
        uint64_t numeric_part =
            std::stoull(input_line.substr(0, input_line.size() - 1));
        if (numeric_part != UINT64_MAX) {
            count1 += numeric_part * min_num_presses;
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
