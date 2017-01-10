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
#include "otbMWImageIO.h"
#include "itkByteSwapper.h"
#include "otbSystem.h"
#include "itksys/SystemTools.hxx"

#include "otbMacro.h"


namespace otb
{

MWImageIO::MWImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;
  if (itk::ByteSwapper<char>::SystemIsLittleEndian() == true)
    {
    m_ByteOrder = LittleEndian;
    }
  else
    {
    m_ByteOrder = BigEndian;
    }

  m_FileByteOrder = BigEndian;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to [0.5 , 0.5]
  // (consistency between ImageIO, see Mantis #942)
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;

  m_FlagWriteImageInformation = true;
  m_Ncom = 0;

  this->AddSupportedWriteExtension(".img");
  this->AddSupportedWriteExtension(".IMG");
  this->AddSupportedWriteExtension(".mw");
  this->AddSupportedWriteExtension(".MW");

  this->AddSupportedReadExtension(".img");
  this->AddSupportedReadExtension(".IMG");
  this->AddSupportedReadExtension(".mw");
  this->AddSupportedReadExtension(".MW");

}

MWImageIO::~MWImageIO()
{
  if (m_File.is_open())
    {
    m_File.close();
    }
}

bool MWImageIO::CanReadFile(const char* filename)
{
  std::string lFileName(filename);
  // Test the extension
  std::string extension = GetExtension(filename);
  if ((extension != "img") && (extension != "") && (extension != "mw")) return false;
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true) return false;
  if (m_File.is_open()) m_File.close();

  std::fstream header_file;
  header_file.open(filename,  std::ios::in | std::ios::binary);
  if (header_file.fail())
    {
    otbMsgDevMacro(<< "MWImageIO::CanReadFile() failed header open ! ");
    return false;
    }

  //Read header information
  bool lResult = InternalReadHeaderInformation(header_file, false);
  header_file.close();
  return (lResult);
}

// Used to print information about this object
void MWImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void MWImageIO::ReadVolume(void*)
{
}

// Read image
void MWImageIO::Read(void* buffer)
{
  char * p = static_cast<char *>(buffer);

  int lNbLignes   = this->GetIORegion().GetSize()[1];
  int lNbColonnes = this->GetIORegion().GetSize()[0];
  int lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " MWImageIO::Read()  ");
  otbMsgDevMacro(<< " Dimensions de l'image  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region lue (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components       : " << this->GetNumberOfComponents());

  std::streamoff headerLength =
    static_cast<std::streamoff> (64 * sizeof(char)) + static_cast<std::streamoff> (m_Ncom * sizeof(char));
  std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>(this->GetComponentSize() * m_Dimensions[0]);
  std::streamoff  offset;
  std::streamsize numberOfBytesToBeRead = static_cast<std::streamsize>(this->GetComponentSize() * lNbColonnes);
  std::streamsize numberOfBytesRead;
  std::streamsize cpt = 0;
  for (int LineNo = lPremiereLigne; LineNo < lPremiereLigne + lNbLignes; LineNo++)
    {
    offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
    offset +=  static_cast<std::streamoff>(this->GetComponentSize() * lPremiereColonne);
    m_File.seekg(offset, std::ios::beg);
    m_File.read(static_cast<char *>(p + cpt), numberOfBytesToBeRead);
    numberOfBytesRead = m_File.gcount();
#ifdef __APPLE_CC__
    // fail() is broken in the Mac. It returns true when reaches eof().
    if (numberOfBytesRead != numberOfBytesToBeRead)
#else
    if ((numberOfBytesRead != numberOfBytesToBeRead)  || m_File.fail())
#endif
      {
      itkExceptionMacro(<< "MWImageIO::Read() Can Read the specified Region"); // read failed
      }
    cpt += numberOfBytesToBeRead;
    }

  unsigned long numberOfPixelsPerLines = lNbLignes * lNbColonnes;
  // Swap bytes if necessary
  if (0) {}
  otbSwappFileToSystemMacro(float, FLOAT, buffer, numberOfPixelsPerLines)
  else
    {
    itkExceptionMacro(<< "MWImageIO::Read() undefined component type! ");
    }
}

void MWImageIO::ReadImageInformation()
{
  if (m_File.is_open())
    {
    m_File.close();
    }

  m_File.open(m_FileName.c_str(),  std::ios::in | std::ios::binary);
  if (m_File.fail())
    {
    itkExceptionMacro(<< "MWImageIO::ReadImageInformation() failed header open ! ");
    }

  //Read header information
  InternalReadHeaderInformation(m_File, true);

  otbMsgDebugMacro(<< "Driver to read: MW");
  otbMsgDebugMacro(<< "         Read  file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

bool MWImageIO::InternalReadHeaderInformation(std::fstream& file, const bool reportError)
{

  char * headerInformation = new char[64];
  file.seekg(0, std::ios::beg);
  file.read(headerInformation, 64);
  unsigned short NbCol(0);
  unsigned short NbLig(0);
  unsigned short Nbcom(0);

  //Set file byte order
  if (headerInformation[0] == *("R") ||  headerInformation[1] == *("I"))
    {
    m_FileByteOrder = BigEndian;
    //Read image dimensions
    Nbcom = (static_cast<unsigned int>(headerInformation[3]) << 8) + static_cast<unsigned char> (headerInformation[2]);
    m_Ncom = Nbcom;
    NbCol = (static_cast<unsigned int>(headerInformation[5]) << 8) + static_cast<unsigned char> (headerInformation[4]);
    NbLig = (static_cast<unsigned int>(headerInformation[7]) << 8) + static_cast<unsigned char> (headerInformation[6]);
    }
  else if (headerInformation[0] == *("I") ||  headerInformation[1] == *("R"))
    {
    m_FileByteOrder = LittleEndian;
    //Read image dimensions
    Nbcom = ((static_cast<unsigned int>(headerInformation[2])) << 8) + static_cast<unsigned char> (headerInformation[3]);
    m_Ncom = Nbcom;
    NbCol = ((static_cast<unsigned int>(headerInformation[4])) << 8) + static_cast<unsigned char> (headerInformation[5]);
    NbLig = ((static_cast<unsigned int>(headerInformation[6])) << 8) + static_cast<unsigned char> (headerInformation[7]);
    }
  else
    {
    delete [] headerInformation;

    if (reportError == true)
      {
      itkExceptionMacro(<< "MW : impossible to determine CodePix information of the image");
      }
    return false;
    }

  SetComponentType(FLOAT);

  //Initialization of image information
  m_Dimensions[0] = NbCol;
  m_Dimensions[1] = NbLig;
  this->SetNumberOfComponents(1);
  this->SetFileTypeToBinary();
  this->SetNumberOfDimensions(2);
  delete[] headerInformation;
  return (true);
}

bool MWImageIO::CanWriteFile(const char* filename)
{
  std::string lFileName(filename);
  std::string extension = GetExtension(filename);
  if (extension != "mw") return false;
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true) return false;

  return true;
}

void MWImageIO::Write(const void* buffer)
{
  if (m_FlagWriteImageInformation == true)
    {
    this->WriteImageInformation();
    m_FlagWriteImageInformation = false;
    }

  unsigned long lNbLignes   = this->GetIORegion().GetSize()[1];
  unsigned long lNbColonnes = this->GetIORegion().GetSize()[0];
  unsigned long lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int           lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]

  if ((lNbLignes == m_Dimensions[1]) && (lNbColonnes == m_Dimensions[0]))
    {
    otbMsgDevMacro(<< "Forcing offset to [0, 0]");
    lPremiereLigne = 0;
    lPremiereColonne = 0;
    }

  otbMsgDevMacro(<< " MWImageIO::Write()  ");
  otbMsgDevMacro(<< " Dimensions de l'image  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region lue (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components       : " << this->GetNumberOfComponents());
  otbMsgDevMacro(<< " GetComponentSize       : " << this->GetComponentSize());

  std::streamoff numberOfBytesPerLines = this->GetComponentSize() * m_Dimensions[0];
  std::streamoff headerLength =
    static_cast<std::streamoff> (64 * sizeof(char)) + static_cast<std::streamoff> (m_Ncom * sizeof(char));
  std::streamoff  offset;
  std::streamsize numberOfBytesToBeWrite = this->GetComponentSize() * lNbColonnes;
//   std::streamsize numberOfBytesToBeWriteFloat = sizeof(float) * lNbColonnes;
  std::streamsize cpt = 0;

  const char * p = static_cast<const char *>(buffer);
//   const float * p = dynamic_cast<const float *>(buffer);

//   float floatImage[m_Dimensions[0]*m_Dimensions[1]];

//   std::streamsize counter = 0;
//   std::cout << "begin conversion" << std::endl;
//   for (unsigned int i=0; i<m_Dimensions[0]*m_Dimensions[1]; ++i)
//   {
//     floatImage[i] = static_cast< float > ( *(p+counter) );
//     std::cout << "float " << floatImage[i] << std::endl;
//     counter+=sizeof(char);
//   }

  for (unsigned long LineNo = lPremiereLigne; LineNo < lPremiereLigne + lNbLignes; LineNo++)
    {
    offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
    offset +=  static_cast<std::streamoff>(this->GetComponentSize() * lPremiereColonne);
    m_File.seekp(offset, std::ios::beg);
    m_File.write(static_cast<const char *>(p + cpt), numberOfBytesToBeWrite);
//     m_File.write( (char *)( floatImage + cpt ), numberOfBytesToBeWriteFloat );
    cpt += numberOfBytesToBeWrite;
    }
}

void MWImageIO::WriteImageInformation()
{

  if (m_FileName == "")
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    }
  if (CanWriteFile(m_FileName.c_str()) == false)
    {
    itkExceptionMacro(<< "The file " << m_FileName.c_str() << " is not defined as a MW file");
    }
  // Close file from any previous image
  if (m_File.is_open())
    {
    m_File.close();
    }

  // Open the new file for writing
  // Actually open the file
  m_File.open(m_FileName.c_str(),  std::ios::out | std::ios::trunc | std::ios::binary);
  if (m_File.fail())
    {
    itkExceptionMacro(<< "Cannot write requested file " << m_FileName.c_str() << ".");
    }

  //Comment by MG to review write process

  //Writing header information
//   if ( 0 ) {}
//   if (  (this->GetComponentType() != FLOAT) )
//   {
//     itkExceptionMacro(<< "MegaWave fimage format only accepts FLOAT32 data type");
//   }
  m_ComponentType = FLOAT;

  m_File.seekp(0, std::ios::beg);

  char        header[64];
  std::string comments("Image written with otb mw_IO_factory");
  m_Ncom = comments.length();
  //Initialization to 0
  for (unsigned int i = 0; i < 64; ++i)
    header[i] = static_cast<unsigned char>(0);
  //Write image size and other information
  unsigned short lNbComments = m_Ncom;
  unsigned short lNbLignes   = static_cast<unsigned short>(this->GetIORegion().GetSize()[1]);
  unsigned short lNbColonnes = static_cast<unsigned short>(this->GetIORegion().GetSize()[0]);

  unsigned short low, high;
  if (m_ByteOrder == BigEndian)
    {
    header[0] = *("R");
    header[1] = *("I");

    ByteSplitting(lNbComments, low, high);
    header[2] = static_cast<unsigned char>(low);
    header[3] = static_cast<unsigned char>(high);

    ByteSplitting(lNbColonnes, low, high);
    header[4] = static_cast<unsigned char>(low);
    header[5] = static_cast<unsigned char>(high);

    ByteSplitting(lNbLignes, low, high);
    header[6] = static_cast<unsigned char>(low);
    header[7] = static_cast<unsigned char>(high);
    }
  else if (m_ByteOrder == LittleEndian)
    {
    header[0] = *("I");
    header[1] = *("R");

    ByteSplitting(lNbComments, low, high);
    header[3] = static_cast<unsigned char>(low);
    header[2] = static_cast<unsigned char>(high);

    ByteSplitting(lNbColonnes, low, high);
    header[5] = static_cast<unsigned char>(low);
    header[4] = static_cast<unsigned char>(high);

    ByteSplitting(lNbLignes, low, high);
    header[7] = static_cast<unsigned char>(low);
    header[6] = static_cast<unsigned char>(high);
    }
  else
    {
    itkExceptionMacro(<< "Unknown Byte order");
    }

  m_File.write(header, 64);
  m_File.write(comments.data(), 36);

  otbMsgDebugMacro(<< "Driver to write: MW");
  otbMsgDebugMacro(<< "         Write file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         Type Mw           : " << m_TypeMW);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

std::string MWImageIO::GetExtension(const std::string& filename)
{
  // This assumes that the final '.' in a file name is the delimiter
  // for the file's extension type
  const std::string::size_type it = filename.find_last_of(".");

  // This determines the file's type by creating a new string
  // who's value is the extension of the input filename
  // eg. "myimage.gif" has an extension of "gif"
  std::string fileExt(filename, it + 1, filename.length());

  //If the extension has a "/" in it then this is not an extension and there are no extension.
  std::string::size_type it2 = fileExt.find_last_of("/");
  if (it2 != std::string::npos)
    {
    std::string fileExt3("");
    return (fileExt3);
    }

  return (fileExt);

}

} // end namespace otb
