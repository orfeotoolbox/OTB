/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   17 mai 2006
  Version   :   
  Role      :   Read/Write ONERA format data
  $Id$

=========================================================================*/
#include "otbONERAImageIO.h"
#include "itkByteSwapper.h"
#include <itksys/SystemTools.hxx>
#include <string.h>  // for strncpy

#include "itkExceptionObject.h"
#include "otbMacro.h"
#include "itkMacro.h"

#define ONERA_MAGIC_NUMBER     33554433
#define ONERA_HEADER_LENGTH    0
#define ONERA_FILE_ID_OFFSET   0

namespace otb
{

ONERAImageIO::ONERAImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  // By default set pixel type to scalar.
  m_PixelType = COMPLEX;
  // By default set component type to float
  m_ComponentType = FLOAT;
  m_NbOctetPixel = 4;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_currentfile = NULL;

}

ONERAImageIO::~ONERAImageIO()
{
}


bool ONERAImageIO::CanReadFile( const char* filename ) 
{ 
  // First check the filename extension
  std::ifstream file;
  std::string fname(filename);
  if ( fname == "" )
    {
    otbDebugMacro(<< "No filename specified.");
    }

  bool extensionDescriptionFileFound = true;
  bool extensionDataFileFound = false;
  
  std::string::size_type ONERAPos;
  ONERAPos = fname.rfind(".DAT");
  if ((ONERAPos != std::string::npos)
      && (ONERAPos == fname.length() - 4))
    {
    extensionDataFileFound = true;
    }

  ONERAPos = fname.rfind(".dat");
  if ((ONERAPos != std::string::npos)
      && (ONERAPos == fname.length() - 4))
    {
    extensionDataFileFound = true;
    }


  if( (!extensionDataFileFound) || (!extensionDescriptionFileFound) )
    {
    itkDebugMacro(<<"The filename extension is not recognized");
    return false;
    }


  // Now check the content
  std::ifstream inputStream;
  inputStream.open( filename, std::ios::in | std::ios::binary );
  if( inputStream.fail() )
    {
    return false;
    }

  // Check to see if its a ONERA file
  unsigned long file_id;
  file.seekg(ONERA_FILE_ID_OFFSET, std::ios::beg );
  file.read((char*)(&file_id),4);
  itk::ByteSwapper<unsigned long>::SwapFromSystemToBigEndian(&file_id);

  otbDebugMacro(<< "Magic number: " << file_id);

  file.close();
  return file_id == ONERA_MAGIC_NUMBER;

}



// Used to print information about this object
void ONERAImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void ONERAImageIO::ReadVolume(void*)
{
}


// Read image 
void ONERAImageIO::Read(void* buffer)
{
  std::ifstream file;
  otbMsgDebugMacro( << "ONERAImageIO::Read() Start");

  //read header information file:
  this->OpenOneraDataFileForReading(file, m_FileName.c_str());
  int HeaderLength = ONERA_HEADER_LENGTH + 4 + 4*2* m_width;
  
  file.seekg(HeaderLength, std::ios::beg);

  if( !this->ReadBufferAsBinary( file, buffer, this->GetImageSizeInBytes()) )
    {
    itkExceptionMacro(<<"Read failed: Wanted " << this->GetImageSizeInBytes()
                      << " bytes, but read " << file.gcount() << " bytes.");
    }

  //byte swapping depending on pixel type:
  if(this->GetComponentType() == FLOAT)
    {
    itk::ByteSwapper<float>::SwapRangeFromSystemToBigEndian(
        reinterpret_cast<float *>(buffer),
        this->GetImageSizeInComponents() );
    }

  //closing file:
  file.close();

  otbMsgDebugMacro( << "ONERAImageIO::Read() Finish");

}


bool ONERAImageIO::OpenOneraDataFileForReading(std::ifstream& os, 
                                             const char* filename)
                                       
{
  // Make sure that we have a file to 
  if ( filename == "" )
    {
    itkExceptionMacro(<<"A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if ( os.is_open() )
    {
    os.close();
    }
  
  // Open the new file for reading
  otbDebugMacro(<< "Initialize: opening file " << filename);

  // Actually open the file
  os.open( filename, std::ios::in | std::ios::binary );

  if ( os.fail() )
    {
    return false;
    }

  return true;
}


void ONERAImageIO::ReadImageInformation()
{
  std::ifstream file;
  this->InternalReadImageInformation(file);
  file.close();
}




void ONERAImageIO::InternalReadImageInformation(std::ifstream& file)
{
  //read .ent file (header)
  if ( ! this->OpenOneraDataFileForReading(file, m_FileName.c_str()) )
    {
    itkExceptionMacro(<< "Cannot read requested file");
    }


  // Find info...
  float NbCol;
  float NbRow;

  file.seekg(ONERA_FILE_ID_OFFSET + 4, std::ios::beg );
  file.read((char*)(&NbCol), 4);
  file.read((char*)(&NbRow), 4);
  itk::ByteSwapper<float>::SwapFromSystemToBigEndian(&NbCol);
  itk::ByteSwapper<float>::SwapFromSystemToBigEndian(&NbRow);
    
  file.seekg(0, std::ios::end);
  long gcountHead = static_cast<long>(ONERA_HEADER_LENGTH + 2*4*NbCol);
  long gcount     = static_cast<long>(file.tellg());

  // Set dim X,Y
  m_width = static_cast<int> ( NbCol );
  m_height = static_cast<int> ( gcount-gcountHead / (4*2) );

  if( m_width==0 & m_height==0)
      {
      itkExceptionMacro(<<"Unknown image dimension");
      }
    else
      {
      // Set image dimensions into IO
      m_Dimensions[0] = m_width;
      m_Dimensions[1] = m_height;
      otbMsgDebugMacro(<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]);
      }

  this->SetNumberOfDimensions(2);
  
  otbMsgDebugMacro(<<"Nb of rows    : "<<m_Dimensions[1]);
  otbMsgDebugMacro(<<"Nb of columns : "<<m_Dimensions[0]);

  m_NbBands = 2;
  this->SetNumberOfComponents(m_NbBands);
  otbMsgDebugMacro(<<"NbBands : "<<m_NbBands);
  otbMsgDebugMacro(<<"Nb of Components : "<<this->GetNumberOfComponents());

  // Automatically set the Type to Binary for GDAL data
  this->SetFileTypeToBinary();

  this->SetNumberOfComponents(2);
  this->SetPixelType(COMPLEX);

    
}


/** Actually we decide that we can't write ONERA images format */
bool ONERAImageIO::CanWriteFile( const char* name )
{
        return false;
}

/** TODO : Write Method not implemented */
void ONERAImageIO::Write(const void* buffer)
{
}

/** TODO : WriteImageInformation Method  not implemented */
void ONERAImageIO::WriteImageInformation()
{
}

  
} // end namespace otb

