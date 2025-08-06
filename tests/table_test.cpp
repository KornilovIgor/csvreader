#include <gtest/gtest.h>
#include "table.h"
#include <sstream>
#include <fstream>
#include <cstdio>

class TableTest : public ::testing::Test {
protected:
    void SetUp() override {
        createTestFile("valid.csv", ",A,B\n1,10,20\n2,30,40\n");
        createTestFile("invalid_col.csv", ",A1,B2\n1,10,20\n");
        createTestFile("bad_columns.csv", ",A,B\n1,5\n");
        createTestFile("dup_cols.csv", ",A,B,A\n1,2,3,4\n");
        createTestFile("dup_rows.csv", ",A,B\n1,2,3\n1,4,5\n");
        createTestFile("basic_expr.csv", ",A,B,Cell\n1,1,0,-1\n2,2,=A1+Cell30,0\n30,0,=B1+A1,5\n");
        createTestFile("div_zero.csv", ",A,B\n1,10,0\n2,=A1/B1,5\n");
        createTestFile("invalid_addr.csv", ",A\n1,5\n2,=Z99+1\n");
        createTestFile("cycle.csv", ",A,B\n1,=B1+0,=A1+0\n");
        createTestFile("print_test.csv", ",ColA,ColB\n1,10,20\n2,30,40\n");
    }

    void TearDown() override {
        remove("valid.csv");
        remove("invalid_col.csv");
        remove("bad_columns.csv");
        remove("dup_cols.csv");
        remove("dup_rows.csv");
        remove("basic_expr.csv");
        remove("div_zero.csv");
        remove("invalid_addr.csv");
        remove("cycle.csv");
        remove("print_test.csv");
    }

    void createTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        file << content;
        file.close();
    }
    
    std::string evaluateAndGetCell(const std::string& filename, 
                                  const std::string& cellRef,
                                  std::ostream& err) 
    {
        Table table;
        if (!table.loadFromCSV(filename, err)) return "";
        if (!table.evaluate(err)) return "";
        
        std::ostringstream out;
        table.print(out);
        std::string output = out.str();
        
        return parseOutputForCell(output, cellRef);
    }
    
    std::string parseOutputForCell(const std::string& output, const std::string& cellRef) {
        std::istringstream iss(output);
        std::string line;
        std::vector<std::vector<std::string>> rows;
        
        while (std::getline(iss, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;
            
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            rows.push_back(row);
        }
        
        if (cellRef.size() < 2) return "";
        
        char colChar = cellRef[0];
        int rowId = std::stoi(cellRef.substr(1));
        
        size_t colIndex = 0;
        size_t rowIndex = 0;
        
        if (rows.empty()) return "";
        for (size_t i = 1; i < rows[0].size(); i++) {
            if (rows[0][i] == std::string(1, colChar)) {
                colIndex = i;
                break;
            }
        }
        if (colIndex == 0) return "";
        
        for (size_t i = 1; i < rows.size(); i++) {
            if (rows[i][0] == std::to_string(rowId)) {
                rowIndex = i;
                break;
            }
        }
        if (rowIndex == 0) return "";
        
        return rows[rowIndex][colIndex];
    }
};

TEST_F(TableTest, ParsesValidCsvWithoutFormulas) {
    Table table;
    std::ostringstream err;
    ASSERT_TRUE(table.loadFromCSV("valid.csv", err)) << err.str();
    ASSERT_TRUE(err.str().empty());
    
    std::ostringstream out;
    table.print(out);
    std::string output = out.str();
    
    EXPECT_NE(output.find("1,10,20"), std::string::npos);
    EXPECT_NE(output.find("2,30,40"), std::string::npos);
}

TEST_F(TableTest, FailsOnInvalidColumnNames) {
    Table table;
    std::ostringstream err;
    ASSERT_FALSE(table.loadFromCSV("invalid_col.csv", err));
    EXPECT_NE(err.str().find("Invalid column name"), std::string::npos);
}

TEST_F(TableTest, HandlesInconsistentColumnCount) {
    Table table;
    std::ostringstream err;
    ASSERT_FALSE(table.loadFromCSV("bad_columns.csv", err));
    EXPECT_NE(err.str().find("Inconsistent number of columns"), std::string::npos);
}

TEST_F(TableTest, DetectsDuplicateColumnNames) {
    Table table;
    std::ostringstream err;
    ASSERT_FALSE(table.loadFromCSV("dup_cols.csv", err));
    EXPECT_NE(err.str().find("Duplicate column name"), std::string::npos);
}

TEST_F(TableTest, DetectsDuplicateRowIds) {
    Table table;
    std::ostringstream err;
    ASSERT_FALSE(table.loadFromCSV("dup_rows.csv", err));
    EXPECT_NE(err.str().find("Duplicate row ID"), std::string::npos);
}

TEST_F(TableTest, EvaluatesBasicExpressions) {
    std::ostringstream err;
    std::string b2 = evaluateAndGetCell("basic_expr.csv", "B2", err);
    std::string b30 = evaluateAndGetCell("basic_expr.csv", "B30", err);
    
    EXPECT_EQ(b2, "6");
    EXPECT_EQ(b30, "1");
    EXPECT_TRUE(err.str().empty());
}

TEST_F(TableTest, DetectsDivisionByZero) {
    std::ostringstream err;
    evaluateAndGetCell("div_zero.csv", "A2", err);
    
    EXPECT_FALSE(err.str().empty());
    EXPECT_NE(err.str().find("Division by zero"), std::string::npos);
}

TEST_F(TableTest, HandlesInvalidCellAddress) {
    std::ostringstream err;
    evaluateAndGetCell("invalid_addr.csv", "A2", err);
    
    EXPECT_FALSE(err.str().empty());
    EXPECT_NE(err.str().find("Cell not found"), std::string::npos);
}

TEST_F(TableTest, DetectsCyclicDependencies) {
    std::ostringstream err;
    evaluateAndGetCell("cycle.csv", "A1", err);
    
    EXPECT_FALSE(err.str().empty());
    EXPECT_NE(err.str().find("Cycle detected"), std::string::npos);
}

TEST_F(TableTest, PrintsTableCorrectly) {
    Table table;
    std::ostringstream err;
    ASSERT_TRUE(table.loadFromCSV("print_test.csv", err)) << err.str();
    EXPECT_TRUE(err.str().empty()) << "Error: " << err.str();
    
    std::ostringstream out;
    table.print(out);
    
    std::string expected = ",ColA,ColB\n1,10,20\n2,30,40\n";
    EXPECT_EQ(out.str(), expected);
}