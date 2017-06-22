
#include "../QJEvent.h"
#include <iostream>
#include <thread>


using namespace std;

class ThreadTestBase{};
class TestBase{};

enum State
{
    State_Normal = 1,
    State_Press,
    State_Focus,
    State_Disable,
    State_Select,
};

class ThreadTest : public ThreadTestBase
{
public:
    //QJSIGNAL(int,int) m_addSignal;
    Event_Def(m_addSignal,int,int);
    Event_Def(KClickEvent, State);
    void run()
    {
        while(1)
        {
            //QJEMITSYNC(m_addSignal, rand() % 100, rand() % 100);
            ESendEvent_Asyn(this,m_addSignal,rand() % 100, rand() % 100);
            ESendEvent(this,KClickEvent,State_Select);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

class Test : public TestBase
{
public:
    Test(){}
    void add(int a, int b)
    {
        cout<<a<< '+'<< b<< '='<<a + b<<endl;
    }
    void ded(int a, int b)
    {
        cout<<a<< '-'<< b<< '='<<a - b<<endl;
    }
    void state(State s)
    {
        cout<<s<<endl;
    }
};


int main()
{
    ThreadTest *tt = new ThreadTest;
    Test t;

    qijia::QJEventLoop::App().init();
    //QJCONNECT(tt->m_addSignal, &t, &Test::add);
    ERegister(tt,m_addSignal,&t, &Test::add);
    ERegister(tt,m_addSignal,&t, &Test::ded);
    ERegister(tt,KClickEvent,&t, &Test::state);
    std::thread th(std::mem_fn(&ThreadTest::run),tt);
    th.detach();
    std::thread th1(std::mem_fn(&ThreadTest::run),tt);
    th1.detach();
    static bool flag = true;
    while (1)
    {
        if (flag)
        {
            ERegister(tt,KClickEvent,&t, &Test::state);
        }
        else
        {
            EUnRegister(tt,KClickEvent,&t, &Test::state);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        qijia::QJEventLoop::App().loop();
        flag = !flag;

    }



    return 0;
}
