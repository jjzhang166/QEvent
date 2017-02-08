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
注册：
        QJconnect(sp,Sprite::send,sp,Sprite::test);
注销：
        QJunconnect(sp,Sprite::send,sp,Sprite::test);
触发：
　　QJemit(Sprite::send,"send you mess" );



