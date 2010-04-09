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
#ifndef __otbTileMapFetcher_h
#define __otbTileMapFetcher_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <deque>
#include <string>

#include "otbVectorImage.h"
#include "otbTileRecord.h"

/* Curl Library*/
#include <curl/curl.h>

namespace otb
{
/** \class TileMapFetcher
 * \brief Get tiles from servers like Open street map.
 *
 */
class ITK_EXPORT TileMapFetcher : public itk::Object
{
public:
  /** Standard typedefs */
  typedef TileMapFetcher                Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef unsigned char                  PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef ImageType::Pointer             ImagePointerType;

  itkNewMacro(Self);

  /** Constructor */
  TileMapFetcher();

  /** Destructor */
  virtual ~TileMapFetcher();

  /** Accessors */
  itkSetStringMacro(ServerName);
  itkGetStringMacro(ServerName);

  itkSetStringMacro(CacheDirectory);
  itkGetStringMacro(CacheDirectory);

  itkSetMacro(CacheSize, unsigned int);
  itkGetMacro(CacheSize, unsigned int);

  /** Fetcher */
  ImagePointerType FetchTile(unsigned int x, unsigned int y, unsigned int z);

protected:

private:
  /** Generate filename */
  void GenerateFileName();

  /** Generate OSM URL */
  void GenerateOSMurl();

  /** Check if file is in cache */
  bool IsTileInCache();

  /** Fetch Tile from Server */
//    bool FetchTileFromServer(bool isInCache);   // Comment because cache is not persistant
  bool FetchTileFromServer();

  /** Read Tile From Cache */
  ImagePointerType ReadTileFromCache(bool fetchingIsOk);

  /** Register Tile in cache */
  bool RegisterTileInCache();

  /** Unregister cached tile */
  bool UnregisterFirstCachedTile();

  /** Unregister All Cached Tiles */
  bool UnregisterAllCachedTiles();

  /** Generate Tile with logo */
  void GenerateLogoTile();

  /** Fill buffer with a logo */
  void LogoToBuffer(PixelType* buffer);

  /** File suffix */
  std::string m_FileSuffix;

  /** Server Name */
  std::string m_ServerName;

  /** Cache directory */
  std::string m_CacheDirectory;

  /** Cache Size */
  unsigned int m_CacheSize;

  /** Coordinates */
  unsigned int m_X;
  unsigned int m_Y;
  unsigned int m_Z;

  /** FileName */
  std::string m_FileName;

  /** OSM Url */
  std::string m_OsmUrl;

  /** Cache Management */
  std::deque<otb::TileRecord> m_CacheManagement;

  /** Logo tile */
  ImagePointerType m_LogoTile;

};

} // end namespace otb

#endif
