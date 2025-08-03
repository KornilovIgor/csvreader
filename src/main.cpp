#include "csv_parser.h"
#include "evaluator.h"

//TODO: Сделать класс-обёртку (а надо ли в такой программе?)

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr <<  "Usage: " << argv[0] << " <file.csv>" << std::endl;
        return 1;
    }

    Table table;

    if (!parse_csv(argv[1], table, std::cerr)) {
        return 1;
    }

    if (!evaluate_table(table, std::cerr)) {
        return 1;
    }

    print_table(table);

    return 0;
}