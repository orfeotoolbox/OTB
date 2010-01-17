//*****************************************************************************
// FILE: ossimRpcProjection.h
//
// LGPL
//
// DESCRIPTION: Contains declaration of class ossimRpcProjection.
//   This is a replacement model utilizing the Rational Polynomial Coefficients
//   (RPC), a.k.a. Rapid Positioning Capability, and Universal Sensor Model
//   (USM).
//
//*****************************************************************************
//  $Id: ossimRpcProjection.h 11805 2007-10-05 14:54:28Z dburken $

#ifndef ossimRpcProjection_HEADER
#define ossimRpcProjection_HEADER

#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/matrix/newmat.h>

/*!****************************************************************************
 *
 * CLASS:  ossimRpcProjection
 *
 *****************************************************************************/
class OSSIM_DLL ossimRpcProjection : public ossimProjection,
                                     public ossimOptimizableProjection,
                                     public ossimAdjustableParameterInterface
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

   ossimRpcProjection& operator=(const ossimRpcProjection& source);

   void initAdjustableParameters();
   virtual void adjustableParametersChanged();
   
   virtual ossimObject* getBaseObject();
   virtual const ossimObject* getBaseObject()const;
   
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
   
   /*
    * optimizable interface
    */
    virtual bool setupOptimizer(const ossimString& setup);
    inline virtual bool useForward()const {return true;}
    virtual ossim_uint32 degreesOfFreedom()const;
    virtual double optimizeFit(const ossimTieGptSet& tieSet, double* targetVariance=NULL);
   /*!
    * METHOD: getForwardDeriv()
    * gives forward() partial derivative regarding parameter parmIdx (>=0)
    * default implementation is centered finite difference
    * -should be reimplemented with formal derivative in child class
    */
   virtual ossimDpt getForwardDeriv(int parmIdx, const ossimGpt& gpos, double hdelta=1e-11);

   /*!
    * METHOD: getInverseDeriv()
    * gives inverse() partial derivative regarding parameter parmIdx (>=0)
    * default implementation is centered finite difference
    * -should be reimplemented with formal derivative in child class
    */
   virtual ossimGpt getInverseDeriv(int parmIdx, const ossimDpt& ipos, double hdelta=1e-11);

   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return true.
    */
   virtual bool isAffectedByElevation() const { return true; }

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
   
   /*!
    * METHOD: buildNormalEquation
    * builds linearized system  (LMS equivalent)
    * A*dp = projResidue
    * 
    * A: symetric matrix = tJ*J
    * dp: system parameter shift that we want to estimate
    * projResidue = tJ * residue
    *
    * t: transposition operator
    * J = jacobian of transform relative to parameters p, transform can be forward() or inverse()
    * jacobian is obtained via finite differences
    * residue can be image (2D) or ground residue(3D)
    *
    * TODO: use image/ground points covariance matrices
    */
   void buildNormalEquation(const ossimTieGptSet& tieSet,
                                      NEWMAT::SymmetricMatrix& A,
                                      NEWMAT::ColumnVector& residue,
                                      NEWMAT::ColumnVector& projResidue,
                                      double pstep_scale);

   /*!
    * METHOD: getResidue()
    * returns ground opr image residue
    */
   NEWMAT::ColumnVector getResidue(const ossimTieGptSet& tieSet);

   NEWMAT::ColumnVector solveLeastSquares(NEWMAT::SymmetricMatrix& A,  NEWMAT::ColumnVector& r)const;

   /*!
    * stable invert stolen from ossimRpcSolver
    */
   NEWMAT::Matrix invert(const NEWMAT::Matrix& m)const;

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

   //*
   // Quantities derived from the adjustable parameters:
   //*
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
