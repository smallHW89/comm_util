/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:test.cpp
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-10
*   Describe:
*
********************************************************/

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>


#include "fifo.h"


queue<int>    g_queue;


void *thread_produce(void * arg)
{
    int i = 0;
    while(i<=10000000)
    {
        g_queue.push(i); 
        printf("push %u\n", i);
        i++;
        usleep(::rand()%20);
    }
}


void *thread_consume(void *arg)
{
    int i=0;
    while(i<10000000)
    {
        bool b = g_queue.pop(&i); 
        if(b)
        {
            printf("pop %u\n",i);
        }
        usleep(::rand()%20);
    }
}


int main()
{
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, 0, thread_consume,0);
    pthread_create(&t2, 0, thread_consume,0);
    
    pthread_create(&t3, 0, thread_produce,0);
    pthread_create(&t4, 0, thread_produce,0);

    while(1)
    {
        sleep(2) ;
    }
    return 0;
}
