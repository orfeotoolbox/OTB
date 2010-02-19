//*****************************************************************************
// FILE: ossimRpcModel.h
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimRpcModel.
//   This is a replacement model utilizing the Rational Polynomial Coefficients
//   (RPC), a.k.a. Rapid Positioning Capability, and Universal Sensor Model
//   (USM).
//
//*****************************************************************************
//  $Id: ossimRpcModel.h 16174 2009-12-23 16:34:06Z dburken $

#ifndef ossimRpcModel_HEADER
#define ossimRpcModel_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDblGrid.h>
#include <ossim/base/ossimFilename.h>


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

   /** @brief RPC model structure used for access function */
   struct rpcModelStruct
   {
      double lineScale;
      double sampScale;
      double latScale;
      double lonScale;
      double hgtScale;
      double lineOffset;
      double sampOffset;
      double latOffset;
      double lonOffset;
      double hgtOffset;
      double lineNumCoef[20];
      double lineDenCoef[20];
      double sampNumCoef[20];
      double sampDenCoef[20];
      char   type;
   };

   /** @brief default constructor */
   ossimRpcModel();

   /** @brief copy construtor */
   ossimRpcModel(const ossimRpcModel& copy_this);

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
                      bool computeGsdFlag=true);

   void setMetersPerPixel(const ossimDpt& metersPerPixel);

   /**
    * @brief Sets data member theBiasError, theRandError.
    *
    * @param biasError Error - Bias 68% non time - varying error estimate
    * assumes correlated images.  Units = meters.
    *
    * @param randError Error - Random 68% time - varying error estimate
    * assumes uncorrelated images. Units = meters.
    *
    * @param initNominalPostionErrorFlag If true the base data member
    * theNominalPosError will be initialized with:
    * sqrt(theBiasError*theBiasError +theRandError*theRandError)
    */
   void setPositionError(const ossim_float64& biasError,
                         const ossim_float64& randomError,
                         bool initNominalPostionErrorFlag);

   /**
    * @brief worldToLineSample()
    * Overrides base class implementation. Directly computes line-sample from
    * the polynomials.
    */
   virtual void  worldToLineSample(const ossimGpt& world_point,
                                   ossimDpt&       image_point) const;
   /**
    * @brief print()
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

   /**
    * @brief saveState
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   /**
    * @brief loadState
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   //***
   // @brief lineSampleToWorld()
   // Overrides base class pure virtual.  Intersects DEM.
   //***
   virtual void  lineSampleToWorld(const ossimDpt& image_point,
                                   ossimGpt&       world_point) const;
   //***
   // @brief lineSampleHeightToWorld()
   // Overrides base class pure virtual. Height understood to be relative to
   // standard ellipsoid.
   //***
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   
   /**
    * @brief imagingRay()
    * Overrides base class pure virtual.
    */
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   /**
    * @brief STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);

   virtual void updateModel();
   virtual void initAdjustableParameters();

   /**
    * @brief dup()
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const;

   inline virtual bool useForward()const {return false;}

   /** @brief uses file path to init model */
   virtual bool setupOptimizer(const ossimString& init_file);

   /**
    * @brief Compute partials of samp/line WRT ground point
    *
    * @param parmIdx computational mode:
    *        OBS_INIT, EVALUATE, P_WRT_X, P_WRT_X, P_WRT_X.
    *
    * @param gpos Current ground point estimate.
    *
    * @param h Not used.
    *
    * @return OBS_INT: n/a, EVALUATE: residuals, P_WRT_X/Y/Z: partials.
    */
   virtual ossimDpt getForwardDeriv(int parmIdx,
                                    const ossimGpt& gpos,
                                    double h);

   /**
    * @brief Returns Error - Bias.
    * @return Error - Bias
    * @note See NITF field "ERR_BIAS" from RPC00x tag where x = A or B.
    */
   double getBiasError() const;

   /**
    * @brief Returns Error - Random.
    * @return Error - Random
    * @note See NITF field "ERR_RAND" from RPC00x tag where x = A or B.
    */
   double getRandError() const;

   /**
    * @brief Returns RPC parameter set in structure.
    * @param rpcModelStruct structure to initialize.
    */
   void getRpcParameters(ossimRpcModel::rpcModelStruct& model) const;
   

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

   /** @brief virtual destructor */
   virtual ~ossimRpcModel();
   
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
   double dPoly_dHgt(const double& nlat,
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
   // Quantities derived from the adjustable parameters:
   //***
   double theIntrackOffset;
   double theCrtrackOffset;
   double theIntrackScale;
   double theCrtrackScale;
   double theCosMapRot;
   double theSinMapRot;

   /** error */
   double theBiasError; // meters
   double theRandError; // meters

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
