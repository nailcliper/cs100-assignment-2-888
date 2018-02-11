/*
Jonathan Oaks
SID: 861303298

This program will run a loop prompting the user for bash commands
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "../header/component.h"
#include "../header/decorator.h"
#include "../header/strategy.h"
#include "../header/parser.h"

using namespace std;

int main()
{
    string input;
    Parser* parser = new Parser;
    Container* container = new Container;

    cout << "Running" << endl << "$ ";
    getline(cin, input);
    parser->parseInput(input, container);

    container->runCommands();

    return 0;
}

