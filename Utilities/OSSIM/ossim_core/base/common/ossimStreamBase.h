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
//  $Id: ossimStreamBase.h,v 1.3 2005/11/14 13:58:51 gpotts Exp $
//
#ifndef ossimStreamBase_HEADER
#define ossimStreamBase_HEADER
#include <iostream>
#include "ossimObject.h"

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
