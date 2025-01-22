#ifndef SET_HPP
#define SET_HPP

#include <set>
#include <mutex>

template <typename T>
class Set
{
private:
    std::set<T> m_set;
    mutable std::mutex mtx;

public:
    Set();
    ~Set();
    void insert(T const &e);
    bool contains(T const &e) const;
};
template <typename T>
Set<T>::Set()
{
}
template <typename T>
Set<T>::~Set()
{
}

template <typename T>
void Set<T>::insert(T const &e)
{
    std::unique_lock<std::mutex> g{mtx};
    m_set.insert(e);
}
template <typename T>
bool Set<T>::contains(T const &e) const
{
    std::unique_lock<std::mutex> g{mtx};
    return m_set.find(e) != m_set.end();
}

#endif