#include "../header/decorator.h"

bool AndConnector::execute()
{
    //TODO
    return true;
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
    //TODO
    return true;
}

void OrConnector::setLeft(Component* l)
{
    left = l;
}

void OrConnector::setRight(Component* r)
{
    right = r;
}
