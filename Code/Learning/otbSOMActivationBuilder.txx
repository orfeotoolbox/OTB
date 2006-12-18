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
#ifndef _otbSOMActivationBuilder_txx
#define _otbSOMActivationBuilder_txx

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::SOMActivationBuilder()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNthOutput(0,OutputImageType::New());
}
/**
 * Destructor.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::~SOMActivationBuilder()
{}
/** Set the vector set image. 
 * \param image the vector set image.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void 
SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::SetVectorSet(InputImageType * image)
{
  this->itk::ProcessObject::SetNthInput(1,const_cast<InputImageType *>(image));
}
/**
 * Get the vector set image.
 * \return the vector set image.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
typename SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::InputImageType * 
SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::GetVectorSet(void)
{
  return static_cast<InputImageType *>(this->itk::ProcessObject::GetInput(1));
}
/** 
 * Main computation method 
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void 
SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::GenerateData(void)
{
  // Retrieve the inputs and output pointers
  InputImagePointerType vectorSet = this->GetVectorSet();
  InputMapType *  map = const_cast<InputMapType*>(this->GetInput());
  OutputImagePointerType output = this->GetOutput();
  
  // output initialisation
  output->SetRegions(map->GetLargestPossibleRegion());
  output->Allocate();
  output->FillBuffer(static_cast<typename OutputImageType::PixelType>(0));

  // Iterators typedefs
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typedef itk::ImageRegionConstIterator<InputImageType> VectorSetIteratorType;
  typename InputMapType::IndexType index;
  // Iterators instantiation
  OutputIteratorType outIt(output,output->GetLargestPossibleRegion());
  VectorSetIteratorType it(vectorSet,vectorSet->GetLargestPossibleRegion());

  // For each vector in the set
  for(it.GoToBegin();!it.IsAtEnd();++it)
    {
      // Retrieve the index of the winner
      index = map->GetWinner(it.Get());
      // increment the activation map
      outIt.SetIndex(index);
      outIt.Set(outIt.Get()+1);
    }
}
/** 
 *PrintSelf method 
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void 
SOMActivationBuilder<TInputImage,TInputMap,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
} // end namespace otb
#endif
