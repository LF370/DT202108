#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<tinyxml.h>
using namespace std;


class MsgPumpConfig
{
public:
     const map<string,string>& getBrokerConf() const { return m_broker_set; }
     const map<string,double>& getChannelConf() const  { return m_channel_map; }


public:
    bool readConfig();   


protected:    
    // 基本登录信息
    map<string,string> m_broker_set;
   

    // 信息频道
    // key: topic
    // value:repeate tm
    map<string,double> m_channel_map;


};


