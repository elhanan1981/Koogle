#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP

#include "./settings.hpp"

#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>

template <typename T>
class MyQueue
{
private:
    std::vector<std::queue<T>> m_queue;
    mutable std::mutex mtx;
    std::condition_variable cond_not_empty;

    size_t size_threads_waiting = 0;

    bool empty() const;
    bool help_front_pop(T &e, size_t &index);
    void handle_threads(std::unique_lock<std::mutex> &g);

public:
    MyQueue();
    ~MyQueue();

    void push(T const &e, const size_t &depth);
    bool pop(T &e, size_t &index);

    void special_task();
    void check_finish();
};

template <typename T>
MyQueue<T>::MyQueue()
    : m_queue(1) // Initialize with one empty queue
{
}

template <typename T>
MyQueue<T>::~MyQueue()
{
}

template <typename T>
void MyQueue<T>::push(T const &e, const size_t &depth)
{
    std::unique_lock<std::mutex> g{mtx};

    if (depth >= m_queue.size())
    {
        m_queue.resize(depth + 1);
    }

    m_queue[depth].push(e);
    cond_not_empty.notify_one();
}

template <typename T>
bool MyQueue<T>::pop(T &e, size_t &index)
{
    std::unique_lock<std::mutex> g{mtx};
    this->handle_threads(g);

    if (this->help_front_pop(e, index))
    {
        return true;
    }
    return false;
}

template <typename T>
bool MyQueue<T>::empty() const
{
    for (const auto &q : m_queue)
    {
        if (!q.empty())
            return false;
    }
    return true;
}

template <typename T>
bool MyQueue<T>::help_front_pop(T &e, size_t &index)
{
    for (size_t i = 0; i < m_queue.size(); ++i)
    {
        if (!m_queue[i].empty())
        {
            e = m_queue[i].front();
            index = i;
            m_queue[i].pop();
            return true;
        }
    }
    return false;
}

template <typename T>
void MyQueue<T>::special_task()
{
    for (size_t i = 0; i < 10; ++i)
    {
        m_queue[0].push(Settings::POISON_APPLE);
    }
}

template <typename T>
void MyQueue<T>::check_finish()
{
    if (size_threads_waiting == Settings::NUM_THREADS)
    {
        this->special_task();
        cond_not_empty.notify_all();
        std::cout << "finished\n";
    }
}

template <typename T>
void MyQueue<T>::handle_threads(std::unique_lock<std::mutex> &g)
{
    size_threads_waiting++;
    this->check_finish();
    cond_not_empty.wait(g, [this]
                        { return !this->empty(); });
    size_threads_waiting--;
}
#endif
