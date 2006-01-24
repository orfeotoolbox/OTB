#include "otbCAIImageIO.h"
#include "itkExceptionObject.h"

#include <iostream>
#include <list>
#include <string>
#include <math.h>

/*
#define CAI_NB_EXTENSIONS
extern char CAI_ERREUR[];
extern char *CAI_SUFFIXES[];
extern char *CAI_NOM_FORMAT[];
extern long int CAI_NUMEROS[];
*/

namespace otb
{

/** Constructor */
CAIImageIO::CAIImageIO()
{
  m_ByteOrder = BigEndian;
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;

  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
  
  m_Cai = NULL;
  m_NbOctetPixel = 0;
}


/** Destructor */
CAIImageIO::~CAIImageIO()
{
  if ( m_Cai != NULL) cai_ferme_image(m_Cai);
  m_Cai = NULL;
}

bool CAIImageIO::GetInfosCAI( const char * filename, std::string & CaiFileName, std::string & CaiFormat  )
{
        CaiFormat = "";
        CaiFileName = "";
        int ind(0);
        bool trouve(false);
        char * image=(char*)(filename);
        
        for( int i = 0 ; i< CAI_NB_EXTENSIONS ; i++)
        {
                if(strstr(image,CAI_SUFFIXES[i]) != NULL )
                {
                        ind=CAI_NUMEROS[i];
                        trouve = true;
                        CaiFormat = (std::string)(CAI_NOM_FORMAT[ind]);
                }
        }
      	char car='.';
        char * pch=(char *)strrchr(image,car);
        if (pch != NULL) { *pch='\0'; } 
        CaiFileName = (std::string)(pch);
        
        itkDebugMacro(<< "Format CAI detecte    : "<< CaiFormat.c_str()<<".");
        itkDebugMacro(<< "Fichier CAI determine : "<< CaiFileName.c_str()<<".");
        
        return trouve;
}


bool CAIImageIO::CanReadFile( const char* filename ) 
{ 
        // First check the filename extension
        std::string fname = filename;
        if ( fname == "" )
        {
                itkDebugMacro(<< "Fichier image non specifie.");
        }

        bool formatFound = false;
        int NbCanaux;                /* Nombre de canaux de l'image */
        int NbLignes;                /* Nombre de lignes de l'image */
        int NbColonnes;              /* Nombre de colonnes de l'image */
        int NbOctetPixel;            /* Nombre octets/pixel l'image */
 
        m_Cai = cai_ouvre_lecture_image(	(char *)filename,
						"AUTO", // Detection automatique
						&NbCanaux,
						&NbOctetPixel,
						&NbColonnes,
						&NbLignes);
        if (m_Cai == NULL)
        {
                formatFound = false;
                std::string identificationErreur;
                identificationErreur = "Format non CAI : " + (std::string)(CAI_ERREUR);
                itkDebugMacro(<<identificationErreur.c_str());
        }
        else
        {
                formatFound = true;
                cai_ferme_image(m_Cai);
                m_Cai = NULL;
        }
        return formatFound;
}
  



bool CAIImageIO::CanWriteFile( const char * filename )
{
        bool formatFound = false;
        std::string fname = filename;
        int NbCanaux(1);                /* Nombre de canaux de l'image */
        int NbLignes(10);                /* Nombre de lignes de l'image */
        int NbColonnes(10);              /* Nombre de colonnes de l'image */
        int NbOctetPixel(1);            /* Nombre octets/pixel l'image */
 
        if ( fname == "" )
        {
                itkDebugMacro(<< "Fichier image non specifie.");
        }
        std::string CaiFileName("");
        std::string CaiFormat("");
        formatFound = GetInfosCAI( filename, CaiFileName, CaiFormat  );
        if( formatFound == false )
        {
                std::string identificationErreur;
                identificationErreur = "Format CAI non detecte dans l'extension du fichier.";
                itkDebugMacro(<<identificationErreur.c_str());
        }
        else
        {
                m_Cai = cai_ouvre_creation_image(	(char *)CaiFileName.c_str(),
		        				(char *)CaiFormat.c_str(), // Detection automatique
			        			NbCanaux,
				        		NbOctetPixel,
					        	NbColonnes,
						        NbLignes,
                                                        "LABEL");
                                                        
                if (m_Cai == NULL)
                {
                        formatFound = false;
                        std::string identificationErreur;
                        identificationErreur = "Format non CAI : " + (std::string)(CAI_ERREUR);
                        itkDebugMacro(<<identificationErreur.c_str());
                }
                else
                {
                        formatFound = true;
                        cai_ferme_image(m_Cai);
                        m_Cai = NULL;
                }
        }
        return formatFound;
}


 
void CAIImageIO::Read(void* buffer)
{
        unsigned long l=0;
        unsigned long step = this->GetNumberOfComponents();
        unsigned char * p = static_cast<unsigned char *>(buffer);
        CAI_OK_KO lCrCai=CAI_KO;
        int lNbLignes   = m_Dimensions[1];
        int lNbColonnes = m_Dimensions[0];
        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        
        unsigned char* value = new unsigned char[m_NbOctetPixel*lNbPixels];

        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
        
                lCrCai = cai_lecture_canal(     m_Cai,
                                                nbComponents+1,         // Numero canal
                                                1,                      // Premiere ligne [1... ]
                                                1,                      // Premiere colonne [1...]
                                                lNbLignes,              // Nombre de lignes
                                                lNbColonnes,            // Nombre de colonnes
                                                1,                      // Pas en X
                                                1,                      // Pas en Y
                                                value );
        
                if (lCrCai != CAI_OK)
                {
    	        	itk::ExceptionObject ex(__FILE__, __LINE__);
    		        itk::OStringStream Message;
            		Message << "Erreur lors de la lecture de l'image (format CAI) " << m_FileName.c_str() <<" ("<<CAI_ERREUR<<") .";
    	        	ex.SetDescription(Message.str().c_str());
    		        throw ex;
                }
                // Recopie dans le buffer 
                
                unsigned long cpt(0);
                cpt = (unsigned long )(nbComponents);
                for ( unsigned long  i=0 ; i < lNbPixels ; i++ )
                {
                        p[cpt] = value[i];
                        cpt += step;
                }
          }

        delete [] value;
}

/** 
 *  Read Information about the BMP file
 *  and put the cursor of the stream just before the first data pixel
 */
void CAIImageIO::ReadImageInformation()
{
        int NbCanaux;                /* Nombre de canaux de l'image */
        int NbLignes;                /* Nombre de lignes de l'image */
        int NbColonnes;              /* Nombre de colonnes de l'image */
        int NbOctetPixel;            /* Nombre octets/pixel l'image */
        m_Cai = cai_ouvre_lecture_image(	(char *)m_FileName.c_str(),
						"AUTO", // Detection automatique
						&NbCanaux,
						&NbOctetPixel,
						&NbColonnes,
						&NbLignes);
        if (m_Cai == NULL)
        {
    		itk::ExceptionObject ex(__FILE__, __LINE__);
    		itk::OStringStream Message;
    		Message << "Impossible de lire les informations sur l'image " << m_FileName.c_str() <<" ("<<CAI_ERREUR<<") .";
    		ex.SetDescription(Message.str().c_str());
    		throw ex;
        }
        
        this->SetNumberOfDimensions(2);
        m_Dimensions[0] = NbColonnes;
        m_Dimensions[1] = NbLignes;
        this->SetNumberOfComponents(NbCanaux);
        m_NbOctetPixel = NbOctetPixel;
        
        if( this->GetNumberOfComponents() == 1 )
        {
                m_PixelType = SCALAR;
        }
        else
        {
                m_PixelType = VECTOR;
        }

        if (strncmp(m_Cai->COD_PIX,"OCT",3) == 0)
        {
                this->SetComponentType( CHAR );
        }
        else if (strncmp(m_Cai->COD_PIX,"UOCT",4) == 0)
        {
                this->SetComponentType( UCHAR );
        }
        else if (strncmp(m_Cai->COD_PIX,"I2",2) == 0)
        {
                this->SetComponentType( SHORT );
        }
        else if (strncmp(m_Cai->COD_PIX,"UI2",3) == 0)
        {
                this->SetComponentType( USHORT );
        }
        else if (strncmp(m_Cai->COD_PIX,"I4",2) == 0)
        {
                this->SetComponentType( INT );
        }
        else if (strncmp(m_Cai->COD_PIX,"UI4",3) == 0)
        {
                this->SetComponentType( UINT );
        }
        else if (strncmp(m_Cai->COD_PIX,"R4",2) == 0)
        {
                this->SetComponentType( FLOAT );
        }
        else if (strncmp(m_Cai->COD_PIX,"R8",2) == 0)
        {
                this->SetComponentType( DOUBLE );
        }
        else if (strncmp(m_Cai->COD_PIX,"UND",3) == 0)
        {
                this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
        else
        {
                this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
       
}


/** TODO : Methode WriteImageInformation non implementee */
void CAIImageIO::WriteImageInformation(void)
{

}


/** The write function is not implemented */
void CAIImageIO::Write( const void* buffer) 
{
        const unsigned char * p = static_cast<const unsigned char *>(buffer);
        unsigned long l=0;
        unsigned long step = this->GetNumberOfComponents();
        CAI_OK_KO lCrCai=CAI_KO;
        int lNbLignes   = m_Dimensions[1];
        int lNbColonnes = m_Dimensions[0];
        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        
        unsigned char* value = new unsigned char[m_NbOctetPixel*lNbPixels];

        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
                // Recopie dans le buffer 
                unsigned long cpt(0);
                cpt = (unsigned long )(nbComponents);
                for ( unsigned long  i=0 ; i < lNbPixels ; i++ )
                {
                        value[i] = p[cpt];
                        cpt += step;
                }
        
                lCrCai = cai_ecriture_canal(    m_Cai,
                                                nbComponents+1,         // Numero canal
                                                1,                      // Premiere ligne [1... ]
                                                lNbLignes,              // Nombre de lignes
                                                lNbColonnes,            // Nombre de colonnes
                                                value );
        
                if (lCrCai != CAI_OK)
                {
    	        	itk::ExceptionObject ex(__FILE__, __LINE__);
    		        itk::OStringStream Message;
    		        Message << "Erreur lors de l'ecriture dans l'image (format CAI) " << m_FileName.c_str() <<" ("<<CAI_ERREUR<<") .";
    	        	ex.SetDescription(Message.str().c_str());
    		        throw ex;
                }
                
          }

        delete [] value;
}

/** Print Self Method */
void CAIImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{


}



} // end namespace otb
