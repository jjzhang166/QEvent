#pragma once
#include <mutex>
namespace qijia
{

class QJMutex
{
public:
    explicit QJMutex(std::mutex& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }
    ~QJMutex()
    {
        m_mutex.unlock();
    }
private:
    std::mutex& m_mutex;
};
#define QJMutex(x) error "Missing guard object name"

}



