#include"msgpump.h"
// 信息泵
// 按照定时发布消息
extern class MsgPumpConfig g_msgpump_conf;

vector<MsgEngine> MsgPump::m_msg_engines;


MsgPump::MsgPump( int _start_tm)
{
    m_start_tm = _start_tm;
    
    init();
}
void MsgPump::init()
{        
    EV_A = EV_DEFAULT;
    auto channel_map = g_msgpump_conf.getChannelConf();
    m_rep_watcher_map.resize( channel_map.size() );
    int i = 0;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    {
        auto& watcher_act = m_rep_watcher_map[i];
        
        watcher_act.data = reinterpret_cast<void*>(i);
        
        ev_timer_init( &watcher_act, pump_cb, m_start_tm, itr->second );
    } 
}

    // 装配好消息引擎
int MsgPump::assemblyMsgEngine( )
{
    int success = 0;
    // 通过配置文件获取服务地址，IP 密码等
    // 以及配套的主题
    // 这里先简单设定固定值

    auto broker_map = g_msgpump_conf.getBrokerConf();
    auto channel_map = g_msgpump_conf.getChannelConf();
    
    string broke_ip = broker_map["ip"];
    string broke_port = broker_map["port"];
    string username = broker_map["user"];
    string pwd = broker_map["pwd"];
    

    //组装引擎    
    cout <<"Channel size:" << m_msg_engines.size() <<endl;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr )
    {
        auto engine = MsgEngine(broke_ip, broke_port, username, pwd);
        engine.setTopic( itr->first );        
        m_msg_engines.push_back( engine );        
        cout <<"create engine:"<<" "<<itr->first<<" tm:"<<itr->second<<endl;
    }
    // 完成引擎连接
    for( int i = 0; i < m_msg_engines.size(); i++ )
    {
        auto& engine = m_msg_engines[i];
        if( !engine.connect() )
        {            
            cout<<"Assmbly MsgEngine failed!:"<< i << endl;                
        }
        else{            
            success++;
        }
    }

    cout <<"had assmblied "<< success <<" successfuly...." <<endl;
    return success;
    
}

void MsgPump::start()
{
    for( auto itr = m_rep_watcher_map.begin(); itr != m_rep_watcher_map.end(); ++itr )
    {
        auto& ev_tm = *itr;
        ev_timer_start( EV_A_ &ev_tm);
    }   
    
    ev_run(EV_A_ 0 );
}

void MsgPump::stop()
{
    for_each( m_msg_engines.begin(), m_msg_engines.end(),[](MsgEngine& ele){
        ele.disconnect();
    });        
}

void MsgPump::pump_cb(EV_P_ ev_timer* w, int revents)
{        
    int * ptopic_id = reinterpret_cast<int*>(w->data);
    long  topic_id  = reinterpret_cast<long>(ptopic_id);
    cout <<"topic no:"<<topic_id;
    m_msg_engines[topic_id].publish();        
}    
    