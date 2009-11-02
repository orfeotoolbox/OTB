//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Color normalized fusion
//
//*************************************************************************
// $Id: ossimColorNormalizedFusion.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimColorNormalizedFusion_HEADER
#define ossimColorNormalizedFusion_HEADER
#include <ossim/imaging/ossimFusionCombiner.h>

class ossimColorNormalizedFusion : public ossimFusionCombiner
{
public:

   ossimColorNormalizedFusion();
   ossimColorNormalizedFusion(ossimObject* owner);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

protected:
   virtual ~ossimColorNormalizedFusion();
TYPE_DATA
};

#endif
