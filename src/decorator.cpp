#include "../header/decorator.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <fstream>
#include <fcntl.h>

using namespace std;

bool NextConnector::execute()
{
    if(left)
        left->execute();
    if(right)
    {
        if(right->execute())
            return true;
    }
    return false;
}

void NextConnector::setLeft(Component* l)
{
    left = l;
}

void NextConnector::setRight(Component* r)
{
    right = r;
}

bool AndConnector::execute()
{
    if(left)
    {
        if(left->execute())
        {
            if(right)
            {
                if(right->execute())
                    return true;
            }
            return false;
        }
    }
    return false;
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
    if(left)
    {
        if(left->execute())
            return true;
    }

    if(right)
    {
        if(right->execute())
            return true;
    }    
    return false;
}

void OrConnector::setLeft(Component* l)
{
    left = l;
}

void OrConnector::setRight(Component* r)
{
    right = r;
}

bool InputRedirector::execute()
{
    int status = 1;
    pid_t pid = fork();
    if(pid == -1) //Fork Failed
    {
        perror("fork Error");
        exit(-1);
    }
    else if(pid == 0) //Child Process
    {
		close(0); //stdin
        open(filename.c_str(),O_RDONLY); //Opens file at fd 0
		
		if(left->execute())
		{
			exit(0);
		}
		exit(1);
    }
    else if(pid > 0) //Parent Process
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

void InputRedirector::setLeft(Component* l)
{
    left = l;
}

void InputRedirector::setRight(Component*r)
{
    right = r;
}

void InputRedirector::setFile(std::string name)
{
    filename = name;
}

bool OutputRedirector::execute()
{   
    int status = 1;
    pid_t pid = fork();
    if(pid == -1) //Fork Failed
    {
        perror("fork Error");
        exit(-1);
    }
    else if(pid == 0) //Child Process
    {
		close(1); //stdout
		if(flag == ">")
		{
			open(filename.c_str(),O_CREAT|O_WRONLY|O_TRUNC,0664); //Opens file at fd 1, truncate to 0
		}
		else //flag == ">>"
		{
			open(filename.c_str(),O_CREAT|O_WRONLY,0664); //Open file at fd 1
			lseek(1,0,SEEK_END);
		}
		
		if(left->execute() && right->execute())
		{
			exit(0);
		}
		exit(1);
    }
    else if(pid > 0) //Parent Process
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

void OutputRedirector::setLeft(Component* l)
{
    left = l;
}

void OutputRedirector::setRight(Component*r)
{
    right = r;
}
void OutputRedirector::setFile(std::string name)
{
    filename = name;
}

bool Piper::execute()
{
    //TODO
	int fd[2];
	pipe(fd);
	
    int status = 1;
    pid_t pid = fork();
    if(pid == -1) //Fork Failed
    {
        perror("fork Error");
        exit(-1);
    }
    else if(pid == 0) //Child Process
    {
		close(fd[0]); //close unused read pipe
		dup2(fd[1],1); //dup write pipe to 1(stdout)
		bool success = left->execute();
		close(1);
		if(success)
		{
			exit(0);
		}
		exit(1);
    }
    else if(pid > 0) //Parent Process
    {
        if(waitpid(pid, &status, WCONTINUED) < 0)
        {
            perror("waitpid Failed");
            exit(-1);
        }
        if(status == 0)
		{
			close(fd[1]); //close unused write pipe
			int stdin_store = dup(0);
			dup2(fd[0],0); //dupes read-pipe to 0
			bool success = right->execute();
			dup2(stdin_store,0);
			if(success)
			{
				return true;
			}
		}
        return false;
    }
	
    return false;
}

void Piper::setLeft(Component* l)
{
    left = l;
}

void Piper::setRight(Component* r)
{
    right = r;
}
