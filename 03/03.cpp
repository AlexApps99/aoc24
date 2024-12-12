#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

// this must be called when i points to "m",
// it seeks forward to the first char that doesn't match
// the pattern mul\([0-9]+,[0-9]+\).
// returns number to be added.
static uint64_t scan_mul(const std::string &input, size_t &i) {
    size_t next_bad_val = 0;

    const std::string preamble{"mul("};

    // seek forward as appropriate here
    for (char k : preamble) {
        if (i > input.length() || input[i] != k) {
            return 0;
        }
        i += 1;
    }

    if (i > input.length()) {
        return 0;
    }

    // find next non-number
    next_bad_val = input.find_first_not_of("0123456789", i);
    if (next_bad_val == std::string::npos) {
        i = input.length();
        return 0;
    }
    if (next_bad_val == i) {
        // keep searching from this point
        return 0;
    }
    const std::string a_str = input.substr(i, next_bad_val - i);
    const uint64_t a = std::strtoul(a_str.c_str(), nullptr, 10);
    i = next_bad_val;

    // at this point, we should have something like this:
    // mul\([0-9]+.
    //            ^

    if (input[i] != ',') {
        // keep searching from this point
        return 0;
    }
    i += 1;
    if (i > input.length()) {
        return 0;
    }

    // find next non-number
    next_bad_val = input.find_first_not_of("0123456789", i);
    if (next_bad_val == std::string::npos) {
        i = input.length();
        return 0;
    }
    if (next_bad_val == i) {
        // keep searching from this point
        return 0;
    }
    const std::string b_str = input.substr(i, next_bad_val - i);
    const uint64_t b = std::strtoul(b_str.c_str(), nullptr, 10);
    i = next_bad_val;

    if (input[i] != ')') {
        // keep searching from this point
        return 0;
    }
    i += 1;

    return a * b;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_03(std::istream &in, std::string &out1, std::string &out2) {
    // read input to string
    const std::string input{std::istreambuf_iterator<char>(in), {}};

    uint64_t result_off = 0;
    uint64_t result_on = 0;
    bool is_mul_on = true;

    size_t i = 0;

    const std::string DO_FUNC{"do()"};
    const std::string DONT_FUNC{"don't()"};

    while (i < input.length()) {
        if (input[i] == 'm') {
            const uint64_t scan_result = scan_mul(input, i);
            if (is_mul_on) {
                result_on += scan_result;
            } else {
                result_off += scan_result;
            }
        } else if (input[i] == 'd') {
            if (input.substr(i, DO_FUNC.length()) == DO_FUNC) {
                is_mul_on = true;
                i += DO_FUNC.length();
            } else if (input.substr(i, DONT_FUNC.length()) == DONT_FUNC) {
                is_mul_on = false;
                i += DONT_FUNC.length();
            } else {
                i += 1;
            }
        } else {
            // will be > input.length() if none found
            i = input.find_first_of("dm", i);
        }
    }

    out1 = std::to_string(result_off + result_on);
    out2 = std::to_string(result_on);
}
