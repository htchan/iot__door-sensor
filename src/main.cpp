#include <Arduino.h>
#include "./const.h"

#include "./tasks/available/task.h"
#include "./tasks/deep_sleep/task.h"
#include "./tasks/door_status/task.h"
#include "./tasks/network/task.h"
#include "./tasks/power/task.h"
#include "./tasks/uptime/task.h"
#include "./tasks/wifi_strength/task.h"

std::list<Task *> tasks;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

AvailableTask availableTask(&client);
DoorStatusTask doorStatusTask(&client);
DeepSleepTask deepSleepTask(&client, &tasks, &doorStatusTask);
PowerTask powerTask(&client);
UptimeTask uptimeTask(&client);
WifiStrengthTask wifiStrengthTask(&client);

bool alreadyPublished = false;

void networkConnected()
{
  unsigned long ms = millis();
  Serial.printf("Network connected at %lu\n", ms);
  if (!alreadyPublished)
  {
    for (auto &task : tasks)
    {
      task->publishDiscovery();
    }

    alreadyPublished = true;
  }

  powerTask.subscribe();
  deepSleepTask.subscribe();
}

void subscribeHandler(char *topic, byte *payload, unsigned int length)
{
  std::string message(reinterpret_cast<char *>(payload), length);

  for (auto &task : tasks)
  {
    if (task->matchTopic(topic))
    {
      task->msgHandler(topic, message);
    }
  }
}

NetworkTask networkTask(&client, networkConnected, subscribeHandler);

void setup()
{
  Serial.begin(9600);
  Serial.println("Testing - v0.0.1");
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  tasks.push_back(&doorStatusTask);
  tasks.push_back(&powerTask);
  tasks.push_back(&availableTask);
  tasks.push_back(&uptimeTask);
  tasks.push_back(&deepSleepTask);
  tasks.push_back(&networkTask);
  tasks.push_back(&wifiStrengthTask);

  for (auto &task : tasks)
  {
    task->setup();
  }
}

void loop()
{
  unsigned long ms = millis();

  for (auto &task : tasks)
  {
    task->loop(&ms);
  }

  delay(LOOP_INTERVAL_MS - (millis() % LOOP_INTERVAL_MS));
}
