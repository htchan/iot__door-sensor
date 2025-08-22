#include "task.h"
#include "../../utils/network/network.h"
#include <WiFi.h>

int8_t lastRssi = 0;
QueueHandle_t wifiStrengthQueue = NULL;

int8_t readWiFiStrength() {
    int8_t strength = 2 * (WiFi.RSSI() + 100);

    if (strength < 0) strength = 0;
    if (strength > 100) strength = 100;

    return strength;
}

void wifiStrengthTask(void * parameter) {
    // Wait for start signal
    uint8_t signal;
    wifiStrengthQueue = xQueueCreate(1, sizeof(uint8_t));
    xQueueReceive(wifiStrengthQueue, &signal, portMAX_DELAY);

    while (!mqttClient.connected()) {
        vTaskDelay(pdMS_TO_TICKS(MQTT_CONN_CHECK_INTERVAL));
    }

    while(true) {
        if (mqttClient.connected()) {
            int8_t currentRssi = readWiFiStrength();
            Serial.printf("Current WiFi Strength: %d%%\n", currentRssi);
            
            // Only publish if value changed or first reading
            if (currentRssi != lastRssi) {
                char rssiStr[8];
                snprintf(rssiStr, sizeof(rssiStr), "%d", currentRssi);

                mqttClient.publish((WIFI_STRENGTH_ENTITY + STATE_TOPIC).c_str(), rssiStr, true);

                lastRssi = currentRssi;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(WIFI_STRENGTH_TASK_INTERVAL)); // Update every 30 seconds
    }
}

void publishWifiStrengthDiscovery() {
    mqttClient.publish((WIFI_STRENGTH_ENTITY + DISCOVERY_TOPIC).c_str(), WIFI_STRENGTH_DISCOVERY_PAYLOAD.c_str(), true);
}

void setupWifiStrengthTask() {
    pinMode(DOOR_STATUS_PIN, INPUT); // Set wakeup pin as input
    xTaskCreate(
        wifiStrengthTask,
        "WifiStrengthTask",
        2048, // Stack size in words
        NULL,
        1, // Priority
        NULL // Store task handle in provided pointer
    );
    
    publishWifiStrengthDiscovery();
}