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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbMacro.h"

#include "otbScalarBufferToImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
template<class TBufferType, class TOutputPixelType>
ScalarBufferToImageFileWriter<TBufferType, TOutputPixelType>
::ScalarBufferToImageFileWriter() : m_Buffer(NULL), m_NumberOfChannels(0), m_InverseXSpacing(false)
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

	RegionType l_Region;
	IndexType l_Id;
	l_Id.Fill(0);
	l_Region.SetIndex(l_Id);
	l_Region.SetSize(m_ImageSize);

	typename ImageType::Pointer l_Image = ImageType::New();
	l_Image->SetRegions( l_Region );
	l_Image->SetNumberOfComponentsPerPixel(m_NumberOfChannels);
	l_Image->Allocate();
	PixelType l_Pix;
	
	l_Pix.SetSize(m_NumberOfChannels);
	l_Pix.Fill( itk::NumericTraits<OutputPixelType>::Zero );
	l_Image->FillBuffer(l_Pix);

	// 1 specific loop for each case to save time processing
	if(m_InverseXSpacing == false)
	{
	itk::ImageRegionIterator<ImageType> it(l_Image, l_Region);
	it.GoToBegin();

	unsigned int cpt(0);
		while( it.IsAtEnd()==false )
		{
			for(unsigned int i=0; i<m_NumberOfChannels; i++)
			{
				l_Pix[i] = static_cast<OutputPixelType>(m_Buffer[cpt]);
				cpt++;
			}
	
			it.Set( l_Pix );
			++it;
		}
	}		
	else
	{
		itk::ImageRegionIteratorWithIndex<ImageType> it(l_Image, l_Region);
		it.GoToBegin();
		// cpt is the first component of the last pixel 
		unsigned int cpt(0);
		while( it.IsAtEnd()==false )
		{
			IndexType index = it.GetIndex();
			cpt = (m_ImageSize[1] - 1 - index[1]) * m_NumberOfChannels * m_ImageSize[0] + m_NumberOfChannels * index[0];
			
			for(unsigned int i=0; i<m_NumberOfChannels; i++)
			{
				l_Pix[i] = static_cast<OutputPixelType>(m_Buffer[cpt+i]);
			}
			
			it.Set( l_Pix );
			++it;
		}
	}
	
	m_Writer->WriteGeomFileOff();
	m_Writer->SetInput( l_Image );
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

