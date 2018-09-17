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

#ifndef otbBaseTypesExtern_h
#define otbBaseTypesExtern_h

#include "OTBExternExport.h"
#include "itkVariableLengthVector.h"
#include "itkVector.h"
#include "itkMatrix.h"
#include "itkPoint.h"
#include "itkPath.h"
#include "itkParametricPath.h"
#include "itkPolyLineParametricPath.h"
#include "itkContinuousIndex.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbVectorData.h"
#include "otbPolygon.h"
#include "otbDataNode.h"

namespace itk {
// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<double>;
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<float>;
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<unsigned int>;
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<int>;
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<unsigned short>;
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<short>;
extern template class OTBExtern_EXPORT_TEMPLATE VariableLengthVector<unsigned char>;

extern template class OTBExtern_EXPORT_TEMPLATE Point<double>;
extern template class Matrix<double>;
extern template class OTBExtern_EXPORT_TEMPLATE Vector<double>;
extern template class OTBExtern_EXPORT_TEMPLATE Point<float>;
extern template class OTBExtern_EXPORT_TEMPLATE Matrix<float>;
extern template class OTBExtern_EXPORT_TEMPLATE Vector<float>;
extern template class OTBExtern_EXPORT_TEMPLATE Path<double, itk::ContinuousIndex<double, 2>, 2>;
extern template class OTBExtern_EXPORT_TEMPLATE ParametricPath<2>;
extern template class OTBExtern_EXPORT_TEMPLATE PolyLineParametricPath<2>;
extern template class OTBExtern_EXPORT_TEMPLATE MetaDataObject<otb::ImageKeywordlist>;
extern template class OTBExtern_EXPORT_TEMPLATE SimpleDataObjectDecorator<double>;
}

namespace otb {
extern template class OTBExtern_EXPORT_TEMPLATE VectorData<double,2,double>;
extern template class Polygon<double>;
extern template class DataNode<double,2,double>;
}

#endif // otbBaseTypes_h
