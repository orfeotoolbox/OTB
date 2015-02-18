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
#ifndef __otbPointSetToPointSetFilter_txx
#define __otbPointSetToPointSetFilter_txx

#include "otbPointSetToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::PointSetToPointSetFilter()
{
  this->SetNumberOfRequiredInputs(1);

}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::SetInput(unsigned int idx, const InputPointSetType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(idx,
                                       const_cast<InputPointSetType *>(input));
  this->GetInput()->ReleaseDataFlagOff();
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::SetInput(const InputPointSetType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(0,
                                       const_cast<InputPointSetType *>(input));
  this->GetInput()->ReleaseDataFlagOff();
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
typename PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>::InputPointSetType *
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::GetInput(unsigned int idx)
{

  return static_cast<InputPointSetType*>
           (this->ProcessObjectType::GetInput(idx));
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
typename PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>::InputPointSetType *
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1) return 0;

  return static_cast<InputPointSetType*>
           (this->ProcessObjectType::GetInput(0));
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
