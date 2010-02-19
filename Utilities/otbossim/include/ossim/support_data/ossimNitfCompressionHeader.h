//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: LGPL see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfCompressionHeader.h 16035 2009-12-03 21:32:27Z dburken $
#ifndef ossimNitfCompressionHeader_HEADER
#define ossimNitfCompressionHeader_HEADER
#include <ossim/base/ossimObject.h>

#include <iosfwd>
#include <string>

class OSSIM_DLL ossimNitfCompressionHeader : public ossimObject
{
public:
   virtual void parseStream(std::istream& in) = 0;

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix) const=0;
   
protected:

TYPE_DATA;   
};
#endif
