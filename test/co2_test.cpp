#include "gtest/gtest.h"
#include "FreeRTOS_defs/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C" {
#include <mh_z19.h>
#include "co2.h"
}

// Declare and define fakes using FFF
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);
FAKE_VALUE_FUNC(uint16_t, readCO2);

// Test fixture for CO2 functions
class CO2Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the fakes and clear the history before each test
        RESET_FAKE(mh_z19_takeMeassuring);
        RESET_FAKE(readCO2);
        FFF_RESET_HISTORY();
    }
};


// Test the readCO2() function
TEST_F(CO2Test, ReadCO2) {
    // Set up test variables
    uint16_t CO2;

    // Call the function under test
    CO2 = readCO2();

    // Check the result
    EXPECT_EQ(0, CO2);
}