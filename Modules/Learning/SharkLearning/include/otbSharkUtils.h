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
#ifndef __SharkUtils_h
#define __SharkUtils_h

//#include <shark/Algorithms/Trainers/RFTrainer.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <shark/Data/Dataset.h>
#pragma GCC diagnostic pop
#else
#include <shark/Data/Dataset.h>
#endif

namespace otb
{
namespace Shark
{
template <class T> void ListSampleRangeToSharkVector(const T * listSample, std::vector<shark::RealVector> & output, const unsigned int & start, const unsigned int& size)
{
  assert(listSample != ITK_NULLPTR);
  assert(start+size<=listSample->Size());

  // Sample index
  unsigned int sampleIdx = start;
    
  //Check for valid listSample
  if(listSample->Size()>0)
    {
    // Retrieve samples count
    output.clear();


    // Retrieve samples size alike
    const unsigned int sampleSize = listSample->GetMeasurementVectorSize();

    // Fill the output vector
    while(sampleIdx<start+size)
      {
      // Retrieve sample
      typename T::MeasurementVectorType sample = listSample->GetMeasurementVector(sampleIdx);
	   
      // Define a shark::RealVector
      shark::RealVector rv(sampleSize);
      // Loop on sample size
      for(unsigned int i = 0; i < sampleSize; ++i)
        {
        rv[i] = sample[i];
        }
      output.push_back(rv);
      ++sampleIdx;
      }
    }
}

template <class T> void ListSampleRangeToSharkVector(const T * listSample, std::vector<unsigned int> & output, const unsigned int & start, const unsigned int & size)
{
  assert(listSample != ITK_NULLPTR);
  assert(start+size<=listSample->Size());

  // Sample index
  unsigned int sampleIdx = start;
    
  //Check for valid listSample
  if(listSample->Size()>0)
    {
    // Retrieve samples count
    output.clear();

    // Fill the output vector
    while(sampleIdx<start+size)
      {
      // Retrieve sample
      typename T::MeasurementVectorType sample = listSample->GetMeasurementVector(sampleIdx);
	   
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
