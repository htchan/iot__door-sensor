#ifndef OTA_TASK_CONST
#define OTA_TASK_CONST

#include "../../const.h"
#define OTA_UPDATE_COMMAND "update"
#define OTA_UPDATE_NULL_COMMAND "null"

const std::string OTA_ENTITY = "homeassistant/button/" + DEVICE_ID + "/ota";
const std::string OTA_DISCOVERY_PAYLOAD =
    R"({
    "device": )" +
    DEVICE_PAYLOAD + R"(,
    "unique_id": ")" +
    DEVICE_ID + R"(__ota_update",
    "name": "OTA Update",
    "command_topic": ")" +
    OTA_ENTITY + COMMAND_TOPIC + R"(",
    "icon": "mdi:cloud-upload",
    "payload_press": ")" +
    OTA_UPDATE_COMMAND + R"(",
    "retain": true,
    "command_retain": true
})";

#endif