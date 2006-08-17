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

#include "otbLUMImageIO.h"
#include "itkExceptionObject.h"
#include "otbMacro.h"
#include "otbSystem.h"

#include "cai_image.h"

#include <fstream>

// CAI_LUM header file
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <fcntl.h>
# include <malloc.h>
# include <math.h>
# include <unistd.h>


namespace otb
{

LUMImageIO::LUMImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);

  // By default set pixel type to scalar.
  m_PixelType = SCALAR;

  // By default set component type to unsigned char
  m_ComponentType = UCHAR;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;

  m_FlagWriteImageInformation = true;

}

LUMImageIO::~LUMImageIO()
{

}


bool LUMImageIO::CanReadFile( const char* FileNameToRead )
{
  std::fstream Datafile;
  std::string filename(FileNameToRead);
  
  
  otbMsgDevMacro(<<"LUMImageIO::CanReadFile() IN ! " );

  if( Datafile.is_open() )
    {
    Datafile.close();
    }

  const std::string DataFileName = System::GetRootName(filename)+".lum";

  Datafile.open( DataFileName.c_str(),  std::ios::in );
  if( Datafile.fail() )
    {
    otbMsgDevMacro(<<"LUMImageIO::CanReadFile() failed data open ! " );
    return false;
    }

  if(  FileNameToRead == NULL )
    {
      itkDebugMacro(<<"No filename specified.");
      return false;
    }
	
  return true;

}




// Used to print information about this object
void LUMImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void LUMImageIO::ReadVolume(void*)
{
}


// Read image 
void LUMImageIO::Read(void* buffer)
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

        otbMsgDevMacro( <<" LUMImageIO::Read()  ");
        otbMsgDevMacro( <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
        otbMsgDevMacro( <<" Region lue (IORegion)  : "<<this->GetIORegion());
        otbMsgDevMacro( <<" Nb Of Components  : "<<this->GetNumberOfComponents());

        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
	unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*lNbPixels;
        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"Erreur allocation mémoire");
                return;
        }

       // Update the step variable
        step = step * (unsigned long)(m_NbOctetPixel);

        CAI_OK_KO       resultatLecture;
        unsigned long   cpt(0);
	int pasLig =1;
	int pasCol =1;
	
	
        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
		int noCanal = nbComponents + 1 ;
		int FirstRow = lPremiereLigne +1;
		int FirstCol = lPremiereColonne +1;
		   
		resultatLecture = cai_lecture_canal_lum(m_image, &noCanal, 
							&FirstRow, &FirstCol,
							&lNbLignes, &lNbColonnes,&pasLig, &pasCol, value);
					
		if (resultatLecture == CAI_KO)
                {
    	        	itkExceptionMacro(<< "Error reading image (LUM format)");
                }

                // copying buffer                 

                cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
                for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
                {
                        memcpy((void*)(&(p[cpt])),(const void*)(&(value[i])),(size_t)(m_NbOctetPixel));
                        cpt += step;
                }
        }

        delete [] value;
otbMsgDevMacro( << "LUMImageIO::Read() terminee");
    
}


void LUMImageIO::ReadImageInformation()
{

  if(  m_FileName.empty() == true )
  {
        itkExceptionMacro(<<"LUM read : empty image file name file.");
  }

  std::string shortFileName = System::GetShortFileName(m_FileName);
  std::string pathFileName  = System::GetPathName(m_FileName);

  // shortFileName pass to the cai_ouvre_lecture_lum method is
  // the file name without file extension
  shortFileName = System::GetRootName(shortFileName);
  
  // Get Dataset 
  char repert[pathFileName.length()];
  sprintf(repert, "%s" ,pathFileName.c_str() );  
  char image[shortFileName.length()];
  sprintf(image, "%s" ,shortFileName.c_str() );  

  
  m_image = cai_ouvre_lecture_lum(repert,image, &m_NbBands, 
  				  &m_NbOctetPixel,&m_width,&m_height); 
				  
  m_image->NB_COL = m_width;
  m_image->NB_LIG = m_height;
   
  if( m_image == NULL )
  {
      itkExceptionMacro(<<"The LUM dataset file is NULL.");
      return;
  }
  else
  {
  
     // Get image dimensions
     if( (m_width==0) || (m_height==0))
      {
      itkExceptionMacro(<<"undefined dimension.");
      }
      else
      {
      // Set image dimensions into IO
      m_Dimensions[0] = m_width;
      m_Dimensions[1] = m_height;
      otbMsgDevMacro(<<"Get Dimensions : x="<<m_Dimensions[0]<<" & y="<<m_Dimensions[1]);
      }

    // Get Number of Bands
    this->SetNumberOfComponents(m_NbBands);
    otbMsgDevMacro(<<"NbBands : "<<m_NbBands);
    otbMsgDevMacro(<<"Nb of Components : "<<this->GetNumberOfComponents());

    // Set the number of dimensions 
    this->SetNumberOfDimensions(2);
    otbMsgDevMacro(<<"Nb of Dimensions : "<<m_NumberOfDimensions);

   // Automatically set the Type to Binary for LUM data
    this->SetFileTypeToBinary();

    std::string codePixel(m_image->COD_PIX);
    bool codeFound = false;
    	
    if(codePixel == "OCT")
      {
      this->SetComponentType(CHAR);
      codeFound = true;
      }      
    if(codePixel == "UOCT") 
      {
      this->SetComponentType(UCHAR);
      codeFound = true;
      }
    if(codePixel == "I2") 
      {
      this->SetComponentType(SHORT);
      codeFound = true;
      }
    if(codePixel == "UI2") 
      {
      this->SetComponentType(USHORT);
      codeFound = true;
      }
    if(codePixel == "I4") 
      {
      this->SetComponentType(INT);
      codeFound = true;
      }
    if(codePixel == "UI4") 
      {
      this->SetComponentType(UINT);
      codeFound = true;
      }
    if( (codePixel == "R4") ) 
      {
      this->SetComponentType(FLOAT);
      codeFound = true;
      }
    if(codePixel == "R8") 
      {
      this->SetComponentType(DOUBLE);
      codeFound = true;
      }
    
    if( codeFound == false )
      {
      itkExceptionMacro(<<"unknown pixel code");
      }
     	

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




bool LUMImageIO::CanWriteFile( const char* FileNameToWrite )
{
  std::string filename(FileNameToWrite);

  const std::string DataFileName = System::GetRootName(filename)+".lum";

  if( filename == DataFileName )
  {
        return true;
  }
  else
  {
        return false;
  }
}

void LUMImageIO::Write(const void* buffer)
{
   
  if( m_FlagWriteImageInformation == true )
  {
    	this->InternalWriteImageInformation();
  	m_FlagWriteImageInformation = false;
  }

	unsigned long step = this->GetNumberOfComponents();
        const unsigned char *p = static_cast<const unsigned char *>(buffer);
        unsigned long cpt(0);
        
        if(p==NULL)
        {
                itkExceptionMacro(<<"LUM : Bad alloc");
                return;
        }
	  
        int lNbLignes   = this->GetIORegion().GetSize()[1];
        int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1]; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0]; // [1... ]
		
	// Cas particuliers : on controle que si la région à écrire est de la même dimension que l'image entière,
	// on commence l'offset à 0 (lorsque que l'on est pas en "Streaming")
	if( (lNbLignes == m_Dimensions[1]) && (lNbColonnes == m_Dimensions[0]))
	{
                otbMsgDevMacro(<<"Force l'offset de l'IORegion à 0");
		lPremiereLigne = 0;
		lPremiereColonne = 0;
	}

otbMsgDevMacro( << "LUMImageIO::Write() IORegion Start["<<this->GetIORegion().GetIndex()[0]<<","<<this->GetIORegion().GetIndex()[1]<<"] Size ["<<this->GetIORegion().GetSize()[0]<<","<<this->GetIORegion().GetSize()[1]<<"] on Image size ["<<m_Dimensions[0]<<","<<m_Dimensions[1]<<"]");

        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*(lNbPixels);
otbMsgDevMacro( <<" TailleBuffer alloué : "<< lTailleBuffer);

        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"LUMImageIO::Write() Error in memory allocation");
                return;
        }

        // update the step variable 
        step = step * (unsigned long)(m_NbOctetPixel);
        CAI_OK_KO       resultatEcriture;
	         
        for ( int nbComponents = 0 ; nbComponents < this->GetNumberOfComponents() ; nbComponents++)
        {
	        cpt = (unsigned long )(nbComponents)* (unsigned long)(m_NbOctetPixel);
     
        	for ( unsigned long  i=0 ; i < lTailleBuffer ; i = i+m_NbOctetPixel )
        	{
	                memcpy((void*)(&(value[i])),(const void*)(&(p[cpt])),(size_t)(m_NbOctetPixel));
                        cpt += step;
        	}
        	otbMsgDevMacro( << "NbOctets/pix : " << m_NbOctetPixel << " pxType : " << m_PxType);
        	otbMsgDevMacro( << "PremiereCol : " << lPremiereColonne << " PremiereLig : " << lPremiereLigne);
        	otbMsgDevMacro( << "NbCol : " << lNbColonnes << " NbLig : " << lNbLignes);

		int noCanal = nbComponents + 1 ;
		int FirstRow = lPremiereLigne +1;
		resultatEcriture = cai_ecriture_canal_lum(m_image, &noCanal, 
							&FirstRow,&lNbLignes,
							&lNbColonnes, value);
					
		if (resultatEcriture == CAI_KO)
                {
    	        	itkExceptionMacro(<< "Error writing image (LUM format)" << m_FileName.c_str()<<".");
                }
        }


        delete [] value;
	otbMsgDevMacro( << "LUMImageIO::Write() terminee");
}

void LUMImageIO::WriteImageInformation()
{

}
 
void LUMImageIO::InternalWriteImageInformation()
{
  if(  m_FileName.empty() == true )
  {
        itkExceptionMacro(<<"LUM write : empty image file name file.");
  }

  std::string shortFileName = System::GetShortFileName(m_FileName);
  std::string pathFileName  = System::GetPathName(m_FileName);

  // shortFileName pass to the cai_ouvre_lecture_lum method is
  // the file name without file extension
  shortFileName = System::GetRootName(shortFileName);
  
  // Get Dataset 
  char repert[pathFileName.length()];
  sprintf(repert, "%s" ,pathFileName.c_str() );  
  char image[shortFileName.length()];
  sprintf(image, "%s" ,shortFileName.c_str() );  
 
  m_NbBands = this->GetNumberOfComponents();
  m_width   = m_Dimensions[0];
  m_height  = m_Dimensions[1];
  
 if( (m_Dimensions[0]==0) && (m_Dimensions[1]==0))
 {
    itkExceptionMacro(<<"Dimensions are not defined.");
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
    
        // Automatically set the Type to Binary for GDAL data
        this->SetFileTypeToBinary();
    
  
  m_image = cai_ouvre_creation_lum( repert,image, &m_NbBands, 
  				    &m_NbOctetPixel,&m_width,&m_height,image); 
				  
  m_image->NB_COL     = m_width;
  m_image->NB_LIG     = m_height;
  m_image->NBOCT_PIX  = m_NbOctetPixel; 
  

  if( m_image == NULL )
  {
      itkExceptionMacro(<<"The LUM dataset file is NULL.");
      return;
  }
  
}






/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_ima_lum                                          */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format lum                    */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_lum(repert,tab_ima)             */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_lum   (S) (int)     :     = -1 repertoire inexistant    */                                    
/*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
long int LUMImageIO::cai_liste_ima_lum (char *repert,char ***tab_ima)


{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images lum identifiees             */
   long int i;          /* indice de boucle                            */
   long int ll,dif;     /* variables de travail                        */
   char blanc;          /* caractere blanc                             */


/*........................................
   INITIALISATION et ouverture du repertoire choisi
  .........................................*/
   nb_ima=0;

   dirp=opendir(repert);

   if(dirp!=NULL)
   {
/*......................................

   premiere passe pour comptabiliser les images du format donne


   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE LUM
      (EXTENSION =.lum)
     ..........................................*/

      while((dp=readdir(dirp))!=NULL)
      {
    	 pch0=strstr(dp->d_name,".lum");
     	if (pch0 != NULL)
     	{
        	ll = strlen(dp->d_name);
        	dif = (long)(dp->d_name-pch0);
        	dif=labs(dif);
        	pch1=pch0+4;
        	if (((ll-4)==dif)||((*pch1)==blanc))
	     		nb_ima=nb_ima+1;
     
    	 }

      }

    closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT LUM
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE LUM
       (EXTENSION =.lum)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
 
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".lum");
     if (pch0 != NULL)
     {
        ll = strlen(dp->d_name);
        dif = (long)(dp->d_name-pch0);
        dif=labs(dif);
        pch1=pch0+4;
        if (((ll-4)==dif)||((*pch1)==blanc))
        {
            *pch0='\0';
            strcpy((*tab_ima)[nb_ima],dp->d_name);
	    nb_ima=nb_ima+1;
        }
     
     }
   }   

    closedir(dirp);

   }
   else
   {
     /* Erreur : Repertoire inexistant */
      nb_ima=-1;
   }

    return(nb_ima);

}
/* Fin de l'operation cai_liste_ima_lum*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_lum                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format lum       */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_lum(nom_image,           */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_lum (S) (CAI_IMAGE *)  : = NULL si pb               */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */                                                                         /*                                                                           */  
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des UOCT, I4,UI2,UI4 et R8    */
/*                            en lecture et ecriture en big et little endian */
/*****************************************************************************/

CAI_IMAGE* LUMImageIO::cai_ouvre_lecture_lum(	char *repert,
			  	     		char *nom_image,
				     		int  *nb_can,
				     		int  *nboct_pix,
				     		int  *nb_col,
				     		int  *nb_lig)
{
   FILE *fic_in;	      /* pointeur sur descripteur de fichier         */
   CAI_IMAGE *image1;	      /* pointeur su descripteur de fichier image    */       
   int  iret,icr;	      /* indicateur de retour ok ou pas              */
   char nom_fic[1024];	      /* nom du fichier en-tete		             */
   char nom_fic_canal[1024];  /* nom fichier canal dont on teste existence   */
   int nbbits;		      /* nb de bits par pixel du fichier en-tete     */
   int retour;		      /* retour de la fonction comparaison chaines   */ 
   int i,num;		      /* indice 			             */
   char *type_cod;	      /* type de codage du pixel                     */
   char cod_pix[5];           /* codage du pixel                             */
   char type_image[3];        /* type code+ordre rangement octets dans entete*/
   char *type_machine;        /* ordre rangement octets machine travail      */
   char  buf[15];             /* buffer contenant entete image               */
   int v;	              /* comparaison type machine-type image         */
   char *v1;                  /* comparaison type image avec chaine vide     */
   int nb_col2, nb_lig2,nb;   /* variables intermediaires inversion octets   */
   int taille;                /* nb d'octets par pixel                       */
   
   
   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   nb=2;
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
   
 
   iret=1;
   type_cod = NULL;
   type_cod = (char *) calloc(5,sizeof(char)) ; 
   type_machine = NULL;
   type_machine = (char *) calloc(3,sizeof(char));
   
   if (type_machine == NULL)
   {
      iret=0;
      strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
   }

   image1 = new CAI_IMAGE[1];
     
   
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
    }
   else
   {
/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.lum",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.lum",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"rb");
   if (fic_in==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete LUM");
    }
   else
   {
   
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/   
/*..................................
   recuperation infos completes 
  ......................................*/   
   	fread(buf,12,1,fic_in);
   		
/*..................................
   recuperation du type de codage des pixels
        et donc du type de l'image 
  ......................................*/
	memcpy(type_cod,&buf[8],4);
	taille = cai_recuptyp_lum(type_cod,type_image,&nbbits,cod_pix);
        if (taille != -1)
        {
            sprintf(image1->TYPE_COD,type_cod);
	    image1->NB_BITS_PIX=nbbits;
    	    image1->NBOCT_PIX=taille;
	    sprintf(image1->COD_PIX,cod_pix);
	    *nboct_pix = image1->NBOCT_PIX;
	    *nb_can = 1;	
        }
	else
	{
	   iret=0;
     	   sprintf(CAI_ERREUR,"Type de codage LUM non reconnu : %s",type_cod);
           goto ERREUR;
        }
			
 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/   
   	type_machine = cai_test_pFpf();
   
 /*..................................
   determination des autres infos 
   en fonction inversion
  ......................................*/  
   
	v = strncmp(type_machine,type_image,2);
	v1 = strrchr(type_image,'E');
	  	  	
/*..................................
   recuperation du nombre de colonne 
  ......................................*/
	memcpy(nb_col,&buf[0],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)nb_col, (void*)&nb_col2,4 );
		cai_inverser_tableau((short *)nb_col, (short *)&nb_col2,nb);
	}
/*..................................
   recuperation du nombre de lignes 
  ......................................*/
	memcpy(nb_lig,&buf[4],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) && (v1!=NULL) )
	{
		swab( (void*)nb_lig, (void*)&nb_lig2,4 );
		cai_inverser_tableau((short *)nb_lig, (short *)&nb_lig2,nb);
	}
	
/**********************/	
/* fin modifications  */
/**********************/	
  }

/*..................................
   Fermeture du fichier 
  ......................................*/
  if (fic_in != NULL)  fclose(fic_in);

   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     if ((long)strlen(repert)!=0)
     {
       sprintf(nom_fic_canal,"%s/%s.lum",repert,nom_image);
      }
      else
      {
       sprintf(nom_fic_canal,"%s.lum",nom_image);
      }
  
     num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
           sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
           image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=(*nb_col)*image1->NBOCT_PIX;
	}
	
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/ 
	   if ( (v!=0) && (v1!=NULL) )
	   {
		image1->INV_OCTETS =1;
	   }
	   else
	   {
		image1->INV_OCTETS =0;
	   }
	   
/**************************/
/*    fin modifications   */ 
/**************************/        
      }
      else
         iret=0;
     }
    }
//    if (type_cod!=NULL) free(type_cod);
//    if (type_machine!=NULL) free(type_machine);

ERREUR:
  
    if (iret == 0)
    {
	return (NULL);
     }
    else
    {
      return(image1);
     }
}
/* 
   Fin de l'operation cai_ouvre_lecture_lum
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_lum                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format lum et de tous les fichiers qui la composent          */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_lum(nom_image,           */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_lum (S) (CAI_IMAGE *) :     =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE*  LUMImageIO::cai_ouvre_creation_lum(	char *repert,
				      		char * nom_image,
				     		int  *nb_can,
				      		int  *nboct_pix,
				      		int  *nb_col,
				      		int  *nb_lig,
				      		char *label)


{
   FILE *fic;		      /* pointeur sur descripteur de fichier         */
   CAI_IMAGE *image1;	      /* pointeur su descripteur de fichier image    */ 
   char nom_fic[1024];	      /* nom du fichier en-tete		             */
   long retour;		      /* retour de fonction 			     */ 
   int i,num;		      /* indice 				     */
   struct stat	buf_stat;     /* structure decrivant status de fichier       */
   unsigned char *Header;     /* entete du fichier lum 	                     */
   int taille_entete;	      /* taille de l'entete		             */
   char *type_image;          /* type_code+infos ordre des octets dans entete*/
   int reconnaissance_type;   /* taille du pixel                             */

   fic = NULL;
   image1=NULL;


/*********************************************/
/* Verification qu'un seul canal est demande */
/*********************************************/
   if (*nb_can != 1)
   {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image LUM ne peut contenir qu'un seul canal");
	goto ERREUR;
   }
    
/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   
   image1 = new CAI_IMAGE[1];

/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_creation");
     goto ERREUR;
   }

/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if ((long)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.lum",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.lum",nom_image);
    }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s LUM existe \n" , nom_image );
	goto ERREUR;
   }

/*......................................................
  Ouverture du fichier entete en ecriture 
  Ecriture du contenu
  ..........................................................*/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
  fic = NULL;
  fic = fopen(nom_fic,"wb");
  if (fic == NULL)
  {
      sprintf(CAI_ERREUR,"Erreur: Creation fichier %s impossible\n",nom_fic);
      goto ERREUR;
   }
   taille_entete = (*nb_col)*(*nboct_pix);
   Header = (unsigned char *) calloc(taille_entete,sizeof( unsigned char));
   if (Header == NULL)
   {
      sprintf(CAI_ERREUR,"Erreur alloc: Creation fichier %s impossible\n",
				nom_fic);
      goto ERREUR;
   }

/*********************************************/
/* ecriture du nombre de colonnes de l'image */
/*********************************************/
   memcpy(Header,nb_col , 4 );

/*******************************************/
/* ecriture du nombre de lignes de l'image */
/*******************************************/
   memcpy(&Header[4],nb_lig , 4 );


/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  	
  
/*******************************************/
/* ecriture du type de l'image dans la zone*/
/*     reservee au type de codage          */
/*******************************************/
   
  /*....................................
  test sur la machine de travail pour 
         typer image en LE ou BE      
  .....................................*/
  
  type_image = cai_test_pFpf();


/**************************************/
/* ecriture type de codage de l'image */
/**************************************/
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  Maj du tag TYPE lorsque l'utilisateur le      */
/*                                  precise dans le param  "label"           */
/*****************************************************************************/
/* Verification si type de codage reconnu dans le label */
  reconnaissance_type = cai_istyp_connu(label);
  if (strncmp(type_image,"BE",2)==0)
  {
     if (*nboct_pix == 1) 
     {
	if (reconnaissance_type < 1)
 	   sprintf(image1->TYPE_COD,"08BI");
	else if (reconnaissance_type ==1)
           sprintf(image1->TYPE_COD,"08BU");
        else 
        {
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }          
      }
      else if (*nboct_pix == 2)
      {
	if (reconnaissance_type ==3)
           sprintf(image1->TYPE_COD,"16BU");
        else if ((reconnaissance_type == -1)|| (reconnaissance_type == 2))
           sprintf(image1->TYPE_COD,"16BI");
        else 
        {
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }     
      }
      else if (*nboct_pix == 4) 
      {
	if (reconnaissance_type ==4)
           sprintf(image1->TYPE_COD,"32BI");
        else if (reconnaissance_type ==5)
           sprintf(image1->TYPE_COD,"32BU");
        else if ((reconnaissance_type == -1)|| (reconnaissance_type == 6))
           sprintf(image1->TYPE_COD,"FLOA");
        else 
        {
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }     
      }
      else if (*nboct_pix == 8) 
      {
          if ((reconnaissance_type == -1)|| (reconnaissance_type ==7))
               sprintf(image1->TYPE_COD,"R8B ");
          else 
          {
	    sprintf(CAI_ERREUR,"incoherence entre TYPE et Nbbits par pixels\n");
	    goto ERREUR;
           } 
      }
    }
  else
  {
     if (*nboct_pix == 1) 
     {
	if (reconnaissance_type < 1)
 	   sprintf(image1->TYPE_COD,"08LI");
	else if (reconnaissance_type ==1)
           sprintf(image1->TYPE_COD,"08LU");
        else 
        {
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }      
      }
      else if (*nboct_pix == 2)
      {
	if (reconnaissance_type ==3)
           sprintf(image1->TYPE_COD,"16LU");
        else if ((reconnaissance_type == -1)|| (reconnaissance_type == 2))
           sprintf(image1->TYPE_COD,"16LI");
        else 
        {
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }     
      }
      else if (*nboct_pix == 4) 
      {
	if (reconnaissance_type ==4)
           sprintf(image1->TYPE_COD,"32LI");
        else if (reconnaissance_type ==5)
           sprintf(image1->TYPE_COD,"32LU");
        else if ((reconnaissance_type == -1)|| (reconnaissance_type == 6))
           sprintf(image1->TYPE_COD,"FLOL");
        else 
        {
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }     
      }
      else if (*nboct_pix == 8) 
      {
          if ((reconnaissance_type == -1)|| (reconnaissance_type ==7))
               sprintf(image1->TYPE_COD,"R8L ");
          else 
          {
	    sprintf(CAI_ERREUR,"incoherence entre TYPE et Nbbits par pixels\n");
	    goto ERREUR;
           } 
      }

   }
   memcpy(&Header[8],image1->TYPE_COD , 4 );
  
/***********************/
/* fin modifications   */
/***********************/
  
   retour = fwrite ( Header , taille_entete , 1 , fic );
   free(Header);
   if ( retour == 0 )
   {
      sprintf ( CAI_ERREUR ,
		 "Erreur : ecriture entete impossible dans fichier %s\n",
		  nom_fic );
      goto ERREUR;
   }
       
/*************************************************/
/* Fermeture du fichier                          */
/*************************************************/

   if (fic != NULL) fclose ( fic );

   num = open(nom_fic,O_RDWR|O_BINARY,PERMS); 
   if (num == -1)
   {

     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic);
     goto ERREUR;
   } 

   for (i=0;i<*nb_can;i=i+1)
   {
     image1->ACCES_CANAL_INT[i]=num;
     image1->POS_OCTET[i]=taille_entete;
    }

  return (image1);

ERREUR :

   if (fic !=NULL) cai_dest_ima_lum ( repert ,  nom_image );
   return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_lum
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_lum                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image LUM                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_lum(image1, canal,        */  
/* ------------------                        premiere_ligne,                 */
/*                                           premiere_colonne,               */
/*                                           nombre_lignes_lire ,            */
/*                                           nombre_colonnes_lire,           */
/*                                           pas_ligne ,pas_colonne,         */
/*                 		             data_image )                    */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     canal                (E) (int) : numero du canal a lire               */
/*     premiere_ligne       (E) (int) : premiere ligne a lire dans l'image   */
/*     premiere _colonne    (E) (int) : premiere colonne a lire dans l'image */
/*     nombre_lignes_lire   (E) (int) : nombre de lignes a lire              */
/*     nombre_colonnes_lire (E) (int) : nombre de colonnes a lire            */
/*     pas_ligne            (E) (int) : pas en ligne                         */
/*     pas_colonne          (E) (int) : pas en colonne                       */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels lus alloue et     */
/*                                      libere par l'appelant                */
/*									     */
/*     cai_lecture_canal_lum (S) (CAI_OK_KO) :     = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO LUMImageIO::cai_lecture_canal_lum(	CAI_IMAGE *image1 ,  
                                    		int *canal ,           
    		                    		int *premiere_ligne  ,   
     		                    		int *premiere_colonne,
                                    		int *nombre_lignes_lire ,
                                    		int *nombre_colonnes_lire,
                                   		int *pas_ligne ,          
                                    		int *pas_colonne,
		                    		unsigned char *data_image )        

{

 int   num,i,j,k,kk;        /* variables de travail                          */
 long pt_oct,n;		    /* position courante dans le fichier image       */
 int no_oct;                /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int   oct_pix;             /* nb octets par pixels                          */
 long  nb_oct;              /* nb oct =1er octet a lire - position courante  */
 long  oct_lig;             /* nb oct. par ligne en considerant taille pixel */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 unsigned char *data_image2;/* pointeur donnees image pour inversion octets  */
 int taille_image,Nb;       /* taille en octets de l'image                   */ 
 

/*********************************************/
/* Verification qu'un seul canal est demande */
/*********************************************/
 if (*canal != 1)
 {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image LUM ne contient qu'un seul canal");
        iret=CAI_KO;
 }
 else
 {
/*...........................
  Initialisations diverses
  ..........................*/
   iret=CAI_OK;
   num=image1->ACCES_CANAL_INT[*canal-1];
   oct_pix=image1->NBOCT_PIX;
   taille_image = (*nombre_lignes_lire)*oct_pix*(*nombre_colonnes_lire);
   data_image2 = NULL;
   data_image2 = (unsigned char *) calloc(taille_image,sizeof(unsigned char));

   if (data_image2 == NULL)
   {
      strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_lecture_canal");
      iret=CAI_KO;
      goto FIN;
   }

/*.............................................................*/
/* verification si lecture non sous-echantillonnee par bandeau */
/*.............................................................*/
  if ((*nombre_colonnes_lire==image1->NB_COL)&&(*premiere_colonne==1)&&
      (*pas_ligne==1)&&(*pas_colonne==1))
  {
       oct_lig=(*nombre_lignes_lire)*oct_pix*(image1->NB_COL);
       pch=data_image;

/*............................................
	Recherche de la position courante dans le fichier
  ............................................*/
        pt_oct=lseek(num,0L,1);

/*................................
        Calcul du numero du 1er octet a lire
  ....................................*/
        lig=*premiere_ligne;  
	no_oct=image1->POS_OCTET[*canal-1]+
	          (lig-1)*image1->NB_COL*oct_pix;


/*..................................
        Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
        nb_oct=(long)(no_oct-pt_oct);
        lseek(num,nb_oct,1);

/*...........................................
        lecture des lignes completes
  ................................................*/
         n=read(num,pch,oct_lig);
 
         if (n<=0)
         {
             	iret=CAI_KO;
           	strcpy(CAI_ERREUR,"Erreur lecture donnee image LUM");

	        goto FIN;
         }

   }
   else
   {
	oct_lig=oct_pix*image1->NB_COL;
	

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
        buf_lec = NULL;
      	buf_lec = (unsigned char *) calloc(oct_lig,sizeof(unsigned char));
      	if (buf_lec == NULL)
      	{
     	 	iret=CAI_KO;
      		strcpy(CAI_ERREUR,"Probleme allocation memoire");
      	}
      	else
      	{
/*....................
        Initialisation
  ......................*/
            lig=*premiere_ligne;
       	    i=1;
            pch=data_image;

/*.....................
         Pour chaque ligne a lire
  .......................................*/
           while ((i <= *nombre_lignes_lire)&&(iret==CAI_OK))
           {  
/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
               pt_oct=lseek(num,0L,1);

/*................................
            Calcul du numero du 1er octet a lire
  ....................................*/
               
	      no_oct=image1->POS_OCTET[*canal-1]+
	          (lig-1)*image1->NB_COL*oct_pix;


/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
              nb_oct=(long)(no_oct-pt_oct);
              lseek(num,nb_oct,1);

/*...........................................
            lecture d'une ligne image
  ................................................*/
              n=read(num,buf_lec,oct_lig);
              if (n<=0)
              {
             	iret=CAI_KO;
             	strcpy(CAI_ERREUR,"Erreur lecture donnee image LUM");
              }
              else
              { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/           
                 deb=buf_lec+(*premiere_colonne-1)*oct_pix;
                 j=1;
                 while (j <= *nombre_colonnes_lire)
	         {  
                	for (k=0;k<oct_pix;k=k+1)
                	{
                  	  *(pch+k)=*(deb+k);
                 	}
                 	j=j+1;
                	 pch=pch+oct_pix;
                	 deb=deb+(*pas_colonne*oct_pix);
                  }   
             
              }
              i=i+1;
              lig=lig+(*pas_ligne);
           }        
          	    
	   free(buf_lec);
           buf_lec=NULL;
      	}
    }
  }
  
  
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/    

     
   if ( (iret!= CAI_KO) && (image1->INV_OCTETS==1) && (oct_pix!=1) )
   {
	
	/*modification ordre selon nb octets par pixel*/
       	swab( (void*)(data_image), (void*)(data_image2), taille_image);
		
	if (oct_pix!=2)
        { 
           Nb = taille_image/2;
	   cai_inverser_tableau(data_image, data_image2,Nb);
	}
	else 
	   for (i=0; i<taille_image;i++)
	   {
	   /*stockage dans data_image*/
	   data_image[i] = data_image2[i];
	   }	   
	   
    }
       
/*************************/
/*   fin modifications   */    
/*************************/
 FIN:  
   if (data_image2!= NULL) free(data_image2);

  return(iret);
}

/*
  Fin de l'operation cai_lecture_canal_lum
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_lum                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image lum                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_lum(image1, canal,       */ 
/* ------------------                                 premiere_ligne,        */
/*                                                    nombre_lignes_ecrire,  */
/*                                                    nombre_colonnes_ecrire,*/
/*                 		                      data_image )           */
/*									     */
/*     image1      (E) (CAI_IMAGE *)   : pointeur sur descripteur de fichier */
/*     canal                 (E) (int) : numero du canal a ecrire            */
/*     premiere_ligne        (E) (int) : premiere ligne a ecrire ds l'image  */
/*     nombre_lignes_ecrire  (E) (int) : nombre de lignes a ecrire           */
/*     nombre_colonnes_ecrire(E) (int) : nombre de colonnes a ecrire         */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels ecris alloue et   */
/*                                      libere par l'appelant                */
/*									     */
/*     cai_ecriture_canal_lum    (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO LUMImageIO::cai_ecriture_canal_lum (	CAI_IMAGE *image1 ,  
                                      		int *canal ,           
    		                     	 	int *premiere_ligne  ,   
                                      		int *nombre_lignes_ecrire ,
                                      		int *nombre_colonnes_ecrire,
		                      		unsigned char *data_image )
{
long	position_first_lig;	/* position de la 1ere ligne a ecrire    */
long	position_courante;	/* position courante dans le fichier     */
long	deplacement;		/* deplacement a effectuer               */
unsigned char *buff_lig;	/* pointeur de travail  bandeau a ecrire */
long	cr_write;		/* code retour de write                  */
int num;                        /* descripteur de fichier                */
int nboct_pix;			/* nombre d'octets par pixel             */
int nb_canaux;			/* nombre de canaux de l'image totale    */
int nboct_ecrire;		/* nb d'octets du bandeau a ecrire       */

  
/*********************************************/
/* Verification qu'un seul canal est demande */
/*********************************************/
	if (*canal != 1)
 	{
		image1 = NULL;
 		sprintf ( CAI_ERREUR , 
			"Une image LUM ne contient qu'un seul canal");
		goto ERREUR;
 	}

/* ................
   Initialisations 
  .................*/

	num = image1->ACCES_CANAL_INT[*canal-1];
	nboct_pix = image1->NBOCT_PIX;
	nb_canaux = 1;
	position_first_lig = 0;
	position_courante = 0;
	deplacement = 0;
	nboct_ecrire = 0;

/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

	position_first_lig = (long) ( image1->POS_OCTET[*canal-1] + 
 		+ (*premiere_ligne - 1) * 
		(*nombre_colonnes_ecrire) * nboct_pix );
		
/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

	position_courante = lseek ( num , 0L , 1 );

/********************************************************/
/* Calcul du deplacement jusqu'a la 1ere ligne a ecrire	*/
/********************************************************/

	deplacement = position_first_lig - position_courante;

/*******************************************************/
/*	deplacement jusqu'a la premiere ligne a ecrire*/
/*******************************************************/

	lseek ( num , deplacement , 1 ); 

/***********************************************/
/*	position en debut de bandeau a ecrire  */
/***********************************************/

	buff_lig = data_image; 

/********************************************************/
/*	ecriture des lignes				*/
/********************************************************/
	nboct_ecrire = (*nombre_colonnes_ecrire)*nboct_pix*
			(*nombre_lignes_ecrire);

	cr_write = write ( num , buff_lig,nboct_ecrire);

	if ( cr_write !=  nboct_ecrire)
	{
		   strcpy ( CAI_ERREUR , "Erreur ecriture canal image lum\n" );
		   goto ERREUR;
	}
 
return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/* 
  Fin de l'operation cai_ecriture_canal_lum
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_lum                                              */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format lum et de           */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_lum(image1)                       */  
/* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_lum    (S) (CAI_OK_KO) :       = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO  LUMImageIO::cai_ferme_lum (CAI_IMAGE *image1)


{

 int iret;                 /* code retour de l'operation         */
 int num;                  /* descripteur du fichier a fermer    */
 unsigned char Header[12]; /* entete a MAJ si creation           */
 int temp;                 /* variable temporaire                */
 long pt_oct;		   /* position courante ds fichier image */
 long retour;

/*--------------------------*/
/* Initialisations diverses */
/*--------------------------*/
 iret=CAI_OK;
 num=image1->ACCES_CANAL_INT[0];

/*--------------------------------------------------*/
/* Mise a jour de l'entete si ouverture en creation */
/*--------------------------------------------------*/
if (image1->MODE == CAI_ECRITURE)
{
   pt_oct=lseek(num,0L,0);
 
/*******************************************************/
/* ecriture nombre de colonnes		               */
/*******************************************************/
   temp = image1->NB_COL;
   memcpy(Header,&temp , 4 );

/*******************************************************/
/* ecriture nombre de ligne			       */
/*******************************************************/
   temp = image1->NB_LIG;
   memcpy(&Header[4],&temp , 4 );

/*****************************************************************/
/* ecriture du type de codage en fonctions de la taille du pixel */
/* et du codage des Poids Forts, poids faibles                   */
/*****************************************************************/
   memcpy(&Header[8],image1->TYPE_COD , 4 );

   retour = write ( num,Header , 12 );
   if ( temp == 0 )
   {
      sprintf ( CAI_ERREUR ,
       "Erreur : MAJ entete a la fermeture impossible ds fichier lum\n");
      goto ERREUR;
   }

}

/*.............................................
   Fermeture du fichier qui contient tous les canaux
  .............................................*/
   if (num>0) close(num);

  return (iret);

ERREUR:
  return(CAI_KO);

}

/*
  Fin de l'operation cai_ferme_lum
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_lum                                           */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en lum et de         */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_lum(repertoire,nom_ima)        */  
/* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_lum (S) (CAI_OK_KO) : = CAI_KO si pb                     */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO LUMImageIO::cai_dest_ima_lum ( char *repertoire , 
                                 	 char *nom_ima )


{
int	i_can;			/* compteur de canaux */
int     iret;                   /* code retour fonction fermetrue lecture */
char	str_num_canal[3];	/* suffixe numero du canal */
char	nom_fic_entete[1024];	/* nom du fichier entete a detruire */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image */
char	nom_fic_canal[1024];	/* nom du fichier canal a detruire */

int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 

CAI_IMAGE   *image1;             /* structure ouverture image */
int         nb_can;	        /* nombre de canaux de l'image */
int         nboct_pix;		/* nombre d'octets par pixel image */
int         nb_col;	        /* nombre de colonnes de l'image */
int         nb_lig;             /* nombre de lignes de l'image */

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/

if ((long)strlen(repertoire)!= 0)
{
  sprintf( nom_ima_complet, "%s/%s" ,repertoire,nom_ima);
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}
  sprintf(nom_fic_entete ,"%s.lum", nom_ima_complet);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/

cr_stat = stat ( nom_fic_entete , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_entete );


return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_lum
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_lum                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     lum et de tous les fichiers qui la composent                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_lum(nom_image,           */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_lum (S) (CAI_IMAGE *)  : = NULL si pb               */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */                                                                            /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE* LUMImageIO::cai_ouvre_modifie_lum(	char *repert,
				     		char *nom_image,
				     		int  *nb_can,
				     		int  *nboct_pix,
				     		int  *nb_col,
				     		int  *nb_lig)


{
   FILE *fic_in;	      /* pointeur sur descripteur de fichier         */
   CAI_IMAGE *image1;	      /* pointeur su descripteur de fichier image    */       
   int  iret,icr;	      /* indicateur de retour ok ou pas              */
   char nom_fic[1024];	      /* nom du fichier en-tete		             */
   char nom_fic_canal[1024];  /* nom fichier canal dont on teste existence   */
   int nbbits;		      /* nb de bits par pixel du fichier en-tete     */
   int retour;		      /* retour de la fonction comparaison chaines   */ 
   int i,num;		      /* indice 			             */
   char *type_cod;	      /* type de codage du pixel                     */
   char cod_pix[5];           /* codage du pixel                             */
   char type_image[3];        /* type code+ordre rangement octets dans entete*/
   char *type_machine;        /* ordre rangement octets machine travail      */
   char  buf[15];             /* buffer contenant entete image               */
   int v;	              /* comparaison type machine-type image         */
   char *v1;                  /* comparaison type image avec chaine vide     */
   int nb_col2, nb_lig2,nb;   /* variables intermediaires inversion octets   */
   int taille;                /* nb d'octets par pixel                       */

    
   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   nb=2;
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;
   
   type_cod = NULL;
   type_cod = (char *) calloc(5,sizeof(char)) ; 
   type_machine = NULL;
   type_machine = (char *) calloc(3,sizeof(char));
   
   if (type_machine == NULL)
   {
      iret=0;
      strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
   }

   image1 = new CAI_IMAGE[1];
   
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

 if (image1==NULL)
 {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_modifie");
 }
 else
 {

/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.lum",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.lum",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"rb");
   if (fic_in==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete LUM");
    }
   else
   {
    	
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  
 /*..................................
   recuperation infos completes 
  ......................................*/   
   	fread(buf,12,1,fic_in);
	
   /*..................................
   recuperation du type de codage des pixels
        et donc du type de l'image 
  ......................................*/
	memcpy(type_cod,&buf[8],4);
	taille = cai_recuptyp_lum(type_cod,type_image,&nbbits,cod_pix);

        if (taille != -1)
        {
            sprintf(image1->TYPE_COD,type_cod);
	    image1->NB_BITS_PIX=nbbits;
    	    image1->NBOCT_PIX=taille;
	    sprintf(image1->COD_PIX,cod_pix);
	    *nboct_pix = image1->NBOCT_PIX;
	    *nb_can = 1;	
        }
	else
	{
	   iret=0;
     	   sprintf(CAI_ERREUR,"Type de codage LUM non reconnu : %s",type_cod);
           goto ERREUR;
        }
	   	
 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/   
   
   	type_machine = cai_test_pFpf();
	
 /*..................................
   determination des autres infos 
   en fonction inversion
  ......................................*/  
   
	v = strncmp(type_machine,type_image,2);
	v1 = strrchr(type_image,'E');
	  	  	
/*..................................
   recuperation du nombre de colonne 
  ......................................*/
	memcpy(nb_col,&buf[0],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) && (v1!=NULL) )
	
	{	
		swab( (void*)nb_col, (void*)&nb_col2,4 );
		cai_inverser_tableau((short *)nb_col, (short *)&nb_col2,nb);
	}
/*..................................
   recuperation du nombre de lignes 
  ......................................*/
	memcpy(nb_lig,&buf[4],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) && (v1!=NULL) )
	{
		swab( (void*)nb_lig, (void*)&nb_lig2,4 );
		cai_inverser_tableau((short *)nb_lig, (short *)&nb_lig2,nb);
	}


/************************/
/*  fin modifications   */
/************************/
  }
  
/*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in != NULL) fclose(fic_in);

   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     if ((long)strlen(repert)!=0)
     {
       sprintf(nom_fic_canal,"%s/%s.lum",repert,nom_image);

      }
      else
      {
      	 sprintf(nom_fic_canal,"%s.lum",nom_image);
        }
  
     num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
           sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
            image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=(*nb_col)*image1->NBOCT_PIX;
        }
	
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  	
	
        if ( (v!=0) && (v1!=NULL) )
	{
	   image1->INV_OCTETS =1;
	}
	else
	{
	   image1->INV_OCTETS =0;
	}
	
/***********************/
/*  fin modifications  */        
/***********************/

      }
      else
         iret=0;
     }
    }
ERREUR:
    if (iret == 0)
    {
	return (NULL);
     }
    else
    {
      return(image1);
     }

}
/* 
   Fin de l'operation cai_ouvre_modifie_lum

*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_recuptyp_lum                                           */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de verification si pixels de type lum ou non           */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  retour = cai_recuptyp_lum(type_code,SensCode,nbbits , */
/*					             cod_pix)                */
/*  ---------------- 		                                             */
/*                                                                           */
/*     type_code    (E) (char *) : type de codage a verifier                 */
/*     SensCode     (S) (char *) : sens du codage BE ou LE                   */
/*     nbbits       (S) (char *) : nombre de bits significatifs ds le pixel  */
/*     cod_pix      (S) (char *) : codage du pixel                           */
/*     retour       (S) (int)    : Taille du pixel en octet si OK reconnu    */ 
/*				   -1 si non reconnu			     */
/*                                                                           */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
int LUMImageIO::cai_recuptyp_lum( char *type_code, 
				  char *SensCode, 
				  int *nbbits, 
				  char *cod_pix)
{
	int ind;	        /* indice de boucle sur les types reconnus */
	int trouve,icr,taille;  /* indice pour la recherche                */
        int mod2;		/* modulo2				   */
        char* pch0;

/* Initialisations diverses */
	trouve = -1;
	taille = -1;
	*nbbits=-1;
	ind = 0;
        pch0=NULL;
	while ((trouve==-1)&&(ind< CAI_NB_LUM))
	{
	  pch0=strstr(type_code,CAI_LUM_TYP[ind]);
	  if (pch0!=NULL)
	       trouve=ind;
	  else
	       ind=ind+1;
	}
	if (trouve !=-1)
        {
            mod2=ind%2;
            if (mod2==0) sprintf(SensCode,"BE");
		 else sprintf(SensCode,"LE");            

	    if (ind < 4)
            {
               icr=sscanf(type_code,"%d",nbbits);
               if (icr ==0) *nbbits=-1 ;
	       else
               {
		    trouve =(int)(*nbbits/8);
		    if ((*nbbits%8)!=0) taille =trouve+1;
			else taille  = trouve;		    
               } 
	       pch0 = strstr(type_code,"U");
               if (taille == 1)
               {
		  if (pch0==NULL) sprintf(cod_pix,"OCT");
			else sprintf(cod_pix,"UOCT");
               }
               else if (taille == 2)
               {
		  if (pch0==NULL) sprintf(cod_pix,"I2");
			else sprintf(cod_pix,"UI2");
               }
               else if (taille == 4)
               {
		  if (pch0==NULL) sprintf(cod_pix,"I4");
			else sprintf(cod_pix,"UI4");
               }
               else taille=-1;
            }
            else if  ((ind ==4)||(ind==5) ) 
            {
                taille=4;
	        *nbbits = 32;
		sprintf(cod_pix,"R4");

            }
            else if ((ind ==6)||(ind==7) )
            {
                taille=8;
	        *nbbits = 64;
		sprintf(cod_pix,"R8");
            }
        }

/* sortie normale */
	return(taille);
}
/* 
   Fin de l'operation cai_recuptyp_lum
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_test_pFpf                                              */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de test qui determine ordre des poids forts et faible  */
/* -----     pour lire correctement entete et image                          */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  char *cai_test_pFpf()                                 */
/*  ---------------- 		                                             */
/*				                 			     */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
 char* LUMImageIO::cai_test_pFpf ()
 {

/*variable contenant 3 caracteres*/
/*-------------------------------*/

static char type_code[3];

/*codage sur 2 octets*/ 
/*de la valeur 1*/
/*-------------------*/ 

unsigned short a=1;

/*test sur la valeur 
/*de l'octet de gauche*/
/*--------------------*/

if (*(char *) &a == 0) 
	{
	strcpy(type_code, "BE"); /*affectation d'une chaine de caracteres*/
	}
if (*(char *) &a == 1) 
	{
	strcpy(type_code, "LE"); /*affectation d'une chaine de caracteres*/
	}

/*retourne le contenu de la variable*/
/*comme il s'agit de tableau pas besoin du &*/
/*------------------------------------------*/	
return(type_code);

} 
/* 
   Fin de l'operation cai_test_pFpf
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_inverser_tableau                                       */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'inversion des poids forts et faible                  */
/* -----              							     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  cai_inverser_tableau(sortie, entree,nb)               */
/*  ---------------- 		                                             */
/*				                 			     */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

void LUMImageIO::cai_inverser_tableau(short *BufL,short *BufE,int Nb)
{
  int pix;
  for (pix=0;pix<Nb;pix=pix+2)
  {
    BufL[pix]=BufE[pix+1];
    BufL[pix+1]=BufE[pix];
  }
}

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_istyp_connu                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de verification si pixels de type connu ou non         */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    retour = cai_istyp_connu(type_code)                 */
/*  ---------------- 		                                             */
/*                                                                           */
/*     type_code    (E) (char *) : type de codage a verifier                 */
/*     retour       (S) (int)    : 1 si OK reconnu                           */ 
/*				   -1 si non reconnu			     */
/*                                                                           */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
int LUMImageIO::cai_istyp_connu(char *type_code)
{
	int ind;	 /* indice de boucle sur les types reconnus */
	int trouve,icr;  /* indice pour la recherche                */

/* Initialisations diverses */
	trouve = -1;
	ind = 0;
	while ((trouve==-1)&&(ind< CAI_NB_TYPE))
	{
	  icr=strncmp(type_code,CAI_TYPE_CONNU[ind],strlen(type_code));
	  if (icr==0)
	       trouve=ind;
	  else
	       ind=ind+1;
	}
	

/* sortie normale */
	return(trouve);
}
/* 
   Fin de l'operation cai_istyp_connu
*/


  
} // end namespace otb

