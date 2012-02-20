#ifndef ossimWebResponse_HEADER
#define ossimWebResponse_HEADER
#include <ossim/base/ossimObject.h>
#include <iostream>
#include <vector>

class OSSIM_DLL ossimWebResponse : public ossimObject
{
public:
   typedef std::vector<char> ByteBuffer;
   ossimWebResponse():ossimObject(){}
   
   /**
    * This might have to change in the future if we start doing other protocols 
    * such as ftp scp, ... etc.
    *
    * We currently will return the main input stream that can be used to read the 
    * bytes returned from the request.  If you need access to say a header stream 
    * from an http protocol then you can dynamic_cast to an ossimHttpResponse.
    */
   virtual std::istream* getInputStream()=0;

   void copyAllDataFromInputStream(ByteBuffer& buffer);
   
TYPE_DATA;
};
#endif
