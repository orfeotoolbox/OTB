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
#ifndef otbImageToLineSpatialObjectListFilter_txx
#define otbImageToLineSpatialObjectListFilter_txx

#include "otbImageToLineSpatialObjectListFilter.h"

namespace otb
{

/**
 *
 */
template<class TInputImage>
ImageToLineSpatialObjectListFilter<TInputImage>::ImageToLineSpatialObjectListFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  typename LinesListType::Pointer list = LinesListType::New();
  this->SetOutput(list);

}

template <class TInputImage>
void
ImageToLineSpatialObjectListFilter<TInputImage>
::SetInput(const InputImageType *image)
{
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputImageType *>(image));
}

template <class TInputImage>
const typename ImageToLineSpatialObjectListFilter<TInputImage>::InputImageType *
ImageToLineSpatialObjectListFilter<TInputImage>
::GetInput(void)
{
  return static_cast<const InputImageType *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage>
void
ImageToLineSpatialObjectListFilter<TInputImage>
::SetOutput(const LinesListType *list)
{
  this->ProcessObjectType::SetNthOutput(0,
                                        const_cast<LinesListType *>(list));
}

template <class TInputImage>
typename ImageToLineSpatialObjectListFilter<TInputImage>::LinesListType *
ImageToLineSpatialObjectListFilter<TInputImage>
::GetOutput(void)
{
  return static_cast<LinesListType *>
           (this->ProcessObjectType::GetOutput(0));
}

/**
 *
 */
template<class TInputImage>
void
ImageToLineSpatialObjectListFilter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
