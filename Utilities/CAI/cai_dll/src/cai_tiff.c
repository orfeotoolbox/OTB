/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_tiff.c                                                    */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format tiff                                       */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    fevrier 1995                                        */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*****************************************************************************/
/*****************************************************************************/
/**							      		    **/
/**    CE MODULE A ETE DEVELOPPE EN UTILISANT LA BIBLIOTHEQUE 		    **/
/**  D'ACCES AUX IMAGES TIFF DIPONIBLE EN FREE_WARE AVEC XV  		    **/
/**	libtiff.a version 3.0				      		    **/
/**							      		    **/
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* VERSION : V3.3                                                            */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 3.3: DM : optimisation des acces                          */
/*****************************************************************************/
/* MOD : VERSION :4.1 :FA: Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 :FA:Verification que le nombre d'octets par pixel est */
/* egal a 1 et le nombre de canaux est 1 ou 3. tiff est un format de         */
/* restitution							             */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
/* MOD : VERSION : 4.3 :FA: Suppression de la contrainte sur le nb d'octets  */
/* par pixel et du nombre de canaux                                          */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : DM : modification du nombre de lignes en ecriture   */
/* dans le TAG ROWPERSTRIP pour manipulation aussi bien pour chaque ligne,   */
/* tous les canaux qu pour chaque canal toutes les lignes                    */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : En ouverture en modification, ouverture en "a" */
/* au lieu de "w"                                                            */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.9 : DM :  Lecture d'une image sur 1 bit. Chaque bit est */
/*  expensé sur un octet avant d'être renvoyé à l'appelant                   */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM :  A l'ouverture d'une image en lecture, renvoie */
/* de COD_PIX à I4 ou R4 pour les images codees en I4 ou R4                  */
/*****************************************************************************/
/* MOD : VERSION : 5.0 : FA :  Image 1bit  :prise en compte de la lecture de */
/*  portion de lignes ou de sous-echantillonnage                             */
/*****************************************************************************/
/* MOD : VERSION : 5.1 : FA :  Image 1bit  :prise en compte de la lecture de */
/*  l'image en entier                                                        */
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Mise à jour du tag SAMPLEFORMAT lorsque       */
/* l'image est codée en reel  et que l'utilisateur le precise dans le param  */
/* "label"                                                                   */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
#include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>

#define _CAI_IMAGE
#define IN_CAI_TIFF

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_tiff.c,v 1.11 01/05/31 10:47:19 cmf Exp $";


extern char *getenv();

/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_ima_tiff                                         */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format tiff                   */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_tiff (repert,tab_ima)           */
/* -----------------                                                         */
/*                                                                           */
/*     repert        (E) (char *)   : repertoire a explorer		     */
/*     tab_ima       (S) (char ***) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_tiff      (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_ima_tiff (char *repert,
		       		char ***tab_ima)


{
   FILE *fp;		  /* descripteur de fichier. retour fopen        */

   struct dirent *dp;     /* structure resultat de la fonction readdir   */

   DIR *dirp;		  /* pointeur sur structure ouverture repertoire */
   char *pch0;	          /* pointeurs sur chaine de caracteres          */
   unsigned short magic;  /* nombre magique de reconnaissance tiff       */
   char nomfic[1024];     /* nom du fichier dont on verifie s'il est tiff*/
   int nb_ima;            /* nombre d'images tiff  identifiees           */
   int i;                 /* indice de boucle                            */
   char car;              /* variable de travail                         */    

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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE tiff
    lecture du nombre magique 
      ........................................*/

   while((dp=readdir(dirp))!=NULL)
   {
        nomfic[0]='\0';
        sscanf(repert,"%s",nomfic);
        strcat(nomfic,"/");
        strcat(nomfic,dp->d_name);
	fp=fopen(nomfic,"r");
        if (fp != NULL)
        {
         fread(&magic,2,1,fp);
         fclose(fp);
         if ((magic == TIFF_BIGENDIAN)||(magic== TIFF_LITTLEENDIAN))
          {
	   nb_ima=nb_ima+1;
          }
         }
     }

    (void)closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT BSQ
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne
   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE tiff
    lecture du nombre magique 
    si oui ,stockage dans la liste des images TIFF
      ........................................*/
   nb_ima=0;
   dirp=opendir(repert);

   while((dp=readdir(dirp))!=NULL)
   {

        nomfic[0]='\0';
        sscanf(repert,"%s",nomfic);
        strcat(nomfic,"/");
        strcat(nomfic,dp->d_name);
	fp=fopen(nomfic,"r");

        if (fp != NULL)

        {

         fread(&magic,2,1,fp);
         fclose(fp);
         if ((magic == TIFF_BIGENDIAN)||(magic== TIFF_LITTLEENDIAN))
          {

/*         suppression de l'extension */
           car='.';
           pch0=strrchr(dp->d_name,car);
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
           if ((pch0 != NULL)&&((*(pch0+1)=='t')||(*(pch0+1)=='T')))
           {
               *pch0 =  '\0';
               strcpy((*tab_ima)[nb_ima],dp->d_name);
	       nb_ima=nb_ima+1;
            }
          }
         }
     }

    (void)closedir(dirp);

   }
   else
   {
     /* Erreur:Repertoire inexistant */
       nb_ima=-1;
   }

    return(nb_ima);

}

/* Fin de l'operation cai_liste_ima_tiff
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_tiff                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format tiff      */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_lecture_tiff (nom_image,        */
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
/*     cai_ouvre_lecture_tiff    (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_tiff(char *repert,
				  char *nom_image,
				  int  *nb_can,
				  int  *nboct_pix,
				  int  *nb_col,
				  int  *nb_lig)

{
   TIFF *tif;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;	    	/* pointeur su descripteur de fichier image */ 
   int  iret,icr;		/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char Extension[6];           /* extension du fichier .tiff ou .tif       */
   int i,Coher;			/* indice 				    */
   unsigned int imagelength;    /* nombre de lignes de l'image              */
   unsigned int imagewidth;     /* nombre de colonnes de l'image	    */
   short samplesperpixel;       /* nombre de canaux			    */
   short compression;		/* indicateur de compression		    */
   short photo;			/* indicateur de photometric interpretation */
   short bitspersample;	        /* nombre de bits par pixels                */
   short sampleformat;		/* type de codage du pixel                  */
   int Istile;			/* indicateur de stockage en pave	    */
   int Mode;			/* mode test existence d'un fichier         */


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   iret=1;
   Mode = F_OK;
   icr= 0;
   sprintf(Extension,".tiff");
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

/*****************************************************************************/
/* MOD : VERSION :4.1 :FA:Rajout des tests sur les retours d'allocation     */
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
   Constitution du nom du fichier TIFF 
   .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.tiff",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.tiff",nom_image);
   }
/* test sur l'existence avec .tiff sinon .tif */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      icr = access(nom_fic,Mode);
      if (icr !=0)
      {
	nom_fic[strlen(nom_fic)-1]='\0';
        Extension[4]='\0';
        icr = access(nom_fic,Mode);
        if (icr !=0)
        {
   	    sprintf(Extension,".TIF");
   	    if ((long int)strlen(repert)!=0)
   	    {
      		sprintf(nom_fic,"%s/%s%s",repert,nom_image,Extension);
   	    }
   	    else
   	    {
      		sprintf(nom_fic,"%s.%s",nom_image,Extension);
   	    }
        }
      }


/*........................
    Ouverture du fichier entete
  ..........................................*/
    tif = TIFFOpen(nom_fic,"r");
    if (tif==NULL)
    {
      iret=0;
      strcpy(CAI_ERREUR,"Erreur ouverture fichier tiff");
     }
    else
    {
/*..........................................................
      verification si l'image n'est pas comprimee
.............................................................*/
      TIFFGetField(tif,TIFFTAG_COMPRESSION,&compression);
      if (compression != COMPRESSION_NONE)
      {  
        iret=0;
        strcpy(CAI_ERREUR,"Le fichier tiff est comprime. Format non traite !");
       }
       else
       {
          
/*..........................................................
        verification si le fichier n'est pas code en pave
.............................................................*/
        Istile=TIFFIsTiled(tif);
        if (Istile!=0)
         {  
          iret=0;
          strcpy(CAI_ERREUR,"Le fichier tiff est en paves . Format non traite !");
         }
         else
          {
/*....................................
	  recuperation du nombre de colonnes 
............................................*/
	  TIFFGetField(tif,TIFFTAG_IMAGEWIDTH,&imagewidth);
   	  *nb_col=imagewidth;
/*....................................
	  recuperation du nombre de lignes
............................................*/
          TIFFGetField(tif,TIFFTAG_IMAGELENGTH,&imagelength);
	  *nb_lig=imagelength;

/*......................................
	  recuperation du nombre de canaux
............................................*/
	  TIFFGetFieldDefaulted(tif,TIFFTAG_SAMPLESPERPIXEL,&samplesperpixel);
          *nb_can=samplesperpixel;
/*.........................................
	  recuperation du nombre d'octets par pixel
  .................................................*/
/* Nombre de bits par pixels */
/*****************************/
          TIFFGetField(tif,TIFFTAG_BITSPERSAMPLE,&bitspersample);
          if ((bitspersample %8)==0)
		*nboct_pix=(int)(bitspersample/8);
          else
		*nboct_pix=(int)((bitspersample/8)+1);
          image1->NB_BITS_PIX=bitspersample;

/*.........................................
	  recuperation du type de codage
  .................................................*/
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM :  A l'ouverture d'une image en lecture, renvoie */
/* de COD_PIX à I4 ou R4 pour les images codees en I4 ou R4                  */
/*****************************************************************************/
          TIFFGetField(tif,TIFFTAG_SAMPLEFORMAT,&sampleformat);

/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/
	  Coher = 1;
	  if (*nboct_pix==1)
          {
	    sprintf(image1->COD_PIX,"UOCT");
	    if (sampleformat == SAMPLEFORMAT_INT) 
		sprintf(image1->COD_PIX,"OCT");
	    else if (sampleformat != SAMPLEFORMAT_UINT) 
		Coher=0;
          }
	  else if (*nboct_pix==2) 
          {
	    sprintf(image1->COD_PIX,"UI2");
	    if (sampleformat == SAMPLEFORMAT_INT) 
		sprintf(image1->COD_PIX,"I2");
	    else if (sampleformat != SAMPLEFORMAT_UINT) 
		Coher=0;
          }
	  else if (*nboct_pix==4)
          {
	     sprintf(image1->COD_PIX,"I4");
	     if (sampleformat == SAMPLEFORMAT_IEEEFP) 
			sprintf(image1->COD_PIX,"R4");
              else if (sampleformat == SAMPLEFORMAT_UINT)
			sprintf(image1->COD_PIX,"UI4");
              else if  (sampleformat != SAMPLEFORMAT_INT)
		Coher=0;
          }
	  else if (*nboct_pix==8)
          {
	     sprintf(image1->COD_PIX,"R8");
	     if (sampleformat != SAMPLEFORMAT_IEEEFP) 
				Coher=0;
	  }
	  else
		 sprintf(image1->COD_PIX,"UND");

/*............................................*/
/* Recuperation du type de codages des pixels */
/*............................................*/
	TIFFGetField(tif,TIFFTAG_PLANARCONFIG,&photo);

/*...................................................
          recuperation de l'indice de photometric interpretation
 ..............................................................*/
	TIFFGetField(tif,TIFFTAG_PHOTOMETRIC,&photo);
        if ((photo<0)&&(photo>3))
         {
	  iret=0;
          strcpy(CAI_ERREUR,"Photometric non connue . Format non traite !");
         }
      
        }
       }
      }
     if (iret != 0)
     {

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

 
       deb = image1->NOM_FIC_CAN;
       deb1= image1->NOM_UTIL_CAN;

       for (i=0;i<*nb_can;i=i+1)
       {
	   sprintf(deb,"%s%s",nom_image,Extension);
  	   deb=deb+100;
	   sprintf(deb1,"%s%s",nom_image,Extension);
           deb1=deb1+100;
           image1->POS_OCTET[i]=0;
           image1->ACCES_CANAL_TIFF[i]=tif;
        }
      }
     }
     if (iret==0)
     {
    	return (NULL);
      }
      else  
      {   
        return(image1);
      }
  
}

/* 
   Fin de l'operation cai_ouvre_lecture_tiff
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_tiff                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format tiff et de tous les fichiers qui la composent         */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_creation_tiff (nom_image,       */
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
/*     cai_ouvre_creation_tiff    (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_IMAGE *cai_ouvre_creation_tiff(char *repert,
				   char *nom_image,
				   int  *nb_can,
				   int  *nboct_pix,
				   int  *nb_col,
				   int  *nb_lig,
				   char *label)

{
   TIFF *tif;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   int retour;			/* retour de fonction 			    */ 
   int i;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   unsigned int imagelength;    /* nombre de lignes de l'image              */
   unsigned int imagewidth;     /* nombre de colonnes de l'image	    */
   short samplesperpixel;       /* nombre de canaux			    */
   short photo;			/* indicateur de photometric interpretation */
   int  rowsperstrip;           /* taille totale de l'image en octets       */
   short bitspersample;		/* nombre de bits par pixels                */
   short sampleformat;		/* type de codage du pixel                  */
   int reconnaissance_type,Coher;

/*****************************************************************************/
/* MOD : VERSION : 4.3 :FA: Suppression de la contrainte sur le nb d'octets  */
/* par pixel et du nombre de canaux                                          */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.2 :FA:Verification que le nombre d'octets par pixel est */
/* egal a 1 et le nombre de canaux est 1 ou 3. tiff est un format de         */
/* restitution							             */
/*****************************************************************************/
/* Verification sur le nombre d'octets par pixel */

 /*  if (*nboct_pix !=1)
   {
     strcpy(CAI_ERREUR,"Erreur une image TIFF ne peut etre codee que sur un seul octet");
     goto ERREUR;
   }
*/
/* Verification sur le nombre de canaux */
/*   if ((*nb_can !=1)&&(*nb_can !=3))
   {
     strcpy(CAI_ERREUR,"Erreur une image TIFF a 1 ou 3 canaux exclusivement");
     goto ERREUR;
   }
*/

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION :4.1 :FA:Rajout des tests sur les retours d'allocation     */
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
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.tif",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.tif",nom_image);
   }
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s TIFF existe \n" , nom_image );
	goto ERREUR;
   }

/*************************************************/
/* Ouverture du fichier 			 */ 
/*************************************************/
tif = TIFFOpen ( nom_fic , "w");
if ( tif == NULL )
{
	sprintf ( CAI_ERREUR , "Erreur : ouverture fichier %s impossible \n" ,
	nom_fic );
	goto ERREUR ;
}
/*************************************************/
/* Ecriture de l'enregistrement d'entete         */
/*************************************************/
/* Nombre de colonnes */
/**********************/
imagewidth=(unsigned int)(*nb_col);
TIFFSetField(tif,TIFFTAG_IMAGEWIDTH,imagewidth);

/* Nombre de lignes */
/********************/
imagelength=(unsigned int)(*nb_lig);
TIFFSetField(tif,TIFFTAG_IMAGELENGTH,imagelength);

/*****************************************************************************/
/* MOD : VERSION : 4.5 : DM : modification du nombre de lignes en ecriture   */
/* dans le TAG ROWPERSTRIP pour manipulation aussi bien pour chaque ligne,   */
/* tous les canaux qu pour chaque canal toutes les lignes                    */
/*****************************************************************************/

/* Nombre de lignes par strip */
/******************************/
/*rowsperstrip=(int)((*nb_lig));*/
rowsperstrip = 1;
TIFFSetField(tif,TIFFTAG_ROWSPERSTRIP,rowsperstrip);

/* Nombre de canaux */
/********************/
samplesperpixel=(short)(*nb_can);
TIFFSetField(tif,TIFFTAG_SAMPLESPERPIXEL,samplesperpixel);

/* Indicateur de non compression */
/*********************************/
TIFFSetField(tif,TIFFTAG_COMPRESSION,COMPRESSION_NONE);

/* Nombre de bits par pixels */
/*****************************/
bitspersample=(short)(*nboct_pix);
bitspersample=bitspersample*8;
TIFFSetField(tif,TIFFTAG_BITSPERSAMPLE,bitspersample);

/* Type de codage du pixel */
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Mise à jour du tag SAMPLEFORMAT lorsque       */
/* l'image est codée en reel  et que l'utilisateur le precise dans le param  */
/* "label"                                                                   */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 5.3 : DM :   Correction erreur lorsque label non reconnu */
/*****************************************************************************/

/*****************************************************************************/
/* Verification si type de codage reconnu dans le label */
  reconnaissance_type = cai_istyp_connu(label);
  if (reconnaissance_type == -1)
  {
     if (*nboct_pix == 1) sampleformat=SAMPLEFORMAT_UINT;
     else if (*nboct_pix==2) sampleformat=SAMPLEFORMAT_INT;
     else if ((*nboct_pix==4) && ((strcasecmp(label,"R4") == 0)||
				(strcasecmp(label,"FLOAT")== 0)||
				(strcasecmp(label,"REEL") == 0)))
                                   sampleformat = SAMPLEFORMAT_IEEEFP;
     else sampleformat=SAMPLEFORMAT_VOID;
  }
  else
  {

/* Verification de coherence entre le nb d'octets par pixel et le type donne */
     Coher=1;
     if(*nboct_pix == 1) 
     {
	sampleformat=SAMPLEFORMAT_UINT;
	if (reconnaissance_type == 0)
 	   sampleformat=SAMPLEFORMAT_INT;
	else if (reconnaissance_type !=1)
          Coher=0;  
     }    
     else if(*nboct_pix == 2) 
     {
	sampleformat=SAMPLEFORMAT_INT;
	if (reconnaissance_type == 3)
 	   sampleformat=SAMPLEFORMAT_UINT;
	else if (reconnaissance_type !=2)
          Coher=0;  
     }
     else if(*nboct_pix == 4) 
     {
	sampleformat=SAMPLEFORMAT_IEEEFP;
	if (reconnaissance_type == 4)
 	   sampleformat=SAMPLEFORMAT_INT;
	else if (reconnaissance_type == 5)
 	   sampleformat=SAMPLEFORMAT_UINT;
	else if (reconnaissance_type !=6)
          Coher=0; 
     } 
     else if(*nboct_pix == 8) 
     {
	sampleformat=SAMPLEFORMAT_IEEEFP;
	if (reconnaissance_type != 7)  Coher=0;
     }
     else 
	   Coher = 0;
     if (Coher == 0) 
     {      	
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
      }    
      else
      {
     		sprintf ( image1->COD_PIX , "%s" ,label);
       }
   }
  
/* 
  FIN MOD 5.1 */
/**************/
TIFFSetField(tif,TIFFTAG_SAMPLEFORMAT,sampleformat);

/*indicateur de photometric suivant le nombre de canaux */
if (*nb_can==1)
 {
   photo=1; 
   TIFFSetField(tif,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG);

  }
else
  {
   photo=2;
   TIFFSetField(tif,TIFFTAG_PLANARCONFIG,PLANARCONFIG_SEPARATE);

   }
TIFFSetField(tif,TIFFTAG_PHOTOMETRIC,photo);
  for (i=0;i<*nb_can;i=i+1)
  {
    image1->ACCES_CANAL_TIFF[i]=tif;
    image1->POS_OCTET[i]=0;
   }
  return (image1);

ERREUR :
  cai_dest_ima_tiff ( repert , nom_image );
  return(NULL);
}
 /* 
  Fin de l'operation cai_ouvre_creation_tiff
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_tiff                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image tiff                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_tiff (image1, canal,      */       		      /* ------------------                              premiere_ligne,           */
/*                                          	   premiere_colonne,         */
/*                                          	   nombre_lignes_lire ,      */
/*                                          	   nombre_colonnes_lire,     */
/*                                           	   pas_ligne ,pas_colonne,   */
/*                 		             	   data_image )              */
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
/*     cai_lecture_canal_tiff    (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.9 : DM :  Lecture d'une image sur 1 bit. Chaque bit est */
/*  expensé sur un octet avant d'être renvoyé à l'appelant                   */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_tiff(CAI_IMAGE *image1 ,  
                          	 int *canal ,           
    		          	 int *premiere_ligne  ,   
     		          	 int *premiere_colonne,
                           	 int *nombre_lignes_lire ,
                           	 int *nombre_colonnes_lire,
                          	 int *pas_ligne ,          
                           	 int *pas_colonne,
		           	 unsigned char *data_image )        

{
 TIFF *tif;		    /* descripteur de fichier , retour de TIFFOpen   */
 int num,i,j,k,val;         /* variables de travail                          */
 int mul,decal;             /* variables de travail                          */
 int iret;                  /* indicateur d'erreur                           */
 long lig;                  /* indice de boucle sur les lignes               */
 long  nb_oct;		    /* nb oct =1er octet a lire - position courante  */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 int   oct_pix;             /* nb octets par pixels                          */
 unsigned char val1;
 unsigned char Masque[8]={1,2,4,8,16,32,64,128};
 int temp1,temp2,temp3;

/*...........................
  Initialisations diverses
  ..........................*/

   iret=CAI_OK;
   strcpy(CAI_ERREUR,"        ");

   tif=image1->ACCES_CANAL_TIFF[0];
   oct_pix = image1->NBOCT_PIX;
   pch=data_image;

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
   nb_oct=TIFFScanlineSize(tif);
   if ((nb_oct==(image1->NB_COL)*(image1->NB_CANAUX))&&(image1->NB_CANAUX!=1))
   {
/* ligne multispectrale */
      num=0;
      val=(*canal-1);
      mul=image1->NB_CANAUX;
    }
   else
   {
/* ligne monospectrale */
      num=*canal-1;
      mul=1;
      val=0; 
   }
   buf_lec = (unsigned char *) malloc(nb_oct);
   if (buf_lec == NULL)
   {
      iret=CAI_KO;
      strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
   }
   else
   {

/*....................
        Initialisation
  ......................*/
        lig=(long)(*premiere_ligne); 
	lig=lig-1;   
        i=0;
        if ( image1->NB_BITS_PIX!=1)
        {
          while (i<(*nombre_lignes_lire))
            {
	       TIFFReadScanline(tif,buf_lec,lig,num);
/*......................................
               Sauvegarde des donnees lues entenant compte du pas 
               d'echantillonnage en colonne
  ........................................*/    
               decal= ((*premiere_colonne-1)*mul*oct_pix)+val;   
               deb=buf_lec+decal;
               j=1;
               while (j <= (*nombre_colonnes_lire))
	       {  
                  for (k=0;k<oct_pix;k=k+1)
                  {
                 	 *(pch+k)=*(deb+k);
                  }
                  pch=pch+oct_pix;
                  j=j+1;
                  deb=deb+((*pas_colonne)*oct_pix*mul);   
               }   
               i=i+1;
               lig=lig+(long)(*pas_ligne);

            } 
        }
        else  
        {
/*****************************************************************************/
/* MOD : VERSION : 4.9 : DM :  Lecture d'une image sur 1 bit. Chaque bit est */
/*  expensé sur un octet avant d'être renvoyé à l'appelant                   */
/*****************************************************************************/

          while (i<(*nombre_lignes_lire))
            {
	       TIFFReadScanline(tif,buf_lec,lig,num);
/*......................................
               Sauvegarde des donnees lues entenant compte du pas 
               d'echantillonnage en colonne
  ........................................*/   
/*****************************************************************************/
/* MOD : VERSION : 5.0 : FA :  Image 1bit  :prise en compte de la lecture de */
/*  portion de lignes ou de sous-echantillonnage                             */
/*****************************************************************************/
/* se déplacer jusqu'au premier paquet octet concerné */
               decal= (*premiere_colonne-1)/8;           
               deb=buf_lec+decal;
               j=0;
               while (j < (*nombre_colonnes_lire))
	       {  
		  val1 = *(deb);
		  if (j==0)
                  {
/* se déplacer jusqu'à la première colonne concernée */
                      k = (*premiere_colonne -1) - (decal*8);
                  }
		  else
		    k=k-8;
                  while ((k<8)&&(j <(*nombre_colonnes_lire)))
                  {
                       *(pch+j) = val1 & Masque[7-k];
                        if (*(pch+j) != 0) *(pch+j)=1;
			k=k+(*pas_colonne);
                        j=j+1;
                  }
                  deb=deb+1;   
               }   
/*****************************************************************************/
/* MOD : VERSION : 5.1 : FA :  Image 1bit  :prise en compte de la lecture de */
/*  l'image en entier                                                        */
/*****************************************************************************/
               pch=pch+(*nombre_colonnes_lire);
/**********/
/*FIN MOD */
/**********/
               i=i+1;
               lig=lig+(long)(*pas_ligne);

            } 
        }
 
    	free(buf_lec);
   }

   return(iret);
}
/*
  Fin de l'operation cai_lecture_canal_tiff
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_tiff                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image tiff                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_OK_KO cai_ecriture_canal_tiff (image1, canal,      */       		      /* ------------------                                 premiere_ligne,        */
/*                                                    nombre_lignes_ecrire,  */
/*                                                    nombre_colonnes_ecrire,*/
/*                 		                      data_image )           */
/*									     */
/*     image1      (E) (CAI_IMAGE *)   : pointeur sur descripteur de fichier */
/*     canal                 (E) (int) : numero du canal a ecrire            */
/*     premiere_ligne        (E) (int) : premiere ligne a ecrire ds l'image  */
/*     nombre_lignes_ecrire  (E) (int) : nombre de lignes a ecrire           */
/*     nombre_colonnes_ecrire(E) (int) : nombre de lignes a ecrire           */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels ecris alloue et   */
/*                                      libere par l'appelant                */
/*									     */
/*     cai_ecriture_canal_tiff       (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_ecriture_canal_tiff(CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image ) 
{
int nboct_pix;			/* nombre d'octets par pixel              */
unsigned int row,sample;	/* variables intermediaires               */
int	i_ligne,lig;		/* compteur de lignes			  */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire   */
TIFF *tif;			/* descriteur de fichier,issu de TIFFopen */
int icr;

/* ................
   Initialisations 
  .................*/

i_ligne = 0;
lig=(*premiere_ligne);
tif = image1->ACCES_CANAL_TIFF[(*canal)-1];
nboct_pix = image1->NBOCT_PIX;
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
	row=(unsigned int)(lig);
	row=row-1;
	sample=(unsigned int)(*canal);
	sample=sample-1;
        icr=TIFFWriteScanline(tif,buff_lig,row,sample);
/********************************************************/
/*	incrementation du nombre de lignes		*/
/********************************************************/
        lig=lig+1;
	i_ligne++;

/********************************************************/
/* FIN TANT QUE						*/
/********************************************************/
}
return (CAI_OK);


}
/* 
  Fin de l'operation cai_ecriture_canal_tiff
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_tiff                                             */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format tiff et de          */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_tiff (image1)                     */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_tiff       (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ferme_tiff (CAI_IMAGE *image1)


{
 int iret;       /* code retour de l'operation      */
 TIFF *num;	/* descripteur du fichier a fermer */


 iret=CAI_OK;
/*.............................................
   Fermeture de tous les canaux qui sont tous contenus
   dans un seul fichier .srf
  .............................................*/

   num=image1->ACCES_CANAL_TIFF[0];
   TIFFClose(num);

  return (iret);
}
/*
  Fin de l'operation cai_ferme_tiff
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_tiff                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en onera et de       */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_tiff (format,nom_ima)          */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_tiff    (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_dest_ima_tiff ( char *repertoire , 
                       	      char *nom_ima )

{

char	nom_fichier[1024];	/* nom du fichier entete a detruire 	     */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image               */
int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier     */
int     Mode,icr; 


/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/
if ((long)strlen(repertoire)!= 0)
{
  sprintf ( nom_ima_complet, "%s/%s" ,repertoire,nom_ima);
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}
sprintf ( nom_fichier , "%s.tiff",nom_ima_complet );

/* test sur l'existence avec .tiff sinon .tif */
 Mode = F_OK;
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
 icr = access(nom_fichier,Mode);
 if (icr !=0)
 {
	nom_fichier[strlen(nom_fichier)-1]='\0';
        icr = access(nom_fichier,Mode);
        if (icr !=0)
        {
   	    if ((long int)strlen(repertoire)!=0)
   	    {
      		sprintf(nom_fichier,"%s/%s.TIF",repertoire,nom_ima_complet);
   	    }
   	    else
   	    {
      		sprintf(nom_fichier,"%s.TIF",nom_ima_complet);
   	    }
        }
 }

/************************************************/
/* Destruction du fichier TIFF   		*/
/************************************************/
cr_stat = stat ( nom_fichier , &buf_stat );
if ( cr_stat == 0 )
{
	unlink ( nom_fichier );
	return (CAI_OK);
}
else
{
   return (CAI_KO);
}


}
/*
 Fin de l'operation cai_dest_ima_tiff
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_tiff                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au forma   */
/* -----     tiff et de tous les fichiers qui la composent                   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_modifie_tiff (nom_image,        */
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
/*     cai_ouvre_modifie_tiff    (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_tiff(char *repert,
				  char *nom_image,
				  int  *nb_can,
				  int  *nboct_pix,
				  int  *nb_col,
				  int  *nb_lig)

{
   TIFF *tif;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;	    	/* pointeur su descripteur de fichier image */ 
   int  iret,icr;		/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int i,Coher;			/* indice 				    */
   unsigned int  imagelength;   /* nombre de lignes de l'image              */
   unsigned int  imagewidth;    /* nombre de colonnes de l'image	    */
   short samplesperpixel;       /* nombre de canaux			    */
   short compression;		/* indicateur de compression		    */
   short photo;			/* indicateur de photometric interpretation */
   short bitspersample;	        /* nombre de bits par pixels                */
   int Istile;			/* indicateur de stockage en pave	    */
   short sampleformat;		/* type de codage du pixel                  */
   int Mode;
   char Extension[6];

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   iret=1;
   Mode = F_OK;
   icr = 0;
   sprintf(Extension,".tiff");

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION :4.1 :FA:Rajout des tests sur les retours d'allocation     */
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
   Constitution du nom du fichier TIFF 
   .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
	   sprintf(nom_fic,"%s%s.tiff",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.tiff",nom_image);
   }

/* test sur l'existence avec .tiff sinon .tif */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      icr = access(nom_fic,Mode);
      if (icr !=0)
      {
	nom_fic[strlen(nom_fic)-1]='\0';
        Extension[4]='\0';
        icr = access(nom_fic,Mode);
        if (icr !=0)
        {
   	    sprintf(Extension,".TIF");
   	    if ((long int)strlen(repert)!=0)
   	    {
      		sprintf(nom_fic,"%s/%s%s",repert,nom_image,Extension);
   	    }
   	    else
   	    {
      		sprintf(nom_fic,"%s.%s",nom_image,Extension);
   	    }
        }
      }

/*........................
    Ouverture du fichier entete
  ..........................................*/
/******************************************************************************/
/* MOD : VERSION : 4.6 : FA : En ouverture en modification, ouverture en "r+" */
/* au lieu de "w"                                                             */
/******************************************************************************/

    tif = TIFFOpen(nom_fic,"r+");
    if (tif==NULL)
    {
      iret=0;
      strcpy(CAI_ERREUR,"Erreur ouverture fichier tiff");
     }
    else
    {
/*..........................................................
      verification si l'image n'est pas comprimee
.............................................................*/
      TIFFGetField(tif,TIFFTAG_COMPRESSION,&compression);
      if (compression != COMPRESSION_NONE)
      {  
        iret=0;
        strcpy(CAI_ERREUR,"Le fichier tiff est comprime. Format non traite !");
       }
       else
       {
          
/*..........................................................
        verification si le fichier n'est pas code en pave
.............................................................*/
        Istile=TIFFIsTiled(tif);
        if (Istile!=0)
         {  
          iret=0;
          strcpy(CAI_ERREUR,"Le fichier tiff est en paves . Format non traite !");
         }
         else
          {
/*....................................
	  recuperation du nombre de colonnes 
............................................*/
	  TIFFGetField(tif,TIFFTAG_IMAGEWIDTH,&imagewidth);
   	  *nb_col=imagewidth;
/*....................................
	  recuperation du nombre de lignes
............................................*/
          TIFFGetField(tif,TIFFTAG_IMAGELENGTH,&imagelength);
	  *nb_lig=imagelength;

/*......................................
	  recuperation du nombre de canaux
............................................*/
	  TIFFGetFieldDefaulted(tif,TIFFTAG_SAMPLESPERPIXEL,&samplesperpixel);
          *nb_can=samplesperpixel;
/*.........................................
	  recuperation du nombre d'octets par pixel
  .................................................*/
/* Nombre de bits par pixels */
/*****************************/
          TIFFGetField(tif,TIFFTAG_BITSPERSAMPLE,&bitspersample);
          if ((bitspersample %8)==0)
		*nboct_pix=(int)(bitspersample/8);
          else
		*nboct_pix=(int)((bitspersample/8)+1);
 /*.........................................
	  recuperation du type de codage
  .................................................*/
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM :  A l'ouverture d'une image en lecture, renvoie */
/* de COD_PIX à I4 ou R4 pour les images codees en I4 ou R4                  */
/*****************************************************************************/
          TIFFGetField(tif,TIFFTAG_SAMPLEFORMAT,&sampleformat);

/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/
	  Coher = 1;
	  if (*nboct_pix==1)
          {
	    sprintf(image1->COD_PIX,"UOCT");
	    if (sampleformat == SAMPLEFORMAT_INT) 
		sprintf(image1->COD_PIX,"OCT");
	    else if (sampleformat != SAMPLEFORMAT_UINT) 
		Coher=0;
          }
	  else if (*nboct_pix==2) 
          {
	    sprintf(image1->COD_PIX,"UI2");
	    if (sampleformat == SAMPLEFORMAT_INT) 
		sprintf(image1->COD_PIX,"I2");
	    else if (sampleformat != SAMPLEFORMAT_UINT) 
		Coher=0;
          }
	  else if (*nboct_pix==4)
          {
	     sprintf(image1->COD_PIX,"I4");
	     if (sampleformat == SAMPLEFORMAT_IEEEFP) 
			sprintf(image1->COD_PIX,"R4");
              else if (sampleformat == SAMPLEFORMAT_UINT)
			sprintf(image1->COD_PIX,"UI4");
              else if  (sampleformat != SAMPLEFORMAT_INT)
		Coher=0;
          }
	  else if (*nboct_pix==8)
          {
	     sprintf(image1->COD_PIX,"R8");
	     if (sampleformat != SAMPLEFORMAT_IEEEFP) 
				Coher=0;
	  }
	  else
		 sprintf(image1->COD_PIX,"UND");

/*............................................*/
/* Recuperation du type de codages des pixels */
/*............................................*/
	TIFFGetField(tif,TIFFTAG_PLANARCONFIG,&photo);

/*...................................................
          recuperation de l'indice de photometric interpretation
 ..............................................................*/
	TIFFGetField(tif,TIFFTAG_PHOTOMETRIC,&photo);
        if ((photo<0)&&(photo>3))
         {
	  iret=0;
          strcpy(CAI_ERREUR,"Photometric non connue . Format non traite !");
         }
      
        }
       }
      }
     }
     if (iret != 0)
     {

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

 
       deb = image1->NOM_FIC_CAN;
       deb1= image1->NOM_UTIL_CAN;

       for (i=0;i<*nb_can;i=i+1)
       {
   	   sprintf(deb,"%s%s",nom_image,Extension);
  	   deb=deb+100;
  	   sprintf(deb1,"%s%s",nom_image,Extension);
           deb1=deb1+100;
           image1->POS_OCTET[i]=0;
           image1->ACCES_CANAL_TIFF[i]=tif;
        }
    	return (image1);
      }
      else  
      {   
        return(NULL);
      }
  
}

/* 
   Fin de l'operation cai_ouvre_modifie_tiff
*/
