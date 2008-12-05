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
#ifndef __otbPointSetFileReader_txx
#define __otbPointSetFileReader_txx

#include "otbPointSetFileReader.h"
#include "otbMacro.h"

#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>

#include <fstream>  // std::ifstream
#include <iostream> // std::cout

namespace otb
{
  template <class TOutputPointSet>
      PointSetFileReader<TOutputPointSet>
  ::PointSetFileReader() : otb::PointSetSource<TOutputPointSet>()
  {
    m_NumberOfPoints=-1;
    m_MinX=0;
    m_MaxX=0;
    m_MinY=0;
    m_MaxY=0;
  }
  
  template <class TOutputPointSet>
      PointSetFileReader<TOutputPointSet>
  ::~PointSetFileReader()
  {
  }

  
  
  template <class TOutputPointSet>
      void 
      PointSetFileReader<TOutputPointSet>
  ::GenerateOutputInformation(void)
  {
    typename TOutputPointSet::Pointer output = this->GetOutput();
  
    otbDebugMacro(<<"Reading file for GenerateOutputInformation()" << m_FileName);
  
  // Check to see if we can read the file given the name or prefix
    //
    if ( m_FileName == "" )
    {
      throw itk::ImageFileReaderException(__FILE__, __LINE__, "FileName must be specified", ITK_LOCATION);
    }
   

    
    // Test if the file exists and if it can be open.
    // An exception will be thrown otherwise.
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
  
    std::ifstream ifs;
    ifs.open(m_FileName.c_str(), std::ios::in | std::ios::binary);
    liblas::LASReader reader(ifs);
  
    liblas::LASHeader const& header = reader.GetHeader();
    
    otbDebugMacro(<< "Signature: " << header.GetFileSignature());
    otbDebugMacro(<< "Points count: " << header.GetPointRecordsCount());

    m_NumberOfPoints = header.GetPointRecordsCount();
    m_MinX = header.GetMinX();
    m_MaxX = header.GetMaxX();
    m_MinY = header.GetMinY();
    m_MaxY = header.GetMaxY();
    ifs.close();
  
  }
  
  
  template <class TOutputPointSet>
      void
      PointSetFileReader<TOutputPointSet>
  ::TestFileExistanceAndReadability()
  {
  // Test if the file exists.
    if( ! itksys::SystemTools::FileExists( m_FileName.c_str() ) )
    {
      itk::ImageFileReaderException e(__FILE__, __LINE__);
      itk::OStringStream msg;
      msg <<"The file doesn't exist. "
          << std::endl << "Filename = " << m_FileName
          << std::endl;
      e.SetDescription(msg.str().c_str());
      throw e;
      return;
    }

  // Test if the file can be open for reading access.
    std::ifstream readTester;
    readTester.open( m_FileName.c_str() );
    if( readTester.fail() )
    {
      readTester.close();
      itk::OStringStream msg;
      msg <<"The file couldn't be opened for reading. "
          << std::endl << "Filename: " << m_FileName
          << std::endl;
      itk::ImageFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
      throw e;
      return;
    
    }
    readTester.close();
  }
  
  template <class TOutputPointSet>
      void PointSetFileReader<TOutputPointSet>
  ::GenerateData()
  {
  

    typename TOutputPointSet::Pointer output = this->GetOutput();
    
    std::ifstream ifs;
    ifs.open(m_FileName.c_str(), std::ios::in | std::ios::binary);
    liblas::LASReader reader(ifs);
  
    liblas::LASHeader const& header = reader.GetHeader();
    
    otbDebugMacro(<< "Signature: " << header.GetFileSignature());
    otbDebugMacro(<< "Points count: " << header.GetPointRecordsCount());

    m_NumberOfPoints = header.GetPointRecordsCount();
  
    while (reader.ReadNextPoint())
    {
      liblas::LASPoint const& p = reader.GetPoint();

      PointType point;
      point[0] = p.GetX();
      point[1] = p.GetY();


      unsigned long i = output->GetNumberOfPoints();
      output->SetPoint( i, point );        

      PixelType V;
      V = static_cast<PixelType>( p.GetZ() );
      output->SetPointData( i, V );
    
    }
    
    
    ifs.close();
  }



  template <class TOutputPointSet>
      void PointSetFileReader<TOutputPointSet>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Number of points: " << this->m_NumberOfPoints << std::endl;
    os << indent << "Min X: " << this->m_MinX << std::endl;
    os << indent << "Max X: " << this->m_MaxX << std::endl;
    os << indent << "Min Y: " << this->m_MinY << std::endl;
    os << indent << "Max Y: " << this->m_MaxY << std::endl;
    os << indent << "m_FileName: " << this->m_FileName << "\n";
  }
  
} //namespace otb

#endif

