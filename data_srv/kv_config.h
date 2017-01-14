/********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
*   Filename:kv_config.h
*   Author  :huangwei  497225735@qq.com
*   Date    :2017-01-08
*   Describe:  用谷歌命名规范
*   解析配置文件， k = v  格式
*
*   例子:
*   time = 1000
*   name = huangwei
*
********************************************************/
#ifndef _KV_CONFIG_H
#define _KV_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <iostream>

#include<map>

class CKvConfig {
public:
    CKvConfig(const char * file): config_file_(file),config_init_(0) {
    }
    ~CKvConfig() {
    }
    bool InitConfig();
    void ShowConfig();
    bool GetConf(const std::string & conf_key, std::string & conf_value );

private:
    bool  ProcessLine(const char *szline,std::string &okey,std::string &ovalue);

    std::map<std::string, std::string>  config_map_;
    const char * config_file_;
    uint16_t     config_init_;
};


 
#endif //KV_CONFIG_H
