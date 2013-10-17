#ifndef ossimJpipMessageDecoder_HEADER
#define ossimJpipMessageDecoder_HEADER 1
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimByteStreamBuffer.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimException.h>
#include <ossim/support_data/ossimJpipMessage.h>

class OSSIM_DLL ossimJpipMessageDecoder : public ossimReferenced
{
public:
    ossimJpipMessageDecoder();
    ossimByteStreamBuffer& inputStreamBuffer(){return m_streamBuffer;}
    std::istream& inputStream()const{return m_inputStream;}
    
    // virtual ossimJpipMessage* readMessage() throw(ossimException);
    virtual ossimJpipMessage* readMessage();
    
protected:
    /**
     * Reads a Variable-length Byte-Aligned Segment. 
     * 
     * @return the value which has been read from the VBAS.
     * 
     * @throws ossimException if an I/O error occurs.
     */
    // ossim_int64 readVBAS() throw(ossimException);
    ossim_int64 readVBAS();
    
    // ossimJpipMessage* readEORMessage() throw(ossimException);
    ossimJpipMessage* readEORMessage(); 
    
    ossim_int32 m_lastClass;
    
    /**
     * Contains the last CSn value. It is a state variables used when dependent
     * form is used.
     */
    ossim_int32 m_lastCSn;
    
    /**
     * Indicates the length of the JPIP message header. This attribute is
     * useful for statistics.
     */
    ossim_int64 m_headerLength;
    
    mutable ossimByteStreamBuffer m_streamBuffer;
    mutable std::istream m_inputStream;
};

#endif
