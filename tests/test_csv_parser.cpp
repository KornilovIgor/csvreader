#include "csv_parser.h"
#include <gtest/gtest.h>
#include <fstream>

TEST(CsvParserTest, ParsesValidCsvWithoutFormulas) {
    std::ofstream file("simple.csv");
    file << ",A,B\n1,10,20\n2,30,40\n";
    file.close();

    Table table;
    std::stringstream err;
    ASSERT_TRUE(parse_csv("simple.csv", table, err));

    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table[1][1], "10");
    ASSERT_EQ(table[2][2], "40");

    std::remove("simple.csv");
}

TEST(CsvParserTest, FailsOnInvalidColumnNames) {
    std::ofstream file("invalid.csv");
    file << ",A1,B2\n1,10,20\n";
    file.close();

    Table table;
    std::stringstream err;
    ASSERT_FALSE(parse_csv("invalid.csv", table, err));
    EXPECT_TRUE(err.str().find("Invalid column name") != std::string::npos);

    std::remove("invalid.csv");
}
