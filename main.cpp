#include<vector>
#include<string>
#include<chrono>
#include<set>
#include"msgengine.h"
#include"msgpump.h"
#include"msgpumpconf.h"
using namespace std;


MsgPumpConfig g_msgpump_conf;


int main( void )
{
   
   // 初始化发布端
   
   bool ret = g_msgpump_conf.readConfig();
   if( !ret )
   {
       cout <<"初始化消息泵配置文件错误"<<endl;
       exit(-1);
   }
   {
       cout <<"初始化消息泵配置文件...OK"<<endl;
   }
   
#if 1
     MsgPump msg_pump;

    int engine_sz = msg_pump.assemblyMsgEngine();
    if( engine_sz > 0 )
    {
        cout <<"There are "<<engine_sz<<" engines assemblied.....OK" << endl;
        msg_pump.start();
    }
    
   msg_pump.stop();


#else

    

    const string address("tcp://172.25.113.48:1883" );
    const string clientid("paho_cpp_async_publish");
    const string PRESIST_DIR("./persist" );
    const string TOPIC("Helo-Myms");
    const char* LWT_PAYLOAD = "This is my payload, so so ..... testament";
    const char* PAYLOAD1 = "lifei:Helo World!";
    const char* PAYLOAD2 = "Hi I have been there!";
    const char* PAYLOAD3 = "O? Is anyone listening?";
    int QOS = 1;
    
    
    string cliendid1 = "cli_"+to_string((uintmax_t)time(NULL))+"_"+to_string(1);
    string cliendid2 = "cli_"+to_string((uintmax_t)time(NULL))+"_"+to_string(2);

    cout <<"Initializing for server "<<  address << "...."<<endl;    
    mqtt::async_client_ptr client,client1;
    client = std::make_shared<mqtt::async_client>(address, cliendid1 );
    client1 = std::make_shared<mqtt::async_client>(address, cliendid2 );

    callback cb;
    client->set_callback(cb);
    client1->set_callback(cb);

    auto connOpts = mqtt::connect_options_builder().clean_session()
    .will( mqtt::message(TOPIC, LWT_PAYLOAD, QOS )).user_name("lifei").password("lifei").finalize();
    
    try{
    {
        cout <<"\nConnecting client1..."<<endl;
        //mqtt::token_ptr conntok = 
        client->connect( connOpts )->wait();
        cout <<"Waiting for the connection...";
        //conntok->wait();
        cout<<"....OK" <<endl;

    }
    {
        cout <<"\nConnecting client2..."<<endl;
        //mqtt::token_ptr conntok = 
        client1->connect( connOpts )->wait();
        cout <<"Waiting for the connection...";
        //conntok->wait();
        cout<<"....OK" <<endl;

    }
        

        

        // cout <<"\nSending message....." << endl;
        // mqtt::message_ptr  pubmsg = mqtt::make_message(TOPIC, PAYLOAD1);
        // pubmsg->set_qos(QOS);
        // client.publish( pubmsg )->wait_for(TIMEOUT);
        // cout<<"sent ....OK"<<endl;

        // cout <<"\nSending next message ......"<<endl;
        // mqtt::delivery_token_ptr pubtok;
        // pubtok = client.publish( TOPIC, PAYLOAD2, strlen(PAYLOAD2),QOS, false );
        // cout <<"...with tocken:"<< pubtok->get_message_id() << endl;
        // cout <<"...for message with:"<< pubtok->get_message()->get_payload().size()<<" bytes" << endl;
        // pubtok->wait_for(TIMEOUT);
        // cout <<"sent ....OK" <<endl;

        // cout <<"\nSending next message ......" << endl;
        // action_listener listener;
        // mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, PAYLOAD3);
        // mqtt::delivery_token_ptr pubtok = client->publish( pubmsg, nullptr, listener );
        // pubtok->wait();
        // cout <<"sent ....OK" <<endl;



        client->disconnect()->wait();
        client1->disconnect()->wait();
        cout<<"....finished"<<endl;
    }
    catch( const mqtt::exception& exc )
    {
        cerr << exc.what() << exc.error_str << endl;
        
        return 1;
    }
    #endif
    
    return 1;
}