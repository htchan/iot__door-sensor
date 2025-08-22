#ifndef UPTIME_TASK_H
#define UPTIME_TASK_H

#include "../../const.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

const std::string UPTIME_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/uptime";
const std::string UPTIME_DISCOVERY_PAYLOAD = 
R"({
    "device":)" + DEVICE_PAYLOAD + R"(,
    "unique_id":")" + DEVICE_ID + R"(__uptime",
    "name":"Uptime",
    "unit_of_measurement":"ms",
    "state_topic":")" + UPTIME_ENTITY + STATE_TOPIC + R"(",
    "entity_category":"diagnostic",
    "state_class":"measurement",
    "device_class":"duration"
})";

extern QueueHandle_t uptimeQueue;

void setupUptimeTask();
void publishUptimeDiscovery();
void publishUptime();

#endif
