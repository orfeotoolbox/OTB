/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_fonctions.c                                               */
/* -------								     */
/*                                                                           */
/* ROLE   :    contient l'interface entre l'utilisateur et la Couche Acces   */
/* -------     Image, avec les fonctionnalites de premier niveau             */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    fevrier 1995                                        */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V2                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 2 : DM : optimisation				     */
/*****************************************************************************/
/* MOD : VERSION : 4.0 : DM : integration du format GESSIMU (sept 1999)      */
/* MOD : VERSION : 4.0 : DM : integration du format GRILLES (sept 1999)      */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : integration des formats SPOT1A4 et SPOT5       */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modification lecture des repertoires pour PC   */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : suppression du format multiscope, modif SPOT5  */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : DM : integration du format RADAR_CNES               */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA  : avertissement des formats non disponibles     */
/*                             en modification                               */
/*****************************************************************************/
/* MOD : VERSION : 4.8 : DM:  Sous Cygwin separateur='/' ou '\'              */
/*****************************************************************************/
/* MOD : VERSION : 5.3 : DM :  prise en compte du format bandeau             */
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define _CAI_IMAGE
#define IN_CAI_FONC

#include "cai_image.h"

 char IND_GRILLES;    /* indicateur si image ou grille */ 
 char IND_SPOT;       /* indicateur si SPOT1A4 (1),SPOT5BIL (B),SPOT5TIF(T) */
/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_fonctions.c,v 1.13 01/02/15 14:24:58 cmf Exp $";


/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_format                                           */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des formats reconnus par la Couche Image    */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_format(tab_format,tab_suffixe)      */
/* -----------------                                                         */
/*                                                                           */
/*     tab_format[]   (S) (char **) : pointeur sur tableau des formats connus*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*     tab_suffixe[]  (S) (char **) : pointeur sur le tableau des suffixes   */
/*				      des formats reconnus : 1 pour 1 canal  */
/*				           2 pour plusieurs canaux           */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_format(S) (long int)     :   nombre de formats reconnus par CAI  */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_format(char ***tab_format,char **(tab_suffixe[2]))                  

{ 
long int nb;	        /* code retour de la fonction cai_liste_format  */
long int i,j;

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES FORMATS DISPONIBLES
     ET LA LISTE DES SUFFIXES CORRESPONDANT
     ......................................*/
    nb=CAI_NB_FORMAT;
    *tab_format  = (char **) calloc(nb,sizeof(char *));
    tab_suffixe[0] = (char **) calloc(nb,sizeof(char *));
    tab_suffixe[1] = (char **) calloc(nb,sizeof(char *));




    for (i=0;i<nb;i++)
    {
      (*tab_format)[i]=(char *)calloc(1,20);
      strcpy((*tab_format)[i],CAI_NOM_FORMAT[i]);
      for (j=0;j<2;j++)
      {
        tab_suffixe[j][i]=(char *)calloc(1,8);
        strcpy(tab_suffixe[j][i],CAI_SUF_FORMAT[j][i]);
	}

      }
return(nb);

}
/* Fin de l'operation cai_liste_format
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_image                                            */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images d'un format donne                */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_image(repert,type_image,tab_ima)    */
/* -----------------                                                         */
/*                                                                           */
/*     repert         (E) (char *)  : repertoire a explorer		     */
/*     type_image     (E) (char *)  : format des images a lister             */
/*     tab_ima[]      (S) (char **) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_image(S) (long int)     : =-1 repertoire inexistant	     */
/*                                    =-2 format non pris en compte          */
/*                                    =>0 nombre d'images listees            */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_image(char *repert,
                          char *type_image,
                          char ***tab_ima)
                  
 
{
 long int nb;	        /* code retour de la fonction cai_liste_image  */
 long int (*comp) ( );  /* pointeur sur fonction                       */
 DIR *dirp;		/* pointeur sur structure ouverture repertoire */ 

/*....................
   Initialisations 
  ..................*/
  nb=0;
  CAI_ERREUR[0]='\0';
  IND_GRILLES = '\0';
  IND_SPOT = '\0';
/*..............................................
  Verification existence repertoire demande
  ...............................................*/
  dirp=opendir(repert);
  if (dirp ==0)
  {
    /* repertoire demande inexistant */
     nb=-1;
    sprintf(CAI_ERREUR,"repertoire %s inexistant \n",repert);

  }
  else
  {
/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */

   if (strcasecmp(type_image,"INRIA") == 0)
   {
      comp = (cai_liste_ima_inria) ;
    }

   else if (strcasecmp(type_image,"ONERA") == 0)
   {
      comp = (cai_liste_ima_onera) ;
    }

   else if (strcasecmp(type_image,"BSQ") == 0)
   {
      comp = (cai_liste_ima_bsq) ;
    }

   else if (strcasecmp(type_image,"BIP") == 0)
   {
      comp = (cai_liste_ima_bip) ;
    }
#ifdef _HDF16
   else if (strncasecmp(type_image,"HDF",3) == 0)
   {
      comp = (cai_liste_ima_hdf16) ;
    }
#endif

   else if (strcasecmp(type_image,"BIL") == 0)
   {
      comp = (cai_liste_ima_bil) ;
    }

   else if (strcasecmp(type_image,"LUM") == 0)
   {
      comp = (cai_liste_ima_lum) ;
    }

   else if (strcasecmp(type_image,"GESSIMU") == 0)
   {
      comp = (cai_liste_ima_gessimu) ;
    }

   else if (strcasecmp(type_image,"GDB") == 0)
   {
      IND_GRILLES = 'B';
      comp = (cai_liste_ima_grilles) ;
    }

   else if (strcasecmp(type_image,"GDR") == 0)
   {
      IND_GRILLES = 'R';
      comp = (cai_liste_ima_grilles) ;
    }

   else if (strcasecmp(type_image,"TIFF") == 0)
   {
      comp = (cai_liste_ima_tiff) ;
    }

   else if (strcasecmp(type_image,"RASTERFILE") == 0)
   {
      comp = (cai_liste_ima_sun_raster) ;
    }
   else if (strcasecmp(type_image,"JPEG") == 0)
   {
      comp = (cai_liste_ima_jpeg) ;
    }
   else if (strcasecmp(type_image,"SPOT1A4") == 0)
   {
      IND_SPOT = '1';
      comp = (cai_liste_ima_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5TIF") == 0)
   {
      IND_SPOT = 'T';
      comp = (cai_liste_ima_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5BIL") == 0)
   {
      IND_SPOT = 'B';
      comp = (cai_liste_ima_spot) ;
    }
   else if (strcasecmp(type_image,"RADAR") == 0)
   {
      comp = (cai_liste_ima_radar) ;
    }
   else if (strcasecmp(type_image,"BANDEAU") == 0)
   {
      comp = (cai_liste_ima_bandeau) ;
    }

   else 
    { 
 
   /* format non connu de la Couche Acces Image */
    nb = -2;
    sprintf(CAI_ERREUR,"format %s non reconnu \n",type_image);

   }

   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 
	     repertoire de stockage des images
             pointeur sur tableau contenant la liste des images a lister
  ......................................................................*/
   closedir(dirp);
   if ( nb == 0 )
   { 
     if (IND_GRILLES == '\0')
     {
        if (IND_SPOT == '\0')
	        nb = (*comp)(repert,tab_ima);
	else
	        nb = (*comp)(IND_SPOT,repert,tab_ima);
      }
      else
	nb = (*comp)(IND_GRILLES,repert,tab_ima);
    }
  }
   IND_GRILLES = '\0';
   IND_SPOT    = '\0';
   return (nb);

}
/* Fin de l'operation cai_liste_image
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_image                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image d'un format donne   */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_image (nom_image,        */
/*  ---------------- 		                           type_image,       */
/*                 		                           nb_can,           */
/*				                           nboct_pix,	     */	
/*				                           nb_col,           */
/*				                           nb_lig)           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     type_image     (E) (char *) : format de l'image a ouvrir              */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_image (S) (CAI_IMAGE *)    : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_image (char *nom_image,
                  		    char *type_image,
                 		    int  *nb_can,
				    int  *nboct_pix,		
				    int  *nb_col,
				    int  *nb_lig)                  

{

 int nb;             	  /* indicateur d'erreurs rencontres              */
 CAI_IMAGE *image1;       /* descripteur de fichier retour fonction       */
 CAI_IMAGE *(*comp) ( );  /* pointeur sur fonction                        */
 char repert[1024];       /* repertoire de stockage de l'image            */
 char image[1024];        /* nom de l'image a ouvrir en lecture           */
 char format[20];         /* format de stockage de l'image a lire         */
 long int i,ind;          /* variables de travail                         */
 char *pch,*deb;          /* pointeurs de travail                         */
 long int nb1,nb2;	  /* indice de longueur significative             */
 char car;		  /* variable de travail                          */
 DIR *dirp;		  /* pointeur sur structure ouverture repertoire  */
 CAI_OK_KO icr;           /* Compte-rendu des fonctions DA                */

/*......................
  Initialisations
  .......................*/
  nb=1;
  strcpy(format,type_image);
  CAI_ERREUR[0]='\0';
  IND_GRILLES = '\0';
  IND_SPOT = '\0';

/*...................................................
   Recherche du type de fichier si reconnaissance automatique demande
  ...................................................*/
  if (strcasecmp(format,"AUTO")== 0)
  {
     for( i = 0 ; i< CAI_NB_EXTENSIONS ; i++)
      {
       if(strstr(nom_image,CAI_SUFFIXES[i]) != NULL )
       {
          ind=CAI_NUMEROS[i];
          strcpy(format,CAI_NOM_FORMAT[ind]);
       }
    }
  }
 

/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */

   if (strcasecmp(format,"INRIA") == 0)
   {
      comp = (cai_ouvre_lecture_inria) ;
    }

   else if (strcasecmp(format,"ONERA") == 0)
   {
      comp = (cai_ouvre_lecture_onera) ;
    }

   else if (strcasecmp(format,"BSQ") == 0)
   {
      comp = (cai_ouvre_lecture_bsq) ;
    }

   else if (strcasecmp(format,"BIP") == 0)
   {
      comp = (cai_ouvre_lecture_bip) ;
    }
#ifdef _HDF16
   else if (strncasecmp(format,"HDF",3) == 0)
   {
      comp = (cai_ouvre_lecture_hdf16) ;
    }
#endif

   else if (strcasecmp(format,"BIL") == 0)
   {
      comp = (cai_ouvre_lecture_bil) ;
    }

   else if (strcasecmp(format,"LUM") == 0)
   {
      comp = (cai_ouvre_lecture_lum) ;
    }

   else if (strcasecmp(format,"GESSIMU") == 0)
   {
      comp = (cai_ouvre_lecture_gessimu) ;
    }

   else if (strcasecmp(format,"GDB") == 0)
   {
      IND_GRILLES = 'B';
      comp = (cai_ouvre_lecture_grilles) ;
    }

   else if (strcasecmp(format,"GDR") == 0)
   {
      IND_GRILLES = 'R';
      comp = (cai_ouvre_lecture_grilles) ;
    }

   else if (strcasecmp(format,"TIFF") == 0)
   {
      comp = (cai_ouvre_lecture_tiff) ;
    }

   else if (strcasecmp(format,"RASTERFILE") == 0)
   {
      comp = (cai_ouvre_lecture_sun_raster) ;
    }
   else if (strcasecmp(format,"JPEG") == 0)
   {
      comp = (cai_ouvre_lecture_jpeg) ;
    }

   else if (strcasecmp(type_image,"SPOT1A4") == 0)
   {
      IND_SPOT = '1';
      comp = (cai_ouvre_lecture_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5TIF") == 0)
   {
      IND_SPOT = 'T';
      comp = (cai_ouvre_lecture_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5BIL") == 0)
   {
      IND_SPOT = 'B';
      comp = (cai_ouvre_lecture_spot) ;
    }
  else if (strcasecmp(format,"RADAR") == 0)
   {
      comp = (cai_ouvre_lecture_radar) ;
    }
  else if (strcasecmp(format,"BANDEAU") == 0)
   {
      comp = (cai_ouvre_lecture_bandeau) ;
    }

   else 
    { 
 
   /* format non connu de la Couche Acces Image */
     nb = 0;
     sprintf(CAI_ERREUR,"format %s non reconnu \n",format);

   }
   /*...............................................................
      Separation du repertoire de stockage de l'image et du nom propre
     .................................................................*/
    if (nb == 1)
    {
      repert[0]='\0';
      image[0]='\0';
      car='/';

#ifdef _WIN
      car='\\';
#endif
      pch=(char *)strrchr(nom_image,car);
      if (pch == NULL) 
      {
         /* le repertoire est celui courant */
           strcpy(repert,"./");
#ifdef _WIN
           strcpy(repert,".\\");
#endif
           sscanf(nom_image,"%s",image);
/*----------------------------------------------------------*/
/* Modification septembre 98 demandee par H.Vadon           */
/* Afin de permettre d'utiliser une image monocanale en BSQ */
/* avec l'extension libre sans .c1 forcement		    */
/*----------------------------------------------------------*/
	   if (strcasecmp(format,"BSQ") != 0)
           {
           	car='.';
           	pch=(char *)strrchr(image,car);
           	if (pch != NULL)
               		*pch='\0'; 
           }       
      }
      else
      {
  
           nb1=( long int)(pch)-( long int)(nom_image);
           strncpy(repert,nom_image,nb1);
           repert[nb1]='\0';
           deb=nom_image+nb1+1;
           nb2=strlen(nom_image)-(nb1+1);
	   strncpy(image,deb,nb2);
           image[nb2]='\0';

/*----------------------------------------------------------*/
/* Modification septembre 98 demandee par H.Vadon           */
/* Afin de permettre d'utiliser une image monocanale en BSQ */
/* avec l'extension libre sans .c1 forcement		    */
/*----------------------------------------------------------*/
	   if (strcasecmp(format,"BSQ") != 0)
           {
           	car='.';
           	pch=(char *)strrchr(image,car);
           	if (pch != NULL)
               		*pch='\0';    
           }      

     }
        
   } 
/*..............................................
  Verification existence repertoire demande
  ...............................................*/
  if ( nb == 1 )
  {
    dirp=opendir(repert);
    if (dirp ==(DIR *)NULL)
    {
    /* repertoire demande inexistant */
     nb=0;
     sprintf(CAI_ERREUR,"repertoire %s inexistant \n",repert);
     }
     else
        closedir(dirp);

    }
  
   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 
             repertoire de stockage de l'image
  	     nom de l'image 
             nombre de canaux
	     nombre d'octets par pixels
	     nombre de colonnes
             nombre de lignes
  ......................................................................*/
    if ( nb == 1 )
   {
     if (IND_GRILLES == '\0')
     {
	if (IND_SPOT == '\0')
            image1 = (*comp)(repert,image,nb_can,nboct_pix,nb_col,nb_lig);
        else
            image1 = (*comp)(IND_SPOT,repert,image,nb_can,nboct_pix,nb_col,
						nb_lig);
      }
     else
         image1 = (*comp)(IND_GRILLES,repert,image,nb_can,nboct_pix,nb_col,
						nb_lig);
     IND_GRILLES = '\0';
     IND_SPOT = '\0';
     if (image1 != NULL)
     {
       strcpy(image1->TYPE_IMAGE,format);
       strcpy(image1->NOM_IMAGE,nom_image);
       image1->NB_CANAUX=*nb_can;
       image1->NB_COL=*nb_col;
       image1->NB_LIG=*nb_lig;
       image1->NBOCT_PIX=*nboct_pix;
       image1->MODE=CAI_LECTURE;

/***   Traitement de la Colormap si format autre que TIFF et RASTERFILE ***/

       if ((strcasecmp(format,"RASTERFILE") != 0)&&(strcasecmp(format,"TIFF") != 0))
       {

        	icr = cai_lecture_colormap(image1);

        	if ( icr == CAI_KO) 
         		 return (NULL);
        	else
          		return (image1);
        }
        else
        {
          return (image1);
        }
       }
      else
       return(NULL);
    } 
    else
    {
     return (NULL);
    }
}
/*
  Fin de l'operation cai_ouvre_lecture_image.c
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_image                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     dans un format donne et de tous les fichiers qui la composent   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_creation_image (nom_image,       */
/*  ---------------- 		                           type_image,       */
/*                 		                           nb_can,           */
/*				                           nboct_pix,	     */
/*				                           nb_col,           */
/*				                           nb_lig,           */
/*				                           label)            */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     type_image     (E) (char *) : format de l'image a ouvrir              */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_image (S) (CAI_IMAGE *)    : =NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_creation_image ( char *nom_image,
                  		      char *type_image,
                 		      int nb_can,
				      int nboct_pix,		
				      int nb_col,
				      int nb_lig,
				      char *label)                  

{

  int nb;                 /* indicateur d'erreurs rencontres              */
 CAI_IMAGE *image1;       /* descripteur de fichier retour fonction       */
 CAI_IMAGE *(*comp) ( );  /* pointeur sur fonction                        */
 char repert[1024];       /* repertoire de stockage de l'image            */
 char image[1024];        /* nom de l'image a ouvrir en ecriture          */
 char *pch,*deb;          /* pointeurs de travail                         */
 long int nb1,nb2;        /* indice de longueur significative             */
 char car;		  /* variable de travail                          */
 DIR *dirp;		  /* pointeur sur structure ouverture repertoire  */


/*......................
  Initialisations
  .......................*/

 nb=1; 
 CAI_ERREUR[0]='\0';
 IND_GRILLES = '\0';
 IND_SPOT = '\0';
/*................................................................
  Verification de la coherence des donnees en entree
  .............................................................*/
  if (nb_can<=0)
  {
      sprintf(CAI_ERREUR,"Nombre du canal a ecrire incorrect"); 
      nb=0;
  }

  if (nb_lig<=0)
  {
      sprintf(CAI_ERREUR,"Nombre de lignes a ecrire incorrect"); 
      nb=0;

  }

   if (nb_col<=0)
  {
      sprintf(CAI_ERREUR,"Nombre de colonnes a ecrire incorrect"); 
      nb=0;
  }


/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */
   if (strcasecmp(type_image,"INRIA") == 0)
   {
      comp = (cai_ouvre_creation_inria) ;
    }

   else if (strcasecmp(type_image,"ONERA") == 0)
   {
      comp = (cai_ouvre_creation_onera) ;
    }

   else if (strcasecmp(type_image,"BSQ") == 0)
   {
      comp = (cai_ouvre_creation_bsq) ;
    }

   else if (strcasecmp(type_image,"BIP") == 0)
   {
      comp = (cai_ouvre_creation_bip) ;
    }

#ifdef _HDF16

   else if (strncasecmp(type_image,"HDF",3) == 0)
   {
      comp = (cai_ouvre_creation_hdf16) ;
    }
#endif

   else if (strcasecmp(type_image,"BIL") == 0)
   {
      comp = (cai_ouvre_creation_bil) ;
    }

   else if (strcasecmp(type_image,"LUM") == 0)
   {
      comp = (cai_ouvre_creation_lum) ;
    }

   else if (strcasecmp(type_image,"GESSIMU") == 0)
   {
      comp = (cai_ouvre_creation_gessimu) ;
    }

   else if (strcasecmp(type_image,"GDB") == 0)
   {
      IND_GRILLES = 'B';
      comp = (cai_ouvre_creation_grilles) ;
    }

   else if (strcasecmp(type_image,"GDR") == 0)
   {
      IND_GRILLES = 'R';
      comp = (cai_ouvre_creation_grilles) ;
    }

   else if (strcasecmp(type_image,"TIFF") == 0)
   {
      comp = (cai_ouvre_creation_tiff) ;
    }

   else if (strcasecmp(type_image,"RASTERFILE") == 0)
   {
      comp = (cai_ouvre_creation_sun_raster) ;
    }
   else if (strcasecmp(type_image,"JPEG") == 0)
   {
      comp = (cai_ouvre_creation_jpeg) ;
    }
  else if (strcasecmp(type_image,"SPOT1A4") == 0)
   {
      IND_SPOT = '1';
      comp = (cai_ouvre_creation_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5TIF") == 0)
   {
      IND_SPOT = 'T';
      comp = (cai_ouvre_creation_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5BIL") == 0)
   {
      IND_SPOT = 'B';
      comp = (cai_ouvre_creation_spot) ;
    }
   else if (strcasecmp(type_image,"RADAR") == 0)
   {
      comp = (cai_ouvre_creation_radar) ;
    }
   else if (strcasecmp(type_image,"BANDEAU") == 0)
   {
     nb = 0;
     sprintf(CAI_ERREUR,"Ecriture au format BANDEAU non prévue dans CAI \n");      
    }
   else 
    { 
 
   /* format non connu de la Couche Acces Image */
     nb = 0;
     sprintf(CAI_ERREUR,"format %s non reconnu \n",type_image);

   }
      /*...............................................................
      Separation du repertoire de stockage de l'image et du nom propre
     .................................................................*/
    if (nb == 1)
    {
      repert[0]='\0';
      image[0]='\0';
      car = '/';
#ifdef _WIN
      car='\\';
#endif
      pch=(char *)strrchr(nom_image,car);
      if (pch == NULL) 
      {
         /* le repertoire est celui courant */
           strcpy(repert,"./");
#ifdef _WIN
           strcpy(repert,".\\");
#endif
             sscanf(nom_image,"%s",image);
      }
      else
      {
  
           nb1=( long int)(pch)-(long int)(nom_image);
           strncpy(repert,nom_image,nb1);
	   repert[nb1]='\0';
           deb=nom_image+nb1+1;
           nb2=strlen(nom_image)-(nb1+1);
	   strncpy(image,deb,nb2);
           image[nb2]='\0';
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';      
     }
        
   }
/*..............................................
  Verification existence repertoire demande
  ...............................................*/
  if ( nb == 1 )
  {
    dirp=opendir(repert);
    if (dirp ==0)
    {
    /* repertoire demande inexistant */
     nb=0;
     sprintf(CAI_ERREUR,"repertoire %s inexistant \n",repert);
     }
     else
     {
        closedir(dirp);
     }


    }
   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 
            repertoire de stockage de l'image
     	     nom de l'image 
             nombre de canaux
	     nombre d'octets par pixels
	     nombre de colonnes
             nombre de lignes
	     label lie a l'image
  ......................................................................*/
    if ( nb == 1 )
    {
     if ((long int)strlen(label)==0) 
	strcpy(label,image);
    if (IND_GRILLES == '\0')
    {
      if (IND_SPOT == '\0')
        image1 = (*comp)(repert,image,&nb_can,&nboct_pix,&nb_col,&nb_lig,label);
      else
        image1 = (*comp)(IND_SPOT,repert,image,&nb_can,&nboct_pix,
			        &nb_col,&nb_lig,label);
     }
     else
      image1 = (*comp)(IND_GRILLES,repert,image,&nb_can,&nboct_pix,
			&nb_col,&nb_lig,label);
     IND_GRILLES = '\0';
     IND_SPOT = '\0';
     if (image1 != NULL)
     {
       strcpy(image1->COMMENT,label);
       strcpy(image1->TYPE_IMAGE,type_image);
       strcpy(image1->NOM_IMAGE,nom_image);
       image1->NB_CANAUX=nb_can;
       image1->NB_COL=nb_col;
       image1->NB_LIG=nb_lig;
       image1->NBOCT_PIX=nboct_pix;
       image1->MODE=CAI_ECRITURE;
 
       return (image1);
        
     }
     else
        return(NULL);
     }
    else
       return (NULL);

}
/*
  Fin de l'operation cai_ouvre_creation_image.c
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image dans un format donne */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  int cai_lecture_canal(image1, canal, premiere_ligne,  */       		      /* ------------------                        premiere_colonne,               */
/*                                           nombre_lignes_lire ,            */
/*                                           nombre_colonnes_lire,           */
/*                                           pas_ligne ,pas_colonne,         */
/*                 		             data_image )                    */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     premiere_ligne       (E) ( int) : premiere ligne a lire dans l'image   */
/*     premiere _colonne    (E) ( int) : premiere colonne a lire dans l'image */
/*     nombre_lignes_lire   (E) ( int) : nombre de lignes a lire              */
/*     nombre_colonnes_lire (E) ( int) : nombre de colonnes a lire            */
/*     canal                (E) ( int) : numero du canal a lire               */
/*     pas_ligne            (E) ( int) : pas en ligne                         */
/*     pas_colonne          (E) ( int) : pas en colonne                       */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels lus alloue et     */
/*                                      libere par l'appelant                */
/*									     */
/*     cai_lecture_canal    (S) (CAI_OK_KO) : = CAI_OK si correct            */
/*                                            = CAI_KO si PB                 */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_lecture_canal(CAI_IMAGE *image1 ,  
                            int canal ,           
    		            int premiere_ligne  ,   
     		            int premiere_colonne,
                            int nombre_lignes_lire ,
                            int nombre_colonnes_lire,
                            int pas_ligne ,          
                            int pas_colonne,
		            unsigned char *data_image )        

{
 long int (*comp) ( );   /* pointeur sur fonction                      */
 char type_image[20];    /* format de stockage de l'image a lire       */
 long int retour;        /* indicateur d'erreur de l'operation         */

/*......................
  Initialisations
  .......................*/
  retour=CAI_OK;
  strcpy( type_image,image1->TYPE_IMAGE);
  CAI_ERREUR[0]='\0';
  IND_GRILLES = '\0';
  IND_SPOT = '\0';
/*.............................................................
  Verification de la coherence des donnees en entree
  .............................................................*/
  if ((premiere_ligne<=0)||(premiere_ligne> image1->NB_LIG))
  {
      sprintf(CAI_ERREUR,"Premiere ligne a lire incorrecte");
      retour=CAI_KO;

  }

  else if ((premiere_colonne<=0)||(premiere_colonne> image1->NB_COL))
  {
      sprintf(CAI_ERREUR,"Premiere colonne a lire incorrecte");
      retour=CAI_KO;
  }


  else if ((canal<=0)||(canal> image1->NB_CANAUX))
  {
      sprintf(CAI_ERREUR,"Numero du canal a lire incorrect"); 
      retour=CAI_KO;
  }

  else if ((nombre_lignes_lire<=0)||
	 ((nombre_lignes_lire + premiere_ligne -1) > image1->NB_LIG))
  {
      sprintf(CAI_ERREUR,"Nombre de lignes a lire incorrect");
      retour=CAI_KO;
  }

  else if ((nombre_colonnes_lire<=0)||
	((nombre_colonnes_lire + premiere_colonne -1) > image1->NB_COL))
  {
      sprintf(CAI_ERREUR,"Nombre de colonnes a lire incorrect");
      retour=CAI_KO;
  }
 if (retour == CAI_OK)
 {

/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */
   if (strcasecmp(type_image,"INRIA") == 0)
   {
      comp = (cai_lecture_canal_inria) ;
    }

   else if (strcasecmp(type_image,"ONERA") == 0)
   {
      comp = (cai_lecture_canal_onera) ;
    }

   else if (strcasecmp(type_image,"BSQ") == 0)
   {
      comp = (cai_lecture_canal_bsq) ;
    }

   else if (strcasecmp(type_image,"BIP") == 0)
   {
      comp = (cai_lecture_canal_bip) ;
    }

#ifdef _HDF16
   else if (strncasecmp(type_image,"HDF",3) == 0)
   {
      comp = (cai_lecture_canal_hdf16) ;
    }
#endif

   else if (strcasecmp(type_image,"BIL") == 0)
   {
      comp = (cai_lecture_canal_bil) ;
    }

   else if (strcasecmp(type_image,"LUM") == 0)
   {
      comp = (cai_lecture_canal_lum) ;
    }

   else if (strcasecmp(type_image,"GESSIMU") == 0)
   {
      comp = (cai_lecture_canal_gessimu) ;
    }

   else if (strcasecmp(type_image,"GDB") == 0)
   {
       comp = (cai_lecture_canal_grilles) ;
    }

   else if (strcasecmp(type_image,"GDR") == 0)
   {
       comp = (cai_lecture_canal_grilles) ;
    }

   else if (strcasecmp(type_image,"TIFF") == 0)
   {
      comp = (cai_lecture_canal_tiff) ;
    }

   else if (strcasecmp(type_image,"RASTERFILE") == 0)
   {
      comp = (cai_lecture_canal_sun_raster) ;
    }
   else if (strcasecmp(type_image,"JPEG") == 0)
   {
      comp = (cai_lecture_canal_jpeg) ;
    }
   else if (strcasecmp(type_image,"SPOT1A4") == 0)
   {
      comp = (cai_lecture_canal_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5TIF") == 0)
   {
      comp = (cai_lecture_canal_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5BIL") == 0)
   {
      comp = (cai_lecture_canal_spot) ;
    }
   else if (strcasecmp(type_image,"RADAR") == 0)
   {
      comp = (cai_lecture_canal_radar) ;
    }
   else if (strcasecmp(type_image,"BANDEAU") == 0)
   {
      comp = (cai_lecture_canal_bandeau) ;
    }
   else 
    { 
 
   /* format non connu de la Couche Acces Image */
     retour = CAI_KO;
     sprintf(CAI_ERREUR,"format %s non reconnu \n",type_image);


   }
 }
   
   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 

	 pointeur sur descripteur des fichiers
 	 numero du canal a lire
 	 premiere ligne a lire dans l'image
	 premiere colonne a lire dans l'image
	 nombre de lignes a lire
	 nombre de colonnes a lire
	 pas en ligne
	 pas en colonne
         tableau des pixels lus
  ......................................................................*/
   if ( retour == CAI_OK)
   {
           retour=(*comp)(image1 ,  
                    &canal ,           
    		    &premiere_ligne  ,   
     		    &premiere_colonne   ,
                    &nombre_lignes_lire ,
                    &nombre_colonnes_lire,
                    &pas_ligne ,          
                    &pas_colonne, 
                    data_image) ; 
 	return(retour);
    }
    else
    {
     return (CAI_KO);
    }

}
/*
  Fin de l'operation cai_lecture_canal.c
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal                                         */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image dans un format donne */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  int cai_ecriture_canal(image1, canal, premiere_ligne, */       		      /* ------------------                         nombre_lignes_ecrire,          */
/*                                            nombre_colonnes_ecrire,        */
/*                 		              data_image )                   */
/*									     */
/*     image1      (E) (CAI_IMAGE *)   : pointeur sur descripteur de fichier */
/*     canal                 (E) ( int) : numero du canal a ecrire            */
/*     premiere_ligne        (E) ( int) : premiere ligne a ecrire ds l'image  */
/*     nombre_lignes_ecrire  (E) ( int) : nombre de lignes a ecrire           */
/*     nombre_colonnes_ecrire(E) ( int) : nombre de lignes a ecrire           */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels a ecrire alloue   */
/*                                      et libere par l'appelant             */
/*									     */
/*     cai_ecriture_canal    (S) (CAI_OK_KO) : = CAI_KO si pb                */
/*                                             = CAI_OK si OK                */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ecriture_canal (CAI_IMAGE *image1 ,
		 	      int canal ,
			      int premiere_ligne,
		              int nombre_lignes_ecrire ,
			      int nombre_colonnes_ecrire,
			      unsigned char *data_image )
		               

{
 long int (*comp) ( );   /* pointeur sur fonction                      */
 char type_image[20];    /* format de stockage de l'image a ecrire     */
 long  int retour;       /* indicateur d'erreur de l'operation         */


/*......................
  Initialisations
  .......................*/

  retour=CAI_OK;
  type_image[0]='\0';
  strcpy( type_image,image1->TYPE_IMAGE);
  CAI_ERREUR[0]='\0';

 /*................................................................
  Verification de la coherence des donnees en entree
  .............................................................*/
/* sur la premiere ligne a ecrire */

  if ((premiere_ligne<=0)||(premiere_ligne> image1->NB_LIG))
  {
      sprintf(CAI_ERREUR,"Premiere ligne a ecrire incorrecte");
      retour=CAI_KO;
  }

/* sur le numero du canal a ecrire */

  if ((canal<=0)||(canal> image1->NB_CANAUX))
  {
      sprintf(CAI_ERREUR,"Numero du canal a ecrire incorrect"); 
      retour=CAI_KO;

  }

/* sur le nombre de lignes a ecrire */
  if ((nombre_lignes_ecrire<=0)||
	((nombre_lignes_ecrire + premiere_ligne-1) > image1->NB_LIG))
  {
      sprintf(CAI_ERREUR,"Nombre de lignes a ecrire incorrect");
      retour=CAI_KO;
  }

/* sur le nombre de colonnes a ecrire */


  if ((nombre_colonnes_ecrire<=0)||
		(nombre_colonnes_ecrire  > image1->NB_COL))
  {
      sprintf(CAI_ERREUR,"Nombre de colonnes a ecrire incorrect");
      retour=CAI_KO;
  }

 if (retour == CAI_OK)
 {
/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */

   if (strcasecmp(type_image,"INRIA") == 0)
   {
      comp = (cai_ecriture_canal_inria) ;
    }

   else if (strcasecmp(type_image,"ONERA") == 0)
   {
      comp = (cai_ecriture_canal_onera) ;
    }

   else if (strcasecmp(type_image,"BSQ") == 0)
   {
      comp = (cai_ecriture_canal_bsq) ;
    }

   else if (strcasecmp(type_image,"BIP") == 0)
   {
      comp = (cai_ecriture_canal_bip) ;
    }

#ifdef _HDF16

   else if (strncasecmp(type_image,"HDF",3) == 0)
   {
      comp = (cai_ecriture_canal_hdf16) ;
    }
#endif

   else if (strcasecmp(type_image,"BIL") == 0)
   {
      comp = (cai_ecriture_canal_bil) ;
    }

   else if (strcasecmp(type_image,"LUM") == 0)
   {
      comp = (cai_ecriture_canal_lum) ;
    }

   else if (strcasecmp(type_image,"GESSIMU") == 0)
   {
      comp = (cai_ecriture_canal_gessimu) ;
    }

   else if (strcasecmp(type_image,"GDB") == 0)
   {
       comp = (cai_ecriture_canal_grilles) ;
    }

   else if (strcasecmp(type_image,"GDR") == 0)
   {
       comp = (cai_ecriture_canal_grilles) ;
    }

   else if (strcasecmp(type_image,"TIFF") == 0)
   {
      comp = (cai_ecriture_canal_tiff) ;
    }

   else if (strcasecmp(type_image,"RASTERFILE") == 0)
   {
      comp = (cai_ecriture_canal_sun_raster) ;
    }
   else if (strcasecmp(type_image,"JPEG") == 0)
   {
      comp = (cai_ecriture_canal_jpeg) ;
    }

   else if (strcasecmp(type_image,"SPOT1A4") == 0)
   {
      comp = (cai_ecriture_canal_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5TIF") == 0)
   {
      comp = (cai_ecriture_canal_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5BIL") == 0)
   {
      comp = (cai_ecriture_canal_spot) ;
    }
   else if (strcasecmp(type_image,"RADAR") == 0)
   {
      comp = (cai_ecriture_canal_radar) ;
    }
   else if (strcasecmp(type_image,"BANDEAU") == 0)
   {
     retour = CAI_KO;
     sprintf(CAI_ERREUR,"Ecriture au format BANDEAU non prévue dans CAI \n");      
    }
   else 
    { 
 
   /* format non connu de la Couche Acces Image */
     retour = CAI_KO;
     sprintf(CAI_ERREUR,"format %s non reconnu \n",type_image);


   }
  }
   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 

	 pointeur sur descripteur des fichiers
 	 numero du canal a ecrire
 	 premiere ligne a ecrire dans l'image
	 nombre de lignes a ecrire
	 nombre de colonnes a ecrire
         tableau des pixels a ecrire
  ......................................................................*/
   if ( retour == CAI_OK)
   {
          retour=(*comp)(image1 ,  
                    &canal ,           
    		    &premiere_ligne  ,   
                    &nombre_lignes_ecrire ,
                    &nombre_colonnes_ecrire,
                    data_image) ;  
     }
 
     return (retour);

}
/* 
  Fin de l'operation cai_ecriture_canal.c
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_image                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image dans un format donne et de    */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  int cai_ferme_image(image1)                           */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_image    (S) (CAI_OK_KO) :    = CAI_KO si pb                */
/*                                             = CAI_OK si OK                */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ferme_image (CAI_IMAGE *image1)

{
 long int iret;           /* indicateur retour fonction   */
 long int (*comp)();      /* pointeur sur fonction        */

/*......................
  Initialisations
  .......................*/

  iret=CAI_OK;
  CAI_ERREUR[0]='\0';
  IND_GRILLES = '\0';
  IND_SPOT = '\0';
/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */

   if (strcasecmp(image1->TYPE_IMAGE,"INRIA") == 0)
   {
      comp = (cai_ferme_inria) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"ONERA") == 0)
   {
      comp = (cai_ferme_onera) ;
    }
   else if (strcasecmp(image1->TYPE_IMAGE,"BSQ") == 0)
   {
      comp = (cai_ferme_bsq) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"BIP") == 0)
   {
      comp = (cai_ferme_bip) ;
    }

#ifdef _HDF16

   else if (strncasecmp(image1->TYPE_IMAGE,"HDF",3) == 0)
   {
      comp = (cai_ferme_hdf16) ;
    }
#endif

   else if (strcasecmp(image1->TYPE_IMAGE,"BIL") == 0)
   {

      comp = (cai_ferme_bil) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"LUM") == 0)
   {

      comp = (cai_ferme_lum) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"GESSIMU") == 0)
   {
      comp = (cai_ferme_gessimu) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"GDB") == 0)
   {
      IND_GRILLES = 'B';
      comp = (cai_ferme_grilles) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"GDR") == 0)
   {
      IND_GRILLES = 'R';
      comp = (cai_ferme_grilles) ;
    }


   else if (strcasecmp(image1->TYPE_IMAGE,"TIFF") == 0)
   {
      comp = (cai_ferme_tiff) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"RASTERFILE") == 0)
   {
      comp = (cai_ferme_sun_raster) ;
    }
   else if (strcasecmp(image1->TYPE_IMAGE,"JPEG") == 0)
   {
      comp = (cai_ferme_jpeg) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"SPOT1A4") == 0)
   {
      IND_SPOT = '1';
      comp = (cai_ferme_spot) ;
    }
   else if (strcasecmp(image1->TYPE_IMAGE,"SPOT5TIF") == 0)
   {
      IND_SPOT = 'T';
      comp = (cai_ferme_spot) ;
    }
   else if (strcasecmp(image1->TYPE_IMAGE,"SPOT5BIL") == 0)
   {
      IND_SPOT = 'B';
      comp = (cai_ferme_spot) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"RADAR") == 0)
   {

      comp = (cai_ferme_radar) ;
    }

   else if (strcasecmp(image1->TYPE_IMAGE,"BANDEAU") == 0)
   {

      comp = (cai_ferme_bandeau) ;
    }

   else 
    { 
 
   /* format non connu de la Couche Acces Image */
     iret = CAI_KO;
     sprintf(CAI_ERREUR,"format %s non reconnu \n",image1->TYPE_IMAGE);


   }


   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 
	     pointeur sur descripteur des fichiers de l'image
  ......................................................................*/

   if (( iret == CAI_OK ) && (image1 != NULL))
   {
/***   Traitement de la Colormap  ***/
       if ((image1->COLORMAP !=0)&& ( image1->MODE == CAI_ECRITURE))
       {
        iret = cai_ecriture_colormap(image1);
       }

       if (image1->COLORMAP !=0)
       {
           free(image1->TABCOLOR);
        }
        if (IND_GRILLES == '\0')
        {
	   if (IND_SPOT == '\0')
                iret = (*comp)(image1);
	   else
                iret = (*comp)(IND_SPOT,image1); 
        }
	else
          iret = (*comp)(IND_GRILLES,image1);     
    }
    free(image1);

    image1 = (CAI_IMAGE *)NULL;

    return (iret);
}
/*
  Fin de l'operation cai_ferme_image.c
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_destruction_image                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image dans un format donne et de  */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : int cai_destruction_image(format,nom_ima)              */ 
/* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_destruction_image   (S) (CAI_OK_KO) :    = CAI_KO si pb           */
/*                                                  = CAI_OK si OK           */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_destruction_image (char *format ,
                                 char *nom_ima )


{

long int (*ptr_dest_ima)();	/* ptr sur fctn de destruction d'une image */
long int cr_cai;		/* code retour de fonction appellee        */
long int i,ind;                 /* variables de travail                    */
char repert[1024];              /* repertoire de stockage de l'image       */
char image[1024];               /* nom de l'image a detruire sans repert   */
char form[20];                  /* format de l'image a detruire            */
char *pch,*deb;                 /* variables de travail                    */
long int nb1,nb2;               /* variables de travail                    */
char car;                       /* variables de travail                    */
/*................. 
 Initialisations 
  ................*/

cr_cai = CAI_OK;
strcpy(form,format);
CAI_ERREUR[0]='\0';
IND_GRILLES = '\0';
IND_SPOT = '\0';
/*...................................................
   Recherche du type de fichier si reconnaissance automatique demande
  ...................................................*/

  if (strcasecmp(form,"AUTO")== 0)
  {
     for( i = 0 ; i< CAI_NB_EXTENSIONS ; i++)
      {
       if(strstr(nom_ima,CAI_SUFFIXES[i]) != NULL )
       {
          ind=CAI_NUMEROS[i];
          strcpy(form,CAI_NOM_FORMAT[ind]);
       }
    }
  }

/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */

   if (strcasecmp(form,"INRIA") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_inria) ;
    }

   else if (strcasecmp(form,"ONERA") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_onera) ;
    }

   else if (strcasecmp(form,"BSQ") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_bsq) ;
    }

   else if (strcasecmp(form,"BIP") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_bip) ;
    }

#ifdef _HDF16

   else if (strncasecmp(form,"HDF",3) == 0)
   {
      ptr_dest_ima = (cai_dest_ima_hdf16) ;
    }
#endif

   else if (strcasecmp(form,"BIL") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_bil) ;
    }

   else if (strcasecmp(form,"LUM") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_lum) ;
    }

   else if (strcasecmp(form,"GESSIMU") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_gessimu) ;
    }

   else if (strcasecmp(form,"GDB") == 0)
   {
      IND_GRILLES = 'B';
      ptr_dest_ima = (cai_dest_ima_grilles) ;
    }

   else if (strcasecmp(form,"GDR") == 0)
   {
      IND_GRILLES = 'R';
      ptr_dest_ima = (cai_dest_ima_grilles) ;
    }

   else if (strcasecmp(form,"TIFF") == 0)
   {
      ptr_dest_ima  = (cai_dest_ima_tiff) ;
    }

   else if (strcasecmp(form,"RASTERFILE") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_sun_raster) ;
    }
   else if (strcasecmp(form,"JPEG") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_jpeg) ;
    }
   else if (strcasecmp(form,"SPOT1A4") == 0)
   {
      IND_SPOT = '1';
      ptr_dest_ima = (cai_dest_ima_spot) ;
    }
   else if (strcasecmp(form,"SPOT5TIF") == 0)
   {
      IND_SPOT = 'T';
      ptr_dest_ima = (cai_dest_ima_spot) ;
    }
   else if (strcasecmp(form,"SPOT5BIL") == 0)
   {
      IND_SPOT = 'B';
      ptr_dest_ima = (cai_dest_ima_spot) ;
    }
   else if (strcasecmp(form,"RADAR") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_radar) ;
    }
   else if (strcasecmp(form,"BANDEAU") == 0)
   {
      ptr_dest_ima = (cai_dest_ima_bandeau) ;
    }
   else 
    { 
 
   /* format non connu de la Couche Acces Image */
    cr_cai = CAI_KO;
    sprintf(CAI_ERREUR,"format %s non reconnu \n",form);

   }
      /*...............................................................
      Separation du repertoire de stockage de l'image et du nom propre
     .................................................................*/
    if (cr_cai ==CAI_OK)
    {
      repert[0]='\0';
      image[0]='\0';
      car='/';
#ifdef _WIN
      car='\\';
#endif
      pch=(char *)strrchr(nom_ima,car);
      if (pch == NULL) 
      {
         /* le repertoire est celui courant */
           strcpy(repert,"./");
#ifdef _WIN
           strcpy(repert,".\\");
#endif
           sscanf(nom_ima,"%s",image);
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';          

      }
      else
      {
  
           nb1=(long int)(pch)-(long int)(nom_ima);
           strncpy(repert,nom_ima,nb1);
	   repert[nb1]='\0';
           deb=nom_ima+nb1+1;
           nb2=strlen(nom_ima)-(nb1+1);
	   strncpy(image,deb,nb2);
           image[nb2]='\0';
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';          

     }
        
   }


/****************************************************/
/* appel de la fonction pointee par ptr_dest_ima    */
/* avec les parametres suivants :      		    */
/* - repertoire de stockage de l'image		    */
/* - nom de l'image			            */
/****************************************************/

   if (cr_cai==CAI_OK)
   {
     if (IND_GRILLES == '\0')
     {
        if (IND_SPOT == '\0')
             cr_cai = (*ptr_dest_ima)(repert, image);
         else
	     cr_cai = (*ptr_dest_ima)(IND_SPOT,repert, image);
      }
      else
         cr_cai = (*ptr_dest_ima)(IND_GRILLES,repert, image);

    }
   return (cr_cai);
}
/*
  Fin de l'operation cai_destruction_image.c
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_modif_nbbitspix                                        */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction permetant de modifier le nombre de bits significatifs  */
/* -----     par pixel (uniquement pour les formats BSQ ,BIP,BIL et HDF)     */
/*                                                                           */
/* Remarque !! L'image doit deja etre ouverte                                */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  int cai_modif_nbbitspix(format,image1,NbBitsPix)      */       		      /* ------------------                                                        */
/*     format      (E) (char *)       : format de l'image a modifier         */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     NbBitsPix   (E) (long int)          : nb de bits significatifs par pixel   */
/*									     */
/*     cai_modif_nbbitspix   (S) (CAI_OK_KO) :    = CAI_KO si pb             */
/*                                                = CAI_OK si OK             */
/*			= CAI_IMPOS si format autre que BSQ ou HDF16           */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_modif_nbbitspix ( char *format ,
                                  CAI_IMAGE *image1,int NbBitsPix)

{

long int cr_cai;		/* code retour de fonction appellee        */
char form[20];                  /* format de l'image a modifier            */
CAI_EXTENS *TabExtens;
/*................. 
 Initialisations 
  ................*/

TabExtens = NULL;
cr_cai = CAI_OK;
strcpy(form,format);
CAI_ERREUR[0]='\0';

/*..............................................
  Lancement de la fonction dependant du format des images
  ................................................ */

   if (strcasecmp(form,"BSQ") == 0)
   {
      cr_cai = cai_modif_nbbitspix_bsq(image1,NbBitsPix) ;
    }


#ifdef _HDF16

   else if (strncasecmp(form,"HDF",3) == 0)
   {
      image1->NB_BITS_PIX = NbBitsPix;
      cr_cai = cai_rename_fichiers_hdf16(image1,TabExtens) ;
    }
#endif
    else if (strcasecmp(form,"BIP") == 0)
    {
      cr_cai = cai_modif_nbbitspix_bip(image1,NbBitsPix) ;
     }
    else if (strcasecmp(form,"BIL") == 0)
    {
      cr_cai = cai_modif_nbbitspix_bil(image1,NbBitsPix) ;
     }

    else 
    { 

   /* Fonction non prevue pour un autre format */
       cr_cai = CAI_IMPOS;
       sprintf(CAI_ERREUR,"format %s : modification non prevue \n",form);

      }

   return (cr_cai);
}
/* Fin de l'operation cai_modif_nbbitspix
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_image                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image d'un format*/
/* -----      donne et de tous les fichiers qui la composent                 */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_image (nom_image,        */
/*  ---------------- 		                           type_image,       */
/*                 		                           nb_can,           */
/*				                           nboct_pix,	     */	
/*				                           nb_col,           */
/*				                           nb_lig)           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     type_image     (E) (char *) : format de l'image a ouvrir              */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_image (S) (CAI_IMAGE *)    : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V3                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_image (char *nom_image,
                  		    char *type_image,
                 		    int  *nb_can,
				    int  *nboct_pix,		
				    int  *nb_col,
				    int  *nb_lig)            
{
  
  int nb;             /* indicateur d'erreurs rencontres              */
 CAI_IMAGE *image1;       /* descripteur de fichier retour fonction       */
 CAI_IMAGE *(*comp) ( );  /* pointeur sur fonction                        */
 char repert[1024];       /* repertoire de stockage de l'image            */
 char image[1024];        /* nom de l'image a ouvrir en lecture           */
 char format[20];         /* format de stockage de l'image a lire         */
 long int i,ind;          /* variables de travail                         */
 char *pch,*deb;          /* pointeurs de travail                         */
 long int nb1,nb2;        /* indice de longueur significative             */
 char car;		  /* variable de travail                          */
 DIR *dirp;		  /* pointeur sur structure ouverture repertoire  */
 CAI_OK_KO icr;           /* Compte-rendu des fonctions DA                */


/*......................
  Initialisations
  .......................*/
  nb=1;
  strcpy(format,type_image);
 
  CAI_ERREUR[0]='\0';
  IND_GRILLES = '\0';
  IND_SPOT = '\0';

/*...................................................
   Recherche du type de fichier si reconnaissance automatique demande
  ...................................................*/
  if (strcasecmp(format,"AUTO")== 0)
  {
     for( i = 0 ; i< CAI_NB_EXTENSIONS ; i++)
      {
       if(strstr(nom_image,CAI_SUFFIXES[i]) != NULL )
       {
          ind=CAI_NUMEROS[i];
          strcpy(format,CAI_NOM_FORMAT[ind]);
       }
    }
  }
 
/*..............................................
  Initialisation d'un pointeur sur fonction dependant du format des images
  ................................................ */

   if (strcasecmp(format,"INRIA") == 0)
   {
      comp = (cai_ouvre_modifie_inria) ;
    }

   else if (strcasecmp(format,"ONERA") == 0)
   {
      comp = (cai_ouvre_modifie_onera) ;
    }

   else if (strcasecmp(format,"BSQ") == 0)
   {
      comp = (cai_ouvre_modifie_bsq) ;
    }

   else if (strcasecmp(format,"BIP") == 0)
   {
      comp = (cai_ouvre_modifie_bip) ;
    }
#ifdef _HDF16
   else if (strncasecmp(format,"HDF",3) == 0)
   {
      comp = (cai_ouvre_modifie_hdf16) ;
    }
#endif

   else if (strcasecmp(format,"BIL") == 0)
   {
      comp = (cai_ouvre_modifie_bil) ;
    }

   else if (strcasecmp(format,"LUM") == 0)
   {
      comp = (cai_ouvre_modifie_lum) ;
    }

   else if (strcasecmp(format,"GESSIMU") == 0)
   {
      comp = (cai_ouvre_modifie_gessimu) ;
    }

   else if (strcasecmp(format,"GDB") == 0)
   {
      IND_GRILLES = 'B';
      comp = (cai_ouvre_modifie_grilles) ;
    }

   else if (strcasecmp(format,"GDR") == 0)
   {
      IND_GRILLES = 'R';
      comp = (cai_ouvre_modifie_grilles) ;
    }

   else if (strcasecmp(format,"TIFF") == 0)
   {
/*     comp = (cai_ouvre_modifie_tiff) ; */
      nb = 0;
      sprintf(CAI_ERREUR,
		"fonction de modification non disponible pour TIFF");
    }

   else if (strcasecmp(format,"RASTERFILE") == 0)
   {
      comp = (cai_ouvre_modifie_sun_raster) ;
    }
   else if (strcasecmp(format,"JPEG") == 0)
   {
      nb = 0;
      sprintf(CAI_ERREUR,"fonction de modification non disponible pour JPEG");
    }
   else if (strcasecmp(type_image,"SPOT1A4") == 0)
   {
      IND_SPOT = '1';
      comp = (cai_ouvre_modifie_spot) ;
    }
   else if (strcasecmp(type_image,"SPOT5TIF") == 0)
   {
      IND_SPOT = 'T';
/*      comp = (cai_ouvre_modifie_spot) ;*/
      nb = 0;
      sprintf(CAI_ERREUR,
		"fonction de modification non disponible pour SPOT5TIF");

    }
   else if (strcasecmp(type_image,"SPOT5BIL") == 0)
   {
      IND_SPOT = 'B';
      comp = (cai_ouvre_modifie_spot) ;
    }
   else   if (strcasecmp(format,"RADAR") == 0)
   {
      comp = (cai_ouvre_modifie_radar) ;
   }

   else if (strcasecmp(type_image,"BANDEAU") == 0)
   {
     nb = 0;
     sprintf(CAI_ERREUR,
	"fonction de modification non disponible pour BANDEAU \n");      
    }

   else 
    { 
 
   /* format non connu de la Couche Acces Image */
     nb = 0;
     sprintf(CAI_ERREUR,"format %s non reconnu \n",format);

   }
   /*...............................................................
      Separation du repertoire de stockage de l'image et du nom propre
     .................................................................*/
    if (nb == 1)
    {
      repert[0]='\0';
      image[0]='\0';
      car='/';
#ifdef _WIN
      car='\\';
#endif
      pch=(char *)strrchr(nom_image,car);
      if (pch == NULL) 
      {
         /* le repertoire est celui courant */
           strcpy(repert,"./");
#ifdef _WIN
           strcpy(repert,".\\");
#endif
           sscanf(nom_image,"%s",image);
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';        
      }
      else
      {
  
           nb1=(long int)(pch)-(long int)(nom_image);
           strncpy(repert,nom_image,nb1);
           repert[nb1]='\0';
           deb=nom_image+nb1+1;
           nb2=strlen(nom_image)-(nb1+1);
	   strncpy(image,deb,nb2);
           image[nb2]='\0';
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';          

     }
        
   } 
/*..............................................
  Verification existence repertoire demande
  ...............................................*/
  if ( nb == 1 )
  {
    dirp=opendir(repert);
    if (dirp ==(DIR *)NULL)
    {
    /* repertoire demande inexistant */
     nb=0;
     sprintf(CAI_ERREUR,"repertoire %s inexistant \n",repert);
     }
     else
        closedir(dirp);

    }
  
   /*...........................................
   Appel de la fonction pointee par comp avec la liste des parametres
   suivants : 
             repertoire de stockage de l'image
  	     nom de l'image 
             nombre de canaux
	     nombre d'octets par pixels
	     nombre de colonnes
             nombre de lignes
  ......................................................................*/
    if ( nb == 1 )
   {
    if (IND_GRILLES == '\0')
    {
	if (IND_SPOT == '\0')
            image1 = (*comp)(repert,image,nb_can,nboct_pix,nb_col,nb_lig);
	else
	    image1 = (*comp)(IND_SPOT,repert,image,nb_can,nboct_pix,nb_col,
			 	nb_lig);
    }
    else
        image1 = (*comp)(IND_GRILLES,repert,image,nb_can,nboct_pix,nb_col,
			 nb_lig);

     if (image1 != NULL)
     {
       strcpy(image1->TYPE_IMAGE,format);
       strcpy(image1->NOM_IMAGE,nom_image);
       image1->NB_CANAUX=*nb_can;
       image1->NB_COL=*nb_col;
       image1->NB_LIG=*nb_lig;
       image1->NBOCT_PIX=*nboct_pix;
       image1->MODE=CAI_ECRITURE;

/***   Traitement de la Colormap si format autre que TIFF et RASTERFILE ***/

       if ((strcasecmp(format,"RASTERFILE") != 0)&&(strcasecmp(format,"TIFF") != 0))
       {

        	icr = cai_lecture_colormap(image1);

        	if ( icr == CAI_KO) 
         		 return (NULL);
        	else
          		return (image1);
        }
        else
        {
          return (image1);
        }
       }
      else
       return(NULL);
    } 
    else
    {
     return (NULL);
    }
}
/* Fin de l'operation cai_ouvre_modifie_image
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
 char *cai_test_pFpf ()
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

void cai_inverser_tableau(short *BufL,short *BufE,int Nb)
{
  int pix;
  for (pix=0;pix<Nb;pix=pix+2)
  {
    BufL[pix]=BufE[pix+1];
    BufL[pix+1]=BufE[pix];
  }
}
/* 
   Fin de l'operation cai_inverser_tableau
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

char* cai_En_majuscule( char *mot )

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
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_inverser_tableau_complet                               */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'inversion des poids forts et faible                  */
/* -----              							     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  cai_inverser_tableau_complet(tableau,nbpix,tailpix)   */
/*  ---------------- 		                                             */
/*				                 			     */
/*  tableau	(E) (* uchar)   : tableau à inverser                         */
/*  nbpix	(E) (int)      : nb de pixels du tableau                     */
/*  tailpix     (E) (int)      : nb d'octets d'un pixel                      */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

void cai_inverser_tableau_complet(unsigned char *tableau,int nbpix,int tailpix)
{
  int pix,ind,oct;
  unsigned char *temp; 

  
  temp = (unsigned char *) calloc(tailpix,sizeof(unsigned char));

  for (pix=0;pix<nbpix;pix=pix+1)
  {
     ind = pix*tailpix;
     for (oct=0;oct<tailpix;oct++)
     {
       temp[oct] = tableau[ind + tailpix - oct -1 ];
     }
     for (oct=0;oct<tailpix;oct++)
     {
       tableau[ind + oct ] = temp[oct];
     }
  }
}
/* 
   Fin de l'operation cai_inverser_tableau_complet
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_En_minuscule                                           */
/* ----------								     */
/*                                                                           */
/* ROLE : Cette fonction transforme la chaine de caracteres donnee en        */
/* ROLE : parametre en minuscule et renvoie le resultat                      */
/*                                                                           */
/* CONTEXTE D'APPEL :                                                        */
/*                                                                           */
/* RETOUR : chaine en majuscule                                              */
/*                                                                           */
/*****************************************************************************/

char* cai_En_minuscule( char *mot )
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
   /* transformer en minuscule la lettre lue */
   retour[i] = tolower ( mot[i] );
   i++;
   }
  /* retourner le resultat */ 
  return retour;   
  }
/* 
   Fin de l'operation cai_En_minuscule
*/
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
int cai_istyp_connu(char *type_code)
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
