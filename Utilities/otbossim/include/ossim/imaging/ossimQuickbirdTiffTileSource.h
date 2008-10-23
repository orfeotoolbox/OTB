//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
// 
// Copied from ossimQuickbirdNitfTileSource written by Garrett Potts.
//
// Description:
//
// Class declaration for specialized image handler to pick up offsets from
// Quick Bird ".TIL" files.
// 
//----------------------------------------------------------------------------
// $Id: ossimQuickbirdTiffTileSource.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimQuickbirdTiffTileSource_HEADER
#define ossimQuickbirdTiffTileSource_HEADER

#include <ossim/imaging/ossimTiffTileSource.h>

class ossimQuickbirdTiffTileSource : public ossimTiffTileSource
{
public:
  virtual bool open();

 protected:

TYPE_DATA   
};
#endif
