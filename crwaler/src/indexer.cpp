#include "../inc/indexer.hpp"

std::unordered_map<std::string, size_t> Indexer::index(std::vector<std::string> &str_vec)
{
    std::unordered_map<std::string, size_t> ump;
    for (auto &item : str_vec)
    {
        ump[item]++;
    }
    return ump;
}