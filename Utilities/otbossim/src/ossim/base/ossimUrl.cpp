#include <ossim/base/ossimUrl.h>

ossimUrl::ossimUrl()
{

}

ossimUrl::ossimUrl(const ossimString& url)
{
    setFieldsFromUrl(url);
}

ossimUrl::ossimUrl(const ossimString& protocol,
                   const ossimString& ip,
                   const ossimString& port,
                   const ossimString& path,
                   const ossimString& params)
:m_protocol(protocol),
m_ip(ip),
m_port(port),
m_path(path),
m_params(params)
{

}
void ossimUrl::setIp(const ossimString& ip)
{
    const std::string& s = ip.string();
    std::string::size_type pos = s.find_first_of(':');
    if(pos != std::string::npos)
    {
        m_ip = ossimString(s.begin(), s.begin()+pos);
        m_port = ossimString(s.begin()+pos+1, s.end());
    }
    else
    {
        m_ip = ip;
    }
}

void ossimUrl::setParams(const ossimKeywordlist& kwl)
{
    m_params = "";
    const ossimKeywordlist::KeywordMap& map = kwl.getMap();
    ossimKeywordlist::KeywordMap::const_iterator iter = map.begin();
    while(iter!=map.end())
    {
        if(m_params.empty())
        {
            m_params = iter->first + "=" + iter->second;
        }
        else
        {
            m_params+=("&"+iter->first + "=" + iter->second);
        }
        ++iter;
    }
}

void ossimUrl::setFieldsFromUrl(const ossimString& url)
{
    m_protocol = m_ip = m_port = m_path = m_params = "";
    
    // look for something of the form <protocol>://
    const std::string& s = url.string();
    std::string::size_type pos = s.find_first_of(":");
    
    if(pos == std::string::npos) return;
    
    m_protocol = ossimString(s.begin(), s.begin()+pos);
    
    // now find the ip and then extract port if embedded in IP
    //
    pos = s.find_first_not_of('/', pos+1);
    
    if(pos == std::string::npos) return;
    
    std::string::size_type nextPos = s.find_first_of('/', pos+1);
    
    if(nextPos == std::string::npos)
    {
        setIp(ossimString(s.begin()+pos, s.end()));
        return;
    }
    else
    {
        setIp(ossimString(s.begin()+pos, s.begin()+nextPos));
    }
    // now find the path portion
    //
    pos = s.find_first_of('?', nextPos);
    if(pos == std::string::npos)
    {
        m_path = ossimString(s.begin()+nextPos+1, s.end());
        return;
    }
    else
    {
        m_path = ossimString(s.begin()+nextPos+1, s.begin()+pos);
    }
    
    m_params = ossimString(s.begin()+pos+1, s.end());
    
}

ossimString ossimUrl::toString()const
{
    return (m_protocol+"://"+m_ip + (m_port.empty()?"":":"+m_port) +"/"+m_path + (m_params.empty()?"":("?"+m_params)));
}
