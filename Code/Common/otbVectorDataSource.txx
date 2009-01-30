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

#ifndef __otbVectorDataSource_txx
#define __otbVectorDataSource_txx

#include "otbVectorDataSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputVectorData>
VectorDataSource<TOutputVectorData>
::VectorDataSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0,OutputVectorDataType::New().GetPointer());
}

template <class TOutputVectorData>
VectorDataSource<TOutputVectorData>
::~VectorDataSource()
{
}

template <class TOutputVectorData>
void
VectorDataSource<TOutputVectorData>
::AllocateOutputs()
{
  OutputVectorDataPointer outputPtr;

  // Allocate the output memory
  for (unsigned int i=0; i < this->GetNumberOfOutputs(); i++)
  {
    outputPtr = this->GetOutput(i);
    outputPtr->Clear();
  }
}


/**
 * Get the output vector data
 * \return The vector data produced.
 */
template <class TOutputVectorData>
typename VectorDataSource<TOutputVectorData>::OutputVectorDataType *
VectorDataSource<TOutputVectorData>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs()<1)
  {
    return 0;
  }
  return static_cast<OutputVectorDataType *> (this->ProcessObject::GetOutput(0));
}

template <class TOutputVectorData>
typename VectorDataSource<TOutputVectorData>::OutputVectorDataType *
VectorDataSource<TOutputVectorData>
::GetOutput(unsigned int idx)
{
  return static_cast<OutputVectorDataType*>
         (this->Superclass::GetOutput(idx));
}

/**
 * PrintSelf Method
 */
template<class TOutputVectorData>
void
VectorDataSource<TOutputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}// End namespace otb

#endif
