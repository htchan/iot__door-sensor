#ifndef CONST_H
#define CONST_H

#include <string>
#include <WiFi.h>

#define VERSION "1.0.0"

std::string uint64ToHex(uint64_t value);

const std::string DEVICE_NAME_VIEW = "Door Sensor";
const std::string DEVICE_NAME = "door_sensor";
const std::string DEVICE_ID = DEVICE_NAME + "-" + uint64ToHex(ESP.getEfuseMac());
const std::string DEVICE_PAYLOAD =
    R"({
    "identifiers":[")" +
    DEVICE_ID + R"("],
    "name":")" +
    DEVICE_NAME_VIEW + R"(",
    "model":"D1 Mini",
    "manufacturer":"Espressif"
})";

const std::string STATE_TOPIC = "/state";
const std::string COMMAND_TOPIC = "/command";
const std::string DISCOVERY_TOPIC = "/config";

#define LED_PIN GPIO_NUM_8

#define LOOP_INTERVAL_MS 500

#endif