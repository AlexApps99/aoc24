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

void exec_emu(uint64_t ra, uint64_t rb, uint64_t rc, const std::string &data,
              std::string &output) {
    output.clear();

    uint32_t pc = 0;
    while (pc + 1 < data.size()) {
        uint8_t op_c = data[pc];
        if (op_c < '0' || op_c > '7') {
            throw std::runtime_error("bad op");
        }
        uint8_t lit_c = data[pc + 1];
        uint8_t lit = lit_c - '0';
        if (lit > 7) {
            throw std::runtime_error("bad lit");
        }
        uint64_t arg = lit;
        if (arg == 4) {
            arg = ra;
        } else if (arg == 5) {
            arg = rb;
        } else if (arg == 6) {
            arg = rc;
        } else if (arg >= 7) {
            // bad, don't use
        }

        uint32_t next_pc = pc + 2;

        switch (op_c) {
        case '0':
            ra = ra >> arg;
            break;
        case '1':
            rb ^= lit;
            break;
        case '2':
            rb = (arg % 8);
            break;
        case '3':
            if (ra != 0) {
                next_pc = lit;
            }
            break;
        case '4':
            rb ^= rc;
            break;
        case '5':
            output.push_back(static_cast<uint8_t>(arg % 8) + '0');
            break;
        case '6':
            rb = ra >> arg;
            break;
        case '7':
            rc = ra >> arg;
            break;
        }

        pc = next_pc;
    }
}

void exec_emu_hardcoded_p2(uint64_t ra, const std::string &data,
                           std::string &output) {
    output.clear();
    do {
        // get inverted lowest nibble in A
        uint8_t prb = ra & 0x7;
        // uninvert lowest nibble, but XOR with rc
        prb ^= static_cast<uint8_t>(ra >> (prb ^ 7));
        uint8_t o = prb & 0x7;
        ra >>= 3;

        // nib(n) = low nib of n
        // ~ inverts nibble
        // after a loop iter:
        // o = nib(ra) ^ nib(ra >> ~nib(ra))
        // ra >>= 3
        output.push_back(o + '0');
    } while (ra);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_17(std::istream &in, std::string &out1, std::string &out2) {
    // grab input
    std::vector<std::string> map{};
    std::string input_line{};

    if (!std::getline(in, input_line)) {
        return;
    }
    uint64_t reg_a_offset = input_line.find("Register A: ");
    if (reg_a_offset == std::string::npos) {
        return;
    }
    uint64_t ra = std::stoull(input_line.substr(reg_a_offset + 12));

    if (!std::getline(in, input_line)) {
        return;
    }
    uint64_t reg_b_offset = input_line.find("Register B: ");
    if (reg_b_offset == std::string::npos) {
        return;
    }
    uint64_t rb = std::stoull(input_line.substr(reg_b_offset + 12));

    if (!std::getline(in, input_line)) {
        return;
    }
    uint64_t reg_c_offset = input_line.find("Register C: ");
    if (reg_c_offset == std::string::npos) {
        return;
    }
    uint64_t rc = std::stoull(input_line.substr(reg_c_offset + 12));

    if (!std::getline(in, input_line)) {
        return;
    }

    if (!std::getline(in, input_line)) {
        return;
    }
    uint64_t program_offset = input_line.find("Program: ");
    if (program_offset == std::string::npos) {
        return;
    }

    // remove commas
    std::string data = input_line.substr(program_offset + 9);
    data.erase(std::remove(data.begin(), data.end(), ','), data.end());

    // part 1
    std::string output{};
    exec_emu(ra, rb, rc, data, output);

    // add commas
    out1.clear();
    for (char c : output) {
        out1.push_back(c);
        out1.push_back(',');
    }
    if (!out1.empty()) {
        out1.pop_back();
    }

    uint64_t data_as_num = 0;
    for (char c : data) {
        data_as_num <<= 3;
        data_as_num |= static_cast<uint64_t>(c - '0');
    }

    // part 2 (TOOOO SLOOOOW)
    if (data.size() == 16) {
        std::string output{};
        ra = 0;
        for (uint32_t n = 1; n <= data.size(); n++) {
            std::string minimal_data = data.substr(data.size() - n, n);
            ra <<= 3;
            do {
                exec_emu_hardcoded_p2(ra, data, output);
                if (output != minimal_data) {
                    ra++;
                } else {
                    break;
                }
            } while (true);
        }

        out2 = std::to_string(ra);
    } else {
        out2 = "-1";
    }
}
