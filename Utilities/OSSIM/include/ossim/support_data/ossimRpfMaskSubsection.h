//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfMaskSubsection.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfMaskSubsection_HEADER
#define ossimRpfMaskSubsection_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimErrorContext.h>

class ossimRpfMaskSubheader;
class ossimRpfImageDescriptionSubheader;

class ossimRpfMaskSubsection
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfMaskSubsection& data);
   ossimRpfMaskSubsection();
   virtual ~ossimRpfMaskSubsection();

   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   virtual void print(ostream& out)const;
   
private:
   void deleteAll();
   ossimRpfMaskSubheader *theMaskSubheader;   
};

#endif
