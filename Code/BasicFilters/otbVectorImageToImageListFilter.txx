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
#ifndef _otbVectorImageToImageListFilter_txx
#define _otbVectorImageToImageListFilter_txx

#include "otbVectorImageToImageListFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include <vector>
#include "otbMacro.h"

namespace otb
{
/**
 * Main computation method
 */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType,TImageList>
::GenerateData(void)
{

  OutputImageListPointerType outputPtr = this->GetOutput();
  InputVectorImageConstPointerType inputPtr = this->GetInput();

  typedef itk::ImageRegionConstIterator<InputVectorImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;

  InputIteratorType inputIt(inputPtr,inputPtr->GetRequestedRegion());
  
  std::vector<OutputIteratorType> outputIteratorList;
  
  for(unsigned int i = 0;i<inputPtr->GetNumberOfComponentsPerPixel();++i)
    {
      typename OutputImageType::Pointer tmpImagePtr = OutputImageType::New();
      tmpImagePtr->SetRegions(inputPtr->GetRequestedRegion());
      tmpImagePtr->Allocate();
      outputPtr->PushBack(tmpImagePtr);
      outputIteratorList.push_back(OutputIteratorType(outputPtr->Back(),inputPtr->GetRequestedRegion()));
      outputIteratorList.back().GoToBegin();
    }

  inputIt.GoToBegin();
  while(!inputIt.IsAtEnd())
    {
      unsigned int counter = 0;
      for(typename std::vector<OutputIteratorType>::iterator it = outputIteratorList.begin();
	  it!=outputIteratorList.end();++it)
	{
	  (*it).Set(static_cast<typename OutputImageType::PixelType>(inputIt.Get()[counter]));
	  ++(*it);
	  ++counter;
	}
      ++inputIt;
    }
}
/**
 * PrintSelf Method
 */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType,TImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
