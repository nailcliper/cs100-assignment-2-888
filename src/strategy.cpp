#include "../header/strategy.h"
#include "../header/component.h"

#include <iostream>

void Container::addCommand(Component* command)
{
    commands.push_back(command);
}

void Container::runCommands()
{
    int size = commands.size();
    for(int i = 0; i < size; i++)
    {
        bool status = commands[i]->execute();
        std::cout << status << std::endl;
    }
}
