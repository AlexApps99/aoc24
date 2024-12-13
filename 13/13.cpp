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

struct Button {
    int64_t dx;
    int64_t dy;
};

static std::optional<Button> parse_button(const std::string &line) {
    // Example: "Button A: X+19, Y+50"
    std::string::size_type x_pos = line.find("X");
    std::string::size_type y_pos = line.find("Y");
    if (x_pos == std::string::npos || y_pos == std::string::npos) {
        return std::nullopt;
    }

    std::string dx_str = line.substr(x_pos + 1, y_pos - x_pos - 3);
    std::string dy_str = line.substr(y_pos + 1);
    int64_t dx = std::stoll(dx_str);
    int64_t dy = std::stoll(dy_str);

    return Button{dx, dy};
}

static int64_t min_presses_to_win_1(const Button a, const Button b,
                                    const int64_t x, const int64_t y) {
    // find i, j >= 0 such that
    // i * a.dx + j * b.dx == x,
    // i * a.dy + j * b.dy == y,

    // i * a.dx - x == -j * b.dx,
    // where i + j is minimized.
    int64_t i = -1;
    int64_t j = -1;
    for (int64_t possible_i = 0; possible_i < 100 && possible_i * a.dx <= x;
         possible_i++) {
        // TODO what if a.dx or b.dx is zero?
        int64_t possible_j = (x - possible_i * a.dx) / b.dx;
        if ((x - possible_i * a.dx) % b.dx == 0 && possible_j >= 0) {
            if (possible_i * a.dy + possible_j * b.dy != y) {
                continue;
            }
            if (i < 0 || j < 0 || possible_i + possible_j < i + j) {
                i = possible_i;
                j = possible_j;
                break;
            }
        }
    }

    if (i >= 0 && j >= 0) {
        return 3 * i + j;
    } else {
        return -1;
    }
}

static int64_t min_presses_to_win_2(const Button a, const Button b,
                                    const int64_t x, const int64_t y) {
    int64_t i_q = b.dy * x - b.dx * y;
    int64_t i_d = a.dx * b.dy - a.dy * b.dx;

    if (i_q % i_d != 0) {
        return -1;
    }

    int64_t j_q = -a.dy * x + a.dx * y;
    int64_t j_d = a.dx * b.dy - a.dy * b.dx;

    if (j_q % j_d != 0) {
        return -1;
    }

    int64_t i = i_q / i_d;
    int64_t j = j_q / j_d;

    if (i >= 0 && j >= 0) {
        return 3 * i + j;
    } else {
        return -1;
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_13(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::string input_line;

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    while (true) {
        if (!std::getline(in, input_line) || input_line.empty()) {
            break;
        }
        Button button_a = parse_button(input_line).value();
        if (!std::getline(in, input_line) || input_line.empty()) {
            break;
        }
        Button button_b = parse_button(input_line).value();
        if (!std::getline(in, input_line) || input_line.empty()) {
            break;
        }

        // Example: "Prize: X=12091, Y=18429"
        std::string::size_type x_pos = input_line.find("X=");
        std::string::size_type y_pos = input_line.find("Y=");
        if (x_pos == std::string::npos || y_pos == std::string::npos) {
            break;
        }

        std::string x_str = input_line.substr(x_pos + 2, y_pos - x_pos - 4);
        std::string y_str = input_line.substr(y_pos + 2);
        int64_t x = std::stoll(x_str);
        int64_t y = std::stoll(y_str);

        int64_t result1 = min_presses_to_win_2(button_a, button_b, x, y);
        if (result1 >= 0) {
            count1 += static_cast<uint64_t>(result1);
        }

        int64_t result2 = min_presses_to_win_2(
            button_a, button_b, 10000000000000 + x, 10000000000000 + y);
        if (result2 >= 0) {
            count2 += static_cast<uint64_t>(result2);
        }

        // ignore
        std::getline(in, input_line);
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
