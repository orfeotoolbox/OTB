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

#include "otbWrapperInputImageParameter.h"
#include "otbWrapperTypes.h"
#include "otbWrapperInputImageParameterMacros.h"

namespace otb
{
namespace Wrapper
{
otbGetImageAndVectorImageMacro(UInt8);
// otbGetImageMacro(UInt8VectorImage);
otbGetImageAndVectorImageMacro(UInt16);
// otbGetImageMacro(UInt16VectorImage);
otbGetImageAndVectorImageMacro(UInt32);
// otbGetImageMacro(UInt32VectorImage);
otbGetImageAndVectorImageMacro(Int16);
// otbGetImageMacro(Int16VectorImage);
otbGetImageAndVectorImageMacro(Int32);
// otbGetImageMacro(Int32VectorImage);
otbGetImageAndVectorImageMacro(Float);
// otbGetImageMacro(FloatVectorImage);
otbGetImageAndVectorImageMacro(Double);
// otbGetImageMacro(DoubleVectorImage);
otbGetImageAndVectorImageMacro(ComplexInt16);
// otbGetImageMacro(ComplexInt16VectorImage);
otbGetImageAndVectorImageMacro(ComplexInt32);
// otbGetImageMacro(ComplexInt32VectorImage);
otbGetImageAndVectorImageMacro(ComplexFloat);
// otbGetImageMacro(ComplexFloatVectorImage);
otbGetImageAndVectorImageMacro(ComplexDouble);
// otbGetImageMacro(ComplexDoubleVectorImage);
} // end namespace wrapper
} // end namespace otb
