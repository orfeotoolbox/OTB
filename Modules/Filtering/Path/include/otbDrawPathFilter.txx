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

#ifndef otbDrawPathFilter_txx
#define otbDrawPathFilter_txx

#include "otbDrawPathFilter.h"
#include "otbDrawPathListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TInputPath, class TOutputImage>
DrawPathFilter<TInputImage, TInputPath, TOutputImage>
::DrawPathFilter()
{
  m_Value = static_cast<OutputImagePixelType>(255.0);
}

template <class TInputImage, class TInputPath, class TOutputImage>
inline void DrawPathFilter<TInputImage, TInputPath, TOutputImage>
::SetInputPath(const TInputPath * path)
{
  this->SetPathInput(path);
}

template <class TInputImage, class TInputPath, class TOutputImage>
const TInputPath * DrawPathFilter<TInputImage, TInputPath, TOutputImage>
::GetInputPath(void)
{
  return this->GetPathInput();
}

/**
 * Main computation method.
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
DrawPathFilter<TInputImage, TInputPath, TOutputImage>
::GenerateData(void)
{
  typedef otb::DrawPathListFilter<TInputImage, TInputPath, TOutputImage> DrawListFilterType;
  typedef typename DrawListFilterType::InputPathListType                 PathListType;
  typename PathListType::Pointer list = PathListType::New();
  InputPathPointer               path = const_cast<TInputPath*>(this->GetPathInput());
  list->PushBack(path);

  typename DrawListFilterType::Pointer drawer = DrawListFilterType::New();
  drawer->SetInput(this->GetImageInput());
  drawer->SetInputPath(list);
  drawer->SetPathValue(m_Value);

  drawer->GraftOutput(this->GetOutput());
  drawer->Update();
  this->GraftOutput(drawer->GetOutput());
}
/**
 * Printself Method
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
DrawPathFilter<TInputImage, TInputPath, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_Value << std::endl;
}
} // end namespace otb

#endif
