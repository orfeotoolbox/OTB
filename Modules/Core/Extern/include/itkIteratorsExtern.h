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

#ifndef itkIteratorsExtern_h
#define itkIteratorsExtern_h

#include "OTBExternExport.h"
#include "itkImageConstIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageScanlineConstIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhood.h"
#include "itkDefaultPixelAccessor.h"
#include "itkDefaultVectorPixelAccessor.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "otbVectorImageExtern.h"
#include "otbImageExtern.h"
#include "itkRGBAPixel.h"
#include "itkRGBPixel.h"

#include <complex>

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx

// Following template does not compile at the moment (error in ITK ?)
// extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::VectorImage<double>>;

extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<unsigned char>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<short>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<unsigned short>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<int>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<unsigned int>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<float>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<double>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<std::complex<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<std::complex<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<std::complex<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultPixelAccessor<std::complex<double>>;

extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<unsigned char>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<short>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<unsigned short>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<int>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<unsigned int>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<float>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<double>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<std::complex<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<std::complex<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<std::complex<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE DefaultVectorPixelAccessor<std::complex<double>>;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<double>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<double>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<double>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<double>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<double>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<double,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<double, 2>, otb::Image<double, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<double, 2>, otb::Image<double, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<double, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<std::complex<double>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<double>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<double>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<double>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<double>>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<std::complex<double>,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<double>, 2>, otb::Image<std::complex<double>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<double>, 2>, otb::Image<std::complex<double>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<double>, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<float>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<float,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<float, 2>, otb::Image<float, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<float, 2>, otb::Image<float, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<float, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<std::complex<float>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<float>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<float>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<float>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<float>>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<std::complex<float>,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<float>, 2>, otb::Image<std::complex<float>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<float>, 2>, otb::Image<std::complex<float>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<float>, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<short>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<short,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<short, 2>, otb::Image<short, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<short, 2>, otb::Image<short, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<short, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<unsigned short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<unsigned short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<unsigned short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<unsigned short>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<unsigned short>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<unsigned short,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<unsigned short, 2>, otb::Image<unsigned short, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<unsigned short, 2>, otb::Image<unsigned short, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<unsigned short, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<std::complex<short>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<short>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<short>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<short>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<short>>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<std::complex<short>,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<short>, 2>, otb::Image<std::complex<short>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<short>, 2>, otb::Image<std::complex<short>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<short>, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<int>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<int,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<int, 2>, otb::Image<int, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<int, 2>, otb::Image<int, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<int, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<unsigned int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<unsigned int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<unsigned int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<unsigned int>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<unsigned int>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<unsigned int,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<unsigned int, 2>, otb::Image<unsigned int, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<unsigned int, 2>, otb::Image<unsigned int, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<unsigned int, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<std::complex<int>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<std::complex<int>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<std::complex<int>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<std::complex<int>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<std::complex<int>>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<std::complex<int>,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<std::complex<int>, 2>, otb::Image<std::complex<int>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<std::complex<int>, 2>, otb::Image<std::complex<int>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<std::complex<int>, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<unsigned char>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::VectorImage<unsigned char>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<unsigned char>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::VectorImage<unsigned char>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<unsigned char>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<unsigned char,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<unsigned char, 2>, otb::Image<unsigned char, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<unsigned char, 2>, otb::Image<unsigned char, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<unsigned char, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<itk::RGBAPixel<unsigned char>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<itk::RGBAPixel<unsigned char>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<itk::RGBAPixel<unsigned char>>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<itk::RGBAPixel<unsigned char>,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<itk::RGBAPixel<unsigned char>, 2>, otb::Image<itk::RGBAPixel<unsigned char>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<itk::RGBAPixel<unsigned char>, 2>, otb::Image<itk::RGBAPixel<unsigned char>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<itk::RGBAPixel<unsigned char>, 2> >;

extern template class OTBExtern_EXPORT_TEMPLATE ImageConstIterator<otb::Image<itk::RGBPixel<unsigned char>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageRegionConstIterator<otb::Image<itk::RGBPixel<unsigned char>>>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageScanlineConstIterator<otb::Image<itk::RGBPixel<unsigned char>>>;
extern template class OTBExtern_EXPORT_TEMPLATE Neighborhood<itk::RGBPixel<unsigned char>,2>;
extern template class OTBExtern_EXPORT_TEMPLATE ImageBoundaryCondition<otb::Image<itk::RGBPixel<unsigned char>, 2>, otb::Image<itk::RGBPixel<unsigned char>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ZeroFluxNeumannBoundaryCondition<otb::Image<itk::RGBPixel<unsigned char>, 2>, otb::Image<itk::RGBPixel<unsigned char>, 2> >;
extern template class OTBExtern_EXPORT_TEMPLATE ConstNeighborhoodIterator<otb::Image<itk::RGBPixel<unsigned char>, 2> >;

}

#endif // itkIteratorsExtern_h
