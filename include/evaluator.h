#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using Table = std::vector<std::vector<std::string>>;

bool evaluate_table(Table& table, std::ostream& errStream);