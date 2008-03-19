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

#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbSystem.h"

/**
   sample error debug callback expecting no client object
*/
void error_callback(const char *msg, void *client_data) {
  (void)client_data;
  fprintf(stdout, "[ERROR] %s", msg);
}
/**
   sample warning debug callback expecting no client object
*/
void warning_callback(const char *msg, void *client_data) {
  (void)client_data;
  fprintf(stdout, "[WARNING] %s", msg);
}
/**
   sample debug callback expecting no client object
*/
void info_callback(const char *msg, void *client_data) {
  (void)client_data;
  fprintf(stdout, "[INFO] %s", msg);
}


namespace otb
{

  JPEG2000ImageIO::JPEG2000ImageIO()
  {
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

    m_NbOctetPixel = 1;
  }

  JPEG2000ImageIO::~JPEG2000ImageIO()
  {}

  bool JPEG2000ImageIO::CanReadFile( const char* filename )
  {
    if(filename == NULL)
      {
	return false;
      }
    // Creating a codec
    opj_codec_t * codec = opj_create_decompress(CODEC_J2K);
    if(!codec)
      {
	std::cout<<"Impossible to create codec."<<std::endl;
	return false;
      }
    opj_set_info_handler(codec, info_callback,00);
    opj_set_warning_handler(codec, warning_callback,00);
    opj_set_error_handler(codec, error_callback,00);
 
    // Setting default parameters
    opj_dparameters_t parameters;
    opj_set_default_decoder_parameters(&parameters);
 
    if(! opj_setup_decoder(codec,&parameters))
      {
	std::cout<<"Impossible to set parameter."<<std::endl;
	opj_destroy_codec(codec);
	return false;
      }

    // Creating a file
    FILE * file  = fopen(filename,"rb");
    if(!file)
      {
	std::cout<<"Impossible to open file."<<std::endl;
	opj_destroy_codec(codec);
	return false;
      }
    // Creating a file stream
    opj_stream_t * stream  = opj_stream_create_default_file_stream(file,true);
    if(!stream)
      {
	std::cout<<"Impossible to create stream."<<std::endl;
	opj_destroy_codec(codec);
	fclose(file);
	return false;
      }


    // trying to read image header
    opj_image_t * image;
    OPJ_INT32 tile_x0,tile_y0;
    OPJ_UINT32 tile_width,tile_height,nb_tiles_x,nb_tiles_y;
    bool resp = opj_read_header(codec,
				&image,
				&tile_x0,
				&tile_y0,
				&tile_width,
				&tile_height,
				&nb_tiles_x,
				&nb_tiles_y,
				stream);
    // Destroy the codec
    opj_stream_destroy(stream);
    fclose(file);
    opj_destroy_codec(codec);
    opj_image_destroy(image);
    return resp;
  }


  // Used to print information about this object
  void JPEG2000ImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

  // Read a 3D image (or event more bands)... not implemented yet
  void JPEG2000ImageIO::ReadVolume(void*)
  {
  }

  // Read image 
  void JPEG2000ImageIO::Read(void* buffer)
  {
    //  unsigned long step = this->GetNumberOfComponents();
    char * charstarbuffer = static_cast<char *>(buffer);
  
    int buffer_size_y = this->GetIORegion().GetSize()[1];
    int buffer_size_x = this->GetIORegion().GetSize()[0];
    int buffer_y0     = this->GetIORegion().GetIndex()[1] ; 
    int buffer_x0     = this->GetIORegion().GetIndex()[0] ;
  
    otbMsgDevMacro( <<" JPEG2000ImageIO::Read()  ");
    otbMsgDevMacro( <<" ImageDimension   : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
    otbMsgDevMacro( <<" IORegion         : "<<this->GetIORegion());
    otbMsgDevMacro( <<" Nb Of Components : "<<this->GetNumberOfComponents());

    otbMsgDevMacro(<<"IORegion: "<<this->GetIORegion());
    otbMsgDevMacro(<<"Area to read: "<<buffer_x0<<" "<<buffer_y0  <<" "<< buffer_x0+buffer_size_x-1 <<" "<<buffer_y0+buffer_size_y-1 );
    otbMsgDevMacro(<<"Component type: "<<this->GetComponentTypeAsString(this->GetComponentType()));

    // Creating openjpeg objects
    m_Codec = opj_create_decompress(CODEC_J2K);
    opj_set_info_handler(m_Codec, info_callback,00);
    opj_set_warning_handler(m_Codec, warning_callback,00);
    opj_set_error_handler(m_Codec, error_callback,00);

    if(!m_Codec)
      {
	itkExceptionMacro(<<"Failed to create openjpeg codec.");
      }

    // Create default parameters
    opj_set_default_decoder_parameters(&m_Parameters);
    // Set the requested region
    opj_restrict_decoding(&m_Parameters,buffer_x0,buffer_y0,buffer_x0+buffer_size_x-1,buffer_y0+buffer_size_y-1);

    if(!opj_setup_decoder(m_Codec,&m_Parameters))
      {
	itkExceptionMacro(<<"Failed to set up decoder parameters.");
      }

    m_File = fopen(m_FileName.c_str(),"rb");

    if(!m_File)
      {
	itkExceptionMacro(<<"Failed to open file: "<<m_FileName);
      }
  
    m_OpenJpegStream = opj_stream_create_default_file_stream(m_File,true);
  
    if(!m_OpenJpegStream)
      {
	itkExceptionMacro(<<"Failed to create file stream.");
      }

    OPJ_INT32 tile_x0,tile_y0;
    OPJ_UINT32 tile_width,tile_height,nb_tiles_x,nb_tiles_y;

    if(!opj_read_header(m_Codec,
			&m_OpenJpegImage,
			&tile_x0,
			&tile_y0,
			&tile_width,
			&tile_height,
			&nb_tiles_x,
			&nb_tiles_y,
			m_OpenJpegStream))
      {
	itkExceptionMacro(<<"Failed to read image header.");
      }

    bool goesOn = true;
    OPJ_BYTE * tile_data;
    OPJ_INT32  tile_x1,tile_y1;
    OPJ_UINT32 data_size,nb_comps;
    OPJ_UINT32 tile_index;

    while(goesOn)
      {
      	if(! opj_read_tile_header(m_Codec,
				  &tile_index,
				  &data_size,
				  &tile_x0,
				  &tile_y0,
				  &tile_x1,
				  &tile_y1,
				  &nb_comps,
				  &goesOn,
				  m_OpenJpegStream))
	  {
	    itkExceptionMacro(<<"Error while reading tile header.");
	  }
	if(goesOn)
	  {
	    otbMsgDevMacro(<<"==========================");
	    otbMsgDevMacro(<<"Tile index: "<<tile_index);
	    otbMsgDevMacro(<<"Data size: "<<data_size);
	    otbMsgDevMacro(<<"Tile (x0,y0): "<<tile_x0<<" "<<tile_y0);
	    otbMsgDevMacro(<<"Tile (x1,y1): "<<tile_x1<<" "<<tile_y1);
	    otbMsgDevMacro(<<"Tile number of component: "<<nb_comps);
	    otbMsgDevMacro(<<"Goes on: "<<goesOn);
	    otbMsgDevMacro(<<"--------------------------");
	    
	    
	    tile_data = new OPJ_BYTE[data_size];
	    
	    if(! opj_decode_tile_data(m_Codec,tile_index,tile_data,data_size,m_OpenJpegStream))
	      {
		itkExceptionMacro(<<"Error while reading tile data.");
	      }
	    
	    std::streamsize tile_component_size = data_size/nb_comps;
	    std::streamoff  buffer_skip         = std::max(0,tile_y0-buffer_y0)*buffer_size_x*nb_comps*m_NbOctetPixel;
	    std::streamoff tile_skip            = std::max(0,buffer_y0-tile_y0)*(tile_x1-tile_x0)*m_NbOctetPixel;
	    std::streamoff tile_offset_begin    = std::max(0,buffer_x0-tile_x0)*m_NbOctetPixel;
	    std::streamoff tile_offset_end      = std::max(0,tile_x1 - buffer_x0 - buffer_size_x)*m_NbOctetPixel;
	    std::streamoff buffer_offset_begin  = std::max(0,tile_x0-buffer_x0)*nb_comps*m_NbOctetPixel;
	    std::streamoff buffer_offset_end    = std::max(0,buffer_x0+buffer_size_x-tile_x1)*nb_comps*m_NbOctetPixel;
	    std::streamsize line_size           = (std::min(tile_x1,buffer_x0+buffer_size_x)-std::max(tile_x0,buffer_x0))*m_NbOctetPixel;
	    std::streamsize nb_lines            = std::min(tile_y1,buffer_y0+buffer_size_y)- std::max(tile_y0,buffer_y0);	    
	    std::streampos buffer_step          = nb_comps * m_NbOctetPixel;

	    otbMsgDevMacro(<<"buffer_skip: "<<buffer_skip);
	    otbMsgDevMacro(<<"tile_skip: "<<tile_skip);
	    otbMsgDevMacro(<<"buffer_offset_begin: "<<buffer_offset_begin);
	    otbMsgDevMacro(<<"buffer_offset_end: "<<buffer_offset_end);
	    otbMsgDevMacro(<<"tile_offset_begin: "<<tile_offset_begin);
	    otbMsgDevMacro(<<"tile_offset_end: "<<tile_offset_end);
	    otbMsgDevMacro(<<"line_size: "<<line_size);
	    otbMsgDevMacro(<<"nb_lines: "<<nb_lines);
	    
	    
	    std::streamoff buffer_pos,tile_pos;
	    
	    for(unsigned int comp=0;comp<nb_comps;++comp)
	      {
		buffer_pos = buffer_skip + comp * m_NbOctetPixel;
		tile_pos   = tile_skip +comp * tile_component_size;
		for(int lines = 0;lines<nb_lines;++lines)
	    	  {
	    	    buffer_pos += buffer_offset_begin;
	    	    tile_pos   += tile_offset_begin;
		    
	    	    for(int cols = 0; cols < line_size;cols++)
	    	      {
			memcpy((void*)(&(charstarbuffer[buffer_pos])),(const void*)(&(tile_data[tile_pos])),(size_t)(m_NbOctetPixel));
			buffer_pos+=buffer_step;
			tile_pos +=m_NbOctetPixel;
	    	      }
		    buffer_pos += buffer_offset_end;
		    tile_pos   += tile_offset_end;
	    	  }
	      }
	    
	    delete[] tile_data;
	  }
      }
    otbMsgDevMacro(<<"==========================");

   
    opj_stream_destroy(m_OpenJpegStream);
    fclose(m_File);
    opj_destroy_codec(m_Codec);
    opj_image_destroy(m_OpenJpegImage);
  }


  void JPEG2000ImageIO::ReadImageInformation()
  {
    if(  m_FileName.empty() == true )
      {
	itkExceptionMacro(<<"JPEG2000ImageIO: empty image filename.");
      }
  
    // Creating openjpeg objects
    m_Codec = opj_create_decompress(CODEC_J2K);
    opj_set_info_handler(m_Codec, info_callback,00);
    opj_set_warning_handler(m_Codec, warning_callback,00);
    opj_set_error_handler(m_Codec, error_callback,00);

    if(!m_Codec)
      {
	itkExceptionMacro(<<"Failed to create openjpeg codec.");
      }

    // Create default parameters
    opj_set_default_decoder_parameters(&m_Parameters);

    if(!opj_setup_decoder(m_Codec,&m_Parameters))
      {
	itkExceptionMacro(<<"Failed to set up decoder parameters.");
      }

    m_File = fopen(m_FileName.c_str(),"rb");

    if(!m_File)
      {
	itkExceptionMacro(<<"Failed to open file: "<<m_FileName);
      }
  
    m_OpenJpegStream = opj_stream_create_default_file_stream(m_File,true);
  
    if(!m_OpenJpegStream)
      {
	itkExceptionMacro(<<"Failed to create file stream.");
      }

    OPJ_INT32 tile_x0,tile_y0;
    OPJ_UINT32 tile_width,tile_height,nb_tiles_x,nb_tiles_y;

    if(!opj_read_header(m_Codec,
			&m_OpenJpegImage,
			&tile_x0,
			&tile_y0,
			&tile_width,
			&tile_height,
			&nb_tiles_x,
			&nb_tiles_y,
			m_OpenJpegStream))
      {
	itkExceptionMacro(<<"Failed to read image header.");
      }

    m_Dimensions[0]=m_OpenJpegImage->x1 - m_OpenJpegImage->x0;
    m_Dimensions[1]=m_OpenJpegImage->y1 - m_OpenJpegImage->y0;
  
    this->SetNumberOfDimensions(2);

    if(m_Dimensions[0] == 0 || m_Dimensions[1] == 0)
      {
	itkExceptionMacro(<<"Image size is null.");
      }
  
    this->SetNumberOfComponents(m_OpenJpegImage->numcomps);
  
    this->SetFileTypeToBinary();
  
    unsigned int precision = m_OpenJpegImage->comps->prec;
    int isSigned = m_OpenJpegImage->comps->sgnd;

    if(precision<=8)
      {
	m_NbOctetPixel = 1;
	if(isSigned)
	  {
	    SetComponentType(CHAR);
	  }
	else
	  {
	    SetComponentType(UCHAR);
	  }
      }
    else if(precision<=16)
      {
	m_NbOctetPixel = 2;
	if(isSigned)
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
	m_NbOctetPixel = 4;
	if(isSigned)
	  {
	    SetComponentType(INT);
	  }
	else
	  {
	    SetComponentType(UINT);
	  }
      }

    if( this->GetNumberOfComponents() == 1 )
      {
	this->SetPixelType(SCALAR);
      }
    else
      {
	this->SetPixelType(VECTOR);
      }	

    otbMsgDevMacro(<<"==========================");
    otbMsgDevMacro(<<"ReadImageInformation: ");
    otbMsgDevMacro(<<"Tile (x0,y0): "<<tile_x0<<" "<<tile_y0);
    otbMsgDevMacro(<<"Tile size: "<<tile_width<<" x "<<tile_height);
    otbMsgDevMacro(<<"Number of tiles: "<<nb_tiles_x<<" "<<nb_tiles_y);
    otbMsgDevMacro(<<"Precision: "<<precision);
    otbMsgDevMacro(<<"Signed: "<<isSigned);
    otbMsgDevMacro(<<"Number of octet per value: "<<m_NbOctetPixel);
    otbMsgDevMacro(<<"==========================");


    otbMsgDebugMacro( <<"Driver to read: JPEG2000");
    otbMsgDebugMacro( <<"         Read  file         : "<< m_FileName);
    otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
    otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
    otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
    otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
    otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

    opj_stream_destroy(m_OpenJpegStream);
    fclose(m_File);
    opj_destroy_codec(m_Codec);
    opj_image_destroy(m_OpenJpegImage);
  }


  bool JPEG2000ImageIO::CanWriteFile( const char* filename )
  {
    return false;
#if 0
    std::string lFileName(filename);
    if( System::IsADirName(lFileName) == true )
      {
	return false;
      }
    const std::string Extension = System::GetExtension(filename);
    if( (Extension == "j2k") || (Extension == "J2K") || (Extension == "jp2") || (Extension == "JP2"))
      {
	return true;
      }
    else
      {
	return false;
      }
#endif
  }

  void JPEG2000ImageIO::Write(const void* buffer)
  {









  }


  void JPEG2000ImageIO::WriteImageInformation()
  {
    if ( m_FileName == "" )
      {
	itkExceptionMacro(<<"A FileName must be specified.");
      }
    if( CanWriteFile(m_FileName.c_str()) == false)
      {
	itkExceptionMacro(<< "The file "<<m_FileName.c_str()<<" is not defined as a JPEG2000 file");
      }

    otbMsgDebugMacro( <<"Driver to write: JPEG2000");
    otbMsgDebugMacro( <<"         Write file         : "<< m_FileName);
    otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
    otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
    otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
    otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
    otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

  }

  
} // end namespace otb

