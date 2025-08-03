#include "evaluator.h"

#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <regex>

bool to_int(const std::string& s, int &result) {
    try  {
        size_t pos;
        result = std::stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

int get_column_index(const std::string& column_name, const std::vector<std::string>& header, std::unordered_map<std::string, int>& column_map) {
    auto it = column_map.find(column_name);

    if (it != column_map.end()) {
        return it->second;
    }

    for (size_t i = 1; i < header.size(); ++i) {
        if (header[i] == column_name) {
            column_map[column_name] = static_cast<int>(i);
            return i;
        }
    }

    return -1;
}

int get_row_index(int row_num, const Table& table, std::unordered_map<int, int>& row_map) {
    auto it = row_map.find(row_num);
    if (it != row_map.end()) {
        return it->second;
    }

    for (size_t row = 1; row < table.size(); ++row) {
        int row_header;
        if (to_int(table[row][0], row_header) && row_header == row_num) {
            row_map[row_num] = static_cast<int>(row);
            return row;
        }
    }
    row_map[row_num] = -1;
    return -1;
}

bool evaluate_cell(Table& table, int row, int column,
    std::unordered_set<std::string>& visited, std::ostream& error_msg, int& result, 
    bool& error_flag, std::unordered_map<std::string, int>& column_map, 
    std::unordered_map<int, int>& row_map);

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
    std::ostream& err_msg, bool& err_flag, int& result, std::unordered_map<std::string, int> column_map, std::unordered_map<int, int> row_map) {

    if (to_int(arg, result)) {
        return true;
    }

    size_t index_of_start_row_num = 0;
    while (index_of_start_row_num < arg.size() && std::isalpha(arg[index_of_start_row_num])) {
        index_of_start_row_num++;
    }

    std::string column_name = arg.substr(0, index_of_start_row_num);
    std::string row_str = arg.substr(index_of_start_row_num);

    int row_num;
    if (!to_int(row_str, row_num)) {
        return false;
    }

    int column_index = get_column_index(column_name, header, column_map);
    if (column_index == -1) {
        return false;
    }

    int row_index = get_row_index(row_num, table, row_map);
    if (row_index == -1) {
        return false;
    }


    return evaluate_cell(table, row_index, column_index, visited, err_msg, result, err_flag, column_map, row_map);
}

bool evaluate_cell(Table& table, int row, int column, std::unordered_set<std::string>& visited, std::ostream& error_msg, int& result, bool& error_flag, std::unordered_map<std::string, int>& column_map, std::unordered_map<int, int>& row_map) {

    std::string& cell = table[row][column];

    if (to_int(cell, result)) {
        return true;
    }

    std::string key = std::to_string(row) + "_" +std::to_string(column);
    if (visited.count(key)) {
        error_msg << "Cycle detected at cell: " << table[0][column] << table[row][0] << std::endl;
        error_flag = true;
        return false;
    }
    visited.insert(key);

    std::string arg1_str, arg2_str;
    char op;

    if (!parse_expression(cell, arg1_str, op, arg2_str)) {
        error_msg << "Invalid expression: " << cell << std::endl;
        return false;
    }

    //TODO: переделать в лямбда-функцию если останется время
    int arg1, arg2;
    if (!evaluate_argument(arg1_str, table, table[0], visited, error_msg, error_flag, arg1, column_map, row_map)) {
        if (!error_flag) {
            error_msg << "Invalid arg1: " << arg1_str << std::endl;
        }
        return false;
    }

    if (!evaluate_argument(arg2_str, table, table[0], visited, error_msg, error_flag, arg2, column_map, row_map)) {
        if (!error_flag) {
            error_msg << "Invalid arg2: " << arg2_str << std::endl;
        }
        return false;
    }

    switch (op) {
        case '+':
            result = arg1 + arg2;
            break;
        case '-':
            result = arg1 - arg2;
            break;
        case '*':
            result = arg1 * arg2;
            break;
        case '/':
            if (arg2 == 0) {
                error_msg << "Division by zero in " << cell << std::endl;
                return false;
            }
            result = arg1 / arg2;
            break;
        default:
            error_msg << "Invalid operator: " << op << std::endl;
            return false;
    }

    cell = std::to_string(result);

    return true;
}

bool evaluate_table(Table& table, std::ostream& err_stream) {
    bool err_flag = false;
    std::unordered_map<std::string, int> column_map;
    std::unordered_map<int, int> row_map;

    for (size_t row = 1; row < table.size(); ++row) {
        for (size_t column = 1; column < table[row].size(); ++column) {
            int result;
            std::unordered_set<std::string> visited;

            if (!evaluate_cell(table, row, column, visited, err_stream, result, err_flag, column_map, row_map)) {
                return false;
            }
        }
    }
    return true;
}