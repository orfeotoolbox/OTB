//*****************************************************************************
// FILE: ossimRgbGridRemapEngine.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimRgbGridRemapEngine
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimRgbGridRemapEngine.h,v 1.5 2005/10/17 18:42:27 gpotts Exp $

#ifndef ossimRgbGridRemapEngine_HEADER
#define ossimRgbGridRemapEngine_HEADER

#include <imaging/tonal_balance/ossimGridRemapEngine.h>

/*!****************************************************************************
 *
 * CLASS: ossimRgbGridRemapEngine 
 *
 *****************************************************************************/
class ossimRgbGridRemapEngine : public ossimGridRemapEngine
{
public:
   ossimRgbGridRemapEngine()
      : ossimGridRemapEngine(3, 3) {}
   
   virtual ossimObject* dup() const;
   
   virtual void remapTile(const ossimDpt& origin_point,
                          ossimGridRemapSource* remapper,
                          ossimRefPtr<ossimImageData>& tile);

   virtual void assignRemapValues(std::vector<ossimAtbPointSource*>& sources);

   virtual void computeSourceValue(ossimRefPtr<ossimImageData>& source,
                                   void* result);

protected:
   virtual void computeRemapNode(ossimAtbPointSource*  point_source,
                                 void*                 source_value,
                                 void*                 target_value);

   TYPE_DATA
};

#endif
