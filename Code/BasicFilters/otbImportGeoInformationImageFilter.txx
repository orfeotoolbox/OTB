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
#ifndef _otbImportGeoInformationImageFilter_txx
#define _otbImportGeoInformationImageFilter_txx

#include "itkDataObject.h"
#include "otbImportGeoInformationImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage, class TSourceImage>
ImportGeoInformationImageFilter<TImage,TSourceImage>
::ImportGeoInformationImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNthInput(1,SourceImageType::New().GetPointer());
}

template <class TImage, class TSourceImage>
void
ImportGeoInformationImageFilter<TImage,TSourceImage>
::SetSource(const TSourceImage * source)
{
  this->SetNthInput(1,const_cast<TSourceImage *>(source));
}
template <class TImage, class TSourceImage>
const TSourceImage *
ImportGeoInformationImageFilter<TImage,TSourceImage>
::GetSource()
{
  return static_cast<const TSourceImage *>(this->GetInput(1));
}
/**
 * Main computation method.
 */
template <class TImage, class TSourceImage>
void
 ImportGeoInformationImageFilter<TImage,TSourceImage>
::GenerateData(void)
{
  // Since this filter is not multi-threaded, we have to call this method by ourselves
  this->AllocateOutputs();
  // Get output and source pointer
  ImagePointerType outputPtr = this->GetOutput();
  const SourceImageType * sourcePtr = this->GetSource();
  // Import metdata
  outputPtr->CopyInformation(sourcePtr);
}
/**
 * PrintSelf Method
 */
template <class TImage, class TSourceImage>
void
ImportGeoInformationImageFilter<TImage,TSourceImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
