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

static constexpr uint64_t SPACE_WIDTH = 101;
static constexpr uint64_t SPACE_HEIGHT = 103;

static inline int64_t wrap_add(int64_t a, int64_t b, int64_t mod) {
    return (a + b + mod) % mod;
}

struct Robot {
    int64_t x;
    int64_t y;
    int64_t vx;
    int64_t vy;

    void step(std::vector<std::vector<uint16_t>> &num_robots) {
        num_robots[y][x]--;
        x = wrap_add(x, vx, SPACE_WIDTH);
        y = wrap_add(y, vy, SPACE_HEIGHT);
        num_robots[y][x]++;
    }

    static std::optional<Robot> from_string(const std::string &s) {
        // Example: "p=0,4 v=3,-3"

        size_t p_pos = s.find("p=");
        size_t p_comma_pos = s.find(",", p_pos);
        size_t v_pos = s.find("v=");
        size_t v_comma_pos = s.find(",", v_pos);

        if (p_pos == std::string::npos || p_comma_pos == std::string::npos ||
            v_pos == std::string::npos || v_comma_pos == std::string::npos) {
            return std::nullopt;
        }

        std::string x_str = s.substr(p_pos + 2, p_comma_pos - p_pos - 2);
        std::string y_str = s.substr(p_comma_pos + 1, v_pos - p_comma_pos - 2);
        std::string vx_str = s.substr(v_pos + 2, v_comma_pos - v_pos - 2);
        std::string vy_str = s.substr(v_comma_pos + 1);

        int64_t x = std::stoi(x_str);
        int64_t y = std::stoi(y_str);
        int64_t vx = std::stoi(vx_str);
        int64_t vy = std::stoi(vy_str);

        return Robot{x, y, vx, vy};
    }
};

static void print_robots(const std::vector<std::vector<uint16_t>> &num_robots) {
    for (uint64_t y = 0; y < SPACE_HEIGHT; y++) {
        for (uint64_t x = 0; x < SPACE_WIDTH; x++) {
            std::cerr << (num_robots[y][x] > 0 ? "#" : " ");
        }
        std::cerr << '\n';
    }
}

static bool
looks_like_tree(const std::vector<std::vector<uint16_t>> &num_robots,
                const std::vector<Robot> &robots) {
    if (robots.size() < 20) {
        // nothing hidden in example
        return true;
    }
    // find more than 10 in a row
    uint64_t num_in_row = 0;
    for (uint64_t y = 0; y < SPACE_HEIGHT; y++) {
        for (uint64_t x = 0; x < SPACE_WIDTH; x++) {
            if (num_robots[y][x] > 0) {
                num_in_row++;
            } else {
                num_in_row = 0;
            }

            if (num_in_row > 10) {
                return true;
            }
        }
    }

    return false;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_14(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::string input_line;

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    std::string input;
    std::vector<Robot> robots_orig;
    std::vector<std::vector<uint16_t>> num_robots_orig;
    for (uint64_t y = 0; y < SPACE_HEIGHT; y++) {
        num_robots_orig.push_back(std::vector<uint16_t>(SPACE_WIDTH, 0));
    }
    while (std::getline(in, input_line)) {
        std::optional<Robot> robot = Robot::from_string(input_line);
        if (robot.has_value()) {
            robots_orig.push_back(robot.value());
            num_robots_orig[robot.value().y][robot.value().x]++;
        }
    }

    std::vector<Robot> robots = robots_orig;
    std::vector<std::vector<uint16_t>> num_robots = num_robots_orig;

    for (uint64_t t = 0; t < 100; t++) {
        for (auto &robot : robots) {
            robot.step(num_robots);
        }
    }

    uint64_t upper_left_count = 0;
    uint64_t lower_right_count = 0;
    uint64_t upper_right_count = 0;
    uint64_t lower_left_count = 0;

    for (auto &robot : robots) {
        num_robots[robot.y][robot.x]++;
        if (robot.x < SPACE_WIDTH / 2 && robot.y < SPACE_HEIGHT / 2) {
            upper_left_count++;
        }
        if (robot.x > SPACE_WIDTH / 2 && robot.y > SPACE_HEIGHT / 2) {
            lower_right_count++;
        }
        if (robot.x > SPACE_WIDTH / 2 && robot.y < SPACE_HEIGHT / 2) {
            upper_right_count++;
        }
        if (robot.x < SPACE_WIDTH / 2 && robot.y > SPACE_HEIGHT / 2) {
            lower_left_count++;
        }
    }

    count1 = upper_left_count * lower_right_count * upper_right_count *
             lower_left_count;

    uint64_t seconds_elapsed = 0;
    while (!looks_like_tree(num_robots_orig, robots_orig)) {
        for (auto &robot : robots_orig) {
            robot.step(num_robots_orig);
        }
        seconds_elapsed++;
    }

    count2 = seconds_elapsed;

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
