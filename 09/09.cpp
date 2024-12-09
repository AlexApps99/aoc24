#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Link {
    uint8_t size;
    const uint64_t id;
    Link *prev = nullptr;
    Link *next = nullptr;

    Link(uint8_t size, uint64_t id) : size(size), id(id) {}

    bool is_free_space() const { return id == UINT64_MAX; }
};

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_09(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::string input;
    std::getline(in, input);

    std::vector<Link> links;
    links.reserve(input.size() * 4);
    std::vector<uint64_t> blocks;
    uint64_t id = 0;
    for (uint64_t i = 0; i < input.size(); i++) {
        uint8_t size = input[i] - '0';
        bool is_free_space = (i % 2) != 0;
        uint64_t this_id = is_free_space ? UINT64_MAX : id++;
        for (uint8_t j = 0; j < size; j++) {
            blocks.push_back(this_id);
        }
        links.emplace_back(size, this_id);
        if (i > 0) {
            // std::cerr << '-' << '\n';
            links[i - 1].next = &links[i];
            links[i].prev = &links[i - 1];
        }
    }

    for (uint64_t i = blocks.size(); i-- > 0;) {
        if (blocks[i] == UINT64_MAX) {
            continue;
        }

        // find first sentinel instance in blocks
        for (uint64_t j = 0; j < i; j++) {
            if (blocks[j] == UINT64_MAX) {
                blocks[j] = blocks[i];
                blocks[i] = UINT64_MAX;
                break;
            }
        }
    }

    // this code is evil - what can I say except sorry
    for (uint64_t i = links.size(); i-- > 0;) {
        Link *moving_link = &links[i];

        if (moving_link->is_free_space()) {
            continue;
        }

        Link *link = &links[0];
        while (link) {
            if (link->id == moving_link->id) {
                break;
            }

            if (link->is_free_space() && link->size >= moving_link->size) {
                link->size -= moving_link->size;
                // make a new placeholder empty block
                const uint64_t placeholder_id = UINT64_MAX;
                links.emplace_back(moving_link->size, placeholder_id);
                Link *new_link = &links.back();
                // remove moving_link from list
                if (moving_link->next) {
                    moving_link->next->prev = new_link;
                    new_link->next = moving_link->next;
                }
                if (moving_link->prev) {
                    moving_link->prev->next = new_link;
                    new_link->prev = moving_link->prev;
                }
                // insert moving_link into list
                if (link->prev) {
                    link->prev->next = moving_link;
                }
                moving_link->prev = link->prev;
                moving_link->next = link;
                link->prev = moving_link;
                break;
            }
            link = link->next;
        }
    }
    uint64_t count1 = 0;
    uint64_t count2 = 0;

    Link *link = &links[0];
    uint64_t pos = 0;
    while (link) {
        if (link->is_free_space()) {
            for (uint64_t j = 0; j < link->size; j++) {
                std::cerr << ".";
            }
            pos += link->size;
        } else {
            for (uint64_t j = 0; j < link->size; j++) {
                std::cerr << link->id;
                count2 += pos * link->id;
                pos += 1;
            }
        }
        link = link->next;
    }
    std::cerr << '\n';

    for (uint64_t i = 0; i < blocks.size(); i++) {
        if (blocks[i] != UINT64_MAX) {
            count1 += i * blocks[i];
        }
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
