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

#include "otbVectorPrediction.h"

namespace otb
{
namespace Wrapper
{

using VectorRegression = VectorPrediction<true, float, float>;

template<>
void
VectorRegression
::CreatePredictionField(OGRFeatureDefn & layerDefn, otb::ogr::Layer & outLayer)
{
  int idx = layerDefn.GetFieldIndex(GetParameterString("cfield").c_str());
  if (idx >= 0)
  {
    if (layerDefn.GetFieldDefn(idx)->GetType() != OFTReal)
      itkExceptionMacro("Field name "<< GetParameterString("cfield") << " already exists with a different type!");
  }
  else
  {
    OGRFieldDefn predictedField(GetParameterString("cfield").c_str(), OFTReal);
    ogr::FieldDefn predictedFieldDef(predictedField);
    outLayer.CreateField(predictedFieldDef);
  }
}

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorRegression)
