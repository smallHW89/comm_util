/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:fifo.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-12
*   Describe:
*   多读，多写的 无锁队列，利用链表,利用cas指令
*   利用 CAS 或者 原子操作
*   1、利用gcc 的接口
*   2、自己用汇编实现
*   用链表实现
*
*   gcc 4.1.2开始 内置接口 
*   bool    __sync_bool_compare_and_swap(type *ptr , type oldval ,tpye newval, ...)
*   type    __sync_val_compare_and_swap (type *ptr , type oldval ,type newval, ...)
*
*   汇编实现CAS
*
********************************************************/
#ifndef _FIFO_H
#define _FIFO_H

template < typename ElmType >
struct  qnode
{
    struct qnode * _next;
    ElmType        _data; 
};

/*
 * 链表第一个节点不放数据
 */

template <typename  ElmType >
class queue
{
    private:
        struct qnode<ElmType> * volatile   _head;
        struct qnode<ElmType> * volatile   _tail;
        struct qnode<ElmType> * volatile   _start;
    public:
        queue()
        {
            _start = _head = _tail = new qnode<ElmType> ;
            _head->_next = NULL;
            _tail->_next = NULL;
        }
        ~queue()
        {
            struct qnode<ElmType> * volatile tmp;
            while( _start )
            {
                tmp = _head->_next;
                delete _head;
                _head = tmp;
            }
        }
        void push(const ElmType &e)
        {
            struct qnode<ElmType> * p = new qnode<ElmType>;
            p->_next = NULL;
            p->_data = e;

            struct qnode<ElmType> * t = _tail;
            struct qnode<ElmType> * old_t = _tail;

            /*do {
                t  = _tail;
            }while( __sync_bool_compare_and_swap(&t->_next, NULL, p ) );
            */ 
            do {
                while(t->_next != NULL) 
                    t = t->_next;
            } while( !__sync_bool_compare_and_swap(&t->_next, NULL, p ) );


            __sync_bool_compare_and_swap(&_tail, old_t , p);
        }

        bool  pop( ElmType * e)
        {
            struct qnode<ElmType> * p  = NULL;
            struct qnode<ElmType> * np  = NULL;

            do{
                p  = _head;
                np = p->_next;
                if( p->_next == NULL )
                    return false;
            }while( ! __sync_bool_compare_and_swap(&_head , p , p->_next ));
            *e = p->_next->_data;
            //delete p;  //回收要单独的线程来左，只能单线程, 从_start 回收到 _head 前一个
            return true;
        }
};
 
#endif //FIFO_H



