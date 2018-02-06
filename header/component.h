#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <vector>

class Component
{
public:
    Component() {}
    virtual bool execute() = 0;
};

class Executable : public Component
{
private:
    std::vector<char*> argv;
public:
    Executable(std::vector<char*> argument_vector) : argv(argument_vector) {}
    bool execute();
};

#endif //__COMPONENT_H__
