#include "gtest/gtest.h"
#include "FreeRTOS_defs/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C" {
#include <hih8120.h>
#include "humidity_and_temperature.h"
}

// Declare and define fakes using FFF
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);
FAKE_VALUE_FUNC(int16_t, ReadTemperature);
FAKE_VALUE_FUNC(uint16_t, ReadHumidity);


// Test fixture for TEMHUM functions
class TEMHUMTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the fakes and clear the history before each test
        RESET_FAKE(hih8120_measure);
        RESET_FAKE(ReadHumidity);
        RESET_FAKE(ReadTemperature);
        FFF_RESET_HISTORY();
    }
};


// Test the ReadHumidity() function
TEST_F(TEMHUMTest, ReadHumidity) {
    // Set up test variables
    uint16_t HUM;

    // Call the function under test
    HUM = ReadHumidity();

    // Check the result
    EXPECT_EQ(0, HUM);
}

// Test the ReadTemperature() function
TEST_F(TEMHUMTest, ReadTemperature) {
    // Set up test variables
    int16_t TEM;

    // Call the function under test
    TEM = ReadTemperature();

    // Check the result
    EXPECT_EQ(0, TEM);
}