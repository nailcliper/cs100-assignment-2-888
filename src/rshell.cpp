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
#include "../header/parser.h"

using namespace std; 

int main()
{
    cout << "Running. --q to quit" << endl;
    do
    {
        string input;
        Parser* parser = new Parser;

        cout << "    $ ";
        getline(cin, input);
        parser->parseInput(input);
		
    } while(true);

    return 0;
}

