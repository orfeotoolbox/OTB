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
#ifndef __otbClosingOpeningMorphologicalFilter_txx
#define __otbClosingOpeningMorphologicalFilter_txx

#include "otbClosingOpeningMorphologicalFilter.h"

#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"
#include "itkProgressAccumulator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TKernel>
ClosingOpeningMorphologicalFilter<TInputImage, TOutputImage, TKernel>
::ClosingOpeningMorphologicalFilter()
{
  m_Kernel.SetRadius(1);
  m_Kernel.CreateStructuringElement();
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TKernel>
void
ClosingOpeningMorphologicalFilter<TInputImage, TOutputImage, TKernel>
::GenerateData()
{
  // Filters Typedefs (this class is actually a composite filter)
  typedef itk::GrayscaleMorphologicalOpeningImageFilter<InputImageType, OutputImageType, KernelType> OpenFilterType;
  typedef itk::GrayscaleMorphologicalClosingImageFilter<InputImageType, OutputImageType, KernelType> CloseFilterType;
  // Filters initialization
  typename OpenFilterType::Pointer  opening = OpenFilterType::New();
  typename CloseFilterType::Pointer closing = CloseFilterType::New();
  // Set the kernel
  opening->SetKernel(this->GetKernel());
  closing->SetKernel(this->GetKernel());
  // Progression report management
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(closing, .5f);
  progress->RegisterInternalFilter(opening, .5f);
  // Internal Pipeline connexion
  opening->SetInput(this->GetInput());
  closing->SetInput(opening->GetOutput());
  // Output connexion
  closing->GraftOutput(this->GetOutput());
  closing->Update();
  this->GraftOutput(closing->GetOutput());
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage, class TKernel>
void
ClosingOpeningMorphologicalFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Kernel: " << m_Kernel << std::endl;
}
} // End namespace otb
#endif
