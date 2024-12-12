#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Region {
    int8_t tile_char;
    uint64_t area = 0;
    uint64_t perimeter = 0;
    uint64_t edge_count = 0;

    Region(int8_t tile_char) : tile_char(tile_char) {}
};

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_12(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::vector<std::string> input;
    std::vector<std::vector<bool>> visited;
    std::string input_line;
    while (std::getline(in, input_line)) {
        input.push_back(input_line);
        visited.emplace_back(input_line.size(), false);
    }

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    for (int64_t y = 0; y < static_cast<int64_t>(input.size()); y++) {
        for (int64_t x = 0; x < static_cast<int64_t>(input[y].size()); x++) {
            if (!visited[y][x]) {
                std::vector<std::vector<uint8_t>> edge_map{};
                for (int64_t i = 0; i < static_cast<int64_t>(input.size());
                     i++) {
                    edge_map.emplace_back(input[i].size(), 0);
                }

                Region region(input[y][x]);
                std::vector<std::pair<int64_t, int64_t>> stack;
                stack.push_back({x, y});
                while (!stack.empty()) {
                    auto [x, y] = stack.back();
                    stack.pop_back();
                    if (y >= 0 && y < static_cast<int64_t>(input.size()) &&
                        x >= 0 && x < static_cast<int64_t>(input[y].size()) &&
                        !visited[y][x] && input[y][x] == region.tile_char) {
                        visited[y][x] = true;
                        region.area++;

                        // right perimeter
                        if (x + 1 >= static_cast<int64_t>(input[y].size()) ||
                            input[y][x + 1] != region.tile_char) {
                            edge_map[y][x] |= 0b0001;
                            region.perimeter++;
                        }
                        // left perimeter
                        if (x - 1 < 0 || input[y][x - 1] != region.tile_char) {
                            edge_map[y][x] |= 0b0010;
                            region.perimeter++;
                        }
                        // bottom perimeter
                        if (y + 1 >= static_cast<int64_t>(input.size()) ||
                            input[y + 1][x] != region.tile_char) {
                            edge_map[y][x] |= 0b0100;
                            region.perimeter++;
                        }
                        // top perimeter
                        if (y - 1 < 0 || input[y - 1][x] != region.tile_char) {
                            edge_map[y][x] |= 0b1000;
                            region.perimeter++;
                        }

                        stack.push_back({x + 1, y});
                        stack.push_back({x - 1, y});
                        stack.push_back({x, y + 1});
                        stack.push_back({x, y - 1});
                    }
                }

                // count horiz edges, where contiguous edges are only counted
                // once
                for (int64_t i = 0; i < static_cast<int64_t>(edge_map.size());
                     i++) {
                    bool following_bottom_edge = false;
                    bool following_top_edge = false;
                    for (int64_t j = 0;
                         j < static_cast<int64_t>(edge_map[i].size()); j++) {
                        bool has_bottom_edge = edge_map[i][j] & 0b0100;
                        bool has_top_edge = edge_map[i][j] & 0b1000;
                        if (has_bottom_edge) {
                            if (!following_bottom_edge) {
                                region.edge_count++;
                                following_bottom_edge = true;
                            }
                        } else {
                            following_bottom_edge = false;
                        }

                        if (has_top_edge) {
                            if (!following_top_edge) {
                                region.edge_count++;
                                following_top_edge = true;
                            }
                        } else {
                            following_top_edge = false;
                        }
                    }
                }

                // count vert edges, where contiguous edges are only counted
                // once
                for (int64_t i = 0;
                     i < static_cast<int64_t>(edge_map[0].size()); i++) {
                    bool following_right_edge = false;
                    bool following_left_edge = false;
                    for (int64_t j = 0;
                         j < static_cast<int64_t>(edge_map.size()); j++) {
                        bool has_right_edge = edge_map[j][i] & 0b0001;
                        bool has_left_edge = edge_map[j][i] & 0b0010;
                        if (has_right_edge) {
                            if (!following_right_edge) {
                                region.edge_count++;
                                following_right_edge = true;
                            }
                        } else {
                            following_right_edge = false;
                        }

                        if (has_left_edge) {
                            if (!following_left_edge) {
                                region.edge_count++;
                                following_left_edge = true;
                            }
                        } else {
                            following_left_edge = false;
                        }
                    }
                }

                count1 += region.area * region.perimeter;
                count2 += region.area * region.edge_count;
            }
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
