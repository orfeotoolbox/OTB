#ifndef ossimProtocolStream_HEADER
#define ossimProtocolStream_HEADER
#include <ossim/base/ossimStreamBase.h>

class ossimProtocolStream : public ossimStreamBase
{
public:
   ossimProtocolStream(){}
   ossimProtocolStream(std::streambuf* buf):ossimStreamBase(buf)
      {
      }

      /**
       * This is a generic interface to any stream for opening and closing
       * If supported this could be an interface to not only file streams
       * but also http, https, ftp, ... etc
       */
   virtual void open(const char* protocolString,
                     int openMode=0)=0;

   /**
    * Closes the stream
    */
   virtual void close()=0;

   
   virtual bool is_open()const=0;

   virtual bool isCompressed()const=0;
};

#endif
