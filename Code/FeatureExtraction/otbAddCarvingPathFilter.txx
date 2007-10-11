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

#ifndef __otbAddCarvingPathFilter_txx
#define __otbAddCarvingPathFilter_txx

#include "otbAddCarvingPathFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TInputPath,class TOutputImage>
AddCarvingPathFilter<TInputImage,TInputPath,TOutputImage>
::AddCarvingPathFilter()
{
  m_Value = static_cast<OutputImagePixelType>(255.0);
  m_Direction = 0;
}
/**
 * Main computation method.
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
AddCarvingPathFilter<TInputImage,TInputPath,TOutputImage>
::GenerateData(void)
{  
  
  const InputImageType * inputImage = this->GetInput();
  OutputImageType * outputImage       = this->GetOutput();
  InputPathType * inputPath = static_cast<const InputPathType *>
      (this->itk::ProcessObject::GetInput(1));
  
  outputImage->SetBufferedRegion( outputImage->GetRequestedRegion() );
  outputImage->Allocate();
  
  
  InputIteratorType inputIterator(inputImage, 
                                  inputImage->GetLargestPossibleRegion());
  OutputIteratorType outputIterator(outputImage, 
                                  outputImage->GetLargestPossibleRegion());
  
  unsigned int dir0;
  unsigned int dir1;
  
  if (m_Direction == 0)
  {
    dir0=0;
    dir1=1;
  }
  else
  {
    dir0=1;
    dir1=0;
  }
  
  inputIterator.SetFirstDirection( dir0 );
  inputIterator.SetSecondDirection( dir1 );
  inputIterator.GoToBegin();
  outputIterator.SetFirstDirection( dir0 );
  outputIterator.SetSecondDirection( dir1 );
  outputIterator.GoToBegin();
  
  int line=-1;
  typename InputImageType::IndexType index;
  typename InputPathType::VertexListType::ConstPointer vertexList = inputPath->GetVertexList();
  typename InputPathType::VertexListType::ConstIterator pathIterator;
  typename InputImageType::IndexType indexToAdd;
  
  typename InputImageType::SizeType inputSize;
  inputSize = inputImage->GetLargestPossibleRegion().GetSize();
  
  //go to the end of the path
  pathIterator = vertexList->Begin();
  while(pathIterator != vertexList->End())
  {
    ++pathIterator;
  }
  --pathIterator;
  
  while (!outputIterator.IsAtEnd())
  {
    while (!outputIterator.IsAtEndOfSlice())
    {
      bool flag = false;
      while(!outputIterator.IsAtEndOfLine())
      {
        index = outputIterator.GetIndex();
        
        if(index[dir1] != line)
        {
          line = index[dir1];
          typename InputImageType::PointType tmpIndex;
          inputImage->TransformContinuousIndexToPhysicalPoint(pathIterator.Value(),tmpIndex);
          inputImage->TransformPhysicalPointToIndex(tmpIndex,indexToAdd);
          --pathIterator;
          if(index[dir1] != indexToAdd[dir1])
          {
            std::cout << "Error : " << index[dir1] << " , " << indexToAdd[dir1] << std::endl;
          }
        } 
        
        if (flag || ((index[dir0] != indexToAdd[dir0]) && (index[dir0] < inputSize[dir0])))
        {
          outputIterator.Set(inputIterator.Get());
          ++inputIterator;
        }
        else
        {
//           std::cout << "Adding the value at " << outputIterator.GetIndex() << std::endl;
          flag = true;
//           std::cout << "[" << index[0] << "," << index[1] << "] ";
          OutputImagePixelType newValue;
          newValue = (--inputIterator).Get();
          newValue += (++inputIterator).Get();
          newValue /= 2;
//           newValue = 0; //TODO just for test
          outputIterator.Set(newValue);
        }
        ++outputIterator;
        
      }
      
      if((outputIterator.GetIndex())[dir0] != (inputIterator.GetIndex())[dir0]+1)
      {
        std::cout << "Error 2: "<< (outputIterator.GetIndex())[dir0] <<  " , " << (inputIterator.GetIndex())[dir0] << std::endl;
      }
      inputIterator.NextLine();
      outputIterator.NextLine();
    }
    inputIterator.NextSlice();
    outputIterator.NextSlice();
  }
  
  
}


/**
 * Printself Method
 */
template <class TInputImage, class TInputPath,class TOutputImage>
void
AddCarvingPathFilter<TInputImage,TInputPath,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_Value << std::endl;
}



template <class TInputImage, class TInputPath,class TOutputImage>
    void 
        AddCarvingPathFilter<TInputImage,TInputPath,TOutputImage>
  ::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
  unsigned int dir0;
  unsigned int dir1;
  
  if (m_Direction == 0)
  {
    dir0=0;
    dir1=1;
  }
  else
  {
    dir0=1;
    dir1=0;
  }
  
  // we need to compute the output spacing, the output image size, and the
  // output image start index
  const typename TInputImage::SpacingType&
      inputSpacing = inputPtr->GetSpacing();
  const typename TInputImage::SizeType&   inputSize
      = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TInputImage::IndexType&  inputStartIndex
      = inputPtr->GetLargestPossibleRegion().GetIndex();

  typename TOutputImage::SpacingType      outputSpacing;
  typename TOutputImage::SizeType         outputSize;
  typename TOutputImage::IndexType        outputStartIndex;
  
  for (unsigned int i = 0; i < TOutputImage::ImageDimension; i++)
  {
    outputSpacing[i] = inputSpacing[i];
    outputSize[i] = (unsigned int) (inputSize[i]);
    outputStartIndex[i] = inputStartIndex[i];
  }
  // we remove one column of the image
  outputSize[dir0] = inputSize[dir0] + 1;
  

  outputPtr->SetSpacing( outputSpacing );

  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );

  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

template <class TInputImage, class TInputPath,class TOutputImage>
    void 
        AddCarvingPathFilter<TInputImage,TInputPath,TOutputImage>
  ::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  // get pointers to the input and output
  InputImagePointer  inputPtr = 
  const_cast< InputImageType * >( this->GetInput() );
  if ( !inputPtr )
  {
    itkExceptionMacro( << "Input has not been set." );
  }
  
  inputPtr->SetRequestedRegion( inputPtr->GetLargestPossibleRegion() );
  
}
  
} // end namespace otb

#endif
 
