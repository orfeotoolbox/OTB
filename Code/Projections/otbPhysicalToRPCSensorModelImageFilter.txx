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
::PhysicalToRPCSensorModelImageFilter(): m_GridSpacing(2),m_DEMDirectory("") 
{
  // Initialize the rpc estimator
  m_GCPsToSensorModelFilter = GCPsToSensorModelType::New();
}

template <class TImage>
PhysicalToRPCSensorModelImageFilter<TImage>
::~PhysicalToRPCSensorModelImageFilter() 
{
}


template <class TImage>
void
PhysicalToRPCSensorModelImageFilter<TImage>
::GenerateOutputInformation()//GenerateData() 
{
  Superclass::GenerateOutputInformation();

  // Get the input 
  ImageType * input = const_cast<ImageType*>(this->GetInput());
  input->UpdateOutputInformation();

  // Build the grid 
  // Generate GCPs from physical sensor model
  RSTransformPointerType  rsTransform = RSTransformType::New();
  rsTransform->SetInputKeywordList(input->GetImageKeywordlist());
  
  if(!m_DEMDirectory.empty())
    {
    // Set the DEM to the Remote Sensing Transform
    rsTransform->SetDEMDirectory(m_DEMDirectory);
    m_GCPsToSensorModelFilter->SetUseDEM(true);
    }

  rsTransform->InstanciateTransform();
  
  // Compute the size of the grid 
  typename ImageType::SizeType  size = input->GetLargestPossibleRegion().GetSize();
  unsigned int gridSizeX = static_cast<unsigned int>(size[0]/m_GridSpacing);
  unsigned int gridSizeY = static_cast<unsigned int>(size[1]/m_GridSpacing);

  for(unsigned int px = 0; px<gridSizeX;++px)
    {
    for(unsigned int py = 0; py<gridSizeY;++py)
      {
      PointType inputPoint =  input->GetOrigin();
      inputPoint[0]+= (px * m_GridSpacing + 0.5) * input->GetSpacing()[0];
      inputPoint[1]+= (py * m_GridSpacing + 0.5) * input->GetSpacing()[1];
      PointType outputPoint = rsTransform->TransformPoint(inputPoint);
      //std::cout<<"Adding gcp "<<inputPoint<<" -> "<<outputPoint<<std::endl;
      m_GCPsToSensorModelFilter->AddGCP(inputPoint,outputPoint);
      }
    }
  
  m_GCPsToSensorModelFilter->SetInput(input);
  m_GCPsToSensorModelFilter->UpdateOutputInformation();
  std::cout<<"RPC model estimated. RMS ground error: "<<m_GCPsToSensorModelFilter->GetRMSGroundError()
           <<", Mean error: "<<m_GCPsToSensorModelFilter->GetMeanError()<<std::endl;
  
  // Encapsulate the keywordlist
  itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
  itk::EncapsulateMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, m_GCPsToSensorModelFilter->GetKeywordlist());
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
