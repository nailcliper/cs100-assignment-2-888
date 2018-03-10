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
#include "../header/decorator.h"

using namespace std;

void Parser::parseStrings(list<string>& token_list)
{
    list<string>::iterator list_it;

    //Split string by " and ', keep delimiters
    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        string token = *list_it;
        list<string>::iterator temp_it = list_it;
        boost::char_separator<char> str_sep("","\"\'");
        boost::tokenizer< boost::char_separator<char> > str_tok(token, str_sep);
        boost::tokenizer< boost::char_separator<char> >::iterator it;

        for(it = str_tok.begin(); it != str_tok.end(); ++it)
        {
            token_list.push_back(*it);
            ++temp_it;
        }
        token_list.erase(list_it);
        list_it = temp_it;
    }

    //Reconstruct compounded strings into token
    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        string token = *list_it;
        if(token == "\"" || token == "\'")
        {
            ++list_it;
            if(list_it != token_list.end() && *list_it != token)
            {
                string master = *list_it;
                list<string>::iterator temp_it = list_it;
                ++temp_it;
                while(temp_it != token_list.end() && *temp_it != token)
                {
                    master = master + *temp_it;
                    token_list.erase(temp_it);
                    temp_it = list_it;
                    ++temp_it;
                }
                if(temp_it == token_list.end())
                {
                    token_list.push_back(token);
                }
                *list_it = master;
                ++list_it;
            }
        } 
    }
}

void Parser::parseSymbols(list<string>& token_list)
{
    list<string>::iterator list_it;

    //Iterate over non-" X " type tokens, splitting by " ;()[]", keeping ";()[]"
    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        string token = *list_it;
        if(token == "\"" || token == "\'") //Skip string tokens and remove the markers
        {
            list<string>::iterator left_it = list_it;
            ++list_it;
            list<string>::iterator right_it = list_it;
            ++right_it;
            token_list.erase(left_it);
            token_list.erase(right_it);
        }
        else
        {
            boost::char_separator<char> cmd_sep(" ",";()[]");
            boost::tokenizer< boost::char_separator<char> > cmd_tok(*list_it, cmd_sep);
            boost::tokenizer< boost::char_separator<char> >::iterator cmd_it;

            for(cmd_it = cmd_tok.begin(); cmd_it != cmd_tok.end(); ++cmd_it)
            {
                string cmd = *cmd_it;
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
                    list<string>::iterator temp2 = list_it;
                    ++temp2;
                    token_list.resize(std::distance(token_list.begin(),temp2));
                }
            }
            list<string>::iterator temp_it = list_it;
            --list_it;
            token_list.erase(temp_it);
        }
    }
    
}

bool Parser::isHigherPrecedence(string a, string b)
{
    if(b == "(")
        return true;
    
    if(a == "<" || a == ">" || a == ">>" || a == "|")
    {
        if(b == ";" || b == "&&" || b == "||")
        {
            return true;
        }
    }
    return false;
}

list< vector<string> > Parser::infixToPostfix(list<string>& token_list)
{
    list<string>::iterator list_it;
    
    vector<string> stack;
    vector<string> argv;
    list< vector<string> > postfix_list;

    for(list_it = token_list.begin(); list_it != token_list.end(); ++list_it)
    {
        string token = *list_it;
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
                string op = stack.back();
                vector<string> op_vec;
                op_vec.push_back(op);
                postfix_list.push_back(op_vec);
                stack.pop_back();
            }
            if(!stack.empty())
            {
                stack.pop_back();
            }
        }
        
        else if(token == ";" || token == "&&" || token == "||" ||
                token == "<" || token == ">" || token == ">>" || token == "|")
        {
            if(argv.size() > 0)
            {
                postfix_list.push_back(argv);
                argv.clear();
            }
            while(!stack.empty() && !isHigherPrecedence(token, stack.back()))
            {
                string op = stack.back();
                vector<string> op_vec;
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
        string op = stack.back();
        vector<string> op_vec;
        op_vec.push_back(op);
        postfix_list.push_back(op_vec);
        stack.pop_back();
    }

    return postfix_list;
}

Component* Parser::postfixToTree(list< vector<string> >& postfix_list)
{
    std::vector<Component*> component_stack;
    std::list< std::vector<std::string> >::iterator post_it;
    for(post_it = postfix_list.begin(); post_it != postfix_list.end(); ++post_it
)
    {
        std::vector<std::string> token = *post_it;
        if(token[0] == ";")
        {
            NextConnector* next_con = new NextConnector();
            if(!component_stack.empty())
            {
                next_con->setRight(component_stack.back());
                component_stack.pop_back();
            }
            if(!component_stack.empty())
            {
                next_con->setLeft(component_stack.back());
                component_stack.pop_back();
            }
            component_stack.push_back(next_con);
        }
        else if(token[0] == "&&")
        {
            AndConnector* and_con = new AndConnector();
            if(!component_stack.empty())
            {
                and_con->setRight(component_stack.back());
                component_stack.pop_back();
            }
            if(!component_stack.empty())
            {
                and_con->setLeft(component_stack.back());
                component_stack.pop_back();
            }
            component_stack.push_back(and_con);

        }
        else if(token[0] == "||")
        {
            OrConnector* or_con = new OrConnector();
            if(!component_stack.empty())
            {
                or_con->setRight(component_stack.back());
                component_stack.pop_back();
            }
            if(!component_stack.empty())
            {
                or_con->setLeft(component_stack.back());
                component_stack.pop_back();
            }
            component_stack.push_back(or_con);

        }
        else if(token[0] == "[" || token[0] == "test")
        {
            Test* test = new Test(token);
            component_stack.push_back(test);
        }
        else if(token[0] == "--q")
        {
            Quit* quit = new Quit();
            component_stack.push_back(quit);
        }
        else if(token[0] == "<")
        {
            InputRedirector* indir = new InputRedirector();
            if(!component_stack.empty())
            {
				if(Executable* temp = dynamic_cast<Executable*>(component_stack.back()))
				{
					indir->setFile(temp->getArg(0));
					component_stack.pop_back();
				}
                else
				{
					cout << "Unexpected Symbol found for filename" << endl;
					component_stack.pop_back();
				}
            }
			if(!component_stack.empty())
            {
                indir->setLeft(component_stack.back());
                component_stack.pop_back();
            }
			
			component_stack.push_back(indir);
        }
        else if(token[0] == ">" || token[0] == ">>")
        {
            OutputRedirector* outdir = new OutputRedirector(token[0]);
            if(!component_stack.empty())
            {
				if(Executable* temp = dynamic_cast<Executable*>(component_stack.back()))
				{
					outdir->setFile(temp->getArg(0));
					component_stack.pop_back();
				}
				else
				{
					cout << "Unexpected Symbol found for filename" << endl;
					component_stack.pop_back();
				}
            }
			if(!component_stack.empty())
            {
                outdir->setLeft(component_stack.back());
                component_stack.pop_back();
            }
			
			component_stack.push_back(outdir);
        }
        else if(token[0] == "|")
        {
            Piper* piper = new Piper();
            if(!component_stack.empty())
            {
                piper->setRight(component_stack.back());
                component_stack.pop_back();
            }
            if(!component_stack.empty())
            {
                piper->setLeft(component_stack.back());
                component_stack.pop_back();
            }
			
			component_stack.push_back(piper);
        }
        else
        {
            Executable* executable = new Executable(token);
            component_stack.push_back(executable);
        }
    }

    return component_stack.back();
}

void Parser::parseInput(string input)
{
    list<string> token_list;
    list<string>::iterator list_it;
    token_list.push_back(input);

    //Create a list of string tokens
    //Treat string literals as single tokens
    parseStrings(token_list);

    //Split non-string literal tokens by spaces and special symbols
    parseSymbols(token_list);

    //Convert from infix to postfix
    list< vector<string> > postfix_list = infixToPostfix(token_list);

    //Convert from postfix to expression tree
	Component* root = postfixToTree(postfix_list);
	root->execute();
}
