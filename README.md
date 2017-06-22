QEvent - 一个线程安全的信号槽 

About
=====

QEvent - 一个跨平台线程安全的信号槽 ，使用方法如同Qt里的信号槽，全部代码量只有２百多行．核心代码只有几十行

It is released under the terms of the GNU Lesser General Public License, version 2.1 or later.

Requirements
============

QEvent依赖c++11 可以在任何平台运行

Use
================
使用和Qt里的信号槽使用相似
定义：
    Event_Def(m_addSignal,int,int);
注册：
    ERegister(tt,m_addSignal,&t, &Test::add);
注销：
    EUnRegister(tt,m_addSignal,&t, &Test::add);
触发：
    ESendEvent(this,m_addSignal,rand() % 100, rand() % 100);

具体使用可以参考/test/main.cpp


