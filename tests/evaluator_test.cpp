#include <gtest/gtest.h>

#include <evaluator.h>

TEST(EvaluatorTest, BasicAddition) {
    Table table = {
        {"", "A", "B", "Cell"},
        {"1", "1", "0", "-1"},
        {"2", "2", "=A1+Cell30", "0"},
        {"30", "0", "=B1+A1", "5"}
    };

    std::ostringstream err;

    bool ok = evaluate_table(table, err);

    EXPECT_TRUE(ok);
    EXPECT_EQ(table[2][2], "6");   // Ячейка B2: =A1 + Cell30 = 1 + 5 = 6
    EXPECT_EQ(table[3][2], "1");   // Ячейка B30: =B1 + A1 = 0 + 1 = 1
    EXPECT_EQ(err.str(), "");
}

TEST(EvaluatorTest, DivisionByZero) {
    Table table = {
        {"", "A", "B"},
        {"1", "10", "0"},
        {"2", "=A1/B1", "5"}
    };

    std::ostringstream err;
    bool ok = evaluate_table(table, err);

    EXPECT_FALSE(ok);
    EXPECT_NE(err.str().find("Division by zero"), std::string::npos);
}