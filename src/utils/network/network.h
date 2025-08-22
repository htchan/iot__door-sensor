#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

extern WiFiClient espClient;
extern PubSubClient mqttClient;

typedef void (*WiFiConnectedCallback)();

struct NetworkTaskParams {
    WiFiConnectedCallback onConnected;
};

void setupNetworkTask(NetworkTaskParams* params);
void publishAtomizerDiscovery();
// void networkTask(void * parameter);

#endif