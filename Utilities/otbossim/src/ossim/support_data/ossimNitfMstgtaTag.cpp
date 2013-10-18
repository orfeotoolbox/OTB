//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: MSTGTA Mission Target Information Extension Format
// tag class definition.
//
// See document STDI-0002 Table 8-16 for more info.
// 
//----------------------------------------------------------------------------
// $Id

#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>

#include <ossim/support_data/ossimNitfMstgtaTag.h>


RTTI_DEF1(ossimNitfMstgtaTag, "ossimNitfMstgtaTag", ossimNitfRegisteredTag);

ossimNitfMstgtaTag::ossimNitfMstgtaTag()
   : ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfMstgtaTag::~ossimNitfMstgtaTag()
{
}

std::string ossimNitfMstgtaTag::getRegisterTagName() const
{
   return std::string("MSTGTA");
}

void ossimNitfMstgtaTag::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theTgtNum, TGT_NUM_SIZE);
   in.read(theTgtId, TGT_ID_SIZE);
   in.read(theTgtBe, TGT_BE_SIZE);
   in.read(theTgtPri, TGT_PRI_SIZE);
   in.read(theTgtReq, TGT_REQ_SIZE);
   in.read(theTgtLtiov, TGT_LTIOV_SIZE);
   in.read(theTgtType, TGT_TYPE_SIZE);
   in.read(theTgtColl, TGT_COLL_SIZE);
   in.read(theTgtCat, TGT_CAT_SIZE);
   in.read(theTgtUtc, TGT_UTC_SIZE);
   in.read(theTgtElev, TGT_ELEV_SIZE);
   in.read(theTgtElevUnit, TGT_ELEV_UNIT_SIZE);
   in.read(theTgtLoc, TGT_LOC_SIZE);
}

void ossimNitfMstgtaTag::writeStream(std::ostream& out)
{
   out.write(theTgtNum, TGT_NUM_SIZE);
   out.write(theTgtId, TGT_ID_SIZE);
   out.write(theTgtBe, TGT_BE_SIZE);
   out.write(theTgtPri, TGT_PRI_SIZE);
   out.write(theTgtReq, TGT_REQ_SIZE);
   out.write(theTgtLtiov, TGT_LTIOV_SIZE);
   out.write(theTgtType, TGT_TYPE_SIZE);
   out.write(theTgtColl, TGT_COLL_SIZE);
   out.write(theTgtCat, TGT_CAT_SIZE);
   out.write(theTgtUtc, TGT_UTC_SIZE);
   out.write(theTgtElev, TGT_ELEV_SIZE);
   out.write(theTgtElevUnit, TGT_ELEV_UNIT_SIZE);
   out.write(theTgtLoc, TGT_LOC_SIZE);
}

ossim_uint32 ossimNitfMstgtaTag::getSizeInBytes()const
{
   return 101;
}

void ossimNitfMstgtaTag::clearFields()
{
   // BCS-N's to '0's, BCS-A's to ' '(spaces)

   // clear
   memset(theTgtNum, '0', TGT_NUM_SIZE);
   memset(theTgtId, ' ', TGT_ID_SIZE);
   memset(theTgtBe, ' ', TGT_BE_SIZE);
   memset(theTgtPri, ' ', TGT_PRI_SIZE);
   memset(theTgtReq, ' ', TGT_REQ_SIZE);
   memset(theTgtLtiov, ' ', TGT_LTIOV_SIZE);
   memset(theTgtType, ' ', TGT_TYPE_SIZE);
   memset(theTgtColl, '0', TGT_COLL_SIZE);
   memset(theTgtCat, ' ', TGT_CAT_SIZE);
   memset(theTgtUtc, ' ', TGT_UTC_SIZE);
   memset(theTgtElev, ' ', TGT_ELEV_SIZE);
   memset(theTgtElevUnit, ' ', TGT_ELEV_UNIT_SIZE);
   memset(theTgtLoc, ' ', TGT_LOC_SIZE);

   // null terminate
   theTgtNum[TGT_NUM_SIZE] = '\0';
   theTgtId[TGT_ID_SIZE] = '\0';
   theTgtBe[TGT_BE_SIZE] = '\0';
   theTgtPri[TGT_PRI_SIZE] = '\0';
   theTgtReq[TGT_REQ_SIZE] = '\0';
   theTgtLtiov[TGT_LTIOV_SIZE] = '\0';
   theTgtType[TGT_TYPE_SIZE] = '\0';
   theTgtColl[TGT_COLL_SIZE] = '\0';
   theTgtCat[TGT_CAT_SIZE] = '\0';
   theTgtUtc[TGT_UTC_SIZE] = '\0';
   theTgtElev[TGT_ELEV_SIZE] = '\0';
   theTgtElevUnit[TGT_ELEV_UNIT_SIZE] = '\0';
   theTgtLoc[TGT_LOC_SIZE] = '\0';
}

std::ostream& ossimNitfMstgtaTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "TGT_NUM:" << theTgtNum << "\n"
       << pfx << std::setw(24) << "TGT_ID:" << theTgtId << "\n"
       << pfx << std::setw(24) << "TGT_BE:" << theTgtBe << "\n"
       << pfx << std::setw(24) << "TGT_PRI:" << theTgtPri << "\n"
       << pfx << std::setw(24) << "TGT_REQ:" << theTgtReq << "\n"
       << pfx << std::setw(24) << "TGT_LTIOV:" << theTgtLtiov << "\n"
       << pfx << std::setw(24) << "TGT_TYPE:" << theTgtType << "\n"
       << pfx << std::setw(24) << "TGT_COLL:" << theTgtColl << "\n"
       << pfx << std::setw(24) << "TGT_CAT:" << theTgtCat << "\n"
       << pfx << std::setw(24) << "TGT_UTC:" << theTgtUtc << "\n"
       << pfx << std::setw(24) << "TGT_ELEV:" << theTgtElev << "\n"
       << pfx << std::setw(24) << "TGT_ELEV_UNIT:" << theTgtElevUnit << "\n"
       << pfx << std::setw(24) << "TGT_LOC:" << theTgtLoc << "\n";
   
   return out;
}
