#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static uint32_t count_in_line(const std::vector<std::string> &puzzle,
                              const std::array<int8_t, 2> dir,
                              const std::array<size_t, 2> start_pos) {
    const std::string keyword{"XMAS"};
    size_t matched_letters = 0;
    uint32_t matches = 0;

    int64_t x = start_pos[0];
    int64_t y = start_pos[1];

    int8_t dx = dir[0];
    int8_t dy = dir[1];

    while (y >= 0 && y < static_cast<int64_t>(puzzle.size()) && x >= 0 &&
           x < static_cast<int64_t>(puzzle[0].size())) {
        if (puzzle[y][x] == keyword[matched_letters]) {
            matched_letters++;
            if (matched_letters >= keyword.size()) {
                matches++;
                matched_letters = 0;
            }

            x += dx;
            y += dy;
        } else if (matched_letters != 0) {
            matched_letters = 0;
        } else {
            x += dx;
            y += dy;
        }
    }

    return matches;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_04(std::istream &in, std::string &out1, std::string &out2) {
    std::vector<std::string> puzzle;

    uint32_t count1 = 0;

    const std::vector<std::array<int8_t, 2>> dirs{
        {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    std::string line;
    while (std::getline(in, line)) {
        puzzle.push_back(line);
    }

    if (puzzle.empty()) {
        out1 = "0";
        out2 = "0";
        return;
    }

    // go through each direction to search in, and add them up

    for (const std::array<int8_t, 2> dir : dirs) {
        bool go_along_all_x = false;
        size_t fixed_y_for_all_x = 0;

        bool go_along_all_y = false;
        size_t fixed_x_for_all_y = 0;

        if (dir[0] < 0) {
            go_along_all_y = true;
            fixed_x_for_all_y = puzzle[0].size() - 1;
        } else if (dir[0] > 0) {
            go_along_all_y = true;
            fixed_x_for_all_y = 0;
        }

        if (dir[1] < 0) {
            go_along_all_x = true;
            fixed_y_for_all_x = puzzle.size() - 1;
        } else if (dir[1] > 0) {
            go_along_all_x = true;
            fixed_y_for_all_x = 0;
        }

        bool avoid_double_count = go_along_all_x && go_along_all_y;

        if (go_along_all_x) {
            for (size_t ax = 0; ax < puzzle[0].size(); ax++) {
                count1 += count_in_line(puzzle, dir, {ax, fixed_y_for_all_x});
            }
        }

        if (go_along_all_y) {
            for (size_t ay = 0; ay < puzzle.size(); ay++) {
                if (!avoid_double_count || ay != fixed_y_for_all_x) {
                    count1 +=
                        count_in_line(puzzle, dir, {fixed_x_for_all_y, ay});
                }
            }
        }
    }

    uint32_t count2 = 0;

    for (int64_t y = 1; y < static_cast<int64_t>(puzzle.size() - 1); y++) {
        for (int64_t x = 1; x < static_cast<int64_t>(puzzle.size() - 1); x++) {
            if (puzzle[y][x] != 'A') {
                continue;
            }

            bool valid = true;
            for (int8_t diag : {1, -1}) {
                char a = puzzle[y + diag][x + 1];
                char b = puzzle[y - diag][x - 1];
                valid &= ((a == 'M' && b == 'S') || (a == 'S' && b == 'M'));
            }
            if (!valid) {
                continue;
            }

            if (valid) {
                count2++;
            }
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
