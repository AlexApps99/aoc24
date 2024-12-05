#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void dfs_visit(std::array<bool, 256> &visited,
               std::array<bool, 256> &in_da_list,
               const std::array<bool, 256> &should_be_included,
               const std::array<std::vector<uint8_t>, 256> &adjacency,
               std::vector<uint8_t> &seq, int n) {
    if (in_da_list[n] || !should_be_included[n]) {
        return;
    }
    if (visited[n]) {
        std::cerr << "bruh\n";
        return;
    }
    visited[n] = true;
    for (const uint8_t m : adjacency[n]) {
        if (should_be_included[m]) {
            dfs_visit(visited, in_da_list, should_be_included, adjacency, seq,
                      m);
        }
    }
    in_da_list[n] = true;
    seq.push_back(n);
}

// edges: pairs of edges a -> b
// adjacency: adjacency list
// rev_adjacency: reverse adjacency list
// seq: sequence of nodes
// mapping: mapping from node ID to index in seq
void reorder(const std::array<std::vector<uint8_t>, 256> &adjacency,
             std::vector<uint8_t> &seq) {

    std::array<bool, 256> should_be_included{};
    for (const uint8_t node : seq) {
        should_be_included[node] = true;
    }

    seq.clear();

    // perform topological sort using DFS
    std::array<bool, 256> visited{};
    std::array<bool, 256> in_da_list{};

    for (uint16_t node = 0; node < 256; node++) {
        dfs_visit(visited, in_da_list, should_be_included, adjacency, seq,
                  node);
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_05(std::istream &in, std::string &out1, std::string &out2) {
    std::array<std::vector<uint8_t>, 256> adjacency{};
    std::array<std::vector<uint8_t>, 256> rev_adjacency{};
    std::vector<std::array<uint8_t, 2>> edges{};

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            // orderings done
            break;
        }

        size_t pos = line.find('|');
        if (pos == std::string::npos) {
            // bad syntax
            return;
        }
        uint64_t a = std::strtoul(line.substr(0, pos).c_str(), nullptr, 10);
        uint64_t b = std::strtoul(
            line.substr(pos + 1, line.size() - pos - 1).c_str(), nullptr, 10);
        if (a > 255 || b > 255) {
            // bad syntax
            return;
        }

        adjacency[a].push_back(b);
        rev_adjacency[b].push_back(a);
        std::array<uint8_t, 2> this_pair{static_cast<uint8_t>(a),
                                         static_cast<uint8_t>(b)};
        edges.push_back(this_pair);
    }

    uint32_t count1 = 0;
    uint32_t count2 = 0;
    while (std::getline(in, line)) {
        std::vector<uint8_t> seq{};
        std::stringstream ss{line};

        std::string num_str;
        std::array<uint16_t, 256> mapping{};
        mapping.fill(UINT16_MAX);
        while (std::getline(ss, num_str, ',')) {
            uint64_t num = std::strtoul(num_str.c_str(), nullptr, 10);
            if (num > 255) {
                return;
            }
            // num to idx
            mapping[num] = seq.size();
            seq.push_back(num);
        }

        bool is_valid = true;
        for (const std::array<uint8_t, 2> edge : edges) {
            if (mapping[edge[0]] >= mapping[edge[1]] &&
                mapping[edge[0]] < 256 && mapping[edge[1]] < 256) {
                is_valid = false;
                break;
            }
        }

        if (is_valid) {
            uint8_t mid_val = seq[seq.size() / 2];
            count1 += mid_val;
        } else {
            reorder(adjacency, seq);
            uint8_t mid_val = seq[seq.size() / 2];
            count2 += mid_val;
        }

        // check
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
