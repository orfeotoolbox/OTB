/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbDrawLineSpatialObjectFilter_hxx
#define otbDrawLineSpatialObjectFilter_hxx

#include "otbDrawLineSpatialObjectFilter.h"

#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkImageRegionIterator.h"
#include "itkImageIteratorWithIndex.h"
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT;
namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>::DrawLineSpatialObjectFilter()
{
  OTB_DISABLE_DYNAMIC_MT;
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_Value = static_cast<OutputPixelType>(255.0);
  m_DrawLineListFilter = DrawLineSpatialObjectListFilterType::New();
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::SetInputLine(const InputLineType *line)
{
  this->ProcessObjectType::SetNthInput(1,
                                       const_cast<InputLineType *>(line));
}

template <class TInputImage, class TOutputImage>
typename DrawLineSpatialObjectFilter<TInputImage, TOutputImage>::InputLineType *
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::GetInputLine(void)
{
  return static_cast<InputLineType *>
           (this->ProcessObjectType::GetInput(1));
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>
::GenerateData(void)
{

  typename InputImageType::ConstPointer input    = this->GetInput();
  InputLineType *                       line     = this->GetInputLine();

  typename OutputImageType::Pointer output   = this->GetOutput();

  /** Create a new list line with one line*/
  LineSpatialObjectListPointer lineList = LineSpatialObjectListType::New();
  lineList->push_back(line);

  /** Invoke the DrawLineSpatialObjectListFilter to draw the line */
  m_DrawLineListFilter->SetInput(input);
  m_DrawLineListFilter->SetInputLineSpatialObjectList(lineList);

  m_DrawLineListFilter->GraftOutput(this->GetOutput());
  m_DrawLineListFilter->Update();
  this->GraftOutput(m_DrawLineListFilter->GetOutput());

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
DrawLineSpatialObjectFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_Value << std::endl;
}

} // end namespace otb

#endif
