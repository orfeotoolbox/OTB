//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ENVI Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimEnviInfo.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/support_data/ossimEnviHeader.h>
#include <iostream>

ossimEnviInfo::ossimEnviInfo()
   : ossimInfoBase(),
     m_file()
{
}

ossimEnviInfo::~ossimEnviInfo()
{
}

bool ossimEnviInfo::open( const ossimFilename& file )
{
   bool result = false;

   m_file = file;

   if ( file.ext().downcase() != "hdr" )
   {
      // Typical case, we were fed the image file.  Look for a header file beside image.
      m_file.setExtension("hdr"); // image.hdr
      if ( !m_file.exists() )
      {
         m_file.setExtension("HDR"); // image.HDR
         if ( !m_file.exists() )
         {
            m_file = file;
            m_file.string() += ".hdr"; // image.ras.hdr
         }
      }
   }

   if ( m_file.exists() )
   {
      if ( ossimEnviHeader::isEnviHeader( m_file ) )
      {
         result = true;
      }
   }

   if ( !result ) 
   {
      m_file.clear();
   }

   return result;
}

std::ostream& ossimEnviInfo::print( std::ostream& out ) const
{
   if ( m_file.size() )
   {
      ossimEnviHeader hdr;
      if ( hdr.open( m_file ) )
      {
         hdr.getMap().addPrefixToAll( ossimString( "envi." ) );
         out << hdr.getMap() << std::endl;
      }
   }
   return out; 
}
