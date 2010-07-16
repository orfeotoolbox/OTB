//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfEmbeddedRpfDes.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <istream>
#include <ostream>
#include <ossim/support_data/ossimNitfEmbeddedRpfDes.h>


RTTI_DEF1(ossimNitfEmbeddedRpfDes, "ossimNitfEmbeddedRpfDes", ossimNitfRegisteredTag)


ossimNitfEmbeddedRpfDes::ossimNitfEmbeddedRpfDes()
{
}

ossimNitfEmbeddedRpfDes::~ossimNitfEmbeddedRpfDes()
{
}

void ossimNitfEmbeddedRpfDes::parseStream(std::istream& /* in */ )
{
}

std::ostream& ossimNitfEmbeddedRpfDes::print(std::ostream& out, const std::string& /* prefix */)const
{
   return out;
}
