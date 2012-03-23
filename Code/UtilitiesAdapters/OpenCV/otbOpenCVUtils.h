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
#ifndef __otbOpenCVUtils_h
#define __otbOpenCVUtils_h

#include "ml.h"

namespace otb
{
/** Converts a ListSample of VariableLengthVector to a CvMat. The user
 *  is responsible for freeing the output pointer with the
 *  cvReleaseMat function.  A null pointer is resturned in case the
 *  conversion failed.
 */
template <typename T> CvMat * VariableLengthVectorListSampleToCvMat(const T * listSample) {
  // Intialize to NULL
  CvMat * output = NULL;

  // Sample index
  unsigned int sampleIdx = 0;

  // Check for valid listSample
  if(listSample != NULL && listSample->Size() > 0)
    {
    // Retrieve samples count
    unsigned int sampleCount = listSample->Size();
    
    // Build an iterator
    typename T::ConstIterator sampleIt = listSample->Begin();

    // Retrieve samples size alike
    unsigned int sampleSize = sampleIt.GetMeasurementVector().Size();

    // Allocate CvMat
    output = cvCreateMat(sampleCount,sampleSize,CV_32FC1);

    // Fill the cv matrix
    for(;sampleIt!=listSample->End();++sampleIt,++sampleIdx)
      {
      // Retrieve sample
      typename T::MeasurementVectorType sample = sampleIt.GetMeasurementVector();

      // Loop on sample size
      for(unsigned int i = 0; i<sampleSize;++i)
        {
        cvSetReal2D(output,sampleIdx,i,sample[i]);
        }
      }
    }

  // return
  return output;
}

template <typename T> CvMat * VariableLengthVectorListSampleToCvMat(typename T::Pointer listSample) {
  return VariableLengthVectorListSampleToCvMat(listSample.GetPointer());
}

template <typename T> CvMat * VariableLengthVectorListSampleToCvMat(typename T::ConstPointer listSample) {
  return VariableLengthVectorListSampleToCvMat(listSample.GetPointer());
}

}

#endif
