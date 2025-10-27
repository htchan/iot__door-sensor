#ifndef DEEP_SLEEP_TASK_H
#define DEEP_SLEEP_TASK_H

#include "../task.h"
#include "../door_status/task.h"
#include "const.h"
#include <climits>

class DeepSleepTask : public Task
{
public:
    DeepSleepTask(PubSubClient *, std::list<Task *> *tasks, DoorStatusTask *doorStatusTask);
    void loop(unsigned long *) override;
    void publishDiscovery() override;
    bool matchTopic(char *) override;
    void msgHandler(char *, std::string) override;
    void subscribe();

private:
    unsigned long long sleepDurationMs = ULONG_LONG_MAX;
    unsigned long connectedSince;
    DoorStatusTask *doorStatusTask;
    std::list<Task *> *tasks;
    void sleep(unsigned long long);
    bool allTasksCompleted();
    unsigned long long parseMsgValue(std::string);
};

#endif