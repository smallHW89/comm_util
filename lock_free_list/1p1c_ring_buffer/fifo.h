/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:fifo.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-10
*   Describe: 环形队列，定长元素
*
*   |----------------------------|
*   in                 out
*         out          in
*                    in out
*   out                         in                
********************************************************/
#ifndef _FIFO_H
#define _FIFO_H

#include<stdint.h>

template <typename  Ele>
class  ring_queue
{
private:
    uint32_t  _size;    
    uint32_t  _in;
    uint32_t  _out;
    Ele       *_buffer;

public:
    ring_queue(uint32_t size)
    {
        _buffer = new Ele[size];
        _size = size;
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
        if(_in == _out )
        { //queue is Empty
            return false;
        }

        *e = _buffer[_out];
        if(_out+1 == _size)
            _out = 0;
        else
            _out++;
        return true;
    }

    bool push(const Ele  &e)
    {
        if(_in < _out  && _in+1 == _out) //full
            return false;
        if( _in > _out  && _in -_out == _size-1) //full
            return false;

        _buffer[_in] = e;
        if( _in +1 == _size)
            _in = 0;
        else
            _in ++;
        return true;
    }
};





 
#endif //FIFO_H
