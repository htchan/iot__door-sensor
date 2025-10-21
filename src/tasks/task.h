#ifndef META_TASK_H
#define META_TASK_H

#include <PubSubClient.h>
#include <string>
#include <list>

class Task
{
public:
    Task(PubSubClient *);
    virtual void setup() {};
    virtual void cleanup() {};
    virtual void loop(unsigned long *ms) {};
    virtual void publishDiscovery() {};
    virtual bool matchTopic(char *) { return false; };
    virtual void msgHandler(char *, std::string) {};
    virtual bool canDeepSleep() { return true; };

protected:
    bool isConnected();
    void publish(std::string, std::string);
    void subscribe(std::string);
    PubSubClient *client;
};

extern bool isSameInterval(unsigned long time1, unsigned long time2, unsigned long interval);

#endif