/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:fifo.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-10
*   Describe: 1读1写，循环buffer ，变长元素的, 无锁队列
*
*   |================================================|
*
*   _in==_out  empty
*
*   _in                   _out   _in< _out      _out - _in
*           _in,_out             _in+1 == _out , full
*   _out                                           _in  full
*               _out    _in                     size-1-(_in-_out)
*
********************************************************/
#ifndef _FIFO_H
#define _FIFO_H

#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define NODE_SIZE  8

typedef struct Element
{
    Element(): _data_len(0){}
    uint32_t   _data_len;
    char  *    _data;
    uint32_t   ele_size()
    {
        //取8的倍数
        uint32_t ilen = sizeof(_data_len) + _data_len;
        return ((ilen + 8 -1) >>3) << 3 ;
    }
}Element;


class ring_queue
{
    private:
        char   *  _buffer;
        uint64_t  _size;
        uint64_t  _in;
        uint64_t  _out;
        uint64_t  _tail;

    public:
        ring_queue(uint32_t  size)
        {
            //取8的倍数
            _size = ((size + 8 -1 ) >>3) << 3;
            _buffer = new char[_size];
            _in = _out = 0;
            _tail = _size ;
        }

        ~ring_queue()
        {
            if(NULL != _buffer )
                delete []_buffer;
        }

        bool  push(Element *ele)
        {
            uint32_t ilen      = ele->ele_size(); //按8取整
            uint32_t iLeftLen  = (_size + _out - _in-1)%_size; //_size 长的列表，并不能放满 ,剩余空间
            if( iLeftLen < ilen )
            {
                return false;
            }
            int  old_in   = _in;
            int  new_in   = (old_in + ilen)% _size;
            if( new_in  &&  new_in < old_in  ) //swap, 结尾剩余空间不够,new_in == 0 表示结尾刚刚放满
            {
                if( ilen >= _out  ) //space not enough
                {
                    return false;
                }
                new_in  = 0 + ilen;
            }
            //push
           if( new_in && new_in < old_in )
           { //swap around
                _tail = old_in; //old_in和后面的略过
                //内容包括4个字节的长度，后面是内容
                *(uint32_t*)_buffer = ele->_data_len;
                memcpy(_buffer + sizeof(uint32_t), ele->_data,  ele->_data_len); 
                _in =  new_in;
           }
           else
           {
                *(uint32_t*)(_buffer + old_in) = ele->_data_len;
                memcpy(_buffer + old_in + sizeof(uint32_t), ele->_data,  ele->_data_len);
                _in = new_in;
                if(0 == new_in)
                { //结尾满了
                    _tail =  _size;
                }
           }
           printf("_oldin:%u---_newin:%u---tail:%u--datalen:%u\n",old_in, new_in,_tail,ele->_data_len);
           return true;
        }
        bool  pop (Element *ele)
        {
            int old_out = _out;
            int out = 0;
            if( old_out >= _tail ) //tail后面都是skip过去的
            {
                out = 0; //swap到头部
            }
            else
            {
                out = old_out; 
            }
            if( out == _in ) //empty
            {
                return false;
            }


            ele->_data_len = *(uint32_t*)(_buffer + out);
            _out =  (out + ele->ele_size() )%  _size;

            printf("oldout:%u----newout:%u---datalen:%u\n", old_out, _out,ele->_data_len );
            
            memcpy(ele->_data, _buffer + out + sizeof(uint32_t), ele->_data_len );


            return true;
        }
};

#endif //FIFO_H
