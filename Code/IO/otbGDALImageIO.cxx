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
#include "itkByteSwapper.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"

#include <gdal.h>
#include <gdal_priv.h>
//#include <iostream.h>
#include <string.h>
#include <list>
#include <vector>
#include <math.h>
//#include <zlib.h>

#include "otbGDALImageIO.h"
#include "otbMacro.h"
#include "otbSystem.h"
#include "otbImageBase.h"

#include "itkMetaDataObject.h"
#include "itkPNGImageIO.h"
#include "itkJPEGImageIO.h"

#include <iostream>
#include <fstream>

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
  m_poBands     = NULL;
  m_hDriver     = NULL;
  m_poDataset   = NULL;
  
  m_NbBands = 0;
  m_FlagWriteImageInformation = true;

}

GDALImageIO::~GDALImageIO()
{
//THOMAS
//	if( m_hDriver != NULL ) GDALClose( m_hDriver ); //Ne pas le faire  sinon SegFault !!!!
//         if( m_poBands != NULL ) delete [] m_poBands;
// 	if( m_poDataset != NULL ) delete m_poDataset;
 
   if(m_poDataset != NULL)
     {
       
       delete m_poDataset;
       m_poDataset = NULL;
     }
   if(m_poBands != NULL)
     {
       delete [] m_poBands;
     }
  GDALDestroyDriverManager();
}



// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file) 
{
  // First check the extension
  if(  file == NULL )
    {
      itkDebugMacro(<<"No filename specified.");
      return false;
    }
        bool lCanRead(false);

        //Traitement particulier sur certain format o� l'on pr�f�re utiliser 
        // Format PNG -> lecture avec ITK (pas GDAL car certains tests sortent en erreurs)
  		itk::PNGImageIO::Pointer lPNGImageIO = itk::PNGImageIO::New();
        lCanRead = lPNGImageIO->CanReadFile(file);
        if ( lCanRead == true)
        {
                return false;
        } 
 		itk::JPEGImageIO::Pointer lJPEGImageIO = itk::JPEGImageIO::New();
        lCanRead = lJPEGImageIO->CanReadFile(file);
        if ( lCanRead == true)
        {
                return false;
        }

        // Regarde si c'est un r�pertoire
/*        std::string lFileNameGdal;
	otbMsgDevMacro(<<"GDALImageIO::CanReadFile()");
        bool found = GetGdalReadImageFileName(file,lFileNameGdal);
	if( found == false )
	{
        	return false;
	}
*/
        std::string lFileNameGdal;
        lFileNameGdal = std::string(file);
	
        // Init GDAL parameters 
        GDALAllRegister();

  // Open file with GDAL 
        m_poDataset = static_cast<GDALDataset *>(GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly ));
        if(m_poDataset==NULL)
        {
                fprintf( stderr,
                 "GDALOpen failed - %d\n%s\n",
                 CPLGetLastErrorNo(), CPLGetLastErrorMsg() );
		 
                GDALDumpOpenDatasets( stderr );
                GDALDestroyDriverManager();
                CPLDumpSharedList( NULL );
                itkDebugMacro(<<"No dataset ");
	        otbMsgDevMacro(<<"Not CanReadFile GDAL");
                return false;
        }
        else
        {
                delete m_poDataset;
                m_poDataset = NULL;
	        otbMsgDevMacro(<<"CanReadFile GDAL");
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


// Read image with GDAL
void GDALImageIO::Read(void* buffer)
{
        std::streamoff step = static_cast<std::streamoff>(this->GetNumberOfComponents());
        unsigned char * p = static_cast<unsigned char *>(buffer);
        if(p==NULL)
        {
                itkExceptionMacro(<<"Memory allocation error");
                return;
        }

        int lNbLignes   = this->GetIORegion().GetSize()[1];
        int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]

otbMsgDevMacro( <<" GDALImageIO::Read()  ");
otbMsgDevMacro( <<" Image size  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDevMacro( <<" Region read (IORegion)  : "<<this->GetIORegion());
otbMsgDevMacro( <<" Nb Of Components  : "<<this->GetNumberOfComponents());

        std::streamoff lNbPixels = (static_cast<std::streamoff>(lNbColonnes))*(static_cast<std::streamoff>(lNbLignes));
		std::streamoff lTailleBuffer = static_cast<std::streamoff>(m_NbOctetPixel)*lNbPixels;

otbMsgDevMacro( <<" Allocation buff tempon taille : "<<lNbPixels<<"*"<<m_NbOctetPixel<<" (NbOctetPixel) = "<<lTailleBuffer);
otbMsgDevMacro( <<" sizeof(streamsize)    : "<<sizeof(std::streamsize));
otbMsgDevMacro( <<" sizeof(streampos)     : "<<sizeof(std::streampos));
otbMsgDevMacro( <<" sizeof(streamoff)     : "<<sizeof(std::streamoff));
otbMsgDevMacro( <<" sizeof(std::ios::beg) : "<<sizeof(std::ios::beg));
otbMsgDevMacro( <<" sizeof(size_t)        : "<<sizeof(size_t));
//otbMsgDevMacro( <<" sizeof(pos_type)      : "<<sizeof(pos_type));
//otbMsgDevMacro( <<" sizeof(off_type)      : "<<sizeof(off_type));
otbMsgDevMacro( <<" sizeof(unsigned long) : "<<sizeof(unsigned long));



        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
          itkExceptionMacro(<<"Memory allocation error");
                return;
        }


        CPLErr lCrGdal;
        std::streamoff cpt(0);
	
  	if( GDALDataTypeIsComplex(m_PxType) )
	{
			otbMsgDevMacro( << " GDALDataTypeIsComplex begin ");
			lCrGdal = m_poBands[0]->RasterIO( GF_Read,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 );

			if (lCrGdal == CE_Failure)
            {
    	        	itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str()<<".");
            }
			cpt = 0;
            for ( std::streamoff  i=0 ; i < lTailleBuffer ; i = i+static_cast<std::streamoff>(m_NbOctetPixel) )
            {
                memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));		       
		    	cpt += static_cast<std::streamoff>(m_NbOctetPixel);
            }		
	}
	else
	{
        	// Mise a jour du step
        	step = step * static_cast<std::streamoff>(m_NbOctetPixel);

        	for (unsigned int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        	{
			lCrGdal = m_poBands[nbComponents]->RasterIO( GF_Read,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
			if (lCrGdal == CE_Failure)
                	{
                          itkExceptionMacro(<< "Error while reading image (GDAL format) " << m_FileName.c_str()<<".");
                	}
                	// Recopie dans le buffer                 
                	cpt = static_cast<std::streamoff>(nbComponents)*static_cast<std::streamoff>(m_NbOctetPixel);
                	for ( std::streamoff  i=0 ; i < lTailleBuffer ; i = i+static_cast<std::streamoff>(m_NbOctetPixel) )
                	{
                        	memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
                        	cpt += step;
                	}
          	}
	}
	
        delete [] value;
        value = NULL;
otbMsgDevMacro( << "GDALImageIO::Read() completed");
}

void GDALImageIO::ReadImageInformation()
{
  //std::ifstream file;
  this->InternalReadImageInformation();
}
  
void GDALImageIO::InternalReadImageInformation()
{
  int i;

  if(  m_FileName.empty() == true )
    {
        itkExceptionMacro(<<"GDAl read : empty image file name file.");
    }
  
  // Recupre le nom r�el du fichier a ouvrir
/*  std::string lFileNameGdal;
  bool found = GetGdalReadImageFileName(m_FileName.c_str(),lFileNameGdal);
  if( found == false )
  {
        itkExceptionMacro(<<"The file "<<m_FileName<<" is not supported by GDAL !!!");
  }
*/
  std::string lFileNameGdal = m_FileName;

  // Init GDAL parameters 
  GDALAllRegister();
  
  // Get Dataset 
  if(m_poDataset != NULL)
    {
      delete m_poDataset;
      m_poDataset = NULL;
    }
  m_poDataset = static_cast<GDALDataset *>( GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly ));
  if(m_poDataset==NULL)
    {
    itkExceptionMacro(<<"Gdal dataset is null.");
    return;
    }
  
  else
    {
    // Get image dimensions
      m_width = m_poDataset->GetRasterXSize();
      m_height = m_poDataset->GetRasterYSize();
       
    if( (m_width==0) || (m_height==0))
      {
      itkExceptionMacro(<<"Dimension is undefined.");
      }
    else
      {
      // Set image dimensions into IO
      m_Dimensions[0] = m_width;
      m_Dimensions[1] = m_height;
      otbMsgDevMacro(<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]);
      }
    
    // Get Number of Bands
    m_NbBands = m_poDataset->GetRasterCount();
    this->SetNumberOfComponents(m_NbBands);
   
    otbMsgDevMacro(<<"NbBands : "<<m_NbBands);
    otbMsgDevMacro(<<"Nb of Components : "<<this->GetNumberOfComponents());
    
    // Set the number of dimensions (verify for the dim )
      this->SetNumberOfDimensions(2);

    otbMsgDevMacro(<<"Nb of Dimensions : "<<m_NumberOfDimensions);

    // Automatically set the Type to Binary for GDAL data
    this->SetFileTypeToBinary();
    
   // Get all the Bands
    m_poBands = new GDALRasterBand * [m_NbBands];
    if(m_poBands==NULL)
      {
      itkExceptionMacro(<<"Erreur d'allocation memoire du 'rasterBands'");
      return;
      }
    for(i=0 ; i<m_NbBands ; i++)
      m_poBands[i] = m_poDataset->GetRasterBand(i+1);

    // Get Data Type
    // Consider only the data type given by the first band!!!!!
    // Maybe be could changed (to check)
    m_PxType = m_poBands[0]->GetRasterDataType();

    // Following the data type given by GDAL we set it for ImageIO
    // BE CAREFUL !!!! At this time the complex data type are regarded
    // as integer data type in hope that ITK uses that kind of system
    // (take time to check !!)
    if(m_PxType == GDT_Byte)
      {
//      SetComponentType(CHAR);
      SetComponentType(UCHAR);
      }
    else if(m_PxType == GDT_UInt16)
      {
      SetComponentType(USHORT);
      }
    else if((m_PxType == GDT_Int16) || (m_PxType == GDT_CInt16) )
      {
      SetComponentType(SHORT);
      }
    else if(m_PxType == GDT_UInt32)
      {
      SetComponentType(UINT);
      }
    else if((m_PxType == GDT_Int32) || (m_PxType == GDT_CInt32) )
      {
      SetComponentType(INT);
      }
    else if((m_PxType == GDT_Float32) || (m_PxType == GDT_CFloat32) )
      {
      SetComponentType(FLOAT);
      }
    else if((m_PxType == GDT_Float64) || (m_PxType == GDT_CFloat64) )
      {
      SetComponentType(DOUBLE);
      }
    else
      {
      itkExceptionMacro(<<"Type de codage des pixels non reconu");
      }


        if ( this->GetComponentType() == CHAR )
        {
                m_NbOctetPixel = 1;
        }
        else if ( this->GetComponentType() == UCHAR )
        {
                m_NbOctetPixel = 1;
        }
        else if ( this->GetComponentType() == USHORT )
        {
                m_NbOctetPixel = 2;
        }
        else if ( this->GetComponentType() == SHORT )
        {
                m_NbOctetPixel = 2;
        }
        else if ( this->GetComponentType() == INT )
        {
                m_NbOctetPixel = 4;
        }
        else if ( this->GetComponentType() == UINT )
        {
                m_NbOctetPixel = 4;
        }
        else if ( this->GetComponentType() == FLOAT )
        {
                m_NbOctetPixel = 4;
        }
        else if ( this->GetComponentType() == DOUBLE )
        {
                m_NbOctetPixel = 8;
        }
        else 
        {
                m_NbOctetPixel = 1;
        }
    

    /******************************************************************/
    // Pixel Type always set to Scalar for GDAL ? maybe also to vector ?

// Modif Patrick: LIRE LES IMAGES COMPLEXES
 	if( GDALDataTypeIsComplex(m_PxType) )
	{
    		otbMsgDevMacro(<<"SetPixelType(COMPLEX)");
		m_NbOctetPixel = m_NbOctetPixel * 2 ;
          	this->SetNumberOfComponents( 2 );
                this->SetPixelType(COMPLEX);
		// Is this necessary ?
		//if(m_NbBands !=1) itkExceptionMacro(<<"GDALImageIO::InternalReadImageInformation() Can read only one band image ");
	}
	else
	{
    		otbMsgDevMacro(<<"SetPixelType(NoCOMPLEX)");
        	this-> SetNumberOfComponents(m_NbBands);
        	if( this->GetNumberOfComponents() == 1 )
        	{
                	this->SetPixelType(SCALAR);
        	}
        	else
        	{
                	this->SetPixelType(VECTOR);
        	}
	}	

    otbMsgDevMacro(<<"m_PxType : "<<m_PxType);
    otbMsgDevMacro(<<"Component Type : "<<m_ComponentType);
    otbMsgDevMacro(<<"NbOctetPixel   : "<<m_NbOctetPixel);


    }
    
/*----------------------------------------------------------------------*/
/*-------------------------- METADATA ----------------------------------*/
/*----------------------------------------------------------------------*/
 
    // Now initialize the itk dictionary
    itk::MetaDataDictionary & dico = this->GetMetaDataDictionary();
      
    
/* -------------------------------------------------------------------- */
/*  Get Spacing								*/
/* -------------------------------------------------------------------- */    
       
    // Default Spacing
     m_Spacing[0]=1;
     m_Spacing[1]=1;
    if(m_NumberOfDimensions==3)
      m_Spacing[2]=1;
      
    char** papszMetadata;
    papszMetadata =  m_poDataset->GetMetadata( NULL );
        
//     const char *pszValue;
    
//     pszValue = CSLFetchNameValue( papszMetadata, "CEOS_LINE_SPACING_METERS" );
//     if ( pszValue != NULL )
//        m_Spacing[0] = atof( pszValue );
    
//     pszValue = CSLFetchNameValue( papszMetadata, "CEOS_PIXEL_SPACING_METERS" );
//     if ( pszValue != NULL )
//        m_Spacing[1] = atof( pszValue );
 
    
    
    /* -------------------------------------------------------------------- */
    /*      Report general info.                                            */
    /* -------------------------------------------------------------------- */
    GDALDriverH		hDriver;
    
    hDriver = m_poDataset->GetDriver();
            
    itk::EncapsulateMetaData<std::string>(dico, MetaDataKey::m_DriverShortNameKey, 
            	static_cast<std::string>( GDALGetDriverShortName( hDriver ) ) );        
    itk::EncapsulateMetaData<std::string>(dico, MetaDataKey::m_DriverLongNameKey, 
            	static_cast<std::string>( GDALGetDriverLongName( hDriver ) ) );
            	 

/* -------------------------------------------------------------------- */    
/* Get the projection coordinate system of the image : ProjectionRef	*/
/* -------------------------------------------------------------------- */
    
    if( m_poDataset->GetProjectionRef() != NULL )
    {
        OGRSpatialReference*   pSR;
        const char *		      pszProjection = NULL;
        
        pSR = new OGRSpatialReference( pszProjection );

        pszProjection =  m_poDataset->GetProjectionRef();

        if( pSR->importFromWkt( (char **)(&pszProjection) ) == CE_None )
        {
            char *	pszPrettyWkt = NULL;

            pSR->exportToPrettyWkt( &pszPrettyWkt, FALSE );
              itk::EncapsulateMetaData<std::string> ( dico, MetaDataKey::m_ProjectionRefKey, 
            	static_cast<std::string>( pszPrettyWkt ) ); 

              CPLFree( pszPrettyWkt );
        }
        else
            itk::EncapsulateMetaData<std::string>(dico, MetaDataKey::m_ProjectionRefKey, 
            	static_cast<std::string>( m_poDataset->GetProjectionRef() ) );
            	
        if (pSR != NULL)
        {
           delete pSR;
           pSR = NULL;
        }

    }

/* -------------------------------------------------------------------- */    
/* Get the GCP projection coordinates of the image : GCPProjection	*/
/* -------------------------------------------------------------------- */
        
    if( m_poDataset->GetGCPCount() > 0 )
    {
        itk::EncapsulateMetaData<std::string>(dico, MetaDataKey::m_GCPProjectionKey, 
            	static_cast<std::string>( m_poDataset->GetGCPProjection() ) );
            	
        std::string key;

        itk::EncapsulateMetaData<unsigned int>(dico, MetaDataKey::m_GCPCountKey, 
            	static_cast<unsigned int>( m_poDataset->GetGCPCount() ) );
            	
//         double minGCPRow(m_width);
//         double minGCPCol(m_height);
//         double minGCPX;
//         double minGCPY;
            	        
        
        for( int cpt = 0; cpt < m_poDataset->GetGCPCount(); cpt++ )
        {
            const GDAL_GCP	*psGCP;

            psGCP = m_poDataset->GetGCPs() + cpt;

            OTB_GCP	pOtbGCP(psGCP);
                   
            // Origin           
//             if ( ( psGCP->dfGCPLine < minGCPRow ) &&
//                  ( psGCP->dfGCPPixel < minGCPCol ) )
//                { 
//                minGCPRow = psGCP->dfGCPLine;
//                minGCPCol = psGCP->dfGCPPixel;	  
//                minGCPX = psGCP->dfGCPX;
//                minGCPY = psGCP->dfGCPY;
//                }

            // Complete the key with the GCP number : GCP_i
            ::itk::OStringStream lStream;
            lStream << MetaDataKey::m_GCPParametersKey << cpt;
            key = lStream.str(); 
            
            itk::EncapsulateMetaData<OTB_GCP>(dico, key, pOtbGCP); 
            
        }
        
// 	m_Origin[0] = minGCPX;
// 	m_Origin[1] = minGCPY;
        
    }

/* -------------------------------------------------------------------- */    
/*  Get the six coefficients of affine geoTtransform			*/
/* -------------------------------------------------------------------- */

    double adfGeoTransform[6];
    VectorType VadfGeoTransform;
      
     if( m_poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
     {
    	for(int cpt = 0 ; cpt < 6 ; cpt++ ) VadfGeoTransform.push_back(adfGeoTransform[cpt]);
    	
        itk::EncapsulateMetaData<VectorType>(dico, MetaDataKey::m_GeoTransformKey, VadfGeoTransform);

	/// retrieve orgin and spacing from the geo transform
	m_Origin[0]=VadfGeoTransform[0];
	m_Origin[1]=VadfGeoTransform[3];
	m_Spacing[0]=VadfGeoTransform[1];
	m_Spacing[1]=VadfGeoTransform[5];
     }

/* -------------------------------------------------------------------- */
/*      Report metadata.                                                */
/* -------------------------------------------------------------------- */

    papszMetadata = m_poDataset->GetMetadata(NULL);
    if( CSLCount(papszMetadata) > 0 )
    {
        std::string key;

        for( int cpt = 0; papszMetadata[cpt] != NULL; cpt++ )
        {       
            ::itk::OStringStream lStream;
            lStream << MetaDataKey::m_MetadataKey << cpt;
            key = lStream.str(); 
            
            itk::EncapsulateMetaData<std::string>(dico, key, 
            	static_cast<std::string>( papszMetadata[cpt] ) );
        }
    }

/* -------------------------------------------------------------------- */
/*      Report subdatasets.                                             */
/* -------------------------------------------------------------------- */

    papszMetadata = m_poDataset->GetMetadata("SUBDATASETS");
    if( CSLCount(papszMetadata) > 0 )
    {
        std::string key;
        
        for( int cpt = 0; papszMetadata[cpt] != NULL; cpt++ )
        {
            ::itk::OStringStream lStream;
            lStream << MetaDataKey::m_SubMetadataKey << cpt;
            key = lStream.str(); 
            
            itk::EncapsulateMetaData<std::string>(dico, key, 
            	static_cast<std::string>( papszMetadata[cpt] ) );
        }
    }

    
/* -------------------------------------------------------------------- */
/* Report corners							*/
/* -------------------------------------------------------------------- */
   
    double GeoX(0), GeoY(0);
    VectorType VGeo;

    GDALInfoReportCorner( "Upper Left", 0.0, 0.0, GeoX, GeoY  );    
    VGeo.push_back(GeoX);
    VGeo.push_back(GeoY);
    
    itk::EncapsulateMetaData<VectorType>(dico, MetaDataKey::m_UpperLeftCornerKey, VGeo);
        
    VGeo.clear();

    GDALInfoReportCorner( "Upper Right", m_width, 0.0, GeoX, GeoY );
    VGeo.push_back(GeoX);
    VGeo.push_back(GeoY);

    itk::EncapsulateMetaData<VectorType>(dico, MetaDataKey::m_UpperRightCornerKey, VGeo);

    VGeo.clear();
    
    GDALInfoReportCorner( "Lower Left", 0.0, m_height, GeoX, GeoY);
    VGeo.push_back(GeoX);
    VGeo.push_back(GeoY);
    
    itk::EncapsulateMetaData<VectorType>(dico, MetaDataKey::m_LowerLeftCornerKey, VGeo);

    VGeo.clear();
   
    GDALInfoReportCorner( "Lower Right", m_width, m_height, GeoX, GeoY );
    VGeo.push_back(GeoX);
    VGeo.push_back(GeoY);

    itk::EncapsulateMetaData<VectorType>(dico, MetaDataKey::m_LowerRightCornerKey, VGeo);

    VGeo.clear();
 
/* -------------------------------------------------------------------- */
/* Color Table								*/
/* -------------------------------------------------------------------- */ 

   for(int iBand = 0; iBand <m_poDataset->GetRasterCount(); iBand++ )
    {
      GDALColorTableH	hTable;
      GDALRasterBandH	hBand;
      hBand=GDALGetRasterBand(m_poDataset,iBand+1);
      if(GDALGetRasterColorInterpretation(hBand)==GCI_PaletteIndex&&(hTable=GDALGetRasterColorTable(hBand))!=NULL)
	{
	  unsigned int ColorEntryCount=GDALGetColorEntryCount(hTable);    
	  
	  itk::EncapsulateMetaData<std::string>(dico, MetaDataKey::m_ColorTableNameKey, 
					    static_cast<std::string>( GDALGetPaletteInterpretationName( 
												       GDALGetPaletteInterpretation( hTable ) ) ) );    
            				    
            itk::EncapsulateMetaData<unsigned int>(dico, MetaDataKey::m_ColorEntryCountKey, ColorEntryCount);        

            for(int i = 0; i < GDALGetColorEntryCount( hTable ); i++ )
            {
                GDALColorEntry	sEntry;
                VectorType VColorEntry;

                GDALGetColorEntryAsRGB( hTable, i, &sEntry );
                
                VColorEntry.push_back(sEntry.c1);
                VColorEntry.push_back(sEntry.c2);
                VColorEntry.push_back(sEntry.c3);
                VColorEntry.push_back(sEntry.c4);
                
                itk::EncapsulateMetaData<VectorType>(dico, MetaDataKey::m_ColorEntryAsRGBKey, VColorEntry);
                
            }
        } 
      }        
   otbMsgDebugMacro( <<"Driver to read: GDAL");
   otbMsgDebugMacro( <<"         Read  file         : "<< m_FileName);
   otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
   otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
   otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
}

bool GDALImageIO::CanWriteFile( const char* name )
{
        // First check the extension
 	if(  name == NULL )
    	{
      		itkDebugMacro(<<"No filename specified.");
      		return false;
    	}

        //Traitement particulier sur certain format o� l'on pr�f�re utiliser 
        // Format PNG -> lecture avec ITK (pas GDAL)
/*  	itk::PNGImageIO::Pointer lPNGImageIO = itk::PNGImageIO::New();
        lCanWrite = lPNGImageIO->CanWriteFile(name);
        if ( lCanWrite == true)
        {
                return false;
        }
*/
  	// Recuperation du type a partir du nom de fichier
        std::string extGDAL = TypeConversion(name);
  	if (extGDAL=="NOT-FOUND")
  	{
  		return false;
  	}
        
        return true;  	

}

/** TODO : Methode Write non implementee */
void GDALImageIO::Write(const void* buffer)
{
        if( m_FlagWriteImageInformation == true )
        {
        	this->InternalWriteImageInformation();
                m_FlagWriteImageInformation = false;
        }

		std::streamoff step = static_cast<std::streamoff>(this->GetNumberOfComponents());
        const unsigned char *p = static_cast<const unsigned char *>(buffer);
        std::streamoff cpt(0);
        if(p==NULL)
        {
                itkExceptionMacro(<<"GDAL : Bad alloc");
                return;
        }

        unsigned int lNbLignes   = this->GetIORegion().GetSize()[1];
        unsigned int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]

	// Cas particuliers : on controle que si la r�gion � �crire est de la m�me dimension que l'image enti�re,
	// on commence l'offset � 0 (lorsque que l'on est pas en "Streaming")
	if( (lNbLignes == m_Dimensions[1]) && (lNbColonnes == m_Dimensions[0]))
	{
                otbMsgDevMacro(<<"Forcing IORegion offset at 0");
		lPremiereLigne = 0;
		lPremiereColonne = 0;
	}

otbMsgDevMacro( << "GDALImageIO::Write() IORegion Start["<<this->GetIORegion().GetIndex()[0]<<","<<this->GetIORegion().GetIndex()[1]<<"] Size ["<<this->GetIORegion().GetSize()[0]<<","<<this->GetIORegion().GetSize()[1]<<"] on Image size ["<<m_Dimensions[0]<<","<<m_Dimensions[1]<<"]");

        std::streamoff lNbPixels = static_cast<std::streamoff>(lNbColonnes)*static_cast<std::streamoff>(lNbLignes);
        std::streamoff lTailleBuffer = static_cast<std::streamoff>(m_NbOctetPixel)*lNbPixels;
otbMsgDevMacro( <<" TailleBuffer allocated : "<< lTailleBuffer);
        
        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"Memory allocation error");
                return;
        }

        // Mise a jour du step
        step = step * static_cast<std::streamoff>(m_NbOctetPixel);

        CPLErr lCrGdal;
        
 
        for (unsigned int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
	        cpt = static_cast<std::streamoff>(nbComponents)* static_cast<std::streamoff>(m_NbOctetPixel);
     
        	for ( std::streamoff i=0 ; i < lTailleBuffer ; i = i+static_cast<std::streamoff>(m_NbOctetPixel) )
        	{
	                memcpy((void*)(&(value[i])),(const void*)(&(p[cpt])),(size_t)(m_NbOctetPixel));
                        cpt += step;
        	}
        	otbMsgDevMacro( << "NbOctets/pix : " << m_NbOctetPixel << " pxType : " << m_PxType);
        	otbMsgDevMacro( << "PremiereCol : " << lPremiereColonne << " PremiereLig : " << lPremiereLigne);
        	otbMsgDevMacro( << "NbCol : " << lNbColonnes << " NbLig : " << lNbLignes);
                GDALRasterBand *poBand;
                poBand =  m_poBands[nbComponents]; //m_poDataset->GetRasterBand(nbComponents+1);
//        	lCrGdal = poBand->RasterIO(GF_Write,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
        	lCrGdal = m_poBands[nbComponents]->RasterIO(GF_Write,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
        	if (lCrGdal == CE_Failure)
        	{
    			itkExceptionMacro(<< "Error while writing image (GDAL format) " << m_FileName.c_str()<<".");
        	}
        }

        delete [] value;
        value = NULL;
	otbMsgDevMacro( << "GDALImageIO::Write() terminee");

}

/** TODO : Methode WriteImageInformation non implementee */
void GDALImageIO::WriteImageInformation()
{

}
  
void GDALImageIO::InternalWriteImageInformation()
{

        char **papszOptions = NULL;
 	std::string extGDAL;
  	m_NbBands = this->GetNumberOfComponents();

    	if( (m_Dimensions[0]==0) && (m_Dimensions[1]==0))
      	{
      	        itkExceptionMacro(<<"Dimensions are not defined.");
      	}

        if ( this->GetComponentType() == CHAR )
        {
                m_NbOctetPixel = 1;
                m_PxType = GDT_Byte;
        }
        else if ( this->GetComponentType() == UCHAR )
        {
                m_NbOctetPixel = 1;
                m_PxType = GDT_Byte;
        }
        else if ( this->GetComponentType() == USHORT )
        {
                m_NbOctetPixel = 2;
                m_PxType = GDT_UInt16;
        }
        else if ( this->GetComponentType() == SHORT )
        {
                m_NbOctetPixel = 2;
                m_PxType = GDT_Int16;
        }
        else if ( this->GetComponentType() == INT )
        {
                m_NbOctetPixel = 4;
                m_PxType = GDT_Int32;
        }
        else if ( this->GetComponentType() == UINT )
        {
                m_NbOctetPixel = 4;
                m_PxType = GDT_UInt32;
        }
        else if ( this->GetComponentType() == FLOAT )
        {
                m_NbOctetPixel = 4;
                m_PxType = GDT_Float32;
        }
        else if ( this->GetComponentType() == DOUBLE )
        {
                m_NbOctetPixel = 8;
                m_PxType = GDT_Float64;
        }
        else 
        {
                m_NbOctetPixel = 1;
                m_PxType = GDT_Byte;
        }
    
        // Automatically set the Type to Binary for GDAL data
        this->SetFileTypeToBinary();

  	GDALAllRegister();


        extGDAL = TypeConversion(m_FileName);
  	if (extGDAL=="NOT-FOUND")
  	{
   	        itkExceptionMacro(<< "GDAL Writing failed : the image file name '"<<m_FileName.c_str()<<"' is not reconized by GDAL.");
  	}

        m_hDriver = GetGDALDriverManager()->GetDriverByName(extGDAL.c_str());
        if (m_hDriver == NULL)
        {
   	        itkExceptionMacro(<< "GDAL Writing failed : Driver not reconized");
        }
        std::string realFileName = GetGdalWriteImageFileName(extGDAL, m_FileName);
        
	if(m_poDataset != NULL)
	  {
	    delete m_poDataset;
	    m_poDataset = NULL;
	  }
	m_poDataset = m_hDriver->Create( realFileName.c_str(), m_Dimensions[0],m_Dimensions[1],m_NbBands,m_PxType, papszOptions );
        if (m_poDataset == NULL)
        {
   	        itkExceptionMacro(<< "GDAL Writing failed : Impossible to create the image file name '"<<realFileName.c_str()<<"'.");
        }

   	// Get all the Bands
    	m_poBands = new GDALRasterBand* [m_NbBands];
    	
    	if(m_poBands==NULL)
      	{
              	itkExceptionMacro(<<"Erreur d'allocation memoire du 'rasterBands'");
      	}
        for(int i=0 ; i<m_NbBands ; i++)
        {
	        m_poBands[i] = m_poDataset->GetRasterBand(i+1);
        }

	otbMsgDebugMacro( <<"Driver to write: GDAL - "<<extGDAL);
	otbMsgDebugMacro( <<"         Write file         : "<< m_FileName);
	otbMsgDebugMacro( <<"         GDAL file name     : "<< realFileName);
	otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
	otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
	otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
	
	
	// JULIEN: ADDING SUPPORT FOR METADATA WRITING.
	

	/*----------------------------------------------------------------------*/
	/*-------------------------- METADATA ----------------------------------*/
	/*----------------------------------------------------------------------*/
 
	// Now initialize the itk dictionary
	itk::MetaDataDictionary & dico = this->GetMetaDataDictionary();
      
    
	/* -------------------------------------------------------------------- */
	/*  Set Spacing								*/
	/* -------------------------------------------------------------------- */    
      
	char** papszMetadata;
	papszMetadata =  m_poDataset->GetMetadata( NULL );
 	itk::OStringStream oss;
// 	oss.str("");
// 	oss<<m_Spacing[0];
// 	CSLSetNameValue( papszMetadata, "CEOS_LINE_SPACING_METERS",oss.str().c_str());
// 	oss.str("");
// 	oss<<m_Spacing[1];
// 	CSLSetNameValue( papszMetadata, "CEOS_PIXEL_SPACING_METERS",oss.str().c_str());
// 	oss.str("");    
	

/* -------------------------------------------------------------------- */    
/* Set the projection coordinate system of the image : ProjectionRef	*/
/* -------------------------------------------------------------------- */
    
	if(ImageBase::GetProjectionRef(dico)!="")
	  {
	    m_poDataset->SetProjection(ImageBase::GetProjectionRef(dico).c_str());
	  }

/* -------------------------------------------------------------------- */    
/* Set the GCPs	                                                        */
/* -------------------------------------------------------------------- */
        
	unsigned int gcpCount = ImageBase::GetGCPCount(dico);
	
	GDAL_GCP * gdalGcps = new GDAL_GCP[gcpCount+1];

// 	bool gcpHasOrigin = false;

	for(unsigned int gcpIndex = 0; gcpIndex < gcpCount;++gcpIndex)
	  {
	    gdalGcps[gcpIndex].pszId = const_cast<char *>(ImageBase::GetGCPId(dico,gcpIndex).c_str());
	    gdalGcps[gcpIndex].pszInfo = const_cast<char *>(ImageBase::GetGCPInfo(dico,gcpIndex).c_str());
	    gdalGcps[gcpIndex].dfGCPPixel = ImageBase::GetGCPCol(dico,gcpIndex);
	    gdalGcps[gcpIndex].dfGCPLine = ImageBase::GetGCPRow(dico,gcpIndex);
	    gdalGcps[gcpIndex].dfGCPX = ImageBase::GetGCPX(dico,gcpIndex);
	    gdalGcps[gcpIndex].dfGCPY = ImageBase::GetGCPY(dico,gcpIndex);
	    gdalGcps[gcpIndex].dfGCPZ = ImageBase::GetGCPZ(dico,gcpIndex);
	    // gcpHasOrigin = ImageBase::GetGCPCol(dico,gcpIndex)==0 && ImageBase::GetGCPRow(dico,gcpIndex)==0;
	  }
	gdalGcps[gcpCount].pszId =   const_cast<char*>(std::string("Origin").c_str());
	gdalGcps[gcpCount].pszInfo = const_cast<char*>(std::string("Origin gcp added by OTB").c_str());
	gdalGcps[gcpCount].dfGCPPixel = 0;
	gdalGcps[gcpCount].dfGCPLine = 0;
	gdalGcps[gcpCount].dfGCPX = m_Origin[0];
	gdalGcps[gcpCount].dfGCPY = m_Origin[1];
	gdalGcps[gcpCount].dfGCPZ = 0;

	// if(gcpHasOrigin)
// 	  {
	    m_poDataset->SetGCPs(gcpCount,gdalGcps,ImageBase::GetGCPProjection(dico).c_str());
// 	  }
	// else
// 	  {
// 	    otbMsgDebugMacro(<<"GCPs do not contain origin.");
// 	    m_poDataset->SetGCPs(gcpCount+1,gdalGcps,ImageBase::GetGCPProjection(dico).c_str());
// 	  }
	delete [] gdalGcps;

/* -------------------------------------------------------------------- */    
/*  Set the six coefficients of affine geoTtransform			*/
/* -------------------------------------------------------------------- */

  
// 	if(!ImageBase::GetGeoTransform(dico).empty())
// 	  {
	    double * geoTransform = new double[6];
// 	    std::vector<double> transformVector = ImageBase::GetGeoTransform(dico);
	    /// Reporting origin and spacing
	    geoTransform[0]=m_Origin[0];            
	    geoTransform[3]=m_Origin[1];
	    geoTransform[1]=m_Spacing[0];
	    geoTransform[5]=m_Spacing[1];
            
            // FIXME: Here component 1 and 4 should be replaced by the orientation parameters 
            geoTransform[2]=0.;            
            geoTransform[4]=0.;
	    m_poDataset->SetGeoTransform(geoTransform);
	    delete [] geoTransform;
// 	  }

/* -------------------------------------------------------------------- */
/*      Report metadata.                                                */
/* -------------------------------------------------------------------- */

	std::string svalue="";
	std::vector<std::string> keys = dico.GetKeys();
	MetaDataKey key;
	
	for (unsigned int itkey=0; itkey<keys.size(); itkey++)
	  {
	     if(keys[itkey].compare(0,key.m_MetadataKey.length(),key.m_MetadataKey)==0)
	       {
		 itk::ExposeMetaData<std::string>(dico,keys[itkey],svalue);
		 unsigned int equalityPos = svalue.find_first_of('=');
		 std::string tag = svalue.substr(0,equalityPos);
		 std::string value = svalue.substr(equalityPos+1);
		 otbMsgDevMacro(<<"Metadata: "<<tag<<"="<<value);
		 m_poDataset->SetMetadataItem(tag.c_str(),value.c_str(),NULL);
	       }
	  }

	// END 
	
}	

std::string GDALImageIO::TypeConversion(std::string name)
{
	std::string extension;
	std::string extGDAL;
	
	//Recuperer extension du fichier image
	extension = System::GetExtension(name);
        
	if ((extension=="tif")||(extension=="tiff")||(extension=="TIF")||(extension=="TIFF"))
			extGDAL="GTiff";
	else if ((extension=="hdr")||(extension=="HDR"))
			extGDAL="ENVI";
	else if ((extension=="img")||(extension=="IMG"))
	  extGDAL="HFA";
//Pas PNG car BUG !!
//	else if ((extension=="png")||(extension=="PNG"))
//			extGDAL="PNG";
//Pas JPEG car BUG !!
//	else if ((extension=="jpg")||(extension=="JPG")||(extension=="jpeg")||(extension=="JPEG"))
//			extGDAL="JPEG";
	else
			extGDAL="NOT-FOUND"; 
        return(extGDAL);   
}

std::string GDALImageIO::GetGdalWriteImageFileName(std::string & extGDAL, std::string filename)
{
	std::string gdalFileName;

        gdalFileName = filename;
        //Suppression de l'extension HDR
        if( extGDAL=="ENVI")
        {
                gdalFileName = System::GetRootName(filename);
        }
        return(gdalFileName);   
}

bool GDALImageIO::GDALInfoReportCorner( const char * corner_name, double x, double y,
                           double &GeoX, double &GeoY)

{
    const char  *pszProjection;
    double	adfGeoTransform[6];
 
    bool IsTrue;
        
/* -------------------------------------------------------------------- */
/*      Transform the point into georeferenced coordinates.             */
/* -------------------------------------------------------------------- */
    if( m_poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
        pszProjection = m_poDataset->GetProjectionRef();

        GeoX = adfGeoTransform[0] + adfGeoTransform[1] * x
            + adfGeoTransform[2] * y;
        GeoY = adfGeoTransform[3] + adfGeoTransform[4] * x
            + adfGeoTransform[5] * y;
        IsTrue = true;
    }

    else
    {
    	GeoX = x;
    	GeoY = y;
        IsTrue = false;
    }

    return IsTrue;
}
  
} // end namespace otb
