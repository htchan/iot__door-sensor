#ifndef DOOR_STATUS_TASK_CONST
#define DOOR_STATUS_TASK_CONST

#include "../../const.h"
#include "../available/const.h"

#define DOOR_STATUS_PIN GPIO_NUM_0
#define DOOR_STATUS_TASK_INTERVAL 1000
#define DOOR_STATUS_TOTAL_COUNT 10

#define DOOR_STATUS_OPEN "OPEN"
#define DOOR_STATUS_CLOSE "CLOSE"
const std::string DOOR_STATUS_ENTITY = "homeassistant/sensor/" + DEVICE_ID + "/door_status";
const std::string DOOR_STATUS_DISCOVERY_PAYLOAD =
    R"({
    "device":)" +
    DEVICE_PAYLOAD + R"(,
    "unique_id":")" +
    DEVICE_ID + R"(__door_status",
    "name":"Door Status",
    "state_topic":")" +
    DOOR_STATUS_ENTITY + STATE_TOPIC + R"(",
    "payload_on": ")" +
    DOOR_STATUS_OPEN + R"(",
    "payload_off": ")" +
    DOOR_STATUS_CLOSE + R"("
})";

#endif