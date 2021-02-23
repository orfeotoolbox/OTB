/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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
#include "itksys/SystemTools.hxx"
#include "otbWrapperTypes.h"
#include "otbWrapperInputImageParameterMacros.h"
#include "otb_boost_string_header.h"


namespace otb
{

namespace Wrapper
{

InputImageParameter::InputImageParameter()
{
  this->SetName("Input Image");
  this->SetKey("in");
}

bool InputImageParameter::SetFromFileName(std::string filename)
{
  // First clear previous file chosen
  this->ClearValue();

  // No file existence is done here :
  //  - Done in the reader
  //  - allow appending additional information to the filename
  // myfile.tif:2 for example, or myfile.tif:nocarto
  m_FileName    = move(filename);
  m_UseFilename = true;
  SetActive(true);

  return true;
}


ImageBaseType* InputImageParameter::GetImage()
{
  return m_Image.GetPointer();
}


ImageBaseType const* InputImageParameter::GetImage() const
{
  return m_Image.GetPointer();
}


template <>
ImageBaseType* InputImageParameter::GetImage<ImageBaseType>()
{
  if (m_Image.IsNull())
    return this->GetImage<FloatVectorImageType>();

  return m_Image;
}

otbGetImageMacro(UInt8RGBImage);
otbGetImageMacro(UInt8RGBAImage);


void InputImageParameter::SetImage(ImageBaseType* image)
{
  m_UseFilename = false;
  m_Image       = image;
}


bool InputImageParameter::HasValue() const
{
  return !m_FileName.empty() || !m_Image.IsNull();
}

void InputImageParameter::ClearValue()
{
  m_Image            = nullptr;
  m_Reader           = nullptr;
  m_OutputCaster     = nullptr;
  m_OutputCasted     = nullptr;
  m_FileName         .clear();
  m_PreviousFileName .clear();
  m_UseFilename      = true;
}

ParameterType InputImageParameter::GetType() const
{
  return ParameterType_InputImage;
}

std::string InputImageParameter::ToString() const
{
  return GetFileName();
}

void InputImageParameter::FromString(const std::string& value)
{
  SetFromFileName(value);
}
}
}
