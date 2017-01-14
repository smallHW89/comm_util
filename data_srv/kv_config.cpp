/********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
*   Filename:kv_config.cpp
*   Author  :huangwei  497225735@qq.com
*   Date    :2017-01-08
*   Describe:
*
********************************************************/
#include "kv_config.h"


bool CKvConfig :: InitConfig() {
    if(config_init_ )         return true;
    if(NULL == config_file_ ) return false;     

    FILE *fp = fopen(config_file_ , "r");
    if( !fp )
    {
        perror("fopen config_file error ");
        return false;
    }

    char sztmp[64000];
    while(fgets(sztmp, sizeof(sztmp), fp) ) {
        if('#' == sztmp[0] ) continue ;
        std::string key, value;
        bool b = ProcessLine(sztmp, key, value );                      
        if(b){
            config_map_[key] = value;
        }
    }
    config_init_ = 1;
    return true;
}

void CKvConfig :: ShowConfig(){
    std::map<std::string, std::string>::iterator it = config_map_.begin();
    for(; it != config_map_.end(); it++)
        printf("%s:%s\n",it->first.c_str(), it->second.c_str() );
}

bool CKvConfig::GetConf(const std::string &conf_key, std::string &conf_value)
{
    std::map<std::string, std::string>::iterator it = config_map_.begin();
    it = config_map_.find(conf_key);
    if( it == config_map_.end() )
        return false;
    
    conf_value = it->second; 
    return false;
}




bool CKvConfig::ProcessLine(const char * szline, std::string &okey, std::string &ovalue)
{
    if(NULL == szline) return false;
    char * pstart = (char*)szline;
    char * ppend   = (char*)szline + strlen(szline)-1;
    while( ('\r' == *ppend || '\n' == *ppend || ' ' == *ppend) && ppend > pstart )
        ppend--;
    while( ' ' == *pstart  && pstart < ppend)
        pstart++;
    if( '#' == *pstart || pstart == ppend)
        return false;
    char * split = pstart;
    while(*split != '=' && split < ppend )
        split++;
    if('=' != *split) 
        return false;
    if( pstart == split)
        return false;
    char * pend = split-1;
    while( ' ' == *pend  && pend > pstart)
        pend--;
    okey.assign(pstart, pend-pstart+1);

    pstart = split+1;
    pend   = ppend;
    while(' ' == *pstart && pstart <= ppend)
        pstart++;
    if ( pstart > ppend  )
        return false;
    ovalue.assign(pstart, pend-pstart+1);
    return true;
}
