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

#include "otbImage.h"

namespace otb {

// Explicit instanciation of common types
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<unsigned int, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<int, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<unsigned char, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<char, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<unsigned short, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<short, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<float, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<double, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<std::complex<int>, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<std::complex<short>, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<std::complex<float>, 2>;
template class OTBImageBase_EXPORT_EXPLICIT_TEMPLATE Image<std::complex<double>, 2>;
}
