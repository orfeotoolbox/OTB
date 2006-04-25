/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_arcinfo.c                                                 */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format arcinfo version 7.4 et superieure          */ 
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

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <sys/types.h>
#include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>

#define _CAI_IMAGE
# include "cai_image.h"


/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_arcinfo.c,v 1.5 00/11/29 11:02:59 cmf Exp $";


extern char *getenv();

static void invert_oct();

/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_ima_arcinfo                                      */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format arcinfo                */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_arcinfo(repert,tab_ima)         */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_arcinfo   (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

int cai_liste_ima_arcinfo (repert,
		           tab_ima)

char *repert;
char ***tab_ima;

{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2,*pch3;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images arcinfo identifiees         */
   int i;               /* indice de boucle                            */
 

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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE ARCINFO
      (EXTENSION =.lan ou .gis)
     ..........................................*/

   while((dp=readdir(dirp))!=NULL)
   {

     car='.';
     pch0=strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='g';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='i';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
           car='s';
           pch3=strrchr(dp->d_name,car);
           if ((pch3 != NULL)&&(pch3==pch2+1))
           {
              *pch0='\0';
              *pch1=' ';
	      *pch2=' ';
              *pch3=' ';
	      nb_ima=nb_ima+1;
           }
         }
       }
       else
       {
         car='l';
         pch1=strrchr(dp->d_name,car);
         if ((pch1 != NULL)&&(pch1==pch0+1))
         {
           car='a';
           pch2=strrchr(dp->d_name,car);
           if ((pch2 != NULL)&&(pch2==pch1+1))
           {
             car='n';
             pch3=strrchr(dp->d_name,car);
             if ((pch3 != NULL)&&(pch3==pch2+1))
             {
                *pch0='\0';
                *pch1=' ';
	        *pch2=' ';
                *pch3=' ';
	        nb_ima=nb_ima+1;
             }
           }
         }
       }
     }
   }

    (void)closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT ARCINFO
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE ARCINFO
       (EXTENSION =.lan ou .gis)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
 
   while((dp=readdir(dirp))!=NULL)
   {

     car='.';
     pch0=strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='g';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='i';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
           car='s';
           pch3=strrchr(dp->d_name,car);
           if ((pch3 != NULL)&&(pch3==pch2+1))
           {
              *pch0='\0';
              *pch1=' ';
	      *pch2=' ';
              *pch3=' ';
              strcpy((*tab_ima)[nb_ima],dp->d_name);
	      nb_ima=nb_ima+1;
           }
         }
       }
       else
       {
         car='l';
         pch1=strrchr(dp->d_name,car);
         if ((pch1 != NULL)&&(pch1==pch0+1))
         {
           car='a';
           pch2=strrchr(dp->d_name,car);
           if ((pch2 != NULL)&&(pch2==pch1+1))
           {
             car='n';
             pch3=strrchr(dp->d_name,car);
             if ((pch3 != NULL)&&(pch3==pch2+1))
             {
                *pch0='\0';
                *pch1=' ';
	        *pch2=' ';
                *pch3=' ';
                strcpy((*tab_ima)[nb_ima],dp->d_name);
	        nb_ima=nb_ima+1;
             }
           }
         }
       }
     }
   }

    (void)closedir(dirp);

   }
   else
   {
     /* Erreur : Repertoire inexistant */
      nb_ima=-1;
   }

    return(nb_ima);

}
/* Fin de l'operation cai_liste_ima_arcinfo
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_arcinfo                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format arcinfo   */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_arcinfo(nom_image,       */
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
/*     cai_ouvre_lecture_arcinfo (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_arcinfo(repert,
				     nom_image,
				     nb_can,
				     nboct_pix,
				     nb_col,
				     nb_lig)

char *repert;
char *nom_image;
int  *nb_can;
int  *nboct_pix;
int  *nb_col;
int  *nb_lig;
{
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   short ncan;			/* nombre de canaux dans le fichier en-tete */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[5];			/* indice du canal traite     		    */
   char entete[6];		/* HEADER ou HEAD74 lu dans l'entete        */
   int i,num;			/* indices				    */
   short ipack;                 /* variables de travail                     */
   int lg,col;                  /* variables de travail                     */


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*.................................................
   Constituion du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if (strlen(repert)!= 0)
   {
     sscanf(repert,"%s",nom_fic);
     strcat(nom_fic,"/");
     strcat(nom_fic,nom_image);
     strcat(nom_fic,".lan");
     nom_fic[strlen(repert)+1+strlen(nom_image)+4]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".lan"); 
      nom_fic[strlen(nom_image)+4]='\0';
   }

/*........................
   Ouverture du fichier entete avec l'extension .lan
  ..........................................*/
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*......................................................
    Test sur l'existence du fichier avec l'extension .gis
  .......................................................*/

     strncpy(&nom_fic[strlen(nom_fic)-4],".gis",4);

     fic_in = fopen(nom_fic,"r");
     if (fic_in == NULL)
     {
        iret=0;
        strcpy(CAI_ERREUR,"Erreur ouverture fichier entete ARCINFO");
      }
    }
    if (iret!=0)
    {
	 

	/*-------------------------------------------
          Verification de l'entete
	  -------------------------------------------*/
	  fread(entete,6,1,fic_in);
           if (strncmp(entete,"HEAD74",6)!=0)
          {
 	     iret = 0;
	     strcpy(CAI_ERREUR,"Version autre que 7.4 non traitee");
             goto ERREUR;
          }
      


	/*-------------------------------------------
	  recuperation du nombre d'octets par pixel
	  -------------------------------------------*/
	  fread(&ipack,2,1,fic_in);
          lg=sizeof(ipack)-1;
	  invert_oct(&ipack,lg);
	  if (ipack == 0)
		*nboct_pix = 1;
	  else if (ipack == 2)
		*nboct_pix = 2;
	  else
	  {
		iret = 0;
		strcpy(CAI_ERREUR,"Erreur : les donnees image doivent etre codees sur 8 ou 16 octets");
             goto ERREUR;

	  }

	/*-------------------------------------------
	  recuperation du nombre de canaux
	  -------------------------------------------*/
	  fread(&ncan,2,1,fic_in);
          lg=sizeof(ncan)-1;
	  invert_oct(&ncan,lg);
	  *nb_can = (int) ncan;

	/*-------------------------------------------
          positionnement sur le nombre de colonnes
	  -------------------------------------------*/
	  fseek(fic_in,6,1);

	/*-------------------------------------------
	  recuperation du nombre de colonnes
	  -------------------------------------------*/
	  fread(nb_col,4,1,fic_in);
	  lg = sizeof(*nb_col)-1;
	  invert_oct(nb_col,lg);
	  col=*nb_col;

	/*-------------------------------------------
	  recuperation du nombre de lignes
	  -------------------------------------------*/
	  fread(nb_lig,4,1,fic_in);
	  lg = sizeof(*nb_lig)-1;
	  invert_oct(nb_lig,lg);

	/*-------------------------------------------
	  Fermeture du fichier
	  -------------------------------------------*/
          fclose(fic_in);
    }
   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
    if (*nb_can == 1)
		strcpy(car,".gis");
    else
		strcpy(car,".lan");
 
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;

     for (i=0;i<*nb_can;i=i+1)
     {
       num=open(nom_fic,O_RDONLY,0);
       if (num!= -1)
       {
           sscanf(nom_image,"%s",deb);
           strcat(deb,car);
  	   deb=deb+100;
           sscanf(nom_image,"%s",deb1);
           strcat(deb1,car);
           deb1=deb1+100;
           image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=128+(i*col*(*nboct_pix));
	 }
       
        }
     }

    if (iret == 0)
    {
	return (NULL);
     }
    else
    {
      image1->NB_BITS_PIX=*nboct_pix;
      return(image1);
     }
ERREUR:

   return (NULL);

}
/* 
   Fin de l'operation cai_ouvre_lecture_arcinfo
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_arcinfo                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format arcinfo et de tous les fichiers qui la composent      */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_arcinfo(nom_image,       */
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
/*     cai_ouvre_creation_arcinfo (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_arcinfo(repert,
				      nom_image,
				      nb_can,
				      nboct_pix,
				      nb_col,
				      nb_lig,
				      label)

char *repert;
char *nom_image;
int  *nb_can;
int  *nboct_pix;
int  *nb_col;
int  *nb_lig;
char *label;
{
   FILE *fic;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   int retour;			/* retour de fonction 			    */ 
   int i,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */

  short	ipack;			/* indicateur de pack type des donnees      */
  short	short_tempo;		/* short utilise pour inversion d'octets    */
  int	int_tempo;		/* entier utilise pour inversion d'octets   */
  int	lg;			/* longueur de l'entier - 1                 */
  int taille;                   /* longueur de l'entete a ecrire            */
  unsigned char *buff;          /* permet d'ecrire l'entete fichier arcinfo */





/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   ipack = 0;
 
/*.................................................
   Constitution du nom du fichier et verification qu'il
   n'existe pas deja
   pour un canal ,l'extension est .gis, sinon .lan
  .................................................*/
   nom_fic[0]='\0';
   if (strlen(repert)!= 0)
   {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      if (*nb_can == 1)
       strcat(nom_fic,".gis");
      else
       strcat(nom_fic,".lan");
      nom_fic[strlen(repert)+1+strlen(nom_image)+4]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      if (*nb_can == 1)
       strcat(nom_fic,".gis");
     else
       strcat(nom_fic,".lan"); 
      nom_fic[strlen(nom_image)+4]='\0';
   }


   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s ARCINFO existe \n" , nom_image );
	goto ERREUR;
   }


/*......................................................
  Ouverture du fichier en ecriture 
  Ecriture de l'entete
  ..........................................................*/
  fic = fopen(nom_fic,"w");
  if (fic == NULL)
  {
      sprintf(CAI_ERREUR,"Erreur: Creation fichier %s impossible\n",nom_fic);
      goto ERREUR;
   }
   taille=128 + (*nb_can)*(*nb_col)*(*nboct_pix)*(*nb_lig) ;

   buff = (unsigned char *) malloc(taille);
   if ( fic == NULL )
   {
	strcpy ( CAI_ERREUR , "Erreur : probleme d'allocation \n" );
	unlink(nom_fic);
	goto ERREUR;
   }

   memcpy ( buff , "HEAD74" , 6 );
   if ( *nboct_pix == 1 )
	ipack = 0;
   else if ( *nboct_pix == 2 )
	ipack = 2;
    memcpy ( &buff[6] , &ipack , 2 );

    short_tempo = (short) (*nb_can);
    lg = sizeof(short_tempo)-1;
    invert_oct(&short_tempo,lg);
    memcpy ( &buff[8] , &short_tempo , 2 );

    for ( i = 10 ; i < 16 ; i++ )
    {
	buff[i] = 0;
     }

    int_tempo = *nb_col;
    lg = sizeof(int_tempo)-1;
    invert_oct(&int_tempo,lg);
    memcpy ( &buff[16] , &int_tempo , 4 );

    int_tempo = *nb_lig;
    lg = sizeof(int_tempo)-1;
    invert_oct(&int_tempo,lg);
    memcpy ( &buff[20] , &int_tempo , 4 );

    for ( i=24 ; i<taille ; i++ )
    {
	buff[i] = 0;
    }

    retour = fwrite ( buff , taille ,1 , fic);
    if ( retour == 0 )
    {
      sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n" ,nom_fic );
	unlink(nom_fic);
	goto ERREUR;
     }

     free (buff );
     fclose(fic);

/*...........................................
   Ouverture du fichier pour ecriture radiometrie
   ...............................................*/
    num = open(nom_fic,O_RDWR,0);
    if (num == -1)
    {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic);
     goto ERREUR;
    }
    for (i=0;i<*nb_can;i=i+1)
    {
      image1->ACCES_CANAL_INT[i]=num;
      image1->POS_OCTET[i]=128;
    }
  
  return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_arcinfo
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_arcinfo                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image Arcinfo              */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_arcinfo(image1, canal,    */       		      /* ------------------                        premiere_ligne,                 */
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
/*     cai_lecture_canal_arcinfo (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_arcinfo(image1 ,  
                                    canal ,           
    		                    premiere_ligne  ,   
     		                    premiere_colonne,
                                    nombre_lignes_lire ,
                                    nombre_colonnes_lire,
                                    pas_ligne ,          
                                    pas_colonne,
		                   data_image )        
CAI_IMAGE *image1;
int *canal ;          
int *premiere_ligne  ;   
int *premiere_colonne   ;
int *nombre_lignes_lire ;
int *nombre_colonnes_lire;
int *pas_ligne ;                              
int *pas_colonne;        
unsigned char *data_image;

{

 int   num,i,j,k,n;         /* variables de travail                          */
 int pt_oct;		    /* position courante dans le fichier image       */
 int no_oct;                /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int   oct_pix;             /* nb octets par pixels                          */
 long  nb_oct;              /* nb oct =1er octet a lire - position courante  */
 long  oct_lig;             /* nb oct. par ligne en considerant taille pixel */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */


/*...........................
  Initialisations diverses
  ..........................*/
   iret=CAI_OK;
   num=image1->ACCES_CANAL_INT[*canal-1];
   oct_pix=image1->NBOCT_PIX;
   oct_lig=oct_pix*image1->NB_COL;

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
	          (lig-1)*image1->NB_COL*oct_pix*image1->NB_CANAUX;


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
             strcpy(CAI_ERREUR,"Erreur lecture donnee image ARCINFO");
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
   }
   return(iret);
}
/*
  Fin de l'operation cai_lecture_canal_arcinfo
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_arcinfo                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image arcinfo              */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_arcinfo(image1, canal,   */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_arcinfo    (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ecriture_canal_arcinfo (image1 ,  
                                      canal ,           
    		                      premiere_ligne  ,   
                                      nombre_lignes_ecrire ,
                                      nombre_colonnes_ecrire,
		                      data_image )        
CAI_IMAGE *image1;
int *canal ;          
int *premiere_ligne  ;   
int *nombre_lignes_ecrire ;
int *nombre_colonnes_ecrire;
unsigned char *data_image;

{
long	position_first_lig;	/* position de la 1ere ligne a ecrire    */
long	position_courante;	/* position courante dans le fichier     */
long	deplacement;		/* deplacement a effectuer               */
int	i_ligne;		/* compteur de lignes                    */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire  */
int	cr_write;		/* code retour de write                  */
int num;                        /* descripteur de fichier                */
int nboct_pix;			/* nombre d'octets par pixel             */
int nb_canaux;			/* nombre de canaux de l'image totale    */

/* ................
   Initialisations 
  .................*/

i_ligne = 0;
num = image1->ACCES_CANAL_INT[*canal-1];
nboct_pix = image1->NBOCT_PIX;
nb_canaux = image1->NB_CANAUX;
position_first_lig = 0;
position_courante = 0;
deplacement = 0;

/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

position_first_lig = (long) ( image1->POS_OCTET[*canal-1] + 
     ((*canal) - 1) * (*nombre_colonnes_ecrire) * nboct_pix
 + (*premiere_ligne - 1) * (*nombre_colonnes_ecrire) * nboct_pix * nb_canaux );


/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

position_courante = lseek ( num , 0L , 1 );


/********************************************************/
/* Calcul du deplacement jusqu'a la 1ere ligne a ecrire	*/
/********************************************************/

deplacement = position_first_lig - position_courante;


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
/*	deplacement jusqu'a la ligne a ecrire		*/
/********************************************************/

	lseek ( num , deplacement , 1 ); 

/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/

	cr_write = write ( num , buff_lig , (*nombre_colonnes_ecrire)*nboct_pix );
	if ( cr_write != (*nombre_colonnes_ecrire)*nboct_pix )
	{
		strcpy ( CAI_ERREUR , "Erreur ecriture canal image arcinfo\n" );
		goto ERREUR;
	}
 
/********************************************************/
/*	mise a jour du deplacement jusqu'a la ligne	*/
/*	suivante                                   	*/
/********************************************************/

	deplacement = ( nb_canaux - 1 ) * (*nombre_colonnes_ecrire)*nboct_pix;

/********************************************************/
/*	incrementation du nombre de lignes		*/
/********************************************************/

	i_ligne++;

/********************************************************/
/* FIN TANT QUE						*/
/********************************************************/
}


return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/* 
  Fin de l'operation cai_ecriture_canal_arcinfo
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_arcinfo                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format arcinfo et de       */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_arcinfo(image1)                   */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_arcinfo    (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO  cai_ferme_arcinfo (image1)

CAI_IMAGE *image1;

{

 int iret;    /* code retour de l'operation      */
 int i_can;   /* indice du canal a fermer        */
 int num;     /* descripteur du fichier a fermer */

 iret=CAI_OK;
/*.............................................
   Fermeture du fichier qui contient tous les canaux
  .............................................*/
  num=image1->ACCES_CANAL_INT[0];
  close(num);


  return (iret);
}

/*
  Fin de l'operation cai_ferme_arcinfo
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_arcinfo                                       */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en arcinfo et de     */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_arcinfo(format,nom_ima)        */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_arcinfo (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_dest_ima_arcinfo ( repertoire , 
                                 nom_ima )

char	*repertoire;		/* repertoire de stockage de l'image */
char	*nom_ima;		/* nom de l'image */

{
char	nom_fic_ima[1024];	/* nom du fichier entete a detruire */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image */
int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 


/************************************************/
/* Constitution du nom du fichier       	*/
/************************************************/
if (strlen(repertoire) != 0)
{
 strcpy ( nom_ima_complet , repertoire );
 strcat ( nom_ima_complet, "/" );
 strcat ( nom_ima_complet , nom_ima );
}
else
{
 strcpy ( nom_ima_complet , nom_ima );
}
 strcpy(nom_fic_ima , nom_ima_complet);
 strcat ( nom_fic_ima , ".gis" );
 nom_fic_ima[strlen(nom_ima_complet)+4]='\0';
 
/* le nombre de canaux etant inconnu, on construit */
/* d'abord le nom d'un fichier image avec un seul  */
/* canal et on teste son existence; si un tel      */
/* fichier n'existe pas, on construit le nom d'un  */
/* fichier image avec plusieurs canaux             */

cr_stat = stat ( nom_fic_ima , &buf_stat );
if ( cr_stat != 0 )
{
	strncpy ( &nom_fic_ima[strlen(nom_fic_ima)-4] , ".lan" , 4 );
	cr_stat = stat ( nom_fic_ima , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_ima );
}
else
{
	unlink ( nom_fic_ima );
}

 
return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_arcinfo
*/

/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS INTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    invert_oct                                                 */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    Inversion des octets poids forts,poids faibles                  */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      void invert_oct(s,nboct)                          */
/* -----------------                                                         */
/*                                                                           */
/*     s              (E/S) (char *) : chaine a inverser                     */
/*     nboct          (E)   (int)    : longueur de la chaine                 */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


static void invert_oct ( s , nboct )
char *s;
int nboct;
{
	int c;
	char *t;

	for ( t=s+nboct ; s<t ; s++,t-- )
	{
		c=*s;
		*s=*t;
		*t=c;
	}
}
/* Fin de l'operation invert_oct */
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_arcinfo                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format arcinfo   */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_arcinfo(nom_image,       */
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
/*     cai_ouvre_modifie_arcinfo (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_arcinfo(repert,
				     nom_image,
				     nb_can,
				     nboct_pix,
				     nb_col,
				     nb_lig)

char *repert;
char *nom_image;
int  *nb_can;
int  *nboct_pix;
int  *nb_col;
int  *nb_lig;
{
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   short ncan;			/* nombre de canaux dans le fichier en-tete */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[5];			/* indice du canal traite     		    */
   char entete[6];		/* HEADER ou HEAD74 lu dans l'entete        */
   int i,num;			/* indices				    */
   short ipack;                 /* variables de travail                     */
   int lg,col;                  /* variables de travail                     */


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*.................................................
   Constituion du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if (strlen(repert)!= 0)
   {
     sscanf(repert,"%s",nom_fic);
     strcat(nom_fic,"/");
     strcat(nom_fic,nom_image);
     strcat(nom_fic,".lan");
     nom_fic[strlen(repert)+1+strlen(nom_image)+4]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".lan"); 
      nom_fic[strlen(nom_image)+4]='\0';
   }

/*........................
   Ouverture du fichier entete avec l'extension .lan
  ..........................................*/
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*......................................................
    Test sur l'existence du fichier avec l'extension .gis
  .......................................................*/

     strncpy(&nom_fic[strlen(nom_fic)-4],".gis",4);

     fic_in = fopen(nom_fic,"r");
     if (fic_in == NULL)
     {
        iret=0;
        strcpy(CAI_ERREUR,"Erreur ouverture fichier entete ARCINFO");
      }
    }
    if (iret!=0)
    {
	 

	/*-------------------------------------------
          Verification de l'entete
	  -------------------------------------------*/
	  fread(entete,6,1,fic_in);
           if (strncmp(entete,"HEAD74",6)!=0)
          {
 	     iret = 0;
	     strcpy(CAI_ERREUR,"Version autre que 7.4 non traitee");
             goto ERREUR;
          }
      


	/*-------------------------------------------
	  recuperation du nombre d'octets par pixel
	  -------------------------------------------*/
	  fread(&ipack,2,1,fic_in);
          lg=sizeof(ipack)-1;
	  invert_oct(&ipack,lg);
	  if (ipack == 0)
		*nboct_pix = 1;
	  else if (ipack == 2)
		*nboct_pix = 2;
	  else
	  {
		iret = 0;
		strcpy(CAI_ERREUR,
	"Erreur : les donnees image doivent etre codees sur 8 ou 16 octets");
             goto ERREUR;

	  }

	/*-------------------------------------------
	  recuperation du nombre de canaux
	  -------------------------------------------*/
	  fread(&ncan,2,1,fic_in);
          lg=sizeof(ncan)-1;
	  invert_oct(&ncan,lg);
	  *nb_can = (int) ncan;

	/*-------------------------------------------
          positionnement sur le nombre de colonnes
	  -------------------------------------------*/
	  fseek(fic_in,6,1);

	/*-------------------------------------------
	  recuperation du nombre de colonnes
	  -------------------------------------------*/
	  fread(nb_col,4,1,fic_in);
	  lg = sizeof(*nb_col)-1;
	  invert_oct(nb_col,lg);
	  col=*nb_col;

	/*-------------------------------------------
	  recuperation du nombre de lignes
	  -------------------------------------------*/
	  fread(nb_lig,4,1,fic_in);
	  lg = sizeof(*nb_lig)-1;
	  invert_oct(nb_lig,lg);

	/*-------------------------------------------
	  Fermeture du fichier
	  -------------------------------------------*/
          fclose(fic_in);
    }
   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
    if (*nb_can == 1)
		strcpy(car,".gis");
    else
		strcpy(car,".lan");
 
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;

     for (i=0;i<*nb_can;i=i+1)
     {
       num=open(nom_fic,O_RDWR,0);
       if (num!= -1)
       {
           sscanf(nom_image,"%s",deb);
           strcat(deb,car);
  	   deb=deb+100;
           sscanf(nom_image,"%s",deb1);
           strcat(deb1,car);
           deb1=deb1+100;
           image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=128+(i*col*(*nboct_pix));
	 }
       
        }
     }

    if (iret == 0)
    {
	return (NULL);
     }
    else
    {
      image1->NB_BITS_PIX=*nboct_pix;
      return(image1);
     }
ERREUR:

   return (NULL);

}
/* 
   Fin de l'operation cai_ouvre_modifie_arcinfo
*/
