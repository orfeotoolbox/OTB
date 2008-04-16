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
#include "otbSHPVectorDataIO.h"
#include <itksys/SystemTools.hxx>

#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "otbMacro.h"
#include "otbSystem.h"
namespace otb
{

SHPVectorDataIO::SHPVectorDataIO()
{
  // By default set number of dimensions to two.
  if ( itk::ByteSwapper<char>::SystemIsLittleEndian() == true)
  {
        m_ByteOrder = LittleEndian;
  }
  else
  {
        m_ByteOrder = BigEndian;
  }

  m_FileByteOrder = BigEndian;
  
}

SHPVectorDataIO::~SHPVectorDataIO()
{
  if( m_File.is_open() )
    {
    m_File.close();
    }
}

bool SHPVectorDataIO::CanReadFile( const char* filename )
{
        std::string lFileName(filename);
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
        if( m_File.is_open() )
        {
                m_File.close();
        }

        std::fstream header_file;
        header_file.open( filename,  std::ios::in | std::ios::binary );
        if( header_file.fail() )
        {
                otbMsgDevMacro(<<"SHPVectorDataIO::CanReadFile() failed header open ! " );
                return false;
        }

        //Read header informations
        bool lResult(false);
        lResult = InternalReadHeaderInformation(header_file,false);
        header_file.close();
        return (lResult);
}


// Used to print information about this object
void SHPVectorDataIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read vector data
void SHPVectorDataIO::Read(VectorDataBase* buffer)
{
        VectorDataBase * p = buffer;
}


void SHPVectorDataIO::ReadVectorDataInformation()
{
        if( m_File.is_open() )
        {
                m_File.close();
        }

        m_File.open( m_FileName.c_str(),  std::ios::in | std::ios::binary );
        if( m_File.fail() )
        {
                itkExceptionMacro(<<"SHPVectorDataIO::ReadVectorDataInformation() failed header open ! " );
        }

        //Read header informations
        InternalReadHeaderInformation(m_File,true);
	
otbMsgDebugMacro( <<"Driver to read: SHP");
otbMsgDebugMacro( <<"         Read  file         : "<< m_FileName);
}


bool SHPVectorDataIO::InternalReadHeaderInformation(std::fstream & file, const bool reportError)
{
        return true;
}


bool SHPVectorDataIO::CanWriteFile( const char* filename )
{
        std::string lFileName(filename);
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
        const std::string Extension = System::GetExtension(filename);
        if( (Extension == "shp") || (Extension == "SHP") )
        {
                return true;
        }
        else
        {
                return false;
        }
}

void SHPVectorDataIO::Write(const VectorDataBase* buffer)
{
        if( m_FlagWriteVectorDataInformation == true )
        {
    	        this->WriteVectorDataInformation();
  	        m_FlagWriteVectorDataInformation = false;
        }

otbMsgDevMacro( <<" SHPVectorDataIO::Write()  ");
}


void SHPVectorDataIO::WriteVectorDataInformation()
{

        if ( m_FileName == "" )
        {
                itkExceptionMacro(<<"A FileName must be specified.");
        }
        if( CanWriteFile(m_FileName.c_str()) == false)
        {
                itkExceptionMacro(<< "The file "<<m_FileName.c_str()<<" is not defined as a SHP file");
        }
        // Close file from any previous vector data
        if ( m_File.is_open() )
        {
                m_File.close();
        }
  
        // Open the new file for writing
        // Actually open the file
        m_File.open( m_FileName.c_str(),  std::ios::out | std::ios::trunc | std::ios::binary );
        if( m_File.fail() )
        {
                itkExceptionMacro(<< "Cannot write requested file "<<m_FileName.c_str()<<".");
        } 


otbMsgDebugMacro( <<"Driver to write: SHP");
otbMsgDebugMacro( <<"         Write file         : "<< m_FileName);

}

  
} // end namespace otb

