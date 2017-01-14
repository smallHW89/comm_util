/********************************************************
*   Copyright (C) 2016 All rights reserved.
*   
*   Filename:malloc_info_test.cpp
*   Author  :huangwei  497225735@qq.com
*   Date    :2016-12-27
*   Describe: 查看malloc 分配堆的内存的信息
*
*   struct mallinfo mallinfo();
*   void malloc_stats();
*   void malloc_info(int options ,FILE *f);
*
********************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#include<unistd.h>


void show_malloc_info()
{
    struct mallinfo mi = mallinfo();
    printf("mallinfo show info:\n"); 
    printf("arena :non-mmapped space allocated from system (bytes)---%u\n", mi.arena);
    printf("ordblks :number of free chunks    ---%u\n", mi.ordblks);
    printf("smblks  :number of fastbin blocks ---%u\n", mi.smblks);
    printf("hblks : number of mmapped regions ---%u\n", mi.hblks);
    printf("hblkhd: space in mmapped regions(bytes)  ---%u\n", mi.hblkhd);
    printf("usmblks: maximum total allocated space (bytes) ---%u\n", mi.usmblks);
    printf("fsmblks: space available in freed fastbin blocks(bytes) ---%u\n", mi.fsmblks);
    printf("uordblks: total allocated space(bytes) ---%u\n", mi.uordblks );
    printf("fordblks: total free space(bytes) ---%u\n", mi.fordblks);
    printf("keepcost: top-most, releasable space(bytes) ---%u\n", mi.keepcost);
    printf("\n");
}

void show_malloc_info_v2()
{
    malloc_stats();
}


int main(int argc, char ** argv)
{
    show_malloc_info();
    
    char * ptr_arr[2000];

    for(int i=0; i<2000;  i++ )
    {
        sleep(1); 
        ptr_arr[i] = (char*)malloc(i*1024); 
    }
    
    show_malloc_info();
    show_malloc_info_v2();

    
    return 0;
}
