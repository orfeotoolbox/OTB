/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImportImageFilter_hxx
#define otbImportImageFilter_hxx

#include "otbImportImageFilter.h"
#include "itkObjectFactory.h"

namespace otb
{

/**
 *
 */
template <class TOutputImage>
ImportImageFilter<TOutputImage>
::ImportImageFilter()
{
  unsigned int idx;

  for (idx = 0; idx < TOutputImage::ImageDimension; ++idx)
    {
    m_Spacing[idx] = 1.0;
    m_Origin[idx] = 0.0;
    }
  m_Direction.SetIdentity();

  m_ImportPointer = nullptr;
  m_FilterManageMemory = false;
  m_Size = 0;
}

/**
 *
 */
template <class TOutputImage>
ImportImageFilter<TOutputImage>
::~ImportImageFilter()
{
  if (m_ImportPointer && m_FilterManageMemory)
    {
    delete[] m_ImportPointer;
    }
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  int i;

  Superclass::PrintSelf(os, indent);

  if (m_ImportPointer)
    {
    os << indent << "Imported pointer: (" << m_ImportPointer  << ")" << std::endl;
    }
  else
    {
    os << indent << "Imported pointer: (None)" << std::endl;
    }
  os << indent << "Import buffer size: " << m_Size << std::endl;
  os << indent << "Import buffer size: " << m_Size << std::endl;
  os << indent << "Filter manages memory: " << (m_FilterManageMemory ? "true" : "false") << std::endl;

  os << indent << "Spacing: [";
  for (i = 0; i < static_cast<int>(TOutputImage::ImageDimension) - 1; ++i)
    {
    os << m_Spacing[i] << ", ";
    }
  os << m_Spacing[i] << "]" << std::endl;

  os << indent << "Origin: [";
  for (i = 0; i < static_cast<int>(TOutputImage::ImageDimension) - 1; ++i)
    {
    os << m_Origin[i] << ", ";
    }
  os << m_Origin[i] << "]" << std::endl;
  os << indent << "Direction: " << std::endl << this->GetDirection() << std::endl;
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::SetImportPointer(TPixel *ptr, unsigned long num, bool LetFilterManageMemory)
{
  if (ptr != m_ImportPointer)
    {
    if (m_ImportPointer && m_FilterManageMemory)
      {
      delete[] m_ImportPointer;
      }
    m_ImportPointer = ptr;
    this->Modified();
    }
  m_FilterManageMemory = LetFilterManageMemory;
  m_Size = num;
}

/**
 *
 */
template <class TOutputImage>
typename ImportImageFilter<TOutputImage>::TPixel*
ImportImageFilter<TOutputImage>
::GetImportPointer()
{
  return m_ImportPointer;
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::EnlargeOutputRequestedRegion(itk::DataObject *output)
{
  // call the superclass' implementation of this method
  Superclass::EnlargeOutputRequestedRegion(output);

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // set the requested region to the largest possible region (in this case
  // the amount of data that we have)
  outputPtr->SetRequestedRegion(outputPtr->GetLargestPossibleRegion());
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // we need to compute the output spacing, the output origin, the
  // output image size, and the output image start index
  outputPtr->SetSignedSpacing(m_Spacing);
  outputPtr->SetOrigin(m_Origin);
  outputPtr->SetDirection(m_Direction);
  outputPtr->SetLargestPossibleRegion(m_Region);
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::GenerateData()
{
  // Normally, GenerateData() allocates memory.  However, the application
  // provides the memory for this filter via the SetImportPointer() method.
  // Therefore, this filter does not call outputPtr->Allocate().

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();

  // the output buffer size is set to the size specified by the user via the
  // SetRegion() method.
  outputPtr->SetBufferedRegion(outputPtr->GetLargestPossibleRegion());

  // pass the pointer down to the container during each Update() since
  // a call to Initialize() causes the container to forget the
  // pointer.  Note that we tell the container NOT to manage the
  // memory itself.  This filter will properly manage the memory (as
  // opposed to the container) if the user wants it to.
  outputPtr->GetPixelContainer()->SetImportPointer(m_ImportPointer,
                                                   m_Size, false);
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::SetSpacing(const SpacingType& spacing)
{
  double dspacing[TOutputImage::ImageDimension];
  for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    dspacing[i] = spacing[i];
    }
  this->SetSpacing(dspacing);
}

/**
 *
 */
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::SetOrigin(const OriginType& origin)
{
  double dorigin[TOutputImage::ImageDimension];
  for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    dorigin[i] = origin[i];
    }
  this->SetOrigin(dorigin);
}

//----------------------------------------------------------------------------
template <class TOutputImage>
void
ImportImageFilter<TOutputImage>
::SetDirection(const DirectionType direction)
{
  bool modified = false;
  for (unsigned int r = 0; r < TOutputImage::ImageDimension; ++r)
    {
    for (unsigned int c = 0; c < TOutputImage::ImageDimension; ++c)
      {
      if (m_Direction[r][c] != direction[r][c])
        {
        m_Direction[r][c] = direction[r][c];
        modified = true;
        }
      }
    }
  if (modified)
    {
    this->Modified();
    }
}

} // end namespace otb

#endif
