/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include <stdlib.h>

#include "co2.h"
#include "humidity_and_temperature.h"
#include "servo.h"
#include "display_7seg.h"

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "2BBE8F09765BBF4B"
#define LORA_appKEY "5F83717BC67B4646E3F00DC5EC3417DC"

void lora_handler_task( void *pvParameters );

static lora_driver_payload_t _uplink_payload;
static lora_driver_payload_t _downlink_payload;

MessageBufferHandle_t downlink_message_buffer;

void lora_handler_initialise(UBaseType_t lora_handler_task_priority)
{
    // message buffer for downlink messages
    downlink_message_buffer = xMessageBufferCreate(sizeof(lora_driver_payload_t));

    // Initialise the LoRaWAN driver without down-link buffer
    lora_driver_initialise(1, downlink_message_buffer);

    xTaskCreate(
	lora_handler_task
	,  "LRHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );


}

static void _lora_setup(void)
{
	char _out_buf[20];
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST3); // Led the yellow led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n",
           lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n",
           lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",
           lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n",
           _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n",
           LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",
           lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n",
           lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n",
           500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_fastBlink(led_ST1);
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
            status_leds_longPuls(led_ST3);
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2);
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2);
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1);

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

    lora_driver_returnCode_t rc;

	_uplink_payload.len = 7;
	_uplink_payload.portNo = 2;

    _downlink_payload.len = 12;
    _downlink_payload.portNo = 1;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{

        // ********** UPLINK (TX) **********

		xTaskDelayUntil( &xLastWakeTime, xFrequency );

		// current data payload
		uint16_t co2_ppm = readCO2(); 
		uint16_t hum = ReadHumidity(); 
		int16_t temp = ReadTemperature(); 
		int servstatus;
		servstatus = 1;

		_uplink_payload.bytes[0] = co2_ppm >> 8; 		// most significant byte
		_uplink_payload.bytes[1] = co2_ppm & 0xFF; 		// least significant byte
		_uplink_payload.bytes[2] = hum >> 8; 			// most significant byte
		_uplink_payload.bytes[3] = hum & 0xFF; 			// least significant byte
		_uplink_payload.bytes[4] = temp >> 8; 			// most significant byte
		_uplink_payload.bytes[5] = temp & 0xFF; 		// least significant byte
		_uplink_payload.bytes[6] = servstatus & 0xFF; 	// 8bit boolean

		status_leds_ledOn(led_ST4);
		/*
		 * printf("Upload Message >%s<\n",
               lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
        */
        //rc = lora_driver_sendUploadMessage(false, &_uplink_payload);
        if ((rc = lora_driver_sendUploadMessage(false, &_uplink_payload)) == LORA_MAC_TX_OK)
        {
            status_leds_longPuls(led_ST4);
            printf("Upload Message OK - No Message Received!!\n");
        }
        else
        {
            if (rc == LORA_MAC_RX)
            {
                // message buffer to receive downlink messages in
                xMessageBufferReceive(downlink_message_buffer, &_downlink_payload, sizeof(_downlink_payload), portMAX_DELAY);

                // ********** DOWNLINK MESSAGE HANDLING (RX) **********

                status_leds_slowBlink(led_ST4);

                // for debugging purposes
                printf("Upload Message OK - \n\tDOWN LINK: from port: %d with %d bytes received!",
                       _downlink_payload.portNo, _downlink_payload.len);

                // display the number of bytes received for debugging purposes
                display_7seg_displayHex("512E");
                vTaskDelay(pdMS_TO_TICKS(3000UL));
                display_7seg_display(_downlink_payload.len, 0);

                // check we receive the correct size of the payload
                if (_downlink_payload.len == 12)
                {
                    // fast blink led_ST4 to indicate the payload has the correct size
                    status_leds_fastBlink(led_ST4);
                    // pointers for each variable of the payload
                    uint16_t *minCO2_ptr;
                    uint16_t *maxCO2_ptr;
                    uint16_t *minHum_ptr;
                    uint16_t *maxHum_ptr;
                    int16_t *minTemp_ptr;
                    int16_t *maxTemp_ptr;

                    // allocate memory for the values
                    minCO2_ptr = malloc(sizeof(uint16_t));
                    maxCO2_ptr = malloc(sizeof(uint16_t));
                    minHum_ptr = malloc(sizeof(uint16_t));
                    maxHum_ptr = malloc(sizeof(uint16_t));
                    minTemp_ptr = malloc(sizeof(int16_t));
                    maxTemp_ptr = malloc(sizeof(int16_t));

                    // decode the payload into the variables through the pointers
                    *minCO2_ptr = (_downlink_payload.bytes[0] << 8) | _downlink_payload.bytes[1];
                    *maxCO2_ptr = (_downlink_payload.bytes[2] << 8) | _downlink_payload.bytes[3];
                    *minHum_ptr = (_downlink_payload.bytes[4] << 8) | _downlink_payload.bytes[5];
                    *maxHum_ptr = (_downlink_payload.bytes[6] << 8) | _downlink_payload.bytes[7];
                    *minTemp_ptr = (_downlink_payload.bytes[8] << 8) | _downlink_payload.bytes[9];
                    *maxTemp_ptr = (_downlink_payload.bytes[10] << 8) | _downlink_payload.bytes[11];

                    // use the values to update the servo configuration
                    updateConfiguration(*minCO2_ptr, *maxCO2_ptr, *minTemp_ptr,
                                        *maxTemp_ptr, *minHum_ptr, *maxHum_ptr);

                    // if statement for debugging purposes
                    if (*maxCO2_ptr == 0xFFFF)
                    {
                        // for debugging purposes
                        status_leds_fastBlink(led_ST3);
                    }
                    else
                    {
                        // for debugging purposes
                        status_leds_shortPuls(led_ST3);
                    }

                    vTaskDelay(pdMS_TO_TICKS(20UL));

                    // free the memory
                    free(minCO2_ptr);
                    free(maxCO2_ptr);
                    free(minHum_ptr);
                    free(maxHum_ptr);
                    free(minTemp_ptr);
                    free(maxTemp_ptr);

                    // set pointers to NULL
                    minCO2_ptr = NULL;
                    maxCO2_ptr = NULL;
                    minHum_ptr = NULL;
                    maxHum_ptr = NULL;
                    minTemp_ptr = NULL;
                    maxTemp_ptr = NULL;

                    // turn off LEDs from debug if () after 10 seconds
                    vTaskDelay(pdMS_TO_TICKS(10000UL));
                    status_leds_ledOff(led_ST3);
                    status_leds_ledOff(led_ST4);
                }
            }
            else
            {
                status_leds_fastBlink(led_ST1);
                printf("Upload Message failed >%s<\n", lora_driver_mapReturnCodeToText(rc));
            }
        }
	}
}

/*-----------------------------------------------------------*/
