#include "csv_parser.h"
#include "evaluator.h"

#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./csv_eval file.csv\n";
        return 1;
    }

    std::string filename = argv[1];

    auto table = load_csv(filename);

    std::ostringstream errors;
    evaluate_table(table, errors);

    std::string errs = errors.str();
    if (!errs.empty()) {
        std::cerr << "Errors during evaluation:\n" << errs;
    }

    print_table(table);

    return 0;
}