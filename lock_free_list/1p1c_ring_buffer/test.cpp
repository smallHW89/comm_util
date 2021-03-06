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



ring_queue<int>  g_queue(10000);

void *thread_produce(void * arg)
{
    int i = 0;
    while(i<=10000000)
    {
        bool b = g_queue.push(i); 
        if(b)
        {
        printf("push %u\n", i);
        i++;
        }
        usleep(::rand()%10);
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
        usleep(::rand()%10);
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
