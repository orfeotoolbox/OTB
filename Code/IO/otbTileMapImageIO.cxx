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

#include "itkExceptionObject.h"
#include "itkMacro.h"

#include <string.h>
#include <math.h>

#include <iostream>
#include <fstream>

//This is to check the file existence
#include <sys/stat.h>

#include "otbTileMapImageIO.h"
#include "otbMacro.h"
#include "otbSystem.h"

#include "itkPNGImageIO.h"
#include "itkJPEGImageIO.h"

#include "base/ossimFilename.h"

#include "itkTimeProbe.h"

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
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_NbBands = 3;
  m_FlagWriteImageInformation = true;

  //Resolution depth
  m_Depth = 8;

  m_BytePerPixel=1;

  m_UseCache=false;
  m_ServerName="";
  m_CacheDirectory=".";
  m_FileSuffix="png";
  m_AddressMode=TileMapAdressingStyle::OSM;
  
  m_FileNameIsServerName = false;

  // Initialize browser
  m_Browser = "Mozilla/5.0 (Windows; U; Windows NT 6.0; en-GB; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11";
  // Set maximum of connections to 10
  m_MaxConnect = 10;

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
  if (  file == NULL )
  {
    itkDebugMacro(<<"No filename specified.");
    return false;
  }

  std::string filename = file;
  std::string::size_type gmPos = filename.rfind(".otb");
  
  if ( (gmPos != std::string::npos)
       && (gmPos == filename.length() - 4) )
  {
    m_FileNameIsServerName = false;
    return true;
  }
  // Filename is http server
  else if (filename.find("http://") == 0)
  {
    m_FileNameIsServerName = true;
    return true;
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
  if (p==NULL)
  {
    itkExceptionMacro(<<"Memory allocation error");
    return;
  }

  int totLines   = this->GetIORegion().GetSize()[1];
  int totSamples = this->GetIORegion().GetSize()[0];
  int firstLine   = this->GetIORegion().GetIndex()[1];
  int firstSample = this->GetIORegion().GetIndex()[0];

  int nTilesX = (int) ceil(totSamples/256.)+1;
  int nTilesY = (int) ceil(totLines/256.)+1;

  // Initialize curl multi handle
  m_MultiHandle = curl_multi_init();
  
  if (!m_MultiHandle)
  {
    itkExceptionMacro( << "Tile Map IO : Curl mutli handle init error.");
  }
  
  // Clear vectors
  m_ListCurlHandles.clear();
  m_ListFiles.clear();
  m_ListURLs.clear();
  m_ListTiles.clear();

  //Read all the required tiles
  for (int numTileY=0; numTileY<nTilesY; numTileY++)
  {
    for (int numTileX=0; numTileX<nTilesX; numTileX++)
    {
      double xTile = (firstSample+256*numTileX)/((1 << m_Depth)*256.);
      double yTile = (firstLine+256*numTileY)/((1 << m_Depth)*256.);

      std::string lFilename;

      // Generate Tile filename
      this->GenerateTileInfo(xTile, yTile, numTileX, numTileY);

      // Try to read tile from cache
      if(!this->CanReadFromCache(m_ListTiles.back().filename))
      {
        // Generate curl handle for this tile
        this->GenerateCURLHandle(m_ListTiles.back());
      }
    }
  }
  
  // Fetch tiles from net
  this->FetchTiles();

  // Cleanup datas use to download tiles
  this->Cleanup();
  
  // Generate buffer
  this->GenerateBuffer(p);

  otbMsgDevMacro( << "TileMapImageIO::Read() completed");
}

/*
 * This method build tile filename
 */
void TileMapImageIO::GenerateTileInfo(double x, double y, int numTileX, int numTileY)
{
  std::ostringstream quad2;
  double xorig=x;
  double yorig=y;

  XYToQuadTree2(x, y, quad2);

  std::ostringstream filename;
  BuildFileName(quad2, filename);  
  
  // Build tile informations
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
bool TileMapImageIO::CanReadFromCache(std::string filename)
{
  itk::ImageIOBase::Pointer imageIO;
  //Open the file to fill the buffer
  if (m_FileSuffix == "png")
    {
    imageIO = itk::PNGImageIO::New();
    }
  else if (m_FileSuffix == "jpg")
    {
    imageIO = itk::JPEGImageIO::New();
    }
  else
    {
    itkExceptionMacro( << "TileMapImageIO : Bad addressing Style");
    }
  bool lCanRead(false);
  lCanRead = imageIO->CanReadFile(filename.c_str());
  
  return lCanRead;
}

/*
 * This method generate curl handles and add to multi handle
 */
void TileMapImageIO::GenerateCURLHandle(TileNameAndCoordType tileInfo)
{
  // Generate URL
  this->GenerateURL(tileInfo.x, tileInfo.y);
    
  // Initialize curl handle
  CURL * lEasyHandle;
  lEasyHandle = curl_easy_init();
  
  if (!lEasyHandle)
  {
    itkExceptionMacro( <<"Tile Map IO : Curl easy handle init error.");
  }
  
  // Create file
  FILE* lOutputFile = fopen(tileInfo.filename.c_str(), "w");
  
  if (lOutputFile == NULL)
  {
    itkExceptionMacro(<<"TileMap read : bad file name.");
  }
  
  // Add file to vector
  m_ListFiles.push_back(lOutputFile);
  
  // Param easy handle
  curl_easy_setopt(lEasyHandle, CURLOPT_USERAGENT, m_Browser.data());
  curl_easy_setopt(lEasyHandle, CURLOPT_URL, m_ListURLs.back().data());
  curl_easy_setopt(lEasyHandle, CURLOPT_WRITEDATA, m_ListFiles.back());
  
  // Add easy handle to multi handle
  curl_multi_add_handle(m_MultiHandle, lEasyHandle);
  
  // Add hanle to vector
  m_ListCurlHandles.push_back(lEasyHandle);
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
    urlStream << (long int) (((double) x*(1 << m_Depth)));
    urlStream << "/";
    urlStream << (long int) (((double) y*(1 << m_Depth)));
    urlStream << "." << m_FileSuffix;
  }
  // Near Map
  else if (m_AddressMode == TileMapAdressingStyle::NEARMAP)
  {
    urlStream << m_ServerName;
    urlStream << "hl=en&x=";
    urlStream << vcl_floor(x*(1 << m_Depth) );
    urlStream << "&y=";
    urlStream << vcl_floor(y*(1 << m_Depth) );
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
    itkExceptionMacro( << "TileMapImageIO : Bad addressing Style");
  }
  
  // Add url to vector
  m_ListURLs.push_back(urlStream.str());
}

/*
 * This method perform curl multi handle
 */
void TileMapImageIO::FetchTiles()
{
  // Configure multi handle - set the maximum connections
  curl_multi_setopt(m_MultiHandle, CURLMOPT_MAXCONNECTS, m_MaxConnect);
  curl_multi_setopt(m_MultiHandle, CURLMOPT_PIPELINING, 0);
  
    // Perform
 int lStillRunning;

  while(
    CURLM_CALL_MULTI_PERFORM == curl_multi_perform(m_MultiHandle, &lStillRunning)
  );

  // Now get that URL
  while(lStillRunning)
  {
    struct timeval timeout;
    int rc; // Return code
    
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd;
    
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);
    
    /* set a suitable timeout to play around with */
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    
    /* get file descriptors from the transfers */
    curl_multi_fdset(m_MultiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
    
    rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
    
    switch(rc) {
    case -1:
      /* select error */
      break;
    case 0:
      /* timeout */
    default:
      /* timeout or readable/writable sockets */
      while(
        CURLM_CALL_MULTI_PERFORM == curl_multi_perform(m_MultiHandle, &lStillRunning)
      );
      break;
    }
  }
  
  int remaining_msgs = 1;
  int error = 0;
  CURLMsg *msg;
  while (remaining_msgs)
  {
    msg = curl_multi_info_read(m_MultiHandle, &remaining_msgs);
    if (msg != NULL)
    {
      if (CURLE_OK != msg->data.result)
        error = 1;
    }
  }

  if (error != 0)
  {
    itkExceptionMacro( <<"TileMapImageIO : Error occurs while perform Multi handle");
  }
}

/*
 * This method cleanup datas
 */
void TileMapImageIO::Cleanup()
{
  // Close files
  for (unsigned int currentFile=0; currentFile<m_ListFiles.size(); currentFile++)
  {
    fclose(m_ListFiles[currentFile]);
  }
  m_ListFiles.clear();
  
  // Cleanup easy handles
  for (unsigned int currentHandle=0; currentHandle<m_ListCurlHandles.size(); currentHandle++)
  {
    curl_easy_cleanup(m_ListCurlHandles[currentHandle]);
  }
  m_ListCurlHandles.clear();
  
  // Cleanup multi handle
  curl_multi_cleanup(m_MultiHandle);
  
  // Cleanup url vector
  m_ListURLs.clear();
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

  for (unsigned int currentTile=0; currentTile<m_ListTiles.size(); currentTile++)
  {
    unsigned char * bufferTile = new unsigned char[256*256*nComponents];
 
    // Read tile from cache
    this->ReadTile(m_ListTiles[currentTile].filename, bufferTile);
    
    int numTileX = m_ListTiles[currentTile].numTileX;
    int numTileY = m_ListTiles[currentTile].numTileY;
  
    for (int tileJ=0; tileJ<256; tileJ++)
    {
      long int yImageOffset=(long int) (256*floor(firstLine/256.)+256*numTileY-firstLine+tileJ);
      if ((yImageOffset >= 0) && (yImageOffset < totLines))
      {
        long int xImageOffset = (long int)
                              (256*floor(firstSample/256.)+256*numTileX-firstSample);
        unsigned char * dst = p+nComponents*(xImageOffset+totSamples*yImageOffset);
        unsigned char * src = bufferTile+nComponents*256*tileJ;
        int size = nComponents*256;
        
        if (xImageOffset < 0)
        {
          dst -= nComponents*xImageOffset;
          src -= nComponents*xImageOffset;
          size += nComponents*xImageOffset;
        }
        if (xImageOffset+256 > totSamples)
        {
          size += nComponents*(totSamples-xImageOffset-256);
        }
        if (size > 0)
        {
          memcpy(dst, src, size);
        }
      }
    }//end of tile copy
    delete[] bufferTile;
  }//end of full image copy
}

/*
 * This method read tile in the cache
 */ 
void TileMapImageIO::ReadTile(std::string filename, void * buffer)
{
  otbMsgDevMacro(<< "Retrieving " << filename);
  unsigned char * bufferCacheFault = NULL;
  itk::ImageIOBase::Pointer imageIO; 

  if (m_FileSuffix == "png")
    {
    imageIO = itk::PNGImageIO::New();
    }
  else if (m_FileSuffix == "jpg")
    {
    imageIO = itk::JPEGImageIO::New();
    }
  else
    {
    itkExceptionMacro( << "TileMapImageIO : Bad addressing Style");
    }
  bool lCanRead(false);
  lCanRead = imageIO->CanReadFile(filename.c_str());
  
  if ( lCanRead == true)
  {
    imageIO->SetFileName(filename.c_str());
    imageIO->Read(buffer);
  }
  else
  {
    if (bufferCacheFault == NULL)
    {
      bufferCacheFault = new unsigned char[256*256*3];
      FillCacheFaults(bufferCacheFault);
    }
    memcpy(buffer, bufferCacheFault,256*256*3 );
  }
}

void TileMapImageIO::BuildFileName(const std::ostringstream& quad, std::ostringstream& filename, bool inCache) const
{

  int quadsize=quad.str().size();
  std::ostringstream directory;
  if (inCache)
    {
    directory << m_CacheDirectory;
    }
  //build directory name
  int i=0;
  while ((i<8) && (i<quadsize))
  {
    directory << "/";
    directory << (quad.str().c_str())[i];
    i++;
  }
  ossimFilename directoryOssim(directory.str().c_str());
  directoryOssim.createDirectory();


  filename << directory.str();
  filename << "/";
  filename << "otb-";
  filename << quad.str();
  filename << "." << m_FileSuffix;

}

/* Fill up dhe image information reading the ascii configuration file */
void TileMapImageIO::ReadImageInformation()
{
  if (  m_FileName.empty() == true )
  {
    itkExceptionMacro(<<"TileMap read : empty image file name file.");
  }

  if (m_FileName.find("http://") == 0)
  {
    m_FileNameIsServerName = true;
  }
  
  m_Dimensions[0] = (1 << m_Depth)*256;
  m_Dimensions[1] = (1 << m_Depth)*256;
  otbMsgDevMacro(<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]);
  this->SetNumberOfComponents(3);
  this->SetNumberOfDimensions(2);
  this->SetFileTypeToBinary();
  SetComponentType(UCHAR);
  // Default Spacing
  m_Spacing[0] = 1;
  m_Spacing[1] = 1;
  m_Origin[0] = 0;
  m_Origin[1] = 0;
  
  if (!m_FileNameIsServerName)
  {
    std::ifstream file(m_FileName.c_str(), std::ifstream::in );
    std::getline(file, m_ServerName);
    if  (m_ServerName.find("http://") != 0)
    {
      itkExceptionMacro(<<"Can't read server name from file");
    }
    std::getline(file, m_FileSuffix);
    std::string mode;
    std::getline(file, mode);
    switch (atoi(mode.c_str()))
    {
      case 0:
        m_AddressMode = TileMapAdressingStyle::GM;
        return;
      case 1:
        m_AddressMode = TileMapAdressingStyle::OSM;
        return;
      case 2:
        m_AddressMode = TileMapAdressingStyle::NEARMAP;
        return;
      case 3:
        m_AddressMode = TileMapAdressingStyle::LOCAL;
        return;
      default:
        itkExceptionMacro(<<"Addressing style unknown");
    }

    otbMsgDevMacro( << "File parameters: " << m_ServerName << " " << m_FileSuffix << " " << m_AddressMode);
  }
  else
  {
    m_ServerName = m_FileName;
    if  (m_ServerName.find("http://") != 0)
    {
      itkExceptionMacro(<<"Can't read server name from file");
    }
    std::string osmServer = "http://tile.openstreetmap.org/";
    std::string nmServer = "http://www.nearmap.com/maps/";
    
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
    else
    {
      m_FileSuffix = "jpg";
      m_AddressMode = TileMapAdressingStyle::GM;
    }
    
    // File suffix and addres mode must be set with accessors
    otbMsgDevMacro( << "File parameters: " << m_ServerName << " " << m_FileSuffix << " " << m_AddressMode);
  }
}

bool TileMapImageIO::CanWriteFile( const char* name )
{
  // First if filename is provided
  if (  name == NULL )
  {
    itkDebugMacro(<<"No filename specified.");
    return false;
  }

  // Check for file extension
  std::string filename = name;
  std::string::size_type gmPos = filename.rfind(".otb");
  if ( (gmPos != std::string::npos)
       && (gmPos == filename.length() - 3) )
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
  if (p==NULL)
  {
    itkExceptionMacro(<<"Memory allocation error");
    return;
  }

  if ( m_FlagWriteImageInformation == true )
  {
    this->WriteImageInformation();
    m_FlagWriteImageInformation = false;
  }

  int totLines   = this->GetIORegion().GetSize()[1];
  int totSamples = this->GetIORegion().GetSize()[0];
  int firstLine   = this->GetIORegion().GetIndex()[1];
  int firstSample = this->GetIORegion().GetIndex()[0];
  int originLine   = (int)this->GetOrigin(1);
  int originSample = (int)this->GetOrigin(0);
  int nComponents = this->GetNumberOfComponents();

  otbMsgDevMacro( << "TileMapImageIO::Write: Size " << totLines << ", "<< totSamples);
  otbMsgDevMacro( << "TileMapImageIO::Write: Index " << firstLine << ", "<< firstSample);
  otbMsgDevMacro( << "TileMapImageIO::Write: Origin " << originLine << ", "<< originSample);

  otbMsgDevMacro( <<" Image size  : " << m_Dimensions[0]<<","<<m_Dimensions[1]);
  otbMsgDevMacro( <<" Region read (IORegion)  : "<<this->GetIORegion());
  otbMsgDevMacro( <<" Nb Of Components  : "<<this->GetNumberOfComponents());

  otbMsgDevMacro( <<" sizeof(streamsize)    : "<<sizeof(std::streamsize));
  otbMsgDevMacro( <<" sizeof(streampos)     : "<<sizeof(std::streampos));
  otbMsgDevMacro( <<" sizeof(streamoff)     : "<<sizeof(std::streamoff));
  otbMsgDevMacro( <<" sizeof(std::ios::beg) : "<<sizeof(std::ios::beg));
  otbMsgDevMacro( <<" sizeof(size_t)        : "<<sizeof(size_t));
  otbMsgDevMacro( <<" sizeof(unsigned long) : "<<sizeof(unsigned long));


  //Using integer division:
  int nTilesX = (originSample + totSamples -1)/256 - originSample/256 +1;
  int nTilesY = (originLine + totLines -1)/256 - originLine/256 +1;
  otbMsgDevMacro( << "Number of tile to process " << nTilesX << "x" << nTilesY);

  unsigned char * bufferTile = new unsigned char[256*256*nComponents];

  //Read all the required tiles
  for (int numTileY=0; numTileY<nTilesY; numTileY++)
  {
    for (int numTileX=0; numTileX<nTilesX; numTileX++)
    {

      //Set tile buffer to 0
      for (int iInit=0; iInit<256*256*nComponents; iInit++)
      {
        bufferTile[iInit]=0;
      }


      for (int tileJ=0; tileJ<256; tileJ++)
      {
        long int yImageOffset=(long int) (256*floor((originLine+firstLine)/256.)
                                           +256*numTileY-(originLine+firstLine)+tileJ);
        if ((yImageOffset >= 0) && (yImageOffset < totLines))
        {
          long int xImageOffset = (long int)
                                  (256*floor((originSample+firstSample)/256.)+256*numTileX-(originSample+firstSample));
          unsigned char * dst = bufferTile+nComponents*256*tileJ;
          const unsigned char * src = p+nComponents*(xImageOffset+totSamples*yImageOffset);
          int size = nComponents*256;
          if (xImageOffset < 0)
          {
            src -= nComponents*xImageOffset;
            dst -= nComponents*xImageOffset;
            size += nComponents*xImageOffset;
          }
          if (xImageOffset+256 > totSamples)
          {
            size += nComponents*(totSamples-xImageOffset-256);
          }
          if (size > 0)
          {
            memcpy(dst, src, size);
          }


        }
      }//end of tile copy


      double xTile = (originSample+firstSample+256*numTileX)/((1 << m_Depth)*256.);
      double yTile = (originLine+firstLine+256*numTileY)/((1 << m_Depth)*256.);
      //Write the tile
      InternalWrite(xTile, yTile, bufferTile);


    }
  }//end of full image copy

  delete[] bufferTile;

  otbMsgDevMacro( << "TileMapImageIO::Write() completed");
}


void TileMapImageIO::InternalWrite(double x, double y, const void* buffer)
{
  std::ostringstream quad;

  otbMsgDevMacro( << x << ", " << y );

  XYToQuadTree2(x, y, quad);

  std::ostringstream filename;
  BuildFileName(quad, filename);

  itk::ImageIOBase::Pointer imageIO;
  //Open the file to write the buffer
  if (m_FileSuffix == "png")
    {
    imageIO = itk::PNGImageIO::New();
    }
  else if (m_FileSuffix == "jpg")
    {
    imageIO = itk::JPEGImageIO::New();
    }
  else
    {
    itkExceptionMacro( << "TileMapImageIO : Bad addressing Style");
    }

  bool lCanWrite(false);
  lCanWrite = imageIO->CanWriteFile(filename.str().c_str());
  otbMsgDevMacro( << filename.str());

  if ( lCanWrite == true)
  {
    imageIO->SetNumberOfDimensions(2);
    imageIO->SetDimensions(0,256);
    imageIO->SetDimensions(1,256);
    imageIO->SetSpacing(0,1);
    imageIO->SetSpacing(1,1);
    imageIO->SetOrigin(0,0);
    imageIO->SetOrigin(1,0);
    imageIO->SetNumberOfComponents(3);

    vnl_vector< double > axisDirection(2);

    axisDirection[0] = 1;
    axisDirection[1] = 0;
    imageIO->SetDirection( 0, axisDirection );
    axisDirection[0] = 0;
    axisDirection[1] = 1;
    imageIO->SetDirection( 1, axisDirection );

    imageIO->SetUseCompression(1);

    imageIO->SetFileName(filename.str().c_str());
    imageIO->WriteImageInformation();

    itk::ImageIORegion ioRegion(2);
    for (unsigned int i=0; i<2; i++)
    {
      ioRegion.SetSize(i,256);
      ioRegion.SetIndex(i,0);
    }
    imageIO->SetIORegion(ioRegion);

    imageIO->Write(buffer);
  }
  else
  {
    itkExceptionMacro(<<"TileMap write : bad file name.");
  }

}

/** Generate the quadtree address in qrts style */
int TileMapImageIO::XYToQuadTree(double x, double y, std::ostringstream& quad) const
{
  int lDepth=m_Depth;
  while (lDepth--) // (post-decrement)
  {
    // make sure we only look at fractional part
    x -= floor(x);
    y -= floor(y);
    int quad_index = ((x >= 0.5 ? 1 : 0) + (y >= 0.5 ? 2 : 0));

    switch (quad_index)
    {
    case 0:
      quad<<"q";
      break;
    case 1:
      quad<<"r";
      break;
    case 2:
      quad<<"t";
      break;
    case 3:
      quad<<"s";
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
  int lDepth=m_Depth;
  while (lDepth--) // (post-decrement)
  {
    // make sure we only look at fractional part
    x -= floor(x);
    y -= floor(y);
    int quad_index = ((x >= 0.5 ? 1 : 0) + (y >= 0.5 ? 2 : 0));

    switch (quad_index)
    {
    case 0:
      quad<<"0";
      break;
    case 1:
      quad<<"1";
      break;
    case 2:
      quad<<"2";
      break;
    case 3:
      quad<<"3";
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
                                                             const ImageIORegion &pasteRegion) const
{
  typedef itk::ImageRegion<2> RegionType;
  RegionType tileMapRegion;
  RegionType::IndexType index;
  index[0] = this->GetOrigin(0);
  index[1] = this->GetOrigin(1);
  itk::ImageIORegionAdaptor<2>::Convert(pasteRegion, tileMapRegion, index);
  return m_TileMapSplitter->GetNumberOfSplits(tileMapRegion, numberOfRequestedSplits);
}

itk::ImageIORegion
TileMapImageIO::GetSplitRegionForWritingCanStreamWrite(unsigned int ithPiece,
                                                               unsigned int numberOfActualSplits,
                                                               const ImageIORegion &pasteRegion) const
{
  typedef itk::ImageRegion<2> RegionType;
  RegionType tileMapRegion;
  RegionType::IndexType index;
  index[0] = this->GetOrigin(0);
  index[1] = this->GetOrigin(1);
  itk::ImageIORegionAdaptor<2>::Convert(pasteRegion, tileMapRegion, index);
  ImageIORegion returnRegion;
  itk::ImageIORegionAdaptor<2>::Convert(m_TileMapSplitter->GetSplit(ithPiece, numberOfActualSplits, tileMapRegion), returnRegion, index);
  return returnRegion;
}

/** RGB buffer filling when the tile is not found */
void TileMapImageIO::FillCacheFaults(void* buffer) const
{
  const char * logo =
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337fedZZXZZXZZX\333\332\326"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\5\5\5T\4\6="
    "\3\4\4\4\4\340\337\333\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\330\327\323KKJ<"
    "<:<<;\261\260\255\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\336\335\331\1\1\1\253\10\14n\6\10"
    "\"\"!\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\266\265\262\36\36\36"
    "\261\261\261UUUmmk\341\340\334\322\321\316\312\312\306\312\312\306\312\312"
    "\306\312\312\306\324\323\317\343\342\336\326\325\321\0\0\0\253\11\14Z\5\6"
    "FFE\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\264\263\260+++\377\377\377"
    "zzz\7\7\6\5\5\5\4\4\4\21\21\21\23\23\23\23\23\23\20\20\20\3\3\3\7\7\7\33"
    "\33\32\0\0\0\257\11\15""3\3\4jig\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\250\250\245PPN\13"
    "\13\13;;;\377\377\377\311\311\311\300\300\300\343\343\343\372\372\372\377"
    "\377\377\377\377\377\377\377\377\377\377\377\370\370\370\337\337\337a``\0"
    "\0\0\266\12\16\15\1\1\216\216\213\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\250\247\244++*\20\20\20lll\310\310\310\375"
    "\375\375\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377hhh\0"
    "\0\0\244\11\14\0\0\0\262\261\256\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\334\333\330\\[Z\14\14\14\202\202\202\361\361\361\371\371\371\273"
    "\273\274\246\246\247\377\377\377\277\277\277\206\206\206\257\257\257\351"
    "\352\351\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377POO\1\0\0\200\7\12\0\0\0\326\325\321\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\324\323\320211777\333\333\333\324\324\324bbb\15\15\15\0\0\0...\377"
    "\377\377\214\214\214\2\2\3\2\2\3\2\2\2>>>\254\254\254\375\375\375\377\377"
    "\377\377\377\377\377\377\377766\5\0\0P\4\6\26\26\26\344\343\337\344\343\337"
    "\343\342\336\330\327\323\334\333\330\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\327\326\323--,VVV\360\360\360mmm\2\2\2\24\37-2Lo1Jm...\377\377"
    "\377\244\244\244\6\11\14Df\2241Kl\26!.\1\1\1""222\322\322\322\377\377\377"
    "\377\377\377\35\35\35\0\0\0\0\0\0\12\12\11--,\20\20\17\1\1\1\2\1\1\1\1\1"
    "\4\4\4\34\34\34""443llj\314\313\307\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\342\336EEDHHH\342\342\342111\12\17\26""6RxFj\233Fj\2334Nr.."
    ".\377\377\377\310\310\310\0\0\0Ab\216Fj\233Fj\233:X\177\20\30\"\11\11\11"
    "nnn211\0\0\0\0\0\0\0\0\0\16\2\3j\13\15\250\15\20\275\15\20\306\14\20\303"
    "\14\20\266\15\20\237\14\17~\13\15""5\7\10\14\13\13\230\227\224\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\230\230\225\25\25\25\331\331\331&&&\25\40.Be\223"
    "Fj\233Fj\233Fj\2334Nr...\377\377\377\364\364\364\4\4\4""0IiFj\233Fj\233B"
    "c\220\"2F\2\2\1\0\0\0\0\0\0\0\0\0\12\20\11\36""2\33\16\26\15\0\0\0\24\3\4"
    "d\11\13\272\15\21\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\206\14"
    "\17\11\5\5\253\252\247\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\33700/\212\212\212IIJ\17\27!Dg\227"
    "Fj\233Fj\233Fj\233Fj\2334Nr...\377\377\377\377\377\377BBB\25\37,Fj\2330E"
    "c\7\11\14\7\12\16!1F+@]\4\5\10\0\0\0""2V0p\312op\312ob\255`:b7\13\17\11\21"
    "\3\3\225\14\17\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20i\12\14$"
    "$$\342\341\335\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\323\322\317\2\2\2\223\223\223\3\5\7>^\212Fj\233Fj\233Fj\233"
    "Fj\233Fj\2334Nr...\377\377\377\377\377\377\270\271\271\1\1\1\26\37+\5\7\12"
    "(<VDf\225Fj\233Fj\2330Hi\0\0\0""9e7p\312op\312op\312op\312ol\300j-H)\6\2"
    "\2\222\14\17\313\14\20\313\14\20\313\14\20\313\14\20\302\15\21\16\3\3\251"
    "\250\246\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\275\274\271\33\33\33""222!2IFj\233Fj\233Fj\233Fj\233Fj\233Fj\233"
    "4Nr...\377\377\377\377\377\377\363\363\363\32\32\31\1\1\1""4NoFj\233Fj\233"
    "Fj\233Fj\2333Lm\0\0\0:f8p\312op\312op\312op\312op\312op\312o3S/\12\2\2\255"
    "\15\20\313\14\20\313\14\20\313\14\20\313\14\20E\7\10qpn\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\276\275\272\3\3\3\4"
    "\6\11?_\213Fj\233Fj\233Fj\233Fj\233Fj\233Fj\2334Nr...\377\377\377\374\373"
    "\373FEELKK???\3\5\7#5K;Y\2006Rw\37.B\3\4\5\0\0\0.P,p\312op\312op\312op\312"
    "op\312op\312oo\307m\30%\26A\7\10\313\14\20\313\14\20\313\14\20\313\14\20"
    "l\11\13TSR\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\226\225\222!0D=\\\207Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233"
    "4Nr+++\377\377\377\206\205\205GFF\374\374\374\372\373\372\204\205\204\25"
    "\25\25\0\0\0\0\0\0\"#\"EDD\0\0\0\31(\26p\312op\312op\312op\312op\312op\312"
    "op\312oK\203I\35\3\4\313\14\20\313\14\20\313\14\20\313\14\20p\11\13SRQ\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337yyv/"
    "GhFj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\2337Sz\14\14\14\346\345\345"
    ",++\357\356\356\377\377\377\377\377\377\377\377\377\373\373\373\331\331\331"
    "\332\332\332\375\376\376\206\205\205\6\3\3\2\2\1d\257ap\312op\312op\312o"
    "p\312op\312op\312oP\216N!\3\4\313\14\20\313\14\20\313\14\20\313\14\20Q\10"
    "\12fec\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337yxv0IkFj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Df\225\2\3\5SR"
    "S\267\266\266\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377qpp<\6\7\34\4\5,H)p\312op\312op\312o"
    "p\312op\312op\312o-N,=\5\6\313\14\20\313\14\20\313\14\20\313\14\20\37\5\6"
    "\233\232\230\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337~}{-EeFj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233"
    "\23\35+/..\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377]\\\\P\6\10\221\14\17\2\1\1>"
    "f:p\310np\312op\312oo\307nDr@\2\1\1\213\13\15\313\14\20\313\14\20\313\14"
    "\20\254\14\17\4\4\4\323\322\316\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\232\231\227#4MFj\233Fj\233Fj\233Fj\233Fj\233"
    "Fj\233Fj\233Fj\233Fj\2335Pv\6\5\6\312\312\312\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377IHH`"
    "\7\11\313\14\20\177\14\16\4\1\1\26\"\24""5Y2.M,\17\26\15\3\1\1p\12\14\313"
    "\14\20\313\14\20\313\14\20\313\14\20V\10\12DDC\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\315\314\310\23\33"
    "'Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233\36.D\37\37"
    "\37\345\345\346\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377433p\10\12\313\14\20\313\14\20\250\15\20Y\10\12C"
    "\7\10F\7\10h\11\13\260\15\20\313\14\20\313\14\20\313\14\20\313\14\20\261"
    "\15\20\7\1\1\256\255\252\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337:::=\\\207Fj\233Fj\233Fj"
    "\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Eh\230\26!1\40\40\40\304\304\304"
    "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
    "\40\37\37\200\11\13\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313"
    "\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\312\14\20""8\7\10"
    "NML\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\226\226\223\34+>Fj\233Fj\233Fj\233F"
    "j\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Dh\230\37/D\2\2\3QQQ\256\256\256"
    "\344\344\344\361\361\361\335\335\335\247\247\247\11\10\10r\7\11\313\14\20"
    "\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313"
    "\14\20\313\14\20\313\14\20j\13\15\32\32\31\323\322\316\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\342\341\3359982LpFj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj"
    "\233Fj\233Fj\233Fj\233Fj\2339V}\33)<\7\13\20\0\0\1\0\0\0\1\1\2\11\15\23\16"
    "\25\36*\4\5\312\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\313"
    "\14\20\313\14\20\313\14\20\313\14\20s\14\16\17\16\16\265\264\261\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343"
    "\337\344\343\337\344\343\337\344\343\337\344\343\337\312\311\306\31\34\37"
    ";Z\203Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233"
    "Fj\233Fj\233Dg\227Dg\227Eh\230Ei\232\"-;\4\4\3w\14\16\313\14\20\313\14\20"
    "\313\14\20\313\14\20\313\14\20\313\14\20\313\14\20\271\15\21E\11\12\22\22"
    "\21\263\262\257\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\263\262\257\24\30\36""8U}Fj\233Fj\233Fj\233Fj\233"
    "Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Dg\226\30$4g"
    "fd\250\247\244\13\12\12Y\11\13\266\16\21\313\14\20\313\14\20\313\14\20\262"
    "\16\21d\12\14\15\3\3>=<\314\313\307\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\274"
    "\273\270$%&(<XEi\231Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233Fj\233"
    "Fj\233Fj\2339V~\22\31\"}}z\344\343\337\344\343\337\267\266\263876\4\2\2+"
    "\5\5A\5\6&\5\6\3\2\2""887\236\236\233\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\331\330\324hhf\23\30\37*?]Ac\220Fj\233"
    "Fj\233Fj\233Fj\233Fj\233Fj\233Ei\2324Os\27\"2998\266\265\262\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\305\304\300\224\223\220"
    "\210\210\206\232\231\226\313\312\307\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337"
    "\344\343\337\316\315\312xwu000\21\30\"\33)<!2H!3J\35,@\24\36,\34\36!YXW\255"
    "\254\251\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\331\330\324\270\267\264\250\247\244\247\246\243\263"
    "\262\257\314\313\307\344\342\336\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344\343\337\344"
    "\343\337\344\343\337\344\343\337\344\343\337";

  //replicate to make 256x256x3 pixels
  for (int line=0; line<256; line++)
  {
    memcpy(((unsigned char *) buffer)+line*256*3, logo+(line % 64)*64*3, 64*3);
    memcpy(((unsigned char *) buffer)+line*256*3+64*3, logo+(line % 64)*64*3, 64*3);
    memcpy(((unsigned char *) buffer)+line*256*3+64*3*2, logo+(line % 64)*64*3, 64*3);
    memcpy(((unsigned char *) buffer)+line*256*3+64*3*3, logo+(line % 64)*64*3, 64*3);
  }

}

} // end namespace otb
