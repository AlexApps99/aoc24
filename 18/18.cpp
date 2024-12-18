#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
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

static bool check_point(int32_t x, int32_t y, int32_t max_val,
                        const std::vector<std::vector<uint32_t>> &grid,
                        uint32_t threshold) {
    return x >= 0 && x < max_val && y >= 0 && y < max_val &&
           (grid[y][x] == 0 || grid[y][x] > threshold);
}

static uint32_t calc_dist(const std::vector<std::vector<uint32_t>> &grid,
                          std::vector<std::vector<uint32_t>> &dist,
                          std::vector<std::vector<uint32_t>> &dist_copy,
                          uint32_t threshold) {
    int32_t max_val = grid.size();

    for (int32_t y = 0; y < max_val; y++) {
        std::fill(dist[y].begin(), dist[y].end(), UINT32_MAX);
        std::fill(dist_copy[y].begin(), dist_copy[y].end(), UINT32_MAX);
    }
    dist[0][0] = 0;
    dist_copy[0][0] = 0;

    for (int32_t i = 0; i < max_val * max_val; i++) {
        for (int32_t y1 = 0; y1 < max_val; y1++) {
            for (int32_t x1 = 0; x1 < max_val; x1++) {
                for (auto &[dx, dy] : directions) {
                    int32_t x2 = x1 + dx;
                    int32_t y2 = y1 + dy;
                    if (check_point(x2, y2, grid.size(), grid, threshold) &&
                        check_point(x1, y1, grid.size(), grid, threshold)) {
                        if (dist[y1][x1] < UINT32_MAX) {
                            uint32_t new_dist = dist[y1][x1] + 1;
                            if (new_dist < dist[y2][x2]) {
                                dist[y2][x2] = new_dist;
                            }
                        }
                    }
                }
            }
        }
        if (dist == dist_copy) {
            break;
        }
        for (int32_t y = 0; y < max_val; y++) {
            std::copy(dist[y].begin(), dist[y].end(), dist_copy[y].begin());
        }
    }

    return dist[max_val - 1][max_val - 1];
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_18(std::istream &in, std::string &out1, std::string &out2) {
    std::string input_line{};

    std::vector<std::pair<uint8_t, uint8_t>> points{};
    while (std::getline(in, input_line)) {
        size_t pos = input_line.find(',');
        if (pos == std::string::npos) {
            continue;
        }
        uint64_t x = std::stoull(input_line.substr(0, pos));
        uint64_t y = std::stoull(input_line.substr(pos + 1));
        if (x > 70 || y > 70) {
            continue;
        }
        points.emplace_back(x, y);
    }

    uint8_t max_val = 0;
    for (const auto &[x, y] : points) {
        max_val = std::max(max_val, x);
        max_val = std::max(max_val, y);
    }

    max_val += 1;

    std::vector<std::vector<uint32_t>> grid(max_val,
                                            std::vector<uint32_t>(max_val, 0));

    for (uint32_t i = 0; i < points.size(); i++) {
        auto &[x, y] = points[i];
        if (grid[y][x] == 0) {
            grid[y][x] = i + 1;
        }
    }

    std::vector<std::vector<uint32_t>> dist(
        grid.size(), std::vector<uint32_t>(grid.size(), UINT32_MAX));
    auto dist_copy = dist;

    uint32_t part_1_threshold = (grid.size() == 7) ? 12 : 1024;
    uint32_t count1 = calc_dist(grid, dist, dist_copy, part_1_threshold);
    out1 = std::to_string(count1);

    std::vector<uint32_t> counts(points.size(), 0);
    for (uint32_t i = 0; i < points.size(); i++) {
        counts[i] = i;
    }

    uint32_t count2 = UINT32_MAX;
    uint32_t lower = 0;
    uint32_t upper = points.size();

    while (lower <= upper) {
        uint32_t mid = lower + (upper - lower) / 2;
        if (calc_dist(grid, dist, dist_copy, mid) == UINT32_MAX) {
            count2 = mid;
            upper = mid - 1;
        } else {
            lower = mid + 1;
        }
    }

    if (count2 > 0 && (count2 - 1) < points.size()) {
        out2 = std::to_string(points[count2 - 1].first) + "," +
               std::to_string(points[count2 - 1].second);
    }
}
