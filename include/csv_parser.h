#pragma once

#include <string>
#include <vector>

std::vector<std::vector<std::string>> load_csv(const std::string& path_csvfile);
void print_table(const std::vector<std::vector<std::string>>& table);
