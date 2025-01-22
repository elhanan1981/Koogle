#ifndef WORKER_HPP
#define WORKER_HPP

#include "./queue.hpp"
#include "../inc/my_queue.hpp"
#include "./db_manager.hpp"
#include "./set.hpp"

#include <iostream>
class Worker
{
private:
    MyQueue<std::string> m_queue;
    Set<std::string> m_set;
    size_t m_max_depth;
    DBManager m_storing;

    void handle_new_urls(std::vector<std::string> urls, const size_t depth);

public:
    Worker();
    ~Worker();
    void work();
};

#endif
