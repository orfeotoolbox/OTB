/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_bandeau.c                                                 */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format bandeau                                    */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    novembre 2005                                       */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V5.3                                                            */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 5.3 : Prise en compte du format bandeau dans CAI          */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
#include <errno.h>

#define _CAI_IMAGE
#define IN_CAI_BANDEAU

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header$";

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
/* OPERATION :    cai_liste_ima_bandeau                                      */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format bandeau                */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_bandeau(repert,tab_ima)         */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_bandeau   (S) (long int)  : = -1 repertoiren inexistant */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


long int cai_liste_ima_bandeau (char *repert,
		       char ***tab_ima)

{

   DIR *dirp;		/* pointeur sur structure ouverture repertoire */

   struct dirent *dp;   /* structure resultat de la fonction readdir   */

   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2,*pch3;    /* pointeurs sur chaine de caracteres          */
   char car[6];         /* variable de travail                         */  
   int nb_ima;          /* nombre d'images bandeau identifiees         */
   int i;               /* indice de boucle                            */
   long int diff,Long;

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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE BANDEAU_CNES
      (EXTENSION =.band)
   ..........................................*/
     while((dp=readdir(dirp))!=NULL)
     {
        sprintf(car,".band");
        pch0=strstr(dp->d_name,car);
        if (pch0 != NULL)
        {
             *pch0='\0';
	     nb_ima=nb_ima+1;
         }
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
        else
        {
           sprintf(car,".BAND");
           pch1=strstr(dp->d_name,car);
           if (pch1 != NULL)
           {
               *pch1='\0';
	       nb_ima=nb_ima+1;
            }
         }
     }

    (void)closedir(dirp);
   if (nb_ima ==0) goto FIN;

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT BANDEAU
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE BANDEAU_CNES
      (EXTENSION =.band)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
     sprintf(car,".band");
     pch0=strstr(dp->d_name,car);
     if (pch0 != NULL)
     {
             *pch0='\0';
             strcpy((*tab_ima)[nb_ima],dp->d_name);
	     nb_ima=nb_ima+1;
       }
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      else
      {
         sprintf(car,".BAND");
         pch1=strstr(dp->d_name,car);
         if (pch1 != NULL)
         {
             *pch1='\0';
             strcpy((*tab_ima)[nb_ima],dp->d_name);
	     nb_ima=nb_ima+1;
          }
       }
     }

    (void)closedir(dirp);


 }
 else
 {
    /* repertoire inexistant */
     nb_ima=-1;
  }

FIN:
  return(nb_ima);

}

/* Fin de l'operation cai_liste_ima_bandeau
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_bandeau                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format bandeau   */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_lecture_bandeau (nom_image,   */
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
/*     cai_ouvre_lecture_bandeau     (S) (CAI_IMAGE *)  : = NULL si pb       */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
CAI_IMAGE *cai_ouvre_lecture_bandeau(char *repert,
				 char *nom_image,
				 int  *nb_can,
				 int  *nboct_pix,
				 int  *nb_col,
				 int  *nb_lig)
{
   CAI_IMAGE *image1,*image2;   /* pointeur su descripteur de fichier image */ 
   int NbBandeaux;              /* nombre de bandeaux                       */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char label[80];		/* contient un enregistrement du fichier    */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   char format[20];		/* Format de stockage de chaque bandeau     */
   int nbcan, nboctpix;
   int nbcol,nblig;
   int ific,num,ind;
   CH50 tab_ima[20];


   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
 
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   if (image1==NULL)
   {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_lecture");
     goto ERREUR;
    }

/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)== 0)
	sprintf(repert,"./");
   sprintf(nom_fic,"%s/%s.band",repert,nom_image);
   sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);

  /*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/* verification si extension en majuscule */
      nom_fic[0]='\0';
      sprintf(nom_fic,"%s/%s.BAND",repert,nom_image);
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
          strcpy(CAI_ERREUR,"Erreur ouverture fichier entete BANDEAU-CNES");
          goto ERREUR;
     }
   }
   
/*....................................
     Decodage du nombre de BANDEAU
  ............................................*/
     fscanf(fic_in,"%s %d\n",label,&NbBandeaux);
     retour=strncmp(label,"BANDEAU",7);
     if (retour!=0)
     {
       sprintf(CAI_ERREUR,
			"Informations incoherentes ligne BANDEAU dans %s",
			nom_fic);
       goto ERREUR;
     }
/*....................................
     Decodage du nombre du format
  ............................................*/
     fscanf(fic_in,"%s\n",format);

/*....................................
     Recuperation du nom de chaque bandeau
  ............................................*/
   for (ific=0;ific<NbBandeaux;ific++)
   {   
      fscanf(fic_in,"%s %d\n",label,&num);
      strcpy(tab_ima[num-1],label);
   }
         
/*..................................
   Fermeture du fichier 
  ......................................*/
    if (fic_in!=NULL) 
    {
	fclose(fic_in);
	fic_in=NULL;
    }

/*....................................
     Ouverture de chaque bandeau
  ............................................*/
   for (ific=0;ific<NbBandeaux;ific++)
   {   
      sprintf(nom_fic,"%s/%s",repert,tab_ima[ific]);
      image2 = cai_ouvre_lecture_image(nom_fic, format,&nbcan,
                                      &nboctpix,&nbcol,&nblig); 
      if (image2 == NULL)
      {
          sprintf(CAI_ERREUR,"Erreur ouverture du bandeau %s \n",nom_fic);
          goto ERREUR;
      }
      *nb_can = nbcan;
      *nboct_pix = nboctpix;
      *nb_col = nbcol;
      *nb_lig = (*nb_lig) + nblig;
      image1->POS_OCTET[ific]=0;

      FBANDEAU[ific] = image2;

   }
/*..................................
     Initialisation du type de codage, indicateur d'inversion des octets,
			nombre d'octets par pixel,
  ........................................................*/
     sprintf(image1->COD_PIX,image2->COD_PIX);
     image1->NB_BITS_PIX = image2->NB_BITS_PIX;  
     image1->NBOCT_PIX = image2->NBOCT_PIX;
     image1->INV_OCTETS = image2->INV_OCTETS;

/*..................................
 On stocke l'information "nombre de bandeaux" dans ACCES_CANAL_INT
 qui n'est pas utilisé par ailleurs
  ........................................................*/
     image1->ACCES_CANAL_INT[0] = NbBandeaux;
          
     return(image1);

ERREUR:
    if (fic_in!=NULL) 
    {
	fclose(fic_in);
	fic_in=NULL;
    }
     return (NULL);
}

/* 
   Fin de l'operation cai_ouvre_lecture_bandeau
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_bandeau                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image bandeau              */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_bandeau(image1, canal,    */
/* ------------------                            premiere_ligne,             */
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
/*     cai_lecture_canal_bandeau     (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
CAI_OK_KO cai_lecture_canal_bandeau(CAI_IMAGE *image1 ,  
                                int *canal ,           
    		          	int *premiere_ligne  ,   
     		          	int *premiere_colonne,
                         	int *nombre_lignes_lire ,
                          	int *nombre_colonnes_lire,
                          	int *pas_ligne ,          
                          	int *pas_colonne,
		          	unsigned char *data_image )        

{
 int NbBandeaux;            /* Nombre de bandeaux constituant l'image        */
 int iret;                  /* indicateur d'erreur                           */
 long int oct_pix;          /* nb octets par pixels                          */
 long int oct_lig;          /* nb oct. par ligne en considerant taille pixel */
 int lig,ilig;              /* indice de boucle sur les lignes               */
 int ific;                  /* boucle sur le nombre de bandeaux              */
 CAI_IMAGE *image2;         /* pointeur sur descripteur de fichier image     */
 int BandDeb,Tot;           /* indicateur du bandeau de la premiere ligne    */
 int BandFin,Fin;           /* indicateur du bandeau de la derniere ligne    */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *pch;        /* pointeur chaine pour transfert  buffer sortie */
 int Un;

/*...........................
  Initialisations diverses
  ..........................*/
   NbBandeaux = image1->ACCES_CANAL_INT[0];
   iret=CAI_OK;
   oct_pix=image1->NBOCT_PIX;
   oct_lig=(*nombre_colonnes_lire)*oct_pix;
   lig=*premiere_ligne;
   ilig=1;
   Un=1;
   pch = data_image;

/*..............................................
        allocation de memoire pour lire une ligne image complete
 ............................................. */
   buf_lec = (unsigned char *) calloc(oct_lig,sizeof(unsigned char));
   if (buf_lec == NULL)
   {
      strcpy(CAI_ERREUR,"Probleme allocation memoire");
      goto ERREUR;
   }

/*....................
   Recherche du bandeau dans lequel se trouve la premiere ligne à lire
 ............................................. */
   BandDeb = -1;
   ific=0;
   Tot = 1;
   while ((BandDeb == -1) && (ific<NbBandeaux))
   {
	if (ific != 0)  lig = lig - image2->NB_LIG;
	image2 = FBANDEAU[ific];
 	Tot = Tot + image2->NB_LIG;      
        if ((*premiere_ligne) < Tot) BandDeb=ific;
        ific = ific+1;
   }
   if (BandDeb == -1) 
   {
      strcpy(CAI_ERREUR,"Recherche du premier bandeau");
      goto ERREUR;
   }
/*....................
   Recherche du bandeau dans lequel se trouve la derniere ligne à lire
 ............................................. */
   BandFin = -1;
   ific=0;
   Fin = (*premiere_ligne)+(*nombre_lignes_lire)-1;
   Tot = 0;
   while ((BandFin == -1) && (ific<NbBandeaux))
   {
	image2 = FBANDEAU[ific];
 	Tot = Tot + image2->NB_LIG;  
        if (Fin <= Tot) BandFin=ific;
        ific = ific+1;
   }
   if (BandFin == -1) 
   {
      strcpy(CAI_ERREUR,"Recherche du dernier bandeau");
      goto ERREUR;
   }
   
   ific = BandDeb;
   image2 = FBANDEAU[ific];
   if (BandDeb == BandFin)
   {
         iret = cai_lecture_canal(image2,*canal,
				lig,*premiere_colonne,
				*nombre_lignes_lire,*nombre_colonnes_lire,
				*pas_ligne,*pas_colonne,data_image);
	 if (iret != CAI_OK)
                 goto ERREUR;
   }
   else
   {
      while (ilig <= (*nombre_lignes_lire))
      {
         iret = cai_lecture_canal(image2,*canal,lig,*premiere_colonne,Un,
				  *nombre_colonnes_lire,*pas_ligne,
				  *pas_colonne,buf_lec);
	 if (iret != CAI_OK)
                 goto ERREUR;

	  memcpy ( pch, buf_lec , oct_lig );
          pch = pch+oct_lig;
          ilig=ilig+1;
          lig=lig+(*pas_ligne);
          if (lig > image2->NB_LIG)
          {
		lig = lig - image2->NB_LIG;
                ific=ific+1;
		if (ific<NbBandeaux)
   		   image2 = FBANDEAU[ific];
          }
       }
   } 
               
   return(CAI_OK);

ERREUR:
   return(CAI_KO);
 }
/*
  Fin de l'operation cai_lecture_canal_bandeau
*/ 
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_bandeau                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format bandeau et de       */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_bandeau(image1)                   */
/* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_bandeau        (S) (CAI_OK_KO) :   = CAI_KO si pb           */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_bandeau (CAI_IMAGE *image1)

{
   int iret,retour;     /* code retour de l'operation                    */
   int ific;            /* indice du bandeau a fermer                    */
   int NbBandeaux;      /* Nombre de bandeaux constituant l'image        */
   CAI_IMAGE *image2;   /* pointeur sur descripteur de fichier image     */


   NbBandeaux = image1->ACCES_CANAL_INT[0];
   retour=CAI_OK;

/*.............................................
   Fermeture de tous les bandeaux
  .............................................*/

   for (ific=0;ific<NbBandeaux;ific++)
   {   
	image2 = FBANDEAU[ific];
        iret = cai_ferme_image(image2);
        if (iret != CAI_OK) retour=CAI_KO;
   }

  return (retour);
}

/*
  Fin de l'operation cai_ferme_bandeau
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_bandeau                                       */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en bandeau et de     */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_bandeau(repertoire,nom_ima)    */
/* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_bandeau     (S) (CAI_OK_KO) : = CAI_KO si pb             */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_dest_ima_bandeau ( char *repertoire , 
                     	     char *nom_ima )
{ 
   CAI_IMAGE *image1,*image2;   /* pointeur su descripteur de fichier image */ 
   int NbBandeaux;              /* nombre de bandeaux                       */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char label[80];		/* contient un enregistrement du fichier    */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   char format[20];		/* Format de stockage de chaque bandeau     */
   CH50 tab_ima[20];
   int num,ific;
/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repertoire)== 0)
	sprintf(repertoire,"./");
   sprintf(nom_fic,"%s/%s.band",repertoire,nom_ima);
 
  /*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/* verification si extension en majuscule */
      nom_fic[0]='\0';
      sprintf(nom_fic,"%s/%s.BAND",repertoire,nom_ima);
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
          strcpy(CAI_ERREUR,"Erreur ouverture fichier entete BANDEAU-CNES");
          goto ERREUR;
     }
   }
/*....................................
     Decodage du nombre de BANDEAU
  ............................................*/
     fscanf(fic_in,"%s %d\n",label,&NbBandeaux);
     retour=strncmp(label,"BANDEAU",7);
     if (retour!=0)
     {
       sprintf(CAI_ERREUR,
			"Informations incoherentes ligne BANDEAU dans %s",
			nom_fic);
       goto ERREUR;
     }
/*....................................
     Decodage du nombre du format
  ............................................*/
     fscanf(fic_in,"%s\n",format);

/*....................................
     Recuperation du nom de chaque bandeau
  ............................................*/
   for (ific=0;ific<NbBandeaux;ific++)
   {   
      fscanf(fic_in,"%s %d\n",label,&num);
      strcpy(tab_ima[num-1],label);
   }
         
/*..................................
   Fermeture du fichier et destruction du fichier d'entete 
  ......................................*/
    if (fic_in!=NULL) 
    {
	fclose(fic_in);
	fic_in=NULL;
	unlink ( nom_fic );

    }

/*....................................
    destruction de chaque bandeau
  ............................................*/
   for (ific=0;ific<NbBandeaux;ific++)
   {   
         sprintf(nom_fic,"%s/%s",repertoire,tab_ima[ific]);
	 retour = cai_destruction_image(format,nom_fic);

   }

   return(CAI_OK);

ERREUR:
   return(CAI_KO);

}

/*
 Fin de l'operation cai_dest_ima_bandeau
*/


