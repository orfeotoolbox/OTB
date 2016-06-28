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
#ifndef otbConcatenateScalarValueImageFilter_txx
#define otbConcatenateScalarValueImageFilter_txx

#include "otbConcatenateScalarValueImageFilter.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
ConcatenateScalarValueImageFilter<TInputImage, TOutputImage>
::ConcatenateScalarValueImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void
ConcatenateScalarValueImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(
    this->GetInput()->GetNumberOfComponentsPerPixel() + 1 );
}

template <class TInputImage, class TOutputImage>
void
ConcatenateScalarValueImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
