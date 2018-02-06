/*
Jonathan Oaks
SID: 861303298

This program will run a loop prompting the user for bash commands
*/

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
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
    cout << "Running" << endl << "$ ";
    getline(cin, input);

    Parser* parser = new Parser;
    Container* container = new Container;
    parser->parseInput(input, container);

    pid_t pid = fork();
    
    if (pid == -1) //Fork Failed
    {
        perror("fork error");
        return 0;
    }

    else if (pid == 0) //Child Process
    {
        cout << "child: " << pid << endl;
        container->runCommands();
        cout << "Unkown command" << endl;
        return 0;
        
    }
    else if (pid > 0) //Parent Process
    {
        cout << "parent waiting " << pid << endl;
        if(waitpid(pid, 0, WCONTINUED) == 0)
        {
            perror("wait error");
        }
        cout << "parent: " << pid << endl;
    }

    return 1;
}

