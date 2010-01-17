//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: USGS DEM Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <iostream>

#include <ossim/support_data/ossimDemInfo.h>

#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimFilename.h>

#include <ossim/support_data/ossimDemHeader.h>
#include <ossim/support_data/ossimDemUtil.h>

ossimDemInfo::ossimDemInfo()
   : theFile()
{
}

ossimDemInfo::~ossimDemInfo()
{
}

bool ossimDemInfo::open(const ossimFilename& file)
{
   bool result = ossimDemUtil::isUsgsDem(file);

   if ( result )
   {
      theFile = file;
   }
   else
   {
      theFile = ossimFilename::NIL;
   }
   
   return result;
}

std::ostream& ossimDemInfo::print(std::ostream& out) const
{
   if ( theFile.exists() )
   {
      ossimDemHeader hdr;
      if ( hdr.open(theFile) )
      {
         // std::string prefix;
         hdr.print(std::cout);
      }
   }
   return out;
}
