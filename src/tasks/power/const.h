#ifndef POWER_TASK_CONST
#define POWER_TASK_CONST

#include "../../const.h"
#include "../available/const.h"

#define POWER_STATE_TOTAL_COUNT 20
#define POWER_STATE_SEND_EVERY 10
#define POWER_UPDATE_INTERVAL_MS 100

#define POWER_VCC_MULTIPLIER 6.6f

const std::string POWER_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/vcc_voltage";
const std::string POWER_DISCOVERY_PAYLOAD =
    R"({
    "device":)" +
    DEVICE_PAYLOAD + R"(,
    "unique_id":")" +
    DEVICE_ID + R"(__vcc_voltage",
    "name":"VCC Voltage",
    "unit_of_measurement":"V",
    "state_topic":")" +
    POWER_ENTITY + STATE_TOPIC + R"(",
    "availability_topic":")" +
    AVAILABLE_ENTITY + STATE_TOPIC + R"(",
    "payload_not_available":"",
    "entity_category":"diagnostic",
    "state_class":"measurement",
    "device_class":"voltage"
})";

const std::string LOW_POWER_THRESHOLD_ENTITY = "homeassistant/number/" + DEVICE_ID + "/vcc_voltage_low_threshold";
const std::string LOW_POWER_THRESHOLD_DISCOVERY_PAYLOAD =
    R"({
    "device":)" +
    DEVICE_PAYLOAD + R"(,
    "unique_id": ")" +
    DEVICE_ID + R"(__vcc_voltage_low_threshold",
    "name": "VCC Voltage Low Threshold",
    "state_topic": ")" +
    LOW_POWER_THRESHOLD_ENTITY + STATE_TOPIC + R"(",
    "command_topic": ")" +
    LOW_POWER_THRESHOLD_ENTITY + COMMAND_TOPIC + R"(",
    "availability_topic": ")" +
    AVAILABLE_ENTITY + STATE_TOPIC + R"(",
    "payload_not_available": "",
    "mode": "box",
    "entity_category": "diagnostic",
    "device_class": "voltage",
    "min": 3.0,
    "max": 4.5,
    "step": 0.01,
    "retain": true
})";

#endif