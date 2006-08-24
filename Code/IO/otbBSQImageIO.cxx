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

#include "otbBSQImageIO.h"
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
# include <sys/stat.h>
# include <fcntl.h>
# include <fcntl.h>
# include <malloc.h>
# include <math.h>

#if defined(WIN32) || defined(WIN32CE)
/* WIN32 PLATFORM */
        #ifndef WIN32CE
                #  include <io.h>
        #else
                #  include <wce_io.h>
        #endif

#else
/* UNIX PLATFORM */
# include <unistd.h>
# include <dirent.h>
#endif


namespace otb
{

BSQImageIO::BSQImageIO()
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

BSQImageIO::~BSQImageIO()
{

}


bool BSQImageIO::CanReadFile( const char* FileNameToRead )
{
  std::fstream Datafile;
  std::string filename(FileNameToRead);
  
  
  otbMsgDevMacro(<<"BSQImageIO::CanReadFile() IN ! " );

  if( Datafile.is_open() )
    {
    Datafile.close();
    }

  const std::string DataFileName = System::GetRootName(filename)+".hd";

  Datafile.open( DataFileName.c_str(),  std::ios::in );
  if( Datafile.fail() )
    {
    otbMsgDevMacro(<<"BSQImageIO::CanReadFile() failed data open ! " );
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
void BSQImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void BSQImageIO::ReadVolume(void*)
{
}


// Read image 
void BSQImageIO::Read(void* buffer)
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

        otbMsgDevMacro( <<" BSQImageIO::Read()  ");
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
		   
		resultatLecture = cai_lecture_canal_bsq(m_image, &noCanal, 
							&FirstRow, &FirstCol,
							&lNbLignes, &lNbColonnes,&pasLig, &pasCol, value);
					
		if (resultatLecture == CAI_KO)
                {
    	        	itkExceptionMacro(<< "Error reading image (BSQ format)");
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
otbMsgDevMacro( << "BSQImageIO::Read() terminee");
    
}


void BSQImageIO::ReadImageInformation()
{

  if(  m_FileName.empty() == true )
  {
        itkExceptionMacro(<<"BSQ read : empty image file name file.");
  }

  std::string shortFileName = System::GetShortFileName(m_FileName);
  std::string pathFileName  = System::GetPathName(m_FileName);

  // shortFileName pass to the cai_ouvre_lecture_bsq method is
  // the file name without file extension
  shortFileName = System::GetRootName(shortFileName);
  
  // Get Dataset 
  char* repert = new char[pathFileName.length()];
  sprintf(repert, "%s" ,pathFileName.c_str() );  
  char* image = new char[shortFileName.length()];
  sprintf(image, "%s" ,shortFileName.c_str() );  

  
  m_image = cai_ouvre_lecture_bsq(repert,image, &m_NbBands, 
  				  &m_NbOctetPixel,&m_width,&m_height); 

  m_image->NB_COL    = m_width;
  m_image->NB_LIG    = m_height;
  m_image->NBOCT_PIX = m_NbOctetPixel;
  
  if( m_image == NULL )
  {
      itkExceptionMacro(<<"The BSQ dataset file is NULL.");
      delete[] repert;
      delete[] image;
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

   // Automatically set the Type to Binary for BSQ data
    this->SetFileTypeToBinary();

    std::string codePixel(m_image->COD_PIX);
    bool codeFound = false;
    	
    if(codePixel == "OCT")
      {
      this->SetComponentType(CHAR);
      codeFound = true;
      }      
    if(codePixel == "I2") 
      {
      this->SetComponentType(SHORT);
      codeFound = true;
      }
    if(codePixel == "I4") 
      {
      this->SetComponentType(INT);
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
  
  delete[] repert;
  delete[] image;
}




bool BSQImageIO::CanWriteFile( const char* FileNameToWrite )
{
  std::string filename(FileNameToWrite);

  const std::string DataFileName = System::GetRootName(filename)+".hd";

  if( filename == DataFileName )
  {
        return true;
  }
  else
  {
        return false;
  }
}

void BSQImageIO::Write(const void* buffer)
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
                itkExceptionMacro(<<"BSQ : Bad alloc");
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

otbMsgDevMacro( << "BSQImageIO::Write() IORegion Start["<<this->GetIORegion().GetIndex()[0]<<","<<this->GetIORegion().GetIndex()[1]<<"] Size ["<<this->GetIORegion().GetSize()[0]<<","<<this->GetIORegion().GetSize()[1]<<"] on Image size ["<<m_Dimensions[0]<<","<<m_Dimensions[1]<<"]");

        unsigned long lNbPixels = (unsigned long)(lNbColonnes*lNbLignes);
        unsigned long lTailleBuffer = (unsigned long)(m_NbOctetPixel)*(lNbPixels);
otbMsgDevMacro( <<" TailleBuffer alloué : "<< lTailleBuffer);
       
        unsigned char* value = new unsigned char[lTailleBuffer];
        if(value==NULL)
        {
                itkExceptionMacro(<<"BSQImageIO::Write() Error in memory allocation");
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

		resultatEcriture = cai_ecriture_canal_bsq(m_image, &noCanal, 
							&FirstRow,&lNbLignes,
							&lNbColonnes, value);
					
		if (resultatEcriture == CAI_KO)
                {
    	        	itkExceptionMacro(<< "Error writing image (BSQ format)" << m_FileName.c_str()<<".");
                }
        }


        delete [] value;
	otbMsgDevMacro( << "BSQImageIO::Write() terminee");
}

void BSQImageIO::WriteImageInformation()
{

}
 
void BSQImageIO::InternalWriteImageInformation()
{

  if(  m_FileName.empty() == true )
  {
        itkExceptionMacro(<<"BSQ write : empty image file name file.");
  }

  std::string shortFileName = System::GetShortFileName(m_FileName);
  std::string pathFileName  = System::GetPathName(m_FileName);

  // shortFileName pass to the cai_ouvre_lecture_bsq method is
  // the file name without file extension
  shortFileName = System::GetRootName(shortFileName);
  
  // Get Dataset 
  char* repert = new char[pathFileName.length()];
  sprintf(repert, "%s" ,pathFileName.c_str() );  
  char* image = new char[shortFileName.length()];
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
 else if ( this->GetComponentType() == SHORT )
 {
    m_NbOctetPixel = 2;
 }
 else if ( this->GetComponentType() == INT )
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
    
  
  m_image = cai_ouvre_creation_bsq( repert,image, &m_NbBands, 
  				    &m_NbOctetPixel,&m_width,&m_height,image); 
				  
  m_image->NB_COL     = m_width;
  m_image->NB_LIG     = m_height;
  m_image->NBOCT_PIX  = m_NbOctetPixel; 
  

  if( m_image == NULL )
  {
      itkExceptionMacro(<<"The BSQ dataset file is NULL.");
      delete[] repert;
      delete[] image;
      return;
  }
  
  delete[] repert;
  delete[] image;
}




/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_bsq                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format bsq       */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_lecture_bsq (nom_image,       */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels              */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_bsq     (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE* BSQImageIO::cai_ouvre_lecture_bsq(char *repert,
				 char *nom_image,
				 int  *nb_can,
				 int  *nboct_pix,
				 int  *nb_col,
				 int  *nb_lig)
{
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   long int  iret;		/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   char label[80];		/* contient un enregistrement du fichier    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   char *deb,*deb1,*pch0;	/* pointeurs sur chaine de caracteres       */
   char car[4];			/* indice du canal traite     		    */
   int i,num,IndExtens;		/* indice 				    */
   char Extens[25];		/* Extension possible du fichier	    */
   char COD_PIX[5];   	        /* contient le type de codage du pixel      */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   int cmp;			/* retour de comparaison de chaine          */
   int reconnaissance_type;
   char CAN[2];
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;
   IndExtens = 0;
   pch0=NULL;
   reconnaissance_type = -1;

 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

   image1 = new CAI_IMAGE[1];

/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_lecture");
    }
   else
   {
/*.................................................
   Constitution du nom du fichier entete
  .................................................*/

   pch0 = strstr(nom_image,".");
   if (pch0 != NULL)
   {
	*pch0 = '\0';
	pch0=pch0 + 1;
	sprintf(Extens,"%s",pch0);
	if (strcmp(Extens,"hd")!=0)
			IndExtens=1;
   }
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!= 0)
   {
      sprintf(nom_fic,"%s/%s.hd",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.hd",nom_image);
    }
   sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);
/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      nom_fic[0]='\0';
      if((long int)strlen(repert)!=0)
      {
          sprintf(nom_fic,"%s/%s.HD",repert,nom_image);
      }
      else
      {
         sprintf(nom_fic,"%s.HD",nom_image);
       }
      sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
         iret=0;
         strcpy(CAI_ERREUR,"Erreur ouverture fichier entete BSQ-CNES");
      }
    }
    if (iret !=0)
    {
/*....................................
     Decodage des informations TYPE
  ............................................*/
      fscanf(fic_in,"%s\n",label);
     retour=strncmp(label,"TYPE",4);
     if (retour!=0)
     {
       sprintf(CAI_ERREUR,
			"Informations incoherentes ligne TYPE dans %s",
			nom_fic);
       iret=0;
      }
     else
     {
         fscanf(fic_in,"%s\n",COD_PIX);
         pch0=cai_En_majuscule( COD_PIX );

/* Verification si type de codage reconnu */
         reconnaissance_type = cai_istyp_connu(pch0);

/*..................................
       Decodage du label          
  ......................................*/
	 fscanf(fic_in,"%s\n",label);
	 retour=strncmp(label,"LABEL",5);
	 if (retour!=0)
	 {
	   sprintf(CAI_ERREUR,
			"Informations incoherentes ligne LABEL dans %s",
			nom_fic);

	   iret=0;
	  }
	 else
	 {
	   fgets(image1->COMMENT,80,fic_in);
           image1->COMMENT[strlen(image1->COMMENT)-1]='\0';

 /*........................................
	Decodage du nombre de canaux
  .........................................*/
	   fscanf(fic_in,"%s\n",label);
	   retour=strncmp(label,"CHANNELS",8);
	   if (retour!=0)
	   {
	     sprintf(CAI_ERREUR,
			"Informations incoherentes ligne CHANNELS dans  %s",
			nom_fic);

	     iret=0;
	    }
	   else
	   {
	     fscanf(fic_in,"%d\n",&nbplan);

/*.......................................
	  Decodage du nombre de lignes
  ........................................*/
	     fscanf(fic_in,"%s\n",label);
	     retour=strncmp(label,"LINES",5);
	     if (retour!=0)
	     {
	       sprintf(CAI_ERREUR,
			"Informations incoherentes ligne LINES dans %s",
			nom_fic);

	       iret=0;
	      }
	     else
	     {
	       fscanf(fic_in,"%d\n",nb_lig);


/*.......................................
	  Decodage du nombre de colonnes
  ........................................*/
	       fscanf(fic_in,"%s\n",label);
	       retour=strncmp(label,"COLUMNS",7);
	       if (retour!=0)
	       {
	    	 sprintf(CAI_ERREUR,
			"Informations incoherentes ligne COLUMNS dans %s",
			nom_fic);

	    	 iret=0;
	  	}
	       else
	       {

		 fscanf(fic_in,"%d\n",nb_col);

/*.......................................
	  Decodage du nombre de bits par pixel
  ........................................*/
	  	 fgets(label,80,fic_in);
	  	 retour=strncmp(label,"BITS PER PIXEL",14);
		 if (retour!=0)
	  	 {
	    	   sprintf(CAI_ERREUR,
			"Informations incoherentes ligne BITS dans %s",
			nom_fic);

	    	   iret=0;
	  	  }
	  	 else
	    	 {
		   fscanf(fic_in,"%d\n",&nbbit);
                   image1->NB_BITS_PIX = nbbit;

/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
/*****************************************************************************/
/*..................................
       Decodage du sens de codage si présent
  ......................................*/
		   fscanf(fic_in,"%s\n",label);
		   retour=strncmp(label,"SENSCODAGE",10);
	 	   if (retour!=0)
	 	   {
/*      Si absent, pas d'inversion      */    
			image1->INV_OCTETS =0;
			sprintf(image1->SENS_COD,type_machine);
	 	    }
	 	   else
		   {
  			 fscanf(fic_in,"%s\n",label);
  			 retour=strncmp(label,"IEEE",4);
   			 if (retour==0)	
				sprintf(image1->SENS_COD,"BE");
   			 else
				sprintf(image1->SENS_COD,"LE");
   			 cmp = strncmp(type_machine,image1->SENS_COD,2);
   			 if (cmp!=0)
   			 {
	  				image1->INV_OCTETS =1;
   			 }
   			 else
  			 {
	 				 image1->INV_OCTETS =0;
   			 }
                     }
		  }
		}
	      }
	    }
	  }
	
      }
/*..................................
   Fermeture du fichier 
  ......................................*/
     if (fic_in!=NULL) 
      {
	fclose(fic_in);
	fic_in=NULL;
      }
    }

   if (iret !=0)
   {
/*..................................
     Initialisation du nombre d'octets par pixel
  ........................................................*/
       if ((nbbit%8)==0)
		*nboct_pix=(int)(nbbit/8);
       else
		*nboct_pix=(int)((nbbit/8)+1);

       if (reconnaissance_type != -1)
       {
	    	sprintf(image1->COD_PIX,"%s",COD_PIX);
       }
       else
       {
     	  sprintf(image1->COD_PIX,"UND");

          if (*nboct_pix==1)
              sprintf(image1->COD_PIX,"OCT");
          else if (*nboct_pix==2)
	      sprintf(image1->COD_PIX,"I2");
          else if (*nboct_pix==4)
          {
	     retour=strncmp(COD_PIX,"I4",2);
             if (retour==0)
	        sprintf(image1->COD_PIX,"I4");
             else
             {
	 	retour=strncmp(COD_PIX,"R4",2);
          	if (retour==0)
	    	  sprintf(image1->COD_PIX,"R4");
             }
           }
           else if (*nboct_pix==8)
           {
		sprintf(image1->COD_PIX,"R8");	   
           }
         }
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
       deb = image1->NOM_FIC_CAN;
       deb1= image1->NOM_UTIL_CAN;
      if ((nbplan==1)&&(IndExtens==1))
      {
/* Verification de l'existence du fichier demande et ouverture */
          nom_fic[0]='\0';
          if ((long int)strlen(repert)!= 0)
             sprintf(nom_fic,"%s/%s.%s",repert,nom_image,Extens);
 	  else
             sprintf(nom_fic,"%s.%s",nom_image,Extens);
          nom_fic_canal[0]='\0';
          sscanf(nom_fic,"%s",nom_fic_canal);
          num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
          if (num!= -1)
          {
            sprintf(deb,"%s.%s",nom_image,Extens);
  	    deb=deb+100;
            sprintf(deb1,"%s.%s",nom_image,Extens);
            deb1=deb1+100;
          }
          image1->ACCES_CANAL_INT[*nb_can]=num;
          image1->POS_OCTET[*nb_can]=0;

          *nb_can=*nb_can+1;     
      }
      else
      {
/*...................................................
     recherche du nombre de canaux de l'image si pas d'extension donnee
  ..........................................................*/
        nom_fic[0]='\0';
        if ((long int)strlen(repert)!= 0)
        {
            sprintf(nom_fic,"%s/%s.c",repert,nom_image);
         }
        else
        {
            sprintf(nom_fic,"%s.c",nom_image);
         }
 /*****************************************************************************/
/* MOD : VERSION : 4.4 : FA : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
       sprintf(nom_fic_canal,"%s1",nom_fic);
       retour = stat ( nom_fic_canal , &buf_stat );
       if ( retour != 0 )
       {
           sprintf(CAN,"C");
           if ((long int)strlen(repert)!= 0)
           {
               sprintf(nom_fic,"%s/%s.C",repert,nom_image);
            }
            else
            {
                sprintf(nom_fic,"%s.C",nom_image);
             }

       }
       else
           sprintf(CAN,"c");
        for (i=1;i<CAI_NB_MAX_CAN;i=i+1)
        {
          nom_fic_canal[0]='\0';
          sprintf(nom_fic_canal,"%s%d",nom_fic,i);
          num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
          if (num!= -1)
          {
            if (*nb_can<CAI_NB_MAX_CAN)
            {
           	sscanf(nom_image,"%s",deb);
	   	strcat(deb,CAN);
                sprintf(car,"%d",(*nb_can)+1);
           	strcat(deb,car);
  	   	deb=deb+100;
           	sscanf(nom_image,"%s",deb1);
	   	strcat(deb1,CAN);
           	strcat(deb1,car);
           	deb1=deb1+100;
             }
             image1->ACCES_CANAL_INT[*nb_can]=num;
             image1->POS_OCTET[*nb_can]=0;

             *nb_can=*nb_can+1;     
          }
         }
        if (*nb_can!=nbplan)
        {
       		sprintf(CAI_ERREUR,
		"incoherence au niveau du nombre de fichiers canaux dans %s",
		nom_fic);
 
       		iret=0;
         }
      }
     }
    }
    if (iret == 0)
	return (NULL);
    else
      return(image1);
}

/* 
   Fin de l'operation cai_ouvre_lecture_bsq
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_bsq                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format bsq et de tous les fichiers qui la composent          */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    CAI_IMAGE *cai_ouvre_creation_bsq (nom_image,       */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels              */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_bsq     (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE* BSQImageIO::cai_ouvre_creation_bsq(char *repert,
				  char *nom_image,
				  int  *nb_can,
				  int  *nboct_pix,
				  int  *nb_col,
				  int  *nb_lig,
				  char *label)

{
   FILE *fic;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   int retour;			/* retour de fonction 			    */ 
   char car[4];			/* indice du canal traite     		    */
   int i,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int reconnaissance_type;     

      
/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/

   image1 = new CAI_IMAGE[1];
/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_creation");
     goto ERREUR;
   }

/*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if ((long int)strlen(repert)!= 0)
   {
      sprintf(nom_fic,"%s/%s.hd",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.hd",nom_image);
   }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s BSQ existe \n" , nom_image );
	goto ERREUR;
   }
   sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);
/*...............................................................
  Pour chaque canal de l'image,constitution du nom de fichier correspondant
  Verification qu'il n'existe pas deja
  Ouverture du fichier en ecriture
  .................................................................*/

  for (i=0;i<*nb_can;i=i+1)
  {
   nom_fic_canal[0]='\0';
   sprintf(car,"%d",i+1);
   if (i+1<10)
         car[1]='\0';
   else
   {
      if (i+1<100)
          car[2]='\0';
      else
           car[3]='\0';
    }

    if ((long int)strlen(repert)!= 0)
    {
      sprintf(nom_fic_canal,"%s/%s.c%s",repert,nom_image,car);
   }
   else
   {
      sprintf(nom_fic_canal,"%s.c%s",nom_image,car);
   }

   retour = stat ( nom_fic_canal , &buf_stat );

   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s BSQ existe \n" , nom_image );
	goto ERREUR;
   }
   num = open(nom_fic_canal,O_RDWR|O_CREAT|O_BINARY,PERMS); 
   if (num == -1)
   {

     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
     goto ERREUR;
   } 
   image1->ACCES_CANAL_INT[i]=num;
   image1->POS_OCTET[i]=0;
  }

/*......................................................
  Ouverture du fichier entete en ecriture 
  Ecriture du contenu
  ..........................................................*/
  fic = fopen(nom_fic,"w");
  if (fic == NULL)
  {
      sprintf(CAI_ERREUR,"Erreur: Creation fichier %s impossible\n",nom_fic);
      goto ERREUR;
   }

  retour=fprintf ( fic , "TYPE\n");
  if (retour == 0)
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
   }
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  Maj du tag TYPE lorsque l'utilisateur le      */
/*                                  precise dans le param  "label"           */
/*****************************************************************************/
/* Verification si type de codage reconnu dans le label */
  reconnaissance_type = cai_istyp_connu(label);
  if (reconnaissance_type == -1)
  {
 	if (*nboct_pix==1)
    		sprintf ( image1->COD_PIX , "OCT" );
  	else  if (*nboct_pix==2)
   		sprintf ( image1->COD_PIX , "I2" );
  	else if (*nboct_pix==4)
  	{
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Maj du tag TYPE lorsque l'image est codée     */
/* en reel  et que l'utilisateur le precise dans le param  "label"           */
/*****************************************************************************/
     		// Variable de type std::string utilisée pour comparer les chaînes de caractères
     		// Etait utilisée sous LINUX la fonction strcasecmp, non définie sous Windows
     		std::string strLabel(label);
     		
     		if ( (strLabel == "R4")||
			(strLabel == "FLOAT")||
				(strLabel == "REEL") )
       			sprintf ( image1->COD_PIX , "R4" );
     		else
       			sprintf ( image1->COD_PIX , "I4" );     
  	}
  	else
   		sprintf ( image1->COD_PIX , "UND" );
    	fprintf ( fic , "%s\n",image1->COD_PIX );
   }
   else
   {
     sprintf ( image1->COD_PIX , "%s" ,label);
     fprintf ( fic , "%s\n", label );
   }
  fprintf ( fic , "LABEL\n" );
  fprintf ( fic , "%s\n" , label );
  fprintf ( fic , "CHANNELS\n" );
  fprintf ( fic , "%3d\n" , *nb_can );
  fprintf ( fic , "LINES\n" );
  fprintf ( fic , "%5d\n" , *nb_lig );
  fprintf ( fic , "COLUMNS\n" );
  fprintf ( fic , "%5d\n" , *nb_col );
  fprintf ( fic , "BITS PER PIXEL\n" );
  fprintf ( fic , "%2d\n" , (*nboct_pix)*8 );
  fprintf ( fic , "SENSCODAGE\n" );
  retour=strncmp(type_machine,"BE",2);
  sprintf(image1->SENS_COD,"%s",type_machine);
  if (retour == 0)
     fprintf ( fic , "IEEE\n"  );
  else
     fprintf ( fic , "INTEL\n"  );

/*************************************************/
/* Fermeture du fichier                          */
/*************************************************/

  fclose ( fic );
  return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_bsq
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_bsq                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image bsq                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_bsq(image1, canal,        */       		      /* ------------------                            premiere_ligne,             */
/*                                               premiere_colonne,           */
/*                                               nombre_lignes_lire ,        */
/*                                               nombre_colonnes_lire,       */
/*                                               pas_ligne ,pas_colonne,     */
/*                 		                data_image )                 */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     canal                (E) (lint): numero du canal a lire               */
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
/*     cai_lecture_canal_bsq     (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
CAI_OK_KO BSQImageIO::cai_lecture_canal_bsq(CAI_IMAGE *image1 ,  
                                int *canal ,           
    		          	int *premiere_ligne  ,   
     		          	int *premiere_colonne,
                         	int *nombre_lignes_lire ,
                          	int *nombre_colonnes_lire,
                          	int *pas_ligne ,          
                          	int *pas_colonne,
		          	unsigned char *data_image )        

{

 int   num,i,j,k;           /* variables de travail                          */
 long int pt_oct;	    /* position courante dans le fichier image       */
 long int no_oct;           /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 long int lig;              /* indice de boucle sur les lignes               */
 long int   oct_pix;        /* nb octets par pixels                          */
 long int nb_oct;           /* nb oct =1er octet a lire - position courante  */
 long int oct_lig;          /* nb oct. par ligne en considerant taille pixel */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 unsigned char *data_image2;/* pointeur donnees image pour inversion octets  */
 int taille_image,Nb;       /* taille en octets de l'image                   */ 
 long int n;
/*...........................
  Initialisations diverses
  ..........................*/

   iret=CAI_OK;
   num=(int)image1->ACCES_CANAL_INT[(*canal)-1];
   oct_pix=image1->NBOCT_PIX;
   oct_lig=oct_pix*(image1->NB_COL);

/*.......................................*/
/* verification si lecture image entiere */
/*.......................................*/
  if ((*nombre_lignes_lire==image1->NB_LIG)&&(*nombre_colonnes_lire==image1->NB_COL)
       &&(*pas_ligne==1)&&(*pas_colonne==1))
  {
       oct_lig=oct_pix*(image1->NB_COL)*(image1->NB_LIG);

    /* positionnement en debut de fichier */
       pt_oct=lseek(num,0L,0);

   /* lecture du canal complet */
      n=read(num,data_image,oct_lig);
      
      if (n<=0)
      {
             iret=CAI_KO;
             sprintf(CAI_ERREUR,"Erreur lecture donnee image BSQ dans %s",
		image1->NOM_IMAGE);

       }
        else
             iret=CAI_OK;
   }
  else
  {
/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
   buf_lec = (unsigned char *) malloc(oct_lig);
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
        while ((i <= (*nombre_lignes_lire))&&(iret==CAI_OK))
        {  

/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
            pt_oct=lseek(num,0L,1);

/*................................
            Calcul du numero du 1er octet a lire
  ....................................*/
            no_oct=image1->POS_OCTET[(*canal)-1]+((lig-1)*oct_lig);

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
             sprintf(CAI_ERREUR,"Erreur lecture donnee image BSQ  %s",
		image1->NOM_IMAGE);

            }
            else
            { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/   
             deb=buf_lec+((*premiere_colonne)-1)*oct_pix;
             j=1;
             while (j <= (*nombre_colonnes_lire))
	     {  

               for (k=0;k<oct_pix;k=k+1)
                {
                  *(pch+k)=*(deb+k);
                 }
              pch=pch+oct_pix;
              j=j+1;
              deb=deb+((*pas_colonne)*oct_pix);
             }   
          
            }
          i=i+1;
          lig=lig+(*pas_ligne);

        }        

	free(buf_lec);
      }
   }
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
/*****************************************************************************/

   if ( (iret!= CAI_KO) && (image1->INV_OCTETS==1) && (oct_pix!=1) )
   {
        taille_image = (*nombre_lignes_lire)*oct_pix*(*nombre_colonnes_lire);
        data_image2 = NULL;
        data_image2 = (unsigned char *) calloc(taille_image,
						sizeof(unsigned char));    
        if (data_image2 == NULL)
        {
           strcpy(CAI_ERREUR,"Erreur allocation memoire cai_lecture_canal");
           iret=CAI_KO;
        }
	else
        {
	    /*modification ordre selon nb octets par pixel*/
/*#if defined(WIN32) || defined(WIN32CE)*/
		swab( (char*)(data_image), (char*)(data_image2), taille_image);
/*#else
		swab( (void*)(data_image), (void*)(data_image2), taille_image);
#endif*/
       	    	
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

           if (data_image2!= NULL) free(data_image2);
        }   	   
    }
         
   return(iret);
 }
/*
  Fin de l'operation cai_lecture_canal_bsq
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_bsq                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image bsq                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      CAI_OK_KO cai_ecriture_canal_bsq(image1, canal,   */ 
/* ------------------                                 premiere_ligne,        */
/*                                                    nombre_lignes_ecrire,  */
/*                                                    nombre_colonnes_ecrire,*/
/*                 		                      data_image )           */
/*									     */
/*     image1      (E) (CAI_IMAGE *)   : pointeur sur descripteur de fichier */
/*     canal                 (E) (long int) : numero du canal a ecrire       */
/*     premiere_ligne        (E) (long int) : 1ere ligne a ecrire ds l'image */
/*     nombre_lignes_ecrire  (E) (long int) : nombre de lignes a ecrire      */
/*     nombre_colonnes_ecrire(E) (long int) : nombre de lignes a ecrire      */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels ecris alloue et   */
/*                                      libere par l'appelant                */
/*									     */
/*     cai_ecriture_canal_bsq        (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO BSQImageIO::cai_ecriture_canal_bsq (CAI_IMAGE *image1 ,  
                            	  int *canal ,           
    		            	  int *premiere_ligne  ,   
                            	  int *nombre_lignes_ecrire ,
                            	  int *nombre_colonnes_ecrire,
		            	  unsigned char *data_image )        

{

 long int position_first_lig;	/* position de la 1ere ligne a ecrire    */
 long int position_courante;	/* position courante dans le fichier     */
 long int deplacement;		/* deplacement a effectuer               */
 int i_ligne;			/* compteur de lignes                    */
 unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire  */
 long  int cr_write;		/* code retour de write                  */
 int num;                       /* descripteur de fichier                */
 int nboct_pix;		        /* nombre d'octets par pixel             */
 int tot_pix;                   /* nb de pixels total a ecrire           */

/* initialisations */

i_ligne = 0;
num = image1->ACCES_CANAL_INT[*canal-1];
nboct_pix = image1->NBOCT_PIX;

/*.......................................*/
/* verification si ecriture image entiere */
/*.......................................*/
  if ((*nombre_lignes_ecrire==image1->NB_LIG)&&
		(*nombre_colonnes_ecrire==image1->NB_COL))
  {
       tot_pix=nboct_pix*(image1->NB_COL)*(image1->NB_LIG);

    /* positionnement en debut de fichier */
       position_first_lig=lseek(num,0L,0);

   /* ecriture du canal complet */
      cr_write = write ( num , data_image,tot_pix);
      if (cr_write != tot_pix)
      {
              sprintf(CAI_ERREUR,"Erreur ecriture donnee image BSQ  %s",
		image1->NOM_IMAGE);

             goto ERREUR;
       }
   }
  else
  {

/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

position_first_lig =(long int) ( image1->POS_OCTET[*canal-1]+ 
			    (( *premiere_ligne - 1) * (*nombre_colonnes_ecrire)
		              * nboct_pix));
		

/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

position_courante = lseek ( num , 0L , 1 );


/********************************************************/
/* Deplacement jusqu'a la premiere ligne a ecrire	*/
/********************************************************/

deplacement = position_first_lig - position_courante;
lseek ( num , deplacement , 1 ); 

/********************************************************/
/* TANT QUE il y a des lignes a ecrire                 	*/
/********************************************************/
while (i_ligne < (*nombre_lignes_ecrire))
{

/********************************************************/
/*	extraction de la ligne du tableau 		*/
/********************************************************/

	buff_lig = data_image + (*nombre_colonnes_ecrire) * nboct_pix * i_ligne;

/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/
 
	cr_write = write ( num , buff_lig ,
			 (*nombre_colonnes_ecrire) * nboct_pix );
	if ( cr_write != (*nombre_colonnes_ecrire)*nboct_pix )
	{
	    sprintf( CAI_ERREUR , "Erreur ecriture canal image bsq_cnes  %s",
				   image1->NOM_IMAGE);

		goto ERREUR;
	}
 
/********************************************************/
/*	incrementation du nombre de lignes		*/
/********************************************************/

	i_ligne++;
/********************************************************/
/* FIN TANT QUE						*/
/********************************************************/
  }
}


return (CAI_OK);

ERREUR :

return (CAI_KO);


}

/* 
  Fin de l'operation cai_ecriture_canal_bsq
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_bsq                                              */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format bsq et de           */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_bsq(image1)                       */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_bsq        (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO BSQImageIO::cai_ferme_bsq (CAI_IMAGE *image1)

{
 	 int iret;    /* code retour de l'operation      */
 	 int i_can;   /* indice du canal a fermer        */
 	 int num;     /* descripteur du fichier a fermer */

 iret=CAI_OK;
/*.............................................
   Fermeture de tous les canaux
  .............................................*/
 for (i_can=0;i_can<image1->NB_CANAUX;i_can=i_can+1)
 {

   num=image1->ACCES_CANAL_INT[i_can];
   if (num>0) close(num);
 }

  return (iret);
}

/*
  Fin de l'operation cai_ferme_bsq
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_bsq                                           */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en bsq et de         */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_bsq(repertoire,nom_ima)        */       		      /* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_bsq     (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO BSQImageIO::cai_dest_ima_bsq ( char *repertoire , 
                     	     char *nom_ima )

{
int	i_can;			/* compteur de canaux */
long int     iret;              /* code retour fonction fermetrue lecture */
//char	str_num_canal[3];	/* suffixe numero du canal */
char	nom_fic_entete[1024];	/* nom du fichier entete a detruire */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image */
char	nom_fic_canal[1024];	/* nom du fichier canal a detruire */

long int	cr_stat;	/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 

CAI_IMAGE   *image1;            /* structure ouverture image */
int         nb_can;	/* nombre de canaux de l'image */
int         nboct_pix;	/* nombre d'octets par pixel image */
int         nb_col;	/* nombre de colonnes de l'image */
int         nb_lig;        /* nombre de lignes de l'image */

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/
if ((long int)strlen(repertoire)!= 0)
{
  sprintf(nom_ima_complet,"%s/%s",repertoire,nom_ima);
}
else
{
  sprintf(nom_ima_complet,"%s",nom_ima);
}
  sprintf(nom_fic_entete ,"%s.hd", nom_ima_complet);
/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_bsq			*/
/************************************************/

  image1 = cai_ouvre_lecture_bsq ( repertoire,nom_ima, &nb_can ,&nboct_pix ,
				   &nb_col , &nb_lig );	
  if ( image1 == NULL) goto ERREUR;

  image1->NB_CANAUX=nb_can;
  image1->NB_COL=nb_col;
  image1->NB_LIG=nb_lig;
  image1->NBOCT_PIX=nboct_pix;

  iret=cai_ferme_bsq(image1);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/

cr_stat = stat ( nom_fic_entete , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_entete );

 
/************************************************/
/* Constitution un a un des noms des fichiers	*/
/* canaux et destruction 			*/
/************************************************/

for ( i_can = 0 ; i_can < nb_can ; i_can++ )
{
	sprintf(nom_fic_canal,"%s.c%d",nom_ima_complet,i_can+1);
	cr_stat = stat ( nom_fic_canal , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_canal );
	sprintf(nom_fic_canal,"%s.C%d",nom_ima_complet,i_can+1);
	cr_stat = stat ( nom_fic_canal , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_canal );
}


return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/*
 Fin de l'operation cai_dest_ima_bsq
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_modif_nbbitspix_bsq                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    permet de modifier le nombre de bits significatifs par pixel    */
/* -----     dans l'entete de fichier (.hd)				     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_modif_nbbitspix_bsq(image1,NbBitsPix)   */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     NbBitsPix   (E) (long int)     : nb de bits significatifs par pixel   */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO BSQImageIO::cai_modif_nbbitspix_bsq(CAI_IMAGE *image1,int NbBitsPix)

{
  char fichierE[1024];  /* fichier a traiter               */
  FILE *fic;		/* pointeur descripteur fichier    */
  int retour;

        
/* Ouverture du fichier d'entete */

   fichierE[0]='\0';
   sprintf(fichierE,"%s.hd",image1->NOM_IMAGE);
   fic = fopen(fichierE,"w");
   if (fic == NULL)
   {
      sprintf(CAI_ERREUR,
	"Erreur: Modification fichier %s impossible\n",fichierE);
      goto ERREUR;
   }

/* Modification du fichier d'entete */
   rewind(fic);
   fprintf ( fic , "TYPE\n");
   fprintf ( fic , "%s\n",image1->COD_PIX );
   fprintf ( fic , "LABEL\n" );
   fprintf ( fic , "%s\n" ,image1->COMMENT);
   fprintf ( fic , "CHANNELS\n" );
   fprintf ( fic , "%3d\n" , image1->NB_CANAUX);
   fprintf ( fic , "LINES\n" );
   fprintf ( fic , "%5d\n" , image1->NB_LIG);
   fprintf ( fic , "COLUMNS\n" );
   fprintf ( fic , "%5d\n" , image1->NB_COL );
   fprintf ( fic , "BITS PER PIXEL\n" );
   fprintf ( fic , "%2d\n" , NbBitsPix );
   fprintf ( fic,  "SENSCODAGE\n");
   retour=strncmp(image1->SENS_COD,"BE",2);
   if (retour == 0)
     fprintf ( fic , "IEEE\n"  );
   else
     fprintf ( fic , "INTEL\n"  );

  fclose ( fic );

  return(CAI_OK);

ERREUR :
  return(CAI_KO);
}
/*
 Fin de l'operation cai_modif_nbbitspix_bsq
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_bsq                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     bsq et de tous les fichiers qui la composent                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_modifie_bsq (nom_image,       */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels              */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_bsq     (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE* BSQImageIO::cai_ouvre_modifie_bsq(char *repert,
				     char *nom_image,
				     int  *nb_can,
				     int  *nboct_pix,
				     int  *nb_col,
				     int  *nb_lig)


{
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   char label[80];		/* contient un enregistrement du fichier    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   char *deb,*deb1,*pch0;	/* pointeurs sur chaine de caracteres       */
   char car[4];			/* indice du canal traite     		    */
   int i,num,IndExtens;		/* indice 				    */
   char Extens[25];		/* Extension possible du fichier	    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int cmp;			/* retour de comparaison de chaine          */
   int reconnaissance_type;
   char CAN[2];
   char COD_PIX[5];

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;
   IndExtens = 0;
   pch0=NULL;

   image1 = new CAI_IMAGE[1];


 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA:Rajout des tests sur les retours d'allocation    */
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
   pch0 = strstr(nom_image,".");
   if (pch0 != NULL)
   {
	*pch0 = '\0';
	pch0=pch0 + 1;
	sprintf(Extens,"%s",pch0);
	if (strcmp(Extens,"hd")!=0)
			IndExtens=1;
   }
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!= 0)
   {
      sprintf(nom_fic,"%s/%s.hd",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.hd",nom_image);
   }
   sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);
/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
     iret=0;
     sprintf(CAI_ERREUR,"Erreur ouverture fichier entete BSQ-CNES %s",
			nom_fic);
    }
   else
   {
/*....................................
     Decodage des informations TYPE
  ............................................*/
     fscanf(fic_in,"%s\n",label);
     retour=strncmp(label,"TYPE",4);
     if (retour!=0)
     {
       sprintf(CAI_ERREUR,
			"Informations incoherentes ligne TYPE dans %s",
			nom_fic);
       iret=0;
      }
     else
     {
         fscanf(fic_in,"%s\n",COD_PIX);
         pch0=cai_En_majuscule( COD_PIX );

/* Verification si type de codage reconnu */
         reconnaissance_type = cai_istyp_connu(pch0);

/*..................................
       Decodage du label          
  ......................................*/
	 fscanf(fic_in,"%s\n",label);
	 retour=strncmp(label,"LABEL",5);
	 if (retour!=0)
	 {
	   sprintf(CAI_ERREUR,
			"Informations incoherentes ligne LABEL dans %s",
			nom_fic);
	   iret=0;
	  }
	 else
	 {
	   fgets(image1->COMMENT,80,fic_in);
           image1->COMMENT[strlen(image1->COMMENT)-1]='\0';

 /*........................................
	Decodage du nombre de canaux
  .........................................*/
	   fscanf(fic_in,"%s\n",label);
	   retour=strncmp(label,"CHANNELS",8);
	   if (retour!=0)
	   {
	     sprintf(CAI_ERREUR,
			"Informations incoherentes ligne CHANNELS dans %s",
			nom_fic);

	     iret=0;
	    }
	   else
	   {
	     fscanf(fic_in,"%d\n",&nbplan);

/*.......................................
	  Decodage du nombre de lignes
  ........................................*/
	     fscanf(fic_in,"%s\n",label);
	     retour=strncmp(label,"LINES",5);
	     if (retour!=0)
	     {
	       sprintf(CAI_ERREUR,
			"Informations incoherentes ligne LINES dans %s",
			nom_fic);
	       iret=0;
	      }
	     else
	     {
	       fscanf(fic_in,"%d\n",nb_lig);


/*.......................................
	  Decodage du nombre de colonnes
  ........................................*/
	       fscanf(fic_in,"%s\n",label);
	       retour=strncmp(label,"COLUMNS",7);
	       if (retour!=0)
	       {
	    	 sprintf(CAI_ERREUR,
			"Informations incoherentes ligne COLUMNS dans %s",
			nom_fic);
	    	 iret=0;
	  	}
	       else
	       {

		 fscanf(fic_in,"%d\n",nb_col);

/*.......................................
	  Decodage du nombre de bits par pixel
  ........................................*/
	  	 fgets(label,80,fic_in);
	  	 retour=strncmp(label,"BITS PER PIXEL",14);
		 if (retour!=0)
	  	 {
	    	   sprintf(CAI_ERREUR,
			"Informations incoherentes ligne BITS dans %s",
			nom_fic);
	    	   iret=0;
	  	  }
	  	 else
{
		   fscanf(fic_in,"%d\n",&nbbit);
                   image1->NB_BITS_PIX = nbbit;

/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
/*****************************************************************************/
/*..................................
       Decodage du sens de codage si présent
  ......................................*/
		   fscanf(fic_in,"%s\n",label);
		   retour=strncmp(label,"SENSCODAGE",10);
	 	   if (retour!=0)
	 	   {
/*      Si absent, pas d'inversion      */    
			image1->INV_OCTETS =0;
			sprintf(image1->SENS_COD,type_machine);
	 	    }
	 	   else
		   {
  			 fscanf(fic_in,"%s\n",label);
  			 retour=strncmp(label,"IEEE",4);
   			 if (retour==0)	
				sprintf(image1->SENS_COD,"BE");
   			 else
				sprintf(image1->SENS_COD,"LE");
   			 cmp = strncmp(type_machine,image1->SENS_COD,2);
   			 if (cmp!=0)
   			 {
	  				image1->INV_OCTETS =1;
   			 }
   			 else
  			 {
	 				 image1->INV_OCTETS =0;
   			 }
                     }
		  }
		}
	      }
	    }
	  }
	
      }
/*..................................
   Fermeture du fichier 
  ......................................*/
      if (fic_in !=NULL) 
      {
	fclose(fic_in);
        fic_in=NULL;
      }
   }

   if (iret !=0)
   {
/*..................................
     Initialisation du nombre d'octets par pixel
  ........................................................*/
       if ((nbbit%8)==0)
		*nboct_pix=(int)(nbbit/8);
       else
		*nboct_pix=(int)((nbbit/8)+1);

       if (reconnaissance_type != -1)
	    	sprintf(image1->COD_PIX,"%s",COD_PIX);
       else
       {
     	  sprintf(image1->COD_PIX,"UND");

          if (*nboct_pix==1)
          	sprintf(image1->COD_PIX,"OCT");
    	  else if (*nboct_pix==2)
	  	sprintf(image1->COD_PIX,"I2");
         else if (*nboct_pix==4)
         {
	  	retour=strncmp(COD_PIX,"I4",2);
          	if (retour==0)
	    	 	sprintf(image1->COD_PIX,"I4");
          	else
          	{
	 		retour=strncmp(COD_PIX,"R4",2);
          		if (retour==0)
	    	  		sprintf(image1->COD_PIX,"R4");
                }

           }
          else if (*nboct_pix==8)
	     sprintf(image1->COD_PIX,"R8");
      }

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
       deb = image1->NOM_FIC_CAN;
       deb1= image1->NOM_UTIL_CAN;
      if ((nbplan==1)&&(IndExtens==1))
      {
/* Verification de l'existence du fichier demande et ouverture */
          nom_fic[0]='\0';
          if ((long int)strlen(repert)!= 0)
             sprintf(nom_fic,"%s/%s.%s",repert,nom_image,Extens);
 	  else
             sprintf(nom_fic,"%s.%s",nom_image,Extens);
          nom_fic_canal[0]='\0';
          sscanf(nom_fic,"%s",nom_fic_canal);
          num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
          if (num!= -1)
          {
            sprintf(deb,"%s.%s",nom_image,Extens);
  	    deb=deb+100;
            sprintf(deb1,"%s.%s",nom_image,Extens);
            deb1=deb1+100;
          }
          image1->ACCES_CANAL_INT[*nb_can]=num;
          image1->POS_OCTET[*nb_can]=0;
	  *nb_can = *nb_can + 1;
      }
      else
      {
/*...................................................
     recherche du nombre de canaux de l'image si pas d'extension donnee
  ..........................................................*/
        nom_fic[0]='\0';
        if ((long int)strlen(repert)!= 0)
        {
            sprintf(nom_fic,"%s/%s.c",repert,nom_image);
         }
        else
        {
            sprintf(nom_fic,"%s.c",nom_image);
         }
         sprintf(nom_fic_canal,"%s1",nom_fic);
         retour = stat ( nom_fic_canal , &buf_stat );
         if ( retour != 0 )
         {
           sprintf(CAN,"C");
           if ((long int)strlen(repert)!= 0)
           {
               sprintf(nom_fic,"%s/%s.C",repert,nom_image);
            }
            else
            {
                sprintf(nom_fic,"%s.C",nom_image);
             }

         }
         else
           sprintf(CAN,"c");

        for (i=1;i<CAI_NB_MAX_CAN;i=i+1)
        {
          nom_fic_canal[0]='\0';
          sprintf(nom_fic_canal,"%s%d",nom_fic,i);
          num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
          if (num!= -1)
          {
            if (*nb_can<CAI_NB_MAX_CAN)
            {
           	sscanf(nom_image,"%s",deb);
	   	strcat(deb,CAN);
                sprintf(car,"%d",(*nb_can)+1);
           	strcat(deb,car);
  	   	deb=deb+100;
           	sscanf(nom_image,"%s",deb1);
	   	strcat(deb1,CAN);
           	strcat(deb1,car);
           	deb1=deb1+100;
             }
             image1->ACCES_CANAL_INT[*nb_can]=num;
             image1->POS_OCTET[*nb_can]=0;

             *nb_can=*nb_can+1;     
          }
         }
        if (*nb_can!=nbplan)
        {
       		sprintf(CAI_ERREUR,
		"incoherence au niveau du nombre de fichiers canaux dans %s",
		nom_fic);
 
       		iret=0;
         }
      }
     }
    }
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
   Fin de l'operation cai_ouvre_modifie_bsq
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
 char* BSQImageIO::cai_test_pFpf ()
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

void BSQImageIO::cai_inverser_tableau(short *BufL,short *BufE,int Nb)
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
int BSQImageIO::cai_istyp_connu(char *type_code)
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

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_En_majuscule                                           */
/* ----------								     */
/*                                                                           */
/* ROLE : Cette fonction transforme la chaine de caracteres donnee en        */
/* ROLE : parametre en majuscule et renvoie le resultat                      */
/*                                                                           */
/* CONTEXTE D'APPEL :                                                        */
/*                                                                           */
/* RETOUR : chaine en majuscule                                              */
/*                                                                           */
/*****************************************************************************/

char* BSQImageIO::cai_En_majuscule( char *mot )

{
   char *retour;
   int i = 0;
   
   /* allocation des pointeurs */
   
   retour = (char*) NULL;
   retour = (char*) calloc ( strlen ( mot ) + 1 , sizeof (char) );
   
   if ( retour == NULL )
   {
     return (char*)NULL;
   }
   /* Parcourir la chaine */
   
   while ( mot[i] != '\0' )
   {   
   /* transformer en majuscule la lettre lue */
      retour[i] = toupper ( mot[i] );
      i++;
   }
  /* retourner le resultat */ 
  return retour;   
}
/* 
   Fin de l'operation cai_En_majuscule
*/

  
} // end namespace otb

