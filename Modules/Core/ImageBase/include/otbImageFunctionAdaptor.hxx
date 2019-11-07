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

#ifndef otbImageFunctionAdaptor_hxx
#define otbImageFunctionAdaptor_hxx

#include "otbImageFunctionAdaptor.h"

namespace otb
{
template <class TInternalImageFunctionType, class TOutputPrecision>
ImageFunctionAdaptor<TInternalImageFunctionType, TOutputPrecision>::ImageFunctionAdaptor()
{
  m_InternalImageFunction = InternalImageFunctionType::New();
  m_Converter             = ConverterType::New();
}

template <class TInternalImageFunctionType, class TOutputPrecision>
void ImageFunctionAdaptor<TInternalImageFunctionType, TOutputPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Internal Image Function: " << m_InternalImageFunction << std::endl;
}

template <class TInternalImageFunctionType, class TOutputPrecision>
typename ImageFunctionAdaptor<TInternalImageFunctionType, TOutputPrecision>::OutputType
ImageFunctionAdaptor<TInternalImageFunctionType, TOutputPrecision>::EvaluateAtIndex(const IndexType& index) const
{
  OutputType result;
  this->GetInternalImageFunction()->SetInputImage(this->GetInputImage());
  InternalImageFunctionOutputType tmpResult = this->GetInternalImageFunction()->EvaluateAtIndex(index);
  result                                    = m_Converter->Convert(tmpResult);

  return result;
}


} // end namespace otb

#endif
