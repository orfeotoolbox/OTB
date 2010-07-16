#ifndef ossimRpfFrameFileReader_HEADER
#define ossimRpfFrameFileReader_HEADER

#include <iosfwd>

#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/support_data/ossimRpfHeader.h>

class ossimRpfFrameFileReader
{
public:
   friend std::ostream& operator<<(std::ostream& out, const ossimRpfFrameFileReader& data);
   
   ossimRpfFrameFileReader();
   ~ossimRpfFrameFileReader();
   
   ossimErrorCode parseFile(const ossimFilename& fileName);
   std::ostream& print(std::ostream& out) const;
   const ossimRpfHeader* getRpfHeader()const;
   
private:
   void clearAll();
   
   ossimRefPtr<ossimRpfHeader> theRpfHeader;
   ossimFilename               theFilename;
};

#endif
