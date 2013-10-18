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
// $Id: ossimNitfEmbeddedRpfDes.cpp 19682 2011-05-31 14:21:20Z dburken $

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

std::string ossimNitfEmbeddedRpfDes::getRegisterTagName()const
{
   return std::string("RPFDES");
}

void ossimNitfEmbeddedRpfDes::parseStream(std::istream& /* in */ )
{
}

std::ostream& ossimNitfEmbeddedRpfDes::print(std::ostream& out, const std::string& /* prefix */)const
{
   return out;
}
