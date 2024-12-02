#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool is_report_safe(std::vector<uint64_t> report, bool skip_one) {
    // slow but not messy :)
    // starts with OOB idx (so nothing skipped)
    size_t loop_bound = skip_one ? 0 : report.size();
    for (size_t skip_idx = report.size(); skip_idx >= loop_bound; skip_idx--) {
        bool is_this_one_good = true;
        int8_t delta_mul_fac = 0;

        // sentinel if nothing seen yet
        uint64_t prev_num = UINT64_MAX;

        for (size_t i = 0; i < report.size(); i++) {
            if (i == skip_idx) {
                continue;
            }

            uint64_t num = report[i];

            if (prev_num != UINT64_MAX) {
                int64_t delta = (static_cast<int64_t>(num) -
                                 static_cast<int64_t>(prev_num));
                if (delta_mul_fac == 0) {
                    delta_mul_fac = delta < 0 ? -1 : 1;
                }
                delta *= delta_mul_fac;

                if (delta < 1 || delta > 3) {
                    is_this_one_good = false;
                    break;
                }
            }

            prev_num = num;
        }

        if (is_this_one_good) {
            return true;
        }

        // bruh this code is not great
        if (skip_idx == 0) {
            break;
        }
    }

    return false;
}

void aoc_02(std::istream &in, std::ostream &out1, std::ostream &out2) {
    uint32_t counter_1 = 0;
    uint32_t counter_2 = 0;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string num_str;

        std::vector<uint64_t> report{};

        while (std::getline(ss, num_str, ' ')) {
            const uint64_t num = std::strtoul(num_str.c_str(), nullptr, 10);

            report.push_back(num);
        }

        if (is_report_safe(report, false)) {
            counter_1++;
        }

        if (is_report_safe(report, true)) {
            counter_2++;
        }
    }

    out1 << counter_1 << '\n';
    out2 << counter_2 << '\n';
}
