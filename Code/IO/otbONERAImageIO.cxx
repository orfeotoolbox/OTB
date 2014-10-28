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
#include "otbONERAImageIO.h"
#include "itksys/SystemTools.hxx"

#include "otbMacro.h"

#include "otbSystem.h"

#define ONERA_MAGIC_NUMBER     33554433
#define ONERA_HEADER_LENGTH    4
#define ONERA_FILE_ID_OFFSET   0

namespace otb
{

ONERAImageIO::ONERAImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  m_PixelType = COMPLEX;
  m_ComponentType = FLOAT;
  m_BytePerPixel = 4;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to [0.5 , 0.5]
  // (consistency between ImageIO, see Mantis #942)
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;

  m_FlagWriteImageInformation = true;

  if (itk::ByteSwapper<char>::SystemIsLittleEndian() == true)
    {
    m_ByteOrder = LittleEndian;
    }
  else
    {
    m_ByteOrder = BigEndian;
    }

  m_FileByteOrder = BigEndian;

}

ONERAImageIO::~ONERAImageIO()
{
  if (m_Datafile.is_open())
    {
    m_Datafile.close();
    }
  if (m_Headerfile.is_open())
    {
    m_Headerfile.close();
    }
}

bool ONERAImageIO::CanReadFile(const char* FileNameToRead)
{

  std::string filename(FileNameToRead);
  std::string ext;
  if (m_Datafile.is_open())
    {
    m_Datafile.close();
    }
  if (m_Headerfile.is_open())
    {
    m_Headerfile.close();
    }

  if (itksys::SystemTools::FileIsDirectory(filename.c_str()) == true)
    {
    return false;
    }

  const std::string HeaderFileName = System::GetRootName(filename) + ".ent";
  const std::string DataFileName = System::GetRootName(filename) + ".dat";

  m_Headerfile.open(HeaderFileName.c_str(),  std::ios::in);
  if (m_Headerfile.fail())
    {
    otbMsgDevMacro(<< "ONERAImageIO::CanReadFile() failed header open ! ");
    return false;
    }
  m_Datafile.open(DataFileName.c_str(),  std::ios::in);
  if (m_Datafile.fail())
    {
    otbMsgDevMacro(<< "ONERAImageIO::CanReadFile() failed data open ! ");
    return false;
    }

  // Check magic_number
  int magicNumber;
  m_Datafile.seekg(0, std::ios::beg);
  m_Datafile.read((char*) (&magicNumber), 4);

  if (magicNumber == ONERA_MAGIC_NUMBER)
    {
    m_FileByteOrder = m_ByteOrder;
    }
  else
    {
    if (m_ByteOrder == LittleEndian)
      {
      m_FileByteOrder = BigEndian;
      }
    else if (m_ByteOrder == BigEndian)
      {
      m_FileByteOrder = LittleEndian;
      }
    }
  // Swap if necessary
  otbSwappFileOrderToSystemOrderMacro(int, &magicNumber, 1);

  m_Headerfile.close();
  m_Datafile.close();

  if (magicNumber == ONERA_MAGIC_NUMBER)
    {
    return true;
    }
  else
    {
    return false;
    }

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
  unsigned long      numberOfPixels = 1;
  for (dim = 0; dim < dimensions; ++dim)
    {
    numberOfPixels *= m_Dimensions[dim];
    }

  unsigned char * p = static_cast<unsigned char *>(buffer);

  int lNbLines   = this->GetIORegion().GetSize()[1];
  int lNbColumns = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " ONERAImageIO::Read()  ");
  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components  : " << this->GetNumberOfComponents());

  //read header information file:
  if (!this->OpenOneraDataFileForReading(m_FileName.c_str()))
    {
    itkExceptionMacro(<< "Cannot read requested file");
    }

  std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>(2 * m_width * m_BytePerPixel);
  std::streamoff  headerLength = ONERA_HEADER_LENGTH + numberOfBytesPerLines;
  std::streamoff  offset;
  std::streamsize numberOfBytesToBeRead = 2 * m_BytePerPixel * lNbColumns;
  std::streamsize numberOfBytesRead;

  char*           value = new char[numberOfBytesToBeRead];
  std::streamsize cpt = 0;

  for (int LineNo = lFirstLine; LineNo < lFirstLine + lNbLines; LineNo++)
    {
    offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
    offset +=  static_cast<std::streamoff>(m_BytePerPixel * lFirstColumn);
    m_Datafile.seekg(offset, std::ios::beg);
    m_Datafile.read(static_cast<char *>(value), numberOfBytesToBeRead);
    numberOfBytesRead = m_Datafile.gcount();
#ifdef __APPLE_CC__
    // fail() is broken in the Mac. It returns true when reaches eof().
    if (numberOfBytesRead != numberOfBytesToBeRead)
#else
    if ((numberOfBytesRead != numberOfBytesToBeRead)  || m_Datafile.fail())
#endif
      {
      itkExceptionMacro(<< "ONERAImageIO::Read() Can Read the specified Region"); // read failed
      }

    memcpy((void*) (&(p[cpt])), (const void*) (value), (size_t) (numberOfBytesToBeRead));
    cpt += numberOfBytesToBeRead;
    }

  //byte swapping depending on pixel type:
  unsigned long numberOfPixelsPerRegion = lNbLines * lNbColumns * 2;
  // Swap bytes if necessary
  if (0) {}
  otbSwappFileToSystemMacro(float, FLOAT, buffer, numberOfPixelsPerRegion)
  otbSwappFileToSystemMacro(double, DOUBLE, buffer, numberOfPixelsPerRegion)
  else
    {
    itkExceptionMacro(<< "ONERAImageIO::Read() undefined component type! ");
    }

  delete[] value;
  value = NULL;

}

bool ONERAImageIO::OpenOneraDataFileForReading(const char* filename)
{
  // Make sure that we have a file to
  std::string lFileName(filename);
  if (lFileName.empty())
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if (m_Datafile.is_open())
    {
    m_Datafile.close();
    }
  const std::string DataFileName = System::GetRootName(filename) + ".dat";

  // Open the new file for reading
  m_Datafile.open(DataFileName.c_str(),  std::ios::in | std::ios::binary);
  if (m_Datafile.fail())
    {
    otbMsgDebugMacro(<< "ONERAImageIO::CanReadFile() failed data open ! ");
    return false;
    }
  return true;
}

bool ONERAImageIO::OpenOneraHeaderFileForReading(const char* filename)
{
  // Make sure that we have a file to
  std::string lFileName(filename);
  if (lFileName.empty())
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if (m_Headerfile.is_open())
    {
    m_Headerfile.close();
    }
  const std::string HeaderFileName = System::GetRootName(filename) + ".ent";

  // Open the new file for reading
  // Actually open the file
  m_Headerfile.open(HeaderFileName.c_str(),  std::ios::in);
  if (m_Headerfile.fail())
    {
    otbMsgDebugMacro(<< "ONERAImageIO::CanReadFile() failed header open ! ");
    return false;
    }
  return true;
}

void ONERAImageIO::ReadImageInformation()
{
  this->InternalReadImageInformation();
}

void ONERAImageIO::InternalReadImageInformation()
{

  if (!this->OpenOneraDataFileForReading(m_FileName.c_str()))
    {
    itkExceptionMacro(<< "Cannot read ONERA data file " << m_FileName);
    }

  if (!this->OpenOneraHeaderFileForReading(m_FileName.c_str()))
    {
    itkExceptionMacro(<< "Cannot read ONERA header file " << m_FileName);
    }

  // check "Format_valeurs_look"
  char* sHeader = new char[1024];
  // skip 2 lines
  m_Headerfile.getline(sHeader, 1024);
  m_Headerfile.getline(sHeader, 1024);
  m_Headerfile.getline(sHeader, 1024);
  std::string sPixelType(sHeader);

  if (sPixelType.compare("cmplx_real_4"))
    {
    m_NbBands = 1;
    this->SetFileTypeToBinary();
    this->SetNumberOfComponents(2);
    this->SetPixelType(COMPLEX);
    m_ComponentType = FLOAT;
    m_BytePerPixel = 4;
    }
  else
    {
    itkExceptionMacro(<< "data format not supported by OTB (only 'complex_real_4' is available)");
    }

  // Check magic_number
  int magicNumber;
  m_Datafile.seekg(0, std::ios::beg);
  m_Datafile.read((char*) (&magicNumber), 4);
  if (magicNumber == ONERA_MAGIC_NUMBER)
    {
    m_FileByteOrder = m_ByteOrder;
    }
  else
    {
    if (m_ByteOrder == LittleEndian)
      {
      m_FileByteOrder = BigEndian;
      }
    else if (m_ByteOrder == BigEndian)
      {
      m_FileByteOrder = LittleEndian;
      }
    }
  otbSwappFileOrderToSystemOrderMacro(int, &magicNumber, 1);

  // Find info.. : Number of Row , Nb of Columns
  short NbCol;

  m_Datafile.seekg(ONERA_HEADER_LENGTH + 2, std::ios::beg);
  m_Datafile.read((char*) (&NbCol), 2);
  otbSwappFileOrderToSystemOrderMacro(short, &NbCol, 1);

  m_Datafile.seekg(0, std::ios::end);
  long gcountHead = static_cast<long>(ONERA_HEADER_LENGTH + 2 * 4 * NbCol);
  long gcount     = static_cast<long>(m_Datafile.tellg());

  // Defining the image size:
  m_width = static_cast<int> (NbCol);
  m_height = static_cast<int> ((gcount - gcountHead) / (4 * 2 * NbCol));

  if ((m_width == 0) || (m_height == 0))
    {
    itkExceptionMacro(<< "Unknown image dimension");
    }
  else
    {
    // Set image dimensions into IO
    m_Dimensions[0] = m_width;
    m_Dimensions[1] = m_height;
    }

  this->SetNumberOfDimensions(2);

  otbMsgDebugMacro(<< "Driver to read: ONERA");
  otbMsgDebugMacro(<< "         Read  file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         PixelType          : " << this->GetPixelTypeAsString(this->GetPixelType()));
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentTypeAsString(this->GetComponentType()));
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         BytePerPixel       : " << m_BytePerPixel);
  otbMsgDebugMacro(<< "         Host byte order    : " << this->GetByteOrderAsString(m_ByteOrder));
  otbMsgDebugMacro(<< "         File byte order    : " << this->GetByteOrderAsString(m_FileByteOrder));

}

bool ONERAImageIO::OpenOneraDataFileForWriting(const char* filename)
{
  // Make sure that we have a file to
  std::string lFileName(filename);
  if (lFileName.empty())
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if (m_Datafile.is_open())
    {
    m_Datafile.close();
    }
  const std::string DataFileName = System::GetRootName(filename) + ".dat";

  // Open the new file for reading

  // Actually open the file
  m_Datafile.open(DataFileName.c_str(),  std::ios::out | std::ios::trunc | std::ios::binary);
  if (m_Datafile.fail())
    {
    otbMsgDebugMacro(<< "ONERAImageIO::OpenOneraDataFileForWriting() failed data open ! ");
    return false;
    }
  return true;
}

bool ONERAImageIO::OpenOneraHeaderFileForWriting(const char* filename)
{
  // Make sure that we have a file to
  std::string lFileName(filename);
  if (lFileName.empty())
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    return false;
    }

  // Close file from any previous image
  if (m_Headerfile.is_open())
    {
    m_Headerfile.close();
    }
  const std::string HeaderFileName = System::GetRootName(filename) + ".ent";

  // Open the new file for reading
  // Actually open the file
  m_Headerfile.open(HeaderFileName.c_str(),  std::ios::out | std::ios::trunc | std::ios::binary);
  if (m_Headerfile.fail())
    {
    otbMsgDebugMacro(<< "ONERAImageIO::OpenOneraHeaderFileForWriting() failed header open ! ");
    return false;
    }
  return true;
}

bool ONERAImageIO::CanWriteFile(const char* FileNameToWrite)
{
  std::string filename(FileNameToWrite);

  const std::string HeaderFileName = System::GetRootName(filename) + ".ent";
  const std::string DataFileName = System::GetRootName(filename) + ".dat";

  if (filename == HeaderFileName)
    {
    return true;
    }
  else
    {
    return false;
    }
}

void ONERAImageIO::Write(const void* buffer)
{
  if (m_FlagWriteImageInformation == true)
    {
    this->InternalWriteImageInformation();
    m_FlagWriteImageInformation = false;
    }

  unsigned long step = this->GetNumberOfComponents() * 2;

  unsigned int lNbLines   = this->GetIORegion().GetSize()[1];
  unsigned int lNbColumns = this->GetIORegion().GetSize()[0];
  int          lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int          lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " ONERAImageIO::Write()  ");
  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components  : " << this->GetNumberOfComponents());

  // Special case: we check if the region to write is the same as the whole image
  // We begin with a zero offset when we are not in streaming
  if ((lNbLines == m_Dimensions[1]) && (lNbColumns == m_Dimensions[0]))
    {
    otbMsgDevMacro(<< "Force l'offset de l'IORegion a 0");
    lFirstLine = 0;
    lFirstColumn = 0;
    }

  std::streamsize numberOfBytesPerLines = step * lNbColumns * m_BytePerPixel;
  std::streamoff  headerLength = ONERA_HEADER_LENGTH + numberOfBytesPerLines;
  std::streamoff  offset;
  unsigned long   numberOfBytesRegion = step * m_BytePerPixel * lNbColumns * lNbLines;

  char *tempmemory = new char[numberOfBytesRegion];
  memcpy(tempmemory, buffer, numberOfBytesRegion);

  for (unsigned int LineNo = lFirstLine; LineNo < lFirstLine + lNbLines; LineNo++)
    {
    char* value = tempmemory + numberOfBytesPerLines * (LineNo - lFirstLine);

    offset  =  headerLength + static_cast<std::streamoff>(numberOfBytesPerLines) * static_cast<std::streamoff>(LineNo);
    offset +=  static_cast<std::streamoff>(m_BytePerPixel * lFirstColumn);
    m_Datafile.seekp(offset, std::ios::beg);
    m_Datafile.write(static_cast<char *>(value), numberOfBytesPerLines);
    }

  delete[] tempmemory;
  tempmemory = NULL;
}

// To be consistent with the behavior of GDALImageIO
void ONERAImageIO::WriteImageInformation()
{

}

void ONERAImageIO::InternalWriteImageInformation()
{
    if (!this->OpenOneraHeaderFileForWriting(m_FileName.c_str()))
      {
      itkExceptionMacro(<< "Cannot read requested file");
      }

    if (!this->OpenOneraDataFileForWriting(m_FileName.c_str()))
      {
      itkExceptionMacro(<< "Cannot read requested file");
      }

    /*-------- This part deals with writing header information ------ */
    const std::string DataFileName = System::GetRootName(m_FileName.c_str()) + ".dat";

    m_Headerfile << "#                    [fichier en-tete produit par les routines de otb (Orfeo ToolBox) ]" <<
    std::endl;
    m_Headerfile << "# Nom du look :" << std::endl;
    m_Headerfile << "Look.dat= \t" << DataFileName.c_str() <<  std::endl;
    m_Headerfile << std::endl;
    m_Headerfile << "# Structure du fichier et codage des pixels :" << std::endl;
    m_Headerfile << "# 4 octets precedent la premiere ligne : ils correspondent a un nombre magique [I4= 33554433] " <<
    std::endl;
    m_Headerfile << "# [dans ordre LSBfirst = big-endian]" << std::endl;

    std::string sPixelType("cmplx_real_4");
    if ((m_PixelType == COMPLEX) && (m_ComponentType == CFLOAT))
      {
      sPixelType = "cmplx_real_4";
      }
    else
      {
      itkExceptionMacro(<< "data format not supported by OTB (only 'complex_real_4' is available) : ");
      }

    m_Headerfile << "Format_valeurs_look=    \t" << sPixelType << std::endl;
    m_Headerfile << "Nb_case_par_ligne_look= \t" << m_Dimensions[0] << std::endl;
    m_Headerfile << "Nb_ligne_look=          \t" << m_Dimensions[1] <<
    " + 1 ligne en-tete en binaire (entiers 16 bit) " << std::endl;

    // write magic_number
    int   magicNumber = ONERA_MAGIC_NUMBER;
    short NbCol = static_cast<short>(m_Dimensions[0]);
    short NbRow = static_cast<short>(m_Dimensions[1]);
    int   ByteSizeCol = NbCol * 4 * 2;

  //  itk::ByteSwapper< int>::SwapFromSystemToLittleEndian(&magicNumber);
    m_Datafile.seekp(0, std::ios::beg);
    m_Datafile.write((char*) (&magicNumber), 4);

    char * tab = new char[ByteSizeCol];
    for (int i = 0; i < (NbRow + 1); ++i)
      {
      m_Datafile.write((char*) (tab), ByteSizeCol);
      }
    delete[] tab;

    // write number of columns
  //  itk::ByteSwapper<short>::SwapFromSystemToLittleEndian(&NbCol);

    m_Datafile.seekp(ONERA_HEADER_LENGTH + 2, std::ios::beg);
    m_Datafile.write((char*) (&NbCol), 2);

    otbMsgDebugMacro(<< "Driver to write: ONERA");
    otbMsgDebugMacro(<< "         Write file         : " << m_FileName);
    otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
    otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
    otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
    otbMsgDebugMacro(<< "         BytePerPixel       : " << m_BytePerPixel);
    otbMsgDebugMacro(<< "         Host byte order    : " << this->GetByteOrderAsString(m_ByteOrder));
}

} // end namespace otb
