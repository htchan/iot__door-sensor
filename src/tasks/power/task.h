#ifndef POWER_TASK_H
#define POWER_TASK_H

#include "../task.h"
#include "const.h"

class PowerTask : public Task
{
public:
    PowerTask(PubSubClient *);
    void loop(unsigned long *) override;
    void publishDiscovery() override;
    bool matchTopic(char *) override;
    void msgHandler(char *, std::string) override;
    bool canDeepSleep() override { return powerPublishedAt > 0; }
    void subscribe();

private:
    unsigned long powerReadAt = 0;
    unsigned long powerPublishedAt = 0;
    float voltageArray[POWER_STATE_TOTAL_COUNT];
    unsigned long voltageArrayIndex = 0;
    unsigned long lowPowerThresholdSubscribedAt = 0;
    void readData(unsigned long *);
    void publishState(unsigned long *);
};

#endif