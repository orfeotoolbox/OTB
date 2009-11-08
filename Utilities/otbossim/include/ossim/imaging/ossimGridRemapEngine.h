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
//  $Id: ossimGridRemapEngine.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimGridRemapEngine_HEADER
#define ossimGridRemapEngine_HEADER

#include <vector>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>

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
   virtual ~ossimGridRemapEngine() {}

   virtual void computeRemapNode(ossimAtbPointSource*  point_source,
                                 void*                 source_value,
                                 void*                 target_value) = 0;
   int  theNumberOfParams;
   int  theNumberOfBands;

   TYPE_DATA
};

#endif
