#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <list>
#include <iterator>
#include <sstream>
#include <boost/tokenizer.hpp>

#include "../header/parser.h"
#include "../header/component.h"
#include "../header/strategy.h"
#include "../header/decorator.h"

void Parser::parseInput(std::string input, Container* container)
{
    
    //Split input string by " and ', keep delimiters
    boost::char_separator<char> string_sep("","\"\'");
    boost::tokenizer< boost::char_separator<char> > str_tok(input, string_sep);
    boost::tokenizer< boost::char_separator<char> >::iterator it;

    std::list<std::string> token_list;
    std::list<std::string>::iterator list_it;
    for(it = str_tok.begin(); it != str_tok.end(); ++it)
    {
        token_list.push_back(*it);
    }

    //Reconstruct compunded strings into one token
    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        std::string token = *list_it;
        if(token == "\"" || token == "\'")
        {
            ++list_it;
            if(*list_it != token)
            {
                std::string master = *list_it;
                std::list<std::string>::iterator temp_it = list_it;
                ++temp_it;
                while(*temp_it != token)
                {
                    master = master + *temp_it;
                    token_list.erase(temp_it);
                    temp_it = list_it;
                    ++temp_it;
                }
                *list_it = master;
                ++list_it;
            }
        }
    }

    //Iterate over non-" X " type tokens, splitting by " ;()", keeping ";()"
    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        std::string token = *list_it;
        if(token == "\"" || token == "\'") //Skip string tokens and remove the markers
        {
            std::list<std::string>::iterator left_it = list_it;
            ++list_it;
            std::list<std::string>::iterator right_it = list_it;
            ++right_it;
            token_list.erase(left_it);
            token_list.erase(right_it);
        }
        else
        {
            boost::char_separator<char> cmd_sep(" ",";()");
            boost::tokenizer< boost::char_separator<char> > cmd_tok(*list_it, cmd_sep);
    boost::tokenizer< boost::char_separator<char> >::iterator cmd_it;

            for(cmd_it = cmd_tok.begin(); cmd_it != cmd_tok.end(); ++cmd_it)
            {
                std::string cmd = *cmd_it;
                token_list.insert(list_it,*cmd_it);
                
                if(cmd.at(0) == '#') //Truncate list at comment character
                {    
                    boost::tokenizer< boost::char_separator<char> >::iterator temp = cmd_it;
                    ++temp;
                    while(temp != cmd_tok.end())
                    {
                        ++temp;
                        ++cmd_it;
                    }
                    --list_it;
                    std::list<std::string>::iterator temp2 = list_it;
                    ++temp2;
                    token_list.resize(std::distance(token_list.begin(),temp2));
                }
            }
            std::list<std::string>::iterator temp_it = list_it;
            --list_it;
            token_list.erase(temp_it);
        }
    }

    //Convert from infix to postfix
    std::vector<std::string> stack;
    std::vector<std::string> argv;
    std::list< std::vector<std::string> > postfix_list;
    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        std::string token = *list_it;
        if(token == "(")
        {
            if(argv.size() > 0)
            {
                postfix_list.push_back(argv);
                argv.clear();
            }
            stack.push_back(token);
        }
        else if(token == ")")
        {
            if(argv.size() > 0)
            {
                postfix_list.push_back(argv);
                argv.clear();
            }
            while(!stack.empty() && stack.back() != "(")
            {
                std::string op = stack.back();
                std::vector<std::string> op_vec;
                op_vec.push_back(op);
                postfix_list.push_back(op_vec);
                stack.pop_back();
            }
            if(!stack.empty())
            {
                stack.pop_back();
            }
        }
        else if(token == ";" || token == "&&" || token == "||")
        {
            if(argv.size() > 0)
            {
                postfix_list.push_back(argv);
                argv.clear();
            }
            if(!stack.empty() && stack.back() != "(")
            {
                std::string op = stack.back();
                std::vector<std::string> op_vec;
                op_vec.push_back(op);
                postfix_list.push_back(op_vec);
                stack.pop_back();
            }
            stack.push_back(token);
        }
        else
        {
            argv.push_back(token);
        }
    }
    if(argv.size() > 0)
    {
        postfix_list.push_back(argv);
        argv.clear();
    }
    while(!stack.empty())
    {
        std::string op = stack.back();
        std::vector<std::string> op_vec;
        op_vec.push_back(op);
        postfix_list.push_back(op_vec);
        stack.pop_back();
    }


    //Convert from postfix to expression tree
    std::vector<Component*> component_stack;
    std::list< std::vector<std::string> >::iterator post_it;
    for(post_it = postfix_list.begin(); post_it != postfix_list.end(); ++post_it
)
    {
        std::vector<std::string> token = *post_it;
        if(token[0] == ";")
        {
            NextConnector* next_con = new NextConnector();
            next_con->setRight(component_stack.back());
            component_stack.pop_back();
            next_con->setLeft(component_stack.back());
            component_stack.pop_back();
            component_stack.push_back(next_con);
        }
        else if(token[0] == "&&")
        {
            AndConnector* and_con = new AndConnector();
            and_con->setRight(component_stack.back());
            component_stack.pop_back();
            and_con->setLeft(component_stack.back());
            component_stack.pop_back();
            component_stack.push_back(and_con);

        }
        else if(token[0] == "||")
        {
            OrConnector* or_con = new OrConnector();
            or_con->setRight(component_stack.back());
            component_stack.pop_back();
            or_con->setLeft(component_stack.back());
            component_stack.pop_back();
            component_stack.push_back(or_con);

        }
        else
        {
            Executable* executable = new Executable(token);
            component_stack.push_back(executable);
        }
    }

    container->addCommand(component_stack.back());
    component_stack.pop_back();
}
