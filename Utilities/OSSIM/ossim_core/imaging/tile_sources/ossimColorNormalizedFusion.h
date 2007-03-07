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
// $Id: ossimColorNormalizedFusion.h,v 1.4 2005/02/11 15:07:32 dburken Exp $
#ifndef ossimColorNormalizedFusion_HEADER
#define ossimColorNormalizedFusion_HEADER
#include "ossimFusionCombiner.h"

class ossimColorNormalizedFusion : public ossimFusionCombiner
{
public:

   ossimColorNormalizedFusion();
   ossimColorNormalizedFusion(ossimObject* owner);
   virtual ~ossimColorNormalizedFusion();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

protected:
TYPE_DATA
};

#endif
