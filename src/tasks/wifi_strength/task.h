#ifndef WIFI_STRENGTH_TASK_H
#define WIFI_STRENGTH_TASK_H

#include "../../const.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

const std::string WIFI_STRENGTH_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/wifi_strength";
const std::string WIFI_STRENGTH_DISCOVERY_PAYLOAD = 
R"({
    "device":)" + DEVICE_PAYLOAD + R"(,
    "unique_id":")" + DEVICE_ID + R"(__wifi_strength",
    "name":"Wifi Strength",
    "unit_of_measurement":"%",
    "state_topic":")" + WIFI_STRENGTH_ENTITY + STATE_TOPIC + R"(",
    "entity_category":"diagnostic",
    "state_class":"measurement"
})";

extern QueueHandle_t wifiStrengthQueue;

void setupWifiStrengthTask();
void publishWifiStrengthDiscovery();

#endif