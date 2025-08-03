#include "csv_parser.h"
#include "evaluator.h"

#include <gtest/gtest.h>
#include <fstream>

TEST(CsvParserTest, ParseValidCsvWithoutExpr) {
    Table table = {
        {"", "A", "B"},
        {"1", "10", "20"},
        {"2", "30", "40"}
    };

    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table[0].size(), 3);

    EXPECT_EQ(table[1][1], "10");
    EXPECT_EQ(table[2][2], "40");
}

TEST(CsvParserTest, EmptyCsvFile) {
    std::string filename = "empty.csv";

    // Создаём пустой файл
    std::ofstream out(filename);
    out.close();

    auto table = load_csv(filename);

    EXPECT_EQ(table.size(), 0);

    std::remove(filename.c_str());
}