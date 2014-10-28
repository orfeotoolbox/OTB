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
#ifndef __otbDrawLineSpatialObjectFilter_txx
#define __otbDrawLineSpatialObjectFilter_txx

#include "otbDrawLineSpatialObjectFilter.h"

#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkImageRegionIterator.h"
#include "itkImageIteratorWithIndex.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectFilter<TInputImage, TOutputImage>::DrawLineSpatialObjectFilter()
{
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
