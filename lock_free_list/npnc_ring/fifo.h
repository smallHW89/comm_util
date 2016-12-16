/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:fifo.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-10
*   Describe:
*   环形队列，定长元素(固定类型）
*   n读 n 写
*
*   |----------------------------|
*   in                 out
*         out          in
*                    in out
*   out                         in                
*
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
*
********************************************************/
#ifndef _FIFO_H
#define _FIFO_H

#include<stdint.h>

#define  NODE_HAVE_DATA  0xF0F0C1C1
#define  NODE_NO_DATE    0

template <typename  Ele>
class  ring_queue
{
public:
    struct queue_node
    {
        uint32_t  _flag; 
        Ele       _data;
    };


private:
    uint32_t             _size;    
    uint32_t            _in;
    uint32_t             _out;
    struct queue_node   *_buffer;

public:
    ring_queue(uint32_t size)
    {
        _buffer = new struct queue_node[size+10];
        _size = size+10;
        _in =  0;
        _out = 0;
    }
    ~ring_queue()
    {
        if(_buffer != NULL )
            delete []_buffer;
    }

    bool pop(Ele * e)
    {
        if( _out == _in )
            return false;

        uint32_t old_out = _out;
        do
        {
            old_out = _out;
            if( _buffer[old_out]._flag != NODE_HAVE_DATA )
            {
                if( old_out != _out)
                    old_out = _out; //可能被其他线程读取走了
                continue;
            }

            uint32_t new_out = (old_out + 1)%_size;
            //占坑
            if(  __sync_bool_compare_and_swap(&_in, old_out, new_out ) )
            {//这里防止还没有读，就有来写的，可以多预留一些块,解决不了本质问题
            
            }
            else//被其他读线程占坑
            {
                old_out = _out;
            }
        }while(1)
        

        return true;
    }

    bool push(const Ele  &e)
    {
        while(1)
        {
            uint32_t old_in = _in;
            uint32_t empty_len = ( _out - _in + _size -1) %_size;
            if(empty_len <= 5 ) //预留5块内存，不允许出现读时写覆盖
            {
                return false;
            }
            uint32_t new_in = (old_in+1)%_size;
            //先占坑，然后再拷贝内容, 占坑失败，去重试
            if( ! __sync_bool_compare_and_swap(&_in, old_in, new_in ) )
                continue;

            _buffer[_oldin]._data = e; //非原子操作
            _buffer[_oldin]._flag = NODE_HAVE_DATA; //打上标记，有数据，是否要原子操作

            break;
        }
        return true;
    }
};






#endif //FIFO_H
