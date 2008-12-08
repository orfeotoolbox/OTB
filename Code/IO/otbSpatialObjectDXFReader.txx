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
#ifndef __otbSpatialObjectDXFReader_txx
#define __otbSpatialObjectDXFReader_txx

#include "otbSpatialObjectDXFReader.h"
#include "otbDXFToSpatialObjectGroupFilter.h"
#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "itksys/SystemTools.hxx"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbImageFileReader.h"

namespace otb
{
/**
 * Constructor
 */
template <class TSpatialObject>
SpatialObjectDXFReader<TSpatialObject>
::SpatialObjectDXFReader()
{
  m_FileName = "";
  m_LayerName="";
  // this->itk::ProcessObject::ReleaseDataBeforeUpdateFlagOff();
  this->GetOutput()->ReleaseData();
  this->ResetPipeline();
}
/**
 * Destructor
 */
template <class TSpatialObject>
SpatialObjectDXFReader<TSpatialObject>
::~SpatialObjectDXFReader()
{}

template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::SetFileName(char * filename)
{
  m_FileName = filename;
  this->Modified();
}
 /** Test whether the given filename exist and it is readable,
      this is intended to be called before attempting to use
      ImageIO classes for actually reading the file. If the file
      doesn't exist or it is not readable, and exception with an
      approriate message will be thrown. */
template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
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
template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::GenerateData()
{
  TestFileExistanceAndReadability();
  typedef otb::DXFToSpatialObjectGroupFilter<TSpatialObject> CreationFilter;
  typename CreationFilter::Pointer creationClass = CreationFilter::New();
  if (m_LayerName.size()>0)
    {
      creationClass->SetLayer(m_LayerName.c_str());
    }
  DL_Dxf dxf;
  dxf.in(m_FileName, creationClass);
  GroupSpatialObjectType * group = creationClass->GetOutput();
  group->Update();
  this->SetNthOutput(0,group);
}

template <class TSpatialObject>
void
SpatialObjectDXFReader<TSpatialObject>
::Update(void)
{
  this->GenerateData();
}

} //namespace otb

#endif
