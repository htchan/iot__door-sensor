#include "task.h"

DoorStatusTask::DoorStatusTask(PubSubClient *client) : Task(client) {};

void DoorStatusTask::setup()
{
    pinMode(DOOR_STATUS_PIN, INPUT);
}

void DoorStatusTask::loop(unsigned long *ms)
{
    readDoorStatus();

    if (Task::isConnected())
    {
        if (readIndex != publishIndex)
        {
            std::string status = doorStatusArray[publishIndex % DOOR_STATUS_TOTAL_COUNT] ? DOOR_STATUS_OPEN : DOOR_STATUS_CLOSE;
            Task::publish((DOOR_STATUS_ENTITY + STATE_TOPIC), status);
            publishIndex += 1;
        }
    }
}

void DoorStatusTask::publishDiscovery()
{
    Task::publish((DOOR_STATUS_ENTITY + DISCOVERY_TOPIC), DOOR_STATUS_DISCOVERY_PAYLOAD);
}

void DoorStatusTask::readDoorStatus()
{
    bool status = digitalRead(DOOR_STATUS_PIN) == HIGH ? true : false;
    if (readIndex == 0 || doorStatusArray[(readIndex - 1) % DOOR_STATUS_TOTAL_COUNT] != status)
    {
        doorStatusArray[readIndex % DOOR_STATUS_TOTAL_COUNT] = status;
        readIndex += 1;
    }
}