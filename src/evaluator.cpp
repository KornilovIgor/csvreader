#include "evaluator.h"

#include <iostream>
#include <sstream>
#include <unordered_set>
#include <cctype>
#include <regex>

int get_column_index(const std::vector<std::string>& header, const std::string& name) {
    for (size_t i = 1; i < header.size(); ++i) {
        if (header[i] == name) {
            return i;
        }
    }
    return -1;
}

bool to_int(const std::string& s, int &result) {
    try  {
        size_t pos;
        result = std::stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

bool evaluate_cell(Table& table, int row, int column, std::unordered_set<std::string>& visited, std::ostream& err, int &result);

bool parse_expression(const std::string& expr, std::string& arg1, char& op, std::string& arg2) {
    std::regex pattern(R"(^=([A-Za-z]+\d+|\d+)([+*/-])([A-Za-z]+\d+|\d+)$)");
    std::smatch matches;

    if(std::regex_match(expr, matches, pattern)) {
        if(matches.size() == 4) {
            arg1 = matches[1].str();
            op = matches[2].str()[0];
            arg2 = matches[3].str();
            return true;
        }
    }
    return false;
}

bool evaluate_argument(const std::string& arg, Table& table,
    const std::vector<std::string>& header, std::unordered_set<std::string>& visited,
    std::ostream& err, int& result) {

    if (to_int(arg, result)) {
        return true;
    }

    size_t i = 0;
    while (i < arg.size() && std::isalpha(arg[i])) {
        i++;
    }

    std::string column_name = arg.substr(0, i);
    std::string row_str = arg.substr(i);

    int row_num;
    if (!to_int(row_str, row_num)) {
        return false;
    }

    int column_index = get_column_index(table[0], column_name);
    if (column_index == -1) {
        return false;
    }

    for (size_t r = 1; r < table.size(); ++r) {
        int row_header;
        if (to_int(table[r][0], row_header) && row_header == row_num) {
            return evaluate_cell(table, r, column_index, visited, err, result);
        }
    }
    return false;
}

bool evaluate_cell(Table& table, int row, int column, std::unordered_set<std::string>& visited, std::ostream& err, int& result) {

    std::string& cell = table[row][column];

    if (to_int(cell, result)) {
        return true;
    }

    std::string key = std::to_string(row) + "_" +std::to_string(column);
    if (visited.count(key)) {
        err << "Cycle detected at cell (" << table[row][0] << "," << table[0][column] << ")" << std::endl;
        return false;
    }
    visited.insert(key);

    std::string arg1, arg2;
    char op;

    if (!parse_expression(cell, arg1, op, arg2)) {
        err << "Invalid expression: " << cell << std::endl;
        return false;
    }

    int left, right;
    if (!evaluate_argument(arg1, table, table[0], visited, err, left)) {
        err << "Invalid arg1: " << arg1 << std::endl;
        return false;
    }

    if (!evaluate_argument(arg2, table, table[0], visited, err, right)) {
        err << "Invalid arg2: " << arg2 << std::endl;
        return false;
    }

    switch (op) {
        case '+':
            result = left + right;
            break;
        case '-':
            result = left - right;
            break;
        case '*':
            result = left * right;
            break;
        case '/':
            if (right == 0) {
                err << "Division by zero in " << cell << std::endl;
                return false;
            }
            result = left / right;
            break;
        default:
            err << "Invalid operator: " << op << std::endl;
            return false;
    }

    cell = std::to_string(result);

    return true;
}

bool evaluate_table(Table& table, std::ostream& err_stream) {
    for (size_t row = 1; row < table.size(); ++row) {
        for (size_t column = 1; column < table[row].size(); ++column) {
            int result;
            std::unordered_set<std::string> visited;
            evaluate_cell(table, row, column, visited, err_stream, result);
        }
    }
    return true;
}