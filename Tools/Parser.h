//
// Created by iheb on 4/2/2020.
//

#ifndef HZDR_DEMO_SERVER_PARSER_H
#define HZDR_DEMO_SERVER_PARSER_H
#include <string>
#include <vector>
#include <sstream>

class Parser
{
private:

public:
    std::vector<std::string> values;
    void parse(char *buffer);
    void parse(const char *buffer);
    void parse(const std::string& buffer);
};

void Parser::parse(char *buffer)
{
    parse((const char*)buffer);
}

void Parser::parse(const char *buffer)
{
    std::string in = buffer;
    std::istringstream iss(in);

    std::string sub;
    while(iss >> sub)
    {
        values.push_back(sub);
    }

}

void Parser::parse(const std::string& buffer){
    std::stringstream iss(buffer);
    std::istream_iterator<std::string> begin_(iss);
    std::istream_iterator<std::string> end_;
    std::vector<std::string> vstrings(begin_, end_);
    values = vstrings;
}

#endif //HZDR_DEMO_SERVER_PARSER_H
