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

#include "otbWrapperComplexInputImageParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbWrapperTypes.h"


namespace otb
{
namespace Wrapper
{

ComplexInputImageParameter::ComplexInputImageParameter()
{
  this->SetName("Complex Input Image");
  this->SetKey("cin");
  m_FileName="";
  m_PreviousFileName="";
  m_UseFilename = true;
  this->ClearValue();
}

ComplexInputImageParameter::~ComplexInputImageParameter()
{
}

bool
ComplexInputImageParameter::SetFromFileName(const std::string& filename)
{
  // First clear previous file chosen
  this->ClearValue();

  // No file existence is done here :
  //  - Done in the reader
  //  - allow appending additional information to the filename
  // myfile.tif:2 for example, or myfile.tif:nocarto
  m_FileName = filename;
  m_UseFilename = true;
  SetActive(true);
  return true;
}


ComplexFloatVectorImageType*
ComplexInputImageParameter::GetImage()
{
  return this->GetImage<ComplexFloatVectorImageType>();
}


#define otbGetImageMacro(image)                       \
  image##Type *                                       \
  ComplexInputImageParameter::Get##image ()           \
  {                                                   \
    return this->GetImage< image##Type > ();          \
  }

otbGetImageMacro(ComplexFloatImage);
otbGetImageMacro(ComplexDoubleImage);

otbGetImageMacro(ComplexFloatVectorImage);
otbGetImageMacro(ComplexDoubleVectorImage);


#define otbCastImageMacro(ComplexInputImageType, OutputImageType, theMethod)   \
  template<> OutputImageType *                                          \
  ComplexInputImageParameter::CastImage<ComplexInputImageType , OutputImageType>()    \
  {                                                                     \
    return this->theMethod<ComplexInputImageType , OutputImageType>();         \
  }

#define otbGenericCastImageMacro(ComplexInputImageType, theMethod, prefix)     \
  otbCastImageMacro(ComplexInputImageType, ComplexFloat##prefix##ImageType, theMethod) \
  otbCastImageMacro(ComplexInputImageType, ComplexDouble##prefix##ImageType, theMethod)


/*********************************************************************
********************** Image -> Image
**********************************************************************/

  otbGenericCastImageMacro(ComplexFloatImageType, SimpleCastImage, )
  otbGenericCastImageMacro(ComplexDoubleImageType, SimpleCastImage, )


/*********************************************************************
********************** VectorImage -> VectorImage
**********************************************************************/
  otbGenericCastImageMacro(ComplexFloatVectorImageType, SimpleCastImage, Vector)
  otbGenericCastImageMacro(ComplexDoubleVectorImageType, SimpleCastImage, Vector)

  void
ComplexInputImageParameter::SetImage(ComplexFloatVectorImageType* image)
{
  m_UseFilename = false;
  this->SetImage<ComplexFloatVectorImageType>( image );
}

bool
ComplexInputImageParameter::HasValue() const
{
  if( m_FileName.empty() && m_Image.IsNull() )
    return false;
  else
    return true;
}

void
ComplexInputImageParameter::ClearValue()
{
  m_Image   = ITK_NULLPTR;
  m_Reader = ITK_NULLPTR;
  m_Caster = ITK_NULLPTR;
  m_FileName = "";
  m_PreviousFileName="";
  m_UseFilename = true;
}


}
}

