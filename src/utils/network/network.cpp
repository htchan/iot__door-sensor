#include "network.h"
#include "../../secrets/secrets.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long connectedAt = 0;

void networkTask(void * parameter) {
  NetworkTaskParams* params = (NetworkTaskParams*)parameter;
  WiFi.mode(WIFI_STA);

  // Print MAC Address
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.printf("WiFi MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  while(true) {
    // Handle WiFi connection
    Serial.printf("WiFi status: %d; MQTT status: %d\n", WiFi.isConnected(), mqttClient.connected());
    if(!WiFi.isConnected()) {
      WiFi.begin(WIFI_CONN_SSID.c_str(), WIFI_CONN_PASSWORD.c_str());
      
      for (uint8_t i = 0; i < 10; i++) {
        if (WiFi.isConnected()) { break; }
        Serial.print(".");
        vTaskDelay(pdMS_TO_TICKS(500));
      }

      if (!WiFi.isConnected()) {
        WiFi.disconnect();
        vTaskDelay(pdMS_TO_TICKS(1000));
        continue;
      }
      
      Serial.println("\nConnected to WiFi!");
      WiFi.setSleep(true);
      Serial.println("WiFi power saving mode enabled");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }

    // Handle MQTT connection
    if (WiFi.isConnected() && !mqttClient.connected()) {

      char clientID[32];
      snprintf(clientID, sizeof(clientID), "%s%04X", MQTT_CLIENT_ID_PREFIX.c_str(), (uint16_t)(millis() & 0xFFFF));
    
      mqttClient.setServer(MQTT_SERVER.c_str(), MQTT_PORT);
      mqttClient.connect(clientID, MQTT_USER.c_str(), MQTT_PASSWORD.c_str());
    }

    if (mqttClient.connected() && connectedAt == 0) {
      connectedAt = millis();

      if (params->onConnected != nullptr) {
          params->onConnected();
      }
    }

    mqttClient.loop();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setupNetworkTask(NetworkTaskParams* params) {
  // Initialize WiFi and MQTT client
  mqttClient.setServer(MQTT_SERVER.c_str(), MQTT_PORT);
  
  xTaskCreate(
      networkTask,
      "WifiConnectionTask",
      4096, // Stack size in words
      params,
      1, // Priority
      NULL
  );
}