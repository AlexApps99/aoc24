#include <cstdlib>
#include <iostream>

#ifndef LIST_OF_IMPLS
#define LIST_OF_IMPLS
#endif

#define X(id) void aoc_##id(const std::istream &in, const std::ostream &out);

LIST_OF_IMPLS

#undef X

int main(int argc, char *argv[]) {
    // make sure right number of args are passed in
    if (argc != 2) {
        std::cerr << "Example usage: aoc24 01 << 01.txt >> 01.txt" << std::endl;
        return EXIT_FAILURE;
    }

    // grab the day being run
    unsigned long day = strtoul(argv[1], nullptr, 10);
    if (day < 0 || day > 29) {
        std::cerr << "Invalid day specified." << std::endl;
        return EXIT_FAILURE;
    }

    // streams being used
    std::istream &in = std::cin;
    std::ostream &out = std::cout;

    // create switch-case for all days
#define X(id)                                                                  \
    case id:                                                                   \
        aoc_##id(in, out);                                                     \
        break;

    switch (day) {
        LIST_OF_IMPLS
    default:
        std::cerr << "Day not implemented!" << std::endl;
        return EXIT_FAILURE;
    }

#undef X

    return EXIT_SUCCESS;
}
