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
#ifndef otbSharkUtils_h
#define otbSharkUtils_h

#include "otb_shark.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif
#include <shark/Data/Dataset.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{
namespace Shark
{
template <class T> void ListSampleRangeToSharkVector(const T * listSample, std::vector<shark::RealVector> & output, unsigned int start, unsigned int size)
{
  assert(listSample != ITK_NULLPTR);

  if(start+size>listSample->Size())
    {
    itkGenericExceptionMacro(<<"Requested range ["<<start<<", "<<start+size<<"[ is out of bound for input list sample (range [0, "<<listSample->Size()<<"[");
    }
  
  output.clear();
      
  // Sample index
  unsigned int sampleIdx = start;
    
  //Check for valid listSample
  if(listSample->Size()>0)
    {
    // Retrieve samples size alike
    const unsigned int sampleSize = listSample->GetMeasurementVectorSize();

    // Fill the output vector

    for (auto const endOfRange = start+size ; sampleIdx < endOfRange ; ++sampleIdx)
      {
      // Retrieve sample
      typename T::MeasurementVectorType const & sample = listSample->GetMeasurementVector(sampleIdx);
	   
      // // Define a shark::RealVector
      // shark::RealVector rv(sampleSize);
      // // Loop on sample size
      // for(unsigned int i = 0; i < sampleSize; ++i)
      //   {
      //   rv[i] = sample[i];
      //   }
      // using std::move;
      // output.emplace_back(move(rv));

      output.emplace_back(&sample[0], &sample[0]+sampleSize);
      }
    }
}

template <class T> void ListSampleRangeToSharkVector(const T * listSample, std::vector<unsigned int> & output, unsigned int start, unsigned int size)
{
  assert(listSample != ITK_NULLPTR);

  if(start+size>listSample->Size())
    {
    itkGenericExceptionMacro(<<"Requested range ["<<start<<", "<<start+size<<"[ is out of bound for input list sample (range [0, "<<listSample->Size()<<"[");
    }

  output.clear();
  
  // Sample index
  unsigned int sampleIdx = start;
    
  //Check for valid listSample
  if(listSample->Size()>0)
    {
    // Fill the output vector
    while(sampleIdx<start+size)
      {
      // Retrieve sample
      typename T::MeasurementVectorType const & sample = listSample->GetMeasurementVector(sampleIdx);
	   
      // Define a shark::RealVector
      output.push_back(sample[0]);
      ++sampleIdx;
      }
    } 
}

template <class T> void ListSampleToSharkVector(const T * listSample, std::vector<shark::RealVector> & output)
{
  assert(listSample != ITK_NULLPTR);
  ListSampleRangeToSharkVector(listSample,output,0U,static_cast<unsigned int>(listSample->Size()));
}

template <class T> void ListSampleToSharkVector(const T * listSample, std::vector<unsigned int> & output)
{
  assert(listSample != ITK_NULLPTR);
  ListSampleRangeToSharkVector(listSample,output,0, static_cast<unsigned int>(listSample->Size()));
}
  
}
}

#endif

