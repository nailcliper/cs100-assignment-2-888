#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sstream>

#include "../header/parser.h"
#include "../header/component.h"
#include "../header/strategy.h"

void Parser::parseInput(std::string input, Container* container)
{
    char* str = new char [input.length()+1];
    strcpy(str, input.c_str());
    std::vector<char*> argv;
    char* pch = strtok(str, " ");
    while (pch != NULL)
    {
        argv.push_back(pch);
        pch = strtok(NULL, " ");
    }
    argv.push_back(NULL);

    Executable* exe = new Executable(argv);
    container->addCommand(exe);
}
