#pragma once
#include<iostream>
#include<vector>
#include "mqtt/async_client.h"
#include "msgpumpconf.h"
using namespace std;
class callback: public virtual mqtt::callback
{
public:
    void connected(const string& casue ) override;
    void connection_lost(const string& cause) override;
    void delivery_complete(mqtt::delivery_token_ptr tok) override;
};

class action_listener:public virtual mqtt::iaction_listener
{
protected:
    void on_failure( const mqtt::token& tok ) override;
    void on_success( const mqtt::token& tok ) override;
};


// 消息引擎，负责消息代理连接，消息构建，消息发布
class MsgEngine
{

public:
    //MsgEngine( string _server_address, string _port, string _user, string _pwd );
    MsgEngine();
    ~MsgEngine();

    // 初始化信息发布客户端
    void init();

    // 与代理进行连接
    bool connect();
    // 与代理断开
    bool disconnect();
    
    // 信息发布处理
    bool publish();

    // 设定主题，注意一个泵一个主题
    void setTopic(string _topic) { m_topic = _topic;}
    string getTopic() const { return m_topic;}

private:
    // 构建发布信息载体
    string buildPayload();

private:

    string  m_topic;
    int m_qos;
    
    string m_server_address;
    string m_port;
    string m_user;
    string m_pwd;
    string m_cliendid;
    string m_presist_dir;

    string m_lastwords;

private:
    // 引擎核心机
    mqtt::async_client_ptr m_client;

    callback m_conn_cb;
    action_listener m_act_listener;
    

};
