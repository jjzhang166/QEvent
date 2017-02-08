
#include <../QJEventCenter.h>
#include <iostream>
#include <thread>
#include <memory>
using namespace std;
using namespace qijia;
class Sprite : public std::enable_shared_from_this<Sprite>
{
public:
    void test(string _ref)
    {
        cout << std::this_thread::get_id ()<<" "<<this<<"I am here :"<<_ref<<endl;
        return;
    }
    void dosomthing(string _ref)
    {
        cout << std::this_thread::get_id ()<<"  "<<this<< "I am here do sth:"<<_ref<<endl;
        return;
    }
    void send(string i)
    {
        cout << "send !!!"<<i<<endl;
    }
    void signal(string str)
    {
        QJemit(Sprite::send,str );
    }
};

class Test : public std::enable_shared_from_this<Test>
{
public:
    Test(shared_ptr<Sprite> s)
        :sp(s)
    {}
    void dosomthing(string _ref)
    {
        cout << std::this_thread::get_id ()<<"  "<<this<< "Test:dosomething"<<_ref<<endl;
        QJconnect(sp,Sprite::send,sp,Sprite::test);
        QJconnect(sp,Sprite::send,sp,Sprite::dosomthing);
        sp->signal ("Test dosomething send");
        QJunconnect(sp,Sprite::send,sp,Sprite::test);
    }
    shared_ptr<Sprite> sp;
};

shared_ptr<Sprite> sprite(new Sprite() );
shared_ptr<Sprite> sprite1(new Sprite() );
void test1()
{
    QJconnect(sprite,Sprite::send,sprite,Sprite::test);
    QJconnect(sprite,Sprite::send,sprite,Sprite::dosomthing );
    QJconnect(sprite,Sprite::send,sprite1,Sprite::test);
    QJconnect(sprite,Sprite::send,sprite1,Sprite::dosomthing );
    sprite->signal ("all");

    cout << "***************"<<std::this_thread::get_id ()<<endl;
    QJunconnect(sprite,Sprite::send,sprite1,Sprite::test);
    QJunconnect(sprite,Sprite::send,sprite,Sprite::dosomthing );
    sprite->signal ("some");
}

shared_ptr<Sprite> sprite2(new Sprite() );
shared_ptr<Sprite> sprite3(new Sprite() );
void test()
{
    test1();
    cout << "test1***************"<<std::this_thread::get_id ()<<endl;
    shared_ptr<Test> tes1(new Test(sprite2) );
    QJconnect(sprite3,Sprite::send,tes1,Test::dosomthing);
    sprite3->signal ("test 1 all");
    cout << "test1****end***********"<<std::this_thread::get_id ()<<endl;
}

int main()
{
    std::thread t1(test);
    std::thread t2(test);
    std::thread t3(test);
    std::thread t4(test);
    std::thread t5(test);
    std::thread t6(test);
    std::thread t7(test);
    std::thread t8(test);
    t1.detach ();
    t2.detach ();
    t3.detach ();
    t4.detach ();
    t5.detach ();
    t6.detach ();
    t7.detach ();
    t8.detach ();
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    return 0;
}
