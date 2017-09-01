/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStreamingImageRAMWriter_txx
#define otbStreamingImageRAMWriter_txx
#include "otbStreamingImageRAMWriter.h"

#include "itkOutputDataObjectIterator.h"


namespace otb
{

template< typename TInputImage >
StreamingImageRAMWriter < TInputImage >
::StreamingImageRAMWriter()
{
}

template< typename TInputImage >
void StreamingImageRAMWriter < TInputImage >
::AllocateOutputs()
{
  typedef itk::ImageBase< 2 > ImageBaseType;
  typename ImageBaseType::Pointer outputPtr;

  // Allocate the output memory
  for ( itk::OutputDataObjectIterator it(this); !it.IsAtEnd(); it++ )
    {
    // Check whether the output is an image of the appropriate
    // dimension (use ProcessObject's version of the GetInput()
    // method since it returns the input as a pointer to a
    // DataObject as opposed to the subclass version which
    // static_casts the input to an TInputImage).
    outputPtr = dynamic_cast< ImageBaseType * >( it.GetOutput() );

    if ( outputPtr )
      {
      // outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
      // outputPtr->Allocate();
      }
    }
}

template< typename TInputImage >
void StreamingImageRAMWriter < TInputImage >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  typename TInputImage::ConstPointer input = this->GetInput();
  typename TInputImage::Pointer output = this->GetOutput();

  if ( !input || !output )
    {
    return;
    }

  // typename TInputImage::IndexType start;
  // typename TInputImage::SizeType size;
  // start[0] = 0;
  // start[1] = 0;
  // size[0] = input->GetLargestPossibleRegion().GetSize()[0];
  // size[1] = input->GetLargestPossibleRegion().GetSize()[1];
  // typename TInputImage::RegionType region;
  // region.SetSize( size );
  // region.SetIndex( start );
  output->SetLargestPossibleRegion( input->GetLargestPossibleRegion() );
  output->SetRequestedRegion( input->GetRequestedRegion() );
  output->SetBufferedRegion( input->GetLargestPossibleRegion() );
  output->Allocate();

}

template< typename TInputImage >
void StreamingImageRAMWriter < TInputImage >
::ThreadedGenerateData(const InputImageRegionType & outputRegionForThread,
                       ThreadIdType itkNotUsed( threadId ) )
{
  typename TInputImage::ConstPointer input = this->GetInput();
  typename TInputImage::Pointer output = this->GetOutput();

  typename TInputImage::RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread , outputRegionForThread);

  itk::ImageRegionConstIterator < TInputImage > it ( input , 
                                      inputRegionForThread );
  itk::ImageRegionIterator < TInputImage > oit ( output , 
                                      outputRegionForThread );
  it.GoToBegin();
  oit.GoToBegin();
  while( !it.IsAtEnd() )
    {
    oit.Set( it.Get() );
    ++oit;
    ++it;
    }
}


} // end namespace otb

#endif