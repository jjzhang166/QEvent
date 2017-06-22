#pragma once

#include <memory>
#include <list>
#include <thread>
#include "QJMutex.h"

namespace qijia
{

struct IDataBase
{
    virtual ~IDataBase() {}
    virtual void run() = 0;
};

template <typename FunCall>
struct CDataBase : public IDataBase
{
    CDataBase(FunCall&& f)
        : m_fun(std::forward<FunCall>(f))
    {}

    void run()
    {
        m_fun();
    }

    FunCall m_fun;
};
class QJEventData
{

public:
    template <typename FunCall, typename ... Args>
    QJEventData(FunCall&& f, Args&& ... args)
    {
        m_pFun = makeCDataBase(std::bind(std::forward<FunCall>(f), std::forward<Args>(args)...));
    }

    void run()
    {
        m_pFun->run();
    }
private:

    template <typename FunCall>
    std::shared_ptr<CDataBase<FunCall>> makeCDataBase(FunCall&& f)
    {
        return std::make_shared<CDataBase<FunCall>>(std::forward<FunCall>(f));
    }
    std::shared_ptr<IDataBase> m_pFun;
};

// this loop for other thread sendevent to main thread
class QJEventLoop
{
    typedef std::list<std::shared_ptr<QJEventData>> eventDataList;
    typedef std::shared_ptr<eventDataList> eventDataListPtr;
public:
    void init()
    {
        m_mainId = (std::this_thread::get_id ());
    }
    bool loop()
    {
        if ( std::this_thread::get_id () != m_mainId)
        {
            //error not run in main thread
            return false;
        }
        eventDataListPtr eventList;
        {
            qijia::QJMutex lock (mtx);
            eventList.reset (new eventDataList(*m_eventList));
            m_eventList->clear();
        }

        auto iter = eventList->begin();
        while(iter != eventList->end())
        {
            (*iter)->run();
            iter++;
        }
        return true;
    }
    static QJEventLoop& App()
    {
        static QJEventLoop instance;
        return instance;
    }
    void addEventList(std::shared_ptr<QJEventData> event)
    {
        qijia::QJMutex lock (mtx);
        if (!m_eventList.unique ())
        {
            m_eventList.reset (new eventDataList(*m_eventList));
        }
        m_eventList->push_back(event);
    }

private:
    QJEventLoop()
        :m_mainId(std::this_thread::get_id ())
    {
        m_eventList.reset(new eventDataList);
    }
    std::thread::id m_mainId;
    eventDataListPtr m_eventList;
    std::mutex mtx;
};

}



