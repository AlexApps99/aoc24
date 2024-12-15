#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

static void print_warehouse(const std::vector<std::string> &warehouse_map) {
    for (const std::string &line : warehouse_map) {
        std::cerr << line << '\n';
    }
}

static std::vector<std::string>
widen_warehouse(const std::vector<std::string> &warehouse_map) {
    std::vector<std::string> new_warehouse_map{};
    for (const std::string &line : warehouse_map) {
        std::string new_line{};
        for (const char &c : line) {
            if (c == 'O') {
                new_line.append("[]");
            } else if (c == '@') {
                new_line.append("@.");
            } else {
                new_line.append(2, c);
            }
        }
        new_warehouse_map.emplace_back(std::move(new_line));
    }

    return new_warehouse_map;
}

static bool calc_move_offset(char move,
                             const std::vector<std::string> &warehouse_map,
                             uint64_t &x, uint64_t &y) {
    switch (move) {
    case '>':
        x += 1;
        if (x >= warehouse_map[0].size()) {
            return false;
        }
        break;
    case '<':
        if (x == 0) {
            return false;
        }
        x -= 1;
        break;
    case '^':
        if (y == 0) {
            return false;
        }
        y -= 1;
        break;
    case 'v':
        y += 1;
        if (y >= warehouse_map.size()) {
            return false;
        }
        break;
    }
    return true;
}

static bool try_move(std::vector<std::string> &warehouse_map, uint64_t x,
                     uint64_t y, char move) {
    // '[' and ']' should be moved as a single object
    bool is_wide = warehouse_map[y][x] == '[' || warehouse_map[y][x] == ']';

    // x always points to '['
    if (warehouse_map[y][x] == ']') {
        if (x == 0) {
            return false;
        } else {
            x -= 1;
        }
    }

    uint64_t new_x_1 = x;
    uint64_t new_y = y;
    if (!calc_move_offset(move, warehouse_map, new_x_1, new_y)) {
        return false;
    }
    // only used if is_wide
    uint64_t new_x_2 = new_x_1 + 1;
    if (is_wide && new_x_2 >= warehouse_map[0].size()) {
        return false;
    }

    if (is_wide && (move == '^' || move == 'v')) {
        char target_item_1 = warehouse_map[new_y][new_x_1];
        char target_item_2 = warehouse_map[new_y][new_x_2];

        bool success = target_item_1 == 'O' || target_item_1 == '[' ||
                       target_item_1 == ']' || target_item_1 == '.';
        success &= target_item_2 == 'O' || target_item_2 == '[' ||
                   target_item_2 == ']' || target_item_2 == '.';

        bool second_one_is_dupe = target_item_1 == '[' && target_item_2 == ']';
        if (success && target_item_1 != '.') {
            // we need to move the target item first
            success = try_move(warehouse_map, new_x_1, new_y, move);
        }
        if (success && !second_one_is_dupe && target_item_2 != '.') {
            // we need to move the target item first
            success = try_move(warehouse_map, new_x_2, new_y, move);
        }
        if (success) {
            // get the new item in this spot ('.')
            if (is_wide) {
                warehouse_map[y][x] = '.';
                warehouse_map[y][x + 1] = '.';
                warehouse_map[new_y][new_x_1] = '[';
                warehouse_map[new_y][new_x_2] = ']';
            } else {
                warehouse_map[new_y][new_x_1] = warehouse_map[y][x];
                warehouse_map[y][x] = '.';
            }
            return true;
        } else {
            return false;
        }
    } else {
        uint64_t target_x = new_x_1;
        if (is_wide && move == '>') {
            target_x = new_x_2;
        }
        char target_item = warehouse_map[new_y][target_x];

        bool success = target_item == 'O' || target_item == '[' ||
                       target_item == ']' || target_item == '.';
        if (success && target_item != '.') {
            // we need to move the target item first
            success = try_move(warehouse_map, target_x, new_y, move);
        }
        if (success) {
            // get the new item in this spot ('.')
            if (is_wide) {
                warehouse_map[y][x] = '.';
                warehouse_map[y][x + 1] = '.';
                warehouse_map[new_y][new_x_1] = '[';
                warehouse_map[new_y][new_x_2] = ']';
            } else {
                warehouse_map[new_y][new_x_1] = warehouse_map[y][x];
                warehouse_map[y][x] = '.';
            }
            return true;
        } else {
            return false;
        }
    }
}

static void do_move(std::vector<std::string> &warehouse_map, char move) {
    // move is one of '>' '<' '^' 'v'
    for (uint64_t i = 0; i < warehouse_map.size(); i++) {
        for (uint64_t j = 0; j < warehouse_map[i].size(); j++) {
            if (warehouse_map[i][j] == '@') {
                std::vector<std::string> old_warehouse_map = warehouse_map;
                if (!try_move(warehouse_map, j, i, move)) {
                    // revert any changes, the move failed
                    warehouse_map = old_warehouse_map;
                }
                return;
            }
        }
    }
}

static uint64_t calculate_gps(const std::vector<std::string> &warehouse_map) {
    uint64_t gps = 0;

    for (uint64_t i = 0; i < warehouse_map.size(); i++) {
        for (uint64_t j = 0; j < warehouse_map[i].size(); j++) {
            if (warehouse_map[i][j] == 'O' || warehouse_map[i][j] == '[') {
                gps += 100 * i + j;
            }
        }
    }

    return gps;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_15(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::vector<std::string> warehouse_map{};
    std::string input_line{};
    while (std::getline(in, input_line) && !input_line.empty()) {
        warehouse_map.push_back(input_line);
    }

    std::vector<std::string> wide_warehouse_map =
        widen_warehouse(warehouse_map);

    std::string moves{};
    while (std::getline(in, input_line)) {
        moves += input_line;
    }

    // std::cerr << "Initial state:\n";
    // print_warehouse(wide_warehouse_map);
    for (const char &c : moves) {
        // do something
        // std::cerr << "\nMove " << c << ":\n";
        do_move(warehouse_map, c);
        do_move(wide_warehouse_map, c);
        // print_warehouse(wide_warehouse_map);
    }

    uint64_t count1 = calculate_gps(warehouse_map);

    uint64_t count2 = calculate_gps(wide_warehouse_map);

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
