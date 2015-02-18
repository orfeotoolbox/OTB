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
#ifndef __otbRCC8GraphSource_txx
#define __otbRCC8GraphSource_txx

#include "otbRCC8GraphSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputGraph>
RCC8GraphSource<TOutputGraph>
::RCC8GraphSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, TOutputGraph::New().GetPointer());
}
/**
 * Get the output Graph
 * \return The output graph produced.
 */
template <class TOutputGraph>
typename RCC8GraphSource<TOutputGraph>::OutputGraphType *
RCC8GraphSource<TOutputGraph>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputGraphType *> (this->ProcessObject::GetOutput(0));
}
/**
 * PrintSelf Method
 */
template<class TOutputGraph>
void
RCC8GraphSource<TOutputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
