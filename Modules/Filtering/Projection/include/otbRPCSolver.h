/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRPCSolver_h
#define otbRPCSolver_h

#include "itkPoint.h"
#include "otbGeometryMetadata.h"

namespace otb
{
  namespace RPCSolver
  {
    using Point2DType = itk::Point<double, 2>;
    using Point3DType = itk::Point<double, 3>;
    using GCPType = std::pair<Point2DType, Point3DType>;
    using GCPsContainerType = std::vector<GCPType>;

  /** Solve RPC modelling from a set of GCPs and image bounds.
   *  The estimated RPC model is written in a RPCParam structure that can
   *  for instance be added to an ImageMetadata object.
   *  Please note that at least 20 points are required to estimate the
   *  RPC model. Between 20 and 40 points, the estimated model will
   *  not provide elevation support, since there are not enough points
   *  to estimate all the coefficients. Starting at 40 points, a full
   *  RPC model is estimated.
   */
    void Solve(const GCPsContainerType& gcpContainer, double& rmsError, Projection::RPCParam& outputParams);
  };

}


#endif
