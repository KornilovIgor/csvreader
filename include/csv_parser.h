#pragma once

#include <string>
#include <vector>

using Table = std::vector<std::vector<std::string>>;

bool parse_csv(const std::string& filename, Table& table, std::ostream& err);
void print_table(const Table& table);