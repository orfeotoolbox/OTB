/*********************************************************************************************
 *
 * License: LGPL Please see the top level LICENSE.txt
 *
 * Author: Garrett Potts
 * 
 *********************************************************************************************/
#ifndef ossimJpipMessage_HEADER
#define ossimJpipMessage_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/support_data/ossimJpipMessageHeader.h>
#include <vector>
class OSSIM_DLL ossimJpipMessage : public ossimReferenced
{
public:
    typedef std::vector<ossim_uint8> MessageBodyType;
    
    ossimJpipMessage();
    void reset();
    void setHeaderLength(ossim_uint64 length){m_headerLength = length;}
    ossimJpipMessageHeader* header(){return m_header.get();}
    const ossimJpipMessageHeader* header()const{return m_header.get();}
    MessageBodyType& messageBody(){return m_messageBody;}
    const MessageBodyType& messageBody()const{return m_messageBody;}
    void dumpBodyAsHex(std::ostream& out)const;
    void dumpBodyAsText(std::ostream& out)const;
    
protected:
    ossimRefPtr<ossimJpipMessageHeader> m_header;
    ossim_uint64             m_headerLength;
    MessageBodyType m_messageBody;
};

#endif
