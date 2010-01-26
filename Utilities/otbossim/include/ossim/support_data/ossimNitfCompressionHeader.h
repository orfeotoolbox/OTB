//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: LGPL see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfCompressionHeader.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfCompressionHeader_HEADER
#define ossimNitfCompressionHeader_HEADER
#include <ossim/base/ossimObject.h>

#include <iosfwd>

class OSSIM_DLL ossimNitfCompressionHeader : public ossimObject
{
public:
   virtual void parseStream(std::istream& in) = 0;
protected:

TYPE_DATA;   
};
#endif
