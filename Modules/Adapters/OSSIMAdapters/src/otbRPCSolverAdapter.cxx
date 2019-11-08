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


#include "otbRPCSolverAdapter.h"
#include "otbImageKeywordlist.h"
#include "otbMacro.h"

#include "otb_ossim.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/projection/ossimRpcSolver.h"
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/imaging/ossimImageGeometry.h"
#pragma GCC diagnostic pop
#else
#include "ossim/projection/ossimRpcSolver.h"
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/imaging/ossimImageGeometry.h"
#endif


namespace otb
{

void RPCSolverAdapter::Solve(const GCPsContainerType& gcpContainer, double& rmsError, ImageKeywordlist& otb_kwl)
{
  // TODO: for now, this is a simple transfer of what was done in the
  // GCPsToRPCSensorModelImageFilter. We might find some way to improve that.

  // The vector where geo and sensor points are stored
  std::vector<ossimDpt> sensorPoints;
  std::vector<ossimGpt> geoPoints;

  // Temporary points variable
  ossimDpt sensorPoint;
  ossimGpt geoPoint;

  // Retrieve the additional GCPs
  GCPsContainerType::const_iterator gcpIt;
  for (gcpIt = gcpContainer.begin(); gcpIt != gcpContainer.end(); ++gcpIt)
  {
    // Check if point is inside bounds
    // Fill sensor point
    sensorPoint = ossimDpt(internal::ConvertToOSSIMFrame(gcpIt->first[0]), internal::ConvertToOSSIMFrame(gcpIt->first[1]));

    // Fill geo point (lat, lon, elev)
    geoPoint = ossimGpt(gcpIt->second[1], gcpIt->second[0], gcpIt->second[2]);

    // Add the sensor point to the list
    sensorPoints.push_back(sensorPoint);

    // Add the geo point to the list
    geoPoints.push_back(geoPoint);
  }

  // Check for enough points
  if (sensorPoints.size() < 20)
  {
    itkGenericExceptionMacro(<< "At least 20 points are required to estimate the 40 parameters of a RPC model without elevation support, and 40 are required "
                                "to estimate the 80 parameters of a RPC model with elevation support. Only "
                             << sensorPoints.size() << " points were given.");
  }

  // By default, use elevation provided with ground control points
  bool useElevation = true;

  // If not enough points are given for a proper estimation of RPC
  // with elevation support, disable elevation. This will result in
  // all coefficients related to elevation set to zero.
  if (sensorPoints.size() < 40)
  {
    otbGenericWarningMacro("Only " << sensorPoints.size() << " ground control points are provided, can not estimate a RPC model with elevation support (at "
                                                             "least 40 points required). Elevation support will be disabled for RPC estimation. All "
                                                             "coefficients related to elevation will be set to zero, and elevation will have no effect on the "
                                                             "resulting transform.");
    useElevation = false;
  }

  // Build the ossim rpc solver
  ossimRefPtr<ossimRpcSolver> rpcSolver = new ossimRpcSolver(useElevation, false);

  // Call the solve method
  rpcSolver->solveCoefficients(sensorPoints, geoPoints);

  rmsError = rpcSolver->getRmsError();

// Retrieve the output RPC projection
#if OTB_OSSIM_VERSION < 20200
  ossimRefPtr<ossimRpcProjection> rpcProjection = dynamic_cast<ossimRpcProjection*>(rpcSolver->createRpcProjection()->getProjection());
#else
  ossimRefPtr<ossimRpcModel> rpcProjection = rpcSolver->getRpcModel();
#endif

  // Export the sensor model in an ossimKeywordlist
  ossimKeywordlist geom_kwl;
  rpcProjection->saveState(geom_kwl);

  // Build an otb::ImageKeywordList
  otb_kwl.SetKeywordlist(geom_kwl);
}

bool RPCSolverAdapter::Solve(const GCPsContainerType& gcpContainer, double& rmsError, const std::string& outgeom)
{
  // Declare an empty keywordlist
  ImageKeywordlist otb_kwl;

  // Call the Solve method that actually does the solving
  Solve(gcpContainer, rmsError, otb_kwl);

  // Write the keywordlist to disk
  ossimKeywordlist kwl;
  otb_kwl.convertToOSSIMKeywordlist(kwl);
  return kwl.write(outgeom.c_str());
}


} // namespace otb
