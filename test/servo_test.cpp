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
FAKE_VALUE_FUNC(struct Status*, readStatus);
FAKE_VALUE_FUNC(struct Configuration*, readConfiguration);
FAKE_VOID_FUNC(updateConfiguration, uint16_t, uint16_t, int16_t, int16_t, uint16_t, uint16_t, int8_t);

// Test fixture for Servo functions
class ServoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the fakes and clear the history before each test
        RESET_FAKE(initialiseServo);
        RESET_FAKE(readStatus);
        RESET_FAKE(readConfiguration);
        RESET_FAKE(updateConfiguration);
        FFF_RESET_HISTORY();
    }
};

// Test the initialiseServo() function
TEST_F(ServoTest, initialiseServo) {
    // Set up test variables
    UBaseType_t servoTaskPriority = 1;
    uint16_t minCO2 = 0;
    uint16_t maxCO2 = 700;
    int16_t minTemperature = 50;
    int16_t maxTemperature = 500;
    uint16_t minHumidity = 0;
    uint16_t maxHumidity = 1000;
    int8_t degreeRotation = 100;

    // Call the function under test
    initialiseServo(servoTaskPriority, minCO2, maxCO2, minTemperature, maxTemperature,
                    minHumidity, maxHumidity, degreeRotation);

    // Check that initialiseServo was called with the correct arguments
    EXPECT_EQ(1, initialiseServo_fake.arg0_history[0]);
    EXPECT_EQ(0, initialiseServo_fake.arg1_history[0]);
    EXPECT_EQ(700, initialiseServo_fake.arg2_history[0]);
    EXPECT_EQ(50, initialiseServo_fake.arg3_history[0]);
    EXPECT_EQ(500, initialiseServo_fake.arg4_history[0]);
    EXPECT_EQ(0, initialiseServo_fake.arg5_history[0]);
    EXPECT_EQ(1000, initialiseServo_fake.arg6_history[0]);
    EXPECT_EQ(100, initialiseServo_fake.arg7_history[0]);
}

// Test the readStatus() function
TEST_F(ServoTest, readStatus) {
    // Set up the status object with test values
    struct Status status;
    status.servoDegrees = 90;
    status.CO2_value = 400;
    status.humidity_value = 100;
    status.temperature_value = 55;

     // Set the return value for the function
    readStatus_fake.return_val = &status;

    // Call the function under test
    struct Status* result = readStatus();

    // Perform any necessary assertions or checks
    // For example, check if the returned status matches the expected values
    EXPECT_EQ(result->servoDegrees, status.servoDegrees);
    EXPECT_EQ(result->CO2_value, status.CO2_value);
    EXPECT_EQ(result->humidity_value, status.humidity_value);
    EXPECT_EQ(result->temperature_value, status.temperature_value);
}

// Test the readConfiguration() function
TEST_F(ServoTest, readConfiguration) {
    // Set up the status object with test values
    struct Configuration configuration;
    configuration.minCO2_config = 0;
    configuration.maxCO2_config = 700;
    configuration.minTemperature_config = 50;
    configuration.maxTemperature_config = 500;
    configuration.minHumidity_config = 0;
    configuration.maxHumidity_config = 1000;
    configuration.degreeRotation = 100;

     // Set the return value for the function
    readConfiguration_fake.return_val = &configuration;

    // Call the function under test
    struct Configuration* result = readConfiguration();

    // Perform any necessary assertions or checks
    // For example, check if the returned status matches the expected values
    EXPECT_EQ(result->minCO2_config, configuration.minCO2_config);
    EXPECT_EQ(result->maxCO2_config, configuration.maxCO2_config);
    EXPECT_EQ(result->minTemperature_config, configuration.minTemperature_config);
    EXPECT_EQ(result->maxTemperature_config, configuration.maxTemperature_config);
    EXPECT_EQ(result->minHumidity_config, configuration.minHumidity_config);
    EXPECT_EQ(result->maxHumidity_config, configuration.maxHumidity_config);
    EXPECT_EQ(result->degreeRotation, configuration.degreeRotation);
}

// Test the updateConfiguration() function
TEST_F(ServoTest, updateConfiguration) {
    // Set up test variables
    uint16_t minCO2 = 0; 
    uint16_t maxCO2 = 700;
	int16_t minTemperature = 50; 
    int16_t maxTemperature = 500;
	uint16_t minHumidity = 0; 
    uint16_t maxHumidity = 1000;
	int8_t degreeRotation = 100;

    // Call the function under test
    updateConfiguration(minCO2, maxCO2, minTemperature, maxTemperature,
                    minHumidity, maxHumidity, degreeRotation);

    // Check that updateConfiguration was called with the correct arguments
    EXPECT_EQ(0, updateConfiguration_fake.arg0_history[0]);
    EXPECT_EQ(700, updateConfiguration_fake.arg1_history[0]);
    EXPECT_EQ(50, updateConfiguration_fake.arg2_history[0]);
    EXPECT_EQ(500, updateConfiguration_fake.arg3_history[0]);
    EXPECT_EQ(0, updateConfiguration_fake.arg4_history[0]);
    EXPECT_EQ(1000, updateConfiguration_fake.arg5_history[0]);
    EXPECT_EQ(100, updateConfiguration_fake.arg6_history[0]);
}

