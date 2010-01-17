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
// $Id: ossimQuickbirdTiffTileSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimQuickbirdTiffTileSource_HEADER
#define ossimQuickbirdTiffTileSource_HEADER

#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>

class OSSIM_DLL ossimQuickbirdTiffTileSource : public ossimTiffTileSource
{
public:
   virtual bool open();
   virtual ossimImageGeometry* getImageGeometry();
   
protected:
   ossimRefPtr<ossim2dTo2dTransform> m_transform;
   
TYPE_DATA   
};
#endif
