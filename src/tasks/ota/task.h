#ifndef OTA_TASK_H
#define OTA_TASK_H

#include "../task.h"
#include "const.h"
#include <HTTPClient.h>
#include <Update.h>

class OTATask : public Task
{
public:
    OTATask(PubSubClient *);
    void loop(unsigned long *) override;
    void publishDiscovery() override;
    bool matchTopic(char *) override;
    void msgHandler(char *, std::string) override;
    void subscribe();

private:
    HTTPClient client;
    bool checkNewVersion();
    bool updateFireware();
};

#endif