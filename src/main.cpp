#include "csv_parser.h"
#include "evaluator.h"

#include <iostream>
#include <unordered_map>

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr <<  "Usage: " << argv[0] << " <file.csv>" << std::endl;
        return 1;
    }

    Table table;
    std::unordered_map<std::string, size_t> column_map;
    std::unordered_map<std::string, size_t> row_map;

    if (!parse_csv(argv[1], table, column_map, row_map, std::cerr)) {
        return 1;
    }

    if (!evaluate_table(table, std::cerr)) {
        return 1;
    }

    print_table(table);

    return 0;
}