#include "msgengine.h"
#include<set>

// 信息泵
// 按照定时发布消息
extern class MsgPumpConfig g_msgpump_conf;

//#define DEB_STDOUT

set<int> g_client_ids;

 void callback::connected(const string& casue )
 {
     cout <<"Connected:"<< casue << endl;
 }
 void callback::connection_lost(const string& cause)
 {
    cout <<"\nConnection lost" << endl;
    if( !cause.empty() )
    {
        cout <<"\tcause:"<< cause << endl;
    }

 }
     
        
void callback::delivery_complete(mqtt::delivery_token_ptr tok)
{
    cout << "\tDelivery complete for token: "
    << (tok ? tok->get_message_id() : -1) << endl;
    

}

void action_listener::on_failure( const mqtt::token& tok )
{
    cout <<"\tListener failure for token:"<< tok.get_message_id() << endl;

}    
        
void action_listener::on_success( const mqtt::token& tok )
{
    cout <<"[" << tok.get_message_id() <<"] ";
}


// 消息引擎，负责消息代理连接，消息构建，消息发布

#if 0
MsgEngine::MsgEngine( string _server_address, string _port, string _user, string _pwd )
{
    m_server_address = "tcp://" + _server_address + ":" + _port;
    m_user = _user;
    m_pwd  = _pwd;
    
    int i_cid = g_client_ids.size()+1;
    
    m_cliendid = "cli_"+to_string((uintmax_t)time(NULL))+"_"+to_string(i_cid);
    
    g_client_ids.insert( i_cid );

    m_qos = 1;
    m_lastwords = "Ohh, leave!";
    m_presist_dir = "./persist";

    init();

    
    #ifdef DEB_STDOUT
    cout <<"Publish:"<<endl;
    cout <<"=========================\n";
    cout <<m_server_address<<endl;
    cout <<"user:"<<m_user<<"  password:" << m_pwd <<endl;
    cout <<"client tag:"<< m_cliendid << endl;
    #endif
}
#endif
MsgEngine::MsgEngine( )
{    

    init();

    #ifdef DEB_STDOUT
    cout <<"Publish:"<<endl;
    cout <<"=========================\n";
    cout <<m_server_address<<endl;
    cout <<"user:"<<m_user<<"  password:" << m_pwd <<endl;
    cout <<"client tag:"<< m_cliendid << endl;
    #endif
}

MsgEngine::~MsgEngine()
{
}
    
 void MsgEngine::init()
{
    auto broker_map = g_msgpump_conf.getBrokerConf();

    string broke_ip = broker_map["ip"];
    string broke_port = broker_map["port"];
    string username = broker_map["user"];
    string pwd = broker_map["pwd"];    

    m_server_address = "tcp://" +broke_ip + ":" + broke_port;    
    m_user = username;
    m_pwd  = pwd;
    
    int i_cid = g_client_ids.size()+1;    
    m_cliendid = "cli_"+to_string((uintmax_t)time(NULL))+"_"+to_string(i_cid);    
    g_client_ids.insert( i_cid );

    m_qos = 1;
    m_lastwords = "Ohh, leave!";
    m_presist_dir = "./persist";

    // 构建信息发布客户
    m_client = std::make_shared< mqtt::async_client>( m_server_address, m_cliendid) ;    
    

    m_client->set_callback( m_conn_cb );        
}

bool MsgEngine::connect()
{
    const char* LWT_PAYLOAD = "This is my payload, so so ..... testament";
        
    auto connOps = mqtt::connect_options_builder().clean_session().will( mqtt::message( m_topic,LWT_PAYLOAD, m_qos )).user_name(m_user).password(m_pwd).finalize();     
    
    #ifdef DEB_STDOUT
    cout <<"Connect info:"<<endl;
    cout <<"Topic:"<< m_topic<<endl;
    cout <<"User:"<< m_user<<endl;
    cout <<"Pwd:"<< m_pwd<<endl;
    cout <<"Lastword:"<< m_lastwords<<endl;
    cout <<"QOS:"<< m_qos<<endl;
    cout << m_client->get_server_uri() << endl;
    cout << m_client->get_client_id() << endl;         
    #endif

    cout <<"To connect "<< m_topic.c_str() <<" channel......."; 
    
    
     try{        
    
         m_client->connect( connOps )->wait();     
         cout<<"...OK" <<endl;          
    
     }
     catch(const mqtt::exception& exc )
     {
         cerr << "Exception>>>>>>>>>>>:" <<exc.what() << exc.error_str << endl;
         return false;
     }

    return true;
}

bool MsgEngine::disconnect()
{
    m_client->disconnect()->wait();
    cout<<"....finished"<<endl;
    return true;
}

bool MsgEngine::publish( )
{
    try{
        string payload = buildPayload();
        cout <<"\nSending message ...{"<<payload<<"}"<<endl;;
        
        //action_listener listener;
        mqtt::message_ptr pubmsg = mqtt::make_message( m_topic.c_str(), payload.c_str() );
        
        mqtt::delivery_token_ptr pubtok = m_client->publish( pubmsg, nullptr, m_act_listener );        
        
        pubtok->wait();
        

        return true;
    }
    
    catch(const mqtt::exception& exc )
    {
        cerr << exc.what() << exc.error_str << endl;
        return false;
    }

}


string MsgEngine::buildPayload( )
{
    string cur_tm = to_string((uintmax_t)time(NULL) );
    string result = m_topic + " Now time is:";
    result+=cur_tm;            
    return result;
}
    