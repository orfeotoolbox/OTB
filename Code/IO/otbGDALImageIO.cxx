#include "itkExceptionObject.h"
#include "itkByteSwapper.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include <gdal.h>
#include <gdal_priv.h>
#include <iostream.h>
#include <string.h>
#include <list>
#include <math.h>
#include <zlib.h>

#include "otbGDALImageIO.h"

namespace otb
{

GDALImageIO::GDALImageIO()
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

  m_currentfile = NULL;
}

GDALImageIO::~GDALImageIO()
{
}

// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file) 
{
  // First check the extension
  m_currentfile = file;
//  std::cout<<"Filename : "<<m_currentfile<<endl;
  
  if(  file == NULL )
    {
      itkDebugMacro(<<"No filename specified.");
      return false;
    }

  // Init GDAL parameters 
  GDALAllRegister();
  
  // Open file with GDAL 
  poDataset = (GDALDataset *) GDALOpen(file, GA_ReadOnly );
  if(poDataset==NULL)
    {
      itkDebugMacro(<<"No dataset ");
      return false;
    }

  else
    {
      return true;
    }
}

// Used to print information about this object
void GDALImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Compression Level : " << m_CompressionLevel << "\n";
}

// Read a 3D image (or event more bands)... not implemented yet
void GDALImageIO::ReadVolume(void*)
{
}

// Not used anymore 
/*void GDALImageIO::SampleImage(void* buffer,int XBegin, int YBegin, int SizeXRead, int SizeYRead, int XSample, int YSample)
{
  InputPixelType * p = static_cast<InputPixelType *>(buffer);
  // pafimas = new float*[NbBands];
  //p= new unsigned short*;
  
  if(p==NULL)
   {
   std::cout<<"Error in allocating memory"<<endl;
   return;
   }

  if((poBands[0]->RasterIO( GF_Read,XBegin, YBegin,SizeXRead,SizeYRead , p , XSample, YSample, GDT_Byte,0, 0 ))==CE_Failure)
    std::cout<<"[GDALImageIO::Read] Failed in reading image "<<std::endl;
}*/

// Read image with GDAL
void GDALImageIO::Read(void* buffer)
{
  // For the moment, we only deals with USHORT..... See after how to
  // enable different types!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  InputPixelType * p = static_cast<InputPixelType *>(buffer);
  
  if(p==NULL)
   {
   std::cout<<"Error in allocating memory"<<endl;
   return;
   }

  if((poBands[0]->RasterIO( GF_Read,0,0,m_Dimensions[0],m_Dimensions[1] , p , m_Dimensions[0], m_Dimensions[1], GDT_Byte,0, 0 ))==CE_Failure)
    std::cout<<"[GDALImageIO::Read] Failed in reading image "<<std::endl;

}

void GDALImageIO::ReadImageInformation()
{
  //std::ifstream file;
  this->InternalReadImageInformation();
}
  
void GDALImageIO::InternalReadImageInformation()
{
  int i;

  m_FileName = (std::string) m_currentfile;
  if(  m_currentfile == NULL )
    {
    itkDebugMacro(<<"No filename specified.");
    }
  
  // Init GDAL parameters 
  GDALAllRegister();
  
  // Get Dataset 
  poDataset = (GDALDataset *) GDALOpen(m_currentfile, GA_ReadOnly );
  if(poDataset==NULL)
    {
    itkExceptionMacro(<<"No dataset in GDAL file.");
    return;
    }
  
  else
    {
    // Get image dimensions
    width = poDataset->GetRasterXSize();
    height = poDataset->GetRasterYSize();
       
    if( width==0 & height==0)
      {
      itkExceptionMacro(<<"No dimension defined.");
      }
    else
      {
      // Set image dimensions into IO
      m_Dimensions[0] = width;
      m_Dimensions[1] = height;
//      std::cout<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]<<endl;
      }

    // Set Number of components
    // Get Number of Bands
    NbBands = poDataset->GetRasterCount();
    m_NumberOfComponents = NbBands;
//    std::cout<<"Nb of Components : "<<m_NumberOfComponents<<endl;
    
    // Set the number of dimensions (verify for the dim )
    if( NbBands > 1 )
      {
      this->SetNumberOfDimensions(3);
      m_Dimensions[2] = NbBands;
      }
    else
      this->SetNumberOfDimensions(2);

//    std::cout<<"Nb of Dimensions : "<<m_NumberOfDimensions<<endl;

    // Automatically set the Type to Binary for GDAL data
    this->SetFileTypeToBinary();
    
   // Get all the Bands
    poBands = new GDALRasterBand* [NbBands];
    if(poBands==NULL)
      {
      itkExceptionMacro(<<"No dataset in GDAL file.");
      return;
      }
    for(i=0 ; i<NbBands ; i++)
      poBands[i] = poDataset->GetRasterBand(i+1);

    // Get Data Type
    // Consider only the data type given by the first band!!!!!
    // Maybe be could changed (to check)
    PxType = poBands[0]->GetRasterDataType();

    // Following the data type given by GDAL we set it for ImageIO
    // BE CAREFUL !!!! At this time the complex data type are regarded
    // as integer data type in hope that ITK uses that kind of system
    // (take time to check !!)
    if(PxType == GDT_Byte)
      {
      SetComponentType(CHAR);
      }
    else if(PxType == GDT_UInt16)
      {
      SetComponentType(USHORT);
      }
    else if((PxType == GDT_Int16) || (PxType == GDT_CInt16) )
      {
      SetComponentType(SHORT);
      }
    else if(PxType == GDT_UInt32)
      {
      SetComponentType(ULONG);
      }
    else if((PxType == GDT_Int32) || (PxType == GDT_CInt32) )
      {
      SetComponentType(LONG);
      }
    else if((PxType == GDT_Float32) || (PxType = GDT_CFloat32) )
      {
      SetComponentType(FLOAT);
      }
    else if((PxType == GDT_Float64) || (PxType == GDT_CFloat64) )
      {
      SetComponentType(DOUBLE);
      }
    else
      {
      itkExceptionMacro(<<"Unrecognized type");
      }

//    std::cout<<"Component Type : "<<m_ComponentType<<endl;
    
    // See to take the spacing with GDAL
    // Find a way to get the good indice for papszMetadata for all the
    // dataset (the one set is working for dat_01.001!!)
    /**********************************/ 
    /*char** papszMetadata = GDALGetMetadata(poDataset,NULL);
    if(CSLCount(papszMetadata) > 0)
      {
      m_Spacing[0] = (double)papszMetadata[18];
      m_Spacing[1] = (double)papszMetadata[17];
      }*/
    /**********************************/
    
    // Default Spacing
    m_Spacing[0]=1;
    m_Spacing[1]=1;
    if(m_NumberOfDimensions==3)
      m_Spacing[2]=1;
    
    /********************** SPACING ***********************/
    // Look for which type of image it is (Spot ?, RadarSat ?, ...)
    // and set the corresponding spacing
    int i;
    char** papszMetadata;
    bool spot = false;
    bool other = false;
    // Specific to Spot images
    char S[] = "TIFFTAG_IMAGEDESCRIPTION=XS3 XS2 XS1";
    char spacx[] = "CEOS_LINE_SPACING_METERS=";
    char spacy[] = "CEOS_PIXEL_SPACING_METERS=";
    char * cpS;
    char * cpSpacx;
    char * cpSpacy;
    papszMetadata = GDALGetMetadata( poDataset, NULL );
    
    GDALDriverH hDriver = GDALGetDatasetDriver( poDataset );
    printf( "Driver: %s/%s\n",
	    GDALGetDriverShortName( hDriver ),
	    GDALGetDriverLongName( hDriver ) );
    if(strcmp(GDALGetDriverLongName( hDriver ), "CEOS Image")==0)
      {
      m_Spacing[0] = 10;
      m_Spacing[1] = 10;
      }
    
    if( CSLCount(papszMetadata) > 0 )
       {
       printf( "Metadata:\n" );
       for( i = 0; papszMetadata[i] != NULL; i++ )
	 {
	 cpS = strstr (papszMetadata[i], S);
	 if(cpS != NULL)
	   {
	   spot = true;
	   break;
	   }
	 }
       // use the number of bands to know which spot
       if(spot=true)
	 {
	 if(NbBands == 3)
	   {
	   m_Spacing[0] = 20;
	   m_Spacing[1] = 20;
	   }
	 else if(NbBands == 4)
	   {
	   if(m_Dimensions[0] == 6000 && m_Dimensions[1] == 6000 )
	     {
	     m_Spacing[0] = 10;
	     m_Spacing[1] = 10;
	     }
	   else
	     {
	     m_Spacing[0] = 20;
	     m_Spacing[1] = 20;
	     }
	   }
	 else if(NbBands == 1)
	   {
	   if(m_Dimensions[0] == 12000 && m_Dimensions[1] == 12000)
	     {
	     m_Spacing[0] = 5;
	     m_Spacing[1] = 5;
	     }
	   else if(m_Dimensions[0] == 24000 && m_Dimensions[1] == 24000)
	     {
	     m_Spacing[0] = 2.5;
	     m_Spacing[1] = 2.5;
	     }
	   else
	     {
	     m_Spacing[0] = 10;
	     m_Spacing[1] = 10;
	     }
	   }
	 }
       else
	 {
	 for( i = 0; papszMetadata[i] != NULL; i++ )
	   {
	   cpSpacx = strstr(papszMetadata[i], spacx);
	   if(cpSpacx != NULL)
	     {
	     other = true;
	     break;
	     }
	   }
	 if(other)
	   {
	   char * pch = strtok (papszMetadata[i],spacx);
	   double x = atof(pch);
	   
	   for( i = 0; papszMetadata[i] != NULL; i++ )
	     {
	     cpSpacy = strstr(papszMetadata[i], spacy);
	     if(cpSpacy != NULL)
	       break;
	     }
	   pch = strtok (papszMetadata[i],spacy);
	   double y = atof(pch);
	   
	   std::cout<<"spacing : "<<x<<"  "<<y<<std::endl;
	   }
	 }
       }
     
     
     
    /******************************************************************/
    // Pixel Type always set to Scalar for GDAL ? maybe also to vector ?
    int numComp = 1;
    this->SetPixelType(SCALAR);
    this-> SetNumberOfComponents(numComp);
    }

}

/** TODO : Methode CanWriteFile non implementee */
bool GDALImageIO::CanWriteFile( const char* name )
{
}

/** TODO : Methode Write non implementee */
void GDALImageIO::Write(const void* buffer)
{
}

/** TODO : Methode WriteImageInformation non implementee */
void GDALImageIO::WriteImageInformation()
{
}
  
} // end namespace itk

