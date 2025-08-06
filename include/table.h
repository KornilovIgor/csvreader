#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

class Table {
public:

    struct CellReference {
        std::string column;
        int row;
    };

    using Row = std::vector<std::string>;

    bool loadFromCSV(const std::string& fileName, std::ostream& errStream);
    bool evaluate(std::ostream& errStream);
    void print(std::ostream& outStream) const;

private:
    bool evaluateCell(size_t row, size_t column, std::unordered_set<std::string>& visited, std::ostream& errStream);
    static bool parseCell(const std::string& cell, CellReference& ref);
    bool getCellCoordinates(const CellReference& ref, size_t& rowIndex, size_t & columnIndex) const;

    std::vector<std::vector<std::string>> table_;
    std::unordered_map<std::string, size_t> columnMap_;
    std::unordered_map<int, size_t> rowMap_;
};