#ifndef DOOR_STATUS_TASK_H
#define DOOR_STATUS_TASK_H

#include "../../const.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <Arduino.h>

#define DOOR_STATUS_HISTORY_SIZE 5

extern QueueHandle_t publishDoorStatusQueue;
extern int doorStatusHistory[DOOR_STATUS_HISTORY_SIZE];
extern uint8_t historyCount, publishCount;

const std::string DOOR_STATUS_OPEN = "OPEN";
const std::string DOOR_STATUS_CLOSE = "CLOSE";
const std::string DOOR_STATUS_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/door_status";
const std::string DOOR_STATUS_DISCOVERY_PAYLOAD = 
R"({
    "device":)" + DEVICE_PAYLOAD + R"(,
    "unique_id":")" + DEVICE_ID + R"(__door_status",
    "name":"Door Status",
    "state_topic":")" + DOOR_STATUS_ENTITY + STATE_TOPIC + R"(",
    "payload_on": ")" + DOOR_STATUS_OPEN + R"(",
    "payload_off": ")" + DOOR_STATUS_CLOSE + R"("
})";


void setupDoorStatusTask();
void enablePublishDoorStatusTask();
void publishDoorStatusDiscovery();

#endif