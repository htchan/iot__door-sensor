#include "task.h"

DeepSleepTask::DeepSleepTask(PubSubClient *client, std::list<Task *> *tasks, DoorStatusTask *doorStatusTask) : Task(client), tasks(tasks), doorStatusTask(doorStatusTask) {}

bool DeepSleepTask::allTasksCompleted()
{
    for (auto &task : *tasks)
    {
        if (!task->canDeepSleep())
        {
            return false;
        }
    }

    return true;
}

void DeepSleepTask::loop(unsigned long *ms)
{

    // record connected time
    if (connectedSince == 0 && Task::isConnected())
    {
        connectedSince = *ms;
    }

    // force sleep if device up for specific duration
    if (*ms > OFFLINE_FORCE_SLEEP_DURATION_MS)
    {
        Serial.printf("force offline sleep at %lu\n", *ms);
        sleep(DEFAULT_SLEEP_DURATION_MS);
        return;
    }

    if (sleepDurationMs != ULONG_LONG_MAX && Task::isConnected())
    {
        // there is a more restricted length for device up duration if it connected to wifi
        if (connectedSince != 0 && *ms - connectedSince > ONLINE_FORCE_SLEEP_DURATION_MS)
        {
            Serial.printf("force online sleep at %lu\n", *ms);
            sleep(sleepDurationMs);
            return;
        }

        // sleep should be applied earlier if all tasks are completed
        if (allTasksCompleted())
        {
            Serial.printf("all task completed sleep at %lu\n", *ms);
            sleep(sleepDurationMs);
            return;
        }
    }
}

void DeepSleepTask::sleep(unsigned long sleepInterval)
{
    for (auto &task : *tasks)
    {
        task->cleanup();
    }

    Serial.printf("deep sleep for %lu\n", sleepInterval);
    if (doorStatusTask->latestStatus())
    {
        esp_deep_sleep_enable_gpio_wakeup(1ULL << DOOR_STATUS_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
    }
    else
    {
        esp_deep_sleep_enable_gpio_wakeup(1ULL << DOOR_STATUS_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
    }

    esp_sleep_enable_timer_wakeup(sleepInterval * 1000);
    esp_deep_sleep_start();
}

void DeepSleepTask::publishDiscovery()
{
    Task::publish((UPDATE_FREQUENCY_ENTITY + DISCOVERY_TOPIC), UPDATE_FREQUENCY_DISCOVERY_PAYLOAD);
    Task::publish((SLEEP_DURATION_ENTITY + DISCOVERY_TOPIC), SLEEP_DURATION_DISCOVERY_PAYLOAD);
}

bool DeepSleepTask::matchTopic(char *topic)
{
    return (strcmp(topic, (UPDATE_FREQUENCY_ENTITY + STATE_TOPIC).c_str()) == 0);
}

int DeepSleepTask::parseMsgValue(std::string msg)
{
    int value = 30, unit = 1;
    int pos = msg.find(" ");
    std::string valueStr = msg.substr(0, pos);
    std::string unitStr = msg.substr(pos + 1);
    if (unitStr == "min")
    {
        unit = 60;
    }
    else if (unitStr == "hr")
    {
        unit = 3600;
    }

    value = std::stoi(valueStr);

    return value * unit;
}

void DeepSleepTask::msgHandler(char *topic, std::string message)
{
    if (strcmp(message.c_str(), "No Sleep") == 0)
    {
        sleepDurationMs = ULONG_MAX;
        Task::publish((SLEEP_DURATION_ENTITY + STATE_TOPIC), std::to_string(0));
        Task::publish((UPDATE_FREQUENCY_ENTITY + STATE_TOPIC), message);

        return;
    }

    int value = parseMsgValue(message);

    sleepDurationMs = value * 1000;

    Task::publish((SLEEP_DURATION_ENTITY + STATE_TOPIC), std::to_string(value));
    Task::publish((UPDATE_FREQUENCY_ENTITY + STATE_TOPIC), message);
}

void DeepSleepTask::subscribe()
{
    Task::subscribe((UPDATE_FREQUENCY_ENTITY + STATE_TOPIC).c_str());
}