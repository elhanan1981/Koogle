#include "../inc/crawler.hpp"
#include "../inc/settings.hpp"
#include <iostream>

Crawler::Crawler()
{
}

Crawler::~Crawler() = default;

void Crawler::manage_threads()
{
    for (size_t i = 0; i < Settings::NUM_THREADS; i++)
    {
        m_threads.push_back(std::thread([this]
                                        { m_worker.work(); }));
    }
    for (auto &thread : m_threads)
    {
        thread.join();
    }
}
