#include "task.h"
#include "../../utils/network/network.h"

QueueHandle_t uptimeQueue = NULL;

unsigned long readUptime() {
    return millis();  // Returns number of milliseconds since start
}

void publishUptime() {
    if (mqttClient.connected()) {
        unsigned long currentUptime = readUptime();
        
        char uptimeStr[16];
        snprintf(uptimeStr, sizeof(uptimeStr), "%lu", currentUptime);

        mqttClient.publish((UPTIME_ENTITY + STATE_TOPIC).c_str(), uptimeStr, true);
    }
}

void uptimeTask(void * parameter) {
    // Wait for start signal
    uint8_t signal;
    uptimeQueue = xQueueCreate(1, sizeof(uint8_t));
    xQueueReceive(uptimeQueue, &signal, portMAX_DELAY);

    Serial.printf("check connection: %d\n", mqttClient.connected());
    while (!mqttClient.connected()) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        Serial.printf("check connection: %d\n", mqttClient.connected());
    }

    Serial.println("uptime task start");
    while(true) {
        publishUptime();
        
        vTaskDelay(pdMS_TO_TICKS(UPTIME_PUBLISH_INTERVAL_MS)); // Check every 5 seconds
    }
}

void publishUptimeDiscovery() {
    mqttClient.publish((UPTIME_ENTITY + DISCOVERY_TOPIC).c_str(), UPTIME_DISCOVERY_PAYLOAD.c_str(), true);
}


void setupUptimeTask() {
    pinMode(DOOR_STATUS_PIN, INPUT); // Set wakeup pin as input
    xTaskCreate(
        uptimeTask,
        "UptimeTask",
        2048, // Stack size in words
        NULL,
        1, // Priority
        NULL
    );
    
    publishUptimeDiscovery();
}