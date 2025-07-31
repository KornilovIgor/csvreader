#include "csv_parser.h"

#include <iostream>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    auto table = load_csv(filename);

    print_table(table);

    return 0;
}
