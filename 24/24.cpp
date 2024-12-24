#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

/**
 * Observations:
 *
 * z.. is always XOR with anon. OR and XOR as inputs.
 * its anon XOR has the two corresponding x/y vals as inputs.
 * its anon. OR has two anon ANDs as inputs.
 * the first of these is an AND of the previous x/y.
 * the second is an AND of the previous OR, and an XOR of the previous x/y.
 */

constexpr bool output_graphviz = false;
constexpr bool output_tree = false;

using wire = std::array<char, 3>;

namespace std {
template <> struct hash<wire> {
    std::size_t operator()(const wire &w) const {
        return std::hash<uint32_t>{}((static_cast<uint32_t>(w[0]) << 16) |
                                     (static_cast<uint32_t>(w[1]) << 8) |
                                     static_cast<uint32_t>(w[2]));
    }
};
} // namespace std

static std::ostream &operator<<(std::ostream &os, const wire &w) {
    os << w[0] << w[1] << w[2];
    return os;
}

enum class Op { XOR, OR, AND };

static Op parse_op(const std::string &s) {
    if (s == "XOR") {
        return Op::XOR;
    }
    if (s == "OR") {
        return Op::OR;
    }
    if (s == "AND") {
        return Op::AND;
    }
    throw std::runtime_error("Invalid op");
}

static const char *op_to_string(Op op) {
    switch (op) {
    case Op::XOR:
        return "XOR";
    case Op::OR:
        return "OR";
    case Op::AND:
        return "AND";
    }
    throw std::runtime_error("Invalid op");
}

struct Expr {
    wire in1;
    wire in2;
    Op op;
};

static bool eval(std::unordered_map<wire, std::variant<bool, Expr>> &vals,
                 const wire &w, std::unordered_set<wire> &visited,
                 uint64_t depth = 0) {
    if (output_tree) {
        for (uint64_t i = 0; i < depth; i++) {
            std::cerr << "  ";
        }
        std::cerr << w << " = ";
    }

    if (std::holds_alternative<bool>(vals[w])) {
        bool val = std::get<bool>(vals[w]);
        if (output_tree) {
            std::cerr << (val ? '1' : '0') << '\n';
        }
        return val;
    }

    const Expr &expr = std::get<Expr>(vals[w]);
    visited.insert(expr.in1);
    visited.insert(expr.in2);

    if (output_tree) {
        std::cerr << expr.in1 << ' ' << op_to_string(expr.op) << ' ' << expr.in2
                  << '\n';
    }

    bool val1 = eval(vals, expr.in1, visited, depth + 1);
    bool val2 = eval(vals, expr.in2, visited, depth + 1);

    bool val;
    switch (expr.op) {
    case Op::XOR:
        val = val1 != val2;
        break;
    case Op::OR:
        val = val1 || val2;
        break;
    case Op::AND:
        val = val1 && val2;
        break;
    }

    // saves some time :)
    // vals[w] = val;

    return val;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void aoc_24(std::istream &in, std::string &out1, std::string &out2) {
    std::string input_line{};

    std::unordered_map<wire, std::variant<bool, Expr>> vals;

    if (output_graphviz) {
        std::cerr << "digraph G {\n";
    }

    while (std::getline(in, input_line)) {
        if (input_line.empty()) {
            break;
        }
        if (input_line.size() != 6 || input_line[3] != ':' ||
            input_line[4] != ' ' ||
            (input_line[5] != '0' && input_line[5] != '1')) {
            continue;
        }
        wire w{input_line[0], input_line[1], input_line[2]};
        bool val = input_line[5] == '1';
        vals.emplace(w, val);

        if (output_graphviz) {
            std::cerr << "  " << w << " [shape=box label=\"\\N\\n("
                      << input_line[5] << ")\"]\n";
        }
    }

    while (std::getline(in, input_line)) {
        std::istringstream iss(input_line);
        std::string in1;
        std::string op;
        std::string in2;
        std::string arrow;
        std::string out;
        iss >> in1 >> op >> in2 >> arrow >> out;

        wire w_in1{in1[0], in1[1], in1[2]};
        wire w_in2{in2[0], in2[1], in2[2]};
        wire w_out{out[0], out[1], out[2]};
        Op op_enum = parse_op(op);
        vals.emplace(w_out, Expr{w_in1, w_in2, op_enum});

        const char *shape = "circle";
        switch (op_enum) {
        case Op::XOR:
            shape = "doubleoctagon";
            break;
        case Op::OR:
            shape = "diamond";
            break;
        case Op::AND:
            shape = "circle";
            break;
        }

        if (output_graphviz) {
            std::cerr << "  " << w_out << " [shape=" << shape
                      << " label=\"\\N\\n(" << op_to_string(op_enum)
                      << ")\"]\n";

            std::cerr << "  " << w_in1 << " -> " << w_out << '\n';
            std::cerr << "  " << w_in2 << " -> " << w_out << '\n';
        }
    }

    if (output_graphviz) {
        std::cerr << "}\n";
    }

    // for (auto &[w, val] : vals) {
    //     std::cerr << w << " = ";
    //     if (std::holds_alternative<Expr>(val)) {
    //         std::cerr << std::get<Expr>(val).in1 << ' '
    //                   << op_to_string(std::get<Expr>(val).op) << ' '
    //                   << std::get<Expr>(val).in2;
    //     } else {
    //         std::cerr << (std::get<bool>(val) ? '1' : '0');
    //     }
    //     std::cerr << '\n';
    // }

    uint64_t count1 = 0;
    uint64_t count2 = 0;

    for (uint8_t n = 0; n < 99; n++) {
        char c1 = '0' + static_cast<char>(n / 10);
        char c2 = '0' + static_cast<char>(n % 10);
        wire w_out{'z', c1, c2};
        if (vals.find(w_out) == vals.end()) {
            break;
        }
        std::unordered_set<wire> visited{};
        bool val = eval(vals, w_out, visited);
        for (const wire &w : visited) {
            if (w[0] == 'x' || w[0] == 'y' || w[0] == 'z') {
                uint8_t found_n = (w[1] - '0') * 10 + (w[2] - '0');
                if (found_n > n) {
                    std::cerr << "Found " << w << " = "
                              << static_cast<uint64_t>(found_n) << " in "
                              << w_out << '\n';
                }
                break;
            }
        }
        // std::cerr << w_out << " = " << val << '\n';
        count1 |= (val ? 1UL : 0UL) << n;
    }

    out1 = std::to_string(count1);
    out2 = std::to_string(count2);
}
