//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfMaskSubsection.h 11691 2007-09-09 20:02:30Z dburken $
#ifndef ossimRpfMaskSubsection_HEADER
#define ossimRpfMaskSubsection_HEADER
#include <iostream>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfMaskSubheader;
class ossimRpfImageDescriptionSubheader;

class OSSIM_DLL ossimRpfMaskSubsection
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRpfMaskSubsection& data);
   ossimRpfMaskSubsection();
   virtual ~ossimRpfMaskSubsection();

   ossimErrorCode parseStream(std::istream& in, ossimByteOrder byteOrder);
   virtual void print(std::ostream& out)const;
   
private:
   ossimRpfMaskSubheader *theMaskSubheader;   
};

#endif
