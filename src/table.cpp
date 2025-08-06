#include "table.h"

#include <fstream>
#include <sstream>
#include <cctype>
#include <regex>
#include <charconv>
#include <algorithm>

static bool isValidColumnName(const std::string& name) {
    return !name.empty() && std::all_of(name.begin(), name.end(), 
        [](char c) { return std::isalpha(c); });
}

static bool toInt(const std::string& numStr, int& result) {
    if (numStr.empty()) return false;
    auto [ptr, ec] = std::from_chars(numStr.data(), numStr.data() + numStr.size(), result);
    return ec == std::errc() && ptr == numStr.data() + numStr.size();
}

bool Table::loadFromCSV(const std::string& fileName, std::ostream& errStream) {
    if (fileName.empty()) {
        errStream << "Error: Empty filename" << std::endl;
        return false;
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        errStream << "Error: Unable to open file " << fileName << std::endl;
        return false;
    }

    std::string line;
    size_t numColumns = 0;

    while (std::getline(file, line)) {
        Row row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        if (table_.empty()) {
            numColumns = row.size();

            for (size_t i = 1; i < numColumns; ++i) {
                const std::string& columnName = row[i];
                if (!isValidColumnName(columnName)) {
                    errStream << "Error: Invalid column name: " << columnName << std::endl;
                    return false;
                }
                if (!columnMap_.emplace(columnName, i).second) {
                    errStream << "Error: Duplicate column name: " << columnName << std::endl;
                    return false;
                }
            }
        } else if (row.size() != numColumns) {
            errStream << "Error: Inconsistent number of columns in row " << line << std::endl;
            return false;
        }

        if (!table_.empty()) {
            int rowIdNum;
            if (!toInt(row[0], rowIdNum) || rowIdNum <= 0) {
                errStream << "Error: Invalid row ID: " << row[0] << std::endl;
                return false;
            }
            if (!rowMap_.emplace(rowIdNum, table_.size()).second) {
                errStream << "Error: Duplicate row ID: " << rowIdNum << std::endl;
                return false;
            }
        }

        table_.push_back(row);
    }

    if (table_.empty()) {
        errStream << "Error: Empty file " << fileName << std::endl;
        return false;
    }

    return true;
}

bool Table::evaluate(std::ostream& errStream) {
    if (table_.empty() || table_.size() < 2) {
        errStream << "Error: Table is empty" << std::endl;
        return false;
    }
    std::unordered_set<std::string> visited;

    for (size_t row = 1; row < table_.size(); ++row) {
        for (size_t column = 1; column < table_[row].size(); ++column) {
            if (!evaluateCell(row, column, visited, errStream)) {
                return false;
            }
            visited.clear();
        }
    }

    return true;
}

bool Table::evaluateCell(size_t row, size_t column, std::unordered_set<std::string>& visited, std::ostream& errStream) {
    std::string& cell = table_[row][column];
    int cellValue;

    if (toInt(cell, cellValue)) {
        return true;
    }

    std::string key = std::to_string(row) + "_" + std::to_string(column);
    if (visited.count(key)) {
        errStream << "Error: Cycle detected in cell: " << table_[0][column] << table_[row][0] << std::endl;
        return false;
    }
    visited.insert(key);

    if (cell.empty() || cell[0] != '=') {
        errStream << "Error: Invalid expression: " << cell << std::endl;
        return false;
    }

    std::regex expression(R"(^=([A-Za-z]+\d+|\d+)([*/+-])([A-Za-z]+\d+|\d+)$)");
    std::smatch matches;
    std::string arg1, arg2; 
    char op;
    
    if (std::regex_match(cell, matches, expression) && matches.size() == 4) {
        arg1 = matches[1].str();
        op = matches[2].str()[0];
        arg2 = matches[3].str();
    } else {
        errStream << "Error: Invalid expression format: " << cell << std::endl;
        return false;
    }

    int arg1Value, arg2Value;
    CellReference ref;

    auto evaluateExpression = [&](const std::string& arg, int& argValue) -> bool {
        if (toInt(arg, argValue)) {
            return true;
        }

        if (!parseCell(arg, ref)) {
            errStream << "Error: Invalid cell address: " << arg << std::endl;
            return false;
        }

        size_t rowIndex, columnIndex;
        if (!getCellCoordinates(ref, rowIndex, columnIndex)) {
            errStream << "Error: Cell not found: " << arg << std::endl;
            return false;
        }

        if (!evaluateCell(rowIndex, columnIndex, visited, errStream)) {
            return false;
        }
        
        if (!toInt(table_[rowIndex][columnIndex], argValue)) {
            errStream << "Error: Cell value is not integer: " << table_[rowIndex][columnIndex] << std::endl;
            return false;
        }
        return true;
    };

    if (!evaluateExpression(arg1, arg1Value)) {
        return false;
    }
    if (!evaluateExpression(arg2, arg2Value)) {
        return false;
    }

    switch (op) {
        case '+': cellValue = arg1Value + arg2Value; break;
        case '-': cellValue = arg1Value - arg2Value; break;
        case '*': cellValue = arg1Value * arg2Value; break;
        case '/': 
            if (arg2Value == 0) {
                errStream << "Error: Division by zero in " << cell << std::endl;
                return false;
            }
            cellValue = arg1Value / arg2Value;
            break;
        default:
            errStream << "Error: Invalid operator: " << op << std::endl;
            return false;
    }
    cell = std::to_string(cellValue);
    
    return true;
}

bool Table::parseCell(const std::string& cell, CellReference& ref) {
    size_t num_pos = 0;
    while (num_pos < cell.size() && std::isalpha(cell[num_pos])) {
        num_pos++;
    }

    if (num_pos == 0 || num_pos == cell.size()) {
        return false;
    }

    if (!toInt(cell.substr(num_pos), ref.row) || ref.row <= 0) {
        return false;
    }

    ref.column = cell.substr(0, num_pos);
    return true;
}

bool Table::getCellCoordinates(const CellReference& ref, size_t& rowIndex, size_t & columnIndex) const {
    auto columnIt = columnMap_.find(ref.column);
    if (columnIt == columnMap_.end()) {
        return false;
    }
    columnIndex = columnIt->second;

    auto rowIt = rowMap_.find(ref.row);
    if (rowIt == rowMap_.end()) {
        return false;
    }
    rowIndex = rowIt->second;

    return true;
}

void Table::print(std::ostream& outStream) const {
    for (const auto& row : table_) {
        for (size_t i = 0; i < row.size(); ++i) {
            outStream << row[i];
            if (i < row.size() - 1) {
                outStream << ",";
            }
        }
        outStream << std::endl;
    }
}
