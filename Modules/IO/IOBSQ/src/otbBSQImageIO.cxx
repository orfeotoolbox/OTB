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
#include "otbBSQImageIO.h"

#include <fstream>
#include <iostream>

#include "itkByteSwapper.h"
#include "otbSystem.h"
#include "itksys/SystemTools.hxx"

#include "otbMacro.h"


namespace otb
{

BSQImageIO::BSQImageIO()
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

  m_FileByteOrder = m_ByteOrder;
  m_TypeBsq = "";
  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to [0.5 , 0.5]
  // (consistency between ImageIO, see Mantis #942)
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;
  m_ChannelsFile = NULL;
  m_FlagWriteImageInformation = true;

  this->AddSupportedWriteExtension(".hd");
  this->AddSupportedWriteExtension(".HD");

  this->AddSupportedReadExtension(".hd");
  this->AddSupportedReadExtension(".HD");

}

BSQImageIO::~BSQImageIO()
{
  if (m_HeaderFile.is_open())
    {
    m_HeaderFile.close();
    }
  if (m_ChannelsFile !=  NULL)
    {
    for (unsigned int numComponent = 0; numComponent < this->GetNumberOfComponents(); numComponent++)
      {
      if (m_ChannelsFile[numComponent].is_open())
        {
        m_ChannelsFile[numComponent].close();
        }
      }
    delete[] m_ChannelsFile;
    }
}

bool BSQImageIO::CanReadFile(const char* filename)
{
  std::fstream header_file;
  std::string  lFileName(filename);
  std::string  extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if ((extension != ".HD") && (extension != ".hd"))
    {
    return false;
    }
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true)
    {
    return false;
    }
  if (itksys::SystemTools::LowerCase(itksys::SystemTools::GetFilenameLastExtension(lFileName)) != ".hd")
    {
    return false;
    }

  header_file.open(lFileName.c_str(),  std::ios::in);
  if (header_file.fail())
    {
    otbMsgDevMacro(<< "BSQImageIO::CanReadFile() failed header open ! ");
    return false;
    }
  //Read header information
  bool lResult = InternalReadHeaderInformation(lFileName, header_file, false);
  header_file.close();
  return (lResult);
}

// Used to print information about this object
void BSQImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void BSQImageIO::ReadVolume(void*)
{
}

// Read image
void BSQImageIO::Read(void* buffer)
{
  unsigned long step = this->GetNumberOfComponents();
  char *        p = static_cast<char *>(buffer);

  int lNbLines   = this->GetIORegion().GetSize()[1];
  int lNbColumns = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " BSQImageIO::Read()  ");
  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components       : " << this->GetNumberOfComponents());

  std::streamoff  headerLength(0);
  std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>(this->GetComponentSize() * m_Dimensions[0]);
  std::streamoff  offset;
  std::streamsize numberOfBytesToBeRead = this->GetComponentSize() * lNbColumns;
  std::streamsize numberOfBytesRead;
  unsigned long   cpt = 0;

  // Update the step variable
  step = step * (unsigned long) (this->GetComponentSize());

  char * value = new char[numberOfBytesToBeRead];
  if (value == NULL)
    {
    itkExceptionMacro(<< "BSQImageIO::Read(): Bad alloc");
    return;
    }

  otbMsgDevMacro(<< " sizeof(streamsize)    : " << sizeof(std::streamsize));
  otbMsgDevMacro(<< " sizeof(streampos)     : " << sizeof(std::streampos));
  otbMsgDevMacro(<< " sizeof(streamoff)     : " << sizeof(std::streamoff));
  otbMsgDevMacro(<< " sizeof(std::ios::beg) : " << sizeof(std::ios::beg));
  otbMsgDevMacro(<< " sizeof(size_t)        : " << sizeof(size_t));
  otbMsgDevMacro(<< " sizeof(unsigned long) : " << sizeof(unsigned long));

  for (unsigned int nbComponents = 0; nbComponents < this->GetNumberOfComponents(); ++nbComponents)
    {
    cpt = (unsigned long) (nbComponents) * (unsigned long) (this->GetComponentSize());
    //Read region of the channel
    for (int LineNo = lFirstLine; LineNo < lFirstLine + lNbLines; LineNo++)
      {
      offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
      offset +=  static_cast<std::streamoff>(this->GetComponentSize() * lFirstColumn);
      m_ChannelsFile[nbComponents].seekg(offset, std::ios::beg);
      //Read a line
      m_ChannelsFile[nbComponents].read(static_cast<char *>(value), numberOfBytesToBeRead);
      numberOfBytesRead = m_ChannelsFile[nbComponents].gcount();
#ifdef __APPLE_CC__
      // fail() is broken in the Mac. It returns true when reaches eof().
      if (numberOfBytesRead != numberOfBytesToBeRead)
#else
      if ((numberOfBytesRead != numberOfBytesToBeRead)  || m_ChannelsFile[nbComponents].fail())
#endif
        {
        itkExceptionMacro(<< "BSQImageIO::Read() Can Read the specified Region"); // read failed
        }
//                        cpt = (unsigned long )(nbComponents)* (unsigned long)(this->GetComponentSize()) + numberOfBytesToBeRead * this->GetNumberOfComponents() * LineNo;
//                        cpt = (unsigned long )(nbComponents)* (unsigned long)(this->GetComponentSize()) + numberOfBytesToBeRead * this->GetNumberOfComponents();
      for (std::streamsize i = 0;
           i < numberOfBytesToBeRead;
           i = i + static_cast<std::streamsize>(this->GetComponentSize()))
        {
        memcpy((void*) (&(p[cpt])), (const void*) (&(value[i])), (size_t) (this->GetComponentSize()));
        cpt += step;
        }
      }
    }
  unsigned long numberOfPixelsOfRegion = lNbLines * lNbColumns * this->GetNumberOfComponents();

  delete[] value;

  // Swap bytes if necessary
  if (0) {}
  otbSwappFileToSystemMacro(unsigned short, USHORT, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(short, SHORT, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(char, CHAR, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(unsigned char, UCHAR, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(unsigned int, UINT, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(int, INT, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(long, LONG, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(unsigned long, ULONG, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(float, FLOAT, buffer, numberOfPixelsOfRegion)
  otbSwappFileToSystemMacro(double, DOUBLE, buffer, numberOfPixelsOfRegion)
  else
    {
    itkExceptionMacro(<< "BSQImageIO::Read() undefined component type! ");
    }
}

void BSQImageIO::ReadImageInformation()
{
  if (m_HeaderFile.is_open())
    {
    m_HeaderFile.close();
    }
  m_HeaderFile.open(m_FileName.c_str(),  std::ios::in);
  if (m_HeaderFile.fail())
    {
    itkExceptionMacro(<< "BSQImageIO::ReadImageInformation() failed header open ! ");
    }

  //Read header information
  InternalReadHeaderInformation(m_FileName, m_HeaderFile, true);

  otbMsgDebugMacro(<< "Driver to read: BSQ");
  otbMsgDebugMacro(<< "         Read  file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

bool BSQImageIO::InternalReadHeaderInformation(const std::string& file_name, std::fstream& file, const bool reportError)
{

  std::string lString;
  std::string lStrCodePix;
  //Read TYPE information
  file >> lString;
  lString = itksys::SystemTools::UpperCase(lString);
  if (lString != "TYPE")
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "BSQ : the first line of the header file must be contains 'TYPE' caracters.");
      }
    else
      {
      return false;
      }
    }
  file >> lStrCodePix;
  lStrCodePix = itksys::SystemTools::UpperCase(lStrCodePix);
  if (lStrCodePix == "OCT")
    {
    SetComponentType(CHAR);
    }
  else if (lStrCodePix == "UOCT")
    {
    SetComponentType(UCHAR);
    }
  else if (lStrCodePix == "I2")
    {
    SetComponentType(SHORT);
    }
  else if (lStrCodePix == "UI2")
    {
    SetComponentType(USHORT);
    }
  else if (lStrCodePix == "I4")
    {
    SetComponentType(INT);
    }
  else if (lStrCodePix == "UI4")
    {
    SetComponentType(UINT);
    }
  else if (lStrCodePix == "R4")
    {
    SetComponentType(FLOAT);
    }
  else if (lStrCodePix == "R8")
    {
    SetComponentType(DOUBLE);
    }
  else
    {
    if (reportError == true)
      {
      itkExceptionMacro(
        << "BSQ : the value type '" << lStrCodePix <<
        "' (second line) set in the header file is not reconized as correct value.");
      }
    else
      {
      return false;
      }
    }
  //Read LABEL information
  file >> lString;
  lString = itksys::SystemTools::UpperCase(lString);
  if (lString != "LABEL")
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "BSQ : the third line of the header file must be contains 'LABEL' caracters.");
      }
    else
      {
      return false;
      }
    }
  while ((lString != "CHANNELS") || (file.eof()))
    {
    file >> lString;
    lString = itksys::SystemTools::UpperCase(lString);
    }
  if (lString != "CHANNELS")
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "BSQ : 'CHANNELS' keyword is not find in the header file.");
      }
    else
      {
      return false;
      }
    }
  unsigned int lNbChannels;
  file >> lNbChannels;
  this->SetNumberOfComponents(lNbChannels);

  //Read LINES information
  file >> lString;
  lString = itksys::SystemTools::UpperCase(lString);
  if (lString != "LINES")
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "BSQ : 'LINES' keyword is not find in the header file.");
      }
    else
      {
      return false;
      }
    }
  file >> m_Dimensions[1];
  //Read COLUMNS information
  file >> lString;
  lString = itksys::SystemTools::UpperCase(lString);
  if (lString != "COLUMNS")
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "BSQ : 'COLUMNS' keyword is not find in the header file.");
      }
    else
      {
      return false;
      }
    }
  file >> m_Dimensions[0];

  //Read "BITS PER PIXEL" information
  file >> lString;
  std::string lStrBitsPerPixels(lString);
  file >> lString;
  lStrBitsPerPixels = lStrBitsPerPixels + " " + lString;
  file >> lString;
  lStrBitsPerPixels = lStrBitsPerPixels + " " + lString;
  lStrBitsPerPixels = itksys::SystemTools::UpperCase(lStrBitsPerPixels);
  if (lStrBitsPerPixels != "BITS PER PIXEL")
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "BSQ : 'BITS PER PIXEL' keyword is not find in the header file.");
      }
    else
      {
      return false;
      }
    }
  int lNbBitsPerPixels;
  file >> lNbBitsPerPixels;

  //Read "SENSCODAGE" information (optionnal)
  file >> lString;
  if (lString.empty() == false)
    {
    lString = itksys::SystemTools::UpperCase(lString);
    if (lString == "SENSCODAGE")
      {
      file >> lString;
      lString = itksys::SystemTools::UpperCase(lString);
      if (lString == "INTEL")
        {
        m_FileByteOrder = LittleEndian;
        }
      else if (lString == "IEEE")
        {
        m_FileByteOrder = BigEndian;
        }
      else
        {
        if (reportError == true)
          {
          itkExceptionMacro(
            << "BSQ : the value SENSCODAGE '" << lString <<
            "' set in the header file is not reconized as correct value. Possible values are INTEL or IEEE");
          }
        else
          {
          return false;
          }
        }
      }
    }
  file.close();

  //Define channels file name
  std::string lRootName = System::GetRootName(file_name);
  m_ChannelsFileName.clear();
  for (unsigned int i = 0; i < this->GetNumberOfComponents(); ++i)
    {
    std::ostringstream lStream;
    lStream << lRootName << ".c" << i + 1;
    m_ChannelsFileName.push_back(lStream.str());
    }

  m_ChannelsFile = new std::fstream[this->GetNumberOfComponents()];

  //Try to open channels file
  for (unsigned int channels = 0; channels < m_ChannelsFileName.size(); ++channels)
    {
    m_ChannelsFile[channels].open(m_ChannelsFileName[channels].c_str(),  std::ios::in | std::ios::binary);
    if (m_ChannelsFile[channels].fail())
      {
      if (reportError == true)
        {
        itkExceptionMacro(<< "BSQ : impossible to find the file <" << m_ChannelsFileName[channels] << ">.");
        }
      else
        {
        return false;
        }
      }
    }
  this->SetFileTypeToBinary();
  this->SetNumberOfDimensions(2);

  return (true);
}

bool BSQImageIO::CanWriteFile(const char* filename)
{
  std::string lFileName(filename);
  std::string extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if ((extension != ".HD") && (extension != ".hd"))
    {
    return false;
    }
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true)
    {
    return false;
    }
  const std::string Extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if ((Extension == ".hd") || (Extension == ".HD"))
    {
    return true;
    }
  else
    {
    return false;
    }
}

void BSQImageIO::Write(const void* buffer)
{
  if (m_FlagWriteImageInformation == true)
    {
    this->WriteImageInformation();
    m_FlagWriteImageInformation = false;
    }

  unsigned long step = this->GetNumberOfComponents();
  unsigned int  lNbLines   = this->GetIORegion().GetSize()[1];
  unsigned int  lNbColumns = this->GetIORegion().GetSize()[0];
  int           lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int           lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  // Special case: check that the region to write is the same size as the entire
  // image. Start at offset 0 (when no streaming)
  if ((lNbLines == m_Dimensions[1]) && (lNbColumns == m_Dimensions[0]))
    {
    otbMsgDevMacro(<< "Forcing offset to [0, 0]");
    lFirstLine = 0;
    lFirstColumn = 0;
    }

  otbMsgDevMacro(<< " BSQImageIO::Write()  ");
  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components       : " << this->GetNumberOfComponents());
  otbMsgDevMacro(<< " GetComponentSize       : " << this->GetComponentSize());

  std::streamoff  headerLength(0);
  std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>(this->GetComponentSize() * m_Dimensions[0]);
  std::streamsize numberOfBytesToBeWrite = static_cast<std::streamsize>(this->GetComponentSize() * lNbColumns);
  std::streamoff  offset = 0;
  unsigned long   cpt = 0;

  // Update the step variable
  step = step * (unsigned long) (this->GetComponentSize());
  const char * p = static_cast<const char *>(buffer);

  char* value = new char[numberOfBytesToBeWrite];
  if (value == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  for (unsigned int nbComponents = 0; nbComponents < this->GetNumberOfComponents(); ++nbComponents)
    {
    cpt = (unsigned long) (nbComponents) * (unsigned long) (this->GetComponentSize());
    //Read region of the channel
    for (unsigned int LineNo = lFirstLine; LineNo < lFirstLine + lNbLines; LineNo++)
      {
      for (std::streamsize i = 0;
           i < numberOfBytesToBeWrite;
           i = i + static_cast<std::streamsize>(this->GetComponentSize()))
        {
        memcpy((void*) (&(value[i])), (const void*) (&(p[cpt])), (size_t) (this->GetComponentSize()));
        cpt += step;
        }

      offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
      offset +=  static_cast<std::streamoff>(this->GetComponentSize() * lFirstColumn);
      m_ChannelsFile[nbComponents].seekp(offset, std::ios::beg);
      //Write a line
      m_ChannelsFile[nbComponents].write(static_cast<char *>(value), numberOfBytesToBeWrite);
      }
    }
  delete[] value;
}

void BSQImageIO::WriteImageInformation()
{
  if (m_FileName == "")
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    }
  if (CanWriteFile(m_FileName.c_str()) == false)
    {
    itkExceptionMacro(<< "The file " << m_FileName.c_str() << " is not defined as a BSQ file");
    }
  // Close file from any previous image
  if (m_HeaderFile.is_open())
    {
    m_HeaderFile.close();
    }

  // Open the new file for writing
  // Actually open the file
  m_HeaderFile.open(m_FileName.c_str(),  std::ios::out | std::ios::trunc);
  if (m_HeaderFile.fail())
    {
    itkExceptionMacro(<< "Cannot write requested file " << m_FileName.c_str() << ".");
    }

  if (0) {}
  otbSetTypeBsqMacro(CHAR,   "OCT")
  otbSetTypeBsqMacro(UCHAR,  "UOCT")
  otbSetTypeBsqMacro(SHORT,  "I2")
  otbSetTypeBsqMacro(USHORT, "UI2")
  otbSetTypeBsqMacro(INT,    "I4")
  otbSetTypeBsqMacro(UINT,   "UI4")
  otbSetTypeBsqMacro(FLOAT,  "R4")
  otbSetTypeBsqMacro(DOUBLE, "R8")
  else
    {
    itkExceptionMacro(<< "BSQ format doesn't reconized (TYPE).");
    }

  std::string lString;
  //Write TYPE information
  m_HeaderFile << "TYPE" << std::endl;
  m_HeaderFile << m_TypeBsq << std::endl;

  //Write LABEL information
  m_HeaderFile << "LABEL" << std::endl;
  m_HeaderFile << m_TypeBsq << " - This BSQ image file was producted by OTB software." << std::endl;

  //Write CHANNELS information
  m_HeaderFile << "CHANNELS" << std::endl;
  m_HeaderFile << this->GetNumberOfComponents() << std::endl;

  //Write LINES information
  m_HeaderFile <<  "LINES" << std::endl;
  m_HeaderFile << m_Dimensions[1] << std::endl;
  //Write COLUMNS information
  m_HeaderFile <<  "COLUMNS" << std::endl;
  m_HeaderFile << m_Dimensions[0] << std::endl;

  //Write "BITS PER PIXEL" information
  m_HeaderFile <<  "BITS PER PIXEL" << std::endl;
  m_HeaderFile << this->GetComponentSize() * 8 << std::endl;

  //Write "SENSCODAGE" information
  m_HeaderFile <<  "SENSCODAGE" << std::endl;
  if (m_ByteOrder == LittleEndian)
    {
    m_HeaderFile <<  "INTEL" << std::endl;
    }
  else
    {
    m_HeaderFile <<  "IEEE" << std::endl;
    }

  m_HeaderFile.close();

  //Create channels files

  //Define channels file name
  std::string lRootName = System::GetRootName(m_FileName);
  m_ChannelsFileName.clear();
  for (unsigned int i = 0; i < this->GetNumberOfComponents(); ++i)
    {
    std::ostringstream lStream;
    lStream << lRootName << ".c" << i + 1;
    m_ChannelsFileName.push_back(lStream.str());
    }

  //Allocate  buffer of stream file
  m_ChannelsFile = new std::fstream[this->GetNumberOfComponents()];

  //Try to open channels file
  for (unsigned int channels = 0; channels < m_ChannelsFileName.size(); ++channels)
    {
    m_ChannelsFile[channels].open(
      m_ChannelsFileName[channels].c_str(),  std::ios::out | std::ios::trunc | std::ios::binary);
    if (m_ChannelsFile[channels].fail())
      {
      itkExceptionMacro(<< "BSQ : unable to find the file <" << m_ChannelsFileName[channels] << ">");
      }
    }
  this->SetFileTypeToBinary();
  this->SetNumberOfDimensions(2);

  unsigned long headerLength = this->GetComponentSize() * m_Dimensions[0];
  char*         value = new char[headerLength];

  for (unsigned int channels = 0; channels < m_ChannelsFileName.size(); ++channels)
    {
    m_ChannelsFile[channels].seekp(0, std::ios::beg);
    //Write Header line and all file (whitout information)
    for (unsigned int numLigne = 0; numLigne < (m_Dimensions[1]); numLigne++)
      {
      m_ChannelsFile[channels].write(value, headerLength);
      }
    }

  delete[] value;

  otbMsgDebugMacro(<< "Driver to write: BSQ");
  otbMsgDebugMacro(<< "         Write file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         Type Bsq           : " << m_TypeBsq);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

} // end namespace otb
