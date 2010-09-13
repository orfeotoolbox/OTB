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
#ifndef __otbPhysicalToRPCSensorModelImageFilter_txx
#define __otbPhysicalToRPCSensorModelImageFilter_txx

#include "otbPhysicalToRPCSensorModelImageFilter.h"
#include "otbDEMHandler.h"

namespace otb {

template <class TImage>
PhysicalToRPCSensorModelImageFilter<TImage>
::PhysicalToRPCSensorModelImageFilter()
{
  // Initialize the rpc estimator
  m_GCPsToSensorModelFilter = GCPsToSensorModelType::New();
  
  // Initialize the DEMDirectory
  m_DEMDirectory = "";
  
  // Initialize the average elevation used
  //  0. or -32768.0 ???
  m_AverageElevation = 0.;

  // Initialize the gridSize : 16 points to have a correct estimation
  // of the model
  m_GridSize.Fill(4);
}

template <class TImage>
PhysicalToRPCSensorModelImageFilter<TImage>
::~PhysicalToRPCSensorModelImageFilter() 
{
}

template <class TImage>
void
PhysicalToRPCSensorModelImageFilter<TImage>
::GenerateOutputInformation() 
{
  Superclass::GenerateOutputInformation();

  // Get the input 
  ImageType * input = const_cast<ImageType*>(this->GetInput());

  // Build the grid 
  // Generate GCPs from physical sensor model
  RSTransformPointerType  rsTransform = RSTransformType::New();
  rsTransform->SetInputKeywordList(input->GetImageKeywordlist());
  
  if(!m_DEMDirectory.empty())
    {
    // Set the DEM & Average Elevation to the Remote Sensing Transform
    rsTransform->SetDEMDirectory(m_DEMDirectory);
    rsTransform->SetAverageElevation(m_AverageElevation);
    
    // Generate DEMHandler & set it to the GCP2sensorModel
    typename DEMHandler::Pointer demHandler = DEMHandler::New();
    demHandler->OpenDEMDirectory(m_DEMDirectory.c_str());
    m_GCPsToSensorModelFilter->SetUseDEM(true);
    m_GCPsToSensorModelFilter->SetDEMHandler(demHandler);
    }

  rsTransform->InstanciateTransform();
  
  // Compute the size of the grid 
  typename ImageType::SizeType  size = input->GetLargestPossibleRegion().GetSize();
  double gridSpacingX = size[0]/m_GridSize[0];
  double gridSpacingY = size[1]/m_GridSize[1];
  
  for(unsigned int px = 0; px<m_GridSize[0];++px)
    {
    for(unsigned int py = 0; py<m_GridSize[1];++py)
      {
      PointType inputPoint =  input->GetOrigin();
      inputPoint[0]+= (px * gridSpacingX + 0.5) * input->GetSpacing()[0];
      inputPoint[1]+= (py * gridSpacingY + 0.5) * input->GetSpacing()[1];
      PointType outputPoint = rsTransform->TransformPoint(inputPoint);
      m_GCPsToSensorModelFilter->AddGCP(inputPoint,outputPoint);
      }
    }
  
  m_GCPsToSensorModelFilter->SetInput(input);
  m_GCPsToSensorModelFilter->UpdateOutputInformation();
  std::cout<<"RPC model estimated. RMS ground error: "<<m_GCPsToSensorModelFilter->GetRMSGroundError()
           <<", Mean error: "<<m_GCPsToSensorModelFilter->GetMeanError()<<std::endl;
  
  // Encapsulate the keywordlist
  itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
  itk::EncapsulateMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, 
                                             m_GCPsToSensorModelFilter->GetKeywordlist());
}

template <class TImage>
void
PhysicalToRPCSensorModelImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end of namespace otb

#endif
