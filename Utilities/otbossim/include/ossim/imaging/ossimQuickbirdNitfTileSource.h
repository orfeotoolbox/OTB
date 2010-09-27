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
//  $Id: ossimQuickbirdNitfTileSource.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimQuickbirdNitfTileSource_HEADER
#define ossimQuickbirdNitfTileSource_HEADER
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/base/ossim2dTo2dTransform.h>

class ossimQuickbirdNitfTileSource : public ossimNitfTileSource
{
public:
   virtual bool open();

   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
   
protected:
   ossimRefPtr<ossim2dTo2dTransform> m_transform;
TYPE_DATA   
};
#endif
