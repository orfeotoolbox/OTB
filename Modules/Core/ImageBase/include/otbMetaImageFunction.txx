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
#ifndef __otbMetaImageFunction_txx
#define __otbMetaImageFunction_txx

#include "otbMetaImageFunction.h"
#include "otbImageFunctionAdaptor.h"

#include <algorithm>

namespace otb
{
template <class TOutputPrecision, class TCoordRep>
MetaImageFunction<TOutputPrecision, TCoordRep>
::MetaImageFunction() : m_FunctionContainer()
 {

 }

template <class TOutputPrecision, class TCoordRep>
MetaImageFunction<TOutputPrecision, TCoordRep>
::~MetaImageFunction()
 {
  this->ClearFunctions();
 }

template <class TOutputPrecision, class TCoordRep>
void
MetaImageFunction<TOutputPrecision, TCoordRep>
::AddFunction(FunctionType * function)
 {
  m_FunctionContainer.push_back(function);
 }

//template <class TOutputPrecision, class TCoordRep, typename T1, typename T2>
//void
//MetaImageFunction<TOutputPrecision, TCoordRep>
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
void
MetaImageFunction<TOutputPrecision, TCoordRep>
::ClearFunctions()
 {
  m_FunctionContainer.clear();
 }

template <class TOutputPrecision, class TCoordRep>
unsigned int
MetaImageFunction<TOutputPrecision, TCoordRep>
::GetNumberOfFunctions() const
 {
  return m_FunctionContainer.size();
 }

template <class TOutputPrecision, class TCoordRep>
typename MetaImageFunction<TOutputPrecision, TCoordRep>
::FunctionType *
MetaImageFunction<TOutputPrecision, TCoordRep>
::GetNthFunction(unsigned int index)
 {
  return m_FunctionContainer.at(index);
 }

 template <class TOutputPrecision, class TCoordRep>
 void
 MetaImageFunction<TOutputPrecision, TCoordRep>
 ::RemoveNthFunction(unsigned int index)
  {
   typename FunctionContainerType::iterator fIt = m_FunctionContainer.begin()+index;
   m_FunctionContainer.erase(fIt);
  }

template <class TOutputPrecision, class TCoordRep>
typename MetaImageFunction<TOutputPrecision, TCoordRep>
::OutputType
MetaImageFunction<TOutputPrecision, TCoordRep>
::Evaluate(const PointType & point) const
 {
  // Build output
  OutputType resp;

  // For each function
  typename FunctionContainerType::const_iterator fIt = m_FunctionContainer.begin();
  while(fIt != m_FunctionContainer.end())
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
    for(unsigned int i = 0; i < currentVectorSize; ++i)
      {
      resp.SetElement(currentSize+i, static_cast<ValueType>(currentVector[i]));
      }

    // Go to next function
    ++fIt;
    }

  return resp;
 }


template <class TOutputPrecision, class TCoordRep>
void
MetaImageFunction<TOutputPrecision, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os<<indent<<"Number of image functions: "<<this->GetNumberOfFunctions()<<std::endl;
}

} // end namespace otb

#endif
