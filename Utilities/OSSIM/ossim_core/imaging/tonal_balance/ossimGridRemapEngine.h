//*****************************************************************************
// FILE: ossimGridRemapEngine.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGridRemapEngine.h,v 1.5 2005/10/17 18:42:27 gpotts Exp $

#ifndef ossimGridRemapEngine_HEADER
#define ossimGridRemapEngine_HEADER

#include <vector>
#include <base/common/ossimObject.h>
#include <base/common/ossimRefPtr.h>

class ossimDpt;
class ossimGridRemapSource;
class ossimImageData;
class ossimAtbPointSource;

/*!****************************************************************************
 *
 * CLASS: ossimGridRemapEngine 
 *
 *****************************************************************************/
class ossimGridRemapEngine : public ossimObject
{
public:
   virtual ossimObject* dup() const = 0;
   
   virtual void remapTile(const ossimDpt&       origin_point,
                          ossimGridRemapSource* remapper,
                          ossimRefPtr<ossimImageData>& tile) = 0;

   virtual void assignRemapValues(std::vector<ossimAtbPointSource*>& sources)=0;

   virtual void computeSourceValue(ossimRefPtr<ossimImageData>& source,
                                   void* result) = 0;

   int  getNumberOfParams() const { return theNumberOfParams; }
   
   int  getNumberOfBands()  const { return theNumberOfBands; }
   
protected:
   ossimGridRemapEngine(int numParams, int numBands)
      : theNumberOfParams(numParams), theNumberOfBands(numBands) {}

   ossimGridRemapEngine() {}

   virtual void computeRemapNode(ossimAtbPointSource*  point_source,
                                 void*                 source_value,
                                 void*                 target_value) = 0;
   int  theNumberOfParams;
   int  theNumberOfBands;

   TYPE_DATA
};

#endif
