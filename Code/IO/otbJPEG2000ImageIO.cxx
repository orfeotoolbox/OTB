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
#include "otbJPEG2000ImageIO.h"

#include <iomanip>
#include <iostream>
#include <fstream>

#include "otbMacro.h"
#include "otbSystem.h"

#include "itkTimeProbe.h"
#include "itkMacro.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

#include <deque>

extern "C"
{
#include "openjpeg.h"
}

#include "gdal.h"
#include "gdaljp2metadata.h"
#include "cpl_string.h"
#include "ogr_spatialref.h"
#include "ogr_srs_api.h"

#include "itksys/SystemTools.hxx"

#include "otbTinyXML.h"

#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

void OpjImageDestroy(opj_image_t * img)
{
  if(img)
    {
    otbopenjpeg_opj_image_destroy(img);
    }
}

void OpjCodestreamDestroy(opj_codestream_info_v2 * cstr)
{
  otbopenjpeg_opj_destroy_cstr_info_v2(&cstr);
}

/**
Divide an integer by a power of 2 and round upwards
@return Returns a divided by 2^b
*/
inline int int_ceildivpow2(int a, int b) {
  return (a + (1 << b) - 1) >> b;
}

inline unsigned int uint_ceildivpow2(unsigned int a, unsigned int b) {
  return (a + (1 << b) - 1) >> b;
}

/**
   sample error debug callback expecting no client object
*/
void error_callback(const char *msg, void *client_data)
{
  (void) msg;
  (void) client_data;
  otbMsgDevMacro(<< "OpenJPEG error: " << msg);
}
/**
   sample warning debug callback expecting no client object
*/
void warning_callback(const char *msg, void *client_data)
{
  (void) msg;
  (void) client_data;
  otbMsgDevMacro(<< "OpenJPEG warning: " << msg);
}
/**
   sample debug callback expecting no client object
*/
void info_callback(const char *msg, void *client_data)
{
  (void) msg;
  (void) client_data;
  otbMsgDevMacro(<< "OpenJPEG info: " << msg);
}

namespace otb
{

/** Compute offsets needed to read the data from the tile decoded and
 * offsets needed to write into the output buffer.
 *
 * Please note that this function uses a bare pointer instead of a
 * shared one because it is called within LoadTileData, which hides
 * the opj_image_t pointer between a void * for proper hiding of
 * openjpeg API.
 */
void ComputeOffsets(opj_image_t * tile,
                    const itk::ImageIORegion & ioRegion,
                    unsigned int &l_width_src,
                    unsigned int &l_height_dest,
                    unsigned int &l_width_dest,
                    unsigned int &l_start_offset_dest,
                    unsigned int &l_start_offset_src);

/************************************************************************/
/*     JPEG2000 metadata reader based on GDAL functionalities           */
/************************************************************************/
class JPEG2000MetadataReader
{
public:
  JPEG2000MetadataReader(const char *filename )
  {
  m_MetadataIsRead = false;

  if( m_JP2Metadata.ReadAndParse(filename) )
    {
    m_MetadataIsRead = true;
    }

  };

  ~JPEG2000MetadataReader(){};

  /** Get the geoTransform from file*/
  std::vector<double> GetGeoTransform(){
    std::vector<double> geoTransform;
    for (unsigned int i = 0; i< 6; i++ )
      geoTransform.push_back(m_JP2Metadata.adfGeoTransform[i]);
    return geoTransform;
  };

  /** Check if image in the file have a geoTransform*/
  bool HaveGeoTransform(){
    return static_cast<bool>(m_JP2Metadata.bHaveGeoTransform);
  };

  /** Get the nb of GCP from file*/
  int GetGCPCount(){
    return m_JP2Metadata.nGCPCount;
  };

  /** Get the GCPs from file*/
  std::vector<GDAL_GCP> GetGCPs(){
    std::vector<GDAL_GCP> gcps;
    int nbGCP = m_JP2Metadata.nGCPCount;
    for (int i = 0; i< nbGCP; i++ )
      gcps.push_back(m_JP2Metadata.pasGCPList[i]);
    return gcps;
  };

  /** Get the projectionRef from file*/
  const char* GetProjectionRef() {
    if (m_JP2Metadata.pszProjection)
      return m_JP2Metadata.pszProjection;
    else
      return NULL;
  };

  /** Get the GML box from file*/
  char** GetGMLMetadata() {
    if (m_JP2Metadata.papszGMLMetadata)
      return m_JP2Metadata.papszGMLMetadata;
    else
      return NULL;
  };

  // Get the origin from GML box (use tinyxml to parse the GML box)
  bool GetOriginFromGMLBox (std::vector<double> &origin);

  /** Check if the file has been correctly read*/
  bool m_MetadataIsRead;

private:
  /** GDAL structure where store metadata read from JP2 file*/
  GDALJP2Metadata m_JP2Metadata;

};

bool JPEG2000MetadataReader::GetOriginFromGMLBox (std::vector<double> &origin)
{
  if (!m_MetadataIsRead)
    return false;

  std::string gmlString = static_cast<std::string> (m_JP2Metadata.papszGMLMetadata[0]);
  gmlString.erase(0,18); // We need to remove first part to create a true xml stream
  otbMsgDevMacro( << "XML extract from GML box: " << gmlString );

  TiXmlDocument doc;
  doc.Parse(gmlString.c_str()); // Create xml doc from a string

  TiXmlHandle docHandle( &doc );
  TiXmlElement* originTag = docHandle.FirstChild( "gml:FeatureCollection" )
                                     .FirstChild( "gml:featureMember" )
                                     .FirstChild( "gml:FeatureCollection" )
                                     .FirstChild( "gml:featureMember" )
                                     .FirstChild( "gml:GridCoverage" )
                                     .FirstChild( "gml:gridDomain")
                                     .FirstChild( "gml:Grid" )
                                     .FirstChild( "gml:limits" )
                                     .FirstChild( "gml:GridEnvelope" )
                                     .FirstChild( "gml:low").ToElement();
  if(originTag)
    {
    otbMsgDevMacro( << "\t Origin (" <<  originTag->Value() <<" tag)= "<<  originTag->GetText());
    }
  else
    {
    otbMsgDevMacro( << "Didn't find the GML element which indicate the origin!" );
    return false;
    }

  std::vector<itksys::String> originValues;
  originValues = itksys::SystemTools::SplitString(originTag->GetText(),' ', false);

  std::istringstream ss0 (originValues[0]);
  std::istringstream ss1 (originValues[1]);
  ss0 >> origin[1];
  ss1 >> origin[0];
  origin[0]--;
  origin[1]--;

  otbMsgDevMacro( << "\t Origin from GML box: " <<  origin[0] << ", " << origin[1] );

  return true;
}


/************************************************************************/
/*            JPEG2000 internal reader based on openjpeg                */
/************************************************************************/
class JPEG2000InternalReader
{
public:
  JPEG2000InternalReader();

  ~JPEG2000InternalReader()
  {
    this->Clean();
  }
  
  boost::shared_ptr<opj_image_t> DecodeTile(unsigned int tileIndex);

  const std::vector<unsigned int> & GetAvailableResolutions(){return this->m_AvailableResolutions; };

  void Clean();

  int CanRead();

  int Open(const char *filename, unsigned int resolution);

  bool m_IsOpen;
  OPJ_CODEC_FORMAT m_CodecFormat;

  unsigned int         m_Width;
  unsigned int         m_Height;

  unsigned int         m_NbOfComponent;

  std::vector<unsigned int>        m_XResolution;
  std::vector<unsigned int>        m_YResolution;
  std::vector<unsigned int>        m_Precision;
  std::vector<int>                 m_Signed;

  unsigned int         m_TileWidth;
  unsigned int         m_TileHeight;
  unsigned int         m_XNbOfTile;
  unsigned int         m_YNbOfTile;
  
  std::vector<unsigned int> m_AvailableResolutions;

  unsigned int m_ResolutionFactor;

private:
  boost::shared_ptr<FILE> m_File;
  int Initialize();
};


int JPEG2000InternalReader::Open(const char *filename, unsigned int resolution)
{
  this->Clean();

  // Open the file
  this->m_File  = boost::shared_ptr<FILE>(fopen(filename, "rb"),fclose);
  if (!this->m_File)
    {
    this->Clean();
    return 0;
    }

  // Find the codec file format
  std::string lFileName(filename);

  if (System::SetToLower(System::GetExtension(lFileName)) == "j2k")
    {
    this->m_CodecFormat = CODEC_J2K;
    }
  else if (System::SetToLower(System::GetExtension(lFileName)) == "jp2"
           || System::SetToLower(System::GetExtension(lFileName)) == "jpx")
    {
    this->m_CodecFormat = CODEC_JP2;
    }
  else
    {
    this->Clean();
    return 0;
    }

  this->m_ResolutionFactor = resolution;

  // Initialize the codec and the stream
  if (!this->Initialize())
    {
    this->Clean();
    return 0;
    }

  this->m_IsOpen = true;
  return 1;
}

void JPEG2000InternalReader::Clean()
{
  m_File = boost::shared_ptr<FILE>();

  this->m_XResolution.clear();
  this->m_YResolution.clear();
  this->m_Precision.clear();
  this->m_Signed.clear();
  m_AvailableResolutions.clear();

  this->m_Width = 0;
  this->m_Height = 0;
  this->m_NbOfComponent = 0;
  this->m_TileWidth = 0;
  this->m_TileHeight = 0;
  this->m_XNbOfTile = 0;
  this->m_YNbOfTile = 0;

  this->m_IsOpen = false;
  this->m_CodecFormat = CODEC_UNKNOWN;
}

boost::shared_ptr<opj_image_t> JPEG2000InternalReader::DecodeTile(unsigned int tileIndex)
{

  if (!m_File)
    {
    this->Clean();
    return boost::shared_ptr<opj_image_t>();
    }

  // Creating the file stream
  boost::shared_ptr<opj_stream_t> stream = boost::shared_ptr<opj_stream_t>(otbopenjpeg_opj_stream_create_default_file_stream(m_File.get(), true),otbopenjpeg_opj_stream_destroy);
  if (!stream)
    {
    this->Clean();
    return boost::shared_ptr<opj_image_t>();
    }
  
  // Creating the codec
  boost::shared_ptr<opj_codec_t> codec = boost::shared_ptr<opj_codec_t>(otbopenjpeg_opj_create_decompress_v2(this->m_CodecFormat),otbopenjpeg_opj_destroy_codec);
  
  if (!codec)
    {
    this->Clean();
    return boost::shared_ptr<opj_image_t>();
    }
  
  // Setting default parameters
  opj_dparameters_t parameters;
  otbopenjpeg_opj_set_default_decoder_parameters(&parameters);
  parameters.cp_reduce = static_cast<int>(this->m_ResolutionFactor);
  
  // Set default event mgr
  opj_event_mgr_t eventManager;
  eventManager.info_handler = info_callback;
  eventManager.warning_handler = warning_callback;
  eventManager.error_handler = error_callback;
  
  // Setup the decoder decoding parameters using user parameters
  if (!otbopenjpeg_opj_setup_decoder_v2(codec.get(), &parameters, &eventManager))
    {
    this->Clean();
    return boost::shared_ptr<opj_image_t>();
    }
  
  // Read the main header of the codestream and if necessary the JP2
  // boxes
  opj_image_t * unsafeOpjImgPtr = NULL;
  
  if (!otbopenjpeg_opj_read_header(stream.get(), codec.get(),&unsafeOpjImgPtr))
    {
    boost::shared_ptr<opj_image_t> image = boost::shared_ptr<opj_image_t>(unsafeOpjImgPtr,OpjImageDestroy);
    this->Clean();
    return boost::shared_ptr<opj_image_t>();
    }
  
  boost::shared_ptr<opj_image_t> image = boost::shared_ptr<opj_image_t>(unsafeOpjImgPtr,OpjImageDestroy);

  if( otbopenjpeg_opj_get_decoded_tile(codec.get(), stream.get(), image.get(), tileIndex))
    {
    otbMsgDevMacro(<<"Tile "<<tileIndex<<" read from file");

    return image;
    }
  else
    {
    this->Clean();
    return boost::shared_ptr<opj_image_t>();
    }
}

JPEG2000InternalReader::JPEG2000InternalReader()
{
  this->Clean();
}

int JPEG2000InternalReader::Initialize()
{
  if (m_File)
    {
    // Creating the file stream
    boost::shared_ptr<opj_stream_t> stream = boost::shared_ptr<opj_stream_t>(otbopenjpeg_opj_stream_create_default_file_stream(m_File.get(), true),otbopenjpeg_opj_stream_destroy);
    if (!stream)
      {
      this->Clean();
      return 0;
      }

    // Creating the codec
    boost::shared_ptr<opj_codec_t> codec = boost::shared_ptr<opj_codec_t>(otbopenjpeg_opj_create_decompress_v2(this->m_CodecFormat),otbopenjpeg_opj_destroy_codec);

    if (!codec)
      {
      this->Clean();
      return 0;
      }

    // Setting default parameters
    opj_dparameters_t parameters;
    otbopenjpeg_opj_set_default_decoder_parameters(&parameters);
    parameters.cp_reduce = static_cast<int>(this->m_ResolutionFactor);

    // Set default event mgr
    opj_event_mgr_t eventManager;
    eventManager.info_handler = info_callback;
    eventManager.warning_handler = warning_callback;
    eventManager.error_handler = error_callback;

    // Setup the decoder decoding parameters using user parameters
    if (!otbopenjpeg_opj_setup_decoder_v2(codec.get(), &parameters, &eventManager))
      {
      this->Clean();
      return 0;
      }

    // Read the main header of the codestream and if necessary the JP2
    // boxes
    opj_image_t * unsafeOpjImgPtr = NULL;

    if (!otbopenjpeg_opj_read_header(stream.get(), codec.get(),&unsafeOpjImgPtr))
      {
      boost::shared_ptr<opj_image_t> image = boost::shared_ptr<opj_image_t>(unsafeOpjImgPtr,OpjImageDestroy);
      this->Clean();
      return 0;
      }

    boost::shared_ptr<opj_image_t> image = boost::shared_ptr<opj_image_t>(unsafeOpjImgPtr,OpjImageDestroy);

    // Get the codestream information
    boost::shared_ptr<opj_codestream_info_v2> cstrInfo = boost::shared_ptr<opj_codestream_info_v2>(otbopenjpeg_opj_get_cstr_info(codec.get()),OpjCodestreamDestroy);

    if (!cstrInfo)
      {
      std::cerr << "ERROR while get codestream info" << std::endl;
      this->Clean();
      return 0;
      }

    // We can now retrieve the main information of the image and the codestream
    // (based on the first component and with no subsampling)
    this->m_Width = image->comps->w;
    this->m_Height = image->comps->h;

    otbMsgDevMacro(<< "JPEG2000InternalReader dimension (after reading header) = " << image->comps->w << " x "
                   << image->comps->w << " x " << image->comps->h );

    this->m_TileHeight = cstrInfo->tdy;
    this->m_TileWidth = cstrInfo->tdx;
    this->m_XNbOfTile = cstrInfo->tw;
    this->m_YNbOfTile = cstrInfo->th;

    this->m_NbOfComponent = image->numcomps;

    for (unsigned int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_Precision.push_back( image->comps[itComp].prec);
      this->m_Signed.push_back( image->comps[itComp].sgnd);
      this->m_XResolution.push_back( image->comps[itComp].dx);
      this->m_YResolution.push_back( image->comps[itComp].dy);
      }

    // Warning: This value is based on the first component of the default tile parameters.
    unsigned int numResAvailable = cstrInfo->m_default_tile_info.tccp_info[0].numresolutions;
    for (unsigned int itRes = 0; itRes < numResAvailable; itRes++)
      {
      m_AvailableResolutions.push_back(itRes);
      }
    }

  return 1;
}


int JPEG2000InternalReader::CanRead()
 {
   if  (Initialize() &&
       ( this->m_Width > 0 ) && ( this->m_Height > 0 ) &&
       ( this->m_TileWidth > 0 ) && ( this->m_TileHeight > 0 ) &&
       ( this->m_XNbOfTile > 0 ) && ( this->m_YNbOfTile > 0 ) &&
       ( this->m_NbOfComponent > 0 ) )
     {

     // We manage only JPEG2000 file with characteristics which are equal between components
     for(unsigned int itComp = 0; itComp < this->m_NbOfComponent - 1; itComp++)
       {
       if ( (this->m_Precision[itComp] != this->m_Precision[itComp+1]) &&
            (this->m_Signed[itComp] != this->m_Signed[itComp+1]) &&
            (this->m_XResolution[itComp] != this->m_XResolution[itComp+1]) &&
            (!this->m_XResolution[itComp]) &&
            (this->m_YResolution[itComp] != this->m_YResolution[itComp+1]) &&
            (!this->m_YResolution[itComp]) )
         {
         return 0;
         }
       }

     return 1;
     }
   else return 0;
 }

/************************************************************************/
/*            Class to manage JPEG2000 tile cache system                */
/************************************************************************/
class JPEG2000TileCache
{
public:
  JPEG2000TileCache();
  ~JPEG2000TileCache();

  typedef std::pair<unsigned int, boost::shared_ptr<opj_image_t> > CachedTileType;
  typedef std::deque<CachedTileType> TileCacheType;

  /** Get a tile in cache, return null if cache does not contain the
  tile */
  boost::shared_ptr<opj_image_t> GetTile(unsigned int tileIndex);

  /** Register a new tile in cache */
  void AddTile(unsigned int tileIndex, boost::shared_ptr<opj_image_t> tileData);

  /** Remove the front tile */
  void RemoveOneTile();

  /** Clear the cache */
  void Clear();

  /** Initialize some parameters linked to the cache size in memory*/
  void Initialize(unsigned int originalWidthTile, unsigned int originalHeightTile,
                  unsigned int nbComponent,
                  unsigned int precision,
                  unsigned int resolution)
  {
    this->EstimateTileCacheSize(originalWidthTile, originalHeightTile,
                                nbComponent,
                                precision,
                                resolution);
    m_CacheSizeInByte = m_CacheSizeInTiles * m_TileCacheSizeInByte;
    m_IsReady = true;
  };

  /** Set the size of the cache with in terms of number of tiles */
  void SetCacheSizeInTiles(unsigned int nbOfTiles)
  {
    if (nbOfTiles > 0 && m_IsReady)
      {
      m_CacheSizeInTiles = nbOfTiles;
      m_CacheSizeInByte = m_CacheSizeInTiles * m_TileCacheSizeInByte;
      }
  };

  /** Get the size of the cache in terms of number of tiles */
  unsigned int GetCacheSizeInTiles() {return m_CacheSizeInTiles; };

  /** Set the size of the cache with in terms of Bytes */
  void SetCacheSizeInByte(unsigned int sizeInByte)
  {
    if (sizeInByte > 0 && m_IsReady)
      {
      m_CacheSizeInByte = sizeInByte;
      if (m_TileCacheSizeInByte)
        m_CacheSizeInTiles = m_CacheSizeInByte / m_TileCacheSizeInByte;
      else
        m_CacheSizeInTiles = 0;
      }
  };

  unsigned int GetCurrentNumberOfTileInCache()
  {
    return static_cast<unsigned int>(m_Cache.size());
  };

private:
  TileCacheType m_Cache;
  unsigned int m_CacheSizeInTiles;
  unsigned int m_CacheSizeInByte;
  unsigned int m_TileCacheSizeInByte;
  bool m_IsReady;

  itk::SimpleMutexLock m_Mutex;

  /** Estimate the size of a tile in memory*/
  void EstimateTileCacheSize(unsigned int originalWidthTile, unsigned int originalHeightTile,
                             unsigned int nbComponent,
                             unsigned int precision,
                             unsigned int resolution);

};

JPEG2000TileCache::JPEG2000TileCache() : m_Cache(), m_CacheSizeInTiles(4), m_CacheSizeInByte(0), m_IsReady(false)
{}

JPEG2000TileCache::~JPEG2000TileCache()
{
  this->Clear();
}


void JPEG2000TileCache::EstimateTileCacheSize(unsigned int originalWidthTile, unsigned int originalHeightTile,
                                              unsigned int nbComponent,
                                              unsigned int precision,
                                              unsigned int resolution)
{
  this->m_TileCacheSizeInByte = originalWidthTile * originalHeightTile
                              * nbComponent
                              * sizeof(OPJ_INT32)
                              / vcl_pow(2, 2*static_cast<double>(resolution) );

  if (!this->m_TileCacheSizeInByte)
    {
    otbMsgDevMacro( << "TileCacheSizeInByte is estimated at " << m_TileCacheSizeInByte
                    << " bytes so we don't used the cache");
    }

  otbMsgDevMacro( << "m_TileCacheSizeInByte = " << m_TileCacheSizeInByte );
}

void JPEG2000TileCache::Clear()
{
  m_Cache.clear();
}


boost::shared_ptr<opj_image_t> JPEG2000TileCache::GetTile(unsigned int tileIndex)
{
  for(TileCacheType::iterator it = m_Cache.begin();
      it != m_Cache.end(); ++it)
    {
    if(it->first == tileIndex)
      {
      otbMsgDevMacro(<<"Tile "<<it->first<<" loaded from cache");
      return it->second;
      }
    }
  return boost::shared_ptr<opj_image_t>();
}

void JPEG2000TileCache::RemoveOneTile()
{
  if(!m_Cache.empty())
    {
    m_Cache.pop_front();
    }
}

void JPEG2000TileCache::AddTile(unsigned int tileIndex, boost::shared_ptr<opj_image_t> tileData)
{
  // This helper class makes sure the Mutex is unlocked
  // in the event an exception is thrown.
  itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder(m_Mutex);
  
  if(!m_IsReady)
    {
    std::cerr<<(this)<<" Cache is not configured !"<<std::endl;
    }

  for(TileCacheType::const_iterator it = m_Cache.begin();
      it != m_Cache.end(); ++it)
    {
    if(it->first == tileIndex)
      {
      return;
      }
    }
  if(m_Cache.size() >= m_CacheSizeInTiles)
    {
    this->RemoveOneTile();
    }
  m_Cache.push_back(CachedTileType(tileIndex, tileData));
}

/************************************************************************/
/*                     JPEG2000ImageIO                                  */
/************************************************************************/
JPEG2000ImageIO::JPEG2000ImageIO()
{
  // Initialize multi-threader
  m_Threader = itk::MultiThreader::New();
  m_NumberOfThreads = m_Threader->GetNumberOfThreads();

  for(int i = 0; i<m_NumberOfThreads; ++i)
    {
    m_InternalReaders.push_back(boost::shared_ptr<JPEG2000InternalReader>(new JPEG2000InternalReader));
    }
  m_TileCache = boost::shared_ptr<JPEG2000TileCache>(new JPEG2000TileCache);

  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_BytePerPixel = 1;
  m_ResolutionFactor = 0; // Full resolution by default

  m_CacheSizeInByte = 0; // By default no cache
}

JPEG2000ImageIO::~JPEG2000ImageIO()
{}

bool JPEG2000ImageIO::CanReadFile(const char* filename)
{
  if (filename == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  bool success = true;

  for(ReaderVectorType::iterator it = m_InternalReaders.begin();
      it != m_InternalReaders.end();
      ++it)
    {

    if ( !(*it)->Open(filename, m_ResolutionFactor) )
      {
      success = false;
      }
    }

  // If one of the readers fails, clean everything
  if(!success)
    {
    for(ReaderVectorType::iterator it = m_InternalReaders.begin();
        it != m_InternalReaders.end();
        ++it)
      {
      (*it)->Clean();
      }
    }
  return success;
}

// Used to print information about this object
void JPEG2000ImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Resolution Factor: " << m_ResolutionFactor << "\n";
}

// Read a 3D image not implemented yet
void JPEG2000ImageIO::ReadVolume(void*)
{
}

/** Internal structure used for passing image data into the threading library */
struct ThreadStruct
{
  std::vector<boost::shared_ptr<JPEG2000InternalReader> > Readers;
  std::vector<JPEG2000TileCache::CachedTileType> * Tiles;
  JPEG2000ImageIO::Pointer IO;
  boost::shared_ptr<JPEG2000TileCache> Cache;
  void * Buffer;
};


/** Get Info about all resolution in jpeg2000 file */
bool JPEG2000ImageIO::GetResolutionInfo(std::vector<unsigned int>& res, std::vector<std::string>& desc)
{
  res = this->m_InternalReaders[0]->GetAvailableResolutions();

  if (res.empty())
    return false;

  int originalWidth = m_InternalReaders[0]->m_Width;
  int originalHeight = m_InternalReaders[0]->m_Height;

  for (std::vector<unsigned int>::iterator itRes = res.begin(); itRes < res.end(); itRes++)
    {
    // For each resolution we will compute the tile dim and image dim
    std::ostringstream oss;

    int w = int_ceildivpow2( originalWidth, *itRes);
    int h = int_ceildivpow2( originalHeight, *itRes);

    int tw = int_ceildivpow2(m_InternalReaders[0]->m_TileWidth, *itRes);
    int th = int_ceildivpow2(m_InternalReaders[0]->m_TileHeight, *itRes);

    oss << "Resolution: " << *itRes << " (Image [w x h]: " << w << "x" << h << ", Tile [w x h]: " << tw << "x" << th << ")";

    desc.push_back(oss.str());
    }


  return true;
}

bool JPEG2000ImageIO::GetAvailableResolutions(std::vector<unsigned int>& res)
{
  res = this->m_InternalReaders[0]->GetAvailableResolutions();

  if (res.empty())
    return false;

  return true;
}

unsigned int JPEG2000ImageIO::GetNumberOfOverviews()
{
  std::vector<unsigned int> tempResList = this->m_InternalReaders[0]->GetAvailableResolutions();

  if (tempResList.empty())
    {
    itkExceptionMacro(<< "Available resolutions in JPEG2000 is empty");
    }

  return tempResList.size() - 1;
}


// Read image
void JPEG2000ImageIO::Read(void* buffer)
{
  itk::TimeProbe chrono;
  chrono.Start();

  // Check if conversion succeed
  if (buffer == NULL)
    {
    itkExceptionMacro(<< "JPEG2000ImageIO : Bad alloc");
    return;
    }

  // Re-open the file if it was closed
  bool open = true;
  for(ReaderVectorType::iterator it = m_InternalReaders.begin();
        it != m_InternalReaders.end();
        ++it)
      {
      open = (*it)->m_IsOpen && open;
      }
  
  if ( !open )
    {
    if ( !this->CanReadFile( m_FileName.c_str() ) )
      {
      itkExceptionMacro(<< "Cannot open file " << this->m_FileName << "!");
      return;
      }
    }

  if (m_ResolutionFactor >= this->m_InternalReaders[0]->m_AvailableResolutions.size())
    {
    itkExceptionMacro(<< "Resolution not available in the file!");
    return;
    }

  std::vector<unsigned int> tileList = this->ComputeTileList();
  if (tileList.empty())
    {
    for(ReaderVectorType::iterator it = m_InternalReaders.begin();
        it != m_InternalReaders.end();
        ++it)
      {
      (*it)->Clean();
      }
    itkExceptionMacro(<< " IORegion does not correspond to any tile!");
    }

  // Here we sort between tiles from cache and tiles to read
  std::vector<JPEG2000TileCache::CachedTileType> cachedTiles;
  std::vector<JPEG2000TileCache::CachedTileType> toReadTiles;

  for (std::vector<unsigned int>::iterator itTile = tileList.begin(); itTile < tileList.end(); ++itTile)
    {
    boost::shared_ptr<opj_image_t> currentImage(m_TileCache->GetTile(*itTile));

    JPEG2000TileCache::CachedTileType currentTile = std::make_pair((*itTile), currentImage);

    if(!currentImage)
      {
      toReadTiles.push_back(currentTile);
      }
    else
      {
      cachedTiles.push_back(currentTile);
      }
    }

  // First, load tiles from cache
  for (std::vector<JPEG2000TileCache::CachedTileType>::iterator itTile = cachedTiles.begin(); itTile < cachedTiles.end(); ++itTile)
    {
    this->LoadTileData(buffer, itTile->second.get());
    }

  // If we will read more tiles than the cache size, clear it
  if(toReadTiles.size() > m_TileCache->GetCacheSizeInTiles())
    {
    m_TileCache->Clear();
    }
  // Else if there is not enough rooms from new tiles
  else if(toReadTiles.size() > (m_TileCache->GetCacheSizeInTiles() - m_TileCache->GetCurrentNumberOfTileInCache()))
    {
    int nbTileToRemove = toReadTiles.size() - (m_TileCache->GetCacheSizeInTiles() - m_TileCache->GetCurrentNumberOfTileInCache());
    // Remove from cache as many tiles that will be read in this step
    for (int itTileR = 0; itTileR < nbTileToRemove; ++itTileR)
      {
      m_TileCache->RemoveOneTile();
      }
    }

  // Decode all tiles not in cache in parallel
  if(!toReadTiles.empty())
    {
    unsigned int nbThreads = itk::MultiThreader::GetGlobalDefaultNumberOfThreads();
    if (nbThreads > toReadTiles.size())
      {
      nbThreads = toReadTiles.size();
      }
    this->GetMultiThreader()->SetNumberOfThreads(nbThreads);

    // Set up the multithreaded processing
    ThreadStruct str;
    str.Readers = m_InternalReaders;
    str.Tiles = &toReadTiles;
    str.IO = this;
    str.Cache = m_TileCache;
    str.Buffer = buffer;

    // Set-up multi-threader
    this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);
    
    // multithread the execution
    this->GetMultiThreader()->SingleMethodExecute();
    }

  chrono.Stop();
  otbMsgDevMacro( << "JPEG2000ImageIO::Read took " << chrono.GetTotal() << " sec");

  for(ReaderVectorType::iterator it = m_InternalReaders.begin();
       it != m_InternalReaders.end();
      ++it)
    {
    (*it)->Clean();
    }
}

// The void * interface prevent us to propagate shared_ptr here
void JPEG2000ImageIO::LoadTileData(void * buffer, void * currentTile)
{
  opj_image_t * tile = static_cast<opj_image_t *>(currentTile);

  if(!tile)
    {
    itkExceptionMacro(<<"Tile needed but not loaded.");
    }

 // Get nb. of lines and columns of the region to read
  int lNbLines     = this->GetIORegion().GetSize()[1];
  int lNbColumns   = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]
  unsigned int lWidthSrc; // Width of the input pixel in nb of pixel
  unsigned int lHeightDest; // Height of the area where write in nb of pixel
  unsigned int lWidthDest; // Width of the area where write in nb of pixel
  unsigned int lStartOffsetPxlDest; // Offset where begin to write the area in the otb buffer in nb of pixel
  unsigned int lStartOffsetPxlSrc; // Offset where begin to write the area in the otb buffer in nb of pixel

  ComputeOffsets(tile, this->GetIORegion(), lWidthSrc, lHeightDest, lWidthDest, lStartOffsetPxlDest, lStartOffsetPxlSrc);

  switch (this->GetComponentType())
    {
    case CHAR:
    {
    char *p = static_cast<char *> (buffer);
    for (unsigned int j = 0; j < lHeightDest; ++j)
      {
      char* current_dst_line = p + (lStartOffsetPxlDest + j * lNbColumns) * this->m_NumberOfComponents;

      for (unsigned int k = 0; k < lWidthDest; ++k)
        {
        for (unsigned int itComp = 0; itComp < this->m_NumberOfComponents; itComp++)
          {
          OPJ_INT32* data = tile->comps[itComp].data;
          *(current_dst_line++) = static_cast<char> (data[lStartOffsetPxlSrc + k + j * lWidthSrc]);
          }
        }
      }
    }
    break;
    case UCHAR:
    {
    unsigned char *p = static_cast<unsigned char *> (buffer);
    for (unsigned int j = 0; j < lHeightDest; ++j)
      {
      unsigned char* current_dst_line = p + (lStartOffsetPxlDest + j * lNbColumns) * this->m_NumberOfComponents;

      for (unsigned int k = 0; k < lWidthDest; ++k)
        {
        for (unsigned int itComp = 0; itComp < this->m_NumberOfComponents; itComp++)
          {
          OPJ_INT32* data = tile->comps[itComp].data;
          unsigned char component_val = data[lStartOffsetPxlSrc + k + j * lWidthSrc] & 0xff;
          *(current_dst_line++) = static_cast<unsigned char> (component_val);
          }
        }
      }
    }
    break;
    case SHORT:
    {
    short *p = static_cast<short *> (buffer);
    for (unsigned int j = 0; j < lHeightDest; ++j)
      {
      short* current_dst_line = p + (lStartOffsetPxlDest + j * lNbColumns) * this->m_NumberOfComponents;

      for (unsigned int k = 0; k < lWidthDest; ++k)
        {
        for (unsigned int itComp = 0; itComp < this->m_NumberOfComponents; itComp++)
          {
          OPJ_INT32* data = tile->comps[itComp].data;
          *(current_dst_line++) = static_cast<short> (data[lStartOffsetPxlSrc + k + j * lWidthSrc]);
          }
        }
      }
    }
    break;
    case USHORT:
    {
    unsigned short *p = static_cast<unsigned short *> (buffer);
    for (unsigned int j = 0; j < lHeightDest; ++j)
      {
      unsigned short* current_dst_line = p + (lStartOffsetPxlDest + j * lNbColumns) * this->m_NumberOfComponents;

      for (unsigned int k = 0; k < lWidthDest; ++k)
        {
        for (unsigned int itComp = 0; itComp < this->m_NumberOfComponents; itComp++)
          {
          OPJ_INT32* data = tile->comps[itComp].data;
          *(current_dst_line++) = static_cast<unsigned short> (data[lStartOffsetPxlSrc + k + j * lWidthSrc] & 0xffff);
          }
        }
      }
    }
    break;
    case INT:
    case UINT:
    default:
      itkGenericExceptionMacro(<< "This data type is not handled");
      break;
    }
}

ITK_THREAD_RETURN_TYPE JPEG2000ImageIO::ThreaderCallback( void *arg )
{
  ThreadStruct *str;
  unsigned int total, threadCount;
  int threadId;

  threadId = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  threadCount = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;
  
  str = (ThreadStruct *)(((itk::MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  // Retrieve data
  std::vector<boost::shared_ptr<JPEG2000InternalReader> > readers = str->Readers;
  std::vector<JPEG2000TileCache::CachedTileType> *  tiles = str->Tiles;
  JPEG2000ImageIO::Pointer io = str->IO;
  boost::shared_ptr<JPEG2000TileCache> cache = str->Cache;
  void * buffer = str->Buffer;

  total = std::min((unsigned int)tiles->size(), threadCount);

  if(total == 0)
    {
    return ITK_THREAD_RETURN_VALUE;
    }


  unsigned int tilesPerThread = tiles->size()/total;
  if(tilesPerThread == 0)
    {
    tilesPerThread = 1;
    }

  for(unsigned int i = threadId * tilesPerThread;
        i < tilesPerThread * (threadId+1);
        ++i)
    {
    boost::shared_ptr<opj_image_t> currentTile = readers.at(threadId)->DecodeTile(tiles->at(i).first);

    // Check if tile is valid
    if(!currentTile)
      {
      readers.at(threadId)->Clean();
      itkGenericExceptionMacro(" otbopenjpeg failed to decode the desired tile "<<tiles->at(i).first << "!");
      }

    otbMsgDevMacro(<< " Tile " << tiles->at(i).first << " decoded by thread "<<threadId);

    io->LoadTileData(buffer, currentTile.get());
     
    if (cache->GetCacheSizeInTiles() != 0)
     {
     cache->AddTile(tiles->at(i).first, currentTile);
     }
    }

  unsigned int lastTile = threadCount*tilesPerThread + threadId;

  // TODO: check this last part

  if(lastTile < tiles->size())
    {
    boost::shared_ptr<opj_image_t> currentTile = readers.at(threadId)->DecodeTile(tiles->at(lastTile).first);

    // Check if tile is valid
    if(!currentTile)
      {
      readers.at(threadId)->Clean();
      itkGenericExceptionMacro(" otbopenjpeg failed to decode the desired tile "<<tiles->at(lastTile).first << "!");
      }

    otbMsgDevMacro(<< " Tile " <<tiles->at(lastTile).first  << " decoded by thread "<<threadId);

    io->LoadTileData(buffer, currentTile.get());
     
    if (cache->GetCacheSizeInTiles() != 0)
     {
     cache->AddTile(tiles->at(lastTile).first, currentTile);
     }
    }

  return ITK_THREAD_RETURN_VALUE;
}

void JPEG2000ImageIO::ConfigureCache()
{
  itk::ExposeMetaData<unsigned int>(this->GetMetaDataDictionary(),
                                    MetaDataKey::CacheSizeInBytes,
                                    m_CacheSizeInByte);
  
  // Initialize some parameters of the tile cache
  this->m_TileCache->Initialize(m_InternalReaders.front()->m_TileWidth,
                                m_InternalReaders.front()->m_TileHeight,
                                m_InternalReaders.front()->m_NbOfComponent,
                                m_BytePerPixel,
                                m_ResolutionFactor);

  // If available set the size of the cache
  if (this->m_CacheSizeInByte)
    this->m_TileCache->SetCacheSizeInByte(this->m_CacheSizeInByte);
}

void JPEG2000ImageIO::ReadImageInformation()
{
  // Extract metadata
  // In case the metadata are not set, the function silently returns, doing nothing
  itk::ExposeMetaData<unsigned int>(this->GetMetaDataDictionary(),
                                    MetaDataKey::ResolutionFactor,
                                    m_ResolutionFactor);

  // Now initialize the itk dictionary
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  JPEG2000MetadataReader lJP2MetadataReader(m_FileName.c_str());

  if (lJP2MetadataReader.m_MetadataIsRead)
    {
    otbMsgDevMacro(<<"JPEG2000 file has metadata available!");

    /* GEOTRANSFORM */
    if (lJP2MetadataReader.HaveGeoTransform())
      {
      otbMsgDevMacro(<< "JPEG2000 file has a geotransform!");
      std::vector<double> geoTransform = lJP2MetadataReader.GetGeoTransform();

      itk::EncapsulateMetaData<MetaDataKey::VectorType>(dict, MetaDataKey::GeoTransformKey, geoTransform);

      /*std::cout << "from gml box, geotransform: ";
      for (int i = 0; i < 6; i++)
        {
        std::cout << geoTransform[i] << ", ";
        }
      std::cout << std::endl; */

      // Retrieve origin and spacing from the geo transform
      m_Origin[0] = geoTransform[0];
      m_Origin[1] = geoTransform[3];
      m_Spacing[0] = geoTransform[1];
      m_Spacing[1] = geoTransform[5];

      if ( m_Spacing[0]== 0 || m_Spacing[1] == 0)
        {
        // Manage case where axis are not standard
        if (geoTransform[2] != 0  && geoTransform[4] != 0 )
          {
          m_Spacing[0] = geoTransform[2];
          m_Spacing[1] = geoTransform[4];
          }
        else
          {
          otbWarningMacro(<< "JPEG2000 file has an incorrect geotransform  (spacing = 0)!");
          m_Spacing[0] = 1;
          m_Spacing[1] = 1;
          }
        }
      }

    /* GCPs */
    if (lJP2MetadataReader.GetGCPCount() > 0)
      {
      // No GCPprojRef return by GDALJP2metadata
      std::string gcpProjectionKey = "UNKNOWN";
      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::GCPProjectionKey, gcpProjectionKey);

      int nbGCPs = lJP2MetadataReader.GetGCPCount();
      otbMsgDevMacro(<< "JPEG2000 file has "<< nbGCPs << " GCPs!");
      itk::EncapsulateMetaData<int>(dict, MetaDataKey::GCPCountKey, nbGCPs);

      std::vector<GDAL_GCP> gcps = lJP2MetadataReader.GetGCPs();

      std::string key;
      for (int cpt = 0; cpt < nbGCPs; ++cpt)
        {
        GDAL_GCP currentGCP = gcps[cpt];
        OTB_GCP pOtbGCP;
        pOtbGCP.m_Id = std::string(currentGCP.pszId);
        pOtbGCP.m_Info = std::string(currentGCP.pszInfo);
        pOtbGCP.m_GCPRow = currentGCP.dfGCPLine;
        pOtbGCP.m_GCPCol = currentGCP.dfGCPPixel;
        pOtbGCP.m_GCPX = currentGCP.dfGCPX;
        pOtbGCP.m_GCPY = currentGCP.dfGCPY;
        pOtbGCP.m_GCPZ = currentGCP.dfGCPZ;

        // Complete the key with the GCP number : GCP_i
        std::ostringstream lStream;
        lStream << MetaDataKey::GCPParametersKey << cpt;
        key = lStream.str();

        itk::EncapsulateMetaData<OTB_GCP>(dict, key, pOtbGCP);
        }
      }

    /* GMLMetadata*/
    char** papszGMLMetadata;
    papszGMLMetadata =  lJP2MetadataReader.GetGMLMetadata();
    if (CSLCount(papszGMLMetadata) > 0)
      {
      otbMsgDevMacro(<< "JPEG2000 file has GMLMetadata!");
      std::string key;

      for (int cpt = 0; papszGMLMetadata[cpt] != NULL; ++cpt)
        {
        std::ostringstream lStream;
        lStream << MetaDataKey::MetadataKey << cpt;
        key = lStream.str();
        
        itk::EncapsulateMetaData<std::string>(dict, key, static_cast<std::string> (papszGMLMetadata[cpt]));
        otbMsgDevMacro( << static_cast<std::string>(papszGMLMetadata[cpt]));
        }
      }


    /* ProjectionRef*/
    if (lJP2MetadataReader.GetProjectionRef() && !std::string(lJP2MetadataReader.GetProjectionRef()).empty() )
      {
      OGRSpatialReferenceH pSR = OSRNewSpatialReference(NULL);

      const char * pszProjection = NULL;
      pszProjection =  lJP2MetadataReader.GetProjectionRef();

      if (OSRImportFromWkt(pSR, (char **) (&pszProjection)) == OGRERR_NONE)
        {
        char * pszPrettyWkt = NULL;
        OSRExportToPrettyWkt(pSR, &pszPrettyWkt, FALSE);

        itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                               static_cast<std::string>(pszPrettyWkt));

        CPLFree(pszPrettyWkt);
        }
      else
        {
        itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey,
                                              static_cast<std::string>(lJP2MetadataReader.GetProjectionRef()));
        }

      if (pSR != NULL)
        {
        OSRRelease(pSR);
        pSR = NULL;
        }
      }
    else
      {
      otbMsgDevMacro( << "NO PROJECTION IN GML BOX => SENSOR MODEL " );
      m_Origin[0] = 0; m_Origin[1] = 0;
      m_Spacing[0] = 1; m_Spacing[1] = 1;

      lJP2MetadataReader.GetOriginFromGMLBox(m_Origin);
      }

    otbMsgDevMacro(<< "FROM GML box: " << "Origin: " << m_Origin[0] << ", " << m_Origin[1]
                   << " | Spacing: " << m_Spacing[0] << ", " << m_Spacing[1] );

    }
  else
    {
    otbMsgDevMacro( << "JPEG2000 file has NO metadata available!");
    m_Origin[0] = 0;
    m_Origin[1] = 0;
    m_Spacing[0] = 1;
    m_Spacing[1] = 1;
    }


  // If the internal image was not open we open it.
  // This is usually done when the user sets the ImageIO manually
  if ( !m_InternalReaders.front()->m_IsOpen )
    {
    if ( !this->CanReadFile( m_FileName.c_str() ) )
      {
      itkExceptionMacro(<< "Cannot open file " << this->m_FileName << "!");
      return;
      }
    }

  // Check some internal parameters of the JPEG2000 file
  if ( !this->m_InternalReaders.front()->CanRead())
    {
    itkExceptionMacro(<< "Cannot read this file because some JPEG2000 parameters are not supported!");
    this->m_InternalReaders.front()->Clean();
    return;
    }

  // If one of the readers fails, clean everything
  if(this->m_ResolutionFactor != 0)
    {
    for(ReaderVectorType::iterator it = m_InternalReaders.begin();
        it != m_InternalReaders.end();
        ++it)
      {
      (*it)->Clean();
      }

    bool success = true;

    for(ReaderVectorType::iterator it = m_InternalReaders.begin();
        it != m_InternalReaders.end();
        ++it)
        {

        if ( !(*it)->Open( m_FileName.c_str(), this->m_ResolutionFactor ) )
          {
          success = false;
          }
        (*it)->m_IsOpen = true;
        }

    // If one of the readers fails, clean everything
    if(!success)
      {
      for(ReaderVectorType::iterator it = m_InternalReaders.begin();
          it != m_InternalReaders.end();
          ++it)
        {
        (*it)->Clean();
        }

      itkExceptionMacro(<< "Cannot open this file with this resolution!");
      return;
      }
    }

  // Encapsulate tile hints for streaming
  unsigned int tileHintX = m_InternalReaders.front()->m_TileWidth / static_cast<unsigned int>(vcl_pow(2.0, static_cast<int>(m_ResolutionFactor) ));
  unsigned int tileHintY = m_InternalReaders.front()->m_TileHeight / static_cast<unsigned int>(vcl_pow(2.0, static_cast<int>(m_ResolutionFactor) ));
  
  itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::TileHintX, tileHintX);
  itk::EncapsulateMetaData<unsigned int>(dict, MetaDataKey::TileHintY, tileHintY);

  m_Spacing[0] *= vcl_pow(2.0, static_cast<double>(m_ResolutionFactor));
  m_Spacing[1] *= vcl_pow(2.0, static_cast<double>(m_ResolutionFactor));

  // If we have some spacing information we use it
  // could be needed for other j2k image but not for pleiades
  // if ( (m_InternalReaders.front()->m_XResolution.front() > 0) && (m_InternalReaders.front()->m_YResolution.front() > 0) )
  //   {
  //     // We check previously that the X and Y resolution is equal between the components
  //     m_Spacing[0] *= m_InternalReaders.front()->m_XResolution[0];
  //     m_Spacing[1] *= m_InternalReaders.front()->m_YResolution[0];
  //   }

  m_Dimensions[0] = m_InternalReaders.front()->m_Width;
  m_Dimensions[1] = m_InternalReaders.front()->m_Height;

  this->SetNumberOfDimensions(2);

  if (m_Dimensions[0] == 0 || m_Dimensions[1] == 0)
    {
    itkExceptionMacro(<< "Image size is null.");
    }

  this->SetNumberOfComponents(m_InternalReaders.front()->m_NbOfComponent);

  // Automatically set the Type to Binary for JPEG2000 data
  this->SetFileTypeToBinary();

  // We check previously that these values are equal between all components
  unsigned int precision = m_InternalReaders.front()->m_Precision[0];
  int          isSigned = m_InternalReaders.front()->m_Signed[0];

  if (precision <= 8)
    {
    m_BytePerPixel = 1;
    if (isSigned)
      {
      SetComponentType(CHAR);
      }
    else
      {
      SetComponentType(UCHAR);
      }
    }
  else if (precision <= 16)
    {
    m_BytePerPixel = 2;
    if (isSigned)
      {
      SetComponentType(SHORT);
      }
    else
      {
      SetComponentType(USHORT);
      }
    }
  else
    {
    m_BytePerPixel = 4;
    if (isSigned)
      {
      SetComponentType(INT);
      }
    else
      {
      SetComponentType(UINT);
      }
    }

  if (this->GetNumberOfComponents() == 1)
    {
    this->SetPixelType(SCALAR);
    }
  else
    {
    this->SetPixelType(VECTOR);
    }

  this->ConfigureCache();

  otbMsgDebugMacro(<< "==========================");
  otbMsgDebugMacro(<< "ReadImageInformation: ");
  otbMsgDebugMacro(<< "Tile size (WxH): " << m_InternalReaders.front()->m_TileWidth << " x "
                   << m_InternalReaders.front()->m_TileHeight);
  otbMsgDebugMacro(<< "Number of tiles (Xdim x Ydim) : " << m_InternalReaders.front()->m_XNbOfTile
                   << " x " << m_InternalReaders.front()->m_YNbOfTile);
  otbMsgDebugMacro(<< "Precision: " << precision);
  otbMsgDebugMacro(<< "Signed: " << isSigned);
  otbMsgDebugMacro(<< "Number of octet per value: " << m_BytePerPixel);
  otbMsgDebugMacro(<< "==========================");

  otbMsgDebugMacro(<< "Driver to read: JPEG2000");
  otbMsgDebugMacro(<< "         Read  file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());
}

// Compute the tile index list from the GetRegion
std::vector<unsigned int> JPEG2000ImageIO::ComputeTileList()
{
  std::vector<unsigned int> tileVector;

  // Get nb. of lines and columns of the region to decode
  unsigned int startX = this->GetIORegion().GetIndex()[0];
  unsigned int endX   = this->GetIORegion().GetIndex()[0] + this->GetIORegion().GetSize()[0];
  unsigned int startY = this->GetIORegion().GetIndex()[1];
  unsigned int endY   = this->GetIORegion().GetIndex()[1] + this->GetIORegion().GetSize()[1];

  // Compute index of tile recover by the decoded area
  unsigned int tile_size_x = m_InternalReaders.front()->m_TileWidth;
  unsigned int tile_size_y = m_InternalReaders.front()->m_TileHeight;
  unsigned int width = m_Dimensions[0];
  unsigned int height = m_Dimensions[1];
  unsigned int nbOfTileX = m_InternalReaders.front()->m_XNbOfTile;
  unsigned int nbOfTileY = m_InternalReaders.front()->m_YNbOfTile;

  unsigned int tilePosX0, tilePosX1;
  unsigned int tilePosY0, tilePosY1;

  for (unsigned int itTileY = 0; itTileY < nbOfTileY; itTileY++)
    {
    tilePosY0 = uint_ceildivpow2( itTileY*tile_size_y, m_ResolutionFactor );
    tilePosY1 = std::min( uint_ceildivpow2( (itTileY+1)*tile_size_y, m_ResolutionFactor ), height );

    for (unsigned int itTileX = 0; itTileX < nbOfTileX; itTileX++)
      {
      tilePosX0 = uint_ceildivpow2( itTileX*tile_size_x, m_ResolutionFactor );
      tilePosX1 = std::min( uint_ceildivpow2( (itTileX+1)*tile_size_x, m_ResolutionFactor ), width);

      if ( (tilePosX1 - tilePosX0) && (tilePosY1 - tilePosY0) &&
           (tilePosX1 > startX) && (tilePosX0 < endX ) &&
           (tilePosY1 > startY) && (tilePosY0 < endY ) )
        tileVector.push_back(itTileX + itTileY * nbOfTileX);

      }
    }

  return tileVector;
}

/** Compute the offsets in Pixel to move the input buffer from openjpeg
 * to the corresponding area of the otb output buffer
 * */
void ComputeOffsets( opj_image_t * currentTile,
                     const itk::ImageIORegion & ioRegion,
                     unsigned int &l_width_src, // Width of the input pixel in nb of pixel
                     unsigned int &l_height_dest, // Height of the area where write in nb of pixel
                     unsigned int &l_width_dest, // Width of the area where write in nb of pixel
                     unsigned int &l_start_offset_dest, // Offset where begin to write the area in the otb buffer in nb of pixel
                     unsigned int &l_start_offset_src // Offset where begin to read the data in the openjpeg decoded data in nb of pixel
  )
{
  // Characteristics of the input buffer from openpjeg
  unsigned int l_x0_src = int_ceildivpow2(currentTile->x0, currentTile->comps->factor);
  unsigned int l_y0_src = int_ceildivpow2(currentTile->y0, currentTile->comps->factor);
  unsigned int l_x1_src = int_ceildivpow2(currentTile->x1, currentTile->comps->factor);
  unsigned int l_y1_src = int_ceildivpow2(currentTile->y1, currentTile->comps->factor);

  // Size of input buffer from openjpeg
  l_width_src = l_x1_src - l_x0_src;
  unsigned int l_height_src = l_y1_src - l_y0_src;

  // Characteristics of the otb region
  unsigned int l_x0_dest = ioRegion.GetIndex()[0];
  unsigned int l_x1_dest = ioRegion.GetIndex()[0] + ioRegion.GetSize()[0];
  unsigned int l_y0_dest = ioRegion.GetIndex()[1];
  unsigned int l_y1_dest = ioRegion.GetIndex()[1] + ioRegion.GetSize()[1];

  unsigned int l_start_x_dest, l_start_y_dest;
  unsigned int l_offset_x0_src, l_offset_y0_src;

  /*-----*/
  /* Compute the origin (l_offset_x0_src, l_offset_y0_src )
   * of the input buffer (decoded tile) which will be move
   * in the output buffer.
   * Compute the area of the output buffer (l_start_x_dest,
   * l_start_y_dest, l_width_dest, l_height_dest)  which will be modified
   * by this input area.
   */

  if (l_x0_dest < l_x0_src)
    {
    l_start_x_dest = l_x0_src - l_x0_dest;
    l_offset_x0_src = 0;

    if (l_x1_dest >= l_x1_src)
      {
      l_width_dest = l_width_src;
      }
    else
      {
      l_width_dest = l_x1_dest - l_x0_src;
      }
    }
  else
    {
    l_start_x_dest = 0;
    l_offset_x0_src = l_x0_dest - l_x0_src;

    if (l_x1_dest >= l_x1_src)
      {
      l_width_dest = l_width_src - l_offset_x0_src;
      }
    else
      {
      l_width_dest = l_x1_dest - l_x0_dest;
      }
    }

  if (l_y0_dest < l_y0_src)
    {
    l_start_y_dest = l_y0_src - l_y0_dest;
    l_offset_y0_src = 0;

    if (l_y1_dest >= l_y1_src)
      {
      l_height_dest = l_height_src;
      }
    else
      {
      l_height_dest = l_y1_dest - l_y0_src;
      }
    }
  else
    {
    l_start_y_dest = 0;
    l_offset_y0_src = l_y0_dest - l_y0_src;

    if (l_y1_dest >= l_y1_src)
      {
      l_height_dest = l_height_src - l_offset_y0_src;
      }
    else
      {
      l_height_dest = l_y1_dest - l_y0_dest;
      }
    }
  /*-----*/

  /* Compute the input buffer offset */
  l_start_offset_src = l_offset_x0_src + l_offset_y0_src * l_width_src;

  /* Compute the output buffer offset */
  l_start_offset_dest = l_start_x_dest + l_start_y_dest * (l_x1_dest - l_x0_dest);

  /*
  std::cout << "SRC coordinates: l_start_x_src= " << l_x0_src << ", l_start_y_src= " << l_y0_src << ", l_width_src= "
      << l_width_src << ", l_height_src= " << l_height_src << std::endl;
  std::cout << "SRC tile offset: " << l_offset_x0_src << ", " << l_offset_y0_src << std::endl;
  std::cout << "SRC buffer offset: " << l_start_offset_src << std::endl;

  std::cout << "DEST coordinates: l_start_x_dest= " << l_start_x_dest << ", l_start_y_dest= " << l_start_y_dest
      << ", l_width_dest= " << l_width_dest << ", l_height_dest= " << l_height_dest << std::endl;
  std::cout << "DEST start offset: " << l_start_offset_dest << std::endl;
  */
}

// Not yet implemented
bool JPEG2000ImageIO::CanWriteFile(const char* /*filename*/)
{
  return false;
}

// Not yet implemented
void JPEG2000ImageIO::Write(const void* /*buffer*/)
{

}

void JPEG2000ImageIO::WriteImageInformation()
{
  if (m_FileName == "")
    {
    itkExceptionMacro(<< "A FileName must be specified.");
    }
  if (CanWriteFile(m_FileName.c_str()) == false)
    {
    itkExceptionMacro(<< "The file " << m_FileName.c_str() << " is not defined as a JPEG2000 file");
    }

  otbMsgDebugMacro(<< "Driver to write: JPEG2000");
  otbMsgDebugMacro(<< "         Write file         : " << m_FileName);
  otbMsgDebugMacro(<< "         Size               : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDebugMacro(<< "         ComponentType      : " << this->GetComponentType());
  otbMsgDebugMacro(<< "         NumberOfComponents : " << this->GetNumberOfComponents());
  otbMsgDebugMacro(<< "         ComponentSize      : " << this->GetComponentSize());
  otbMsgDebugMacro(<< "         GetPixelSize       : " << this->GetPixelSize());

}

} // end namespace otb
