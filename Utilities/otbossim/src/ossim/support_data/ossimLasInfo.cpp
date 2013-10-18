//----------------------------------------------------------------------------
//
// File: ossimLibLasInfo.cpp
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: LAS LIDAR info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimLasInfo.h>
#include <ossim/support_data/ossimLasHdr.h>

#include <fstream>

ossimLasInfo::ossimLasInfo()
   : m_file()
{
}

ossimLasInfo::~ossimLasInfo()
{
}

bool ossimLasInfo::open(const ossimFilename& file)
{
   bool result = false;
   std::ifstream istr;
   istr.open(file.c_str(), std::ios_base::in | std::ios_base::binary);
   if ( istr.is_open() )
   {
      ossimLasHdr hdr;
      result = hdr.checkSignature(istr);
      if (result) m_file = file;
   }
   return result;
}

std::ostream& ossimLasInfo::print(std::ostream& out) const
{
   std::ifstream istr;
   istr.open(m_file.c_str(), std::ios_base::in | std::ios_base::binary);
   if ( istr.is_open() )
   {
      ossimLasHdr hdr;
      if ( hdr.checkSignature(istr) )
      {
         hdr.readStream(istr);
         hdr.print(out);
      }
   }
   return out;
}
