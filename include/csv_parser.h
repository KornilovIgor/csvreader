#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using Table = std::vector<std::vector<std::string>>;

bool parse_csv(
    const std::string& filename,
    Table& table,
    std::unordered_map<std::string, size_t>& column_map,
    std::unordered_map<std::string, size_t>& row_map,
    std::ostream& err);
void print_table(const Table& table);