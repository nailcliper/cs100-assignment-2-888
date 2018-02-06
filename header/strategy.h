#ifndef __STRATEGY_H__
#define __STRATEGY_H__

#include <vector>

class Component;

class Container
{
private:
    std::vector<Component*> commands;
public:
    void addCommand(Component* command);
    void runCommands();
};

#endif //__STRATEGY_H__
