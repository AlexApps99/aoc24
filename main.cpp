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

constexpr std::ios_base::openmode OUT_MODE =
    std::ios::out | std::ios::trunc | std::ios::binary;

constexpr std::ios_base::openmode IN_MODE = std::ios::in | std::ios::binary;

#ifndef LIST_OF_IMPLS
#define LIST_OF_IMPLS X(01)
#endif

#define X(id)                                                                  \
    void aoc_##id(std::istream &in, std::ostream &out1, std::ostream &out2);

LIST_OF_IMPLS

#undef X

// typedef of above function signature:
using aoc_func = void(std::istream &in, std::ostream &out1, std::ostream &out2);

std::optional<std::reference_wrapper<aoc_func>> get_aoc_func(uint64_t day) {
    // create switch-case for all days
#define X(id)                                                                  \
    case id:                                                                   \
        return std::ref(aoc_##id);

    switch (day) {
        LIST_OF_IMPLS
    default:
        return std::nullopt;
    }

#undef X
}

// check if the computed output matches the groundtruth,
// return EXIT_SUCCESS if it does, EXIT_FAILURE otherwise.
// if they don't match, print both to stderr.
int test_part(const std::string &name, std::stringbuf &computed,
              std::ifstream &groundtruth) {

    std::stringstream groundtruth_buffer;
    groundtruth_buffer << groundtruth.rdbuf();

    const std::string groundtruth_str = groundtruth_buffer.str();
    const std::string computed_str = computed.str();

    if (computed_str != groundtruth_str) {
        std::cerr << name << " failed." << '\n'
                  << "Computed: " << '\n'
                  << computed_str << '\n'
                  << "Expected: " << '\n'
                  << groundtruth_str << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int test(const std::string &day_str, aoc_func &func,
         const std::string &prefix) {
    // get string of day padded to 2 digits

    const std::string input_file = day_str + "/" + prefix + "input.txt";
    std::ifstream in{input_file, IN_MODE};

    if (!in.is_open()) {
        std::cerr << "Error opening file: " << input_file << '\n';
        return EXIT_FAILURE;
    }

    std::stringbuf out1_buffer;
    std::ostream out1{&out1_buffer}; // NOLINT(misc-const-correctness)

    std::stringbuf out2_buffer;
    std::ostream out2{&out2_buffer}; // NOLINT(misc-const-correctness)

    func(in, out1, out2);

    // grab expected output files as strings
    const std::string expected_output_file1 =
        day_str + "/" + prefix + "output1.txt";
    std::ifstream expected1{expected_output_file1, IN_MODE};

    if (!expected1.is_open()) {
        std::cerr << "Error opening file: " << expected_output_file1 << '\n';
        return EXIT_FAILURE;
    }

    const std::string expected_output_file2 =
        day_str + "/" + prefix + "output2.txt";
    std::ifstream expected2{expected_output_file2, IN_MODE};

    if (!expected2.is_open()) {
        std::cerr << "Error opening file: " << expected_output_file2 << '\n';
        return EXIT_FAILURE;
    }

    const int ret1 = test_part(day_str + " (part 1)", out1_buffer, expected1);
    if (ret1 != EXIT_SUCCESS) {
        return ret1;
    }
    const int ret2 = test_part(day_str + " (part 2)", out2_buffer, expected2);
    if (ret2 != EXIT_SUCCESS) {
        return ret2;
    }

    return EXIT_SUCCESS;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
int main(int argc, char *argv[]) {
    std::unique_ptr<std::ifstream> in_file{};
    std::unique_ptr<std::ofstream> out1_file{};
    std::unique_ptr<std::ofstream> out2_file{};

    bool run_test = false;

    if (argc < 2 || argc > 4) {
        std::cerr << "Example usage: aoc24 01 << 01.txt >> 01.txt" << '\n';
        return EXIT_FAILURE;
    }

    // grab the day being run
    const uint64_t day = strtoul(argv[1], nullptr, 10);
    std::optional<std::reference_wrapper<aoc_func>> func = get_aoc_func(day);

    if (!func) {
        std::cerr << "Invalid day specified." << '\n';
        return EXIT_FAILURE;
    }

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << day;
    const std::string day_str = ss.str();

    // make sure right number of args are passed in
    if (argc == 2) {
        // do nothing, use default streams
    } else if (argc == 3) {
        std::optional<std::string> run_prefix{};
        const std::string run_mode{argv[2]};
        if (run_mode == "test") {
            run_test = true;
        } else if (run_mode == "run") {
            run_prefix = "";
        } else if (run_mode == "example") {
            run_prefix = "example.";
        } else {
            std::cerr << "Invalid mode specified." << '\n';
            return EXIT_FAILURE;
        }

        if (run_prefix) {
            const std::string input_file =
                day_str + "/" + run_prefix.value() + "input.txt";
            const std::string output_file1 =
                day_str + "/" + run_prefix.value() + "output1.txt";
            const std::string output_file2 =
                day_str + "/" + run_prefix.value() + "output2.txt";

            in_file = std::make_unique<std::ifstream>(input_file, IN_MODE);
            out1_file = std::make_unique<std::ofstream>(output_file1, OUT_MODE);
            out2_file = std::make_unique<std::ofstream>(output_file2, OUT_MODE);
        }
    } else if (argc == 4) {
        // open files for writing, if "-" is passed in, use default
        // streams
        const std::string a2{argv[2]};
        const std::string a3{argv[3]};
        if (!a2.empty() && a2 != "-") {
            out1_file = std::make_unique<std::ofstream>(a2, OUT_MODE);
        }
        if (!a3.empty() && a3 != "-") {
            out2_file = std::make_unique<std::ofstream>(a3, OUT_MODE);
        }
    }

    if (in_file && !in_file->is_open()) {
        std::cerr << "Error opening input file." << '\n';
        return EXIT_FAILURE;
    }

    if (out1_file && !out1_file->is_open()) {
        std::cerr << "Error opening output1 file." << '\n';
        return EXIT_FAILURE;
    }

    if (out2_file && !out2_file->is_open()) {
        std::cerr << "Error opening output2 file." << '\n';
        return EXIT_FAILURE;
    }

    // NOLINTBEGIN(misc-const-correctness)
    std::istream &in = in_file ? *in_file : std::cin;
    std::ostream &out1 = out1_file ? *out1_file : std::cout;
    std::ostream &out2 = out2_file ? *out2_file : std::cout;
    // NOLINTEND(misc-const-correctness)

    if (run_test) {
        const int example_ret = test(day_str, func.value(), "example.");
        if (example_ret != EXIT_SUCCESS) {
            return example_ret;
        }
        const int input_ret = test(day_str, func.value(), "");
        if (input_ret != EXIT_SUCCESS) {
            return input_ret;
        }
    } else {
        aoc_func &f = func.value();
        f(in, out1, out2);
        return EXIT_SUCCESS;
    }
}
