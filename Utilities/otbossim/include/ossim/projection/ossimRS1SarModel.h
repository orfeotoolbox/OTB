//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Oscar Kramer
//
// Description:
//
// Sensor Model for Radarsat1 SAR sensor.
// 
//*******************************************************************
//  $Id:$

#ifndef ossimRS1SarModel_HEADER
#define ossimRS1SarModel_HEADER 1

#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/projection/ossimSarModel.h>
#include <ossim/base/ossimLagrangeInterpolator.h>
#include <ossim/support_data/ossimCeosData.h>

// Namespace class forward class declaration.
namespace NEWMAT
{
   class Matrix;
}

//*****************************************************************************
// CLASS:  ossimRS1SarModel
//*****************************************************************************
class ossimRS1SarModel : public ossimSensorModel
{
public:
   ossimRS1SarModel();
   ossimRS1SarModel(const ossimFilename& imageDir);

   virtual ~ossimRS1SarModel();
   
   enum ImagingMode
   {
      UNKNOWN_MODE = 0,
      SCN,     // ScanSAR Narrow Beam
      SCW,     // ScanSAR Wide Beam
      SGC,     // SAR Georeferenced Coarse Resolution
      SGF,     // SAR Georeferenced Fine Resolution
      SGX,     // SAR Georeferenced Extra Fine Resolution
      SLC,     // Single Look Complex
      SPG,     // SAR Precision Geocoded
      SSG,     // SAR Systematically Geocoded
      RAW,
      ERS
   };

   enum DirectionFlag
   {
      UNKNOWN_DIRECTION = 0,
      ASCENDING,
      DESCENDING
   };
   
   //! Fulfills ossimObject base-class pure virtuals. Saves modeling info to KWL.
   //! Returns true if successful.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=NULL) const;
   
   //! Fulfills ossimObject base-class pure virtuals. Reads modeling info from KWL.
   //! Returns true if successful.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=NULL);

   //! Establishes geographic 3D point given image line, sample and ellipsoid height.
   virtual void lineSampleHeightToWorld(const ossimDpt& imagePt, 
                                        const double& heightAboveEllipsoid, 
                                        ossimGpt& worldPt) const;

   //! Given an image point, returns a ray originating at some arbitrarily high
   //! point (in this model at the sensor position) and pointing towards the target.
   virtual void imagingRay(const ossimDpt& image_point, ossimEcefRay& image_ray) const;

   inline virtual bool useForward() const { return false; } //!image to ground faster 

   //!  Returns pointer to a new instance, copy of this.
   virtual ossimObject* dup() const { return 0; } // TBR

protected:
   void setImagingMode(char* modeStr);
   void initFromCeos(const ossimFilename& dataDir);
   void initAdjParms();
   void establishEphemeris();
   void eciToEcfXform(const double& julianDay, NEWMAT::Matrix& xform) const;
   void establishOrpInterp();
   void establishOrpGrid();
   void establishVehicleSpace();
   void interpolatedScanORP(const ossimDpt& orp, ossimEcefPoint& orp_ecf) const;
   void deallocateMemory();
   
   ossimRefPtr<ossimCeosData> theCeosData;
   ossimRefPtr<ossimLagrangeInterpolator> theArpPosInterp;   // in ECF
   ossimRefPtr<ossimLagrangeInterpolator> theArpVelInterp;   // in ECF
   ossimRefPtr<ossimLagrangeInterpolator> theLocalOrpInterp; // in ECF
   ImagingMode     theImagingMode;
   double          theIllumAzimuth;
   double          theIllumElevation;
   ossimEcefPoint  theORP;
   double          theRefHeight;
   double          theGHA;           // Greenwich Hour Angle of first eph. pt.
   double          theEphFirstSampTime; // in seconds from start of day
   double          theSinOrientation;
   double          theCosOrientation;
   double          theSinSkew;
   ossimLsrSpace   theVehicleSpace;
   ossimEcefVector thePosCorrection;
   int             theFirstLineDay;   // julian day
   double          theFirstLineTime;  // seconds
   double          theTimePerLine;    // seconds
   DirectionFlag   theDirectionFlag;
   double          theSrGrCoeff[6];
   ossimDpt        thePixelSpacing; 
   //***
   // Additional data members used for scan-mode imagery:
   //***
   ossimDblGrid    theLatGrid;
   ossimDblGrid    theLonGrid;
   
   //***
   // Adjustable Parameters:
   //***
   double          theInTrackOffset; // meters
   double          theCrTrackOffset; // meters
   double          theRadialOffset;  // meters
   double          theLineScale;
   double          theSkew;
   double          theOrientation; // degrees
   
   //***
   // Adjustable model parameters array indexes:
   //***
   enum ADJUSTABLE_PARAM_INDEXES
   {
      INTRACK_OFFSET,
      CRTRACK_OFFSET,
      RADIAL_OFFSET,
      LINE_SCALE,
      SKEW,
      ORIENTATION,
      NUM_ADJUSTABLE_PARAMS  // not a parameter
   };

};


#endif
