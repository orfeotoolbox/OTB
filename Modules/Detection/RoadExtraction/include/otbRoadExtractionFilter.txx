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
#ifndef otbRoadExtractionFilter_txx
#define otbRoadExtractionFilter_txx

#include "otbRoadExtractionFilter.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputPath>
RoadExtractionFilter<TInputImage, TOutputPath>
::RoadExtractionFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_SpectralAngleDistanceImageFilter = SpectralAngleDistanceImageFilterType::New();
  m_GenericRoadExtractionFilter = GenericRoadExtractionFilterType::New();

}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputPath>
void
RoadExtractionFilter<TInputImage, TOutputPath>
::GenerateData()
{
  // Input images pointers
  typename InputImageType::ConstPointer inputImage     = this->GetInput();
  typename OutputPathListType::Pointer  outputPathList  = this->GetOutput();

  m_SpectralAngleDistanceImageFilter->SetInput(inputImage);

  m_GenericRoadExtractionFilter->SetInput(m_SpectralAngleDistanceImageFilter->GetOutput());

  m_GenericRoadExtractionFilter->Update();
  for (typename GenericRoadExtractionFilterType::OutputPathListType::ConstIterator it
         = m_GenericRoadExtractionFilter->GetOutput()->Begin();
       it != m_GenericRoadExtractionFilter->GetOutput()->End();
       ++it)
    {
    outputPathList->PushBack(it.Get());
    }
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputPath>
void
RoadExtractionFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "m_ReferencePixel: " << m_SpectralAngleDistanceImageFilter->GetReferencePixel() << std::endl;
}

} // End namespace otb
#endif
