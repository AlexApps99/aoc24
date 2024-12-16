#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// aoc24 gen 01 <- optional example <- optional
// aoc24 test 01 <- optional example <- optional
// aoc24 run 01 - - <- optional

// move to more general format of examples (3 had two different examples, for
// example)

static constexpr std::ios_base::openmode OUT_MODE =
    std::ios::out | std::ios::trunc | std::ios::binary;

static constexpr std::ios_base::openmode IN_MODE =
    std::ios::in | std::ios::binary;

// see the wikipedia page for X-macros (you will either love it or hate it)
#ifndef LIST_OF_IMPLS
#define LIST_OF_IMPLS X(01)
#endif

#define X(id)                                                                  \
    void aoc_##id(std::istream &in, std::string &out1, std::string &out2);

LIST_OF_IMPLS

#undef X

// typedef of above function signature:
using aoc_func = void(std::istream &in, std::string &out1, std::string &out2);

static std::optional<std::reference_wrapper<aoc_func>>
get_aoc_func(const uint64_t day) {
    // create switch-case for all days

    if (day > 99) {
        return std::nullopt;
    }

    // (hack is to avoid octal repr - "id" must have leading digit e.g. 01)
    // 10**8 chosen as 10**8 & 0xFF == 00
#define X(id)                                                                  \
    case 100'000'0##id##UL:                                                    \
        return std::ref(aoc_##id);

    switch (100'000'000UL | day) {
        LIST_OF_IMPLS
    default:
        return std::nullopt;
    }

#undef X
}

// check if the computed output matches the groundtruth,
// return EXIT_SUCCESS if it does, EXIT_FAILURE otherwise.
// if they don't match, print both to stderr.
static int test_part(const std::string &name, const std::string &computed_str,
                     const std::string &groundtruth_str,
                     const uint64_t micros) {

    bool test_passed = computed_str == groundtruth_str ||
                       computed_str + '\n' == groundtruth_str;

    float millis = static_cast<float>(micros) / 1000.0f;

    if (test_passed) {
        std::cerr << "✅ " << std::left << std::setw(5) << name << " (in "
                  << std::right << std::setw(7) << std::fixed
                  << std::setprecision(2) << millis << "ms)\n";
    } else {
        std::cerr << "❌ " << std::left << std::setw(5) << name << " failed.\n"
                  << "Computed:\n"
                  << computed_str << '\n'
                  << "Expected:\n"
                  << groundtruth_str << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static int test(const std::string &day_str, aoc_func &func,
                const std::string &prefix, const bool gen_files) {
    // get string of day padded to 2 digits

    const std::string path_with_prefix =
        day_str + "/" + prefix + (prefix.empty() ? "" : ".");

    const std::string input_file = path_with_prefix + "input.txt";
    const std::string expected_output1_file = path_with_prefix + "output1.txt";
    const std::string expected_output2_file = path_with_prefix + "output2.txt";

    std::ifstream in{input_file, IN_MODE};

    if (!in.is_open()) {
        std::cerr << "Error opening file: " << input_file << '\n';
        return EXIT_FAILURE;
    }

    std::string out1_str;
    std::string out2_str;
    std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    func(in, out1_str, out2_str);
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();

    const uint64_t micros =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();

    if (gen_files) {
        std::ofstream expected1{expected_output1_file, OUT_MODE};

        if (!expected1.is_open()) {
            std::cerr << "Error opening file: " << expected_output1_file
                      << '\n';
            return EXIT_FAILURE;
        }

        expected1 << out1_str << '\n';

        std::ofstream expected2{expected_output2_file, OUT_MODE};

        if (!expected2.is_open()) {
            std::cerr << "Error opening file: " << expected_output2_file
                      << '\n';
            return EXIT_FAILURE;
        }

        expected2 << out2_str << '\n';
    } else {
        std::ifstream expected1{expected_output1_file, IN_MODE};

        if (!expected1.is_open()) {
            std::cerr << "Error opening file: " << expected_output1_file
                      << '\n';
            return EXIT_FAILURE;
        }

        std::stringstream expected1_ss;
        expected1_ss << expected1.rdbuf();
        const std::string expected1_str = expected1_ss.str();

        std::ifstream expected2{expected_output2_file, IN_MODE};

        if (!expected2.is_open()) {
            std::cerr << "Error opening file: " << expected_output2_file
                      << '\n';
            return EXIT_FAILURE;
        }

        std::stringstream expected2_ss;
        expected2_ss << expected2.rdbuf();
        const std::string expected2_str = expected2_ss.str();

        std::string shorthand_prefix = " (" + prefix + ")";

        if (prefix.empty()) {
            shorthand_prefix = "";
        } else if (prefix == "example") {
            shorthand_prefix = "e";
        }

        const int ret1 = test_part(day_str + "/1" + shorthand_prefix, out1_str,
                                   expected1_str, micros);
        if (ret1 != EXIT_SUCCESS) {
            return ret1;
        }

        const int ret2 = test_part(day_str + "/2" + shorthand_prefix, out2_str,
                                   expected2_str, micros);
        if (ret2 != EXIT_SUCCESS) {
            return ret2;
        }
    }

    return EXIT_SUCCESS;
}

static int run(aoc_func &func,
               std::optional<std::array<std::ofstream, 2>> &outputs) {
    std::string out1_str;
    std::string out2_str;
    func(std::cin, out1_str, out2_str);

    if (outputs) {
        std::ostream &out1 = outputs.value()[0];
        std::ostream &out2 = outputs.value()[1];
        out1 << out1_str << '\n';
        out2 << out2_str << '\n';
    } else {
        // pretty-print for console
        std::cout << "# Part 1:\n"
                  << out1_str << "\n\n# Part 2:\n"
                  << out2_str << '\n';
    }

    return EXIT_SUCCESS;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
int main(int argc, char *argv[]) {
    std::unique_ptr<std::ifstream> in_file{};
    std::unique_ptr<std::ofstream> out1_file{};
    std::unique_ptr<std::ofstream> out2_file{};

    if (argc >= 2) {
        const std::string run_mode{argv[1]};
        if (run_mode == "run") {
            if (argc == 3 || argc == 5) {
                const uint64_t day = strtoul(argv[2], nullptr, 10);
                std::optional<std::reference_wrapper<aoc_func>> func_opt =
                    get_aoc_func(day);
                if (func_opt) {
                    std::optional<std::array<std::ofstream, 2>> outputs{
                        std::nullopt};
                    if (argc == 5) {
                        std::string out1_file{argv[3]};
                        std::string out2_file{argv[4]};
                        std::ofstream out1{argv[3], OUT_MODE};
                        std::ofstream out2{argv[4], OUT_MODE};
                        if (!out1.is_open()) {
                            std::cerr
                                << "Error opening output file: " << out1_file
                                << '\n';
                            return EXIT_FAILURE;
                        }
                        if (!out2.is_open()) {
                            std::cerr
                                << "Error opening output file: " << out2_file
                                << '\n';
                            return EXIT_FAILURE;
                        }
                        std::array<std::ofstream, 2> files = {std::move(out1),
                                                              std::move(out2)};

                        outputs.emplace(std::move(files));
                    }
                    return run(func_opt.value(), outputs);
                }
            }

            std::cerr << "Example usage: `aoc24 run 01`, "
                         "`aoc24 run 01 out1.txt out2.txt`\n";
            return EXIT_FAILURE;
        }

        if (run_mode == "test" || run_mode == "gen") {
            bool is_gen = run_mode == "gen";
            if (argc == 2) {
#define X(id)                                                                  \
    {                                                                          \
        int ret = test(#id, aoc_##id, "example", is_gen);                      \
        if (ret != EXIT_SUCCESS) {                                             \
            return ret;                                                        \
        }                                                                      \
        ret = test(#id, aoc_##id, "", is_gen);                                 \
        if (ret != EXIT_SUCCESS) {                                             \
            return ret;                                                        \
        }                                                                      \
    }

                LIST_OF_IMPLS

#undef X
                return EXIT_SUCCESS;
            }

            if (argc == 3 || argc == 4) {
                const uint64_t day = std::strtoul(argv[2], nullptr, 10);

                std::stringstream ss;
                ss << std::setw(2) << std::setfill('0') << day;
                const std::string day_str = ss.str();

                std::optional<std::reference_wrapper<aoc_func>> func_opt =
                    get_aoc_func(day);
                if (func_opt) {
                    std::vector<std::string> prefixes{"example", ""};

                    if (argc == 4) {
                        prefixes.clear();
                        std::string arg_prefix = argv[3];
                        if (arg_prefix == "game") {
                            prefixes.emplace_back("");
                        } else {
                            prefixes.emplace_back(std::move(arg_prefix));
                        }
                    }

                    for (const std::string &prefix : prefixes) {
                        const int ret =
                            test(day_str, func_opt.value(), prefix, is_gen);
                        if (ret != EXIT_SUCCESS) {
                            return ret;
                        }
                    }
                    return EXIT_SUCCESS;
                }
            }

            std::cerr << "Example usage: `aoc24 test`, "
                         "`aoc24 test 01`, "
                         "`aoc24 test 01 example`\n";
            return EXIT_FAILURE;
        }
    }

    std::cerr << "Example usage: `aoc24 [ run | test | gen]`\n";
    return EXIT_FAILURE;
}
