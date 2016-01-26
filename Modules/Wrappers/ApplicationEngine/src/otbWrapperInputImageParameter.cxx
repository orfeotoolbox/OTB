/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
  if (!filename.empty())
    {
    FloatVectorReaderType::Pointer reader = FloatVectorReaderType::New();
    reader->SetFileName(filename);

    try
      {
      reader->UpdateOutputInformation();
      }
    catch(itk::ExceptionObject & /*err*/)
      {
      return false;
      }

    // the specified filename is valid => store the value
    m_FileName = filename;
    m_UseFilename = true;
    SetActive(true);
    return true;
    }
  return false;
}


FloatVectorImageType*
InputImageParameter::GetImage()
{
  return this->GetImage<FloatVectorImageType>();
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
  if( m_FileName.empty() && m_Image.IsNull() )
    return false;
  else
    return true;
}

void
InputImageParameter::ClearValue()
{
 m_Image  = NULL;
 m_Reader = NULL;
 m_Caster = NULL;
 m_FileName = "";
 m_PreviousFileName="";
 m_UseFilename = true;
}

}
}
