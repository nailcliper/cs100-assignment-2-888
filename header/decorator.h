#ifndef __DECORATOR_H__
#define __DECORATOR_H__

#include "component.h"

class Decorator : public Component
{
private:
    Component* left;
    Component* right;
public:
    Decorator() {}
    virtual bool execute() = 0;
    virtual void setLeft(Component* l) = 0;
    virtual void setRight(Component* r) = 0;
};

class NextConnector: public Decorator
{
private:
    Component* left;
    Component* right;
public:
    NextConnector() {}
    bool execute();
    void setLeft(Component* l);
    void setRight(Component* r);
};

class AndConnector : public Decorator
{
private:
    Component* left;
    Component* right;
public:
    AndConnector() {}
    bool execute();
    void setLeft(Component* l);
    void setRight(Component* r);
};

class OrConnector : public Decorator
{
private:
    Component* left;
    Component* right;
public:
    OrConnector() {}
    bool execute();
    void setLeft(Component* l);
    void setRight(Component* r);
};

#endif //__DECORATOR_H__
