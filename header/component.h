#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <vector>
#include <string>

class Component
{
public:
    Component() {}
    virtual bool execute() = 0;
};

class Executable : public Component
{
private:
    std::vector<std::string> argv;
public:
    Executable(std::vector<std::string> argument_vector) : argv(argument_vector) {}
    bool execute();
};

class Quit : public Component
{
public:
    Quit() {}
    bool execute();
};

class Test : public Component
{
private:
    std::vector<std::string> argv;
public:
    Test(std::vector<std::string> argument_vector) : argv(argument_vector) {}
    bool execute();
};

#endif //__COMPONENT_H__
