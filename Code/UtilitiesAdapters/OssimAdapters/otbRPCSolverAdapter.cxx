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

#include "otbRPCSolverAdapter.h"
#include "otbImageKeywordlist.h"

#include "ossim/projection/ossimRpcSolver.h"
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/imaging/ossimImageGeometry.h"

namespace otb
{

RPCSolverAdapter::RPCSolverAdapter()
{}

RPCSolverAdapter::~RPCSolverAdapter()
{}

void
RPCSolverAdapter::Solve(const GCPsContainerType& gcpContainer, 
                        const Point2DType& ulImagePoint, 
                        const Point2DType& lrImagePoint,
                        double& rmsError, 
                        ImageKeywordlist& otb_kwl)
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
    if(gcpIt->first[0] >= ulImagePoint[0] && gcpIt->first[1] >= ulImagePoint[1]
    && gcpIt->first[0] <= lrImagePoint[0] && gcpIt->first[1] <= lrImagePoint[1])
      {
      // Fill sensor point
      sensorPoint = ossimDpt(gcpIt->first[0], gcpIt->first[1]);
      
      // Fill geo point (lat, lon, elev)
      geoPoint =  ossimGpt(gcpIt->second[1], gcpIt->second[0], gcpIt->second[2]);
      
      // Add the sensor point to the list
      sensorPoints.push_back(sensorPoint);

      // Add the geo point to the list
      geoPoints.push_back(geoPoint);
      }
    }

  // Check for enough points
  if(sensorPoints.size() < 40)
    {
    itkGenericExceptionMacro(<<"At least 40 points are required to estimate the 80 parameters of the RPC model, but only "<<sensorPoints.size()<<" were given.");
    }
  
  // Build the ossim rpc solver
  ossimRefPtr<ossimRpcSolver> rpcSolver = new ossimRpcSolver(true, false);

  // Call the solve method
  rpcSolver->solveCoefficients(sensorPoints, geoPoints);

  rmsError = rpcSolver->getRmsError();

  // Retrieve the output RPC projection
  ossimRefPtr<ossimRpcModel> rpcModel = dynamic_cast<ossimRpcModel*>(rpcSolver->createRpcModel()->getProjection());

  // Add spacings
  rpcModel->setMetersPerPixel(rpcSolver->createRpcProjection()->getProjection()->getMetersPerPixel());

  // Add ground rectangle
  ossimDrect imageRectangle(ulImagePoint[0],ulImagePoint[1],lrImagePoint[0],lrImagePoint[1]);
  
  ossimGpt ulGround, lrGround, urGround, llGround;

  rpcModel->lineSampleToWorld(imageRectangle.ul(),ulGround);
  rpcModel->lineSampleToWorld(imageRectangle.lr(),lrGround);
  rpcModel->lineSampleToWorld(imageRectangle.ur(),urGround);
  rpcModel->lineSampleToWorld(imageRectangle.ll(),llGround);

  rpcModel->setGroundRect(ulGround, urGround, lrGround, llGround);

  // Export the sensor model in an ossimKeywordlist
  ossimKeywordlist geom_kwl;
  rpcModel->saveState(geom_kwl);
  
  // Build an otb::ImageKeywordList
  otb_kwl.SetKeywordlist(geom_kwl);
}

bool RPCSolverAdapter::Solve(const GCPsContainerType& gcpContainer,
                    const Point2DType& ulImagePoint, 
                    const Point2DType& lrImagePoint,
                    double& rmsError, 
                    const std::string & outgeom)
 {
   // Declare an empty keywordlist
   ImageKeywordlist otb_kwl;

   // Call the Solve method that actually does the solving
   Solve(gcpContainer,ulImagePoint,lrImagePoint,rmsError,otb_kwl);

   // Write the keywordlist to disk
   ossimKeywordlist kwl;
   otb_kwl.convertToOSSIMKeywordlist(kwl);
   return kwl.write(outgeom.c_str());
 }


} // namespace otb
