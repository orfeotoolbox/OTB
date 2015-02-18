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
  // This filter does not modify the image buffer, but only its
  // metadata.Therefore, it can be run inplace to reduce memory print.
  // CastImageFilter has InPlaceOff by default (see UnaryFunctorImgeFilter constructor)
  this->InPlaceOn();

  // Initialize the rpc estimator
  m_GCPsToSensorModelFilter = GCPsToSensorModelType::New();

  // Initialize the gridSize : 16 points to have a correct estimation
  // of the model
  m_GridSize.Fill(4);

  // Flag initilalisation
  m_OutputInformationGenerated = false;
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

  if(!m_OutputInformationGenerated)
    {

    // Get the input
    ImageType * input = const_cast<ImageType*>(this->GetInput());

    // Build the grid
    // Generate GCPs from physical sensor model
    RSTransformPointerType  rsTransform = RSTransformType::New();
    rsTransform->SetInputKeywordList(input->GetImageKeywordlist());
    rsTransform->InstanciateTransform();

    // Compute the size of the grid
    typename ImageType::SizeType  size = input->GetLargestPossibleRegion().GetSize();
    double gridSpacingX = size[0]/m_GridSize[0];
    double gridSpacingY = size[1]/m_GridSize[1];

    for(unsigned int px = 0; px<m_GridSize[0]; ++px)
      {
      for(unsigned int py = 0; py<m_GridSize[1]; ++py)
        {
        PointType inputPoint =  input->GetOrigin();
        inputPoint[0] += (px * gridSpacingX + 0.5) * input->GetSpacing()[0];
        inputPoint[1] += (py * gridSpacingY + 0.5) * input->GetSpacing()[1];
        PointType outputPoint = rsTransform->TransformPoint(inputPoint);
        m_GCPsToSensorModelFilter->AddGCP(inputPoint, outputPoint);
        }
      }

    m_GCPsToSensorModelFilter->SetInput(input);
    m_GCPsToSensorModelFilter->UpdateOutputInformation();

    otbGenericMsgDebugMacro(<<"RPC model estimated. RMS ground error: "<<m_GCPsToSensorModelFilter->GetRMSGroundError()
             <<", Mean error: "<<m_GCPsToSensorModelFilter->GetMeanError());

    // Encapsulate the keywordlist
    itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
    itk::EncapsulateMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey,
                                               m_GCPsToSensorModelFilter->GetKeywordlist());

    // put the flag to true
    m_OutputInformationGenerated = true;
    }
}

template <class TImage>
void
PhysicalToRPCSensorModelImageFilter<TImage>
::Modified()
{
  Superclass::Modified();
  m_OutputInformationGenerated= false;
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
