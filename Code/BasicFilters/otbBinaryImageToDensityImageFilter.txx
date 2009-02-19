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
#ifndef __otbBinaryImageToDensityImageFilter_txx
#define __otbBinaryImageToDensityImageFilter_txx

#include "otbBinaryImageToDensityImageFilter.h"
#include "itkImageRegionIterator.h"
#include "otbMacro.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TOutputImage, class TCountFunction>
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::BinaryImageToDensityImageFilter()
{
  m_NeighborhoodRadius = 1;

  m_CountFunction = CountFunctionType::New();

}
/** Destructor */
template <class TInputImage, class TOutputImage, class TCountFunction>
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::~BinaryImageToDensityImageFilter()
{}


/** Main computation method */
template <class TInputImage, class TOutputImage, class TCountFunction>
void
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::ThreadedGenerateData( const InputImageRegionType &outputRegionForThread, int threadId )
{
  
  InputImagePointerType    input = const_cast<InputImageType * > (this->GetInput());
  OutputImagePointerType   output = this->GetOutput();

  m_CountFunction->SetInputImage(input);

  itk::ImageRegionIterator<InputImageType>   it(input,outputRegionForThread );
  itk::ImageRegionIterator<OutputImageType>   itOut(output,outputRegionForThread );
  
  it.GoToBegin();
  itOut.GoToBegin();
  
  while(!it.IsAtEnd())
    {
      m_CountFunction->SetNeighborhoodRadius(m_NeighborhoodRadius);
      typename InputImageType::IndexType index = it.GetIndex();
      
      if(outputRegionForThread.IsInside(index))
	{
	  itOut.Set(m_CountFunction->EvaluateAtIndex(index));
	}
      
      ++itOut;
      ++it;
    }
}

/** PrintSelf method */
template <class TInputImage, class TOutputImage, class TCountFunction>
void
BinaryImageToDensityImageFilter<TInputImage, TOutputImage, TCountFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Neighborhood Radius : " << m_NeighborhoodRadius
  << std::endl;
}
} // End namespace otb
#endif
