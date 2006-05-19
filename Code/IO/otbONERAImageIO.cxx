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
#include <stdio.h>

#define ONERA_MAGIC_NUMBER     33554433
#define ONERA_HEADER_LENGTH    0
#define ONERA_FILE_ID_OFFSET   0

namespace otb
{

ONERAImageIO::ONERAImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  m_PixelType = COMPLEX;
  m_ComponentType = FLOAT;
  m_NbOctetPixel = 4;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;


}

ONERAImageIO::~ONERAImageIO()
{
  if( m_Datafile.is_open() )
    {
    m_Datafile.close();
    }
  if( m_Headerfile.is_open() )
    {
    m_Headerfile.close();
    }
}


//GetExtension from uiig library.
static std::string
GetExtension( const std::string& filename ) {

  // This assumes that the final '.' in a file name is the delimiter
  // for the file's extension type
  const std::string::size_type it = filename.find_last_of( "." );

  // This determines the file's type by creating a new string
  // who's value is the extension of the input filename
  // eg. "myimage.gif" has an extension of "gif"
  std::string fileExt( filename, it+1, filename.length() );

  return( fileExt );
}

//GetRootName from uiig library.
static std::string
GetRootName( const std::string& filename )
{
  const std::string fileExt = GetExtension(filename);

  // Create a base filename
  // i.e Image.ent --> Image
  if( fileExt.length() > 0 )
    {
    const std::string::size_type it = filename.find_last_of( fileExt );
    std::string baseName( filename, 0, it-fileExt.length() );
    return( baseName );
    }
  //Default to return same as input when the extension is nothing (Analyze)
  return( filename );
}


static std::string
GetHeaderFileName( const std::string & filename )
{
  std::string ImageFileName = GetRootName(filename);
  std::string fileExt = GetExtension(filename);
  //If file was named xxx.img.gz then remove both the gz and the img endings.
  if(fileExt.compare("ent"))
    {
    ImageFileName += ".ent";
    return( ImageFileName );
    }
  else
    {
    return("");
    }  
}

//Returns the base image filename.
static std::string GetImageFileName( const std::string& filename )
{
  std::string fileExt = GetExtension(filename);
  std::string ImageFileName = GetRootName(filename);

  if(fileExt.compare("dat") )
    {
    ImageFileName += ".dat";
    return( ImageFileName );
    }
  else
    {
    //uiig::Reporter* reporter = uiig::Reporter::getReporter();
    //std::string temp="Error, Can not determine compressed file image name. ";
    //temp+=filename;
    //reporter->setMessage( temp );
    return ("");
    }
}

bool ONERAImageIO::CanReadFile( const char* FileNameToRead )
{
  std::string filename(FileNameToRead);
  std::string ext;
  if( m_Datafile.is_open() )
    {
    m_Datafile.close();
    }
  if( m_Headerfile.is_open() )
    {
    m_Headerfile.close();
    }

  // we check that the correction extension is given by the user
  std::string filenameext = GetExtension(filename);
  if(filenameext != std::string("ent") 
    && filenameext != std::string("dat")
    )
    {
    return false;
    }

  const std::string HeaderFileName = GetHeaderFileName(filename);
  //
  // Try to read ENT file
  ext = GetExtension(HeaderFileName);
  if(ext != std::string("ent") && ext != std::string("dat"))
    {
    return false;
    }
  m_Headerfile.open( HeaderFileName.c_str(), 
                     std::ios::in );
  if( m_Headerfile.fail() )
    {
    return false;
    }

/*  if( ! this->ReadBufferAsAscii( local_InputStream, 
                                  (void *)&(this->m_hdr), 
                                  sizeof(struct dsr) ) )
    {
    return false;
    }
*/    

 // try to read DAT file
  const std::string ImageFileName = GetImageFileName(filename);
  ext = GetExtension(ImageFileName);
  if(ext != std::string("ent") && ext != std::string("dat"))
    {
    return false;
    }

  m_Datafile.open( ImageFileName.c_str(), 
                     std::ios::in | std::ios::binary );
  if( m_Datafile.fail() )
    {
    return false;
    }
  //Read header of the DAT file

  this->m_ByteOrder = LittleEndian;

  // Check magic_number
  int magicNumber; 
  m_Datafile.seekg(0, std::ios::beg );
  m_Datafile.read((char*)(&magicNumber),4);

  itk::ByteSwapper< int>::SwapFromSystemToLittleEndian(&magicNumber);

  otbDebugMacro(<< "Magic number : " << magicNumber);

  m_Datafile.close();

  return magicNumber == ONERA_MAGIC_NUMBER;
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
   unsigned int       dim;
   const unsigned int dimensions = this->GetNumberOfDimensions();
   unsigned long        numberOfPixels = 1;
   for(dim=0; dim< dimensions; dim++ )
      {
      numberOfPixels *= m_Dimensions[ dim ];
      }

   unsigned long step = this->GetNumberOfComponents();
   unsigned char * p = static_cast<unsigned char *>(buffer);
   
   int lNbLignes   = this->GetIORegion().GetSize()[1];
   int lNbColonnes = this->GetIORegion().GetSize()[0];
   int lPremiereLigne   = this->GetIORegion().GetIndex()[1] + 1; // [1... ]
   int lPremiereColonne = this->GetIORegion().GetIndex()[0] + 1; // [1... ]

//  std::ifstream file;
  otbMsgDebugMacro( << "ONERAImageIO::Read() Start");

  //read header information file:
//  this->OpenOneraDataFileForReading(file, m_FileName.c_str());

  unsigned long numberOfBytesPerLines = 2* m_width * m_NbOctetPixel;
  unsigned long headerLength = ONERA_HEADER_LENGTH + 4 + numberOfBytesPerLines;
  unsigned long offset;
  unsigned long numberOfBytesToBeRead = 2 * m_NbOctetPixel *lNbColonnes;
  unsigned long numberOfBytesRead;        

  char* value = new char[numberOfBytesToBeRead];
  unsigned long cpt = 0;
 
 
  for(int LineNo = lPremiereLigne;LineNo <lPremiereLigne + lNbLignes; LineNo++ )
    {
	offset  =  headerLength + numberOfBytesPerLines * LineNo;
	offset +=  m_NbOctetPixel * lPremiereColonne;
  	m_Datafile.seekg(offset, std::ios::beg);
        m_Datafile.read( static_cast<char *>( value ), numberOfBytesToBeRead );
    	numberOfBytesRead = m_Datafile.gcount();
        if( numberOfBytesRead != numberOfBytesToBeRead )
	  if( ( numberOfBytesRead != numberOfBytesToBeRead )  || m_Datafile.fail() )
             {
      		itkExceptionMacro(<<"ONERAImageIO::Read() Can Read the specified Region"); // read failed
      	     }

	for( unsigned long  i=0 ; i < numberOfBytesRead ; i = i+m_NbOctetPixel )
           {
            memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
            cpt += m_NbOctetPixel;
           }
    }

  //byte swapping depending on pixel type:
  if(this->GetComponentType() == FLOAT)
    {
    itk::ByteSwapper<float>::SwapRangeFromSystemToLittleEndian(
        reinterpret_cast<float *>(buffer),cpt);
    }

  otbMsgDebugMacro( << "ONERAImageIO::Read() Finish");

}


bool ONERAImageIO::OpenOneraDataFileForReading(const char* filename)
                                       
{
  // Make sure that we have a file to 
  if ( filename == "" )
    {
    itkExceptionMacro(<<"A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if ( m_Datafile.is_open() )
    {
    m_Datafile.close();
    }
  
  // Open the new file for reading
  otbDebugMacro(<< "Initialize: opening file " << filename);

  // Actually open the file
  m_Datafile.open( filename, std::ios::in | std::ios::binary );

  if ( m_Datafile.fail() )
    {
    return false;
    }

  return true;
}


void ONERAImageIO::ReadImageInformation()
{
  this->InternalReadImageInformation(m_Datafile);
}




void ONERAImageIO::InternalReadImageInformation(std::ifstream& file)
{
  //read .ent file (header)
  if ( ! this->OpenOneraDataFileForReading(m_FileName.c_str()) )
    {
    itkExceptionMacro(<< "Cannot read requested file");
    }


  // Find info...
  short NbCol;
  short NbRow;

  m_Datafile.seekg(6, std::ios::beg );
  m_Datafile.read((char*)(&NbCol),2);
  otbDebugMacro(<< "Magic number Before NbCase: " << NbCol);
  
  itk::ByteSwapper< short>::SwapFromSystemToLittleEndian(&NbCol);
    
  m_Datafile.seekg(0, std::ios::end);
  long gcountHead = static_cast<long>(ONERA_HEADER_LENGTH + 2*4*NbCol);
  long gcount     = static_cast<long>(m_Datafile.tellg());


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

