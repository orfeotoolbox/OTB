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
#ifndef __otbGISTableToGISTableFilter_txx
#define __otbGISTableToGISTableFilter_txx

#include "otbGISTableToGISTableFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
   * Constructor
 */
template <class TInputGISTable, class TOutputGISTable>
GISTableToGISTableFilter<TInputGISTable, TOutputGISTable>
::GISTableToGISTableFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TInputGISTable, class TOutputGISTable>
void
GISTableToGISTableFilter<TInputGISTable, TOutputGISTable>
::SetInput(const InputGISTableType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputGISTableType *>(input));
}

template <class TInputGISTable, class TOutputGISTable>
const typename GISTableToGISTableFilter<TInputGISTable, TOutputGISTable>::InputGISTableType *
GISTableToGISTableFilter<TInputGISTable, TOutputGISTable>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputGISTable *>
           (this->itk::ProcessObject::GetInput(0));
}

/**
   * PrintSelf Method
 */
template <class TInputGISTable, class TOutputGISTable>
void
GISTableToGISTableFilter<TInputGISTable, TOutputGISTable>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
