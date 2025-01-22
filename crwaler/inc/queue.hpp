#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
public:
    void push(T const &e);
    bool pop(T &e);

private:
    std::queue<T> q;
    size_t max_size = 10000000;
    mutable std::mutex mtx;
    std::condition_variable cond_not_empty;
    std::condition_variable cond_not_full;
};

template <typename T>
void Queue<T>::push(T const &e)
{
    std::unique_lock<std::mutex> g{mtx};
    while (q.size() == max_size)
    {
        cond_not_full.wait(g);
    }

    q.push(e);
    cond_not_empty.notify_one();
}

template <typename T>
bool Queue<T>::pop(T &e)
{
    std::unique_lock<std::mutex> g{mtx};

    cond_not_empty.wait(g, [this]()
                        { return !q.empty(); });

    if (q.empty())
    {
        return false;
    }

    e = q.front();
    q.pop();
    cond_not_full.notify_one();

    return true;
}

#endif