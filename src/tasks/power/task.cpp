#include "task.h"

PowerTask::PowerTask(PubSubClient *client) : Task(client) {};

void PowerTask::loop(unsigned long *ms)
{
    if (*ms / POWER_UPDATE_INTERVAL_MS <= powerReadAt / POWER_UPDATE_INTERVAL_MS)
    {
        return;
    }

    readData(ms);

    if (Task::isConnected())
    {
        publishState(ms);
    }
}

void PowerTask::publishDiscovery()
{
    Task::publish((POWER_ENTITY + DISCOVERY_TOPIC), POWER_DISCOVERY_PAYLOAD);
    Task::publish((LOW_POWER_THRESHOLD_ENTITY + DISCOVERY_TOPIC), LOW_POWER_THRESHOLD_DISCOVERY_PAYLOAD);
}

bool PowerTask::matchTopic(char *topic)
{
    return (strcmp(topic, (LOW_POWER_THRESHOLD_ENTITY + COMMAND_TOPIC).c_str()) == 0);
}

void PowerTask::msgHandler(char *topic, std::string message)
{
    float threshold = atof(message.c_str());
    if (threshold < 3.0 || threshold > 4.5)
    {
        return;
    }

    Task::publish((LOW_POWER_THRESHOLD_ENTITY + STATE_TOPIC), message);
}

void PowerTask::subscribe()
{
    Task::subscribe(LOW_POWER_THRESHOLD_ENTITY + COMMAND_TOPIC);
}

void PowerTask::readData(unsigned long *ms)
{
    powerReadAt = *ms;

    int adcValue = analogRead(1);                                                                         // Read the ADC value from A0
    voltageArray[voltageArrayIndex % POWER_STATE_TOTAL_COUNT] = (POWER_VCC_MULTIPLIER * adcValue) / 4096; // Calculate VCC based on the ADC value
    voltageArrayIndex += 1;
}

void PowerTask::publishState(unsigned long *ms)
{
    if (voltageArrayIndex % POWER_STATE_SEND_EVERY != 0)
    {
        return;
    }

    int count = 0;
    float sum = 0, min = 0, max = 0;
    for (int i = 0; i < POWER_STATE_TOTAL_COUNT; i++)
    {
        float value = voltageArray[i];
        if (value <= 0)
        {
            continue;
        }

        sum += value;
        count += 1;
        if (min == 0 || value < min)
        {
            min = value;
        }

        if (max == 0 || value > max)
        {
            max = value;
        }
    }

    if (count == 0)
    {
        return;
    }
    else if (count <= 2)
    {
        min = max = 0;
        count += 2;
    }

    float resultVoltage = (sum - min - max) / (count - 2);

    char stateValue[8];
    dtostrf(resultVoltage, 2, 3, stateValue);

    Task::publish((POWER_ENTITY + STATE_TOPIC), stateValue);
    powerPublishedAt = *ms;
}