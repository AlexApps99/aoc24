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

static std::optional<uint8_t> get_level(char c) {
    switch (c) {
    case 'w':
        return 0;
    case 'u':
        return 1;
    case 'b':
        return 2;
    case 'r':
        return 3;
    case 'g':
        return 4;
    default:
        return std::nullopt;
    }
}

struct Level;

struct Level {
    Level *levels[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    bool end = false;
};

struct LevelState {
    Level *level;
    uint32_t count;

    bool operator==(const LevelState &other) const {
        return level == other.level && count == other.count;
    }
};

namespace std {
template <> struct hash<LevelState> {
    size_t operator()(const LevelState &s) const {
        return hash<const Level *>{}(s.level) ^ (hash<uint32_t>{}(s.count));
    }
};
} // namespace std

// constructs a tree of levels from each character in the towels
static Level *
construct_levels_tree(const std::vector<std::string> &towels,
                      std::vector<std::unique_ptr<Level>> &level_buffer) {
    level_buffer.emplace_back(std::make_unique<Level>());
    Level *root = level_buffer.back().get();

    for (const auto &towel : towels) {
        Level *current = root;
        for (const auto &c : towel) {
            const auto level = get_level(c);
            if (!level) {
                break;
            }
            if (!current->levels[*level]) {
                level_buffer.emplace_back(std::make_unique<Level>());
                current->levels[*level] = level_buffer.back().get();
            }
            current = current->levels[*level];
        }
        current->end = true;
    }
    return root;
}

static uint64_t is_design_viable(const std::string &design, Level *const root) {
    // value contains all the states where the state was reached from
    std::unordered_map<LevelState, std::vector<LevelState>> visited{};
    // each index is a counter level (iter backwards for topo sort, it's a DAG)
    std::vector<std::vector<LevelState>> levelsOfState{design.size() + 1};
    std::vector<LevelState> stack{};
    LevelState rootState{root, 0};
    stack.emplace_back(rootState);
    levelsOfState[0].emplace_back(rootState);
    visited[rootState] = {};

    while (!stack.empty()) {
        const auto thisState = stack.back();
        const auto [current, count] = thisState;
        stack.pop_back();

        const auto level = get_level(design[count]);
        if (!level) {
            throw std::runtime_error("bad level");
        }
        const auto next = current->levels[*level];
        if (next) {
            LevelState deeper{next, count + 1};
            LevelState cycle{root, count + 1};
            bool visitedDeeper = visited.find(deeper) != visited.end();
            bool visitedCycle = visited.find(cycle) != visited.end();
            bool exceedsLength = count + 1 >= design.size();
            if (next->end) {
                visited[cycle].push_back(thisState);
                if (!visitedCycle) {
                    levelsOfState[count + 1].emplace_back(cycle);
                    if (!exceedsLength) {
                        stack.emplace_back(cycle);
                    }
                }
            }
            visited[deeper].push_back(thisState);
            if (!visitedDeeper && !exceedsLength) {
                levelsOfState[count + 1].emplace_back(deeper);
                stack.emplace_back(deeper);
            }
        }
    }

    LevelState finalState = {root, static_cast<uint32_t>(design.size())};

    std::unordered_map<LevelState, uint64_t> dp{};
    dp[finalState] = 1;
    for (int i = design.size(); i >= 0; i--) {
        for (const auto &state : levelsOfState[i]) {
            for (const auto &prev : visited[state]) {
                dp[prev] += dp[state];
            }
        }
    }

    // return levelsOfState.back().size();
    return dp[rootState];
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_19(std::istream &in, std::string &out1, std::string &out2) {
    std::string input_line{};

    std::vector<std::string> towels{};
    std::vector<std::string> designs{};

    if (!std::getline(in, input_line)) {
        return;
    }

    // split by commas
    std::istringstream iss(input_line);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        if (token.empty()) {
            continue;
        }
        if (token.back() == ',') {
            token.pop_back();
        }
        towels.push_back(token);
    }

    while (std::getline(in, input_line)) {
        if (!input_line.empty()) {
            designs.push_back(input_line);
        }
    }

    std::vector<std::unique_ptr<Level>> level_buffer{};

    Level *const root = construct_levels_tree(towels, level_buffer);

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    for (const std::string &design : designs) {
        const uint64_t count = is_design_viable(design, root);
        count2 += count;
        if (count) {
            count1++;
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
