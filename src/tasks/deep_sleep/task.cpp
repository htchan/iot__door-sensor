#include "task.h"
#include "../door_status/task.h"
#include "../uptime/task.h"

QueueHandle_t deepSleepQueue = NULL;

void enableDeepSleepTask() {
    uint8_t signal = 1; // Example signal to send
    xQueueSend(deepSleepQueue, &signal, 0);
}

void deepSleepTask(void * parameter) {
    uint8_t signal;

    deepSleepQueue = xQueueCreate(1, sizeof(uint8_t));
    xQueueReceive(deepSleepQueue, &signal, portMAX_DELAY);

    while(true) {
      if (publishCount == 0 || publishCount != historyCount) {
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for the door status to be published
        continue;
      }

      Serial.println("Deep sleep task triggered, entering deep sleep...");
      publishUptime(); // Publish uptime before going to sleep
      // wake up device again when the input change again
      if (digitalRead(DOOR_STATUS_PIN) == HIGH) {
        Serial.println("Door status pin is HIGH, setting wakeup condition to LOW");
        esp_deep_sleep_enable_gpio_wakeup(1ULL << DOOR_STATUS_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
      } else {
        Serial.println("Door status pin is LOW, setting wakeup condition to HIGH");
        esp_deep_sleep_enable_gpio_wakeup(1ULL << DOOR_STATUS_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
      }
      
      // Configure deep sleep
      esp_sleep_enable_timer_wakeup(3600ULL * 1000000ULL); // 1 minute in microseconds
      
      vTaskDelay(pdMS_TO_TICKS(100));
      esp_deep_sleep_start();
      // Will never reach here
      vTaskDelete(NULL);
    }
}

void setupDeepSleepTask() {
    pinMode(DOOR_STATUS_PIN, INPUT); // Set wakeup pin as input
    xTaskCreate(
        deepSleepTask,
        "DeepSleepTask",
        4096,  // Increased stack size
        NULL,
        1,    // Increased priority
        NULL // Store task handle in provided pointer
    );
}
