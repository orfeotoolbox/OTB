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
#ifndef __otbSOMActivationBuilder_txx
#define __otbSOMActivationBuilder_txx

#include "itkImageRegionIterator.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>
::SOMActivationBuilder()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNthOutput(0, OutputImageType::New());
}
/**
 * Destructor.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>
::~SOMActivationBuilder()
{}
/**
 * Main computation method
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void
SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>
::GenerateData(void)
{
  // Retrieve the inputs and output pointers
  InputMapType *         map = const_cast<InputMapType*>(this->GetInput());
  OutputImagePointerType output = this->GetOutput();

  // output initialization
  output->SetRegions(map->GetLargestPossibleRegion());
  output->Allocate();
  output->FillBuffer(static_cast<typename OutputImageType::PixelType>(0));

  // Iterator typedef
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typename InputMapType::IndexType index;
  // Iterators instantiation
  OutputIteratorType outIt(output, output->GetLargestPossibleRegion());

  // For each vector in the set
  for (typename ListSampleType::Iterator it = m_ListSample->Begin();
       it != m_ListSample->End(); ++it)
    {
    // Retrieve the index of the winner
    index = map->GetWinner(it.GetMeasurementVector());
    // increment the activation map
    outIt.SetIndex(index);
    outIt.Set(outIt.Get() + 1);
    }
}
/**
 *PrintSelf method
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void
SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
