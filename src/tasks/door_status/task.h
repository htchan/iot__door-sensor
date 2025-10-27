#ifndef DOOR_STATUS_TASK_H
#define DOOR_STATUS_TASK_H

#include "../task.h"
#include "const.h"

class DoorStatusTask : public Task
{
public:
    DoorStatusTask(PubSubClient *);
    void setup() override;
    void loop(unsigned long *) override;
    void publishDiscovery() override;
    bool canDeepSleep() override
    {
        return readIndex > 0 && readIndex == publishIndex;
    }
    bool latestStatus() { return doorStatusArray[readIndex - 1 % DOOR_STATUS_TOTAL_COUNT]; }

private:
    bool doorStatusArray[DOOR_STATUS_TOTAL_COUNT];
    int readIndex = 0;
    int publishIndex = 0;
    void readDoorStatus();
};

#endif