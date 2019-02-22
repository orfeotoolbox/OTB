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

#ifndef otbImageExportSmartPointer_h
#define otbImageExportSmartPointer_h

#include "otbImage.h"
#include "OTBImageExportExport.h"

extern template class itk::SmartPointer< otb::Image<unsigned char, 2> >;
extern template class itk::SmartPointer< otb::Image<float, 2> >;
extern template class itk::SmartPointer< otb::Image<double, 2> >;

#endif
