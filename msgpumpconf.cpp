#include"msgpumpconf.h"


   
bool MsgPumpConfig::readConfig()
{
    TiXmlDocument doc;
    bool ret = doc.LoadFile("msgpumpconfig.xml");
    if( !ret )
    {           
        return false;
    }
    
        TiXmlDeclaration* pt = doc.ToDeclaration();

    TiXmlElement *root = doc.RootElement();
    if( !root )
    {
        return false;
    }
    
    
    TiXmlNode* broker = root->FirstChild("broker");                
        
    while( broker )
    {  
        for( TiXmlNode *child = broker->FirstChild();child; child = child->NextSibling() )
        {
            m_broker_set.insert( make_pair(child->Value(), child->ToElement()->GetText() ));                
        }
        
        broker = broker->ToElement()->NextSibling("broker");
    }

    TiXmlNode* channel = root->FirstChild("channel");
    
    while( channel )
    {
        string topic_str;
        double rep_tm;
        for( TiXmlNode *child = channel->FirstChild();child; child = child->NextSibling() )
        {
            
            string tx_v = child->Value();                
            if( tx_v =="topic")
            {
                topic_str = child->ToElement()->GetText();
            }
            if( tx_v =="reptm")
            {
                rep_tm = stod( child->ToElement()->GetText() );
            }               
            
        }
        m_channel_map.insert(make_pair(topic_str, rep_tm ));

        channel = channel->ToElement()->NextSibling("channel");
    }
    

    return true;


}


