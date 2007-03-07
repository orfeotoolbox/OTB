//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: Point Hash is a base for hashing functions
//              
//*******************************************************************
//  $Id: ossimPointHash.h,v 1.3 2005/07/15 19:16:59 dburken Exp $

#ifndef ossimPointHash_HEADER
#define ossimPointHash_HEADER
#include "base/data_types/ossimDpt.h"
#include "base/data_types/ossimFpt.h"

class OSSIMDLLEXPORT ossimPointHash
{
public:
   virtual ~ossimPointHash(){}
   virtual long operator ()(const ossimDpt &aPoint)=0;
   virtual long operator ()(const ossimFpt &aPoint)=0;
};

#endif
