//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: CCF Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <iostream>
#include <ossim/base/ossimFilename.h>
#include <ossim/support_data/ossimCcfInfo.h>
#include <ossim/imaging/ossimCcfHead.h>

ossimCcfInfo::ossimCcfInfo()
   : theFile()
{
}

ossimCcfInfo::~ossimCcfInfo()
{
}

bool ossimCcfInfo::open(const ossimFilename& file)
{
   bool result = false;

   ossimString extension = file.ext();

   extension.downcase();

   if (extension == "ccf")
   {
      theFile = file;
      result = true;
   }
   else
   {
      theFile.clear();
   }

   return result;
}

std::ostream& ossimCcfInfo::print(std::ostream& out) const
{
   
   if ( theFile.size() )
   {
      ossimCcfHead ccf(theFile);
      out << "File:  " << theFile << "\n" << ccf;
   }
   return out;
}
