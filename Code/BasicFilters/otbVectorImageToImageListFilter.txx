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
/** Generate the input requested region from the first element in the list. */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType,TImageList>
::GenerateOutputInformation(void)
{
  OutputImageListPointerType outputPtr = this->GetOutput();
  InputVectorImagePointerType inputPtr = this->GetInput();

  if(inputPtr)
    {
      for(unsigned int i=0;i<inputPtr->GetNumberOfComponentsPerPixel();++i)
	{
	 typename  OutputImageType::Pointer tmpImagePtr = OutputImageType::New();
	 tmpImagePtr->CopyInformation(inputPtr);
	 tmpImagePtr->SetRegions(inputPtr->GetLargestPossibleRegion());
	 outputPtr->PushBack(tmpImagePtr);   
	}
    }
}
/** Generate the output information by building the output list. */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType,TImageList>
::GenerateInputRequestedRegion(void)
{
  OutputImageListPointerType outputPtr = this->GetOutput();
  InputVectorImagePointerType inputPtr = this->GetInput();
  
  if(inputPtr)
    {
      if(outputPtr->Size()>0)
	{
	  inputPtr->SetRequestedRegion(outputPtr->GetNthElement(0)->GetRequestedRegion());
	}
    }
}
/**
 * Main computation method
 */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType,TImageList>
::GenerateData(void)
{
  OutputImageListPointerType outputPtr = this->GetOutput();
  InputVectorImagePointerType inputPtr = this->GetInput();

  typedef itk::ImageRegionConstIterator<InputVectorImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;

  InputIteratorType inputIt(inputPtr,inputPtr->GetRequestedRegion());
  
  std::vector<OutputIteratorType> outputIteratorList;
  
 typename OutputImageListType::ConstIterator outputListIt = outputPtr->Begin();
  for(;outputListIt!=outputPtr->End();++outputListIt)
    {
      outputListIt.Get()->Allocate();
      outputIteratorList.push_back(OutputIteratorType(outputListIt.Get(),outputListIt.Get()->GetRequestedRegion()));
      outputIteratorList.back().GoToBegin();
    }

  inputIt.GoToBegin();
  while(!inputIt.IsAtEnd())
    {
      unsigned int counter = 0;
      for(typename std::vector<OutputIteratorType>::iterator it = outputIteratorList.begin();
	  it!=outputIteratorList.end();++it)
	{
	  if(!(*it).IsAtEnd())
	    {
	      (*it).Set(static_cast<typename OutputImageType::PixelType>(inputIt.Get()[counter]));
	      ++(*it);
	      ++counter;
	    }
	  else
	    {
	      itkGenericExceptionMacro("End of image !");
	    }
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
