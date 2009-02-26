//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Implementation for Base fusion.
//              This base class assumes two inputs where the second input is
//              the intensity source.
//
//*************************************************************************
// $Id: ossimFusionCombiner.h 13312 2008-07-27 01:26:52Z gpotts $
#ifndef ossimFusionCombiner_HEADER
#define ossimFusionCombiner_HEADER

#include <ossim/imaging/ossimImageCombiner.h>

class OSSIMDLLEXPORT ossimFusionCombiner : public ossimImageCombiner
{
public:
  ossimFusionCombiner();
  ossimFusionCombiner(ossimObject* owner);
  virtual ~ossimFusionCombiner();   
  virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;
  bool canConnectMyInputTo(ossim_int32 inputIndex,
			   const ossimConnectableObject* object)const;
   ossimScalarType getOutputScalarType() const;
   
  virtual void initialize();
protected:
  ossimRefPtr<ossimImageData> theTile;
  ossimRefPtr<ossimImageData> theNormTile;
  ossimRefPtr<ossimImageData> theNormIntensity;
  ossimImageSource*  theInputConnection;
  ossimImageSource*  theIntensityConnection;
  
  ossimRefPtr<ossimImageData> getNormIntensity(const ossimIrect& rect,
                                               ossim_uint32 resLevel);
  ossimRefPtr<ossimImageData> getNormTile(const ossimIrect& rect,
                                          ossim_uint32 resLevel);
TYPE_DATA  
};

#endif
