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
// $Id: ossimQuickbirdTiffTileSource.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimQuickbirdTiffTileSource_HEADER
#define ossimQuickbirdTiffTileSource_HEADER

#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>

class OSSIM_DLL ossimQuickbirdTiffTileSource : public ossimTiffTileSource
{
public:
   virtual bool open();
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
   
protected:
   ossimFilename m_tileInfoFilename;
   
TYPE_DATA   
};
#endif
