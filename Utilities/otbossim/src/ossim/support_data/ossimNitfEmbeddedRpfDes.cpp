//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfEmbeddedRpfDes.cpp 9094 2006-06-13 19:12:40Z dburken $
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

std::ostream& ossimNitfEmbeddedRpfDes::print(std::ostream& out)const
{
   return out;
}
