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
#include "otbLUMImageIO.h"

#include "itkByteSwapper.h"
#include "otbSystem.h"
#include "itksys/SystemTools.hxx"
#include "otbMacro.h"


namespace otb
{

LUMImageIO::LUMImageIO()
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

  //Definition of CAI image type
  m_CaiLumTyp.clear();
  m_CaiLumTyp.push_back("BI");
  m_CaiLumTyp.push_back("LI");
  m_CaiLumTyp.push_back("BU");
  m_CaiLumTyp.push_back("LU");
  m_CaiLumTyp.push_back("FLOA");
  m_CaiLumTyp.push_back("FLOL");
  m_CaiLumTyp.push_back("R8B");
  m_CaiLumTyp.push_back("R8L");

  this->AddSupportedWriteExtension(".lum");
  this->AddSupportedWriteExtension(".LUM");

  this->AddSupportedReadExtension(".lum");
  this->AddSupportedReadExtension(".LUM");

}

LUMImageIO::~LUMImageIO()
{
  if (m_File.is_open())
    {
    m_File.close();
    }
}

bool LUMImageIO::CanReadFile(const char* filename)
{
  std::string lFileName(filename);
  // Test the extension
  std::string extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if ((extension != ".LUM") && (extension != ".lum"))
    {
    return false;
    }
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true)
    {
    return false;
    }
  if (m_File.is_open())
    {
    m_File.close();
    }

  std::fstream header_file;
  header_file.open(filename,  std::ios::in | std::ios::binary);
  if (header_file.fail())
    {
    otbMsgDevMacro(<< "LUMImageIO::CanReadFile() failed header open ! ");
    return false;
    }

  //Read header information
  bool lResult = InternalReadHeaderInformation(header_file, false);
  header_file.close();
  return (lResult);
}

// Used to print information about this object
void LUMImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void LUMImageIO::ReadVolume(void*)
{
}

// Read image
void LUMImageIO::Read(void* buffer)
{
  char * p = static_cast<char *>(buffer);

  int lNbLines   = this->GetIORegion().GetSize()[1];
  int lNbColumns = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " LUMImageIO::Read()  ");
  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components       : " << this->GetNumberOfComponents());

  std::streamoff  headerLength = static_cast<std::streamoff>(this->GetComponentSize() * m_Dimensions[0]);
  std::streamoff  numberOfBytesPerLines = headerLength;
  std::streamoff  offset;
  std::streamsize numberOfBytesToBeRead = static_cast<std::streamsize>(this->GetComponentSize() * lNbColumns);
  std::streamsize numberOfBytesRead;
  std::streamsize cpt = 0;
  for (int LineNo = lFirstLine; LineNo < lFirstLine + lNbLines; LineNo++)
    {
    offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
    offset +=  static_cast<std::streamoff>(this->GetComponentSize() * lFirstColumn);
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
      itkExceptionMacro(<< "LUMImageIO::Read() Can Read the specified Region"); // read failed
      }
    cpt += numberOfBytesToBeRead;
    }

  unsigned long numberOfPixelsPerLines = lNbLines * lNbColumns;
  // Swap bytes if necessary
  if (0) {}
  otbSwappFileToSystemMacro(unsigned short, USHORT, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(short, SHORT, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(char, CHAR, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(unsigned char, UCHAR, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(unsigned int, UINT, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(int, INT, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(long, LONG, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(unsigned long, ULONG, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(float, FLOAT, buffer, numberOfPixelsPerLines)
  otbSwappFileToSystemMacro(double, DOUBLE, buffer, numberOfPixelsPerLines)
  else
    {
    itkExceptionMacro(<< "LUMImageIO::Read() undefined component type! ");
    }
}

void LUMImageIO::ReadImageInformation()
{
  if (m_File.is_open())
    {
    m_File.close();
    }

  m_File.open(m_FileName.c_str(),  std::ios::in | std::ios::binary);
  if (m_File.fail())
    {
    itkExceptionMacro(<< "LUMImageIO::ReadImageInformation() failed header open ! ");
    }

  //Read header information
  InternalReadHeaderInformation(m_File, true);

  otbMsgDebugMacro(<< "Driver to read: LUM");
  otbMsgDebugMacro(<< "         Read  file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

bool LUMImageIO::InternalReadHeaderInformation(std::fstream& file, const bool reportError)
{

  std::string lStrTypeImage;
  std::string lStrCodePix;
  std::string lStrTypeMachine;
  int         lNbBits(-1);
  char        TypeCode[5];
  file.seekg(8, std::ios::beg);
  file.read((char*) (TypeCode), 4);
  TypeCode[4] = '\0';
  int lTaille = CaiGetTypeLum(TypeCode,
                              lStrTypeImage,
                              lNbBits,
                              lStrCodePix);
  if (lTaille == -1)
    {
    if (reportError == true)
      {
      itkExceptionMacro(<< "LUM : bad read of header information");
      }
    else
      {
      return false;
      }
    }
  //Set file byte order
  if (lStrTypeImage == "BE")
    {
    m_FileByteOrder = BigEndian;
    }
  else
    {
    m_FileByteOrder = LittleEndian;
    }

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
      itkExceptionMacro(<< "LUM : impossible to determine CodePix information of the image");
      }
    else
      {
      return false;
      }
    }

  int NbCol(-1);
  int NbLig(-1);

  //Read image dimensions
  file.seekg(0, std::ios::beg);
  file.read((char*) (&NbCol), 4);
  file.read((char*) (&NbLig), 4);

  //Swapp if necessary
  otbSwappFileOrderToSystemOrderMacro(int, &NbCol, 1);
  otbSwappFileOrderToSystemOrderMacro(int, &NbLig, 1);

  //Initialization of image information
  m_Dimensions[0] = NbCol;
  m_Dimensions[1] = NbLig;
  this->SetNumberOfComponents(1);
  this->SetFileTypeToBinary();
  this->SetNumberOfDimensions(2);
  return (true);
}

bool LUMImageIO::CanWriteFile(const char* filename)
{
  std::string lFileName(filename);
  std::string extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if ((extension != ".LUM") && (extension != ".lum"))
    {
    return false;
    }
  if (itksys::SystemTools::FileIsDirectory(lFileName.c_str()) == true)
    {
    return false;
    }
  const std::string Extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  if ((Extension == ".lum") || (Extension == ".LUM"))
    {
    return true;
    }
  else
    {
    return false;
    }
}

void LUMImageIO::Write(const void* buffer)
{
  if (m_FlagWriteImageInformation == true)
    {
    this->WriteImageInformation();
    m_FlagWriteImageInformation = false;
    }

  unsigned long lNbLines   = this->GetIORegion().GetSize()[1];
  unsigned long lNbColumns = this->GetIORegion().GetSize()[0];
  unsigned long lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int           lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  if ((lNbLines == m_Dimensions[1]) && (lNbColumns == m_Dimensions[0]))
    {
    otbMsgDevMacro(<< "Forcing offset to [0, 0]");
    lFirstLine = 0;
    lFirstColumn = 0;
    }

  otbMsgDevMacro(<< " LUMImageIO::Write()  ");
  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components       : " << this->GetNumberOfComponents());
  otbMsgDevMacro(<< " GetComponentSize       : " << this->GetComponentSize());

  std::streamoff  numberOfBytesPerLines = this->GetComponentSize() * m_Dimensions[0];
  std::streamoff  headerLength = numberOfBytesPerLines;
  std::streamoff  offset;
  std::streamsize numberOfBytesToBeWrite = this->GetComponentSize() * lNbColumns;
  std::streamsize cpt = 0;

  const char * p = static_cast<const char *>(buffer);

  for (unsigned long LineNo = lFirstLine; LineNo < lFirstLine + lNbLines; LineNo++)
    {
    offset  =  headerLength + numberOfBytesPerLines * static_cast<std::streamoff>(LineNo);
    offset +=  static_cast<std::streamoff>(this->GetComponentSize() * lFirstColumn);
    m_File.seekp(offset, std::ios::beg);
    m_File.write(static_cast<const char *>(p + cpt), numberOfBytesToBeWrite);
    cpt += numberOfBytesToBeWrite;
    }
}

void LUMImageIO::WriteImageInformation()
{

  if (m_FileName == "")
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    }
  if (CanWriteFile(m_FileName.c_str()) == false)
    {
    itkExceptionMacro(<< "The file " << m_FileName.c_str() << " is not defined as a LUM file");
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

  //Writing header information
  std::streamsize headerLength = static_cast<std::streamsize>(this->GetComponentSize() * m_Dimensions[0]);

  m_File.seekp(0, std::ios::beg);
  char* value = new char[headerLength];
  //Write Header line and all file (whitout information)
  for (unsigned int numLigne = 0; numLigne < (m_Dimensions[1] + 1); numLigne++)
    {
    m_File.write(value, headerLength);
    }
  delete[] value;
  value = NULL;
  //Set m_TypeLum
  if (0) {}
  otbSetTypeLumMacro(CHAR,   "08BI", "08LI")
  otbSetTypeLumMacro(UCHAR,  "08BU", "08LU")
  otbSetTypeLumMacro(SHORT,  "16BI", "16LI")
  otbSetTypeLumMacro(USHORT, "16BU", "16LU")
  otbSetTypeLumMacro(INT,    "32BI", "32LI")
  otbSetTypeLumMacro(UINT,   "32BU", "32LU")
  otbSetTypeLumMacro(FLOAT,  "FLOA", "FLOL")
  otbSetTypeLumMacro(DOUBLE, "R8B ", "R8L ")
  if ((this->GetComponentType() == LONG) || (this->GetComponentType() == ULONG))
    {
    itkExceptionMacro(<< "LUM format doesn't accept LONG or ULONG data type");
    }

  m_File.seekp(0, std::ios::beg);
  //Write image size
  m_File.write((char*) (&(m_Dimensions[0])), 4);
  m_File.write((char*) (&(m_Dimensions[1])), 4);
  m_File.write((char*) (m_TypeLum.data()), 4);

  otbMsgDebugMacro(<< "Driver to write: LUM");
  otbMsgDebugMacro(<< "         Write file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         Type Lum           : " << m_TypeLum);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

int LUMImageIO::CaiGetTypeLum(const char *          type_code,
                              std::string&   str_sens_code,
                              int&           inbbits,
                              std::string&   str_cod_pix)
{
  unsigned int ind;  /* indice de boucle sur les types reconnus */
  int          trouve, icr, taille; /* indice pour la recherche                */
  int          mod2; /* modulo2           */
  char*        pch0;
  char         sens_code[3]; /* type code+ordre rangement octets dans entete*/
  char         cod_pix[5];
  sens_code[0] = '\0';
  cod_pix[0] = '\0';

  /* Initializations diverses */
  trouve = -1;
  taille = -1;
  int nbbits = -1;
  ind = 0;
  pch0 = NULL;

  std::string str_type_code(type_code);
  while ((trouve == -1) && (ind < m_CaiLumTyp.size()))
    {
    if ( str_type_code.find(m_CaiLumTyp[ind]) != std::string::npos )
      {
      trouve = ind;
      }
    else
      {
      ind = ind + 1;
      }
    }

  //CAI source code
  if (trouve != -1)
    {
    mod2 = ind % 2;
    if (mod2 == 0) sprintf(sens_code, "BE");
    else sprintf(sens_code, "LE");

    if (ind < 4)
      {
      icr = sscanf(type_code, "%d", &nbbits);
      if (icr == 0) nbbits = -1;
      else
        {
        trouve = (int) (nbbits / 8);
        if ((nbbits % 8) != 0) taille = trouve + 1;
        else taille  = trouve;
        }
      pch0 = const_cast<char *>(strstr(type_code, "U"));
      if (taille == 1)
        {
        if (pch0 == NULL) sprintf(cod_pix, "OCT");
        else sprintf(cod_pix, "UOCT");
        }
      else if (taille == 2)
        {
        if (pch0 == NULL) sprintf(cod_pix, "I2");
        else sprintf(cod_pix, "UI2");
        }
      else if (taille == 4)
        {
        if (pch0 == NULL) sprintf(cod_pix, "I4");
        else sprintf(cod_pix, "UI4");
        }
      else taille = -1;
      }
    else if  ((ind == 4) || (ind == 5))
      {
      taille = 4;
      nbbits = 32;
      sprintf(cod_pix, "R4");

      }
    else if ((ind == 6) || (ind == 7))
      {
      taille = 8;
      nbbits = 64;
      sprintf(cod_pix, "R8");
      }
    }

  str_cod_pix = std::string(cod_pix);
  str_sens_code = std::string(sens_code);

  inbbits = nbbits;
  return (taille);
}

} // end namespace otb
