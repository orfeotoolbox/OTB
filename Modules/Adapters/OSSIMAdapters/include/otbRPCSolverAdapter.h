/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbRPCSolverAdapter_h
#define __otbRPCSolverAdapter_h

#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "itkPoint.h"
#include "itkSize.h"


class ossimRpcSolver;

namespace otb {

class ImageKeywordlist;

/** \class RPCSolverAdapter
 *  \brief This is a dummy class to expose the RPC solving function
 *
 *  This class defines some typedefs and exposes the RPC solving functions.
 *
 * \sa GCPsToRPCSensorModelImageFilter
 **/

class RPCSolverAdapter
{
public:
  /** Standard class typedefs. */
  typedef RPCSolverAdapter                    Self;
  typedef itk::Point<double, 2>               Point2DType;
  typedef itk::Point<double, 3>               Point3DType;
  typedef std::pair<Point2DType, Point3DType> GCPType;
  typedef std::vector<GCPType>                GCPsContainerType;

  /** Solve RPC modelling from a set of GCPs and image bounds.
   *  The estimated RPC model is written in a keywordlist understood
   *  by other OTB and classes (like GenericRSTransform for instance).
   *  Please note that at least 20 points are required to estimate the
   *  RPC model. Between 20 and 40 points, the estimated model will
   *  not provide elevation support, since there are not enough points
   *  to estimate all the coefficients. Starting at 40 points, a full
   *  RPC model is estimated.
   */
  static void Solve(const GCPsContainerType& gcpContainer,
                    double& rmsError,
                    ImageKeywordlist& otb_kwl);

  /** Solve RPC modelling from a set of GCPs and image bounds. The
   * estimated RPC model is written to a geom file.
   */
  static bool Solve(const GCPsContainerType& gcpContainer,
                    double& rmsError,
                    const std::string & outgeom);


private:
  RPCSolverAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  RPCSolverAdapter(); // purposely not implemented, this class defines
                      // typedefs and exposes static methods
  virtual ~RPCSolverAdapter(); // purposely not implemented, this class defines
                              // typedefs and exposes static methods
};

} // namespace otb

#endif
