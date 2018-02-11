#include "../header/component.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <signal.h>

bool Executable::execute()
{
    int status = 1;
    pid_t pid = fork();
    if(pid == -1) //Fork Failed
    {
        perror("Fork Error");
        exit(-1);
    }

    else if (pid == 0) //Child Process
    {
        std::vector<char*> argc;
        int vec_size = argv.size();
        for(int i = 0; i < vec_size; i++)
        {
            argc.push_back(const_cast<char*>(argv[i].c_str()));
        }
        argc.push_back(0);
        char **args = &argc[0];

        execvp(args[0], args);
        std::cout << "Unrecognized Command: " << args[0] << std::endl;
        status = 1;
        exit(1);
    }

    else if (pid > 0) //Parent Process
    {
        if(waitpid(pid, &status, WCONTINUED) > 0)
        {
            /*
            if(WIFEXITED(status))
            {
                std::cout << "Child exited with a status of ";
                std::cout << WEXITSTATUS(status) << std::endl;
            }
            if(WIFSIGNALED(status))
            {
                std::cout << "Child exited via signal ";
                std::cout << WTERMSIG(status) << std::endl;
            }
            */
        }
        if(status == 0)
            return true;
        return false;
    }

    return false;
}
