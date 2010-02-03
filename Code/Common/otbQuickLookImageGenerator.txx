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
#include "otbQuickLookImageGenerator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
QuickLookImageGenerator<TInputImage, TOutputImage>
::QuickLookImageGenerator()
{
  m_SampleRatio = 1;
  m_Variance = 0.3;
  m_MaximumError = 0.01;
  m_MaximumKernelWidth = 32;
  m_UseImageSpacing = true;

  // Internal filter
  m_PerBandFilter  = PerBandFilterType::New();
  m_ShrinkFilter   = ShrinkImageFilterType::New();
  m_GaussianFilter = GaussianFilterType::New();
  
  m_PerBandFilter->SetFilter( m_GaussianFilter );
  m_ShrinkFilter->SetInput( m_PerBandFilter->GetOutput() );
}
  

template <class TInputImage, class TOutputImage>
void
QuickLookImageGenerator<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  
  m_PerBandFilter->SetInput(this->GetInput());
  m_PerBandFilter->GenerateOutputInformation();
  m_ShrinkFilter->GenerateOutputInformation();
  this->GetOutput()->SetLargestPossibleRegion( m_ShrinkFilter->GetOutput()->GetLargestPossibleRegion() );
}
  
template <class TInputImage, class TOutputImage>
void
QuickLookImageGenerator<TInputImage,TOutputImage>
::GenerateData()
{
  m_PerBandFilter->SetInput(this->GetInput());
  
  m_ShrinkFilter->GraftOutput(this->GetOutput());
  
  m_ShrinkFilter->Update();
  this->GraftOutput(m_ShrinkFilter->GetOutput());
}

template <class TInputImage, class TOutputImage>
void
QuickLookImageGenerator<TInputImage, TOutputImage>::
PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);

  os << "Sample Ratio : "<< m_SampleRatio<<std::endl;
  os << "Gaussian Filter Parameters : "<<std::endl;
  os << "Variance             : "<< m_Variance<<std::endl;
  os << "Maximum Kernel Width : "<< m_MaximumKernelWidth<<std::endl;
  os << "Maximum Error        : "<< m_MaximumError<<std::endl;
  os << "Use Image Spacing    : "<< m_UseImageSpacing<<std::endl;
}
  

} // End namespace otb
