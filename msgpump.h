#pragma once
#include<vector>
#include<iostream>
#include<ev.h>
#include"msgpumpconf.h"
#include"msgengine.h"


using namespace std;
// 信息泵
// 按照定时发布消息
class MsgPump
{
public:
    MsgPump( int _start_tm = 0 );
     
        
protected:
    void init();    

public:
    // 装配好消息引擎
    int assemblyMsgEngine( );
    
    // 组装好的引擎启动
    void start();
    
    // 停机
    void stop();
    
protected:

    // 定时回调函数
    static void pump_cb(EV_P_ ev_timer* w, int revents);
    
    
    protected:
    EV_P;
    
    // 监听器
    // key:topic
    // value:监听者    
    vector<ev_timer> m_rep_watcher_map;   

    int m_start_tm;
    int m_repeate_tm;

    private:

    // 引擎集
    static vector<MsgEngine> m_msg_engines;

};
