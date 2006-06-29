#include "itkExceptionObject.h"
#include "itkMacro.h"
#include "itkByteSwapper.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"

#include <gdal.h>
#include <gdal_priv.h>
#include <iostream.h>
#include <string.h>
#include <list>
#include <math.h>
//#include <zlib.h>

#include "otbGDALImageIO.h"
//#include "gdal_misc.cpp"
#include "otbMacro.h"
//#include "otbCAIImageIO.h"
#include "itkPNGImageIO.h"

#include "itkMetaDataObject.h"

#include <sys/types.h>
#include <dirent.h>

// ROMAIN : Modification for VC++
#ifdef _MSC_VER 
#define	strcasecmp stricmp // _MICROSOFT_ VC++
#endif


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
  
  m_NbBands = 0;

}

GDALImageIO::~GDALImageIO()
{
//THOMAS
	if( m_hDriver != NULL ) GDALClose( m_hDriver );

        if( m_poBands != NULL ) delete [] m_poBands;
}


bool GDALImageIO::GetGdalImageFileName( const char * filename, std::string & GdalFileName )
{
        DIR *ptr_repertoire;
        struct dirent *cr_readdir;
//        std::string str_debut("DAT");
        std::string str_File("DAT_01.001");
        std::string str_FileName;
        char str_comp[20];
        bool fic_trouve(false);

        // Si c'est un répertoire, on regarde le contenu pour voir si c'est pas du RADARSAT, ERS
        ptr_repertoire = opendir(filename);
        if (ptr_repertoire != NULL)
        {
		while ((cr_readdir=readdir(ptr_repertoire))!=NULL && (fic_trouve==false) )
                {
			str_FileName = std::string(cr_readdir->d_name);
	otbMsgDebugMacro(<<".");
	otbMsgDebugMacro(<<str_FileName.compare(str_File));
 			if(str_FileName.compare(str_File) == 0)
			{
                                 GdalFileName = std::string(filename)+str_File;
	otbMsgDebugMacro(<<"OK");
                                 fic_trouve=true;
			}
			
                        //strncpy(str_comp,cr_readdir->d_name,str_debut.size());
                        //strncpy(str_comp,cr_readdir->d_name,str_debut.size());
			//otbMsgDebugMacro(<<"str_comp : "<<str_comp);
			//str_comp[str_debut.size()]='\0';  
                        //if (strcasecmp(str_comp,str_debut.c_str())==0)
                        //{
                        //         GdalFileName = std::string(filename)+std::string(cr_readdir->d_name);
                        //         fic_trouve=true;
                        //}
                }
	}
        else 
        {
                // Sinon le filename est le nom du fichier a ouvrir
                GdalFileName = std::string(filename);
		fic_trouve=true;
        }
	otbMsgDebugMacro(<<"lFileNameGdal : "<<GdalFileName.c_str());
	otbMsgDebugMacro(<<"fic_trouve : "<<fic_trouve);
	closedir(ptr_repertoire);
	return( fic_trouve );
}

// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file) 
{
  // First check the extension
  m_currentfile = file;
  
  if(  file == NULL )
    {
      itkDebugMacro(<<"No filename specified.");
      return false;
    }
        bool lCanRead(false);

        // Avant toutes choses, on regarde si cette image ne peut pas être lue par CAI
/* 
en commentaire car CAI est mis dasn les Factory avant GDAL
  	otb::CAIImageIO::Pointer lCAIImageIO = otb::CAIImageIO::New();
        lCanRead = lCAIImageIO->CanReadFile(file);
        if ( lCanRead == true)
        {
                //Si oui, alors la priorité est donnée à CAI ; on dit que cette image ne doit pas être lue avec GDAL
                //La priorité est donnée à CAI
                return false;
        }
*/
        //Traitement particulier sur certain format où l'on préfère utiliser 
        // Format PNG -> lecture avec ITK (pas GDAL)
  	itk::PNGImageIO::Pointer lPNGImageIO = itk::PNGImageIO::New();
        lCanRead = lPNGImageIO->CanReadFile(file);
        if ( lCanRead == true)
        {
                return false;
        }

        // Regarde si c'est un répertoire
        std::string lFileNameGdal;
	otbMsgDebugMacro(<<"GDALImageIO::CanReadFile()");
        bool found = GetGdalImageFileName(file,lFileNameGdal);
	if( found == false )
	{
        	return false;
	}
	
	otbMsgDebugMacro(<<"lFileNameGdal : "<<lFileNameGdal.c_str());


  // Init GDAL parameters 
  GDALAllRegister();

  // Open file with GDAL 
  
  m_poDataset = (GDALDataset *)GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly );
  if(m_poDataset==NULL)
    {
        fprintf( stderr,
                 "GDALOpen failed - %d\n%s\n",
                 CPLGetLastErrorNo(), CPLGetLastErrorMsg() );
		 
        GDALDumpOpenDatasets( stderr );
        GDALDestroyDriverManager();
        CPLDumpSharedList( NULL );
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


// Read image with GDAL
void GDALImageIO::Read(void* buffer)
{
        unsigned long step = this->GetNumberOfComponents();
        unsigned char * p = static_cast<unsigned char *>(buffer);
        if(p==NULL)
        {
                itkExceptionMacro(<<"Erreur allocation mémoire");
                return;
        }

        int lNbLignes   = this->GetIORegion().GetSize()[1];
        int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]

otbMsgDebugMacro( <<" GDALImageIO::Read()  ");
otbMsgDebugMacro( <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDebugMacro( <<" Region lue (IORegion)  : "<<this->GetIORegion());
otbMsgDebugMacro( <<" Nb Of Components  : "<<this->GetNumberOfComponents());

        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);

	unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*lNbPixels;
        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"Erreur allocation mémoire");
                return;
        }


        CPLErr lCrGdal;
        unsigned long cpt(0);
	
  	if( GDALDataTypeIsComplex(m_PxType) )
	{
		otbMsgDebugMacro( << " GDALDataTypeIsComplex begin ");
		lCrGdal = m_poBands[0]->RasterIO( GF_Read,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 );

		if (lCrGdal == CE_Failure)
                {
    	        	itkExceptionMacro(<< "Erreur lors de la lecture de l'image (format GDAL) " << m_FileName.c_str()<<".");
                }
		cpt = 0;
                for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                {
                        memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));		       
		        cpt += m_NbOctetPixel;
                }		
	}
	else
	{
        	// Mise a jour du step
        	step = step * (unsigned long)(m_NbOctetPixel);

        	for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        	{
               		otbMsgDebugMacro( << "nbComponents"<<nbComponents);		
			lCrGdal = m_poBands[nbComponents]->RasterIO( GF_Read,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
			if (lCrGdal == CE_Failure)
                	{
    	        		itkExceptionMacro(<< "Erreur lors de la lecture de l'image (format GDAL) " << m_FileName.c_str()<<".");
                	}
                	// Recopie dans le buffer                 
                	cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
                	for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                	{
                        	memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
                        	cpt += step;
                	}
               		otbMsgDebugMacro( << "nbComponents "<<nbComponents<<" ... OK");
          	}
	}
	
        delete [] value;
otbMsgDebugMacro( << "GDALImageIO::Read() terminee");
}

void GDALImageIO::ReadImageInformation()
{
  //std::ifstream file;
  this->InternalReadImageInformation();
}
  
void GDALImageIO::InternalReadImageInformation()
{
  int i;

  if(  m_currentfile == NULL )
    {
        itkExceptionMacro(<<"Le fichier n'est pas précisé.");
    }
  
  // Recupre le nom réel du fichier a ouvrir
  std::string lFileNameGdal;
  bool found = GetGdalImageFileName(m_currentfile,lFileNameGdal);
  if( found == false )
  {
        itkExceptionMacro(<<"The file "<<m_currentfile<<"is not supported by GDAL !!!");
  }

  // Init GDAL parameters 
  GDALAllRegister();
  
  // Get Dataset 
  m_poDataset = (GDALDataset *) GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly );
  if(m_poDataset==NULL)
    {
    itkExceptionMacro(<<"Le dataset du fichier GDAL est null.");
    return;
    }
  
  else
    {
    // Get image dimensions
    m_width = m_poDataset->GetRasterXSize();
    m_height = m_poDataset->GetRasterYSize();
       
    if( (m_width==0) & (m_height==0))
      {
      itkExceptionMacro(<<"La dimension n'est pas définie.");
      }
    else
      {
      // Set image dimensions into IO
      m_Dimensions[0] = m_width;
      m_Dimensions[1] = m_height;
      otbMsgDebugMacro(<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]);
      }

    // Get Number of Bands
    m_NbBands = m_poDataset->GetRasterCount();
    this->SetNumberOfComponents(m_NbBands);
   
    otbMsgDebugMacro(<<"NbBands : "<<m_NbBands);
    otbMsgDebugMacro(<<"Nb of Components : "<<this->GetNumberOfComponents());

    
    // Set the number of dimensions (verify for the dim )
// Modif OTB
/*    if( m_NbBands > 1 )
      {
      this->SetNumberOfDimensions(3);
      m_Dimensions[2] = m_NbBands;
      }
    else
      this->SetNumberOfDimensions(2);*/
      this->SetNumberOfDimensions(2);
// Fin Modif OTB

    otbMsgDebugMacro(<<"Nb of Dimensions : "<<m_NumberOfDimensions);

    // Automatically set the Type to Binary for GDAL data
    this->SetFileTypeToBinary();
    
   // Get all the Bands
    m_poBands = new GDALRasterBand* [m_NbBands];
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
// OTB-FA-00005-CS
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
    int numComp = 1;

// Modif OTB : 
//    this->SetPixelType(SCALAR);
//        this-> SetNumberOfComponents(numComp);

// Modif Patrick: LIRE LES IMAGES COMPLEXES
 	if( GDALDataTypeIsComplex(m_PxType) )
	{
    		otbMsgDebugMacro(<<"SetPixelType(COMPLEX)");
		m_NbOctetPixel = m_NbOctetPixel * 2 ;
          	this->SetNumberOfComponents( 2 );
                this->SetPixelType(COMPLEX);
		if(m_NbBands !=1) itkExceptionMacro(<<"GDALImageIO::InternalReadImageInformation() Can read only one band image ");
	}
	else
	{
    		otbMsgDebugMacro(<<"SetPixelType(NoCOMPLEX)");
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

    otbMsgDebugMacro(<<"m_PxType : "<<m_PxType);
    otbMsgDebugMacro(<<"Component Type : "<<m_ComponentType);
    otbMsgDebugMacro(<<"NbOctetPixel   : "<<m_NbOctetPixel);


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
    
    const char *pszValue;
    
    pszValue = CSLFetchNameValue( papszMetadata, "CEOS_LINE_SPACING_METERS" );
    if ( pszValue != NULL )
       m_Spacing[0] = atof( pszValue );
    
    pszValue = CSLFetchNameValue( papszMetadata, "CEOS_PIXEL_SPACING_METERS" );
    if ( pszValue != NULL )
       m_Spacing[1] = atof( pszValue );
 
    
    
    /* -------------------------------------------------------------------- */
    /*      Report general info.                                            */
    /* -------------------------------------------------------------------- */
    GDALDriverH		hDriver;
    
    hDriver = GDALGetDatasetDriver( m_poDataset );
            
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
            	
        double minGCPRow(m_width);
        double minGCPCol(m_height);
        double minGCPX;
        double minGCPY;
            	        
        
        for( unsigned int cpt = 0; cpt < m_poDataset->GetGCPCount(); cpt++ )
        {
            const GDAL_GCP	*psGCP;

            psGCP = m_poDataset->GetGCPs() + cpt;

            OTB_GCP	pOtbGCP(psGCP);
                   
            // Origin           
            if ( ( psGCP->dfGCPLine < minGCPRow ) &&
                 ( psGCP->dfGCPPixel < minGCPCol ) )
               { 
               minGCPRow = psGCP->dfGCPLine;
               minGCPCol = psGCP->dfGCPPixel;	  
               minGCPX = psGCP->dfGCPX;
               minGCPY = psGCP->dfGCPY;
               }

            // Complete the key with the GCP number : GCP_i
            ::itk::OStringStream lStream;
            lStream << MetaDataKey::m_GCPParametersKey << cpt;
            key = lStream.str(); 
            
            itk::EncapsulateMetaData<OTB_GCP>(dico, key, pOtbGCP); 
            
        }
        
        m_Origin[0] = minGCPX;
        m_Origin[1] = minGCPY;
        
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
    }

/* -------------------------------------------------------------------- */
/*      Report metadata.                                                */
/* -------------------------------------------------------------------- */

    papszMetadata = GDALGetMetadata( m_poDataset, NULL );
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

    papszMetadata = GDALGetMetadata( m_poDataset, "SUBDATASETS" );
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

   for(int iBand = 0; iBand < GDALGetRasterCount( m_poDataset ); iBand++ )
    {
      GDALColorTableH	hTable;
      GDALRasterBandH	hBand;
      
      hBand = GDALGetRasterBand( m_poDataset, iBand+1 );
        
      if( GDALGetRasterColorInterpretation(hBand) == GCI_PaletteIndex 
            && (hTable = GDALGetRasterColorTable( hBand )) != NULL )
        {
            
            unsigned int ColorEntryCount = GDALGetColorEntryCount( hTable );        
            
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

}

/** Romain décide que l'on PEUT créer des images dans les formats GDAL */
bool GDALImageIO::CanWriteFile( const char* name )
{

return false;
#if 0
        // Save image name 
        //m_currentfile=name; 
        m_FileName = std::string(name);
        	
        otbMsgDebugMacro(<<"GDALImageIO::CanWriteFile()");
        std::string extGDAL;
        GDALDriver* hDriver ;
        GDALDataset* dataSet ;
        char c;
        int nXSize=10;
        int nYSize=10;
        GDALDataType eDT = GDT_Float32;
        
        // First check the extension
 	if(  name == NULL )
    	{
      		itkDebugMacro(<<"No filename specified.");
      		return false;
    	}
        bool lCanWrite(false);

        //Traitement particulier sur certain format où l'on préfère utiliser 
        // Format PNG -> lecture avec ITK (pas GDAL)
  	itk::PNGImageIO::Pointer lPNGImageIO = itk::PNGImageIO::New();
        lCanWrite = lPNGImageIO->CanWriteFile(name);
        if ( lCanWrite == true)
        {
                return false;
        }

	std::cout << "nom du fichier : " << name << std::endl;
  	// Recuperation du type a partir du nom de fichier
  	extGDAL = TypeConversion(name);
  	std::cout << "Extension GDAL correspondante : " << extGDAL << std::endl;
  	
  	if (extGDAL=="NOT-FOUND")
  	{
  		return false;
  	}  	

/* THOMAS */
/*    	
  	// Init GDAL parameters 
  	GDALAllRegister();
  
  	// MàJ du driver d'écriture de GDAL
  	hDriver = GetGDALDriverManager()->GetDriverByName(extGDAL.c_str());
  	  	
  	std::cout << "Avant création fichier" << std::endl;
  
  	// Create file with GDAL, parameters are wrong, just useful to test GDALCreate with this file format
  	dataSet = hDriver->Create(name, 10,10,1,eDT,NULL);
  	
        if(dataSet==NULL)
    	{
      		fprintf( stderr,"GDALCreate failed - %d\n%s\n",CPLGetLastErrorNo(), CPLGetLastErrorMsg());
		GDALDumpOpenDatasets( stderr );
        	GDALDestroyDriverManager();
        	CPLDumpSharedList( NULL );
        	itkDebugMacro(<<"Create failed ");
      		return false;
    	}
  	else
    	{
//                GDALClose(hDriver);
      		return true;
    	}
*/
#endif
}

/** TODO : Methode Write non implementee */
void GDALImageIO::Write(const void* buffer)
{
	unsigned long step = this->GetNumberOfComponents();
	//unsigned long step = m_NbBands;
        const unsigned char *p = static_cast<const unsigned char *>(buffer);
        bool entrelaceOutput = false;
        unsigned long cpt(0);
        
        if(p==NULL)
        {
                itkExceptionMacro(<<"Erreur allocation mémoire");
                return;
        }
        int lNbLignes   = this->GetIORegion().GetSize()[1];
        int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]

	otbMsgDebugMacro( << "GDALImageIO::Write()  ");
	otbMsgDebugMacro( <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
	otbMsgDebugMacro( <<" Region écrite (IORegion)  : "<<this->GetIORegion());
	otbMsgDebugMacro( <<" NbBands : "<<this->GetNumberOfComponents());
	
        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*lNbPixels;
        otbMsgDebugMacro( <<" TailleBuffer : "<< lTailleBuffer);
        
        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"Erreur allocation mémoire");
                return;
        }

        // Mise a jour du step
        step = step * (unsigned long)(m_NbOctetPixel);

        CPLErr lCrGdal;
        
              	
        if (!entrelaceOutput)
        {
        	otbMsgDebugMacro( << "Non entrelace : ");
        	for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        	{
	                cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
     
                	for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                	{
	                        memcpy((void*)(&(value[i])),(const void*)(&(p[cpt])),(size_t)(m_NbOctetPixel));
        	                cpt += step;
                	}
                	otbMsgDebugMacro( << "STEP 1 ");
                	otbMsgDebugMacro( << "NbOctets/pix : " << m_NbOctetPixel << " pxType : " << m_PxType);
                	otbMsgDebugMacro( << "PremiereCol : " << lPremiereColonne << " PremiereLig : " << lPremiereLigne);
                	otbMsgDebugMacro( << "NbCol : " << lNbColonnes << " NbLig : " << lNbLignes);
                        
                	lCrGdal = m_poDataset->GetRasterBand(nbComponents+1)->RasterIO(GF_Write,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
//                	lCrGdal = m_poBands[nbComponents]->RasterIO(GF_Write,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
        		otbMsgDebugMacro( << "STEP 2");
        		if (lCrGdal == CE_Failure)
        		{
    	       			itkExceptionMacro(<< "Erreur lors de l'écriture de l'image (format GDAL) " << m_FileName.c_str()<<".");
        		}
                }
        }
        else
        {
                otbMsgDebugMacro( << "entrelace");
                for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                {
	        	memcpy((void*)(&(value[i])),(const void*)(&(p[i])),(size_t)(m_NbOctetPixel));
        	}
               	lCrGdal = m_poDataset->GetRasterBand(1)->RasterIO(GF_Write,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;

//        	lCrGdal = m_poBands[0]->RasterIO(GF_Write,lPremiereColonne,lPremiereLigne,lNbColonnes, lNbLignes, value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
        	if (lCrGdal == CE_Failure)
        	{
    	       		itkExceptionMacro(<< "Erreur lors de l'écriture de l'image (format GDAL) " << m_FileName.c_str()<<".");
        	}
        }                
                
        
        
        delete [] value;
	otbMsgDebugMacro( << "GDALImageIO::Write() terminee");
}

/** TODO : Methode WriteImageInformation non implementee */
void GDALImageIO::WriteImageInformation()
{
	this->InternalWriteImageInformation();
}
  
void GDALImageIO::InternalWriteImageInformation()
{
	int i;
 
 	std::string extGDAL;
//        GDALDriver* hDriver ;
//        GDALDataset* dataSet ;
        char c;
        int nXSize;
        int nYSize;
        GDALDataType eDT ;
 
        
  	// Init GDAL parameters 
  	GDALAllRegister();
  
  	m_NbBands = this->GetNumberOfComponents();
	//m_NbBands = 3;
  	otbMsgDebugMacro(<<"Internal Info :: NbBands : "<<m_NbBands);
  	
      	// Get image dimensions
    	nXSize = this->GetDimensions(0);
    	nYSize = this->GetDimensions(1);
       
    	if( (nXSize==0) && (nYSize==0))
      	{
      	        itkExceptionMacro(<<"La dimension n'est pas définie.");
      	}
/*    	else
      	{
      	// Set image dimensions into IO
      	m_Dimensions[0] = nXSize;
      	m_Dimensions[1] = nYSize;
      	otbMsgDebugMacro(<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]);
      	}
*/
    	otbMsgDebugMacro(<<"Nb of Components : "<<this->GetNumberOfComponents());

      	/*this->SetNumberOfDimensions(2);
    	otbMsgDebugMacro(<<"Nb of Dimensions : "<<m_NumberOfDimensions);*/

    	// Automatically set the Type to Binary for GDAL data
    	this->SetFileTypeToBinary();
    
   	// Get all the Bands
    	m_poBands = new GDALRasterBand* [m_NbBands];
    	
    	if(m_poBands==NULL)
      	{
              	itkExceptionMacro(<<"Erreur d'allocation memoire du 'rasterBands'");
      	}
      	    	
    // Get Data Type
    // Consider only the data type given by the first band!!!!!
    // Maybe be could changed (to check)
    	

    // Following the data type given by GDAL we set it for ImageIO
    // BE CAREFUL !!!! At this time the complex data type are regarded
    // as integer data type in hope that ITK uses that kind of system
    // (take time to check !!)
        if ( this->GetComponentType() == CHAR )
        {
                m_NbOctetPixel = 1;
                eDT = GDT_Byte;
        }
        else if ( this->GetComponentType() == UCHAR )
        {
                m_NbOctetPixel = 1;
                eDT = GDT_Byte;
        }
        else if ( this->GetComponentType() == USHORT )
        {
                m_NbOctetPixel = 2;
                eDT = GDT_UInt16;
        }
        else if ( this->GetComponentType() == SHORT )
        {
                m_NbOctetPixel = 2;
                eDT = GDT_Int16;
        }
        else if ( this->GetComponentType() == INT )
        {
                m_NbOctetPixel = 4;
                eDT = GDT_Int32;
        }
        else if ( this->GetComponentType() == UINT )
        {
                m_NbOctetPixel = 4;
                eDT = GDT_UInt32;
        }
        else if ( this->GetComponentType() == FLOAT )
        {
                m_NbOctetPixel = 4;
                eDT = GDT_Float32;
        }
        else if ( this->GetComponentType() == DOUBLE )
        {
                m_NbOctetPixel = 8;
                eDT = GDT_Float64;
        }
        else 
        {
                m_NbOctetPixel = 1;
                eDT = GDT_Byte;
        }
    
    	m_PxType = eDT ;
    
    otbMsgDebugMacro(<<"Component Type : "<<m_ComponentType);
    otbMsgDebugMacro(<<"NbOctetPixel   : "<<m_NbOctetPixel);
    
    extGDAL = TypeConversion(m_FileName.c_str());
    m_hDriver = GetGDALDriverManager()->GetDriverByName(extGDAL.c_str());
    
//    m_NbBands=3;
//    dataSet = m_hDriver->Create(m_FileName.c_str(), nXSize,nYSize,m_NbBands,eDT,NULL);
    m_poDataset = m_hDriver->Create(m_FileName.c_str(), nXSize,nYSize,m_NbBands,eDT,NULL);

//    if (dataSet == NULL)
     if (m_poDataset == NULL)
   	itkExceptionMacro(<< "GDAL Writing failed");
    	
//    for(i=0 ; i<m_NbBands ; i++)
//	m_poBands[i] = dataSet->GetRasterBand(i+1);
	
otbMsgDebugMacro( << "Driver: GDAL - "<<extGDAL);
otbMsgDebugMacro(<< "         Write files     : "<< m_FileName);
otbMsgDebugMacro( <<"         Size          : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDebugMacro( <<"         ComponentType : "<<this->GetComponentType() );
otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());



}	

std::string GDALImageIO::TypeConversion(const char* name)
{
	std::string extension;
	std::string extGDAL;
	//Recupérer extension du fichier image
	int i=0;
	while (name[i]!='\0')
	{
		if (name[i]=='.')
			extension="";
		else
			extension+=name[i];
		i++;
	}
	
	if ((extension=="tif")||(extension=="tiff")||(extension=="TIF")||(extension=="TIFF"))
			extGDAL="GTiff";
	else if (extension=="hdr")
			extGDAL="ENVI";
	else if (extension=="png")
			extGDAL="PNG";
	else if (extension=="jpg")
			extGDAL="JPEG";
	else
			extGDAL="NOT-FOUND"; 
        return(extGDAL);   
}

bool GDALImageIO::GDALInfoReportCorner( const char * corner_name, double x, double y,
                           double &GeoX, double &GeoY)

{
    const char  *pszProjection;
    double	adfGeoTransform[6];
    OGRCoordinateTransformation *hTransform = NULL;
 
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
