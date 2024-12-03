#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

bool check_next_keyword(const std::string input, size_t &i,
                        const std::string keyword) {
    for (size_t j = 0; j < keyword.size(); j++) {
        if (i > input.length()) {
            return false;
        }
        if (input[i] != keyword[j]) {
            return false;
        }
        i += 1;
    }
    return true;
}

// this must be called when i points to "m",
// it seeks forward to the first char that doesn't match
// the pattern
bool check_next_mul(const std::string input, size_t &i, uint64_t &result) {
    uint64_t a = 0, b = 0;
    size_t next_bad_val = 0;

    // seek forward as appropriate here
    if (!check_next_keyword(input, i, "mul(")) {
        return false;
    }

    if (i > input.length()) {
        goto buffer_end;
    }

    // find next non-number
    next_bad_val = input.find_first_not_of("0123456789", i);
    if (next_bad_val == std::string::npos) {
        goto buffer_end;
    }
    if (next_bad_val == i) {
        // keep searching from this point
        return false;
    }
    {
        std::string a_str = input.substr(i, next_bad_val - i);
        a = std::strtoul(a_str.c_str(), nullptr, 10);
    }
    i = next_bad_val;

    // at this point, we should have something like this:
    // mul\([0-9]+.
    //            ^

    if (input[i] != ',') {
        // keep searching from this point
        return false;
    }
    i += 1;
    if (i > input.length()) {
        goto buffer_end;
    }

    // find next non-number
    next_bad_val = input.find_first_not_of("0123456789", i);
    if (next_bad_val == std::string::npos) {
        goto buffer_end;
    }
    if (next_bad_val == i) {
        // keep searching from this point
        return false;
    }
    {
        std::string b_str = input.substr(i, next_bad_val - i);
        b = std::strtoul(b_str.c_str(), nullptr, 10);
    }
    i = next_bad_val;

    if (input[i] != ')') {
        // keep searching from this point
        return false;
    }
    i += 1;

    result += a * b;

    return true;

buffer_end:
    i = input.length();
    return false;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_03(std::istream &in, std::string &out1, std::string &out2) {
    // read input to string
    std::string input{std::istreambuf_iterator<char>(in), {}};

    uint64_t result_off = 0;
    uint64_t result_on = 0;
    bool is_mul_on = true;

    size_t i = 0;

    while (i < input.length()) {
        if (input[i] == 'm') {
            check_next_mul(input, i, is_mul_on ? result_on : result_off);
            continue;
        } else if (input[i] == 'd') {
            if (input.substr(i, 4) == "do()") {
                is_mul_on = true;
                i += 4;
                continue;
            } else if (input.substr(i, 7) == "don't()") {
                is_mul_on = false;
                i += 7;
                continue;
            } else {
                i += 1;
                continue;
            }
        } else {
            i = input.find_first_of("dm", i);
            if (i == std::string::npos) {
                i = input.length();
            }
        }
    }

    out1 = std::to_string(result_off + result_on);
    out2 = std::to_string(result_on);
}
