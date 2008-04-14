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

#ifndef __otbVectorDataFileReader_txx
#define __otbVectorDataFileReader_txx

#include "otbMacro.h"
#include "otbSystem.h"
#include "otbVectorDataFileReader.h"

namespace otb
{
/**
 * Constructor
 */
template <class TOutputVectorData>
VectorDataFileReader<TOutputVectorData>
::VectorDataFileReader()
{
  m_VectorDataIO = 0;
  m_FileName = "";
}
/**
 * Destructor
 */
template <class TOutputVectorData>
VectorDataFileReader<TOutputVectorData>
::~VectorDataFileReader()
{
}

/*template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::SetFileName(char * filename)
{
  m_FileName = filename;
  this->Modified();
}*/

 /** Test whether the given filename exist and it is readable,
      this is intended to be called before attempting to use 
      ImageIO classes for actually reading the file. If the file
      doesn't exist or it is not readable, and exception with an
      approriate message will be thrown. */
template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::TestFileExistanceAndReadability()
{
    // Test if the file exists.
    if( ! itksys::SystemTools::FileExists( this->m_FileName.c_str() ) )
      {
      itk::VectorDataFileReaderException e(__FILE__, __LINE__);
      itk::OStringStream msg;
      msg <<"The file doesn't exists. "
          << std::endl << "Filename = " << this->m_FileName
          << std::endl;
      e.SetDescription(msg.str().c_str());
      throw e;
      return;
      }

    // Test if the file can be open for reading access.
    //Only if m_FileName speciy a filname (not a dirname)
    if( System::IsAFileName( this->m_FileName ) == true )
    {
        std::ifstream readTester;
        readTester.open( this->m_FileName.c_str() );
        if( readTester.fail() )
        {
                readTester.close();
                itk::OStringStream msg;
                msg <<"The file couldn't be opened for reading. "
                        << std::endl << "Filename: " << this->m_FileName
                        << std::endl;
                itk::VectorDataFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
                throw e;
                return;

        }
        readTester.close();
    }
}

template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::SetImageIO( ImageIOBase * imageIO)
{
  itkDebugMacro("setting ImageIO to " << imageIO ); 
  if (this->m_ImageIO != imageIO ) 
    {
    this->m_ImageIO = imageIO;
    this->Modified(); 
    } 
  m_UserSpecifiedImageIO = true;
}


template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::GenerateOutputInformation(void)
{
  typename TOutputImage::Pointer output = this->GetOutput();

  itkDebugMacro(<<"Reading file for GenerateOutputInformation()" << m_FileName);
  
  // Check to see if we can read the file given the name or prefix
  //
  if ( m_FileName == "" )
    {
    throw VectorDataFileReaderException(__FILE__, __LINE__, "FileName must be specified", ITK_LOCATION);
    }

  // Test if the file exist and if it can be open.
  // and exception will be thrown otherwise.
  //
  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch(itk::ExceptionObject &err)
    {
    m_ExceptionMessage = err.GetDescription();
    }

  if ( m_UserSpecifiedImageIO == false ) //try creating via factory
    {
    m_VectorDataIO = VectorData::CreateImageIO( m_FileName.c_str(), VectorDataIOFactory::ReadMode );
    }
  
  if ( m_VectorDataIO.IsNull() )
    {
    OStringStream msg;
    msg << " Could not create IO object for file "
        << m_FileName.c_str() << std::endl;
    if (m_ExceptionMessage.size())
      {
      msg << m_ExceptionMessage;
      }
    else
      {
      msg << "  Tried to create one of the following:" << std::endl;
      std::list<itk::LightObject::Pointer> allobjects = 
        ObjectFactoryBase::CreateAllInstance("itkImageIOBase");
      for(std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
          i != allobjects.end(); ++i)
        {
        VectorDataIOBase* io = dynamic_cast<VectorDataIOBase*>(i->GetPointer());
        msg << "    " << io->GetNameOfClass() << std::endl; 
        }
      msg << "  You probably failed to set a file suffix, or" << std::endl;
      msg << "    set the suffix to an unsupported type." << std::endl;
      }
    VectorDataFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }
  
  // Got to allocate space for the image. Determine the characteristics of
  // the image.
  //
  m_VectorDataIO->SetFileName(m_FileName.c_str());
  m_VectorDataIO->ReadImageInformation();


  //Copy MetaDataDictionary from instantiated reader to output image.
  output->SetMetaDataDictionary(m_VectorDataIO->GetMetaDataDictionary());
  this->SetMetaDataDictionary(m_VectorDataIO->GetMetaDataDictionary());

}



template <class TOutputImage, class ConvertPixelTraits>
void ImageFileReader<TOutputImage, ConvertPixelTraits>
::GenerateData()
{

  typename TOutputVectorData::Pointer output = this->GetOutput();

  itkDebugMacro ( << "VectorDataFileReader::GenerateData() \n"); 

  // Test if the file exist and if it can be open.
  // and exception will be thrown otherwise.
  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch(itk::ExceptionObject &err)
    {
    m_ExceptionMessage = err.GetDescription();
    }

  // Tell the ImageIO to read the file
  //

  m_VectorDataIO->SetFileName(m_FileName.c_str());

  m_ImageIO->Read(output);

  return;
}

/**
 * Main computation method
 */
template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::GenerateData()
{
  otbGenericMsgTestingMacro(<< "Reading..." ) ;
	
	TestFileExistanceAndReadability();
	typename OutputShapeFileType::Pointer shapeFile = this->GetOutput();
	
	OGRRegisterAll();

  OGRDataSource       *poDS;

  poDS = OGRSFDriverRegistrar::Open( m_FileName.c_str(), FALSE );
  if( poDS == NULL )
  {
      itkExceptionMacro(<< "ShapeFile Open failed.\n" );
  }
	
	shapeFile->SetOGRDataSource(poDS);	
}

template <class TOutputVectorData>
void
VectorDataFileReader<TOutputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (m_VectorDataIO)
    {
    os << indent << "VectorDataIO: \n";
    m_VectorDataIO->Print(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "m_VectorDataIO: (null)" << "\n";
    }

  os << indent << "m_FileName: " << m_FileName << "\n";
}


} //namespace otb

#endif
