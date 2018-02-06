#include "../header/component.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>

bool Executable::execute()
{
    char** arg = &argv[0];

    execvp(arg[0], arg);
    std::cout << "Unrecognized Command: " << arg[0] << std::endl;
    return false;
}
