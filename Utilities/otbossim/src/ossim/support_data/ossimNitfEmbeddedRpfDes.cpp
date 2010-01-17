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
// $Id: ossimNitfEmbeddedRpfDes.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <istream>
#include <ostream>
#include <ossim/support_data/ossimNitfEmbeddedRpfDes.h>
#include <ossim/support_data/ossimNitfEmbeddedRpfHeader.h>


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

std::ostream& ossimNitfEmbeddedRpfDes::print(std::ostream& out,
                                             const std::string& prefix)const
{
   return out;
}
