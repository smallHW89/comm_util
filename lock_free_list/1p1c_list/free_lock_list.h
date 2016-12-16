/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:free_lock_list.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-09
*   Describe: 1读1写的无锁队列
*   
*   volatile  : 提示编译器后面的变量随时可能变化，编译后程序每次需要写或者读这个变量时，要直接从变量地址读取。如果不用这个关键字，
*   编译器可能优化读、写，可能暂时使用寄存器种的值，如果这个变量被别的程序更新了，将出现不一致
*
********************************************************/
#ifndef _FREE_LOCK_LIST_H
#define _FREE_LOCK_LIST_H


#include<iostream>

template < typename ElmType >
struct  qnode
{
    struct qnode * _next;
    ElmType        _data; 
};

template <typename  ElmType >
class queue
{
    private:
        struct qnode<ElmType> * volatile   _head;
        struct qnode<ElmType> * volatile   _tail;
    
    public:
        queue()
        {
            _head = _tail = new qnode<ElmType>  ;
            _head->_next = NULL;
        }
        ~queue()
        {
            struct qnode<ElmType> * volatile tmp;
            while( _head )
            {
                tmp = _head->_next;
                delete _head;
                _head = tmp;
            }
        }

        void push(const ElmType  & e)
        {
            struct qnode<ElmType> * p = new qnode<ElmType>; 

            p->_next = NULL;
            _tail->_next = p;
            _tail->_data = e;
            _tail =  p;
        }

        bool pop(ElmType * e)
        {
            if(_head == _tail) 
                return false;
            struct qnode<ElmType> * p = _head;
            *e = p->_data;
            _head = p->_next;
            delete p;
            return true; 
        }
};




 
#endif //FREE_LOCK_LIST_H
