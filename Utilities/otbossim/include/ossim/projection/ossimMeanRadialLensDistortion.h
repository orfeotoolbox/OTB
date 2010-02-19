//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// LGPL
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// MeanRadialLensDistortion
//*******************************************************************
//  $Id: ossimMeanRadialLensDistortion.h 15929 2009-11-16 18:20:46Z gpotts $
#ifndef ossimMeanRadialLensDistortion_HEADER
#define ossimMeanRadialLensDistortion_HEADER
#include <vector>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/projection/ossimLensDistortion.h>
class ossimMeanRadialLensDistortion : public ossimLensDistortion
{
public:
   ossimMeanRadialLensDistortion()
   {
      theCoefficients.resize(4);
      theCoefficients[0] = 0.0;
      theCoefficients[1] = 0.0;
      theCoefficients[2] = 0.0;
      theCoefficients[3] = 0.0;
   }

  /*!
   * Instantiate a ossimMeanRadialLens with the calibrated data.
   * The data consists of the Principal point,
   * focal distance, radialDistance measures for the distortion
   * deltas, the radial distortion values.
   * 
   * example of construction:
   *
   * lets say we have the following:
   *
   * principal point = (.008 , -.001) mm
   *
   * and
   *
   * field angle       radial distance       deltaR (distortion)
   *_____________________________________________________________
   *  7.5 deg.             .004 mm             .0202 mm
   *  15 deg.              .007 mm             .0411 mm
   *  22.5 deg.            .007 mm             .0635 mm
   *  :
   *  :
   *  :
   *
   * then construct the ossimMeanRadialLensDistortion class to construct the
   * given focal length and principal point and the middle column as
   * the radial distance column vector and the third column as the
   * distortion values.
   */
/*    ossimMeanRadialLensDistortion(ossimDpt    calibratedPrincipalPoint, */
/*                                  const NEWMAT::ColumnVector &radialDistance, */
/*                                  const NEWMAT::ColumnVector &radialDistortion) */
   ossimMeanRadialLensDistortion(ossimDpt    calibratedPrincipalPoint,
                                 const NEWMAT::ColumnVector &radialDistance,
                                 const NEWMAT::ColumnVector &radialDistortion)
      :ossimLensDistortion(calibratedPrincipalPoint)
  {
     theCoefficients.resize(4);
     solveCoefficients(radialDistance, radialDistortion);
  }
      
  /*!
   * Returns the coefficients (k1, k2, k3, k4) for the deltaR polynomial:
   *
   *    deltaR = k1*r + k2*r^3 + k3*r^5 + k4*r^7
   */
  const std::vector<double>& getCoefficients()const{return theCoefficients;}
  std::vector<double>& getCoefficients(){return theCoefficients;}


//  ossimDpt adjustPoint(const ossimDpt &position)const;
  virtual std::ostream& print(std::ostream& out) const
  {return out;}

  /*!
   * dletaR computes how much we should adjust r.  The
   * return value is the adjustment amount.  This method is used
   * by adjustPoint.
   */
  double deltaR(double r)const;

  virtual void undistort(const ossimDpt& input, ossimDpt& output)const;
  
  /*!
   * solves the coefficients given the radial distances and distortion
   * for each distance.
   */
  void solveCoefficients(const NEWMAT::ColumnVector &radialDistance,
			 const NEWMAT::ColumnVector &radialDistortion);
  /*!
   * solves the coefficients given the radial distances and distortion
   * for each distance.
   */
  void solveCoefficients(const std::vector<double>& radialDistance,
			 const std::vector<double>& radialDistortion);
  
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix = 0)const;
  
  virtual bool loadState(const ossimKeywordlist& kwl,
                         const char* prefix = 0);
  
protected:
  std::vector<double> theCoefficients;
  
TYPE_DATA  
};


#endif
