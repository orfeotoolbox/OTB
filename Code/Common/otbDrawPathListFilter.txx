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

#ifndef __otbDrawPathListFilter_txx
#define __otbDrawPathListFilter_txx

#include "otbDrawPathListFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkLineIterator.h"
#include "itkMetaDataObject.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TInputPath,class TOutputImage>
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::DrawPathListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  m_PathValue = static_cast<OutputImagePixelType>(255.0); 
  m_UseInternalPathValue = false;
}


template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::SetInputPath( InputPathListType *path)
{
  this->ProcessObjectType::SetNthInput(1,const_cast<InputPathListType *>(path));
}


template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::SetInputPath( InputPathType * path)
{
  InputPathListPointerType list = InputPathListType::New();
  list->PushBack(path);
  this->ProcessObjectType::SetNthInput(1,static_cast<InputPathListType *>(list));
}

template <class TInputImage, class TInputPath,class TOutputImage>
const typename DrawPathListFilter<TInputImage,TInputPath,TOutputImage>::InputPathListType *
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::GetInputPath(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const InputPathListType *>(this->ProcessObjectType::GetInput(1));
}
/**
 * Main computation method
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::GenerateData(void)
{
  OutputImagePointerType outputPtr = this->GetOutput();
  InputImageConstPointerType inputPtr = this->GetInput();
  InputPathListConstPointerType pathListPtr = this->GetInputPath();
  outputPtr->SetRegions(inputPtr->GetLargestPossibleRegion());
  outputPtr->Allocate();
  outputPtr->FillBuffer(0);

  // First, we copy input to output
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef typename InputPathListType::ConstIterator PathListIteratorType;
  typedef itk::LineIterator<OutputImageType> LineIteratorType;
  typedef typename InputPathType::VertexListType VertexListType;
  typedef typename VertexListType::ConstIterator VertexIteratorType;

  OutputIteratorType outIt(outputPtr,outputPtr->GetLargestPossibleRegion());
  InputIteratorType inIt(inputPtr,inputPtr->GetLargestPossibleRegion());

  for(outIt.GoToBegin(),inIt.GoToBegin();
      (!outIt.IsAtEnd() && !inIt.IsAtEnd());
      ++outIt,++inIt)
    {
      outIt.Set(static_cast<OutputImagePixelType>(inIt.Get()));
    }
  
  // Then we use itk::LineIterator to draw polylines
  OutputImageIndexType source, target;
  VertexIteratorType vertexIt;
  OutputImagePixelType value;
  for(PathListIteratorType plIt = pathListPtr->Begin(); plIt!=pathListPtr->End();++plIt)
    {
      if(m_UseInternalPathValue && plIt.Get()->GetMetaDataDictionary().HasKey("Value"))
	{
	  itk::ExposeMetaData<OutputImagePixelType>(plIt.Get()->GetMetaDataDictionary(),"Value",value);
	}
      else
	{
	  value = static_cast<OutputImagePixelType>(m_PathValue);
	}
       vertexIt = plIt.Get()->GetVertexList()->Begin();
       source[0] = static_cast<unsigned int>( (vertexIt.Value())[0]);
       source[1] = static_cast<unsigned int>( (vertexIt.Value())[1]);
      ++vertexIt;
      while(vertexIt!=plIt.Get()->GetVertexList()->End())
	{
	  target[0]=static_cast<unsigned int>((vertexIt.Value())[0]);
	  target[1]=static_cast<unsigned int>((vertexIt.Value())[1]);
	  LineIteratorType lineIt(outputPtr,source,target);
	  lineIt.GoToBegin();
	  while(!lineIt.IsAtEnd())
	    {
	      lineIt.Set(value);
	      ++lineIt;
	    }
	  source=target;
	  ++vertexIt;
	}
    }
}
/**
 * Printself method
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
DrawPathListFilter<TInputImage,TInputPath,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
 
