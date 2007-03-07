//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: LGPL see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfCompressionHeader.h,v 1.1 2004/11/09 19:11:41 gpotts Exp $
#ifndef ossimNitfCompressionHeader_HEADER
#define ossimNitfCompressionHeader_HEADER
#include <base/common/ossimObject.h>
#include <iostream>

class OSSIM_DLL ossimNitfCompressionHeader : public ossimObject
{
public:
   virtual void parseStream(std::istream& in) = 0;
protected:

TYPE_DATA;   
};
#endif
