#include "table.h"

#include <iostream>
#include <unordered_map>

int main(int argc, const char* argv[]) {
    if (argc >= 2) {
        Table table;
        if (table.loadFromCSV(argv[1], std::cerr)) {
            if (table.evaluate(std::cerr)) {
                table.print(std::cout);
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    } else {
        std::cerr <<  "Usage: " << argv[0] << " <file.csv>" << std::endl;
        return 1;
    } 

    return 0;
}