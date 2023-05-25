#include "gtest/gtest.h"
// Include interfaces and define global variables
// defined by the production code
extern "C" {
}
// Create Test fixture and Reset all Mocks before each test
class Test_production : public ::testing::Test {
    protected:
    void SetUp() override {}
    void TearDown() override {}
};
// Test that the task is created correct
TEST_F(Test_production, Test_something) {
    EXPECT_EQ(1, 1);
}