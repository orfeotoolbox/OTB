#ifndef ossimNmeaMessageSequencer_HEADER
#define ossimNmeaMessageSequencer_HEADER
#include <ossim/support_data/ossimNmeaMessage.h>
#include <ossim/base/ossimFilename.h>
#include <string>

class OSSIM_DLL ossimNmeaMessageSequencer 
{
public:
   ossimNmeaMessageSequencer();
   ossimNmeaMessageSequencer(const ossimFilename& file);
   ossimNmeaMessageSequencer(const std::string& str);
   ~ossimNmeaMessageSequencer();
   
   void initialize(const ossimFilename& file);
   void initialize(const std::string& str);
   
   virtual bool next(ossimNmeaMessage& msg);
   
   virtual void reset();
   virtual bool valid()const;
   
protected:
   void destroy();
   std::istream* m_inputStream;
};
#endif
