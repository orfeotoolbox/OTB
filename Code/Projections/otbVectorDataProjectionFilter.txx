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
#ifndef __otbVectorDataProjectionFilter_txx
#define __otbVectorDataProjectionFilter_txx

#include "otbVectorDataProjectionFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
   * Constructor
 */
  template <class TInputVectorData, class TOutputVectorData >
      VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::VectorDataProjectionFilter()
  {
  }



/**
   * GenerateData Performs the pixel-wise addition
 */
  template <class TInputVectorData, class TOutputVectorData >
      void
          VectorDataProjectionFilter<TInputVectorData,TOutputVectorData>
  ::GenerateData(void)
  {
    this->AllocateOutputs();

    InputVectorDataPointer inputPtr = this->GetInput();
    OutputVectorDataPointer outputPtr = this->GetOutput();

    itk::ProgressReporter progress(this, 0, inputPtr->Size());



  }

} // end namespace otb

#endif
