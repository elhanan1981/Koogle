#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

class Parser
{
private:
    static void remove_punctuation(std::string &word);

public:
    Parser() = default;
    ~Parser() = default;

    static std::vector<std::string> extract_url(const std::string &str);
    static std::vector<std::string> extract_words(const std::string &str);
};
#endif