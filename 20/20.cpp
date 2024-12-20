#include <algorithm>
#include <cmath>
#include <cstdint>
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

static constexpr std::pair<int32_t, int32_t> directions[] = {
    {0, -1}, {0, 1}, {-1, 0}, {1, 0}};

static uint64_t calc_num_cheats(const std::vector<std::string> &lines,
                                const std::vector<std::vector<uint32_t>> &dist,
                                const int32_t num_picoseconds) {
    uint64_t count = 0;
    for (int32_t y = 0; y < lines.size(); y++) {
        for (int32_t x = 0; x < lines[y].size(); x++) {
            if (lines[y][x] != '#') {
                for (int64_t dy = -num_picoseconds; dy <= num_picoseconds;
                     dy++) {
                    for (int64_t dx = -num_picoseconds; dx <= num_picoseconds;
                         dx++) {
                        int64_t path_cost = std::abs(dx) + std::abs(dy);
                        if (path_cost > num_picoseconds || path_cost <= 0) {
                            continue;
                        }
                        int32_t new_x = x + dx;
                        int32_t new_y = y + dy;
                        if (new_y < 0 || new_y >= lines.size() || new_x < 0 ||
                            new_x >= lines[new_y].size()) {
                            continue;
                        }
                        if (lines[new_y][new_x] == '#') {
                            // wall
                            continue;
                        }
                        int32_t uncheated_dist =
                            static_cast<int32_t>(dist[y][x]) - path_cost;
                        int32_t time_saved =
                            uncheated_dist -
                            static_cast<int32_t>(dist[new_y][new_x]);
                        if (time_saved >= 100) {
                            count++;
                        }
                    }
                }
            }
        }
    }
    return count;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_20(std::istream &in, std::string &out1, std::string &out2) {
    std::vector<std::string> lines{};
    std::string input_line{};

    while (std::getline(in, input_line)) {
        lines.push_back(input_line);
    }

    if (lines.empty() || lines[0].empty()) {
        return;
    }

    uint32_t end_x = 0;
    uint32_t end_y = 0;

    for (uint32_t y = 0; y < lines.size(); y++) {
        for (uint32_t x = 0; x < lines[y].size(); x++) {
            if (lines[y][x] == 'E') {
                end_x = x;
                end_y = y;
            }
        }
    }

    std::vector<std::vector<uint32_t>> dist{
        lines.size(), std::vector<uint32_t>(lines[0].size(), UINT32_MAX)};

    std::queue<std::pair<uint32_t, uint32_t>> q{};
    q.push({end_x, end_y});
    dist[end_y][end_x] = 0;
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (const auto &[dx, dy] : directions) {
            int32_t new_x = static_cast<int32_t>(x) + dx;
            int32_t new_y = static_cast<int32_t>(y) + dy;
            if (new_y < 0 || new_y >= lines.size() || new_x < 0 ||
                new_x >= lines[new_y].size()) {
                continue;
            }
            if (lines[new_y][new_x] == '#') {
                // wall
                continue;
            }
            if (dist[new_y][new_x] != UINT32_MAX) {
                // visited
                continue;
            }
            dist[new_y][new_x] = dist[y][x] + 1;
            q.push({new_x, new_y});
        }
    }

    uint64_t count1 = calc_num_cheats(lines, dist, 2);
    uint64_t count2 = calc_num_cheats(lines, dist, 20);

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
