#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

enum Dir { R = 1 << 0, U = 1 << 1, L = 1 << 2, D = 1 << 3 };

std::optional<Dir> get_dir(const char c) {
    switch (c) {
    case '>':
        return Dir::R;
    case '^':
        return Dir::U;
    case '<':
        return Dir::L;
    case 'v':
        return Dir::D;
    default:
        return std::nullopt;
    }
}

void step(int32_t &x, int32_t &y, Dir dir) {
    if (dir == Dir::R) {
        x++;
    } else if (dir == Dir::U) {
        y--;
    } else if (dir == Dir::L) {
        x--;
    } else if (dir == Dir::D) {
        y++;
    }
}

Dir turn_right(Dir dir) {
    if (dir == Dir::R) {
        return Dir::D;
    } else if (dir == Dir::D) {
        return Dir::L;
    } else if (dir == Dir::L) {
        return Dir::U;
    } else {
        return Dir::R;
    }
}

// returns false if a loop was found
bool step_to_end(std::vector<std::string> puzzle, int guard_x, int guard_y,
                 Dir guard_dir, uint32_t &count1, uint32_t &count2,
                 bool is_main_traversal) {
    std::vector<std::vector<uint8_t>> visited;
    for (const auto &row : puzzle) {
        visited.push_back(std::vector<uint8_t>(row.size(), 0));
    }

    constexpr uint8_t obstacle_placed = 1 << 7;

    while (guard_y >= 0 && guard_y < puzzle.size() && guard_x >= 0 &&
           guard_x < puzzle[0].size()) {
        uint8_t visited_flags = visited[guard_y][guard_x] & 0x0F;
        if (visited_flags & guard_dir) {
            // this position has been visited from this direction
            // it's a loop
            return false;
        }
        // increment count if this spot hasn't been touched before
        if (is_main_traversal && visited_flags == 0) {
            count1++;
        }

        // mark this position as visited in this direction
        visited[guard_y][guard_x] |= guard_dir;

        int32_t new_x = guard_x;
        int32_t new_y = guard_y;
        // find out where we would step
        step(new_x, new_y, guard_dir);

        if (new_y >= 0 && new_y < puzzle.size() && new_x >= 0 &&
            new_x < puzzle[0].size() && puzzle[new_y][new_x] == '#') {
            // obstacle ahead, turn right
            guard_dir = turn_right(guard_dir);
        } else {
            // step forward
            if (is_main_traversal && new_y >= 0 && new_y < puzzle.size() &&
                new_x >= 0 && new_x < puzzle[0].size() &&
                !(visited[new_y][new_x] & obstacle_placed)) {
                // for hypothetical, what if we put an obstacle here?
                std::vector<std::string> puzzle_copy = puzzle;
                puzzle_copy[new_y][new_x] = '#';
                visited[new_y][new_x] |= obstacle_placed;
                if (!step_to_end(puzzle_copy, guard_x, guard_y, guard_dir,
                                 count1, count2, false)) {
                    std::cerr << new_x << " " << new_y << '\n';
                    // loop detected
                    count2++;
                }
            }

            guard_x = new_x;
            guard_y = new_y;
        }
    }
    return true;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_06(std::istream &in, std::string &out1, std::string &out2) {
    std::vector<std::string> puzzle;

    uint32_t count1 = 0;
    uint32_t count2 = 0;

    std::string line;
    while (std::getline(in, line)) {
        puzzle.push_back(line);
    }

    if (puzzle.empty()) {
        return;
    }

    int32_t guard_x = 0;
    int32_t guard_y = 0;
    Dir guard_dir = Dir::R;

    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[i].size(); j++) {
            std::optional<Dir> dir = get_dir(puzzle[i][j]);
            if (dir.has_value()) {
                guard_x = j;
                guard_y = i;
                guard_dir = dir.value();
                puzzle[i][j] = '.';
            }
        }
    }

    bool no_loop_found =
        step_to_end(puzzle, guard_x, guard_y, guard_dir, count1, count2, true);
    if (!no_loop_found) {
        std::cerr << ":(\n";
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
