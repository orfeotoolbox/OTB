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

#ifndef __otbShapeFileDataReader_txx
#define __otbShapeFileDataReader_txx

#include "itkImageFileReader.h"

#include "otbMacro.h"
#include "otbSystem.h"
#include "otbShapeFileDataReader.h"

namespace otb
{
/**
 * Constructor
 */
template <class TOutputShapeFile>
ShapeFileDataReader<TOutputShapeFile>
::ShapeFileDataReader()
{
  m_FileName = "";
}
/**
 * Destructor
 */
template <class TOutputShapeFile>
ShapeFileDataReader<TOutputShapeFile>
::~ShapeFileDataReader()
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
template <class TOutputShapeFile>
void
ShapeFileDataReader<TOutputShapeFile>
::TestFileExistanceAndReadability()
{
    // Test if the file exists.
    if( ! itksys::SystemTools::FileExists( this->m_FileName.c_str() ) )
      {
      itk::ImageFileReaderException e(__FILE__, __LINE__);
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
                itk::ImageFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
                throw e;
                return;

        }
        readTester.close();
    }
}

/**
 * Main computation method
 */
template <class TOutputShapeFile>
void
ShapeFileDataReader<TOutputShapeFile>
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

template <class TOutputShapeFile>
void
ShapeFileDataReader<TOutputShapeFile>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
	Superclass::PrintSelf(os, indent);
	os << indent << "ShapeFileReader" << std::endl;
}



} //namespace otb

#endif
