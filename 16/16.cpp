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

enum struct Dir : uint8_t {
    E = 0,
    N = 1,
    W = 2,
    S = 3,
};

static uint16_t get_rotate_cost(Dir from_dir, Dir to_dir) {
    uint8_t f = static_cast<uint8_t>(from_dir);
    uint8_t t = static_cast<uint8_t>(to_dir);
    uint8_t cw_distance = (t + 4 - f) % 4;
    uint8_t ccw_distance = (f + 4 - t) % 4;
    return 1000 * static_cast<uint16_t>(std::min(cw_distance, ccw_distance));
}

struct Pos {
    int16_t x;
    int16_t y;
    Dir dir;

    Pos(int16_t x, int16_t y, Dir dir) : x(x), y(y), dir(dir) {}

    Pos() : x(0), y(0), dir(Dir::E) {}

    void step() {
        switch (dir) {
        case Dir::E:
            x++;
            break;
        case Dir::N:
            y--;
            break;
        case Dir::W:
            x--;
            break;
        case Dir::S:
            y++;
            break;
        }
    }

    bool operator==(const Pos &other) const {
        return x == other.x && y == other.y && dir == other.dir;
    }
};

// hashing of Pos
namespace std {
template <> struct hash<Pos> {
    std::size_t operator()(const Pos &p) const {
        return std::hash<int16_t>{}(p.x) ^ std::hash<int16_t>{}(p.y) ^
               std::hash<uint8_t>{}(static_cast<uint8_t>(p.dir));
    }
};
} // namespace std

uint32_t heuristic(const Pos &p, const Pos &goal) {
    return std::abs(goal.x - p.x) + std::abs(goal.y - p.y);
}

static void a_star(const std::vector<std::string> &map, const Pos initial_pos,
                   Pos goal_pos, uint32_t &best_score,
                   uint32_t &num_tiles_on_best_paths) {
    // normalize goal pos as all are equivalent
    goal_pos.dir = Dir::E;

    constexpr const auto my_comp = [](const std::pair<Pos, uint32_t> &a,
                                      const std::pair<Pos, uint32_t> &b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<Pos, uint32_t>,
                        std::vector<std::pair<Pos, uint32_t>>,
                        decltype(my_comp)>
        open_set{my_comp};
    open_set.emplace(initial_pos, heuristic(initial_pos, goal_pos));

    std::unordered_map<Pos, std::vector<Pos>> came_from{};

    std::unordered_map<Pos, uint32_t> g_score{};
    g_score[initial_pos] = 0;

    best_score = UINT32_MAX;

    auto emplace_pos = [&](Pos current_pos, Pos new_pos,
                           uint32_t tentative_g_score) {
        if (new_pos.x >= 0 && new_pos.y >= 0 &&
            static_cast<size_t>(new_pos.y) < map.size() &&
            static_cast<size_t>(new_pos.x) < map[new_pos.y].size() &&
            map[new_pos.y][new_pos.x] != '#') {

            // normalize goal pos as all are equivalent
            if (new_pos.x == goal_pos.x && new_pos.y == goal_pos.y) {
                new_pos.dir = Dir::E;
            }

            bool new_pos_match = g_score.find(new_pos) != g_score.end();
            if (!new_pos_match || tentative_g_score <= g_score[new_pos]) {
                if (new_pos_match && tentative_g_score == g_score[new_pos]) {
                    // add to came_from
                    came_from[new_pos].push_back(current_pos);
                } else {
                    came_from[new_pos] = {current_pos};
                    // (originally below this else)
                    g_score[new_pos] = tentative_g_score;
                    // only add if not in open_set
                    open_set.emplace(new_pos, tentative_g_score +
                                                  heuristic(new_pos, goal_pos));
                }
            }
        }
    };

    while (!open_set.empty()) {
        Pos current = open_set.top().first;
        open_set.pop();

        if (current.x == goal_pos.x && current.y == goal_pos.y) {
            if (g_score[goal_pos] <= best_score) {
                best_score = g_score[goal_pos];
            } else {
                break;
            }
            continue;
        }

        // step forwards
        Pos new_pos = current;
        new_pos.step();

        emplace_pos(current, new_pos, g_score[current] + 1);

        // spin around
        for (uint8_t i = 0; i < 4; i++) {
            if (i != static_cast<uint8_t>(current.dir)) {
                Pos new_pos = current;
                new_pos.dir = static_cast<Dir>(i);

                uint32_t tentative_g_score =
                    g_score[current] +
                    get_rotate_cost(current.dir, new_pos.dir);

                emplace_pos(current, new_pos, tentative_g_score);
            }
        }
    }

    std::unordered_set<Pos> tiles_on_best_paths{};

    std::vector<Pos> stack{};
    stack.push_back(goal_pos);

    while (!stack.empty()) {
        Pos current = stack.back();
        stack.pop_back();

        Pos norm_p = current;
        norm_p.dir = Dir::E;
        tiles_on_best_paths.insert(norm_p);

        if (came_from.find(current) != came_from.end()) {
            for (const Pos &predecessor : came_from[current]) {
                stack.push_back(predecessor);
            }
        }
    }

    num_tiles_on_best_paths = tiles_on_best_paths.size();

    // for (int16_t y = 0; static_cast<size_t>(y) < map.size(); y++) {
    //     for (int16_t x = 0; static_cast<size_t>(x) < map[y].size();
    //     x++)
    //     {
    //         Pos norm_p{x, y, Dir::E};
    //         if (tiles_on_best_paths.find(norm_p) !=
    //         tiles_on_best_paths.end()) {
    //             std::cerr << 'O';
    //         } else {
    //             std::cerr << map[y][x];
    //         }
    //     }
    //     std::cerr << '\n';
    // }
    // std::cerr << num_tiles_on_best_paths << '\n';
}

static void search_for_best_score(const std::vector<std::string> &map,
                                  uint32_t &best_score,
                                  uint32_t &num_tiles_on_best_paths) {
    int16_t sx = -1;
    int16_t sy = -1;
    int16_t ex = -1;
    int16_t ey = -1;

    for (int16_t y = 0; static_cast<size_t>(y) < map.size(); y++) {
        for (int16_t x = 0; static_cast<size_t>(x) < map[y].size(); x++) {
            if (map[y][x] == 'S') {
                sx = x;
                sy = y;
            } else if (map[y][x] == 'E') {
                ex = x;
                ey = y;
            }
        }
    }

    return a_star(map, Pos(sx, sy, Dir::E), Pos(ex, ey, Dir::E), best_score,
                  num_tiles_on_best_paths);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_16(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::vector<std::string> map{};
    std::string input_line{};
    while (std::getline(in, input_line)) {
        map.push_back(input_line);
    }

    uint32_t count1 = UINT32_MAX;
    uint32_t count2 = 0;

    search_for_best_score(map, count1, count2);

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
