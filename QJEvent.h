#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include "QJEventLoop.h"
#include "QJMutex.h"
namespace qijia
{

template <typename ... Args>
struct ISlotBase
{
    virtual ~ISlotBase() {}
    virtual void sendEvent(Args&& ... args_) = 0;
    virtual std::shared_ptr<QJEventData> sendEventSync(Args&& ... args_) = 0;
};

template <typename PSignal, typename FunSignal, typename ... Args>
struct CSlotBase : public ISlotBase<Args...>
{
    CSlotBase(PSignal* object, const FunSignal& fun)
        : m_object(object)
        , m_fun(fun) {}
    ~CSlotBase() {}

    void sendEvent(Args&& ... args_)
    {
        (m_object->*m_fun)(std::forward<Args>(args_)...);
    }
    std::shared_ptr<QJEventData> sendEventSync(Args&& ... args_)
    {
        return std::shared_ptr<QJEventData>( new QJEventData(std::bind(m_fun, m_object, std::forward<Args>(args_)...)) );
    }
    PSignal* m_object;
    FunSignal m_fun;
};

template <typename ... Args>
struct QJSlot
{
    template <typename PSignal, typename FunSignal>
    QJSlot(PSignal* object, const FunSignal& fun)
    {
        m_pFun = std::make_shared<CSlotBase<PSignal, FunSignal, Args...>>(object, fun);
    }
    void sendEvent(Args&& ... args_)
    {
        m_pFun->sendEvent(std::forward<Args>(args_)...);
    }
    std::shared_ptr<QJEventData> sendEventSync(Args&& ... args_)
    {
        return m_pFun->sendEventSync(std::forward<Args>(args_)...);
    }

    std::shared_ptr<ISlotBase<Args...>> m_pFun;
};


template <typename ... Args>
class QJSignal
{
    typedef std::vector<QJSlot<Args...>*> eventSlotVec;
    typedef std::shared_ptr<eventSlotVec> eventSlotVecPtr;
public:
    QJSignal()
    {
        m_slots.reset(new eventSlotVec);
    }
    ~QJSignal()
    {
        auto iter = m_slots->begin();
        while(iter != m_slots->end())
        {
            delete (*iter);
            iter++;
        }
    }

    template <typename PSignal, typename FunSignal>
    bool connect(PSignal* object, const FunSignal& fun)
    {
        {
            eventSlotVecPtr slot;
            {
                qijia::QJMutex lock (mtx);
                slot = m_slots;
            }
            auto iter =   slot->begin();
            while(iter !=   slot->end())
            {
                auto pSlotbase = static_cast<CSlotBase<PSignal, FunSignal, Args...>*>( (*iter)->m_pFun.get());
                if(  (object == pSlotbase->m_object) && (fun == pSlotbase->m_fun) )
                {
                    break;
                }
                iter++;
            }
            if (iter != slot->end())
            {
                return false;
            }
        }
        //qjbug if now other thread connect maby double add
        qijia::QJMutex lock (mtx);
        if (!m_slots.unique ())
        {
            m_slots.reset (new eventSlotVec(*m_slots));
        }
        QJSlot<Args...>* t = new QJSlot<Args...>(object, fun);
        m_slots->push_back(t);
        return true;
    }


    template <typename PSignal, typename FunSignal>
    bool unconnect(PSignal* object, const FunSignal& fun)
    {
        qijia::QJMutex lock (mtx);
        if (!m_slots.unique ())
        {
            m_slots.reset (new eventSlotVec(*m_slots));
        }
        auto iter =   m_slots->begin();
        while(iter !=   m_slots->end())
        {
            auto pSlotbase = static_cast<CSlotBase<PSignal, FunSignal, Args...>*>( (*iter)->m_pFun.get());
            if(  (object == pSlotbase->m_object) && (fun == pSlotbase->m_fun) )
            {
                break;
            }
            iter++;
        }
        if (iter != m_slots->end())
        {
            delete (*iter);
            m_slots->erase(iter);
            return true;
        }
        return false;
    }

    void sendEvent(Args ... args_)
    {
        eventSlotVecPtr slot;
        {
            qijia::QJMutex lock (mtx);
            slot = m_slots;
        }
        auto iter = slot->begin();
        while(iter != slot->end())
        {
            (*iter)->sendEvent(std::forward<Args>(args_)...);
            iter++;
        }
    }


    void sendEventSync(Args ... args_)
    {
        eventSlotVecPtr slot;
        {
            qijia::QJMutex lock (mtx);
            slot = m_slots;
        }
        auto iter =   slot->begin();
        while(iter !=   slot->end())
        {
            qijia::QJEventLoop::App().addEventList((*iter)->sendEventSync(std::forward<Args>(args_)...));
            iter++;
        }
    }

private:
    eventSlotVecPtr m_slots;
    std::mutex mtx;
};

}

#define QJSIGNAL(...) qijia::QJSignal<__VA_ARGS__>
#define QJCONNECT(Signal, ...) Signal.connect(__VA_ARGS__)
#define QJUNCONNECT(Signal, ...) Signal.unconnect(__VA_ARGS__)
#define QJsendEvent(Signal, ...) Signal.sendEvent(__VA_ARGS__)
#define QJsendEventSYNC(Signal, ...) Signal.sendEventSync(__VA_ARGS__)

#define ERegister(sender, Signal, ...) (sender)->Signal.connect(__VA_ARGS__)
#define EUnRegister(sender, Signal, ...) (sender)->Signal.unconnect(__VA_ARGS__)
#define Event_Def(Signal, ...) qijia::QJSignal<__VA_ARGS__> Signal
#define ESendEvent(sender, Signal, ...) (sender)->Signal.sendEvent(__VA_ARGS__)
#define ESendEvent_Asyn(sender, Signal, ...) (sender)->Signal.sendEventSync(__VA_ARGS__)
