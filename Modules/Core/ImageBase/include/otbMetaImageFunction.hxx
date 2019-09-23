/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMetaImageFunction_hxx
#define otbMetaImageFunction_hxx

#include "otbMetaImageFunction.h"
#include "otbImageFunctionAdaptor.h"

#include <algorithm>

namespace otb
{
template <class TOutputPrecision, class TCoordRep>
MetaImageFunction<TOutputPrecision, TCoordRep>::MetaImageFunction() : m_FunctionContainer()
{
}

template <class TOutputPrecision, class TCoordRep>
MetaImageFunction<TOutputPrecision, TCoordRep>::~MetaImageFunction()
{
  this->ClearFunctions();
}

template <class TOutputPrecision, class TCoordRep>
void MetaImageFunction<TOutputPrecision, TCoordRep>::AddFunction(FunctionType* function)
{
  m_FunctionContainer.push_back(function);
}

// template <class TOutputPrecision, class TCoordRep, typename T1, typename T2>
// void
// MetaImageFunction<TOutputPrecision, TCoordRep>
//::AddFunction(itk::ImageFunction<T1, T2, TCoordRep> * function)
// {
//  // Define the adapter
//  typedef itk::ImageFunction<T1, T2, TCoordRep> ImageFunctionType;
//  typedef otb::ImageFunctionAdapter<T1, ImageFunctionType, TCoordRep > AdapterType;
//
//  typename AdapterType::Pointer adapter = AdapterType::New();
//  adapte
//
// }

template <class TOutputPrecision, class TCoordRep>
void MetaImageFunction<TOutputPrecision, TCoordRep>::ClearFunctions()
{
  m_FunctionContainer.clear();
}

template <class TOutputPrecision, class TCoordRep>
unsigned int MetaImageFunction<TOutputPrecision, TCoordRep>::GetNumberOfFunctions() const
{
  return m_FunctionContainer.size();
}

template <class TOutputPrecision, class TCoordRep>
typename MetaImageFunction<TOutputPrecision, TCoordRep>::FunctionType* MetaImageFunction<TOutputPrecision, TCoordRep>::GetNthFunction(unsigned int index)
{
  return m_FunctionContainer.at(index);
}

template <class TOutputPrecision, class TCoordRep>
void MetaImageFunction<TOutputPrecision, TCoordRep>::RemoveNthFunction(unsigned int index)
{
  typename FunctionContainerType::iterator fIt = m_FunctionContainer.begin() + index;
  m_FunctionContainer.erase(fIt);
}

template <class TOutputPrecision, class TCoordRep>
typename MetaImageFunction<TOutputPrecision, TCoordRep>::OutputType MetaImageFunction<TOutputPrecision, TCoordRep>::Evaluate(const PointType& point) const
{
  // Build output
  OutputType resp;

  // For each function
  typename FunctionContainerType::const_iterator fIt = m_FunctionContainer.begin();
  while (fIt != m_FunctionContainer.end())
  {
    // Store current size
    unsigned int currentSize = static_cast<unsigned int>(resp.GetSize());

    // Call current function evaluation
    OutputType currentVector = (*fIt)->Evaluate(point);

    // Compute current vector size
    unsigned int currentVectorSize = static_cast<unsigned int>(currentVector.GetSize());

    // Enlarge the output vector
    resp.SetSize(currentSize + currentVectorSize, false);

    // Fill the output
    for (unsigned int i = 0; i < currentVectorSize; ++i)
    {
      resp.SetElement(currentSize + i, static_cast<ValueType>(currentVector[i]));
    }

    // Go to next function
    ++fIt;
  }

  return resp;
}


template <class TOutputPrecision, class TCoordRep>
void MetaImageFunction<TOutputPrecision, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of image functions: " << this->GetNumberOfFunctions() << std::endl;
}

} // end namespace otb

#endif
