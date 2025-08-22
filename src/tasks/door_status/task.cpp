#include "./task.h"
#include "../../utils/network/network.h"

QueueHandle_t publishDoorStatusQueue = NULL;
int doorStatusHistory[DOOR_STATUS_HISTORY_SIZE];
uint8_t historyCount = 0;
uint8_t publishCount = 0;

void enablePublishDoorStatusTask() {
    uint8_t signal = 1; // Example signal to send
    xQueueSend(publishDoorStatusQueue, &signal, 0);
}

void readDoorStatusTask(void * parameter) {
    while(true) {
        int doorStatus = digitalRead(DOOR_STATUS_PIN);
        Serial.printf("Door status: %d\n", doorStatus);
        // mark history only when the door status changes
        uint8_t historyIndex = historyCount % DOOR_STATUS_HISTORY_SIZE;
        uint8_t lastHistoryIndex = (historyCount - 1) % DOOR_STATUS_HISTORY_SIZE;
        if (historyCount == 0 || doorStatus != doorStatusHistory[lastHistoryIndex]) {
            doorStatusHistory[historyIndex] = doorStatus;
            historyCount++;
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void publishDoorStatusTask(void * parameter) {
    uint8_t signal;
    
    publishDoorStatusQueue = xQueueCreate(1, sizeof(uint8_t));
    xQueueReceive(publishDoorStatusQueue, &signal, portMAX_DELAY);

    while (true) {
        if (mqttClient.connected() && publishCount < historyCount) {
            // Prepare the door status payload
            uint8_t publishIndex = publishCount % DOOR_STATUS_HISTORY_SIZE;
            if (doorStatusHistory[publishIndex] == LOW) {
                mqttClient.publish((DOOR_STATUS_ENTITY + STATE_TOPIC).c_str(), DOOR_STATUS_OPEN.c_str(), true);
            } else {
                mqttClient.publish((DOOR_STATUS_ENTITY + STATE_TOPIC).c_str(), DOOR_STATUS_CLOSE.c_str(), true);
            }

            publishCount++;
        } 

        vTaskDelay(pdMS_TO_TICKS(DOOR_STATUS_TASK_INTERVAL)); // Sleep for the defined interval
    }
}

void publishDoorStatusDiscovery() {
    mqttClient.publish((DOOR_STATUS_ENTITY + DISCOVERY_TOPIC).c_str(), DOOR_STATUS_DISCOVERY_PAYLOAD.c_str(), true);
}

void setupDoorStatusTask() {
    pinMode(DOOR_STATUS_PIN, INPUT); // Set wakeup pin as input
    xTaskCreate(
        readDoorStatusTask,
        "DoorStatusTask",
        2048, // Stack size in words
        NULL,
        1, // Priority
        NULL // Store task handle in provided pointer
    );
    xTaskCreate(
        publishDoorStatusTask,
        "DoorStatusTask",
        2048, // Stack size in words
        NULL,
        1, // Priority
        NULL // Store task handle in provided pointer
    );
}