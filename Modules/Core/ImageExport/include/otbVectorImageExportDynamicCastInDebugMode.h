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

#ifndef otbVectorImageExportDynamicCastInDebugMode_h
#define otbVectorImageExportDynamicCastInDebugMode_h

#include "otbVectorImage.h"
#include "itkMacro.h"
#include "OTBImageExportExport.h"

extern template otb::VectorImage<unsigned char, 2u>*
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned char, 2u>*, itk::DataObject*>(itk::DataObject*);
extern template otb::VectorImage<float, 2u>*
  itkDynamicCastInDebugMode<otb::VectorImage<float, 2u>*, itk::DataObject*>(itk::DataObject*);
extern template otb::VectorImage<double, 2u>*
  itkDynamicCastInDebugMode<otb::VectorImage<double, 2u>*, itk::DataObject*>(itk::DataObject*);

extern template otb::VectorImage<unsigned char, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned char, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<unsigned short, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned short, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<unsigned int, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned int, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<float, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<float, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<double, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<double, 2u> const*, itk::DataObject const*>(itk::DataObject const*);

extern template otb::VectorImage<std::complex<short>, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<short>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<std::complex<int>, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<int>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<std::complex<float>, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<float>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
extern template otb::VectorImage<std::complex<double>, 2u> const*
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<double>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);

#endif
