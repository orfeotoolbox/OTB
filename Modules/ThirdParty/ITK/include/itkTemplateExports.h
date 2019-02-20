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

#ifndef itkTemplateExports_h
#define itkTemplateExports_h

#include <cstring>

#include "itkImageBase.h"
#include "itkDataObject.h"
#include "itkImageRegion.h"
#include "itkSmartPointer.h"
#include "itkIndex.h"
#include "itkSize.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkMatrix.h"
#include "itkFixedArray.h"
#include "itkLightObject.h"
#include "itkRGBPixel.h"
#include "OTBITKExport.h"

namespace itk
{

extern template class ImageBase<2>;
extern template class ImageRegion<2>;
extern template class SmartPointer<LightObject>;
extern template class SmartPointer<Object>;
extern template class SmartPointer<DataObject>;
extern template class SmartPointer<ImageBase<2> >;
extern template class Index<2>;
extern template class Size<2>;
extern template class FixedArray<double,2>;
extern template class Point<double,2>;
extern template class Vector<double,2>;
extern template class Matrix<double,2,2>;
extern template class RGBPixel<unsigned char>;

}

#endif
