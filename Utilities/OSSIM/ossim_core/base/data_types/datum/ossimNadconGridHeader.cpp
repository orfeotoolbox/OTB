//**************************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// Written by:   Kenneth Melero  <kmelero@sanz.com>
//
//**************************************************************************

#include <fstream>
#include "ossimNadconGridHeader.h"
#include "base/misc/ossimEndian.h"

std::ostream& operator <<(std::ostream& out, const ossimNadconGridHeader& rhs)
{
   out << "theCols:    " << rhs.theCols << endl
       << "theRows:    " << rhs.theRows << endl
       << "theZ:       " << rhs.theZ << endl
       << "theMinX:    " << rhs.theMinX <<endl
       << "theDx:      " << rhs.theDx << endl
       << "theMinY:    " << rhs.theMinY << endl
       << "theDy:      " << rhs.theDy << endl
       << "Bounds:     " << rhs.getBoundingRect() << endl;
   return out;
}

bool ossimNadconGridHeader::readHeader(const ossimFilename& file)
{
   ifstream in(file.c_str(), ios::in|ios::binary);

   if(in)
   {
      return readHeader(in);
   }

   return false;
}

bool ossimNadconGridHeader::readHeader(std::istream& in)
{
   in.read((char*)theCharBuf, 64);
   in.read((char*)&theCols, 4);
   in.read((char*)&theRows, 4);
   in.read((char*)&theZ,   4);
   in.read((char*)&theMinX, 4);
   in.read((char*)&theDx, 4);
   in.read((char*)&theMinY, 4);
   in.read((char*)&theDy, 4);
   ossimEndian anEndian;
   if(anEndian.getSystemEndianType() != OSSIM_LITTLE_ENDIAN)
     {
       anEndian.swap(theCols);
       anEndian.swap(theRows);
       anEndian.swap(theZ);
       anEndian.swap(theMinX);
       anEndian.swap(theDx);
       anEndian.swap(theMinY);
       anEndian.swap(theDy);
     }

   return in.good();
}
