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

#include "otbVectorImageExportDynamicCastInDebugMode.h"

template otb::VectorImage<unsigned char, 2u>* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned char, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::VectorImage<float, 2u>* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<float, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::VectorImage<double, 2u>* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<double, 2u>*, itk::DataObject*>(itk::DataObject*);

template otb::VectorImage<unsigned char, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned char, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<unsigned short, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned short, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<unsigned int, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned int, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<float, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<float, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<double, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<double, 2u> const*, itk::DataObject const*>(itk::DataObject const*);

template otb::VectorImage<std::complex<short>, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<short>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<std::complex<int>, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<int>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<std::complex<float>, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<float>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<std::complex<double>, 2u> const* OTBImageExport_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<std::complex<double>, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
