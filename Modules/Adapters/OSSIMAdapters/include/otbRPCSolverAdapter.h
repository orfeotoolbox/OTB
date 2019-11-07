/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbRPCSolverAdapter_h
#define otbRPCSolverAdapter_h

#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"

#include "itkPoint.h"
#include "itkSize.h"

#include "OTBOSSIMAdaptersExport.h"

class ossimRpcSolver;

namespace otb
{

class ImageKeywordlist;

/** \class RPCSolverAdapter
 *  \brief This is a dummy class to expose the RPC solving function
 *
 *  This class defines some typedefs and exposes the RPC solving functions.
 *
 * \sa GCPsToRPCSensorModelImageFilter
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT RPCSolverAdapter
{
public:
  /** Standard class typedefs. */
  typedef RPCSolverAdapter Self;
  typedef itk::Point<double, 2>               Point2DType;
  typedef itk::Point<double, 3>               Point3DType;
  typedef std::pair<Point2DType, Point3DType> GCPType;
  typedef std::vector<GCPType> GCPsContainerType;

  /** Solve RPC modelling from a set of GCPs and image bounds.
   *  The estimated RPC model is written in a keywordlist understood
   *  by other OTB and classes (like GenericRSTransform for instance).
   *  Please note that at least 20 points are required to estimate the
   *  RPC model. Between 20 and 40 points, the estimated model will
   *  not provide elevation support, since there are not enough points
   *  to estimate all the coefficients. Starting at 40 points, a full
   *  RPC model is estimated.
   */
  static void Solve(const GCPsContainerType& gcpContainer, double& rmsError, ImageKeywordlist& otb_kwl);

  /** Solve RPC modelling from a set of GCPs and image bounds. The
   * estimated RPC model is written to a geom file.
   */
  static bool Solve(const GCPsContainerType& gcpContainer, double& rmsError, const std::string& outgeom);


private:
  RPCSolverAdapter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // This class defines typedefs and exposes static methods
  RPCSolverAdapter()          = delete;
  virtual ~RPCSolverAdapter() = delete;
};

} // namespace otb

#endif
