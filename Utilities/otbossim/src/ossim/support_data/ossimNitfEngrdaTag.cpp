//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ENGRDA - Engineering Data tag class declaration.
//
// See document STDI-0002 (version 3), Appendix N for more info.
// 
//----------------------------------------------------------------------------
// $Id

#include <cstring>
#include <istream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <ossim/support_data/ossimNitfEngrdaTag.h>
#include <ossim/base/ossimNotify.h>


RTTI_DEF1(ossimNitfEngrdaTag, "ossimNitfEngrdaTag", ossimNitfRegisteredTag);

ossimNitfEngrdaTag::ossimNitfEngrdaTag()
   : ossimNitfRegisteredTag()
{
   clearFields();
}

ossimNitfEngrdaTag::~ossimNitfEngrdaTag()
{
}

std::string ossimNitfEngrdaTag::getRegisterTagName() const
{
   return std::string("ENGRDA");
}

void ossimNitfEngrdaTag::parseStream(std::istream& in)
{
   clearFields();

   // theTreLength = RETAG_SIZE + REL_SIZE;
   
   in.read(theReSrc, RESRC_SIZE);
   theTreLength += RESRC_SIZE;
   
   in.read(theReCnt, RECNT_SIZE);
   theTreLength += RECNT_SIZE;

   const ossim_uint16 ELEMENT_COUNT = ossimString(theReCnt).toUInt16();
   
   for (ossim_uint16 i = 0; i < ELEMENT_COUNT; ++i)
   {
      ossimString os;
      char buf[TMP_BUF_SIZE];
      ossimEngDataElement element;

      // ENGLN - label length field
      in.read(buf, ENGLN_SIZE);
      theTreLength += ENGLN_SIZE;
      buf[ENGLN_SIZE] = '\n';
      os = buf;

      // ENGLBL - label field
      ossim_uint32 size = os.toUInt16();
      in.read(buf, size);
      theTreLength += size;
      buf[size] = '\0';
      element.theEngLbl = buf;

      // ENGMTXC - data column count
      in.read(buf, ENGMTXC_SIZE);
      theTreLength += ENGMTXC_SIZE;
      buf[ENGMTXC_SIZE] = '\0';
      os = buf;
      element.theEngMtxC = os.toUInt16();

      // ENGMTXR - data row count
      in.read(buf, ENGMTXR_SIZE);
      theTreLength += ENGMTXR_SIZE;
      buf[ENGMTXR_SIZE] = '\0';
      os = buf;
      element.theEngMtxR = os.toUInt16();

      // ENGTYP - Value Type of Engineering Data Element.
      in.get( element.theEngTyp );
      theTreLength += ENGTYP_SIZE;

      // ENGDTS - Engineering Data Element Size
      element.theEngDts = static_cast<ossim_uint8>(in.get());
      theTreLength += ENGDTS_SIZE;

      // ENGDATU - Engineering Data Units.
      in.read(buf, ENGDATU_SIZE);
      theTreLength += ENGDATU_SIZE;
      buf[ENGDATU_SIZE] = '\0';
      element.theEngDatU = buf;

      // ENGDATC - Engineering Data Count
      in.read(buf, ENGDATC_SIZE);
      theTreLength += ENGDATC_SIZE;
      buf[ENGDATC_SIZE] = '\n';
      os = buf;
      ossim_uint32 engDatC = os.toUInt32();

      // ENGDATA - Engineering Data
      element.theEngDat.resize(engDatC);
      in.read((char*)&(element.theEngDat.front()), (std::streamsize)element.theEngDat.size());
      theTreLength += engDatC;
      
      theData.push_back(element);

   } // Matches: for (ossim_uint16 i = 0; i < ELEMENT_COUNT; ++i)
}

void ossimNitfEngrdaTag::writeStream(std::ostream& out)
{
   //out.write(theAcMsnId, AC_MSN_ID_SIZE);
   // out.write(theAcTailNo, AC_TAIL_NO_SIZE);

   out.write(theReSrc, RESRC_SIZE);
   out.write(theReCnt, RECNT_SIZE);

   const ossim_uint16 ELEMENT_COUNT = ossimString(theReCnt).toUInt16();
   
   for (ossim_uint16 i = 0; i < ELEMENT_COUNT; ++i)
   {
      // ENGLN - label length field      
      std::string s;
      getValueAsString(theData[i].theEngLbl.size(), ENGLN_SIZE, s);
      out.write(s.data(), ENGLN_SIZE);

      // ENGLBL - label field
      out.write(theData[i].theEngLbl.data(), (std::streamsize)theData[i].theEngLbl.size());

      // ENGMTXC - data column count
      getValueAsString(theData[i].theEngMtxC, ENGMTXC_SIZE, s); 
      out.write(s.data(), ENGMTXC_SIZE);

      // ENGMTXR - data row count
      getValueAsString(theData[i].theEngMtxR, ENGMTXR_SIZE, s);
      out.write(s.data(), ENGMTXR_SIZE);

      // ENGTYP - Value Type of Engineering Data Element.
      out.write(&theData[i].theEngTyp, ENGTYP_SIZE);

      // ENGDTS - Engineering Data Element Size
      out.write((char*)(&theData[i].theEngDts), ENGDTS_SIZE);

      // ENGDATU - Engineering Data Units.
      out.write(theData[i].theEngDatU.data(), ENGDATU_SIZE);

      // ENGDATC - Engineering Data Count
      getValueAsString(theData[i].theEngDat.size(), ENGDATC_SIZE, s); 
      out.write(s.data(), ENGDATC_SIZE);

      // ENGDATA - Engineering Data NOTE: should be big endian...
      out.write((char*)&(theData[i].theEngDat.front()),
                (std::streamsize)theData[i].theEngDat.size());

   } // Matches: for (ossim_uint16 i = 0; i < ELEMENT_COUNT; ++i)
   
}

ossim_uint32 ossimNitfEngrdaTag::getSizeInBytes()const
{
   return theTreLength;
}

void ossimNitfEngrdaTag::clearFields()
{
   // BCS-N's to '0's, BCS-A's to ' '(spaces)

   // clear
   memset(theReSrc, ' ', RESRC_SIZE);
   memset(theReCnt, ' ', RECNT_SIZE);
   theData.clear();

   // null terminate
   theReSrc[RESRC_SIZE] = '\0';
   theReCnt[RECNT_SIZE] = '\0';

   theTreLength = 0;
}

std::ostream& ossimNitfEngrdaTag::print(
   std::ostream& out, const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"   << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "RESRC:" << theReSrc << "\n"
       << pfx << std::setw(24) << "RECNT:" << theReCnt << "\n";

   for (ossim_uint32 i = 0; i < theData.size(); ++i)
   {
      out << pfx << "ENGLBL[" << i << std::setw(24) << "]:"
          << theData[i].theEngLbl << "\n"
          << pfx << "ENGMTXC[" << i << std::setw(24) << "]:"
          << theData[i].theEngMtxC << "\n"
          << pfx << "ENGMTXR[" << i << std::setw(24) << "]:"
          << theData[i].theEngMtxR << "\n"
          << pfx << "ENGTYP[" << i << std::setw(24) << "]:"
          << theData[i].theEngTyp << "\n"
          << pfx << "ENGDTS[" << i << std::setw(24) << "]:"
          << theData[i].theEngDts << "\n"
          << pfx << "ENGDATU[" << i << std::setw(24) << "]:"
          << theData[i].theEngDatU << "\n"
          << pfx << "ENGDATC[" << i << std::setw(24) << "]:"
          << theData[i].theEngDat.size() << "\n";
      
      printData(out, theData[i], i, pfx);

      
   }
   
   return out;
}

std::ostream& ossimNitfEngrdaTag::printData(std::ostream& out,
                                            const ossimEngDataElement& element,
                                            ossim_uint32 /* elIndex */,
                                            const std::string& /* prefix */) const
{
   if (element.theEngTyp == 'A')
   {
      std::vector<ossim_uint8>::const_iterator i = element.theEngDat.begin();
      while (i != element.theEngDat.end())
      {
         out << static_cast<ossim_int8>(*i);
         ++i;
      }
      out << "\n";
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfEngrdaTag::printData WARNING unhandled data type."
         << std::endl;
   } 
   return out;
}

template <class T>
void ossimNitfEngrdaTag::getValueAsString(
   T v, ossim_uint16 w, std::string& s) const
{
   std::ostringstream os;
   os << std::setw(w) << std::setfill('0') << setiosflags(ios::right) << v;
   s = os.str();
}
   
