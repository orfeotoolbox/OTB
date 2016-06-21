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
#ifndef otbPersistentVectorizationImageFilter_txx
#define otbPersistentVectorizationImageFilter_txx

#include "otbPersistentVectorizationImageFilter.h"

#include "itkProgressReporter.h"

namespace otb
{

template<class TInputImage, class TOutputPath>
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::PersistentVectorizationImageFilter()
{
  m_MinMaxFilter = MinMaxFilterType::New();
  m_PathList = PathListType::New();
}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::Reset()
{
  m_PathList->Clear();
}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::Synthetize()
{}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::GenerateData()
{
  // Compute the min max and handle mini-pipeline
  m_MinMaxFilter->SetInput(this->GetInput());
  m_MinMaxFilter->GraftOutput(this->GetOutput());
  m_MinMaxFilter->Update();
  this->GraftOutput(m_MinMaxFilter->GetOutput());

  for (PixelType label = m_MinMaxFilter->GetMinimum() + 1; label <= m_MinMaxFilter->GetMaximum(); ++label)
    {
    ImageToEdgePathFilterPointerType edgeFilter = ImageToEdgePathFilterType::New();
    edgeFilter->SetInput(m_MinMaxFilter->GetOutput());
    edgeFilter->SetForegroundValue(label);
    edgeFilter->Update();
    m_PathList->PushBack(edgeFilter->GetOutput());
    }
}

template<class TInputImage, class TOutputPath>
void
PersistentVectorizationImageFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
