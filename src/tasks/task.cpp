#include "task.h"

Task::Task(PubSubClient *c) : client(c) {};

bool Task::isConnected()
{
    return client->connected();
}

void Task::publish(std::string topic, std::string payload)
{
    client->publish(topic.c_str(), payload.c_str(), true);
}

void Task::subscribe(std::string topic)
{
    client->subscribe(topic.c_str());
}

bool isSameInterval(unsigned long time1, unsigned long time2, unsigned long interval)
{
    return time1 / interval == time2 / interval;
}