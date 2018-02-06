#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>

class Container;

class Parser
{
public:
    void parseInput(std::string input, Container* container);
};

#endif //__PARSER_H__
