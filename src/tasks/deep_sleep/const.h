#ifndef DEEP_SLEEP_TASK_CONST
#define DEEP_SLEEP_TASK_CONST

#include "../../const.h"
#include "../available/const.h"

#define OFFLINE_FORCE_SLEEP_DURATION_MS 60000
#define ONLINE_FORCE_SLEEP_DURATION_MS 10000

#define DEFAULT_SLEEP_DURATION_MS 1800000 // 30 min

const std::string UPDATE_FREQUENCY_ENTITY = "homeassistant/select/" + DEVICE_ID + "/update_frequency";
const std::string UPDATE_FREQUENCY_DISCOVERY_PAYLOAD =
    R"({
    "device":)" +
    DEVICE_PAYLOAD + R"(,
    "unique_id": ")" +
    DEVICE_ID + R"(__update_frequency",
    "name": "Update Frequency",
    "command_topic": ")" +
    UPDATE_FREQUENCY_ENTITY + STATE_TOPIC + R"(",
    "state_topic": ")" +
    UPDATE_FREQUENCY_ENTITY + STATE_TOPIC + R"(",
    "availability_topic": ")" +
    AVAILABLE_ENTITY + STATE_TOPIC + R"(",
    "payload_not_available": "",
    "options": [
        "No Sleep",
        "10 min","20 min","30 min","60 min",
        "2 hr","4 hr","6 hr","8 hr","12 hr","24 hr"
    ],
    "retain": true
})";

const std::string SLEEP_DURATION_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/sleep_duration";
const std::string SLEEP_DURATION_DISCOVERY_PAYLOAD =
    R"({
    "device":)" +
    DEVICE_PAYLOAD + R"(,
    "unique_id":")" +
    DEVICE_ID + R"(__sleep_duration",
    "name":"Sleep Duration",
    "unit_of_measurement":"s",
    "state_topic":")" +
    SLEEP_DURATION_ENTITY + STATE_TOPIC + R"(",
    "availability_topic":")" +
    AVAILABLE_ENTITY + STATE_TOPIC + R"(",
    "payload_not_available":"",
    "entity_category":"diagnostic",
    "device_class":"duration"
})";

#endif