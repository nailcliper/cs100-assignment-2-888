#include "../header/component.h"
#include "../header/common.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <signal.h>

bool Executable::execute()
{
    if(!exit_cmd)
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
            if(argv[0] == "--q")
            {
                exit_cmd = true;
                status = 2;
                exit(2);
            }
            if(!exit_cmd)
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
        }

        else if (pid > 0) //Parent Process
        {
            waitpid(pid, &status, WCONTINUED);
            if(status == 0)
                return true;
            if(WEXITSTATUS(status) == 2)
                exit_cmd = true;
            return false;
        }
    }
    return false;
}
