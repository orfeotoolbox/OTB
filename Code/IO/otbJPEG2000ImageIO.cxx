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


#include "otbMacro.h"
#include "otbSystem.h"

#include "itkTimeProbe.h"
#include "itkMacro.h"

#include <deque>

extern "C"
{
#include "openjpeg.h"
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

class JPEG2000ReaderInternal
{
public:
  typedef std::pair<unsigned int, opj_image_t *> CachedTileType;
  typedef std::deque<CachedTileType> TileCacheType;


  JPEG2000ReaderInternal();

  ~JPEG2000ReaderInternal()
  {    
    this->Clean();
  }

  opj_codec_t* GetCodec(){return this->m_Codec; };
  FILE* GetFile(){return this->m_File; };
  opj_image_t* GetImage(){return this->m_Image; };
  opj_image_t* GetPersistentImage(){return this->m_PersistentImage; };
  opj_stream_t* GetStream(){return this->m_Stream; };
  opj_codestream_info_v2* GetCstrInfo(){return this->m_CstrInfo; };

  bool LoadTileFromCache(unsigned int tileIndex);
  
  bool ReadTileFromFile(unsigned int tileIndex);

   void Clean();

  int CanRead();

  int Open(const char *filename);

  bool m_IsOpen;
  OPJ_CODEC_FORMAT m_CodecFormat;

  unsigned int         m_Width;
  unsigned int         m_Height;

  unsigned int         m_NbOfComponent;

  unsigned int*        m_XResolution;
  unsigned int*        m_YResolution;
  unsigned int*        m_Precision;
  int*                 m_Signed;

  unsigned int         m_TileWidth;
  unsigned int         m_TileHeight;
  unsigned int         m_XNbOfTile;
  unsigned int         m_YNbOfTile;

private:
  opj_codec_t *  m_Codec;
  FILE* m_File;
  opj_image_t* m_Image;
  opj_image_t* m_PersistentImage;
  opj_stream_t* m_Stream;
  opj_codestream_info_v2* m_CstrInfo;
  unsigned int m_CacheSizeInTiles;
  TileCacheType m_Cache;

  int Initialize();

  void InsertTileInCache(unsigned int tileIndex,opj_image_t * tileData);

  void ClearCache();
};


int JPEG2000ReaderInternal::Open(const char *filename)
{
  this->Clean();

  // Open the file
  this->m_File  = fopen(filename, "rb");
  if (!this->m_File)
    {
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

  // Initialize the codec and the stream
  if (!this->Initialize())
    {
    this->Clean();
    return 0;
    }

  this->m_IsOpen = true;
  return 1;
}

void JPEG2000ReaderInternal::Clean()
{
  // Destroy the image
  if (this->m_PersistentImage)
    {
    otbopenjpeg_opj_image_destroy(this->m_PersistentImage);
    }
  this->m_PersistentImage = NULL;

  // Close the byte stream
  if (this->m_Stream)
    {
    otbopenjpeg_opj_stream_destroy(this->m_Stream);
    }
  this->m_Stream = NULL;

  // Close the file
  if (this->m_File)
    {
    fclose(this->m_File);
    }
  this->m_File = NULL;

  // Destroy the codec
  if (this->m_Codec)
    {
    otbopenjpeg_opj_destroy_codec(this->m_Codec);
    }
  this->m_Codec = NULL;

  // Destroy the codestream info
  if (this->m_CstrInfo)
    {
    otbopenjpeg_opj_destroy_cstr_info_v2(&(this->m_CstrInfo));
    }

  delete[] this->m_XResolution;
  delete[] this->m_YResolution;
  delete[] this->m_Precision;
  delete[] this->m_Signed;


  this->m_XResolution = NULL;
  this->m_YResolution = NULL;
  this->m_Precision = NULL;
  this->m_Signed = NULL;


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


void JPEG2000ReaderInternal::ClearCache()
{
  for(TileCacheType::iterator it = m_Cache.begin();
      it != m_Cache.end();++it)
    {
    CachedTileType erasedTile = *it;
    
    // Destroy the image
    if (erasedTile.second)
      {
      otbopenjpeg_opj_image_destroy(erasedTile.second);
      }
    erasedTile.second = NULL;
    } 
  m_Cache.clear();
}

bool JPEG2000ReaderInternal::LoadTileFromCache(unsigned int tileIndex)
{
  for(TileCacheType::iterator it = m_Cache.begin();
      it != m_Cache.end();++it)
    {
    if(it->first == tileIndex)
      {
      CachedTileType cachedTile = (*it);
      this->m_Image = cachedTile.second;
      m_Cache.erase(it);
      m_Cache.push_back(cachedTile);
      otbMsgDebugMacro(<<"Tile "<<tileIndex<<" loaded from cache");
      return true;
      }
    }
  return false;
}

bool JPEG2000ReaderInternal::ReadTileFromFile(unsigned int tileIndex)
{
  this->m_Image = otbopenjpeg_opj_image_create0();
  otbopenjpeg_opj_copy_image_header(this->GetPersistentImage(), this->m_Image);

  bool success = otbopenjpeg_opj_get_decoded_tile(this->GetCodec(),this->GetStream(),this->GetImage(),tileIndex);
  

  if(success)
    {
    otbMsgDebugMacro(<<"Tile "<<tileIndex<<" decoded from file");
    InsertTileInCache(tileIndex,this->GetImage());
    }
  
  return success;
}

void JPEG2000ReaderInternal::InsertTileInCache(unsigned int tileIndex, opj_image_t * tileData)
{
  for(TileCacheType::const_iterator it = m_Cache.begin();
      it != m_Cache.end();++it)
    {
    if(it->first == tileIndex)
      {
      return;
      }
    }
  if(m_Cache.size() >= m_CacheSizeInTiles)
    {
    CachedTileType erasedTile = *m_Cache.begin();
    
    // Destroy the image
    if (erasedTile.second)
      {
      otbopenjpeg_opj_image_destroy(erasedTile.second);
      }
    erasedTile.second = NULL;

    m_Cache.pop_front();
    }
  
  m_Cache.push_back(CachedTileType(tileIndex,tileData));
}


JPEG2000ReaderInternal::JPEG2000ReaderInternal()
{
  this->m_Image = NULL;
  this->m_PersistentImage = NULL;
  this->m_Codec = NULL;
  this->m_Stream = NULL;
  this->m_File = NULL;
  this->m_CstrInfo = NULL;

  this->m_XResolution = NULL;
  this->m_YResolution = NULL;
  this->m_Precision = NULL;
  this->m_Signed = NULL;

  this->m_Cache = TileCacheType();
  this->m_CacheSizeInTiles = 4;

  this->Clean();
}

int JPEG2000ReaderInternal::Initialize()
{  
  if (this->m_File)
    {
    // Creating the file stream
    this->m_Stream = otbopenjpeg_opj_stream_create_default_file_stream(this->m_File, true);
    if (!this->m_Stream)
      {
      this->Clean();
      return 0;
      }

    // Creating the codec
    this->m_Codec = otbopenjpeg_opj_create_decompress_v2(this->m_CodecFormat);

    if (!this->m_Codec)
      {
      return 0;
      }

    // Set default event mgr  FIXME
    opj_event_mgr_t event_mgr;
    otbopenjpeg_opj_initialize_default_event_handler(&event_mgr, true);

    // Setting default parameters
    opj_dparameters_t parameters;
    otbopenjpeg_opj_set_default_decoder_parameters(&parameters);

    // Setup the decoder decoding parameters using user parameters
    if (!otbopenjpeg_opj_setup_decoder_v2(this->m_Codec, &parameters, &event_mgr))
      {
      this->Clean();
      return 0;
      }

    // Read the main header of the codestream and if necessary the JP2 boxes
    if (!otbopenjpeg_opj_read_header(this->m_Stream, this->m_Codec, &(this->m_PersistentImage)))
      {
      this->Clean();
      return 0;
      }

    // Get the codestream information
    this->m_CstrInfo = otbopenjpeg_opj_get_cstr_info(this->m_Codec);
    if (!this->m_CstrInfo)
      {
      std::cerr << "ERROR while get codestream info" << std::endl;
      this->Clean();
      return 0;
      }

    // We can now retrieve the main information  of the image and the codestream
    this->m_Width = this->m_PersistentImage->x1 - this->m_PersistentImage->x0;
    this->m_Height = this->m_PersistentImage->y1 - this->m_PersistentImage->y0;

    this->m_TileHeight = this->m_CstrInfo->tdy;
    this->m_TileWidth = this->m_CstrInfo->tdx;
    this->m_XNbOfTile = this->m_CstrInfo->tw;
    this->m_YNbOfTile = this->m_CstrInfo->th;

    this->m_NbOfComponent = this->m_PersistentImage->numcomps;

    this->m_Precision = new unsigned int[this->m_NbOfComponent];
    if (!this->m_Precision)
      {
      this->Clean();
      return 0;
      }

    for (unsigned int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_Precision[itComp] = this->m_PersistentImage->comps[itComp].prec;
      }

    this->m_Signed = new int[this->m_NbOfComponent];
    if (!this->m_Signed)
      {
      this->Clean();
      return 0;
      }
    for (unsigned int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_Signed[itComp] = this->m_PersistentImage->comps[itComp].sgnd;
      }

    this->m_XResolution = new unsigned int[this->m_NbOfComponent];
    if (!this->m_XResolution)
      {
      this->Clean();
      return 0;
      }

    for (unsigned int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_XResolution[itComp] = this->m_PersistentImage->comps[itComp].dx;
      }

    this->m_YResolution = new unsigned int[this->m_NbOfComponent];
    if (!this->m_YResolution)
      {
      this->Clean();
      return 0;
      }

    for (unsigned int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_YResolution[itComp] = this->m_PersistentImage->comps[itComp].dy;
      }

    }

  return 1;
}

int JPEG2000ReaderInternal::CanRead()
 {
   if  ( this->m_File &&
       this->m_Codec &&
       this->m_Stream &&
       this->m_CstrInfo &&
       this->m_PersistentImage &&
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



JPEG2000ImageIO::JPEG2000ImageIO()
{
  m_InternalReader = new JPEG2000ReaderInternal;

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
}

JPEG2000ImageIO::~JPEG2000ImageIO()
{
  m_InternalReader->Clean();
  delete m_InternalReader;
}

bool JPEG2000ImageIO::CanReadFile(const char* filename)
{

  if (filename == NULL)
    {
    itkDebugMacro(<< "No filename specified.");
    return false;
    }

  if ( !this->m_InternalReader->Open(filename) )
    {
    this->m_InternalReader->Clean();
    return false;
    }

  return true;
}

// Used to print information about this object
void JPEG2000ImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image not implemented yet
void JPEG2000ImageIO::ReadVolume(void*)
{
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
  if ( !m_InternalReader->m_IsOpen )
    {
    if ( !this->CanReadFile( m_FileName.c_str() ) )
      {
      itkExceptionMacro(<< "Cannot open file " << this->m_FileName << "!");
      return;
      }
    }

  std::vector<unsigned int> tileList = this->ComputeTileList();
  if (tileList.empty())
    {
    this->m_InternalReader->Clean();
    itkExceptionMacro(<< " IORegion is not correct in terme of tile!");
    }

  // Get nb. of lines and columns of the region to read
  int lNbLines     = this->GetIORegion().GetSize()[1];
  int lNbColumns   = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " JPEG2000ImageIO::Read()  ");
  otbMsgDevMacro(<< " ImageDimension   : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " IORegion         : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components : " << this->GetNumberOfComponents());
  otbMsgDevMacro(<< " Area to read: " << lFirstColumn << " " << lFirstLine  << " "
                 << lFirstColumn + lNbColumns << " " << lFirstLine + lNbLines);
  otbMsgDevMacro(<< "Component type: " << this->GetComponentTypeAsString(this->GetComponentType()));


  // Decode tile need
  for (std::vector<unsigned int>::iterator itTile = tileList.begin(); itTile < tileList.end(); itTile++)
    {
    // First, try to read from the Cache
    if(!m_InternalReader->LoadTileFromCache(*itTile))
      {
      // If failed, try to read tile from file
      if(!m_InternalReader->ReadTileFromFile(*itTile))
        {
        this->m_InternalReader->Clean();
        itkExceptionMacro(<< " otbopenjpeg failed to decode the desired tile "<< *itTile << "!");
        }
      }

    otbMsgDevMacro(<< " Tile " << *itTile << " is decoded.");

    unsigned int lWidthSrc; // Width of the input pixel in nb of pixel
    unsigned int lHeightDest; // Height of the area where write in nb of pixel
    unsigned int lWidthDest; // Width of the area where write in nb of pixel
    unsigned int lStartOffsetPxlDest; // Offset where begin to write the area in the otb buffer in nb of pixel
    unsigned int lStartOffsetPxlSrc; // Offset where begin to write the area in the otb buffer in nb of pixel

    this->ComputeOffsets(lWidthSrc, lHeightDest, lWidthDest, lStartOffsetPxlDest, lStartOffsetPxlSrc);

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
              OPJ_INT32* data = m_InternalReader->GetImage()->comps[itComp].data;
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
              OPJ_INT32* data = m_InternalReader->GetImage()->comps[itComp].data;
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
              OPJ_INT32* data = m_InternalReader->GetImage()->comps[itComp].data;
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
              OPJ_INT32* data = m_InternalReader->GetImage()->comps[itComp].data;
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

  chrono.Stop();
  otbMsgDevMacro( << "JPEG2000ImageIO::Read took " << chrono.GetTotal() << " sec");

  m_InternalReader->Clean();
}

void JPEG2000ImageIO::ReadImageInformation()
{
  // If the internal image was not open we open it.
  // This is usually done when the user sets the ImageIO manually
  if ( !m_InternalReader->m_IsOpen )
    {
    if ( !this->CanReadFile( m_FileName.c_str() ) )
      {
      itkExceptionMacro(<< "Cannot open file " << this->m_FileName << "!");
      return;
      }
    }

  // Check some internal parameters of the JPEG2000 file
  if ( !this->m_InternalReader->CanRead())
    {
    itkExceptionMacro(<< "Cannot read this file because some JPEG2000 parameters are not supported!");
    this->m_InternalReader->Clean();
    return;
    }

  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;

  // If we have some spacing information we use it
  if ( (m_InternalReader->m_XResolution > 0) && (m_InternalReader->m_YResolution > 0) )
    {
      // We check previously that the X and Y resolution is equal between the components
      m_Spacing[0] = m_InternalReader->m_XResolution[0];
      m_Spacing[1] = m_InternalReader->m_YResolution[0];
    }

  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_Dimensions[0] = m_InternalReader->m_Width;
  m_Dimensions[1] = m_InternalReader->m_Height;

  this->SetNumberOfDimensions(2);

  if (m_Dimensions[0] == 0 || m_Dimensions[1] == 0)
    {
    itkExceptionMacro(<< "Image size is null.");
    }

  this->SetNumberOfComponents(m_InternalReader->m_NbOfComponent);

  // Automatically set the Type to Binary for JPEG2000 data
  this->SetFileTypeToBinary();

  // We check previously that these values are equal between all components
  unsigned int precision = m_InternalReader->m_Precision[0];
  int          isSigned = m_InternalReader->m_Signed[0];

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

  otbMsgDebugMacro(<< "==========================");
  otbMsgDebugMacro(<< "ReadImageInformation: ");
  otbMsgDebugMacro(<< "Tile size (WxH): " << m_InternalReader->m_TileWidth << " x "
                   << m_InternalReader->m_TileHeight);
  otbMsgDebugMacro(<< "Number of tiles (Xdim x Ydim) : " << m_InternalReader->m_XNbOfTile
                   << " x " << m_InternalReader->m_YNbOfTile);
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
  int lNbLines     = this->GetIORegion().GetSize()[1];
  int lNbColumns   = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1];
  int lFirstColumn = this->GetIORegion().GetIndex()[0];

  // Compute index of tile recover by the decoded area
  unsigned int tile_size_x = m_InternalReader->GetCstrInfo()->tdx;
  unsigned int tile_size_y = m_InternalReader->GetCstrInfo()->tdy;

  unsigned int l_tile_x_start =  lFirstColumn / tile_size_x;
  unsigned int l_tile_x_end =  (lFirstColumn + lNbColumns + tile_size_x - 1) / tile_size_x;
  unsigned int l_tile_y_start =  lFirstLine / m_InternalReader->GetCstrInfo()->tdy;
  unsigned int l_tile_y_end =  (lFirstLine + lNbLines + tile_size_y - 1) / tile_size_y;

  for (unsigned int itTileY = l_tile_y_start; itTileY < l_tile_y_end; itTileY++)
    {
    for (unsigned int itTileX = l_tile_x_start; itTileX < l_tile_x_end; itTileX++)
      {
      tileVector.push_back(itTileX + itTileY * m_InternalReader->GetCstrInfo()->tw);
      }
    }

  return tileVector;
}

/** Compute the offsets in Pixel to move the input buffer from openjpeg
 * to the corresponding area of the otb output buffer
 * */
void JPEG2000ImageIO::ComputeOffsets( unsigned int &l_width_src, // Width of the input pixel in nb of pixel
                                      unsigned int &l_height_dest, // Height of the area where write in nb of pixel
                                      unsigned int &l_width_dest, // Width of the area where write in nb of pixel
                                      unsigned int &l_start_offset_dest, // Offset where begin to write the area in the otb buffer in nb of pixel
                                      unsigned int &l_start_offset_src // Offset where begin to read the data in the openjpeg decoded data in nb of pixel
                                      )
{
  // Characteristics of the input buffer from openpjeg
  unsigned int l_x0_src = m_InternalReader->GetImage()->x0;
  unsigned int l_y0_src = m_InternalReader->GetImage()->y0;
  unsigned int l_x1_src = m_InternalReader->GetImage()->x1;
  unsigned int l_y1_src = m_InternalReader->GetImage()->y1;

  // Size of input buffer from openjpeg
  l_width_src = l_x1_src - l_x0_src;
  unsigned int l_height_src = l_y1_src - l_y0_src;

  // Characteristics of the otb region
  unsigned int l_x0_dest = this->GetIORegion().GetIndex()[0];
  unsigned int l_x1_dest = this->GetIORegion().GetIndex()[0] + this->GetIORegion().GetSize()[0];
  unsigned int l_y0_dest = this->GetIORegion().GetIndex()[1];
  unsigned int l_y1_dest = this->GetIORegion().GetIndex()[1] + this->GetIORegion().GetSize()[1];

  unsigned int l_start_x_dest , l_start_y_dest;
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
      std::cout << "SRC coordinates: l_start_x_src= "<< l_x0_src << ", l_start_y_src= " <<  l_y0_src
                << ", l_width_src= "<< l_width_src << ", l_height_src= " << l_height_src << std::endl;
      std::cout << "SRC tile offset: "<< l_offset_x0_src << ", " << l_offset_y0_src << std::endl;
      std::cout << "SRC buffer offset: "<< l_start_offset_src << std::endl;

      std::cout << "DEST coordinates: l_start_x_dest= "<<  l_start_x_dest << ", l_start_y_dest= " << l_start_y_dest
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
