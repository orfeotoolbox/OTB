//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: NITF Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <iostream>

#include <ossim/support_data/ossimNitfInfo.h>
#include <ossim/support_data/ossimNitfFile.h>

ossimNitfInfo::ossimNitfInfo()
   : theNitfFile(0)
{
}

ossimNitfInfo::~ossimNitfInfo()
{
   if (theNitfFile)
   {
      delete theNitfFile;
      theNitfFile = 0;
   }
}

bool ossimNitfInfo::open(const ossimFilename& file)
{
   if (theNitfFile)
   {
      delete theNitfFile;
   }

   theNitfFile = new ossimNitfFile();

   bool result = theNitfFile->parseFile(file);

   if (result == false)
   {
      delete theNitfFile;
      theNitfFile = 0;
   }

   return result;
}

std::ostream& ossimNitfInfo::print(std::ostream& out) const
{
   if (theNitfFile)
   {
      std::string prefix;
      theNitfFile->print(out, prefix);
   }
   return out;
}
