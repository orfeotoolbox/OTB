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
  m_FileName="";
  m_PreviousFileName="";
  m_UseFilename = true;
  this->ClearValue();
}

InputImageParameter::~InputImageParameter()
{
}

bool
InputImageParameter::SetFromFileName(const std::string& filename)
{
  // First clear previous file chosen
  this->ClearValue();

  // No file existence is done here :
  //  - Done in the reader
  //  - allow appending additional information to the filename
  // myfile.tif:2 for example, or myfile.tif:nocarto
  m_FileName = filename;
  m_UseFilename = true;
  SetActive( true );

  return true;
}


FloatVectorImageType*
InputImageParameter::GetImage()
{
  return this->GetImage<FloatVectorImageType>();
}

template <>
ImageBaseType*
InputImageParameter::GetImage<ImageBaseType>()
{
  if (m_Image.IsNull())
    return this->GetImage<FloatVectorImageType>();

  return m_Image;
}

otbGetImageMacro(UInt8RGBImage);
otbGetImageMacro(UInt8RGBAImage);

void
InputImageParameter::SetImage(FloatVectorImageType* image)
{
  m_UseFilename = false;
  this->SetImage<FloatVectorImageType>( image );
}


bool
InputImageParameter::HasValue() const
{
  return m_FileName.empty() && m_Image.IsNull();
}

void
InputImageParameter
::ClearValue()
{
  m_Image  = nullptr;
  m_Reader = nullptr;
  m_InputCaster = nullptr;
  m_OutputCaster = nullptr;
  m_FileName = "";
  m_PreviousFileName="";
  m_UseFilename = true;
}

}
}
