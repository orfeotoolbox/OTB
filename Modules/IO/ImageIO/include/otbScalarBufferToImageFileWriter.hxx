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

#ifndef otbScalarBufferToImageFileWriter_hxx
#define otbScalarBufferToImageFileWriter_hxx

#include "otbMacro.h"

#include "otbScalarBufferToImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
template<class TBufferType, class TOutputPixelType>
ScalarBufferToImageFileWriter<TBufferType, TOutputPixelType>
::ScalarBufferToImageFileWriter() : m_Buffer(nullptr), m_NumberOfChannels(0), m_InverseXSpacing(false)
{
  m_Writer = WriterType::New();
  m_ImageSize.Fill(0);
}


template<class TBufferType, class TOutputPixelType>
void
ScalarBufferToImageFileWriter<TBufferType, TOutputPixelType>::GenerateData()
{
  // Check image parameters
  if( (m_ImageSize[0]==0) || (m_ImageSize[0]==0) )
    {
      itkExceptionMacro("Invalid output image size, Size can't be null.");
    }

  if( m_NumberOfChannels==0 )
    {
      itkExceptionMacro("Invalid output image number of channels.");
    }

  RegionType lRegion;
  IndexType lId;
  lId.Fill(0);
  lRegion.SetIndex(lId);
  lRegion.SetSize(m_ImageSize);

  typename ImageType::Pointer lImage = ImageType::New();
  lImage->SetRegions( lRegion );
  lImage->SetNumberOfComponentsPerPixel(m_NumberOfChannels);
  lImage->Allocate();
  PixelType lPix;

  lPix.SetSize(m_NumberOfChannels);
  lPix.Fill( itk::NumericTraits<OutputPixelType>::Zero );
  lImage->FillBuffer(lPix);

  // 1 specific loop for each case to save time processing
  if(m_InverseXSpacing == false)
    {
      itk::ImageRegionIterator<ImageType> it(lImage, lRegion);
      it.GoToBegin();

      unsigned int cpt(0);
      while( it.IsAtEnd()==false )
        {
          for(unsigned int i=0; i<m_NumberOfChannels; ++i)
            {
              lPix[i] = static_cast<OutputPixelType>(m_Buffer[cpt]);
              ++cpt;
            }

          it.Set( lPix );
          ++it;
        }
    }
  else
    {
      itk::ImageRegionIteratorWithIndex<ImageType> it(lImage, lRegion);
      it.GoToBegin();
      // cpt is the first component of the last pixel
      unsigned int cpt(0);
      while( it.IsAtEnd()==false )
        {
          IndexType index = it.GetIndex();
          cpt = (m_ImageSize[1] - 1 - index[1]) * m_NumberOfChannels * m_ImageSize[0] + m_NumberOfChannels * index[0];

          for(unsigned int i=0; i<m_NumberOfChannels; ++i)
            {
              lPix[i] = static_cast<OutputPixelType>(m_Buffer[cpt+i]);
            }

          it.Set( lPix );
          ++it;
        }
    }

  m_Writer->SetInput( lImage );
  m_Writer->Update();
}

template<class TBufferType, class TOutputPixelType>
void
ScalarBufferToImageFileWriter<TBufferType, TOutputPixelType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "FileName" << m_Writer->GetFileName() << std::endl;
  os << indent << "Size" << m_ImageSize << std::endl;
  os << indent << "NumberOfChannels" << m_NumberOfChannels << std::endl;
}

} // end namespace otb

#endif
