#include "task.h"
#include "../../secrets/secrets.h"

OTATask::OTATask(PubSubClient *client) : Task(client) {};

void OTATask::loop(unsigned long *ms)
{
    if (!Task::isConnected())
    {
        return;
    }

    Task::subscribe(OTA_ENTITY + COMMAND_TOPIC);
}

void OTATask::publishDiscovery()
{
    Task::publish((OTA_ENTITY + DISCOVERY_TOPIC), OTA_DISCOVERY_PAYLOAD);
}

bool OTATask::matchTopic(char *topic)
{
    return strcmp(topic, (OTA_ENTITY + COMMAND_TOPIC).c_str()) == 0;
}

void OTATask::msgHandler(char *topic, std::string message)
{
    if (strcmp(message.c_str(), OTA_UPDATE_COMMAND) != 0)
    {
        return;
    }

    if (!checkNewVersion())
    {
        return;
    }

    bool success = updateFireware();
    if (success)
    {
        Task::publish((OTA_ENTITY + COMMAND_TOPIC), OTA_UPDATE_NULL_COMMAND);
        ESP.restart();
    }
}

void OTATask::subscribe()
{
    Task::subscribe(OTA_ENTITY + COMMAND_TOPIC);
}

bool OTATask::checkNewVersion()
{
    client.begin(VERSION_URL.c_str());
    int httpCode = client.GET();
    if (httpCode == 200)
    {
        String newVersion = client.getString();
        newVersion.trim(); // remove whitespace/newlines
        Serial.printf("Current version: %s, New version: %s\n", VERSION, newVersion.c_str());
        client.end();
        return strcmp(newVersion.c_str(), VERSION) != 0;
    }

    client.end();
    Serial.println("Failed to get version info");

    return false;
}

bool OTATask::updateFireware()
{
    client.begin(FIRMWARE_URL.c_str());
    int httpCode = client.GET();
    if (httpCode != 200)
    {
        Serial.printf("HTTP GET failed with code %d\n", httpCode);
        client.end();
        return false;
    }

    int contentLength = client.getSize();
    if (contentLength <= 0)
    {
        Serial.printf("Invalid content length: %d\n", contentLength);
        client.end();
        return false;
    }

    bool canBegin = Update.begin(contentLength);
    if (!canBegin)
    {
        Serial.printf("Update.begin failed. Error #: %d\n", Update.getError());
        client.end();
        return false;
    }

    WiFiClient *stream = client.getStreamPtr();
    size_t written = Update.writeStream(*stream);
    if (written != contentLength)
    {
        Serial.printf("written %d bytes, expected %d bytes\n", written, contentLength);
        client.end();
        return false;
    }

    if (!Update.end())
    {
        Serial.printf("Update failed. Error #: %d\n", Update.getError());
        client.end();
        return false;
    }

    if (Update.isFinished())
    {
        Serial.println("Update successfully completed");
        return true;
    }
    else
    {
        Serial.println("Update not finished");
    }

    client.end();
    return false;
}