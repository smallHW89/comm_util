/********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
*   Filename:kv_config_test.cpp
*   Author  :huangwei  497225735@qq.com
*   Date    :2017-01-14
*   Describe:
*
********************************************************/
#include "kv_config.h"


int main(int argc , char **argv)
{
    if(argc != 2 ){
        printf("usage :%s  config_file\n", argv[0]);
        exit(1);
    }

    CKvConfig    config((const char*)argv[1]);

    bool b = config.InitConfig();
    if(b)
    {
        config.ShowConfig();
    }
}
