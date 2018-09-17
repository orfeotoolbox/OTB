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

#ifndef itkImageSourceExtern_h
#define itkImageSourceExtern_h

#include "OTBExternExport.h"
#include "itkImageSource.h"
#include "otbImageExtern.h"
#include "otbVectorImageExtern.h"
#include <complex>

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
extern template class ImageSource<otb::Image<unsigned int, 2>>;
extern template class ImageSource<otb::Image<int, 2>>;
extern template class ImageSource<otb::Image<unsigned char, 2>>;
extern template class ImageSource<otb::Image<char, 2>>;
extern template class ImageSource<otb::Image<unsigned short, 2>>;
extern template class ImageSource<otb::Image<short, 2>>;
extern template class ImageSource<otb::Image<float, 2>>;
extern template class ImageSource<otb::Image<double, 2>>;
extern template class ImageSource<otb::Image<std::complex<int>, 2>>;
extern template class ImageSource<otb::Image<std::complex<short>, 2>>;
extern template class ImageSource<otb::Image<std::complex<float>, 2>>;
extern template class ImageSource<otb::Image<std::complex<double>, 2>>;
extern template class ImageSource<otb::VectorImage<unsigned int, 2>>;
extern template class ImageSource<otb::VectorImage<int, 2>>;
extern template class ImageSource<otb::VectorImage<unsigned char, 2>>;
extern template class ImageSource<otb::VectorImage<char, 2>>;
extern template class ImageSource<otb::VectorImage<unsigned short, 2>>;
extern template class ImageSource<otb::VectorImage<short, 2>>;
extern template class ImageSource<otb::VectorImage<float, 2>>;
extern template class ImageSource<otb::VectorImage<double, 2>>;
extern template class ImageSource<otb::VectorImage<std::complex<int>, 2>>;
extern template class ImageSource<otb::VectorImage<std::complex<short>, 2>>;
extern template class ImageSource<otb::VectorImage<std::complex<float>, 2>>;
extern template class ImageSource<otb::VectorImage<std::complex<double>, 2>>;

}

#endif // otbImageSource_h
