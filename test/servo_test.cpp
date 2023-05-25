#include "gtest/gtest.h"
#include "FreeRTOS_defs/fff.h"
#include "FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C" {
#include "servo.h"
}

// Declare and define fakes using FFF
FAKE_VOID_FUNC(initialiseServo, UBaseType_t, uint16_t, uint16_t, int16_t, int16_t, uint16_t, uint16_t, int8_t);
FAKE_VOID_FUNC(updateConfiguration, uint16_t, uint16_t, int16_t, int16_t, uint16_t, uint16_t, int8_t);


// Test fixture for Servo functions
class ServoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the fakes and clear the history before each test
        RESET_FAKE(initialiseServo);
        RESET_FAKE(updateConfiguration);
        FFF_RESET_HISTORY();
    }
};

// Test the initialiseServo() function
TEST_F(ServoTest, initialiseServo) {
    // Set up test variables
    UBaseType_t servoTaskPriority = 1;
    uint16_t minCO2 = 400;
    uint16_t maxCO2 = 1000;
    int16_t minTemperature = 20;
    int16_t maxTemperature = 30;
    uint16_t minHumidity = 40;
    uint16_t maxHumidity = 60;
    int8_t degreeRotation = 90;

    // Call the function under test
    initialiseServo(servoTaskPriority, minCO2, maxCO2, minTemperature, maxTemperature,
                    minHumidity, maxHumidity, degreeRotation);

    // Check that initialiseServo was called with the correct arguments
    EXPECT_EQ(1, initialiseServo_fake.arg0_history[0]);
    EXPECT_EQ(400, initialiseServo_fake.arg1_history[0]);
    EXPECT_EQ(1000, initialiseServo_fake.arg2_history[0]);
    EXPECT_EQ(20, initialiseServo_fake.arg3_history[0]);
    EXPECT_EQ(30, initialiseServo_fake.arg4_history[0]);
    EXPECT_EQ(40, initialiseServo_fake.arg5_history[0]);
    EXPECT_EQ(60, initialiseServo_fake.arg6_history[0]);
    EXPECT_EQ(90, initialiseServo_fake.arg7_history[0]);
}


