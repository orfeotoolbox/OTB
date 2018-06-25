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

#ifndef otbStandardMetaImageFunctionBuilder_hxx
#define otbStandardMetaImageFunctionBuilder_hxx

#include "otbStandardMetaImageFunctionBuilder.h"


namespace otb
{
template <class TPrecision, class TCoordRep>
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::StandardMetaImageFunctionBuilder()
{
  m_NeighborhoodRadius = 4;

  m_MetaImageFunction = MetaImageFunctionType::New();

  m_FlusserMomentsParam.push_back(static_cast<ValueType>(this->GetNeighborhoodRadius()));

  m_RadiometricMomentsParam.push_back(static_cast<ValueType>(this->GetNeighborhoodRadius()));

  m_LocalHistogramParam.push_back(static_cast<ValueType>(this->GetNeighborhoodRadius()));
  m_LocalHistogramParam.push_back(16);
  m_LocalHistogramParam.push_back(0);
  m_LocalHistogramParam.push_back(4096);

  m_FourierMellinDescriptorsParam.push_back(static_cast<ValueType>(this->GetNeighborhoodRadius()));
  m_FourierMellinDescriptorsParam.push_back(3);
  m_FourierMellinDescriptorsParam.push_back(3);

  m_HaralickTexturesParam.push_back(static_cast<ValueType>(this->GetNeighborhoodRadius()));
  m_HaralickTexturesParam.push_back(0);
  m_HaralickTexturesParam.push_back(4096);
  m_HaralickTexturesParam.push_back(30);
  m_HaralickTexturesParam.push_back(2);
}

template <class TPrecision, class TCoordRep>
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::~StandardMetaImageFunctionBuilder()
{

}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TPrecision, class TCoordRep>
unsigned int
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetNeighborhoodRadius()
{
  return m_NeighborhoodRadius;
}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::SetNeighborhoodRadius(unsigned int radius)
{
  if (radius>0)
    {
    m_NeighborhoodRadius = radius;

    m_FlusserMomentsParam[0]           = static_cast<ValueType>(radius);
    m_RadiometricMomentsParam[0]       = static_cast<ValueType>(radius);
    m_LocalHistogramParam[0]           = static_cast<ValueType>(radius);
    m_FourierMellinDescriptorsParam[0] = static_cast<ValueType>(radius);
    m_HaralickTexturesParam[0]         = static_cast<ValueType>(radius);
    }
}

template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::ParamContainerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetFlusserMomentsIFParameters()
{
  return m_FlusserMomentsParam;
}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::SetFlusserMomentsIFParameters(ParamContainerType Param)
{
  if (Param.size()==1)
    {
    m_FlusserMomentsParam[0] = Param[0];
    }
}


template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::ParamContainerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetLocalHistogramIFParameters()
{
  return m_LocalHistogramParam;
}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::SetLocalHistogramIFParameters(ParamContainerType Param)
{
  if (Param.size()==4)
    {
    m_LocalHistogramParam[0] = Param[0];
    m_LocalHistogramParam[1] = Param[1];
    m_LocalHistogramParam[2] = Param[2];
    m_LocalHistogramParam[3] = Param[3];
    }
}

template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::ParamContainerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetRadiometricMomentsIFParameters()
{
  return m_RadiometricMomentsParam;
}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::SetRadiometricMomentsIFParameters(ParamContainerType Param)
{
  if (Param.size()==1)
    {
    m_RadiometricMomentsParam[0] = Param[0];
    }
}

template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::ParamContainerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetFourierMellinDescriptorsIFParameters()
{
  return m_FourierMellinDescriptorsParam;
}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::SetFourierMellinDescriptorsIFParameters(ParamContainerType Param)
{
  if (Param.size()==3)
    {
    m_FourierMellinDescriptorsParam[0] = Param[0];
    m_FourierMellinDescriptorsParam[1] = Param[1];
    m_FourierMellinDescriptorsParam[2] = Param[2];
    }
}

template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::ParamContainerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetHaralickTexturesIFParameters()
{
  return m_HaralickTexturesParam;
}

template <class TPrecision, class TCoordRep>
void
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::SetHaralickTexturesIFParameters(ParamContainerType Param)
{
  if (Param.size()==5)
    {
    m_HaralickTexturesParam[0] = Param[0];
    m_HaralickTexturesParam[1] = Param[1];
    m_HaralickTexturesParam[2] = Param[2];
    m_HaralickTexturesParam[3] = Param[3];
    m_HaralickTexturesParam[4] = Param[4];
    }
}

template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::MetaImageFunctionPointerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetMetaImageFunction()
{
  return  m_MetaImageFunction;
}

template <class TPrecision, class TCoordRep>
typename StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>::DataObjectContainerType
StandardMetaImageFunctionBuilder<TPrecision, TCoordRep>
::GetDataObjectContainer()
{
  return  m_DataObjectContainer;
}

}//end namespace
#endif
