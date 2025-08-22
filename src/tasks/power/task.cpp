#include "task.h"
#include "../../utils/network/network.h"
#include "../../const.h"

float voltageValue;
unsigned long voltageArrayIndex = 0;

float readPowerValue() {
    // Replace with actual power reading logic
    return analogRead(SENSOR_POWER_PIN) * (3.3 / 4095.0) * POWER_VCC_MULTIPLIER;  // Example using ADC pin 34
}

void powerTask(void * parameter) {
    while(true) {
        voltageValue = 0;

        for(int i = 0; i < POWER_STATE_TOTAL_COUNT; i++) {
            voltageValue += readPowerValue();
            vTaskDelay(pdMS_TO_TICKS(200));  // 200ms between readings
        }
        voltageValue /= POWER_STATE_TOTAL_COUNT;
        
        if (mqttClient.connected()) {
            char stateValue[8];
            dtostrf(voltageValue, 2, 3, stateValue);
        
            bool result = mqttClient.publish((POWER_ENTITY + STATE_TOPIC).c_str(), stateValue, true);
        }

        vTaskDelay(pdMS_TO_TICKS(POWER_TASK_INTERVAL));    // Sleep for 10 seconds
    }
}

void publishPowerDiscovery() {
    mqttClient.publish((POWER_ENTITY + DISCOVERY_TOPIC).c_str(), POWER_DISCOVERY_PAYLOAD.c_str(), true);
}
void setupPowerTask() {
    pinMode(SENSOR_POWER_PIN, INPUT); // Set power sensor pin as input
    xTaskCreate(
        powerTask,
        "PowerTask",
        2048, // Stack size in words
        NULL,
        1, // Priority
        NULL // Store task handle in provided pointer
    );
    
    publishPowerDiscovery();
}
