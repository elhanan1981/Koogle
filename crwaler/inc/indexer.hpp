#ifndef INDEXER_HPP
#define INDEXER_HPP

#include <vector>
#include <string>
#include <unordered_map>

class Indexer
{

public:
    Indexer() = default;
    ~Indexer() = default;

    static std::unordered_map<std::string, size_t> index(std::vector<std::string> &str_vec);
};
#endif