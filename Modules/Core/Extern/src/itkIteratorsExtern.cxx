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

#include "itkIteratorsExtern.h"

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<unsigned char>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<short>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<unsigned short>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<int>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<unsigned int>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<float>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<std::complex<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<std::complex<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<std::complex<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultPixelAccessor<std::complex<double>>;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<unsigned char>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<short>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<unsigned short>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<int>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<unsigned int>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<float>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<std::complex<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<std::complex<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<std::complex<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DefaultVectorPixelAccessor<std::complex<double>>;

// Following template does not compile at the moment (error in ITK ?)
//template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::VectorImage<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<double>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<double,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<double, 2>, otb::Image<double, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<double, 2>, otb::Image<double, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<double, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<std::complex<double>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<double>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<double>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<double>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<double>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<std::complex<double>,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<double>, 2>, otb::Image<std::complex<double>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<double>, 2>, otb::Image<std::complex<double>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<double>, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<float>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<float,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<float, 2>, otb::Image<float, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<float, 2>, otb::Image<float, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<float, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<std::complex<float>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<float>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<float>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<float>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<float>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<std::complex<float>,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<float>, 2>, otb::Image<std::complex<float>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<float>, 2>, otb::Image<std::complex<float>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<float>, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<short,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<short, 2>, otb::Image<short, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<short, 2>, otb::Image<short, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<short, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<unsigned short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<unsigned short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<unsigned short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<unsigned short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<unsigned short>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<unsigned short,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<unsigned short, 2>, otb::Image<unsigned short, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<unsigned short, 2>, otb::Image<unsigned short, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<unsigned short, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<std::complex<short>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<short>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<short>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<short>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<short>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<std::complex<short>,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<short>, 2>, otb::Image<std::complex<short>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<short>, 2>, otb::Image<std::complex<short>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<short>, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<int,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<int, 2>, otb::Image<int, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<int, 2>, otb::Image<int, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<int, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<unsigned int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<unsigned int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<unsigned int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<unsigned int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<unsigned int>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<unsigned int,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<unsigned int, 2>, otb::Image<unsigned int, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<unsigned int, 2>, otb::Image<unsigned int, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<unsigned int, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<std::complex<int>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<int>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<int>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<int>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<int>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<std::complex<int>,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<int>, 2>, otb::Image<std::complex<int>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<int>, 2>, otb::Image<std::complex<int>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<int>, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<unsigned char>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<unsigned char,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<unsigned char, 2>, otb::Image<unsigned char, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<unsigned char, 2>, otb::Image<unsigned char, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<unsigned char, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<itk::RGBAPixel<unsigned char>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<itk::RGBAPixel<unsigned char>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<itk::RGBAPixel<unsigned char>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<itk::RGBAPixel<unsigned char>,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<itk::RGBAPixel<unsigned char>, 2>, otb::Image<itk::RGBAPixel<unsigned char>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<itk::RGBAPixel<unsigned char>, 2>, otb::Image<itk::RGBAPixel<unsigned char>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<itk::RGBAPixel<unsigned char>, 2> >;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageConstIterator<otb::Image<itk::RGBPixel<unsigned char>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageRegionConstIterator<otb::Image<itk::RGBPixel<unsigned char>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageScanlineConstIterator<otb::Image<itk::RGBPixel<unsigned char>>>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Neighborhood<itk::RGBPixel<unsigned char>,2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ImageBoundaryCondition<otb::Image<itk::RGBPixel<unsigned char>, 2>, otb::Image<itk::RGBPixel<unsigned char>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<itk::RGBPixel<unsigned char>, 2>, otb::Image<itk::RGBPixel<unsigned char>, 2> >;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ConstNeighborhoodIterator<otb::Image<itk::RGBPixel<unsigned char>, 2> >;

}
