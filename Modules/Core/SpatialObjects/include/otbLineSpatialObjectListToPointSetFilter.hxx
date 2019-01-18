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

#ifndef otbLineSpatialObjectListToPointSetFilter_hxx
#define otbLineSpatialObjectListToPointSetFilter_hxx

#include "otbLineSpatialObjectListToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template<class TLinesList, class TPointSet>
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>::LineSpatialObjectListToPointSetFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->SetNthOutput(0, PointSetType::New());

}

template<class TLinesList, class TPointSet>
void
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::SetInput(const LinesListType *list)
{
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<LinesListType  *>(list));
}

template<class TLinesList, class TPointSet>
const typename LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>::LinesListType*
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::GetInput(void)
{
  return static_cast<const LinesListType *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TLinesList, class TPointSet>
void
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::SetOutput(const PointSetType *pointSet)
{
  this->ProcessObjectType::SetNthOutput(0,
                                        const_cast<PointSetType *>(pointSet));
}

template<class TLinesList, class TPointSet>
typename LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>::PointSetType*
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::GetOutput(void)
{
  return static_cast<PointSetType *>
           (this->ProcessObjectType::GetOutput(0));
}

/**
 *
 */

template<class TLinesList, class TPointSet>
void
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
