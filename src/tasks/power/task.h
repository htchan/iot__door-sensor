#ifndef POWER_TASK_H
#define POWER_TASK_H

#include "../../const.h"

const std::string POWER_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/vcc_voltage";
const std::string POWER_DISCOVERY_PAYLOAD = 
R"({
    "device":)" + DEVICE_PAYLOAD + R"(,
    "unique_id":")" + DEVICE_ID + R"(__vcc_voltage",
    "name":"VCC Voltage",
    "unit_of_measurement":"V",
    "state_topic":")" + POWER_ENTITY + STATE_TOPIC + R"(",
    "entity_category":"diagnostic",
    "state_class":"measurement",
    "device_class":"voltage"
})";

void setupPowerTask();
void publishPowerDiscovery();

#endif