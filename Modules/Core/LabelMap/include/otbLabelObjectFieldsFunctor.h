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

#ifndef otbLabelObjectFieldsFunctor_h
#define otbLabelObjectFieldsFunctor_h

#include "itkLabelObject.h"
#include <map>

namespace otb
{

namespace Functor
{

/** \class LabelObjectFieldsFunctor
 *  \brief Returns additional OGR fields to be saved in VectorData
 *
 * \sa LabelObjectWithClassLabelFieldsFunctor
 *
 * \ingroup OTBLabelMap
 */
template <class TLabelObject>
class ITK_EXPORT LabelObjectFieldsFunctor
{
public:
  // The Label Object typedef
  typedef TLabelObject LabelObjectType;
  typedef std::map<std::string, std::string> FieldsMapType;

  FieldsMapType operator()(LabelObjectType* itkNotUsed(labelObject)) const
  {
    // don't return anything for standard LabelObject (label is already handled by LabelmapToVectorData)
    return FieldsMapType();
  }
};

} // end namespace Functor

} // end namespace otb
#endif
