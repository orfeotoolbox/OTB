#include <ossim/support_data/ossimNitfCsproaTag.h>
#include <iomanip>
#include <iostream>

ossimNitfCsproaTag::ossimNitfCsproaTag()
   : ossimNitfRegisteredTag(std::string("CSPROA"), 120)
{
   clearFields();
}

void ossimNitfCsproaTag::parseStream(std::istream& in)
{
   in.read(m_reserved1, RESERVE1_SIZE);
   in.read(m_reserved2, RESERVE2_SIZE);
   in.read(m_reserved3, RESERVE3_SIZE);
   in.read(m_reserved4, RESERVE4_SIZE);
   in.read(m_reserved5, RESERVE5_SIZE);
   in.read(m_reserved6, RESERVE6_SIZE);
   in.read(m_reserved7, RESERVE7_SIZE);
   in.read(m_reserved8, RESERVE8_SIZE);
   in.read(m_reserved9, RESERVE9_SIZE);
   in.read(m_bwc, BWC_SIZE);
}

void ossimNitfCsproaTag::writeStream(std::ostream& out)
{
   out.write(m_reserved1, RESERVE1_SIZE);
   out.write(m_reserved2, RESERVE2_SIZE);
   out.write(m_reserved3, RESERVE3_SIZE);
   out.write(m_reserved4, RESERVE4_SIZE);
   out.write(m_reserved5, RESERVE5_SIZE);
   out.write(m_reserved6, RESERVE6_SIZE);
   out.write(m_reserved7, RESERVE7_SIZE);
   out.write(m_reserved8, RESERVE8_SIZE);
   out.write(m_reserved9, RESERVE9_SIZE);
   out.write(m_bwc, BWC_SIZE);
}

std::ostream& ossimNitfCsproaTag::print(std::ostream& out,
                            const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getTagLength();
   pfx += ".";
   out << setiosflags(ios::left)
       << pfx << std::setw(24) << "CETAG:"      << getTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"        << getTagLength() << "\n"
       << pfx << std::setw(24) << "RESERVED1:"   << m_reserved1 << "\n"
       << pfx << std::setw(24) << "RESERVED2:"   << m_reserved2 << "\n"
       << pfx << std::setw(24) << "RESERVED3:"   << m_reserved3 << "\n"
       << pfx << std::setw(24) << "RESERVED4:"   << m_reserved4 << "\n"
       << pfx << std::setw(24) << "RESERVED5:"   << m_reserved5 << "\n"
       << pfx << std::setw(24) << "RESERVED6:"   << m_reserved6 << "\n"
       << pfx << std::setw(24) << "RESERVED7:"   << m_reserved7 << "\n"
       << pfx << std::setw(24) << "RESERVED8:"   << m_reserved8 << "\n"
       << pfx << std::setw(24) << "RESERVED9:"   << m_reserved9 << "\n"
       << pfx << std::setw(24) << "BWC:"   << m_bwc << "\n";

    return out;   
}

void ossimNitfCsproaTag::clearFields()
{
   memset(m_reserved1, ' ', RESERVE1_SIZE);
   memset(m_reserved2, ' ', RESERVE2_SIZE);
   memset(m_reserved3, ' ', RESERVE3_SIZE);
   memset(m_reserved4, ' ', RESERVE4_SIZE);
   memset(m_reserved5, ' ', RESERVE5_SIZE);
   memset(m_reserved6, ' ', RESERVE6_SIZE);
   memset(m_reserved7, ' ', RESERVE7_SIZE);
   memset(m_reserved8, ' ', RESERVE8_SIZE);
   memset(m_reserved9, ' ', RESERVE9_SIZE);
   memset(m_bwc, ' ', BWC_SIZE);

   char reserve1[]="LATESTCAL";
   memcpy(m_reserved1, reserve1, 9);
   char reserve4[] = "MARKANDFIX";
   memcpy(m_reserved4, reserve4, 10);
   char reserve7[] = "SKIPAGM";
   memcpy(m_reserved7, reserve7, 7);
   char reserve8[] = "INTERP";
   memcpy(m_reserved8, reserve8, 6);

   m_reserved1[RESERVE1_SIZE] = '\0';
   m_reserved2[RESERVE2_SIZE] = '\0';
   m_reserved3[RESERVE3_SIZE] = '\0';
   m_reserved4[RESERVE4_SIZE] = '\0';
   m_reserved5[RESERVE5_SIZE] = '\0';
   m_reserved6[RESERVE6_SIZE] = '\0';
   m_reserved7[RESERVE7_SIZE] = '\0';
   m_reserved8[RESERVE8_SIZE] = '\0';
   m_reserved9[RESERVE9_SIZE] = '\0';
   m_bwc[BWC_SIZE] = '\0';
}
