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
#ifndef __otbUnaryFunctorObjectListFilter_txx
#define __otbUnaryFunctorObjectListFilter_txx

#include "otbUnaryFunctorObjectListFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputList, class TOutputList, class TFunction  >
UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
::UnaryFunctorObjectListFilter()
{
}



/**
 * GenerateData Performs the pixel-wise addition
 */
template <class TInputList, class TOutputList, class TFunction  >
void
UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
::GenerateData(void)
{
  this->AllocateOutputs();
  
  InputListPointer inputPtr = this->GetInput();
  OutputListPointer outputPtr = this->GetOutput();

  itk::ProgressReporter progress(this, 0, inputPtr->Size());
  
  // Define the iterators
  for(InputListIterator it = inputPtr->Begin(); it != inputPtr->End(); ++it)
  {
    outputPtr->PushBack(m_Functor(it.Get()));

    progress.CompletedPixel(); 

  }
  
}

} // end namespace otb

#endif
