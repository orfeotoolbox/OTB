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
#include "otbTileMapImageIO.h"

#include <cstring>
#include <cmath>

#include <iostream>
#include <fstream>


#include "itksys/SystemTools.hxx"
#include "otbSystem.h"
#include "otbMacro.h"

#include "otbGDALImageIO.h"

#include "itkTimeProbe.h"
#include "otbCurlHelper.h"

#include "otbImageKeywordlist.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

#include "otbLogo.inc"

namespace otb
{

TileMapImageIO::TileMapImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);

  // By default set pixel type to scalar.
  m_PixelType = SCALAR;

  // By default set component type to unsigned char
  m_ComponentType = UCHAR;
  m_UseCompression = false;
  m_CompressionLevel = 4; // Range 0-9; 0 = no file compression, 9 = maximum file compression

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to [0.5 , 0.5]
  // (consistency between ImageIO, see Mantis #942)
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;

  m_NbBands = 3;
  m_FlagWriteImageInformation = true;

  //Resolution depth
  m_Depth = 8;

  m_BytePerPixel = 1;
  m_TileSize = 256;

  m_UseCache = false;
  m_ServerName = "";
  m_CacheDirectory = ".";
  m_FileSuffix = "png";
  m_AddressMode = TileMapAdressingStyle::OSM;

  m_FileNameIsServerName = false;

  // Set maximum of connections to 10
  m_MaxConnect = 10;

  m_Curl = CurlHelper::New();

  this->AddSupportedWriteExtension(".otb");
  this->AddSupportedWriteExtension(".OTB");

  this->AddSupportedReadExtension(".otb");
  this->AddSupportedReadExtension(".OTB");

  m_TileMapSplitter = SplitterType::New();

  this->UseStreamedWritingOn();
  this->UseStreamedReadingOn();
}

TileMapImageIO::~TileMapImageIO()
{
}

// Tell only if the file can be read with TileMap.
bool TileMapImageIO::CanReadFile(const char* file)
{
  // First check the extension
  if (file == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  std::string            filename = file;
  std::string::size_type gmPos = filename.rfind(".otb");

  if ((gmPos != std::string::npos)
      && (gmPos == filename.length() - 4))
    {
    m_FileNameIsServerName = false;
    return true;
    }
  // Filename is http server
  else if (filename.find("http://") == 0 ||
           filename.find("https://") == 0)
    {
    m_FileNameIsServerName = true;
    // If the url is available
    if (m_Curl->TestUrlAvailability(filename))
      {
      return true;
      }
    itkDebugMacro(<< "Url is unavailable.");
    }
  return false;
}

// Used to print information about this object
void TileMapImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Compression Level : " << m_CompressionLevel << "\n";
}

// Read image with TileMap
void TileMapImageIO::Read(void* buffer)
{
  unsigned char * p = static_cast<unsigned char *>(buffer);
  if (p == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  int totLines   = this->GetIORegion().GetSize()[1];
  int totSamples = this->GetIORegion().GetSize()[0];
  int firstLine   = this->GetIORegion().GetIndex()[1];
  int firstSample = this->GetIORegion().GetIndex()[0];

  int nTilesX = (int) ceil(totSamples / static_cast<double>(m_TileSize)) + 1;
  int nTilesY = (int) ceil(totLines / static_cast<double>(m_TileSize)) + 1;

  // Clear vectors
  m_ListFilename.clear();
  m_ListURLs.clear();
  m_ListTiles.clear();


  //Read all the required tiles
  for (int numTileY = 0; numTileY < nTilesY; numTileY++)
    {
    for (int numTileX = 0; numTileX < nTilesX; numTileX++)
      {
      double xTile = (firstSample + m_TileSize * numTileX) / ((1 << m_Depth) * static_cast<double>(m_TileSize));
      double yTile = (firstLine + m_TileSize * numTileY) / ((1 << m_Depth) * static_cast<double>(m_TileSize));

      std::string lFilename;

      // Generate Tile filename
      this->GenerateTileInfo(xTile, yTile, numTileX, numTileY);

      // Try to read tile from cache
      if (!this->CanReadFromCache(m_ListTiles.back().filename))
        {
        this->GenerateURL(m_ListTiles.back().x, m_ListTiles.back().y);
        m_ListFilename.push_back(m_ListTiles.back().filename);
        }
      }
    }

  m_Curl->RetrieveFileMulti(m_ListURLs, m_ListFilename, m_MaxConnect);

  m_ListURLs.clear();

  // Generate buffer
  this->GenerateBuffer(p);

  otbMsgDevMacro(<< "TileMapImageIO::Read() completed");
}

/*
 * This method build tile filename
 */
void TileMapImageIO::GenerateTileInfo(double x, double y, int numTileX, int numTileY)
{
  std::ostringstream quad2;
  XYToQuadTree2(x, y, quad2);

  std::ostringstream filename;
  BuildFileName(quad2, filename);

  // Build tile information
  TileNameAndCoordType lTileInfos;
  lTileInfos.numTileX = numTileX;
  lTileInfos.numTileY = numTileY;
  lTileInfos.x = x;
  lTileInfos.y = y;
  lTileInfos.filename = filename.str();

  // Add to vector
  m_ListTiles.push_back(lTileInfos);
}

/*
 * This method try to read tile from cache
 */
bool TileMapImageIO::CanReadFromCache(const std::string& filename)
{
  // Verify that the file exists and is not a directory
  bool fileExists = itksys::SystemTools::FileExists(filename.c_str(), true);
  if (!fileExists)
    {
    return false;
    }

  otb::ImageIOBase::Pointer imageIO;
  imageIO = otb::GDALImageIO::New();
  return imageIO->CanReadFile(filename.c_str());
}

/*
 * This method generate URLs
 */
void TileMapImageIO::GenerateURL(double x, double y)
{
  std::ostringstream urlStream;

  // Google Map
  if (m_AddressMode == TileMapAdressingStyle::GM)
    {
    std::ostringstream quad;
    XYToQuadTree(x, y, quad);

    urlStream << m_ServerName;
    urlStream << quad.str();
    }
  // Open Street Map
  else if (m_AddressMode == TileMapAdressingStyle::OSM)
    {
    urlStream << m_ServerName;
    urlStream << m_Depth;
    urlStream << "/";
    urlStream << (long int) (((double) x * (1 << m_Depth)));
    urlStream << "/";
    urlStream << (long int) (((double) y * (1 << m_Depth)));
    urlStream << "." << m_FileSuffix;
    }
  // Near Map
  else if (m_AddressMode == TileMapAdressingStyle::NEARMAP)
    {
    urlStream << m_ServerName;
    urlStream << "hl=en&x=";
    urlStream << vcl_floor(x * (1 << m_Depth));
    urlStream << "&y=";
    urlStream << vcl_floor(y * (1 << m_Depth));
    urlStream << "&z=";
    urlStream << m_Depth;
    urlStream << "&nml=Vert&s=Ga";
    }
  // Local addressing
  else if (m_AddressMode == TileMapAdressingStyle::LOCAL)
    {
    std::ostringstream quad, filename;
    XYToQuadTree2(x, y, quad);
    BuildFileName(quad, filename, false);
    urlStream << m_ServerName;
    urlStream << filename.str();
    }
  else
    {
    itkExceptionMacro(<< "TileMapImageIO : Bad addressing Style");
    }

  // Add url to vector
  m_ListURLs.push_back(urlStream.str());
}

/*
 * This method generate the output buffer
 */
void TileMapImageIO::GenerateBuffer(unsigned char *p)
{
  int totLines   = this->GetIORegion().GetSize()[1];
  int totSamples = this->GetIORegion().GetSize()[0];
  int firstLine   = this->GetIORegion().GetIndex()[1];
  int firstSample = this->GetIORegion().GetIndex()[0];
  int nComponents = this->GetNumberOfComponents();

  unsigned char * bufferTile = new unsigned char[m_TileSize * m_TileSize * nComponents];
  for (unsigned int currentTile = 0; currentTile < m_ListTiles.size(); currentTile++)
    {

    // Read tile from cache
    this->ReadTile(m_ListTiles[currentTile].filename, bufferTile);

    int numTileX = m_ListTiles[currentTile].numTileX;
    int numTileY = m_ListTiles[currentTile].numTileY;

    for (int tileJ = 0; tileJ < m_TileSize; tileJ++)
      {
      long int yImageOffset = (long int) (m_TileSize * floor(firstLine / static_cast<double>(m_TileSize)) + m_TileSize * numTileY - firstLine + tileJ);
      if ((yImageOffset >= 0) && (yImageOffset < totLines))
        {
        long int xImageOffset = (long int)
                                (m_TileSize * floor(firstSample / static_cast<double>(m_TileSize)) + m_TileSize * numTileX - firstSample);
        unsigned char * dst = p + nComponents * (xImageOffset + totSamples * yImageOffset);
        unsigned char * src = bufferTile + nComponents * m_TileSize * tileJ;
        int             size = nComponents * m_TileSize;

        if (xImageOffset < 0)
          {
          dst -= nComponents * xImageOffset;
          src -= nComponents * xImageOffset;
          size += nComponents * xImageOffset;
          }
        if (xImageOffset + m_TileSize > totSamples)
          {
          size += nComponents * (totSamples - xImageOffset - m_TileSize);
          }
        if (size > 0)
          {
          memcpy(dst, src, size);
          }
        }
      } //end of tile copy
    } //end of full image copy
  delete[] bufferTile;
}

/*
 * This method read tile in the cache
 */
void TileMapImageIO::ReadTile(const std::string& filename, void * buffer)
{
  otbMsgDevMacro(<< "Retrieving " << filename);
  unsigned char *           bufferCacheFault = NULL;

  otb::ImageIOBase::Pointer imageIO;
  imageIO = otb::GDALImageIO::New();

  bool lCanRead = imageIO->CanReadFile(filename.c_str());

  if (lCanRead == true)
    {
    imageIO->SetFileName(filename.c_str());
    imageIO->ReadImageInformation();
    itk::ImageIORegion ioRegion(2);
    ioRegion.SetIndex(0, 0);
    ioRegion.SetIndex(1, 0);
    ioRegion.SetSize(0, m_TileSize);
    ioRegion.SetSize(1, m_TileSize);
    imageIO->SetIORegion(ioRegion);

    imageIO->Read(buffer);
    }
  else
    {
    bufferCacheFault = new unsigned char[m_TileSize * m_TileSize * 3];
    FillCacheFaults(bufferCacheFault);
    memcpy(buffer, bufferCacheFault, m_TileSize * m_TileSize * 3);
    delete [] bufferCacheFault;
    }
}

void TileMapImageIO::BuildFileName(const std::ostringstream& quad, std::ostringstream& filename, bool inCache) const
{

  int                quadsize = quad.str().size();
  std::ostringstream directory;
  if (inCache)
    {
    directory << m_CacheDirectory;
    }
  //build directory name
  int i = 0;
  while ((i < 8) && (i < quadsize))
    {
    directory << "/";
    directory << (quad.str().c_str())[i];
    ++i;
    }

  itksys::SystemTools::MakeDirectory(directory.str().c_str());

  filename << directory.str();
  filename << "/";
  filename << "otb-";
  filename << quad.str();
  filename << "." << m_FileSuffix;

}

/* Fill up dhe image information reading the ascii configuration file */
void TileMapImageIO::ReadImageInformation()
{

  m_Dimensions[0] = (1 << m_Depth) * m_TileSize;
  m_Dimensions[1] = (1 << m_Depth) * m_TileSize;
  otbMsgDevMacro(<< "Get Dimensions : x=" << m_Dimensions[0] << " & y=" << m_Dimensions[1]);

  // Default Spacing
  m_Spacing[0] = 1;
  m_Spacing[1] = 1;
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;


  if (m_FileName.empty() == true)
    {
    itkExceptionMacro(<< "TileMap read : empty image file name file.");
    }

  if (m_FileName.find("http://") == 0)
    {
    m_FileNameIsServerName = true;
    }

  if (!m_FileNameIsServerName)
    {
    std::ifstream file(m_FileName.c_str(), std::ifstream::in);
    std::getline(file, m_ServerName);
    if  (m_ServerName.find("http://") != 0)
      {
      itkExceptionMacro(<< "Can't read server name from file");
      }
    std::getline(file, m_FileSuffix);
    std::string mode;
    std::getline(file, mode);
    switch (atoi(mode.c_str()))
      {
      case 0:
        m_AddressMode = TileMapAdressingStyle::GM;
        break;
      case 1:
        m_AddressMode = TileMapAdressingStyle::OSM;
        break;
      case 2:
        m_AddressMode = TileMapAdressingStyle::NEARMAP;
        break;
      case 3:
        m_AddressMode = TileMapAdressingStyle::LOCAL;
        break;
      default:
        itkExceptionMacro(<< "Addressing style unknown");
      }

    otbMsgDevMacro(<< "File parameters: " << m_ServerName << " " << m_FileSuffix << " " << m_AddressMode);
    }
  else
    {
    m_ServerName = m_FileName;
    if  (m_ServerName.find("http://") != 0)
      {
      itkExceptionMacro(<< "Can't read server name from file");
      }
    std::string osmServer = "http://tile.openstreetmap.org/";
    std::string nmServer = "http://www.nearmap.com/maps/";
    std::string otbServer1 = "http://tile.orfeo-toolbox.org/hillShade/";

    if (m_ServerName == osmServer)
      {
      m_FileSuffix = "png";
      m_AddressMode = TileMapAdressingStyle::OSM;
      }
    else if (m_ServerName == nmServer)
      {
      m_FileSuffix = "jpg";
      m_AddressMode = TileMapAdressingStyle::NEARMAP;
      }
    else if (m_ServerName == otbServer1)
      {
      m_FileSuffix = "jpg";
      m_AddressMode = TileMapAdressingStyle::LOCAL;
      }
    else
      {
      m_FileSuffix = "jpg";
      m_AddressMode = TileMapAdressingStyle::GM;
      }

    // File suffix and addres mode must be set with accessors
    otbMsgDevMacro(<< "File parameters: " << m_ServerName << " " << m_FileSuffix << " " << m_AddressMode);
    }

  // The OSM tiles are 4 bands png, while HillShade & NearMap are 3 bands jpeg
  if (m_AddressMode == TileMapAdressingStyle::OSM)
    this->SetNumberOfComponents(4);
  else
    this->SetNumberOfComponents(3);

  this->SetNumberOfDimensions(2);
  this->SetFileTypeToBinary();
  this->SetComponentType(UCHAR);
  
  ImageKeywordlist otb_kwl;
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
  itk::ExposeMetaData<ImageKeywordlist>(dict,
                                        MetaDataKey::OSSIMKeywordlistKey,
                                        otb_kwl);
  if( otb_kwl.GetSize() != 0 )
    {
      std::ostringstream depth;
      depth << this->GetDepth();
      otb_kwl.AddKey("depth", depth.str());
    }

}

bool TileMapImageIO::CanWriteFile(const char* name)
{
  // First if filename is provided
  if (name == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  // Check for file extension
  std::string            filename = name;
  std::string::size_type gmPos = filename.rfind(".otb");
  if ((gmPos != std::string::npos)
      && (gmPos == filename.length() - 3))
    {
    return true;
    }
  return false;
}

void TileMapImageIO::WriteImageInformation(void)
{
}

void TileMapImageIO::Write(const void* buffer)
{

  const unsigned char * p = static_cast<const unsigned char *>(buffer);
  if (p == NULL)
    {
    itkExceptionMacro(<< "Memory allocation error");
    return;
    }

  if (m_FlagWriteImageInformation == true)
    {
    this->WriteImageInformation();
    m_FlagWriteImageInformation = false;
    }

  int totLines   = this->GetIORegion().GetSize()[1];
  int totSamples = this->GetIORegion().GetSize()[0];
  int firstLine   = this->GetIORegion().GetIndex()[1];
  int firstSample = this->GetIORegion().GetIndex()[0];
  int originLine   = (int) this->GetOrigin(1);
  int originSample = (int) this->GetOrigin(0);
  int nComponents = this->GetNumberOfComponents();

  otbMsgDevMacro(<< "TileMapImageIO::Write: Size " << totLines << ", " << totSamples);
  otbMsgDevMacro(<< "TileMapImageIO::Write: Index " << firstLine << ", " << firstSample);
  otbMsgDevMacro(<< "TileMapImageIO::Write: Origin " << originLine << ", " << originSample);

  otbMsgDevMacro(<< " Image size  : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " Region read (IORegion)  : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components  : " << this->GetNumberOfComponents());

  otbMsgDevMacro(<< " sizeof(streamsize)    : " << sizeof(std::streamsize));
  otbMsgDevMacro(<< " sizeof(streampos)     : " << sizeof(std::streampos));
  otbMsgDevMacro(<< " sizeof(streamoff)     : " << sizeof(std::streamoff));
  otbMsgDevMacro(<< " sizeof(std::ios::beg) : " << sizeof(std::ios::beg));
  otbMsgDevMacro(<< " sizeof(size_t)        : " << sizeof(size_t));
  otbMsgDevMacro(<< " sizeof(unsigned long) : " << sizeof(unsigned long));

  //Using integer division:
  int nTilesX = (originSample + totSamples - 1) / m_TileSize - originSample / m_TileSize + 1;
  int nTilesY = (originLine + totLines - 1) / m_TileSize - originLine / m_TileSize + 1;
  otbMsgDevMacro(<< "Number of tile to process " << nTilesX << "x" << nTilesY);

  unsigned char * bufferTile = new unsigned char[m_TileSize * m_TileSize * nComponents];

  //Read all the required tiles
  for (int numTileY = 0; numTileY < nTilesY; numTileY++)
    {
    for (int numTileX = 0; numTileX < nTilesX; numTileX++)
      {

      //Set tile buffer to 0
      for (int iInit = 0; iInit < m_TileSize * m_TileSize * nComponents; iInit++)
        {
        bufferTile[iInit] = 0;
        }

      for (int tileJ = 0; tileJ < m_TileSize; tileJ++)
        {
        long int yImageOffset = (long int) (m_TileSize * floor((originLine + firstLine) / static_cast<double>(m_TileSize))
                                            + m_TileSize * numTileY - (originLine + firstLine) + tileJ);
        if ((yImageOffset >= 0) && (yImageOffset < totLines))
          {
          long int xImageOffset = (long int)
                                  (m_TileSize *
                                   floor((originSample +
                                          firstSample) / static_cast<double>(m_TileSize)) + m_TileSize * numTileX - (originSample + firstSample));
          unsigned char *       dst = bufferTile + nComponents * m_TileSize * tileJ;
          const unsigned char * src = p + nComponents * (xImageOffset + totSamples * yImageOffset);
          int                   size = nComponents * m_TileSize;
          if (xImageOffset < 0)
            {
            src -= nComponents * xImageOffset;
            dst -= nComponents * xImageOffset;
            size += nComponents * xImageOffset;
            }
          if (xImageOffset + m_TileSize > totSamples)
            {
            size += nComponents * (totSamples - xImageOffset - m_TileSize);
            }
          if (size > 0)
            {
            memcpy(dst, src, size);
            }

          }
        } //end of tile copy

      double xTile = (originSample + firstSample + m_TileSize * numTileX) / ((1 << m_Depth) * static_cast<double>(m_TileSize));
      double yTile = (originLine + firstLine + m_TileSize * numTileY) / ((1 << m_Depth) * static_cast<double>(m_TileSize));
      //Write the tile
      InternalWrite(xTile, yTile, bufferTile);

      }
    } //end of full image copy

  delete[] bufferTile;

  otbMsgDevMacro(<< "TileMapImageIO::Write() completed");
}

void TileMapImageIO::InternalWrite(double x, double y, const void* buffer)
{
  std::ostringstream quad;

  otbMsgDevMacro(<< x << ", " << y);

  XYToQuadTree2(x, y, quad);

  std::ostringstream filename;
  BuildFileName(quad, filename);

  otb::ImageIOBase::Pointer imageIO;
  imageIO = otb::GDALImageIO::New();
  bool lCanWrite = imageIO->CanWriteFile(filename.str().c_str());


  if (lCanWrite)
    {
    itksys::SystemTools::RemoveFile(filename.str().c_str());

    imageIO->CanStreamWrite();
    imageIO->SetNumberOfDimensions(2);
    imageIO->SetDimensions(0, m_TileSize);
    imageIO->SetDimensions(1, m_TileSize);
    imageIO->SetSpacing(0, 1);
    imageIO->SetSpacing(1, 1);
    imageIO->SetOrigin(0, 0.5);
    imageIO->SetOrigin(1, 0.5);
    imageIO->SetNumberOfComponents(3);

    vnl_vector<double> axisDirection(2);

    axisDirection[0] = 1;
    axisDirection[1] = 0;
    imageIO->SetDirection(0, axisDirection);
    axisDirection[0] = 0;
    axisDirection[1] = 1;
    imageIO->SetDirection(1, axisDirection);

    imageIO->SetUseCompression(1);

    imageIO->SetFileName(filename.str().c_str());
    imageIO->WriteImageInformation();

    itk::ImageIORegion ioRegion(2);
    for (unsigned int i = 0; i < 2; ++i)
      {
      ioRegion.SetSize(i, m_TileSize);
      ioRegion.SetIndex(i, 0);
      }

    imageIO->SetIORegion(ioRegion);

    imageIO->Write(buffer);
    }
  else
    {
    itkExceptionMacro(<< "TileMap write : bad file name.");
    }

}

/** Generate the quadtree address in qrts style */
int TileMapImageIO::XYToQuadTree(double x, double y, std::ostringstream& quad) const
{
  int lDepth = m_Depth;
  while (lDepth--) // (post-decrement)
    {
    // make sure we only look at fractional part
    x -= floor(x);
    y -= floor(y);
    int quad_index = ((x >= 0.5 ? 1 : 0) + (y >= 0.5 ? 2 : 0));

    switch (quad_index)
      {
      case 0:
        quad << "q";
        break;
      case 1:
        quad << "r";
        break;
      case 2:
        quad << "t";
        break;
      case 3:
        quad << "s";
        break;
      }
// level down
    x *= 2;
    y *= 2;
    }

  return 0;
}

/** Generate the quadtree address in 0123 style */
int TileMapImageIO::XYToQuadTree2(double x, double y, std::ostringstream& quad) const
{
  int lDepth = m_Depth;
  while (lDepth--) // (post-decrement)
    {
    // make sure we only look at fractional part
    x -= floor(x);
    y -= floor(y);
    int quad_index = ((x >= 0.5 ? 1 : 0) + (y >= 0.5 ? 2 : 0));

    switch (quad_index)
      {
      case 0:
        quad << "0";
        break;
      case 1:
        quad << "1";
        break;
      case 2:
        quad << "2";
        break;
      case 3:
        quad << "3";
        break;
      }
// level down
    x *= 2;
    y *= 2;
    }

  return 0;
}

unsigned int
TileMapImageIO::GetActualNumberOfSplitsForWritingCanStreamWrite(unsigned int numberOfRequestedSplits,
                                                                const ImageIORegion& pasteRegion) const
{
  typedef itk::ImageRegion<2> RegionType;
  RegionType            tileMapRegion;
  RegionType::IndexType index;
  index[0] = this->GetOrigin(0);
  index[1] = this->GetOrigin(1);
  itk::ImageIORegionAdaptor<2>::Convert(pasteRegion, tileMapRegion, index);
  return m_TileMapSplitter->GetNumberOfSplits(tileMapRegion, numberOfRequestedSplits);
}

itk::ImageIORegion
TileMapImageIO::GetSplitRegionForWritingCanStreamWrite(unsigned int ithPiece,
                                                       unsigned int numberOfActualSplits,
                                                       const ImageIORegion& pasteRegion) const
{
  typedef itk::ImageRegion<2> RegionType;
  RegionType            tileMapRegion;
  RegionType::IndexType index;
  index[0] = this->GetOrigin(0);
  index[1] = this->GetOrigin(1);
  itk::ImageIORegionAdaptor<2>::Convert(pasteRegion, tileMapRegion, index);
  ImageIORegion returnRegion;
  itk::ImageIORegionAdaptor<2>::Convert(m_TileMapSplitter->GetSplit(ithPiece,
                                                                    numberOfActualSplits,
                                                                    tileMapRegion), returnRegion, index);
  return returnRegion;
}

/** RGB buffer filling when the tile is not found */
void TileMapImageIO::FillCacheFaults(void* buffer) const
{
  //replicate to make 256x256x3 pixels
  for (int line = 0; line < m_TileSize; line++)
    {
    for (int col = 0; col < m_TileSize/64; col++)
      {
      memcpy(((unsigned char *) buffer) + line * m_TileSize * 3 + 64 * 3 * col,
             kLogoOtb + (line % 64) * 64 * 3,
             64 * 3);
      }
    }
}

void TileMapImageIO::SetCacheDirectory(const char* _arg)
{
  if (_arg && (_arg == this->m_CacheDirectory))
    {
    return;
    }

   if (_arg)
    {
      // if existing dir, check writable
      if ( itksys::SystemTools::FileIsDirectory( _arg ) )
      {
         std::ostringstream oss;
         oss<<_arg<<"/foo";
         if( itksys::SystemTools::Touch( oss.str().c_str(), true ) == false )
         {
            itkExceptionMacro( "Error, no write permission in given CacheDirectory "<<_arg<<".");
         }
         else
         {
            itksys::SystemTools::RemoveFile( oss.str().c_str() );
         }
      }
      // if existing file
      else if( itksys::SystemTools::FileExists(_arg) == true )
      {
         itkExceptionMacro( "Error, given CacheDirectory "<<_arg<<" is an existing file.");
      }
      // doesn't exist, try to create it
      else if( itksys::SystemTools::MakeDirectory( _arg ) == false )
      {
        itkExceptionMacro( "Error, no permission to create the given CacheDirectory "<<_arg<<".");
      }
      else
      {
         itksys::SystemTools::RemoveADirectory( _arg );
      }
      this->m_CacheDirectory = _arg;
      this->m_UseCache = true;
   }
   else
   {
      this->m_CacheDirectory = "";
      this->m_UseCache = false;
   }

   this->Modified();
}

void TileMapImageIO::SetCacheDirectory(const std::string& _arg)
{
  this->SetCacheDirectory(_arg.c_str());
}

} // end namespace otb
