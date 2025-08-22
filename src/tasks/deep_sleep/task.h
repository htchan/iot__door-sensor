#ifndef DEEP_SLEEP_TASK_H
#define DEEP_SLEEP_TASK_H

#include "../../const.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <Arduino.h>

extern QueueHandle_t deepSleepQueue;

void setupDeepSleepTask();
void enableDeepSleepTask();

#endif