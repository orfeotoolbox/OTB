#ifndef ossimUrl_HEADER
#define ossimUrl_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>

class OSSIM_DLL ossimUrl
{
public:
    ossimUrl();
    ossimUrl(const ossimString& url);
    ossimUrl(const ossimString& protocol,
             const ossimString& ip,
             const ossimString& port,
             const ossimString& path,
             const ossimString& params);
    const ossimString& getProtocol()const{return m_protocol;}
    const ossimString& getIp()const{return m_ip;}
    const ossimString& getPort()const{return m_port;}
    const ossimString& getPath()const{return m_path;}
    const ossimString& getParams()const{return m_params;}
    
    void setProtocol(const ossimString& protocol){m_protocol = protocol;}
    void setIp(const ossimString& ip);
    void setPort(const ossimString& port){m_port = port;}
    void setPath(const ossimString& path){m_path = path;}
    void setParams(const ossimString& params){m_params = params;}
    
    void setParams(const ossimKeywordlist& kwl);
    
    void setFieldsFromUrl(const ossimString& url);
    ossimString toString()const;
    
protected:
    ossimString m_protocol;
    ossimString m_ip;
    ossimString m_port;
    ossimString m_path;
    ossimString m_params;
};

#endif
