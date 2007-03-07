//*****************************************************************************
// FILE: ossimRpcProjection.h
//
// Copyright (C) 2004 Intelligence Data Syatems
//
// LGPL
//
// DESCRIPTION: Contains declaration of class ossimRpcProjection.
//   This is a replacement model utilizing the Rational Polynomial Coefficients
//   (RPC), a.k.a. Rapid Positioning Capability, and Universal Sensor Model
//   (USM).
//
//*****************************************************************************
//  $Id: ossimRpcProjection.h,v 1.2 2005/09/09 19:23:58 gpotts Exp $

#ifndef ossimRpcProjection_HEADER
#define ossimRpcProjection_HEADER

#include <projections/ossimProjection.h>
#include <base/data_types/ossimIpt.h>

/*!****************************************************************************
 *
 * CLASS:  ossimRpcProjection
 *
 *****************************************************************************/
class OSSIM_DLL ossimRpcProjection : public ossimProjection
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
   ossimRpcProjection();
   ossimRpcProjection(const ossimRpcProjection& copy_this);
   ~ossimRpcProjection();

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
                      PolynomialType polyType = B);
                      
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
    * METHOD: dup()
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return new ossimRpcProjection(*this); }

   virtual ossimGpt origin()const;
   virtual ossimDpt getMetersPerPixel() const;
   virtual bool operator==(const ossimProjection& projection) const;
   
protected:
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
   
   TYPE_DATA
};

#endif
