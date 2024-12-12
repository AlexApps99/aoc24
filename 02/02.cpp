#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

[[clang::no_sanitize("unsigned-integer-overflow")]]
// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_02(std::istream &in, std::string &out1, std::string &out2) {
    uint32_t counter_1 = 0;
    uint32_t counter_2 = 0;

    std::string line;
    while (std::getline(in, line)) {
        // grab the numbers within this report line
        std::stringstream ss(line);
        const std::vector<int16_t> report{std::istream_iterator<int16_t>(ss),
                                          {}};

        // go through all the choices of value to skip
        // (starting out of bounds, AKA no value is skipped)
        for (size_t skip_idx = report.size() + 1; skip_idx-- > 0;) {
            const bool nothing_skipped = skip_idx == report.size();

            // sentinel until initialized with observed direction
            int16_t sign_correction_factor = 0;
            // negative sentinel if nothing seen yet
            int16_t prev_num = -1;

            // should be set if a bad level is found
            bool no_bad_levels_seen = true;

            // go through each element in the report
            for (size_t i = 0; i < report.size(); i++) {
                if (i == skip_idx) {
                    // this value is being skipped
                    continue;
                }

                if (prev_num >= 0) {
                    // get difference between consecutive numbers
                    int16_t delta = report[i] - prev_num;
                    if (sign_correction_factor == 0) {
                        // init negative direction factor if sequence decreases
                        sign_correction_factor = delta < 0 ? -1 : 1;
                    }
                    // correct sign based on initial direction
                    delta *= sign_correction_factor;

                    // check if direction is good
                    if (delta < 1 || delta > 3) {
                        no_bad_levels_seen = false;
                        break;
                    }
                }

                // store previous number seen
                prev_num = report[i];
            }

            // sweet!
            if (no_bad_levels_seen) {
                if (nothing_skipped) {
                    counter_1++;
                }
                counter_2++;
                break;
            }
        }
    }

    // output results
    out1 = std::to_string(counter_1);
    out2 = std::to_string(counter_2);
}
