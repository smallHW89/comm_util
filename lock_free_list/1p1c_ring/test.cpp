/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:test.cpp
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-10
*   Describe:
*
********************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"fifo.h" 

ring_queue  g_queue(1000000);

void *thread_produce(void * arg)
{

    int i=0; 
    while(i<= 1000000 )
    {
        char buf[100+100];
        Element el;
        el._data_len = ::rand()%100+40;
        sprintf(buf, "element index:%u,len:%u", i, el._data_len);
        el._data    = buf; 

        bool b = g_queue.push(&el);
        if (!b)
        {
            //printf("queue push error\n");
        }
        else
        {
            i++;
            printf("push:%s\n", el._data);
        }
        usleep(::rand()%20);
    }

}


void *thread_consume(void *arg)
{
    int i=0; 
    while(i<= 1000000 )
    {
        char buf[512];
        Element el;
        el._data    = buf; 

        bool b = g_queue.pop(&el);
        if (!b)
        {
            //printf("queue pop error\n");
        }
        else
        {
            i++;
            printf("pop:%s\n", el._data);
        }
        usleep(::rand()%20);

    }

}


int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, 0, thread_consume,0);
    pthread_create(&t2, 0, thread_produce,0);
    while(1)
    {
        sleep(2) ;
    }
    return 0;
}
