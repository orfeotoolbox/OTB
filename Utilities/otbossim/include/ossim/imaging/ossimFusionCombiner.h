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
// $Id: ossimFusionCombiner.h 9094 2006-06-13 19:12:40Z dburken $
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
  ossimImageSourceInterface*  theInputConnection;
  ossimImageSourceInterface*  theIntensityConnection;
  
  ossimRefPtr<ossimImageData> getNormIntensity(const ossimIrect& rect,
                                               ossim_uint32 resLevel);
  ossimRefPtr<ossimImageData> getNormTile(const ossimIrect& rect,
                                          ossim_uint32 resLevel);
TYPE_DATA  
};

#endif
