/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbShiftScaleVectorImageFilter_txx
#define otbShiftScaleVectorImageFilter_txx

#include "otbShiftScaleVectorImageFilter.h"

namespace otb
{
/**
 * Generate output information.
 */
template <class TInputImage, class TOutputImage>
void
ShiftScaleVectorImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  this->Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(
                 this->GetInput()->GetNumberOfComponentsPerPixel()
                                                   );
}
/**
 * Generate input requested region.
 */
template <class TInputImage, class TOutputImage>
void
ShiftScaleVectorImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion(void)
{
  if (this->GetInput())
    {
    typename TInputImage::Pointer    input = const_cast<TInputImage *>(this->GetInput());
    typename TInputImage::RegionType inputRegion;
    this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
    input->SetRequestedRegion(inputRegion);
    }
}
/**
 * Process to execute before entering the multithreaded section.
 */
template <class TInputImage, class TOutputImage>
void
ShiftScaleVectorImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // set up the functor values
  this->GetFunctor().SetScaleValues(m_Scale);
  this->GetFunctor().SetShiftValues(m_Shift);
}

} // end namespace otb
#endif
