#include "../header/component.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

bool Executable::execute()
{
    int status = 1;
    pid_t pid = fork();
    if(pid == -1) //Fork Failed
    {
        perror("fork Error");
        exit(-1);
    }

    else if (pid == 0) //Child Process
    {
        vector<char*> argc;
        int vec_size = argv.size();
        for(int i = 0; i < vec_size; i++)
        {
            argc.push_back(const_cast<char*>(argv[i].c_str()));
        }
        argc.push_back(0);
        char **args = &argc[0];

        if(execvp(args[0], args) < 0);
        {
            perror("execvp Failed");
            exit(-1);
        }
        cout << "Unrecognized Command: " << args[0] << std::endl;
        status = 1;
        exit(1);
    }

    else if (pid > 0) //Parent Process
    {
        if(waitpid(pid, &status, WCONTINUED) < 0)
        {
            perror("waitpid Failed");
            exit(-1);
        }
        if(status == 0)
            return true;
        return false;
    }
    return false;
}

bool Quit::execute()
{
    exit(0); //Exits the main process
    return false;
}

bool Test::execute()
{
    if(argv[0] == "[")
    {
        if(argv.back() == "]")
            argv.pop_back();
    }
    argv.erase(argv.begin());

    string flag = argv[0];
    if(flag == "-e" || flag == "-f" || flag == "-d")
        argv.erase(argv.begin());

    struct stat buf;
    char *cstr = new char[argv[0].length() + 1];
    strcpy(cstr, argv[0].c_str());
    stat(cstr, &buf);
    if(S_ISREG(buf.st_mode))
    {
        if(flag == "-d")
        {
            cout << "(false)" << endl;
            return false;
        }
        cout << "(true)" << endl;
        return true;
    }
    else if(S_ISDIR(buf.st_mode))
    {
        if(flag == "-f")
        {
            cout << "(false)" << endl;
            return false;
        }
        cout << "(true)" << endl;
        return true;
    }
    cout << "(false)" << endl;
    return false;
}
