/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTransformsExtern_h
#define otbTransformsExtern_h

#include "OTBExternExport.h"
#include "itkTransform.h"
#include "itkIdentityTransform.h"
#include "otbTransform.h"
#include "otbSensorModelBase.h"
#include "otbGenericMapProjection.h"
#include "otbInverseSensorModel.h"
#include "otbForwardSensorModel.h"
#include "otbGenericRSTransform.h"
#include "otbCompositeTransform.h"

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
extern template class OTBExtern_EXPORT_TEMPLATE Transform<double,2,2>;
extern template class OTBExtern_EXPORT_TEMPLATE IdentityTransform<double,2>;
}

namespace otb {
extern template class OTBExtern_EXPORT_TEMPLATE Transform<double,2,2>;
extern template class OTBExtern_EXPORT_TEMPLATE SensorModelBase<double,2,2>;
extern template class OTBExtern_EXPORT_TEMPLATE GenericMapProjection<otb::TransformDirection::FORWARD, double, 2u, 2u>;
extern template class OTBExtern_EXPORT_TEMPLATE GenericMapProjection<otb::TransformDirection::INVERSE, double, 2u, 2u>;
extern template class OTBExtern_EXPORT_TEMPLATE InverseSensorModel<double,2,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ForwardSensorModel<double,2,2>;
extern template class OTBExtern_EXPORT_TEMPLATE GenericRSTransform<double,2,2>;
extern template class OTBExtern_EXPORT_TEMPLATE CompositeTransform<itk::Transform<double, 2, 2>, itk::Transform<double, 2, 2>, double, 2, 2>;
}

#endif // otbTransformsExtern_h
