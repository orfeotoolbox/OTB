//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CSCRNA tag class definition.
//
// Corner Footprint TRE.
//
// See document STDI-0006-NCDRD Table 3.2-13 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfCscrnaTag.h>


RTTI_DEF1(ossimNitfCscrnaTag, "ossimNitfCscrnaTag", ossimNitfRegisteredTag);

ossimNitfCscrnaTag::ossimNitfCscrnaTag()
   : ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfCscrnaTag::~ossimNitfCscrnaTag()
{
}

std::string ossimNitfCscrnaTag::getRegisterTagName() const
{
   return std::string("CSCRNA");
}

void ossimNitfCscrnaTag::parseStream(std::istream& in)
{
   clearFields();

   in.read(thePredictedCornerFlag, 1);

   in.read(theUlLat, 9);
   in.read(theUlLon, 10);
   in.read(theUlHt, 8);

   in.read(theUrLat, 9);
   in.read(theUrLon, 10);
   in.read(theUrHt, 8);

   in.read(theLrLat, 9);
   in.read(theLrLon, 10);
   in.read(theLrHt, 8);

   in.read(theLlLat, 9);
   in.read(theLlLon, 10);
   in.read(theLlHt, 8);
}

void ossimNitfCscrnaTag::writeStream(std::ostream& out)
{
   out.write(thePredictedCornerFlag, 1);

   out.write(theUlLat, 9);
   out.write(theUlLon, 10);
   out.write(theUlHt, 8);

   out.write(theUrLat, 9);
   out.write(theUrLon, 10);
   out.write(theUrHt, 8);

   out.write(theLrLat, 9);
   out.write(theLrLon, 10);
   out.write(theLrHt, 8);

   out.write(theLlLat, 9);
   out.write(theLlLon, 10);
   out.write(theLlHt, 8);
}

ossim_uint32 ossimNitfCscrnaTag::getSizeInBytes()const
{
   return 109;
}

void ossimNitfCscrnaTag::clearFields()
{
   // BCS-N's to '0's, BCS-A's to ' '(spaces)
   memset(thePredictedCornerFlag, ' ', 1);
   
   memset(theUlLat, '0', 9);
   memset(theUlLon, '0', 10);
   memset(theUlHt, '0', 8);

   memset(theUrLat, '0', 9);
   memset(theUrLon, '0', 10);
   memset(theUrHt, '0', 8);

   memset(theLrLat, '0', 9);
   memset(theLrLon, '0', 10);
   memset(theLrHt, '0', 8);

   memset(theLlLat, '0', 9);
   memset(theLlLon, '0', 10);
   memset(theLlHt, '0', 8);

   thePredictedCornerFlag[1] = '\0';
   
   theUlLat[9] = '\0';
   theUlLon[10] = '\0';
   theUlHt[8] = '\0';

   theUrLat[9] = '\0';
   theUrLon[10] = '\0';
   theUrHt[8] = '\0';

   theLrLat[9] = '\0';
   theLrLon[10] = '\0';
   theLrHt[8] = '\0';

   theLlLat[9] = '\0';
   theLlLon[10] = '\0';
   theLlHt[8] = '\0';
}

std::ostream& ossimNitfCscrnaTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
      
       << pfx << std::setw(24) << "PREDICT_CORNERS:"
       << thePredictedCornerFlag << "\n"
      
       << pfx << std::setw(24) << "ULCRN_LAT:" << theUlLat << "\n"
       << pfx << std::setw(24) << "ULCRN_LON:" << theUlLon << "\n"
       << pfx << std::setw(24) << "ULCRN_HT:" << theUlHt << "\n"

       << pfx << std::setw(24) << "URCRN_LAT:" << theUrLat << "\n"
       << pfx << std::setw(24) << "URCRN_LON:" << theUrLon << "\n"
       << pfx << std::setw(24) << "URCRN_HT:" << theUrHt << "\n"
   
       << pfx << std::setw(24) << "LRCRN_LAT:" << theLrLat << "\n"
       << pfx << std::setw(24) << "LRCRN_LON:" << theLrLon << "\n"
       << pfx << std::setw(24) << "LRCRN_HT:" << theLrHt << "\n"
      
       << pfx << std::setw(24) << "LLCRN_LAT:" << theLlLat << "\n"
       << pfx << std::setw(24) << "LLCRN_LON:" << theLlLon << "\n"
       << pfx << std::setw(24) << "LLCRN_HT:" << theLlHt << "\n";

      return out;
}
