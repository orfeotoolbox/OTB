//*****************************************************************************
// FILE: ossimRpcModel.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimRpcModel.
//   This is a replacement model utilizing the Rational Polynomial Coefficients
//   (RPC), a.k.a. Rapid Positioning Capability, and Universal Sensor Model
//   (USM).
//
//*****************************************************************************
//  $Id: ossimRpcModel.h,v 1.12 2005/05/12 19:55:13 gpotts Exp $

#ifndef ossimRpcModel_HEADER
#define ossimRpcModel_HEADER

#include <projections/sensor_modeling/ossimSensorModel.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimDblGrid.h>
#include <base/data_types/ossimFilename.h>

/*!****************************************************************************
 *
 * CLASS:  ossimRpcModel
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimRpcModel : public ossimSensorModel
{
public:
   //***
   // Enumeration of supported RPC polynomial formats:
   //***
   enum PolynomialType
   {
      A='A',  // corresponds to "RPC00A"
      B='B'   // corresponds to "RPC00B"
   };
   
   /*!
    * CONSTRUCTORS:
    */
   ossimRpcModel();
   ossimRpcModel(const ossimRpcModel& copy_this);
   ossimRpcModel(const ossimKeywordlist& geom_kwl);
   ~ossimRpcModel();

   void setAttributes(ossim_float64 theSampleOffset,
                      ossim_float64 theLineOffset,
                      ossim_float64 theSampleScale,
                      ossim_float64 theLineScale,
                      ossim_float64 theLatOffset,
                      ossim_float64 theLonOffset,
                      ossim_float64 theHeightOffset,
                      ossim_float64 theLatScale,
                      ossim_float64 theLonScale,
                      ossim_float64 theHeightScale,
                      const std::vector<double>& xNumeratorCoeffs,
                      const std::vector<double>& xDenominatorCoeffs,
                      const std::vector<double>& yNumeratorCoeffs,
                      const std::vector<double>& yDenominatorCoeffs,
                      PolynomialType polyType = B,
                      bool computeGsd=true);

   void setMetersPerPixel(const ossimDpt& metersPerPixel);
   /*!
    * METHOD: worldToLineSample()
    * Overrides base class implementation. Directly computes line-sample from
    * the polynomials.
    */
   virtual void  worldToLineSample(const ossimGpt& world_point,
                                   ossimDpt&       image_point) const;
   /*!
    * METHOD: print()
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

   /*!
    * METHODS:  saveState, loadState
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual void  lineSampleToWorld(const ossimDpt& image_point,
                                   ossimGpt&       world_point) const;
   //***
   // METHOD: lineSampleHeightToWorld()
   // Overrides base class pure virtual. Height understood to be relative to
   // standard ellipsoid.
   //***
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;

   /*!
    * STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);

   virtual void updateModel();
   virtual void initAdjustableParameters();

   /*!
    * METHOD: dup()
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return new ossimRpcModel(*this); }
   
  static const ossimFilename INIT_RPC_GEOM_FILENAME;

protected:
   enum AdjustParamIndex
   {
      INTRACK_OFFSET = 0,
      CRTRACK_OFFSET,
      INTRACK_SCALE,
      CRTRACK_SCALE,
      MAP_ROTATION,
//      YAW_OFFSET,
      NUM_ADJUSTABLE_PARAMS // not an index
   };

   //***
   // Methods for computing RPC polynomial and its derivatives:
   //***
   double polynomial(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
   double dPoly_dLat(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
   double dPoly_dLon(const double& nlat,
                     const double& nlon,
                     const double& nhgt,
                     const double* coeffs) const;
   
   virtual ossimDpt extrapolate (const ossimGpt& gp) const;
   virtual ossimGpt extrapolate (const ossimDpt& ip,
				 const double& height
				 =ossimElevSource::DEFAULT_NULL_HEIGHT) const;
   PolynomialType thePolyType;
   //***
   // Quantities for zero-biasing and normalizing the image point and
   // ground point coordinates referenced in the polynomials:
   //***
   double theLineScale;
   double theSampScale;
   double theLatScale;
   double theLonScale;
   double theHgtScale;
   double theLineOffset;
   double theSampOffset;
   double theLatOffset;
   double theLonOffset;
   double theHgtOffset;

   //***
   // Coefficients:
   //***
   double theLineNumCoef[20];
   double theLineDenCoef[20];
   double theSampNumCoef[20];
   double theSampDenCoef[20];
   
   //***
   // Quantities derived from the adjustable parameters:
   //***
   double theIntrackOffset;
   double theCrtrackOffset;
   double theIntrackScale;
   double theCrtrackScale;
   double theYawSkew;  // = sin(theYawOffset)
   double theCosMapRot;
   double theSinMapRot;
   TYPE_DATA
};

#endif
