#pragma once

#include <stdio.h>
#include <stdint.h>

//FreeRTOS
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

#include <hih8120.h>

void initialiseHumidity();
void measureHumidity();
void humidityTask(void* parameter);