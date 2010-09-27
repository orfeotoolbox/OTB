//*******************************************************************
// Copyright (C) 2010 Radiant Blue Tecnologies Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: William Watkins
// 
//*************************************************************************
#ifndef ossimSICDToDetectedImage_HEADER
#define ossimSICDToDetectedImage_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIM_DLL ossimSICDToDetectedImage : public ossimImageSourceFilter
{
public:

   /**
    * Currently works with SICD data but will generally work for conforming Complex formats    */

	ossimSICDToDetectedImage();


   /**
    * Applies the sqrt(I^2 + Q^2) to generate the Detected SAR image.
	  For 8 bit data the input is assumed to be Magnitude and Phase (Not complex format) and 
	  so it simply returns the magnitude which is the Detected SAR image.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   /**
    * Calls base ossimImageSourceFilter::initialize() to reset connection if
    * needed then nulls out tiles.
    *
    * @note This does not initialize tiles intentionally to avoid repeated
    * deletes and news.  Tiles will be initialized by allocate() method on
    * the first getTile call after an initialize.
    */
   virtual void initialize();
//	virtual ossim_uint32 getNumberOfOutputBands() const
//	{
//		return 1;
//	}
   
   /*---------------------- PROPERTY INTERFACE ---------------------------*/
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /*--------------------- PROPERTY INTERFACE END ------------------------*/

protected:
   virtual ~ossimSICDToDetectedImage();

   /**
    * Called to initialize tiles by first get tile when theTile or theNormTile
    * are not initialized.
    */
   void allocate();

   /**
    * Applies the brightness contrast to tile.  This method simply applies
    * brightness contrast to all bands.  Called if input does not have
    * three bands.
    */
   template <class T> void processComplexTile(T dummy, ossimImageData* tile);
   template<class T> void processAmplitudeAngleTile(T dummy, ossimImageData* tile);
	
   ossimRefPtr<ossimImageData> theTile;
   
TYPE_DATA
};

#endif /* End of "#ifndef ossimSICDToDetectedImage_HEADER" */
