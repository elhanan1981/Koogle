#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include "worker.hpp"
#include <thread>
#include <vector>

class Crawler
{
private:
    Worker m_worker;
    std::vector<std::thread> m_threads;

public:
    Crawler();
    ~Crawler();
    void manage_threads();
};
#endif