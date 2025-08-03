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

TEST(ParserTest, InconsistentColumnCount) {
    std::ofstream file("bad_columns.csv");
    file << ",A,B\n";
    file << "1,5\n";
    file.close();

    Table table;
    std::ostringstream err;
    bool ok = parse_csv("bad_columns.csv", table, err);

    EXPECT_FALSE(ok);
    EXPECT_NE(err.str().find("Inconsistent number of columns"), std::string::npos);
    std::remove("bad_columns.csv");
}

TEST(CSVParserTest, DuplicateColumnNames) {
    std::stringstream csv_input(
        " ,A,B,A\n"
        "1,2,3,4\n"
    );
    Table table;
    std::stringstream err;

    std::ofstream tmp("duplicate_columns.csv");
    tmp << csv_input.str();
    tmp.close();

    EXPECT_FALSE(parse_csv("duplicate_columns.csv", table, err));
    EXPECT_NE(err.str().find("Duplicate column name"), std::string::npos);
    std::remove("duplicate_columns.csv");
}

TEST(CSVParserTest, DuplicateRowNumbers) {
    std::stringstream csv_input(
        " ,A,B\n"
        "1,2,3\n"
        "1,4,5\n"
    );
    Table table;
    std::stringstream err;

    std::ofstream tmp("duplicate_rows.csv");
    tmp << csv_input.str();
    tmp.close();

    EXPECT_FALSE(parse_csv("duplicate_rows.csv", table, err));
    EXPECT_NE(err.str().find("Duplicate row ID"), std::string::npos);
    std::remove("duplicate_rows.csv");
}
