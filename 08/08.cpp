#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using coord = std::pair<int64_t, int64_t>;

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_08(std::istream &in, std::string &out1, std::string &out2) {
    uint64_t count1 = 0;
    uint64_t count2 = 0;

    std::vector<std::string> lines;
    std::vector<std::vector<int64_t>> grid;

    std::unordered_map<char, std::vector<coord>> map;

    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
        grid.emplace_back(line.size(), INT64_MAX);
    }

    if (lines.empty()) {
        return;
    }

    for (int64_t y = 0; y < lines.size(); y++) {
        for (int64_t x = 0; x < lines[y].size(); x++) {
            char c = lines[y][x];
            if (c != '.') {
                // init vec
                if (map.find(c) == map.end()) {
                    map[c] = {};
                }
                map[c].push_back({x, y});
            }
        }
    }

    for (auto const &[key, pairs] : map) {
        for (int i = 0; i < pairs.size(); i++) {
            int64_t ax = pairs[i].first;
            int64_t ay = pairs[i].second;
            for (int64_t j = 0; j < pairs.size(); j++) {
                int64_t bx = pairs[j].first;
                int64_t by = pairs[j].second;

                int64_t dx = ax - bx;
                int64_t dy = ay - by;
                // not really scalable, but works for this size of input :)
                for (int64_t m = -100; m < 100; m++) {
                    int64_t newx = ax + m * dx;
                    int64_t newy = ay + m * dy;
                    if (newx >= 0 && newx < grid[0].size() && newy >= 0 &&
                        newy < grid.size()) {
                        if (grid[newy][newx] != 1) {
                            grid[newy][newx] = m;
                        }
                    }
                }
            }
        }
    }

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == 1) {
                count1++;
            }
            if (grid[y][x] != INT64_MAX) {
                count2++;
            }
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
