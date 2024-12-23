#include <algorithm>
#include <array>
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
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

static uint16_t to_id(const char s[3]) {
    return static_cast<uint16_t>(s[0]) << 8 | static_cast<uint16_t>(s[1]);
}

static std::pair<uint16_t, uint16_t> parse_input(const std::string &line) {
    if (line.size() != 5) {
        throw std::runtime_error("Invalid input line");
    }
    uint16_t a_value =
        static_cast<uint16_t>(line[0]) << 8 | (static_cast<uint16_t>(line[1]));
    uint16_t b_value =
        static_cast<uint16_t>(line[3]) << 8 | (static_cast<uint16_t>(line[4]));

    return {a_value, b_value};
}

static bool starts_with_t(uint16_t id) { return (id >> 8) == 't'; }

static void print_id(uint16_t id) {
    std::cerr << static_cast<char>(id >> 8) << static_cast<char>(id & 0xFF);
}

namespace std {
template <> struct hash<std::array<uint16_t, 3>> {
    std::size_t operator()(const std::array<uint16_t, 3> &a) const {
        return std::hash<uint64_t>{}(static_cast<uint64_t>(a[0]) |
                                     (static_cast<uint64_t>(a[1]) << 16) |
                                     (static_cast<uint64_t>(a[2]) << 32));
    }
};

} // namespace std

static uint64_t vecto64(const std::vector<uint16_t> &v) {
    // only works with 4 or less
    uint64_t result = 0;
    for (auto &id : v) {
        result <<= 16;
        result |= static_cast<uint64_t>(id);
    }
    return result;
}

std::array<uint16_t, 3> to_arr(std::vector<uint16_t> &v) {
    if (v.size() > 3) {
        throw std::runtime_error("Invalid size");
    }
    std::array<uint16_t, 3> arr{0, 0, 0};
    std::copy(v.begin(), v.end(), arr.begin());
    return arr;
}

void bron_kerbosch(std::set<uint16_t> &&r, std::set<uint16_t> &&p,
                   std::set<uint16_t> &&x,
                   const std::unordered_map<uint16_t, std::vector<uint16_t>> &g,
                   std::set<uint16_t> &maximum_set) {
    if (p.empty() && x.empty()) {
        if (r.size() > maximum_set.size()) {
            // copy r to maximum_set
            maximum_set.clear();
            for (auto &node : r) {
                maximum_set.insert(node);
            }
        }
    } else {
        std::set<uint16_t> p_copy = p;
        for (auto &node : p_copy) {
            std::set<uint16_t> new_r = r;
            new_r.insert(node);
            std::set<uint16_t> new_p;
            std::set<uint16_t> new_x;
            for (auto neighbour : g.at(node)) {
                if (p.find(neighbour) != p.end()) {
                    new_p.insert(neighbour);
                }
                if (x.find(neighbour) != x.end()) {
                    new_x.insert(neighbour);
                }
            }
            bron_kerbosch(std::move(new_r), std::move(new_p), std::move(new_x),
                          g, maximum_set);
            p.erase(node);
            x.insert(node);
        }
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_23(std::istream &in, std::string &out1, std::string &out2) {
    std::vector<std::string> lines{};
    std::string input_line{};
    std::unordered_set<uint16_t> nodes;
    std::unordered_map<uint16_t, std::vector<uint16_t>> g;

    while (std::getline(in, input_line)) {
        auto [a, b] = parse_input(input_line);
        g[a].push_back(b);
        g[b].push_back(a);
        nodes.insert(a);
        nodes.insert(b);
    }

    std::set<std::array<uint16_t, 3>> triples{};
    for (auto &node : nodes) {
        std::unordered_set<uint64_t> visited;
        std::vector<std::vector<uint16_t>> stack{};
        stack.push_back({node});

        while (!stack.empty()) {
            auto current = stack.back();
            auto current_node = current.back();
            stack.pop_back();

            if (visited.find(vecto64(current)) == visited.end()) {
                visited.insert(vecto64(current));
                if (current.size() <= 3) {
                    for (auto &neighbour : g[current_node]) {
                        if (current.size() == 3 && neighbour == node) {
                            if (true ||
                                std::any_of(current.begin(), current.end(),
                                            starts_with_t)) {
                                std::array<uint16_t, 3> triple{
                                    current[0], current[1], current[2]};
                                std::sort(triple.begin(), triple.end());
                                triples.insert(triple);
                            }
                        } else if (current.size() < 3) {
                            auto new_current = current;
                            new_current.push_back(neighbour);
                            stack.push_back(new_current);
                        }
                    }
                }
            }
        }
    }

    uint64_t count1 = 0;

    for (auto &triple : triples) {
        if (std::any_of(triple.begin(), triple.end(), starts_with_t)) {
            count1++;
        }
    }

    std::set<uint16_t> p_nodes;
    for (auto &node : nodes) {
        p_nodes.insert(node);
    }

    std::set<uint16_t> big_chugus{};
    bron_kerbosch({}, std::move(p_nodes), {}, g, big_chugus);

    out2 = "";
    for (auto &node : big_chugus) {
        out2.push_back(node >> 8);
        out2.push_back(node & 0xFF);
        out2.push_back(',');
    }
    if (!out2.empty()) {
        out2.pop_back();
    }

    out1 = std::to_string(count1);
}
