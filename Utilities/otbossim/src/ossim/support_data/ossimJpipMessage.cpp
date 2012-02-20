#include <ossim/support_data/ossimJpipMessage.h>

ossimJpipMessage::ossimJpipMessage()
:m_header(new ossimJpipMessageHeader())
{    
}

void ossimJpipMessage::reset()
{
    m_header->reset();
    m_messageBody.clear();
    m_headerLength = 0;
    
}

void ossimJpipMessage::dumpBodyAsHex(std::ostream& out)const
{
    ossim_uint32 idx = 0;
    for(idx = 0; idx < m_messageBody.size();++idx)
    {
        std::cout << std::hex<<(int)m_messageBody[idx]<< " " ;
        if((idx+1)%16 == 0)
        {
            out << "\n";
        }
    }
}
void ossimJpipMessage::dumpBodyAsText(std::ostream& out)const
{
    ossim_uint32 idx = 0;
    for(idx = 0; idx < m_messageBody.size();++idx)
    {
        out << (char)m_messageBody[idx];
    }
}
