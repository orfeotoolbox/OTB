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

#include "otbConvertTypeFunctor.h"

namespace otb
{
namespace Functor
{
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<unsigned char>(unsigned int i, unsigned char const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<short>(unsigned int i, short const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<unsigned short>(unsigned int i, unsigned short const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<int>(unsigned int i, int const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<unsigned int>(unsigned int i, unsigned int const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<float>(unsigned int i, float const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<double>(unsigned int i, double const & pix, std::vector < double > & vPix);

template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<unsigned char>>(unsigned int i, itk::VariableLengthVector<unsigned char> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<short>>(unsigned int i, itk::VariableLengthVector<short> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<unsigned short>>(unsigned int i, itk::VariableLengthVector<unsigned short> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<int>>(unsigned int i, itk::VariableLengthVector<int> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<unsigned int>>(unsigned int i, itk::VariableLengthVector<unsigned int> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<float>>(unsigned int i, itk::VariableLengthVector<float> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<itk::VariableLengthVector<double>>(unsigned int i, itk::VariableLengthVector<double> const & pix, std::vector < double > & vPix);

template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<std::complex<short> >(unsigned int i, std::complex<short> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<std::complex<int> >(unsigned int i, std::complex<int> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<std::complex<float> >(unsigned int i, std::complex<float> const & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillIn<std::complex<double> >(unsigned int i, std::complex<double> const & pix, std::vector < double > & vPix);

template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<unsigned char>(unsigned int i, unsigned char & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<short>(unsigned int i, short & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<unsigned short>(unsigned int i, unsigned short & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<int>(unsigned int i, int & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<unsigned int>(unsigned int i, unsigned int & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<float>(unsigned int i, float & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<double>(unsigned int i, double & pix, std::vector < double > & vPix);

template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<unsigned char> >(unsigned int i, itk::VariableLengthVector<unsigned char> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<short>>(unsigned int i, itk::VariableLengthVector<short> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<unsigned short>>(unsigned int i, itk::VariableLengthVector<unsigned short> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<int>>(unsigned int i, itk::VariableLengthVector<int> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<unsigned int>>(unsigned int i, itk::VariableLengthVector<unsigned int> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<float>>(unsigned int i, itk::VariableLengthVector<float> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<itk::VariableLengthVector<double>>(unsigned int i, itk::VariableLengthVector<double> & pix, std::vector < double > & vPix);

template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<std::complex<short> >(unsigned int i, std::complex<short> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<std::complex<int> >(unsigned int i, std::complex<int> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<std::complex<float> >(unsigned int i, std::complex<float> & pix, std::vector < double > & vPix);
template void OTBImageManipulation_EXPORT_EXPLICIT_TEMPLATE FillOut<std::complex<double> >(unsigned int i, std::complex<double> & pix, std::vector < double > & vPix);
}
}
