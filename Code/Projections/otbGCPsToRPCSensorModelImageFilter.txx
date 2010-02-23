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
#ifndef __otbGCPsToRPCSensorModelImageFilter_txx
#define __otbGCPsToRPCSensorModelImageFilter_txx

#include "otbGCPsToRPCSensorModelImageFilter.h"

#include "itkMetaDataObject.h"

#include "projection/ossimRpcSolver.h"
#include "imaging/ossimImageGeometry.h"

namespace otb {

template < class TImage >
GCPsToRPCSensorModelImageFilter<TImage>
::GCPsToRPCSensorModelImageFilter() : m_UseImageGCPs(true), 
                                  m_RMSGroundError(0.), 
                                  m_UseDEM(false),
                                  m_MeanElevation(0.), 
                                  m_DEMHandler(),
                                  m_GCPsContainer()
{
  // This filter does not modify the image buffer, but only its
  // metadata.Therefore, it can be run inplace to reduce memory print.
  this->SetInPlace(true);

  // Clear the GCPs container
  this->ClearGCPs();

  /** Create the DEM handler */
  m_DEMHandler = DEMHandler::New();
}

template < class TImage >
GCPsToRPCSensorModelImageFilter< TImage >
::~GCPsToRPCSensorModelImageFilter()
{
  // Clear the GCPs container
  this->ClearGCPs();
}

template < class TImage >
typename GCPsToRPCSensorModelImageFilter< TImage >
::GCPsContainerType &
GCPsToRPCSensorModelImageFilter< TImage >
::GetGCPsContainer()
{
  // return the GCPs container
  return m_GCPsContainer;
}

template < class TImage >
void
GCPsToRPCSensorModelImageFilter< TImage >
::SetGCPsContainer(const GCPsContainerType & container)
{
  // Set the GCPs container
  m_GCPsContainer = container;
  // Call modified method
  this->Modified();
}

template < class TImage >
void
GCPsToRPCSensorModelImageFilter< TImage >
::AddGCP(const Point2DType& sensorPoint, const Point3DType & groundPoint)
{
  // Create a new GCP
  GCPType newGCP(sensorPoint,groundPoint);
  // Add it to the container
  m_GCPsContainer.push_back(newGCP);
  // Call the modified method */
  this->Modified();
}

template < class TImage >
void
GCPsToRPCSensorModelImageFilter< TImage >
::AddGCP(const Point2DType& sensorPoint, const Point2DType & groundPoint)
{
  // Create the ground point with elevation
  Point3DType groundPointWithElevation;
  // Fill it with ground point data
  groundPointWithElevation[0] = groundPoint[0];
  groundPointWithElevation[1] = groundPoint[1];
  
  // Check wether we are using a DEM or not
  if(m_UseDEM)
    {
    // If so, use it to get the elevation
    groundPointWithElevation[2] = this->GetHeightAboveMSL(groundPoint);
    }
  else
    {
    // Use the MeanElevation value
    groundPointWithElevation[2] = m_MeanElevation;
    }

  // Call the 3D version of the method
  this->AddGCP(sensorPoint,groundPointWithElevation);
}

template < class TImage >
void
GCPsToRPCSensorModelImageFilter< TImage >
::ClearGCPs()
{
  m_GCPsContainer.clear();
  this->Modified();
}

template< class TImage >
void
GCPsToRPCSensorModelImageFilter< TImage >
::GenerateOutputInformation()
{
  // First, retrieve the image pointer
  typename TImage::Pointer imagePtr = this->GetOutput();

  // The vector where geo and sensor points are stored
  std::vector<ossimDpt> sensorPoints;
  std::vector<ossimGpt>  geoPoints;

  // Temporary points variable
  ossimDpt sensorPoint;
  ossimGpt geoPoint;
  
  // Handle the UseImageGCPs case
  if(m_UseImageGCPs)
    {
    // Iterate on the image GCPs
    for( unsigned int i=0; i<imagePtr->GetGCPCount(); i++ )
      {
      // Store row/col in an index
      typename TImage::IndexType index;
      index[0] = static_cast<long int>(imagePtr->GetGCPCol(i));
      index[1] = static_cast<long int>(imagePtr->GetGCPRow(i));

      // Transform to physical point
      Point2DType physPoint;
      imagePtr->TransformIndexToPhysicalPoint(index,physPoint);
    
      // Fill sensor point
      sensorPoint = ossimDpt(physPoint[0],physPoint[1]);
    
      // Fill geo point
      geoPoint = ossimGpt(imagePtr->GetGCPX(i),imagePtr->GetGCPY(i),
                       imagePtr->GetGCPZ(i));
    
      // Add the sensor point to the list
      sensorPoints.push_back(sensorPoint);
      
      // Add the geo point to the list 
      geoPoints.push_back(geoPoint);
      }
    }

  // Retrieve the additional GCPs
  typename GCPsContainerType::const_iterator gcpIt;
  for(gcpIt = m_GCPsContainer.begin(); gcpIt!=m_GCPsContainer.end();++gcpIt)
    {
    // Fill sensor point
    sensorPoint = ossimDpt(gcpIt->first[0],gcpIt->first[1]);
    
    // Fill geo point
    geoPoint = ossimGpt(gcpIt->second[0],gcpIt->second[1],gcpIt->second[2]);
    
    // Add the sensor point to the list
    sensorPoints.push_back(sensorPoint);

    // Add the geo point to the list 
    geoPoints.push_back(geoPoint);
    }

  // Build the ossim rpc solver
  ossimRefPtr<ossimRpcSolver> rpcSolver = new ossimRpcSolver(false,false);

  // Call the solve method
  rpcSolver->solveCoefficients(sensorPoints,geoPoints);

  // Retrieve the residual ground error
  m_RMSGroundError = rpcSolver->getRmsError();

  // Retrieve the output RPC projection
  ossimRefPtr<ossimRpcModel> rpcModel = dynamic_cast<ossimRpcModel*>(rpcSolver->createRpcModel()->getProjection());
  
  // Export the sensor model in an ossimKeywordlist
  ossimKeywordlist geom_kwl;
  rpcModel->saveState(geom_kwl);

  // Build an otb::ImageKeywordList
  ImageKeywordlist otb_kwl;
  otb_kwl.SetKeywordlist(geom_kwl);

  // Encapsulate it
  itk::MetaDataDictionary& dict = imagePtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData< ImageKeywordlist >( dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl );
}

template < class TImage >
void
GCPsToRPCSensorModelImageFilter< TImage >
::PrintSelf( std::ostream & os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
  os<<indent<<"UseImageGCPs: "<<(m_UseImageGCPs ? "yes" : "no")<<std::endl;
  os<<indent<<"UseDEM: "<<(m_UseDEM ? "yes" : "no")<<std::endl;
  if(!m_UseDEM)
    {
    os<<indent<<"MeanElevation: "<<m_MeanElevation<<std::endl;
    }
  os<<indent<<"RMS ground error: "<<m_RMSGroundError<<std::endl;
}

} // end of namespace otb

#endif


