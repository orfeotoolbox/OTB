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

#include "itkTemplateExports.h"

namespace itk
{

template class OTBITK_EXPORT_EXPLICIT_TEMPLATE ImageBase<2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE ImageRegion<2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE SmartPointer<LightObject>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE SmartPointer<Object>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE SmartPointer<DataObject>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE SmartPointer<ImageBase<2> >;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE Index<2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE Size<2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE FixedArray<double,2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE Point<double,2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE Vector<double,2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE Matrix<double,2,2>;
template class OTBITK_EXPORT_EXPLICIT_TEMPLATE RGBPixel<unsigned char>;

}
