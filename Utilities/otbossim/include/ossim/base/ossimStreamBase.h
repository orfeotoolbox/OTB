//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//
//*******************************************************************
//  $Id: ossimStreamBase.h 9094 2006-06-13 19:12:40Z dburken $
//
#ifndef ossimStreamBase_HEADER
#define ossimStreamBase_HEADER
#include <iostream>
#include <ossim/base/ossimObject.h>

class OSSIM_DLL ossimStreamBase : public ossimObject, virtual public std::ios
{
public:
   ossimStreamBase(){}
   ossimStreamBase(std::streambuf* buf)
   {
      init(buf);
   }
TYPE_DATA   
};
#endif
