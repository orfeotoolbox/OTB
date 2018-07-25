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

#ifndef itkOptimizersExtern_h
#define itkOptimizersExtern_h

#include "OTBExternExport.h"
#include "itkOptimizerParameters.h"
#include "itkOptimizerParametersHelper.h"
#include "itkSymmetricEigenAnalysis.h"
#include "itkBaseTypesExtern.h"
#include "itkFixedArray.h"

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
extern template class OTBExtern_EXPORT_TEMPLATE OptimizerParameters<double>;
extern template class OTBExtern_EXPORT_TEMPLATE OptimizerParametersHelper<double>;
extern template class OTBExtern_EXPORT_TEMPLATE SymmetricEigenAnalysis<itk::Matrix<double, 3u, 3u>, itk::FixedArray<double, 3u>, itk::Matrix<double, 3u, 3u> >;
}

#endif // itkOptimizersExtern_h
