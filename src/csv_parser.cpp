#include "csv_parser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

static bool is_valid_column_name(const std::string& column_name) {
    for (char ch : column_name) {
        if (!std::isalpha(ch)) {
            return false;
        }
    }

    return true;
}

bool parse_csv(const std::string& filename, Table& table, std::ostream& err_msg ){

    std::ifstream file(filename);
    if (!file.is_open()) {
        err_msg << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::string line;
    size_t num_columns = 0;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std:: string cell;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        if (table.empty()) {
            num_columns = row.size();
            for (size_t i = 1; i <= num_columns; ++i) {
                if (!is_valid_column_name(row[i])) {
                    err_msg << "Error: Invalid column name: " << row[i] << std::endl;
                    return false;
                }
            }
        } else if (row.size() != num_columns) {
            err_msg << "Error: Inconsistent number of columns in row " << line << std::endl;
            return false;
        }

        table.push_back(row);
    }

    if (table.empty()) {
        err_msg << "Error: Empty file" << filename << std::endl;
        return false;
    }


    return true;
}

void print_table(const Table& table) {
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