#include "csv_parser.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::vector<std::string>> load_csv(const std::string& filename){

    std::ifstream file(filename);

    std::vector<std::vector<std::string>> data;

    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std:: string cell;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}

void print_table(const std::vector<std::vector<std::string>>& table) {
    for (const auto& row : table) {
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << row[i];
            if (i < row.size() - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
}