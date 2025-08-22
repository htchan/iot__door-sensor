#include <Arduino.h>
#include <WiFi.h>
#include "./const.h"
#include "./tasks/deep_sleep/task.h"
#include "./tasks/door_status/task.h"
#include "./tasks/wifi_strength/task.h"
#include "./tasks/uptime/task.h"
#include "./tasks/power/task.h"
#include "./utils/network/network.h"


void onWiFiConnected() {
  Serial.println("WiFi connected callback started");
  publishWifiStrengthDiscovery();
  publishPowerDiscovery();
  publishDoorStatusDiscovery();
  publishUptimeDiscovery();

  uint8_t signal = 1;
  xQueueSend(wifiStrengthQueue, &signal, 0);
  xQueueSend(uptimeQueue, &signal, 0);
  enablePublishDoorStatusTask();
  enableDeepSleepTask(); // Enable deep sleep task after WiFi connection

  publishUptime();
  Serial.println("WiFi connected callback ended");
}

void testLightTask(void * parameter) {
  while (true) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
  }
}

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate
  mqttClient.setBufferSize(2048);
  Serial.flush();
  Serial.println("Starting setup...");
  
  NetworkTaskParams* networkParams = new NetworkTaskParams{
    .onConnected = onWiFiConnected
  };

  setupNetworkTask(networkParams); // Initialize network task with default parameters

  pinMode(LED_PIN, OUTPUT); // Set LED pin as output
  xTaskCreate(
    testLightTask,
    "LightTask",
    2048, // Stack size in words
    NULL,
    1, // Priority
    NULL // Store task handle in tasks array
  );
  
  setupDeepSleepTask();
  setupDoorStatusTask();
  setupWifiStrengthTask();
  setupUptimeTask();
  setupPowerTask();

  // xTaskCreate(
  //     networkTask,
  //     "WifiConnectionTask",
  //     2048, // Stack size in words
  //     networkParams,
  //     1, // Priority
  //     NULL
  // );
  // // Enable the deep sleep task
  // enableDeepSleepTask();
}

void loop() {
}
