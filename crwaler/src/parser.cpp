#include "../inc/parser.hpp"

#include <regex>
#include <iostream>
#include <string>
#include <sstream>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <htmlcxx/html/ParserDom.h>
#include <algorithm>
#include <cctype>

std::vector<std::string> Parser::extract_url(const std::string &str)
{
    std::vector<std::string> urls;
    std::string regex_str = "<a href=\"(https?://[^\"]+)\"";

    std::regex regex_pattern(regex_str);
    std::smatch match;

    std::string::const_iterator search_start(str.cbegin());
    while (std::regex_search(search_start, str.cend(), match, regex_pattern))
    {
        std::string url = match[1];
        urls.push_back(url);
        search_start = match.suffix().first;
    }
    return urls;
}

std::vector<std::string> Parser::extract_words(const std::string &str)
{
    std::vector<std::string> words;

    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(str);

    std::set<std::string> non_visible_tags = {"style", "script", "noscript", "meta", "link"};

    for (auto it = dom.begin(); it != dom.end(); ++it)
    {
        if (it->isTag() && non_visible_tags.find(it->tagName()) != non_visible_tags.end())
        {
            it.skip_children();
        }
        else if (!it->isTag() && !it->isComment())
        {
            std::string text = it->text();
            std::istringstream iss(text);
            std::string word;
            while (iss >> word)
            {
                remove_punctuation(word);
                if (!word.empty())
                {
                    words.push_back(word);
                }
            }
        }
    }

    return words;
}

void Parser::remove_punctuation(std::string &word)
{
    word.erase(word.begin(), std::find_if(word.begin(), word.end(), [](unsigned char ch)
                                          { return !std::ispunct(ch); }));
    word.erase(std::find_if(word.rbegin(), word.rend(), [](unsigned char ch)
                            { return !std::ispunct(ch); })
                   .base(),
               word.end());
}