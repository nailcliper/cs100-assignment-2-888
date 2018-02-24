#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <list>
#include <vector>

class Container;
class Component;

class Parser
{
private:
    void parseStrings(std::list<std::string>& token_list);
    void parseSymbols(std::list<std::string>& token_list);
    std::list< std::vector<std::string> > infixToPostfix(std::list<std::string>& token_list);
    Component* postfixToTree(std::list< std::vector<std::string> >& postfix_list);
public:
    void parseInput(std::string input, Container* container);
};

#endif //__PARSER_H__
