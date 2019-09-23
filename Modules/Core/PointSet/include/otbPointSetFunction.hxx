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

#ifndef otbPointSetFunction_hxx
#define otbPointSetFunction_hxx

#include "otbPointSetFunction.h"

#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TPointSet, class TOutput>
PointSetFunction<TPointSet, TOutput>::PointSetFunction()
{
  m_PointSet = PointSetType::New();
}

/**
 *
 */
template <class TPointSet, class TOutput>
void PointSetFunction<TPointSet, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

// /**
//  * SetDetector method
//  */
// template <class TPointSet, class  TOutput >
// void
// PointSetFunction< TPointSet,   TOutput>
// ::SetPointSet(PointSetType* PointSet) const
// {
//   m_PointSet = PointSet;
// }

// /**
//  * GetDetector method
//  */
// template <class TPointSet, class  TOutput >
// const
// typename PointSetFunction< TPointSet, TOutput>
// ::PointSetType *
// PointSetFunction< TPointSet,   TOutput>
// ::GetPointSet() const
// {
//   return m_PointSet;
// }
} // end namespace otb

#endif
