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
#include <zlib.h>

#include "otbGDALImageIO.h"
//#include "otbCAIImageIO.h"
#include "itkPNGImageIO.h"

#include <sys/types.h>
#include <dirent.h>

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


void GDALImageIO::GetGdalImageFileName( const char * filename, std::string & GdalFileName )
{
        DIR *ptr_repertoire;
        struct dirent *cr_readdir;
        std::string str_debut("DAT");
        char str_comp[20];
        bool fic_trouve(false);

        // Si c'est un répertoire, on regarde le contenu pour voir si c'est pas du RADARSAT, ERS
        ptr_repertoire = opendir(filename);
        if (ptr_repertoire != NULL)
        {
                while ((cr_readdir=readdir(ptr_repertoire))!=NULL && (fic_trouve==false) )
                {
                        strncpy(str_comp,cr_readdir->d_name,str_debut.size());
                        str_comp[str_debut.size()]='\0';  
                        if (strcasecmp(str_comp,str_debut.c_str())==0)
                        {
                                 GdalFileName = std::string(filename)+std::string(cr_readdir->d_name);
                                 fic_trouve=true;
                        }
                }
	}
        else 
        {
                // Sinon le filename est le nom du fichier a ouvrir
                GdalFileName = std::string(filename);
        }
	closedir(ptr_repertoire);
}

// Tell only if the file can be read with GDAL.
bool GDALImageIO::CanReadFile(const char* file) 
{
  // First check the extension
  m_currentfile = file;
  std::cout<<"Filename : "<<m_currentfile<<endl;
  
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
        GetGdalImageFileName(file,lFileNameGdal);


  // Init GDAL parameters 
  GDALAllRegister();
  
  // Open file with GDAL 
  m_poDataset = (GDALDataset *) GDALOpen(lFileNameGdal.c_str(), GA_ReadOnly );
  if(m_poDataset==NULL)
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
  // pafimas = new float*[m_NbBands];
  //p= new unsigned short*;
  
  if(p==NULL)
   {
   std::cout<<"Error in allocating memory"<<endl;
   return;
   }

  if((m_poBands[0]->RasterIO( GF_Read,XBegin, YBegin,SizeXRead,SizeYRead , p , XSample, YSample, GDT_Byte,0, 0 ))==CE_Failure)
    std::cout<<"[GDALImageIO::Read] Failed in reading image "<<std::endl;
}*/

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
        int lNbLignes   = m_Dimensions[1];
        int lNbColonnes = m_Dimensions[0];
        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*lNbPixels;
        
        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"Erreur allocation mémoire");
                return;
        }

        // Mise a jour du step
        step = step * (unsigned long)(m_NbOctetPixel);

        CPLErr lCrGdal;
        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
                lCrGdal = m_poBands[nbComponents]->RasterIO( GF_Read,0,0,m_Dimensions[0],m_Dimensions[1] , value , lNbColonnes, lNbLignes, m_PxType,0, 0 ) ;
                if (lCrGdal == CE_Failure)
                {
    	        	itkExceptionMacro(<< "Erreur lors de la lecture de l'image (format GDAL) " << m_FileName.c_str()<<".");
                }
                // Recopie dans le buffer 
                
                unsigned long cpt(0);
/*                cpt = (unsigned long )(nbComponents);
                for ( unsigned long  i=0 ; i < lNbPixels ; i++ )
                {
                        p[cpt] = value[i];
                        cpt += step;
                }*/
                cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
                for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                {
                        memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
                        cpt += step;
                }


          }

        delete [] value;

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
        GetGdalImageFileName(m_currentfile,lFileNameGdal);

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
       
    if( m_width==0 & m_height==0)
      {
      itkExceptionMacro(<<"La dimension n'est pas définie.");
      }
    else
      {
      // Set image dimensions into IO
      m_Dimensions[0] = m_width;
      m_Dimensions[1] = m_height;
      std::cout<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]<<endl;
      }

    // Set Number of components
    // Get Number of Bands
    m_NbBands = m_poDataset->GetRasterCount();
//    m_NumberOfComponents = m_NbBands;
    this->SetNumberOfComponents(m_NbBands);
    std::cout<<"NbBands : "<<m_NbBands<<endl;

    std::cout<<"Nb of Components : "<<this->GetNumberOfComponents()<<endl;

    
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

    std::cout<<"Nb of Dimensions : "<<m_NumberOfDimensions<<endl;

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
      SetComponentType(CHAR);
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
    
    std::cout<<"Component Type : "<<m_ComponentType<<std::endl;
    std::cout<<"NbOctetPixel   : "<<m_NbOctetPixel<<std::endl;


    // See to take the spacing with GDAL
    // Find a way to get the good indice for papszMetadata for all the
    // dataset (the one set is working for dat_01.001!!)
    /**********************************/ 
    /*char** papszMetadata = GDALGetMetadata(m_poDataset,NULL);
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
    papszMetadata = GDALGetMetadata( m_poDataset, NULL );
    
    GDALDriverH hDriver = GDALGetDatasetDriver( m_poDataset );
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
       if(spot==true)
	 {
	 if(m_NbBands == 3)
	   {
	   m_Spacing[0] = 20;
	   m_Spacing[1] = 20;
	   }
	 else if(m_NbBands == 4)
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
	 else if(m_NbBands == 1)
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

// Modif OTB : 
//    this->SetPixelType(SCALAR);
//        this-> SetNumberOfComponents(numComp);
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

}

/** On décide que l'on n epeut pas créer des images dans les formats GDAL */
bool GDALImageIO::CanWriteFile( const char* name )
{
        return false;
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

