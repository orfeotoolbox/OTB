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
  if (!filename.empty())
    {
    ComplexFloatVectorReaderType::Pointer reader = ComplexFloatVectorReaderType::New();
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


template <class TComplexInputImage>
void
ComplexInputImageParameter::SetImage(TComplexInputImage* image)
{
  m_Image = image;
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
  m_Image   = NULL;
  m_Reader = NULL;
  m_Caster = NULL;
  m_FileName = "";
  m_PreviousFileName="";
  m_UseFilename = true;
}


}
}

