#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <list>
#include <vector>

class Component;

class Parser
{
private:
    void parseStrings(std::list<std::string>& token_list);
    void parseSymbols(std::list<std::string>& token_list);
    bool isHigherPrecedence(std::string a, std::string b);
    std::list< std::vector<std::string> > infixToPostfix(std::list<std::string>& token_list);
    Component* postfixToTree(std::list< std::vector<std::string> >& postfix_list);
public:
    void parseInput(std::string input);
};

#endif //__PARSER_H__
