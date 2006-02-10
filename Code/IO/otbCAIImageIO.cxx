#include "otbCAIImageIO.h"

// Pour CAI
#define MAIN
#define _CAI_IMAGE
#define _NOHDF16
extern "C"
{
#include "cai_image.h"
}

#include "itkExceptionObject.h"

#include <iostream>
#include <list>
#include <string>
#include <math.h>

// Pour supprimer les allocations faites dans la methode DetermineFormatSpot
#define FREE(A)        { if ((A)!=NULL) { free(A); (A)=NULL; } }
// Pour spécifique CAI
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>


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
  
  m_ptrCai = NULL;
  m_NbOctetPixel = 0;
}


/** Destructor */
CAIImageIO::~CAIImageIO()
{
  if ( m_ptrCai != NULL)
  {
        CAI_IMAGE * lCai = (CAI_IMAGE *)m_ptrCai;
        cai_ferme_image(lCai);
  }
  m_ptrCai = NULL;
}


bool CAIImageIO::DetermineFormatSpot (char typspot, const char * str_repertoire ,
				char * str_debut  )
{

     DIR *ptr_repertoire;
     struct dirent *cr_readdir;
     long lg_comp;
     char str_comp[20];
     int fic_trouve;
     bool retour(true);
     char *str_fichier(NULL);

     strcpy(CAI_ERREUR,"");
     fic_trouve = 0;
     lg_comp = strlen(str_debut);

/* --------------------------------------------------
  Ouverture du repertoire et listage des fichiers
-------------------------------------------------- */
/**************************************/
/* Traitement suivant le type de spot */
/**************************************/
 if (typspot=='1')
{
/* Format SPOT1a4 */

   ptr_repertoire = opendir(str_repertoire);
   if (ptr_repertoire != NULL)
   {
     while ((cr_readdir=readdir(ptr_repertoire))!=NULL)
     {
       strncpy(str_comp,cr_readdir->d_name,lg_comp);
       str_comp[lg_comp]='\0';  
       if ((strcasecmp(str_comp,str_debut)==0) && (fic_trouve == 0)
							 && (retour == true))
	 {
	     str_fichier = (char *) calloc(strlen(str_repertoire)+1+strlen(cr_readdir->d_name)+1, sizeof(char));
	     if (str_fichier != NULL)
	     {
		strcpy(str_fichier,str_repertoire);
	        strcat(str_fichier,"/");
		strcat(str_fichier,cr_readdir->d_name);
			(str_fichier)[strlen(str_repertoire)+1+strlen(cr_readdir->d_name)]='\0';
		fic_trouve++;
	     }
	     else
	     {
		strcpy(CAI_ERREUR,"Erreur : Probleme d'allocation memoire");
	        retour = false;
	     }
          }
	}
	closedir(ptr_repertoire);

	if (fic_trouve == 0)
	{
	    sprintf(CAI_ERREUR,"Erreur : fichier %s/%s... n'existe pas",
			str_repertoire,str_debut);
	   retour = false;
	}
   }
   // if (ptr_repertoire != NULL)
   else retour = false;

 }
 else if (typspot=='B')
{
/* Format SPOT5 BIL */

   ptr_repertoire = opendir(str_repertoire);
   if (ptr_repertoire != NULL)
   {
     while ((cr_readdir=readdir(ptr_repertoire))!=NULL)
     {
       strncpy(str_comp,cr_readdir->d_name,lg_comp);
       str_comp[lg_comp]='\0';
       if ((strcasecmp(str_comp,str_debut)==0) && (fic_trouve == 0)
							 && (retour == true))
       {
         if (((strncasecmp(str_debut,"IM",2)== 0)&&
		(((strstr(cr_readdir->d_name,".BIL")) != 0)||
		  ((strstr(cr_readdir->d_name,".bil")) != 0)))||
	      (strncasecmp(str_debut,"METADATA.DIM",12)== 0))
         {
	     str_fichier = (char *) calloc(strlen(str_repertoire)+1+strlen(cr_readdir->d_name)+1, sizeof(char));
	     if (str_fichier != NULL)
	     {
		strcpy(str_fichier,str_repertoire);
	        strcat(str_fichier,"/");
		strcat(str_fichier,cr_readdir->d_name);
			(str_fichier)[strlen(str_repertoire)+1+strlen(cr_readdir->d_name)]='\0';
		fic_trouve++;
	     }
	     else
	     {
		strcpy(CAI_ERREUR,"Erreur : Probleme d'allocation memoire");
	        retour = false;
	     }
           }
          }
	}
	closedir(ptr_repertoire);

	if (fic_trouve == 0)
	{
	    sprintf(CAI_ERREUR,"Erreur : fichier %s/%s... n'existe pas",
			str_repertoire,str_debut);
	   retour = false;
	}
   }
   // if (ptr_repertoire != NULL)
   else retour = false;


}
else if (typspot=='T')
{
/* Format SPOT5 TIF */

   ptr_repertoire = opendir(str_repertoire);
   if (ptr_repertoire != NULL)
   {
     while ((cr_readdir=readdir(ptr_repertoire))!=NULL)
     {
       strncpy(str_comp,cr_readdir->d_name,lg_comp);
       str_comp[lg_comp]='\0';
       if ((strcasecmp(str_comp,str_debut)==0) && (fic_trouve == 0)
							 && (retour == true))
       {
         if (((strncasecmp(str_debut,"IM",2)== 0)&&
		(((strstr(cr_readdir->d_name,".TIF")) != 0)||
		  ((strstr(cr_readdir->d_name,".tif")) != 0)))||
	      (strncasecmp(str_debut,"METADATA.DIM",12)== 0))
         {
	     str_fichier = (char *) calloc(strlen(str_repertoire)+1+strlen(cr_readdir->d_name)+1, sizeof(char));
	     if (str_fichier != NULL)
	     {
		strcpy(str_fichier,str_repertoire);
	        strcat(str_fichier,"/");
		strcat(str_fichier,cr_readdir->d_name);
			(str_fichier)[strlen(str_repertoire)+1+strlen(cr_readdir->d_name)]='\0';
		fic_trouve++;
	     }
	     else
	     {
		strcpy(CAI_ERREUR,"Erreur : Probleme d'allocation memoire");
	        retour = false;
	     }
           }
          }
	}
	closedir(ptr_repertoire);

	if (fic_trouve == 0)
	{
	    sprintf(CAI_ERREUR,"Erreur : fichier %s/%s... n'existe pas",
			str_repertoire,str_debut);
	   retour = false;
	}
   }
   // if (ptr_repertoire != NULL)
   else retour = false;

}
else	
	retour=false;

   FREE(str_fichier);
   return(retour);
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
        // Le format a pu etre trouve
        if ( trouve == true )
        {
/*              	char car='.';
                char * pch=(char *)strrchr(image,car);
                if (pch != NULL) { *pch='\0'; } 
                CaiFileName = (std::string)(pch);*/
                CaiFileName = (std::string)(filename);
        }
        // Si le format n'a pas pu etre trouve automatiquement, on va rechercher si ce n'est pas un répertoire contenant :
        // - soit du SPOT 1a4
        // - soit du SPOT 5 BIL
        // - soit du SPOT 5 TIF
        else
        {
                // Regarde si l'image est au format SPOT 1a4 :
		if ( 	        (DetermineFormatSpot('1',filename,"lea") == true) && 
                                (DetermineFormatSpot('1',filename,"tra") == true) &&
                                (DetermineFormatSpot('1',filename,"ima") == true) &&
                                (DetermineFormatSpot('1',filename,"vol") == true) && 
	                        (DetermineFormatSpot('1',filename,"NUL") == true) )
    		{
				/*image SPOT1 4*/
				trouve = true;
                                CaiFormat = (std::string)("SPOT1A4");
                                CaiFileName = (std::string)(filename);
    		}
                else if (	(DetermineFormatSpot('B',filename,"METADATA.DIM") == true) &&
	                        (DetermineFormatSpot('B',filename,"im") == true) )
		{
			        /*image SPOT5 BIL*/
				trouve = true;
                                CaiFormat = (std::string)("SPOT5BIL");
                                CaiFileName = (std::string)(filename);
                }
                else if (	(DetermineFormatSpot('T',filename,"METADATA.DIM") == true) &&
	                        (DetermineFormatSpot('T',filename,"im") == true) )
                {
			        /*image SPOT5 TIF*/
				trouve = true;
                                CaiFormat = (std::string)("SPOT5TIF");
                                CaiFileName = (std::string)(filename);
                }
        }
        
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
        CAI_IMAGE* lCai(NULL);
 
        std::string CaiFileName("");
        std::string CaiFormat("");
//        std::string CaiFileNameOrigin(filename);
        formatFound = GetInfosCAI( filename, CaiFileName, CaiFormat  );
        if( formatFound == false )
        {
                std::string identificationErreur;
                identificationErreur = "Format non CAI : " + (std::string)(CAI_ERREUR);
                itkDebugMacro(<<identificationErreur.c_str());
        }
        else
        {

                lCai = cai_ouvre_lecture_image(	(char *)CaiFileName.c_str(),
		        				(char*)CaiFormat.c_str(),
			        			&NbCanaux,
				        		&NbOctetPixel,
					        	&NbColonnes,
						        &NbLignes);
                if (lCai == NULL)
                {
                        formatFound = false;
                        std::string identificationErreur;
                        identificationErreur = "Format non CAI : " + (std::string)(CAI_ERREUR);
                        itkDebugMacro(<<identificationErreur.c_str());
                }
                else
                {
                        formatFound = true;
                        cai_ferme_image(lCai);
                        lCai = NULL;
                }
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
        int NbOctetPixel(8);            /* Nombre octets/pixel l'image */
        CAI_IMAGE * lCai(NULL);
 
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
                lCai = cai_ouvre_creation_image(	(char *)CaiFileName.c_str(),
		        				(char *)CaiFormat.c_str(), // Detection automatique
			        			NbCanaux,
				        		NbOctetPixel,
					        	NbColonnes,
						        NbLignes,
                                                        "LABEL");
                                                        
                if (lCai == NULL)
                {
                        formatFound = false;
                        std::string identificationErreur;
                        identificationErreur = "Format non CAI : " + (std::string)(CAI_ERREUR);
                        itkDebugMacro(<<identificationErreur.c_str());
                }
                else
                {
                        formatFound = true;
                        cai_ferme_image(lCai);
                        // Destruction physique de l'image
                        cai_destruction_image((char *)CaiFormat.c_str(),(char *)CaiFileName.c_str());
                        lCai = NULL;
                }
        }
        return formatFound;
}


 
void CAIImageIO::Read(void* buffer)
{
        unsigned long step = this->GetNumberOfComponents();
        unsigned char * p = static_cast<unsigned char *>(buffer);
        CAI_OK_KO lCrCai=CAI_KO;
        int lNbLignes   = m_Dimensions[1];
        int lNbColonnes = m_Dimensions[0];
        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*lNbPixels;
        
        unsigned char* value = new unsigned char[lTailleBuffer];
        
        // Mise a jour du step
        step = step * (unsigned long)(m_NbOctetPixel);

        CAI_IMAGE * lCai(NULL);

        //Recupere pointeur sur structure CAI_IMAGE
        lCai = (CAI_IMAGE *)m_ptrCai;

        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
        
                lCrCai = cai_lecture_canal(     lCai,
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
    	        	itkExceptionMacro(<< "Erreur lors de la lecture de l'image (format CAI) " << m_FileName.c_str() <<" ("<<CAI_ERREUR<<") .");
                }
                // Recopie dans le buffer 
                
                unsigned long cpt(0);
                cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
                for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                {
                        memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
                        cpt += step;
                }
          }

        delete [] value;
}

void CAIImageIO::ReadImageInformation()
{
        int NbCanaux;                /* Nombre de canaux de l'image */
        int NbLignes;                /* Nombre de lignes de l'image */
        int NbColonnes;              /* Nombre de colonnes de l'image */
        int NbOctetPixel;            /* Nombre octets/pixel l'image */
        std::string CaiFileName("");
        std::string CaiFormat("");
//        std::string CaiFileNameOrigin(m_FileName);
        bool formatFound = GetInfosCAI( m_FileName.c_str(), CaiFileName, CaiFormat  );
        CAI_IMAGE * lCai(NULL);

        lCai = cai_ouvre_lecture_image(	(char *)CaiFileName.c_str(),
						(char *)CaiFormat.c_str(),
						&NbCanaux,
						&NbOctetPixel,
						&NbColonnes,
						&NbLignes);
        if (lCai == NULL)
        {
    		itkExceptionMacro(<< "Impossible de lire les informations sur l'image " << m_FileName.c_str() <<" ("<<CaiFileName.c_str()<<";"<<CaiFormat.c_str()<<") : ("<<CAI_ERREUR<<").");
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

        if (strncmp(lCai->COD_PIX,"OCT",3) == 0)
        {
                this->SetComponentType( CHAR );
        }
        else if (strncmp(lCai->COD_PIX,"UOCT",4) == 0)
        {
                this->SetComponentType( UCHAR );
        }
        else if (strncmp(lCai->COD_PIX,"I2",2) == 0)
        {
                this->SetComponentType( SHORT );
        }
        else if (strncmp(lCai->COD_PIX,"UI2",3) == 0)
        {
                this->SetComponentType( USHORT );
        }
        else if (strncmp(lCai->COD_PIX,"I4",2) == 0)
        {
                this->SetComponentType( INT );
        }
        else if (strncmp(lCai->COD_PIX,"UI4",3) == 0)
        {
                this->SetComponentType( UINT );
        }
        else if (strncmp(lCai->COD_PIX,"R4",2) == 0)
        {
                this->SetComponentType( FLOAT );
        }
        else if (strncmp(lCai->COD_PIX,"R8",2) == 0)
        {
                this->SetComponentType( DOUBLE );
        }
        else if (strncmp(lCai->COD_PIX,"UND",3) == 0)
        {
                this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
        else
        {
                this->SetComponentType( UNKNOWNCOMPONENTTYPE );
        }
        
        //Stock le pointer CAI
        m_ptrCai = (char*)lCai;
}

void CAIImageIO::WriteImageInformation(void)
{
        int NbCanaux;                /* Nombre de canaux de l'image */
        int NbLignes;                /* Nombre de lignes de l'image */
        int NbColonnes;              /* Nombre de colonnes de l'image */
        int NbOctetPixel;            /* Nombre octets/pixel l'image */
        std::string CaiFileName("");
        std::string CaiFormat("");
        bool formatFound = GetInfosCAI( m_FileName.c_str(), CaiFileName, CaiFormat  );

        // Initialisation de m_Cai->COD_PIX
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
        
        NbColonnes = m_Dimensions[0];
        NbLignes = m_Dimensions[1];
        NbCanaux = this->GetNumberOfComponents();
        NbOctetPixel = m_NbOctetPixel;
        CAI_IMAGE * lCai(NULL);

        lCai = cai_ouvre_creation_image(	(char *)CaiFileName.c_str(),
	        				(char *)CaiFormat.c_str(), // Detection automatique
		        			NbCanaux,
			        		NbOctetPixel,
				        	NbColonnes,
					        NbLignes,
                                                "Image creee par l'OTB");
        if (lCai == NULL)
        {
    		itkExceptionMacro(<< "Impossible d'ecrire les informations sur l'image (cai_ouvre_creation_image) " << m_FileName.c_str() <<" ("<<CaiFileName.c_str()<<";"<<CaiFormat.c_str()<<") : ("<<CAI_ERREUR<<").");
        }
        //Stock le pointer CAI
        m_ptrCai = (char*)lCai;
}

/** The write function is not implemented */
void CAIImageIO::Write( const void* buffer) 
{

        // Création de l'image avant
        this->WriteImageInformation();

        const unsigned char * p = static_cast<const unsigned char *>(buffer);
        unsigned long l=0;
        unsigned long step = this->GetNumberOfComponents();
        CAI_OK_KO lCrCai=CAI_KO;
        int lNbLignes   = m_Dimensions[1];
        int lNbColonnes = m_Dimensions[0];
        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*lNbPixels;
        
        unsigned char* value = new unsigned char[lTailleBuffer];
        CAI_IMAGE * lCai(NULL);

        // Mise a jour du step
        step = step * (unsigned long)(m_NbOctetPixel);
        
        //Recupere pointeur sur structure CAI_IMAGE
        lCai = (CAI_IMAGE *)m_ptrCai;
        
        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
                // Recopie dans le buffer 
                unsigned long cpt(0);
                cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
                for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                {
                        memcpy((void*)(&(value[i])),(const void*)(&(p[cpt])),(size_t)(m_NbOctetPixel));
                        cpt += step;
                }
                lCrCai = cai_ecriture_canal(    lCai,
                                                nbComponents+1,         // Numero canal
                                                1,                      // Premiere ligne [1... ]
                                                lNbLignes,              // Nombre de lignes
                                                lNbColonnes,            // Nombre de colonnes
                                                value );
        
                if (lCrCai != CAI_OK)
                {
    	        	itkExceptionMacro(<< "Erreur lors de l'ecriture dans l'image (format CAI) " << m_FileName.c_str() <<" ("<<CAI_ERREUR<<") .");
                }
                
          }

        delete [] value;
}

/** Print Self Method */
void CAIImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{


}



} // end namespace otb
