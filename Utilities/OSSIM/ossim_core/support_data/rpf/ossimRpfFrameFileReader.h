#ifndef ossimRpfFrameFileReader_HEADER
#define ossimRpfFrameFileReader_HEADER
#include <iostream>
#include <fstream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

#include "base/data_types/ossimFilename.h"

class ossimRpfHeader;
class ossimRpfImageDescriptionSubheader;
class ossimRpfMaskSubsection;
class ossimRpfCompressionSectionSubheader;
class ossimRpfAttributeSectionSubheader;

class ossimRpfFrameFileReader
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfFrameFileReader& data);
   
   ossimRpfFrameFileReader();
   virtual ~ossimRpfFrameFileReader();
   
   ossimErrorCode parseFile(const ossimFilename& fileName);
   void print(ostream& out)const;
   const ossimRpfHeader* getRpfHeader()const{return theRpfHeader;}
   
private:
   void clearAll();
   
   ossimRpfHeader* theRpfHeader;
      
   ossimFilename   theFilename;
};

#endif
