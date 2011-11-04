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



/**
   sample error debug callback expecting no client object
*/
void error_callback(const char *msg, void *client_data)
{
  (void) client_data;
  itkGenericExceptionMacro(<< "OpenJPEG error: " << msg);
}
/**
   sample warning debug callback expecting no client object
*/
void warning_callback(const char *msg, void *client_data)
{
  (void) client_data;
  otbGenericMsgDebugMacro(<< "OpenJPEG warning: " << msg);
}
/**
   sample debug callback expecting no client object
*/
void info_callback(const char *msg, void *client_data)
{
  (void) client_data;
  otbMsgDevMacro(<< "OpenJPEG info: " << msg);
}

namespace otb
{

class JPEG2000ReaderInternal
{
public:
  JPEG2000ReaderInternal();

  opj_codec_t* GetCodec(){return this->m_Codec;};
  FILE* GetFile(){return this->m_File;};
  opj_image_t* GetImage(){return this->m_Image;};
  opj_stream_t* GetStream(){return this->m_Stream;};
  opj_codestream_info_v2* GetCstrInfo(){return this->m_CstrInfo;};

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
  opj_stream_t* m_Stream;
  opj_codestream_info_v2* m_CstrInfo;

  int Initialize();

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

  // Destroy the image
  if (this->m_Image)
    {
    otbopenjpeg_opj_image_destroy(this->m_Image);
    }
  this->m_Image = NULL;

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

JPEG2000ReaderInternal::JPEG2000ReaderInternal()
{
  this->m_Image = NULL;
  this->m_Codec = NULL;
  this->m_Stream = NULL;
  this->m_File = NULL;
  this->m_CstrInfo = NULL;

  this->m_XResolution = NULL;
  this->m_YResolution = NULL;
  this->m_Precision = NULL;
  this->m_Signed = NULL;

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
    otbopenjpeg_opj_initialize_default_event_handler(&event_mgr,true);

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
    if (!otbopenjpeg_opj_read_header(this->m_Stream, this->m_Codec, &(this->m_Image)))
      {
      this->Clean();
      return 0;
      }

    // Get the codestream information
    this->m_CstrInfo = otbopenjpeg_opj_get_cstr_info(this->m_Codec);
    if (!this->m_CstrInfo)
      {
      std::cout << "ERROR while get codestream info" << std::endl;
      this->Clean();
      return 0;
      }

    // We can now retrieve the main information  of the image and the codestream
    this->m_Width = this->m_Image->x1 - this->m_Image->x0;
    this->m_Height = this->m_Image->x1 - this->m_Image->x0;

    this->m_TileHeight = this->m_CstrInfo->tdy;
    this->m_TileWidth = this->m_CstrInfo->tdx;
    this->m_XNbOfTile = this->m_CstrInfo->tw;
    this->m_YNbOfTile = this->m_CstrInfo->th;

    this->m_NbOfComponent = this->m_Image->numcomps;

    this->m_Precision = new unsigned int[this->m_NbOfComponent];
    if (!this->m_Precision)
      {
      this->Clean();
      return 0;
      }

    for (int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_Precision[itComp] = this->m_Image->comps[itComp].prec;
      }

    this->m_Signed = new int[this->m_NbOfComponent] ;
    if (!this->m_Signed)
      {
      this->Clean();
      return 0;
      }
    for (int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_Signed[itComp] = this->m_Image->comps[itComp].sgnd;
      }

    this->m_XResolution = new unsigned int[this->m_NbOfComponent];
    if (!this->m_XResolution)
      {
      this->Clean();
      return 0;
      }

    for (int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_XResolution[itComp] = this->m_Image->comps[itComp].dx;
      }

    this->m_YResolution = new unsigned int[this->m_NbOfComponent];
    if (!this->m_YResolution)
      {
      this->Clean();
      return 0;
      }

    for (int itComp = 0; itComp < this->m_NbOfComponent; itComp++)
      {
      this->m_YResolution[itComp] = this->m_Image->comps[itComp].dy;
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
       this->m_Image &&
       ( this->m_Width > 0 ) && ( this->m_Height > 0 ) &&
       ( this->m_TileWidth > 0 ) && ( this->m_TileHeight > 0 ) &&
       ( this->m_XNbOfTile > 0 ) && ( this->m_YNbOfTile > 0 ) &&
       ( this->m_NbOfComponent > 0 ) )
     {

     // We manage only JPEG2000 file with characteristics which are equal between components
     for(int itComp = 0; itComp < this->m_NbOfComponent - 1; itComp++)
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

  // Get nb. of lines and columns of the region to read
  int lNbLines     = this->GetIORegion().GetSize()[1];
  int lNbColumns   = this->GetIORegion().GetSize()[0];
  int lFirstLine   = this->GetIORegion().GetIndex()[1]; // [1... ]
  int lFirstColumn = this->GetIORegion().GetIndex()[0]; // [1... ]

  otbMsgDevMacro(<< " JPEG2000ImageIO::Read()  ");
  otbMsgDevMacro(<< " ImageDimension   : " << m_Dimensions[0] << "," << m_Dimensions[1]);
  otbMsgDevMacro(<< " IORegion         : " << this->GetIORegion());
  otbMsgDevMacro(<< " Nb Of Components : " << this->GetNumberOfComponents());
  otbMsgDevMacro(<< "IORegion: " << this->GetIORegion());
  otbMsgDevMacro(<< "Area to read: " << buffer_x0 << " " << buffer_y0  << " "
                 << buffer_x0 + buffer_size_x - 1 << " " << buffer_y0 + buffer_size_y - 1);
  otbMsgDevMacro(<< "Component type: " << this->GetComponentTypeAsString(this->GetComponentType()));

  itk::TimeProbe chrono;
  chrono.Start();

  // Set the decoded area
  if( !otbopenjpeg_opj_set_decode_area(m_InternalReader->GetCodec(),
                                       m_InternalReader->GetImage(),
                                       lFirstColumn,
                                       lFirstLine,
                                       lFirstColumn + lNbColumns,
                                       lFirstLine + lNbLines) )
    {
    itkExceptionMacro(<< "The decoded area is not correct!");
    m_InternalReader->Clean();
    return;
    }


  if ( !( otbopenjpeg_opj_decode_v2(m_InternalReader->GetCodec(), m_InternalReader->GetStream(), m_InternalReader->GetImage()) &&
          otbopenjpeg_opj_end_decompress(m_InternalReader->GetCodec(), m_InternalReader->GetStream()) )
     )
    {
    itkExceptionMacro(<< "Failed to decode the image!");
    m_InternalReader->Clean();
    return;
    }



  unsigned int nbPixel = (m_InternalReader->GetImage()->x1 - m_InternalReader->GetImage()->x0) *
                         (m_InternalReader->GetImage()->y1 - m_InternalReader->GetImage()->y0);
  //std::cout<< "NbPixel = " <<  nbPixel << std::endl;
  //std::cout<< "NbOfComp = " <<  m_InternalReader->m_NbOfComponent << std::endl;

  // Convert buffer from void * to unsigned char *
  //unsigned char *p = static_cast<unsigned char *>(buffer);



  // move the data into the buffer
  switch (this->GetComponentType())
  {
//    case CHAR:
//    case UCHAR:
//    case SHORT:
    case USHORT:
      {
      unsigned short *p = static_cast<unsigned short *>(buffer);
      for(unsigned int itPixel = 0; itPixel < nbPixel; itPixel++)
        {
        for (unsigned int itComp = 0; itComp < m_InternalReader->m_NbOfComponent; itComp++)
          {
          OPJ_INT32* data = m_InternalReader->GetImage()->comps[itComp].data;
          //unsigned short* datashort = reinterpret_cast<unsigned short*>(data);
          //OPJ_INT32 value = m_InternalReader->m_Image->comps[itComp].data[itPixel];
          *p = static_cast<unsigned short>(data[itPixel] & 0xffff);
          //p += m_BytePerPixel;
          p++;
          }
        }
      }
      break;
//    case INT:
//    case UINT:
  }

  chrono.Stop();
  otbMsgDevMacro(<< "JPEG2000ImageIO::Read took " << chrono.GetTotal() << " sec")

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
