//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimTopographicCorrectionFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimTopographicCorrectionFilter_HEADER
#define ossimTopographicCorrectionFilter_HEADER
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossim2dLinearRegression.h>

class ossimImageData;

class ossimImageSourceConnection;
class ossimImageToPlaneNormalFilter;
/*!
 *
 * class ossimTopographicCorrectionFilter
 *
 * This clas is used to execute terrain correction.  I t requires 2 inputs
 * where the first input is a multi band data source to correct and the
 * second is elevation normals used in light incident calculations.
 *
 * A sample chain might look like:
 *
 *  landsatHandler ---> Cache ---> renderer ---> cache ---------------------------------------
 *                                                                                            |--> Topo correction filter ---> Output
 *  ortho mosaic of elevation data----> imageToPlaneNormals --> Cache ---> Renderer -->Cache --
 *
 * the ossimImageToPlaneNormalsFilter is used to take a single band elevation source and compute normals at each post.  I suggest that you
 * set the renderer to use a higher order convolution than just nearest neighbor, maybe Cubic.  This way you have smoother interpolation
 * as you change zoom levels.
 *
 *
 * Correction Types:
 *
 *   Cosine correction:     In order for this to run it does not need any additional informat but the multi band source and
 *                          an normal calculation input.
 *
 *   Cosine-C correction:   In order for this to run it will need theC array populated for each band.  It defaults
 *                          to 0 forcing a default Cosine correction to take place.
 *
 *   Minnaert correction:   In order for this to run it will need theK array populated for each band.  The defaults should work pretty good
 *  
 */
class OSSIMDLLEXPORT ossimTopographicCorrectionFilter : public ossimImageCombiner
{
public:
   enum ossimTopoCorrectionType
   {
      TOPO_CORRECTION_COSINE    = 0,
      TOPO_CORRECTION_COSINE_C  = 1,
      TOPO_CORRECTION_MINNAERT  = 2
   };
   ossimTopographicCorrectionFilter();

   ossimTopographicCorrectionFilter(ossimImageSource* colorSource,
                                    ossimImageSource* elevSource);
   
   ossimRefPtr<ossimImageData> getTile(const  ossimIrect& tileRect,
                                       ossim_uint32 resLevel=0);
   
   virtual ossim_uint32 getNumberOfOutputBands() const;

   virtual ossimScalarType getOutputScalarType() const;

   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   virtual double getMinPixelValue(ossim_uint32 band=0)const;

   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result) const;
   
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;

   virtual ossim_uint32 getNumberOfDecimationLevels()const;

   virtual double getAzimuthAngle()const;

   virtual double getElevationAngle()const;

   virtual void setAzimuthAngle(double angle);

   virtual void setElevationAngle(double angle);

   virtual void initialize();

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;
      
   virtual void connectInputEvent(ossimConnectionEvent& event);

   virtual void disconnectInputEvent(ossimConnectionEvent& event);

   virtual void propertyEvent(ossimPropertyEvent& event);

   virtual void refreshEvent(ossimRefreshEvent& event);

   virtual ossimTopoCorrectionType getTopoCorrectionType()const;

   virtual void setTopoCorrectionType(ossimTopoCorrectionType topoType);

   const std::vector<double>& getGainValues()const;

   void setGainValues(const std::vector<double>& gainValues);
  
   const vector<double>& getBiasValues()const;

   void setBiasValues(const std::vector<double>& biasValues);

   const vector<double>& getKValues()const;

   double getK(int idx)const;

   void setK(int idx, double value);

   void setKValues(const vector<double>& kValues);

   double getC(int idx)const;

   void setC(int idx, double value);

   void setCValues(const vector<double>& cValues);
   
   void computeLightDirection();

protected:
   virtual ~ossimTopographicCorrectionFilter();
   virtual void allocate();
   
   /*!
    * The result of the illumination equation is stored in
    * here.  This is populated on each call to getTile.
    */
   ossimRefPtr<ossimImageData>   theTile;

   std::vector<ossim_uint32> theBandMapping;
   
//   ossimImageToPlaneNormalFilter* theNormalFilter;
//   ossimScalarRemapper*           theScalarRemapper;
   std::vector<double> theC;
   std::vector<double> theK;
   std::vector<double> theBias;
   std::vector<double> theGain;
   
   /*!
    * Used for the light vector computation.
    */
   double theLightSourceElevationAngle;

   /*!
    * Used for the light vector computation.
    */
   double theLightSourceAzimuthAngle;

   /*!
    * This is computed from the elevation and
    * azimuth angles of the light source.
    */
   double theLightDirection[3];

   double         theJulianDay;
   mutable bool   theCComputedFlag;
   ossimTopoCorrectionType theTopoCorrectionType;   
   double         theNdviLowTest;
   double         theNdviHighTest;
   
   virtual void executeTopographicCorrection(
      ossimRefPtr<ossimImageData>& outputData,
      ossimRefPtr<ossimImageData>& colorData,
      ossimRefPtr<ossimImageData>& normalData);

   template <class T> void executeTopographicCorrectionTemplate(
      T dummy,
      ossimRefPtr<ossimImageData>& outputData,
      ossimRefPtr<ossimImageData>& colorData,
      ossimRefPtr<ossimImageData>& normalData);

   template <class T> void executeTopographicCorrectionMinnaertTemplate(
      T dummy,
      ossimRefPtr<ossimImageData>& outputData,
      ossimRefPtr<ossimImageData>& colorData,
      ossimRefPtr<ossimImageData>& normalData);
  
//    virtual void computeC();

//    template<class T>
//    void addRegressionPointsTemplate(T, //dummy
//                                     std::vector<ossim2dLinearRegression>& regressionPoints,
//                                     std::vector<int>& cosineIBucketCount,
//                                     ossim_int32 maxCountPerBucket,
//                                     ossimImageData* colorData,
//                                     ossimImageData* normalData);
   template <class T> bool computeNdvi(T dummy,
                                       ossim_float64& result,
                                       ossim_uint32 offset,
                                       const std::vector<T*>& bands)const;

   void resizeArrays(ossim_uint32 newSize);

   bool setCCmputedFlag();
TYPE_DATA
};

#endif
