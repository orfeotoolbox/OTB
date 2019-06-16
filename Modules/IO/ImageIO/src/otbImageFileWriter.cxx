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

#include "otbImageFileWriter.h"

namespace otb {

// Explicit instanciation of common types
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<unsigned int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<unsigned char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<unsigned short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<float, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<double, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<std::complex<int>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<std::complex<short>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<std::complex<float>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<Image<std::complex<double>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<unsigned int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<int, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<unsigned char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<char, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<unsigned short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<short, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<float, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<double, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<std::complex<int>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<std::complex<short>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<std::complex<float>, 2>>;
template class OTBImageIO_EXPORT_EXPLICIT_TEMPLATE ImageFileWriter<VectorImage<std::complex<double>, 2>>;

}
