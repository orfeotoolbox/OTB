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

#ifndef __otbGISTableSource_txx
#define __otbGISTableSource_txx

#include "otbGISTableSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputGISTable>
GISTableSource<TOutputGISTable>
::GISTableSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, OutputGISTableType::New().GetPointer());
}

template <class TOutputGISTable>
GISTableSource<TOutputGISTable>
::~GISTableSource()
{
}

template <class TOutputGISTable>
void
GISTableSource<TOutputGISTable>
::AllocateOutputs()
{
  OutputGISTablePointer outputPtr;

  // Allocate the output memory
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
    {
    outputPtr = this->GetOutput(i);
    outputPtr->Clear();
    }
}

/**
 * Get the output vector data
 * \return The vector data produced.
 */
template <class TOutputGISTable>
typename GISTableSource<TOutputGISTable>::OutputGISTableType *
GISTableSource<TOutputGISTable>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputGISTableType *> (this->ProcessObject::GetOutput(0));
}

template <class TOutputGISTable>
typename GISTableSource<TOutputGISTable>::OutputGISTableType *
GISTableSource<TOutputGISTable>
::GetOutput(unsigned int idx)
{
  return static_cast<OutputGISTableType*>
           (this->Superclass::GetOutput(idx));
}

/**
 * PrintSelf Method
 */
template<class TOutputGISTable>
void
GISTableSource<TOutputGISTable>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
