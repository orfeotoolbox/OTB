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

#include "otbBaseTypesExtern.h"

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<float>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<unsigned int>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<int>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<unsigned short>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<short>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VariableLengthVector<unsigned char>;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Point<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Matrix<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Vector<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Point<float>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Matrix<float>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Vector<float>;

template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Path<double, itk::ContinuousIndex<double, 2>, 2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE ParametricPath<2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE PolyLineParametricPath<2>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE MetaDataObject<otb::ImageKeywordlist>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE SimpleDataObjectDecorator<double>;
}

namespace otb {
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE VectorData<double,2,double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE Polygon<double>;
template class OTBExtern_EXPORT_EXPLICIT_TEMPLATE DataNode<double,2,double>;
}
