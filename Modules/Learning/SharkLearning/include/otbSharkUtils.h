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
#include <shark/Data/Dataset.h>
namespace otb
{
namespace Shark
{
template <class T> void ListSampleToSharkVector(const T * listSample, std::vector<shark::RealVector> & output)
{
  assert(listSample != ITK_NULLPTR);
  // Sample index
  unsigned int sampleIdx = 0;
    
  //Check for valid listSample
  if(listSample->Size()>0)
    {
    // Retrieve samples count
    output.clear();
    // Build an iterator
    typename T::ConstIterator sampleIt = listSample->Begin();

    // Retrieve samples size alike
    const unsigned int sampleSize = listSample->GetMeasurementVectorSize();

    // Fill the output vector
    for(;sampleIt != listSample->End();++sampleIt,++sampleIdx)
      {
      // Retrieve sample
      typename T::MeasurementVectorType sample = sampleIt.GetMeasurementVector();
	   
      // Define a shark::RealVector
      shark::RealVector rv(sampleSize);
      // Loop on sample size
      for(unsigned int i = 0; i < sampleSize; ++i)
        {
        rv[i] = sample[i];
        }
      output.push_back(rv);
      }
    }
}

template <class T> void ListSampleToSharkVector(const T * listSample, std::vector<unsigned int> & output)
{
  assert(listSample != ITK_NULLPTR);
  // Sample index
  unsigned int sampleIdx = 0;
    
  //Check for valid listSample
  if(listSample->Size()>0)
    {
    output.clear();
    // Build an iterator
    typename T::ConstIterator sampleIt = listSample->Begin();

    // Fill the output vector
    for(;sampleIt != listSample->End();++sampleIt,++sampleIdx)
      {
      // Retrieve sample
      typename T::MeasurementVectorType sample = sampleIt.GetMeasurementVector();
      output.push_back(sample[0]);
      }
    }
}
}
}

#endif
