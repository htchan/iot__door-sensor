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
    "identifiers":[")" + DEVICE_ID + R"("],
    "name":")" + DEVICE_NAME_VIEW + R"(",
    "model":"D1 Mini",
    "manufacturer":"Espressif"
})";

const std::string STATE_TOPIC = "/state";
const std::string COMMAND_TOPIC = "/command";
const std::string DISCOVERY_TOPIC = "/config";

#define LED_PIN GPIO_NUM_8

#define OFFLINE_FORCE_SLEEP_DURATION_MS 10000
#define ONLINE_FORCE_SLEEP_DURATION_MS 3000


#define DEFAULT_SLEEP_DURATION_MS 12000

#define SENSOR_POWER_PIN GPIO_NUM_1

#define LOOP_INTERVAL_MS 5000


#define WIFI_CONNECT_INTERVAL_MS 7000
#define WIFI_FAST_CONNECT_TIMEOUT_MS 2000
#define MQTT_CONNECT_INTERVAL_MS 7000


#define WIFI_STRENGTH_PUBLISH_INTERVAL_MS 20000

#define CONTINUE_SUBSCRIBE_COUNT 10
#define MQTT_SUBSCRIBE_TASK_INTERVAL 2000

// sensor related const //
#define MQTT_CONN_CHECK_INTERVAL 1000
//// power sensor ////
#define POWER_STATE_TOTAL_COUNT 5
#define POWER_STATE_SEND_EVERY 10
#define POWER_TASK_INTERVAL 20000

#define POWER_VCC_MULTIPLIER 2.0f


//// uptime sensor ////
#define UPTIME_PUBLISH_INTERVAL_MS 20000

//// wifi strength sensor ////
#define WIFI_STRENGTH_TASK_INTERVAL 30000

///// door status sensor ////
#define DOOR_STATUS_PIN GPIO_NUM_0
#define DOOR_STATUS_TASK_INTERVAL 1000

#endif