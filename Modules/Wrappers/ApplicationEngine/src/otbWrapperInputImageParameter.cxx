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

#include <boost/algorithm/string.hpp>

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
  // First clear previous file choosen
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


#define otbGetImageMacro(image)                       \
  image##Type *                                       \
  InputImageParameter::Get##image ()                  \
  {                                                   \
    return this->GetImage< image##Type > ();          \
  }

otbGetImageMacro(UInt8Image)
otbGetImageMacro(UInt16Image);
otbGetImageMacro(Int16Image);
otbGetImageMacro(UInt32Image);
otbGetImageMacro(Int32Image);
otbGetImageMacro(FloatImage);
otbGetImageMacro(DoubleImage);

otbGetImageMacro(UInt8VectorImage);
otbGetImageMacro(UInt16VectorImage);
otbGetImageMacro(Int16VectorImage);
otbGetImageMacro(UInt32VectorImage);
otbGetImageMacro(Int32VectorImage);
otbGetImageMacro(FloatVectorImage);
otbGetImageMacro(DoubleVectorImage);

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


#define otbCastImageMacro(InputImageType, OutputImageType, theMethod)   \
  template<> OutputImageType *                                          \
  InputImageParameter::CastImage<InputImageType , OutputImageType>()    \
  {                                                                     \
    return this->theMethod<InputImageType , OutputImageType>();         \
  }

#define otbGenericCastImageMacro(InputImageType, theMethod, prefix)     \
  otbCastImageMacro(InputImageType, UInt8##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, UInt16##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Int16##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, UInt32##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Int32##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Float##prefix##ImageType, theMethod) \
  otbCastImageMacro(InputImageType, Double##prefix##ImageType, theMethod)


/*********************************************************************
********************** Image -> Image
**********************************************************************/
otbGenericCastImageMacro(UInt8ImageType, SimpleCastImage, )
otbGenericCastImageMacro(Int16ImageType, SimpleCastImage, )
otbGenericCastImageMacro(UInt16ImageType, SimpleCastImage, )
otbGenericCastImageMacro(Int32ImageType, SimpleCastImage, )
otbGenericCastImageMacro(UInt32ImageType, SimpleCastImage, )
otbGenericCastImageMacro(FloatImageType, SimpleCastImage, )
otbGenericCastImageMacro(DoubleImageType, SimpleCastImage, )


/*********************************************************************
********************** VectorImage -> VectorImage
**********************************************************************/
otbGenericCastImageMacro(UInt8VectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(Int16VectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(UInt16VectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(Int32VectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(UInt32VectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(FloatVectorImageType, SimpleCastImage, Vector)
otbGenericCastImageMacro(DoubleVectorImageType, SimpleCastImage, Vector)


/*********************************************************************
********************** Image -> VectorImage
**********************************************************************/
otbGenericCastImageMacro(UInt8ImageType, CastVectorImageFromImage, Vector)
otbGenericCastImageMacro(Int16ImageType, CastVectorImageFromImage, Vector)
otbGenericCastImageMacro(UInt16ImageType, CastVectorImageFromImage, Vector)
otbGenericCastImageMacro(Int32ImageType, CastVectorImageFromImage, Vector)
otbGenericCastImageMacro(UInt32ImageType, CastVectorImageFromImage, Vector)
otbGenericCastImageMacro(FloatImageType, CastVectorImageFromImage, Vector)
otbGenericCastImageMacro(DoubleImageType, CastVectorImageFromImage, Vector)


}
}

