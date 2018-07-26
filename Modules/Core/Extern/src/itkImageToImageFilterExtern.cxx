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

#include "itkImageToImageFilterExtern.h"


namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageSource<otb::Image<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageToImageFilter<otb::Image<double>, otb::Image<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE InPlaceImageFilter<otb::Image<double>, otb::Image<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageSource<otb::VectorImage<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE InPlaceImageFilter<otb::VectorImage<double>, otb::VectorImage<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageToImageFilter<otb::VectorImage<double>, otb::VectorImage<double>>;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageSource<otb::Image<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageToImageFilter<otb::Image<float>, otb::Image<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE InPlaceImageFilter<otb::Image<float>, otb::Image<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageSource<otb::VectorImage<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE InPlaceImageFilter<otb::VectorImage<float>, otb::VectorImage<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageToImageFilter<otb::VectorImage<float>, otb::VectorImage<float>>;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageSource<otb::Image<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageToImageFilter<otb::Image<unsigned char>, otb::Image<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE InPlaceImageFilter<otb::Image<unsigned char>, otb::Image<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageSource<otb::VectorImage<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE InPlaceImageFilter<otb::VectorImage<unsigned char>, otb::VectorImage<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageToImageFilter<otb::VectorImage<unsigned char>, otb::VectorImage<unsigned char>>;
}
