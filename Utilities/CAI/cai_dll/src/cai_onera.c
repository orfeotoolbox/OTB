/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_onera.c                                                   */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format onera                                      */ 
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
/* MOD : VERSION : 3.3: DM : optimisation des acces                          */
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : verification que l'image est codee sur un seul */
/*octet par pixel (contrainte du format)                                     */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : modifications open et fopen pour PC NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : erreur ouverture en modification               */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : ecriture image entiere. Correction du test de  */
/*                            retour sur le nb d'octets ecrits               */
/*****************************************************************************/
/* MOD : VERSION : 5.3 : FA : ouverture en lecture/ecriture                  */
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
#define IN_CAI_ONERA

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_onera.c,v 1.10 01/05/31 10:47:12 cmf Exp $";

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
/* OPERATION :    cai_liste_ima_onera                                        */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format onera                  */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_onera(repert,tab_ima)           */
/* -----------------                                                         */
/*                                                                           */
/*     repert        (E) (char *)   : repertoire a explorer		     */
/*     tab_ima       (S) (char ***) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_onera     (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


long int cai_liste_ima_onera (char *repert,
		         	char ***tab_ima)
 

{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */

struct dirent *dp;   /* structure resultat de la fonction readdir   */

   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2,*pch3;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images onera identifiees           */
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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE ONERA
      (EXTENSION =.inf)
   ..........................................*/

   while((dp=readdir(dirp))!=NULL)
   {

     car='.';
     pch0=strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='i';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='n';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
           car='f';
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

    closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT ONERA
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE ONERA
      (EXTENSION =.inf)
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
       car='i';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='n';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
           car='f';
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

    (void)closedir(dirp);

   }
   else
   {
    /* Repertoire inexistant */
      nb_ima=-1;
   }

    return(nb_ima);

}

/* Fin de l'operation cai_liste_ima_onera
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_onera                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format onera     */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_lecture_onera (nom_image,       */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1)          */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_onera   (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_onera(char *repert,
				   char *nom_image,
				   int *nb_can,
				   int *nboct_pix,
				   int *nb_col,
				   int *nb_lig)

{
   int fic;			/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[2];			/* indice du canal traite     		    */
   int num,ican;		/* indice 				    */
   char *buf_lec ;              /* buffer lecture entete onera              */
   long k;			/* indice 				    */

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
 
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   *nb_can=0;

 
   iret=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
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
   if ((long int)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.inf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.inf",nom_image);
   }


/*........................
   Ouverture du fichier entete
  ..........................................*/
   num=open(nom_fic,O_RDONLY|O_BINARY,0);
   if (num == -1)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete ONERA");
    }
   else
   {
     
/*....................................
     Lecture du fichier entete
  ............................................*/
     buf_lec=(char *) malloc(257);
     if (buf_lec==NULL)
     {
       iret=0;
       strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
      }
      else
      {

     k=read(num,buf_lec,256);
     if (k<=0)
      {
         iret=0;
         strcpy(CAI_ERREUR,"Erreur lecture entete Onera");
         close(num);
       }
       else
       {
        buf_lec[k]='\0';
/*.........................................
       Initialisation du nombre d'octets par pixel . Pour onera les pixels
	sont toujours codes sur 1 octet
  ...............................................................*/
	*nboct_pix=1;
        image1->NB_BITS_PIX=8;
        sprintf(image1->COD_PIX,"OCT");
/*.................................
       Decodage de l'enregistrement
  .........................................*/
          deb=buf_lec;
          sscanf(deb,"%d%d%d",nb_col,nb_lig,nb_can);
 	  close(num);
        }
        free(buf_lec);
     }
   }

   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL et ouverture
      de ces fichiers
  .....................................................*/
  
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;

     ican=1;    
     while((ican<=*nb_can)&&(ican<CAI_NB_MAX_CAN))
     {
          sprintf(deb,"%s.%d",nom_image,ican);
          deb=deb+100;
          sprintf(deb1,"%s.%d",nom_image,ican);
          deb1=deb1+100;
          nom_fic[0]='\0';
          if ((long int)strlen(repert)!=0)
          {
    		sprintf(nom_fic,"%s/%s.%d",repert,nom_image,ican);
          }
	  else
          {
             sprintf(nom_fic,"%s.%d",nom_image,ican);
 	  }
          fic=open(nom_fic,O_RDONLY|O_BINARY,0);
          if (fic != -1) 
          {
           image1->ACCES_CANAL_INT[ican-1]=fic;
           image1->POS_OCTET[ican-1]=0;
           ican=ican+1;
          }
	  else
          {
	    iret=0;
            strcpy(CAI_ERREUR,"incoherence au niveau du nb fichiers canaux");
	    return(NULL);
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
   Fin de l'operation cai_ouvre_lecture_onera
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_onera                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format onera et de tous les fichiers qui la composent        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_creation_onera(nom_image,       */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 )         */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_onera   (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_onera(char *repert,
				    char *nom_image,
				    int *nb_can,
				    int *nboct_pix,
				    int *nb_col,
				    int *nb_lig,
				    char *label)

{
   FILE *fic;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   int retour;			/* retour de fonction 			    */ 
   char car[2];			/* indice du canal traite     		    */
   int i,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */

/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : verification que l'image est codee sur un seul */
/*octet par pixel (contrainte du format)                                     */
/*****************************************************************************/
   if (*nboct_pix != 1)
   {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image onera ne peutetre codee que sur un seul octet par pixel");
	goto ERREUR;
   }


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
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
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.inf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.inf",nom_image);
   }

   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s ONERA existe \n" , nom_image );
	goto ERREUR;
   }

/*...............................................................
  Pour chaque canal de l'image,constitution du nom de fichier correspondant
  Verification qu'il n'existe pas deja
  Ouverture du fichier en ecriture
  .................................................................*/

  for (i=0;i<*nb_can;i=i+1)
  {
   nom_fic_canal[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic_canal,"%s/%s.%d",repert,nom_image,i+1);
   }
   else
   {
      sprintf(nom_fic_canal,"%s.%d",nom_image,i+1); 
   }
   retour = stat ( nom_fic_canal , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s ONERA existe \n" , nom_image );
	goto ERREUR;
   }
   
    num = open(nom_fic_canal,O_RDWR|O_CREAT|O_BINARY,PERMS); 
   if (num == -1)
   {
     sprintf(CAI_ERREUR,"Erreur: Creation %s canl impossible\n",nom_fic_canal);
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
      sprintf(CAI_ERREUR,
	"Erreur: Creation fichier entete  %s impossible\n",
	nom_fic);
      goto ERREUR;
   }
   fprintf ( fic , "%d\n" , *nb_col );
   fprintf ( fic , "%d\n" , *nb_lig);
   fprintf ( fic , "%d" , *nb_can );


/*************************************************/
/* Fermeture du fichier                          */
/*************************************************/

 if (fic!=NULL)  fclose ( fic );
  return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_onera
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_onera                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image onera                */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_onera(image1, canal,      */       		      /* ------------------                              premiere_ligne,           */
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
/*     cai_lecture_canal_onera   (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_onera(CAI_IMAGE *image1 ,  
                           	  int *canal ,           
    		            	  int *premiere_ligne  ,   
     		          	  int *premiere_colonne,
                          	  int *nombre_lignes_lire ,
                          	  int *nombre_colonnes_lire,
                          	  int *pas_ligne ,          
                          	  int *pas_colonne,
		          	  unsigned char *data_image )        

{
 int   num,i,j;             /* variables de travail                          */
 long pt_oct,n;		    /* position courante dans le fichier image       */
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
             strcpy(CAI_ERREUR,"Erreur lecture donnee image ONERA");
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
        while ((i <= *nombre_lignes_lire)&&(iret==CAI_OK))
        {  
/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
            pt_oct=lseek(num,0L,1);

/*................................
            Calcul du numero du 1er octet a lire
  ....................................*/
            no_oct=(lig-1)*image1->NB_COL;

/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
            nb_oct=(long)(no_oct-pt_oct);
            lseek(num,nb_oct,1);

/*...........................................
            lecture d'une ligne image
  ................................................*/
            n=read(num,buf_lec,image1->NB_COL);

            if (n<=0)
            {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image onera");
            }
            else
            { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/           
             deb=buf_lec+(*premiere_colonne-1);
             j=1;
             while (j <= *nombre_colonnes_lire)
	     {                         
              *pch=*deb;
              pch=pch+1;
              j=j+1;
              deb=deb+*pas_colonne;
             }   
             
            }
          i=i+1;
          lig=lig+*pas_ligne;

        }        

	free(buf_lec);
      }
   }

   return(iret);
}
    
/*
  Fin de l'operation cai_lecture_canal_onera
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_onera                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image onera                */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_OK_KO cai_ecriture_canal_onera(image1, canal,      */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_onera      (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_onera (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image ) 
{
long	position_first_lig;	/* position de la 1ere ligne a ecrire 	*/
long	position_courante;	/* position courante dans le fichier 	*/
long	deplacement;		/* deplacement a effectuer		*/
int	i_ligne;		/* compteur de lignes			*/
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire */
long	cr_write;		/* code retour de write			*/
int num;                        /* descripteur de fichier 		*/
int nboct_pix;			/* nombre d'octets par pixel            */
int tot_pix;                    /* nb de pixels total a ecrire          */

/* ................
   Initialisations 
  .................*/
i_ligne = 0;
num = image1->ACCES_CANAL_INT[*canal-1];
nboct_pix = image1->NBOCT_PIX;


/*************************************************/
/* Les donnees doivent etre codees sur 8 bits	 */
/*************************************************/

if ( nboct_pix != 1 )
{
	strcpy ( CAI_ERREUR , "Erreur : les donnees doivent etre codees sur 8 bits\n");
	goto ERREUR;
}
/*.......................................*/
/* verification si ecriture image entiere */
/*.......................................*/
  if ((*nombre_lignes_ecrire==image1->NB_LIG)&&
		(*nombre_colonnes_ecrire==image1->NB_COL))
  {
       tot_pix=nboct_pix*(image1->NB_COL)*(image1->NB_LIG);

    /* positionnement en debut de fichier */
       position_first_lig=lseek(num,0L,0);

/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : ecriture image entiere. Correction du test de  */
/*                            retour sur le nb d'octets ecrits               */
/*           tot_pix au lieu de nboct_pix                                    */
/*****************************************************************************/
   /* ecriture du canal complet */
      cr_write = write ( num , data_image,tot_pix);
      if (cr_write != tot_pix)
      {
              strcpy(CAI_ERREUR,"Erreur ecriture donnee image ONERA");
             goto ERREUR;
       }
   }
  else
  {

/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

position_first_lig =
	(long) ( image1->POS_OCTET[*canal-1] +
        (*premiere_ligne - 1) * (*nombre_colonnes_ecrire) );


/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

position_courante = lseek ( num , 0L , 1 );


/********************************************************/
/* Deplacement jusqu'a la premiere ligne a ecrire	*/
/********************************************************/

deplacement = position_first_lig - position_courante;
lseek ( num, deplacement , 1 ); 


/********************************************************/
/* TANT QUE il y a des lignes a ecrire                 	*/
/********************************************************/
while (i_ligne < *nombre_lignes_ecrire)
{

/********************************************************/
/*	extraction de la ligne du tableau 		*/
/********************************************************/

	buff_lig = data_image + (*nombre_colonnes_ecrire) * i_ligne;

/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/

	cr_write = write ( num , buff_lig ,(*nombre_colonnes_ecrire) );
	if ( cr_write !=(*nombre_colonnes_ecrire) )
	{
		strcpy ( CAI_ERREUR , "Erreur ecriture canal image onera \n" );
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
  Fin de l'operation cai_ecriture_canal_onera
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_onera                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format onera et de         */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_onera(image1)                     */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_onera      (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_onera (CAI_IMAGE *image1)


{
 int iret;    /* code retour de l'operation      */
 int i_can;   /* indice du canal a fermer        */
 int num;     /* descripteur du fichier a fermer */


 iret=CAI_OK;
/*.............................................
   Fermeture des fichiers image 
  .............................................*/
   for (i_can=0;i_can<image1->NB_CANAUX;i_can=i_can+1)
   {
     num=image1->ACCES_CANAL_INT[i_can];
     if (num>0) close(num);
    }


  return (iret);
}

/*
  Fin de l'operation cai_ferme_onera
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_onera                                         */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en onera et de       */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_onera(format,nom_ima)          */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_onera   (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_dest_ima_onera ( char *repertoire , 
                      	       char *nom_ima )

{

int	i_can;			/* compteur de canaux 			     */
long     iret;                   /* code retour fonction fermetrue lecture    */
char	str_num_canal[3];	/* suffixe numero du canal		     */
char	nom_fic_entete[1024];	/* nom du fichier entete a detruire          */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image		     */
char	nom_fic_canal[1024];	/* nom du fichier canal a detruire 	     */
int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier     */ 
CAI_IMAGE   *image1;             /* structure ouverture image 		     */
int         nb_can;	        /* nombre de canaux de l'image 		     */
int         nboct_pix;		/* nombre d'octets par pixel image 	     */
int         nb_col;	        /* nombre de colonnes de l'image 	     */
int         nb_lig;             /* nombre de lignes de l'image		     */

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/
if ((long)strlen(repertoire)!= 0)
{
  sprintf ( nom_ima_complet, "%s/%s",repertoire,nom_ima );

}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}
sprintf(nom_fic_entete ,"%s.inf", nom_ima_complet);

/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_onera			*/
/************************************************/

  image1 = cai_ouvre_lecture_onera ( repertoire,nom_ima, &nb_can ,&nboct_pix ,
				     &nb_col , &nb_lig);	
  if ( image1 == NULL) goto ERREUR;

  image1->NB_CANAUX=nb_can;
  image1->NB_COL=nb_col;
  image1->NB_LIG=nb_lig;
  image1->NBOCT_PIX=nboct_pix;

iret=cai_ferme_onera(image1);
image1 = NULL;

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
	sprintf( nom_fic_canal , "%s.%d" ,nom_ima_complet,i_can+1);
  	cr_stat = stat ( nom_fic_canal , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_canal );
}


return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/*
 Fin de l'operation cai_dest-ima_onera
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_onera                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     onera et de tous les fichiers qui la composent                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_modifie_onera (nom_image,       */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 )         */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_onera   (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_modifie_onera(char *repert,
				   char *nom_image,
				   int  *nb_can,
				   int  *nboct_pix,
				   int  *nb_col,
				   int  *nb_lig)

{
   int fic;			/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[2];			/* indice du canal traite     		    */
   int num,ican;		/* indice 				    */
   char *buf_lec ;              /* buffer lecture entete onera              */
   long k;			/* indice 				    */

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
 
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   *nb_can=0;

 
   iret=1;

 image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
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
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.inf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.inf",nom_image);
   }


/*........................
   Ouverture du fichier entete
  ..........................................*/
   num=open(nom_fic,O_RDWR|O_BINARY,0);
   if (num == -1)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete ONERA");
    }
   else
   {     
/*....................................
     Lecture du fichier entete
  ............................................*/
     buf_lec=(char *) malloc(257);
     if (buf_lec==NULL)
     {
       iret=0;
       strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_modifie");
     }
    else
    {

     k=read(num,buf_lec,256);
     if (k<=0)
      {
         iret=0;
         strcpy(CAI_ERREUR,"Erreur lecture entete Onera");
         close(num);
       }
       else
       {
        buf_lec[k]='\0';
/*.........................................
       Initialisation du nombre d'octets par pixel . Pour onera les pixels
	sont toujours codes sur 1 octet
  ...............................................................*/
	*nboct_pix=1;
        image1->NB_BITS_PIX=8;
        sprintf(image1->COD_PIX,"OCT");
/*.................................
       Decodage de l'enregistrement
  .........................................*/
          deb=buf_lec;
          sscanf(deb,"%d%d%d",nb_col,nb_lig,nb_can);
 	  close(num);
        }
        free(buf_lec);
     }
   }

   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL et ouverture
      de ces fichiers
  .....................................................*/
  
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;

     ican=1;    
     while((ican<=*nb_can)&&(ican<CAI_NB_MAX_CAN))
     {
          sprintf(deb,"%s.%d",nom_image,ican);
          deb=deb+100;
          sprintf(deb1,"%s.%d",nom_image,ican);
          deb1=deb1+100;
          nom_fic[0]='\0';
          if ((long int)strlen(repert)!=0)
          {
    		sprintf(nom_fic,"%s/%s.%d",repert,nom_image,ican);
          }
	  else
          {
             sprintf(nom_fic,"%s.%d",nom_image,ican);
 	  }
/*****************************************************************************/
/* MOD : VERSION : 5.3 : FA : ouverture en lecture/ecriture                  */
/*****************************************************************************/
          fic=open(nom_fic,O_RDWR|O_BINARY,0);
          if (fic != -1) 
          {
           image1->ACCES_CANAL_INT[ican-1]=fic;
           image1->POS_OCTET[ican-1]=0;
           ican=ican+1;
          }
	  else
          {
	    iret=0;
            strcpy(CAI_ERREUR,"incoherence au niveau du nb fichiers canaux");
	    return(NULL);
          }
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
   Fin de l'operation cai_ouvre_modifie_onera
*/
