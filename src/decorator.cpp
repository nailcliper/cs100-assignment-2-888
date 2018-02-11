#include "../header/decorator.h"

bool NextConnector::execute()
{
    left->execute();
    if(right->execute())
        return true;
    return false;
}

void NextConnector::setLeft(Component* l)
{
    left = l;
}

void NextConnector::setRight(Component* r)
{
    right = r;
}

bool AndConnector::execute()
{
    if(left->execute())
    {
        if(right->execute())
            return true;
        return false;
    }
    return false;
}

void AndConnector::setLeft(Component* l)
{
    left = l;
}

void AndConnector::setRight(Component* r)
{
    right = r;
}

bool OrConnector::execute()
{
    if(left->execute())
        return true;
    if(right->execute())
        return true;
    return false;
}

void OrConnector::setLeft(Component* l)
{
    left = l;
}

void OrConnector::setRight(Component* r)
{
    right = r;
}
