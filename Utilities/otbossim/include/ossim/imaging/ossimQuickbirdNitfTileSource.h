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
//  $Id: ossimQuickbirdNitfTileSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimQuickbirdNitfTileSource_HEADER
#define ossimQuickbirdNitfTileSource_HEADER
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/base/ossim2dTo2dTransform.h>

class ossimQuickbirdNitfTileSource : public ossimNitfTileSource
{
public:
   virtual bool open();

   virtual ossimImageGeometry* getImageGeometry();
   
protected:
   ossimRefPtr<ossim2dTo2dTransform> m_transform;
TYPE_DATA   
};
#endif
