//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimQuickbirdNitfTileSource.h,v 1.7 2004/10/28 18:27:18 dburken Exp $
#ifndef ossimQuickbirdNitfTileSource_HEADER
#define ossimQuickbirdNitfTileSource_HEADER
#include "ossimNitfTileSource.h"

class ossimQuickbirdNitfTileSource : public ossimNitfTileSource
{
public:
  virtual bool open();

 protected:

TYPE_DATA   
};
#endif
