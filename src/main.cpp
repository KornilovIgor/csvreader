#include "csv_parser.h"
#include "evaluator.h"

#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./csvreader file.csv\n";
        return 1;
    }

    std::string filename = argv[1];

    auto table = load_csv(filename);

    std::ostringstream errors;

    if (!evaluate_table(table, errors)) {
        std::cerr << "Error during evaluation: " << errors.str() << std::endl;
        return 1;
    }

    print_table(table);

    return 0;
}