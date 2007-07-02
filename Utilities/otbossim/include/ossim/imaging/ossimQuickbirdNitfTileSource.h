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
//  $Id: ossimQuickbirdNitfTileSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimQuickbirdNitfTileSource_HEADER
#define ossimQuickbirdNitfTileSource_HEADER
#include <ossim/imaging/ossimNitfTileSource.h>

class ossimQuickbirdNitfTileSource : public ossimNitfTileSource
{
public:
  virtual bool open();

 protected:

TYPE_DATA   
};
#endif
