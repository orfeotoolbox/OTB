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
#ifndef __otbImageToVectorImageCastFilter_txx
#define __otbImageToVectorImageCastFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "otbImageToVectorImageCastFilter.h"

namespace otb
{

  template <class TInputImage, class TOutputVectorImage>
  void
  ImageToVectorImageCastFilter<TInputImage,TOutputVectorImage>
  ::GenerateData(void)
  {
    const InputImageType * inputPtr = this->GetInput();
    OutputVectorImageType * outputPtr = this->GetOutput();
    typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
    typedef itk::ImageRegionIterator<OutputVectorImageType> OutputIteratorType;
    
    typedef typename OutputVectorImageType::PixelType VectorPixelType;
    typedef typename VectorPixelType::ValueType OutputPrecisionType;

    InputIteratorType inputIt(inputPtr,inputPtr->GetRequestedRegion());
    OutputIteratorType outputIt(outputPtr,outputPtr->GetRequestedRegion());
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    
    while((!inputIt.IsAtEnd())||(!outputIt.IsAtEnd()))
      {
	VectorPixelType vector;
	vector.SetSize(1);
	vector[0] = static_cast<OutputPrecisionType>(inputIt.Get());
	outputIt.Set(vector);
	++inputIt;
	++outputIt;
      }
  }

  template <class TInputImage, class TOutputVectorImage>
  void
  ImageToVectorImageCastFilter<TInputImage,TOutputVectorImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

} // end namespace otb

#endif
