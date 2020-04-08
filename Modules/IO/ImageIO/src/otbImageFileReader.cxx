/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImageFileReader.h"

namespace otb
{

// Explicit instantiation of common types
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<unsigned int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<unsigned char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<unsigned short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<float, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<double, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<std::complex<int>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<std::complex<short>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<std::complex<float>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<Image<std::complex<double>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<unsigned int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<unsigned char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<unsigned short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<float, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<double, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<std::complex<int>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<std::complex<short>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<std::complex<float>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileReader<VectorImage<std::complex<double>, 2>>;
}
