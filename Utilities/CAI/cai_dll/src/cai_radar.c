/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_radar.c                                                   */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format radar                                      */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CS)                                     */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    Juin 2001                                           */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V5.1                                                            */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 1.2 : FA : IMA-FA-011 pb inversion des octets complexes   */
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Maj du tag COD_PIX lorsque l'image est codée  */
/*                                            en reel  a partir du TYPE_CODE */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
#ifndef _MSC_VER
# include <strings.h>
#endif
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
#include <errno.h>

#define _CAI_IMAGE
#define IN_CAI_RADAR

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header$";

extern char *getenv();

#define FREE(A)        { if ((A)!=NULL) { free(A); (A)=NULL; } }

/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_ima_radar                                        */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format radar                  */
/* ------                                                                    */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_radar(repert,tab_ima)           */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_radar  (S) (long int)   : = -1 repertoire inexistant    */ 
/*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_ima_radar (char *repert,
		       char ***tab_ima)
{

   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   int nb_ima;          /* nombre d'images radar identifiees           */
   int ind;		/* indice de boucles			       */
 
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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE RADAR_CNES
      (EXTENSION =.rad ou .RAD)
   ..........................................*/
   while((dp=readdir(dirp))!=NULL)
   {
/* recherche de l'extension en minuscule */
       pch0 = strstr(dp->d_name,".rad");
       if (pch0==NULL)
       {
/* recherche de l'extension en majuscule */
	 pch1=strstr(dp->d_name,".RAD");
	 if ((pch1 != NULL)&&(*(pch1+4) == '\0'))
         {
		nb_ima=nb_ima+1;
         }
       }
      else
      {
	 if (*(pch0+4) == '\0')
		nb_ima=nb_ima+1;
        }
     }
    (void)closedir(dirp);

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT RADAR
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (ind=0;ind<nb_ima;ind++)
      (*tab_ima)[ind]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE RADAR_CNES
      (EXTENSION =.rar ou .RAD)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
/* recherche de l'extension en minuscule */
       pch0 = strstr(dp->d_name,".rad");
       if (pch0==NULL)
       {
/* recherche de l'extension en majuscule */
	 pch1=strstr(dp->d_name,".RAD");
	 if ((pch1 != NULL)&&(*(pch1+4) == '\0'))
         {
		*pch1='\0';
            	strcpy((*tab_ima)[nb_ima],dp->d_name);
		nb_ima=nb_ima+1;
         }
       }
      else
      {
	 if (*(pch0+4) == '\0')
         {
		*pch0='\0';
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
 return(nb_ima);
}

/* Fin de l'operation cai_liste_ima_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_radar                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format radar     */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_lecture_radar (nom_image,     */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels et par canal */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_radar     (S) (CAI_IMAGE *)  : = NULL si pb         */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_radar(char *repert,
				 char *nom_image,
				 int  *nb_can,
				 int  *nboct_pix,
				 int  *nb_col,
				 int  *nb_lig)


{
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   char label[80];		/* contient un enregistrement du fichier    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   char *deb,*pch0;	        /* pointeurs sur chaine de caracteres       */
   int ind,num;			/* indice 				    */
   int nboct;			/* variables de travail			    */
   int ind_typ;                 /* Indice du type de codage                 */
   int is_Cplx;                 /* Indique si type complexe ou pas          */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int cmp;			/* retour de comparaison de chaine          */

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix = 0;
   pch0=NULL;

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
   sprintf(nom_fic,"%s/%s.rad",repert,nom_image);
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
      sprintf(nom_fic,"%s/%s.RAD",repert,nom_image);
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
          strcpy(CAI_ERREUR,"Erreur ouverture fichier entete RADAR-CNES");
          goto ERREUR;
     }
   }
 /*....................................
    Decodage du nombre de colonnes
  ............................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"NBCOLONNES");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes NBCOLONNES dans %s",
		nom_fic);
	goto ERREUR;
   }
   retour=fscanf(fic_in,"%d",nb_col);


 /*....................................
    Decodage du nombre de lignes
  ............................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"NBLIGNES");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes NBLIGNES dans %s",
		nom_fic);
	goto ERREUR;
   }
  retour=fscanf(fic_in,"%d",nb_lig);

 /*........................................
    Decodage du nombre de plans
  .........................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"NBPLANS");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes NBPLANS dans %s",
		nom_fic);
	goto ERREUR;
   }
  retour=fscanf(fic_in,"%d",&nbplan);
  *nb_can=nbplan;

 /*........................................
   Recuperation du type de codage du plan   .........................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"TYPECODAGE");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes TYPECODAGE dans %s",
		nom_fic);
	goto ERREUR;
   }

   retour=fscanf(fic_in,"%s",label);
   pch0=cai_En_majuscule( label );

/* Verification si type de codage reconnu et recuperation de la taille pixel*/
   nboct = cai_istyp_radar(pch0,&ind_typ,&is_Cplx);
   if (nboct == -1)
   {
      sprintf(CAI_ERREUR,"Type de codage du plan %s non reconnu ",
							nom_fic_canal);
      goto ERREUR;
    }
    *nboct_pix=nboct;
    sprintf(image1->TYPE_COD,"%s",pch0);

/* ...........................................................
   Determination du sens de codage de la machine de travail
  .............................................................*/
    type_machine = cai_test_pFpf();

/*.....................................................
   Recuperation du type de codage du plan (INTEL ou IEEE)  .......................................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"SENSCODAGE");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes SENSCODAGE dans %s",
		nom_fic);
	goto ERREUR;
   }

   retour=fscanf(fic_in,"%s\n",label);
   retour=strncmp(label,"IEEE",4);
   if (retour==0)	
	sprintf(image1->COD_PIX,"BE");
   else
	sprintf(image1->COD_PIX,"LE");
   cmp = strncmp(type_machine,image1->COD_PIX,2);
   if (cmp!=0)
   {
/*****************************************************************************/
/* MOD : VERSION : 1.2 : FA : IMA-FA-011 pb inversion des octets complexes   */
/*****************************************************************************/
	if (is_Cplx==0)
	  image1->INV_OCTETS =1;
	else
	  image1->INV_OCTETS =2;
   }
   else
   {
	  image1->INV_OCTETS =0;
   }
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Maj du tag COD_PIX lorsque l'image est codée  */
/*                                            en reel  a partir du TYPE_CODE */
/*****************************************************************************/    sprintf(image1->COD_PIX,"%s",image1->TYPE_COD);

/*...............................................
	Pour chaque plan , decodage des caracteristiques du plans
..................................................................*/
   deb=image1->NOM_FIC_CAN;
   for (ind=0;ind<nbplan;ind++)
   {
/* Recuperation nom du plan */
        retour=fscanf(fic_in,"%s",label);
	sprintf(deb,"%s",label);
        sprintf(nom_fic_canal,"%s/%s",repert,label);
	deb = deb+100;
  
/* Verification de l'existence du plan et ouverture */
        num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
        if (num!= -1)
        {
 		image1->ACCES_CANAL_INT[ind]=num;
          	image1->POS_OCTET[ind]=0;   
        }
	else
	{
           	sprintf(CAI_ERREUR,"Impossible d'ouvrir le plan %s",
							nom_fic_canal);
	   	goto ERREUR;
	}
   }  
	
 /*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in!=NULL) fclose(fic_in);


/* sortie nominale */
     return(image1);

ERREUR:
/* sortie en erreur */
     FREE(image1);
     return (NULL);
}
/* 
   Fin de l'operation cai_ouvre_lecture_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_radar                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format radar et de tous les fichiers qui la composent        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    CAI_IMAGE *cai_ouvre_creation_radar (nom_image,     */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            typcode)         */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nb d'octets pixels                      */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     typcode        (E) (char *) : type de codage   de l'image             */
/*                                                                           */
/*     cai_ouvre_creation_radar     (S) (CAI_IMAGE *) : =NULL si pb          */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_radar(char *repert,
				  char *nom_image,
				  int  *nb_can,
				  int  *nboct_pix,
				  int  *nb_col,
				  int  *nb_lig,
				  char *typcode)

{
   FILE *fic;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   int retour,retour1;		/* retour de fonction 			    */ 
   int can,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int ind_typ;			/* Indice du type de codage radar           */
   int is_Cplx;                 /* Indique si type complexe ou pas          */
   int nboct;
   char *pch0;

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   if (image1==NULL)
   {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_creation");
     goto ERREUR;
   }

/* Vérification si typecode est un type radar si non par défaut mettre und */
   pch0=cai_En_majuscule( typcode );
   nboct = cai_istyp_radar(pch0,&ind_typ,&is_Cplx);
   if (nboct == -1)
   {
	sprintf(typcode,"und");
   }
/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if ((long int)strlen(repert) == 0)
	sprintf(repert,"./");

/* Recherche avec l'extension en majuscule */
   sprintf(nom_fic,"%s/%s.RAD",repert,nom_image);
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	unlink(nom_fic);
   }

/* Recherche avec l'extension en minuscule */
   sprintf(nom_fic,"%s/%s.rad",repert,nom_image);
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	unlink(nom_fic);
   }
   sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);


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

/* Nombre de colonnes */
  retour=fprintf ( fic , "NBCOLONNES ");
  retour1=fprintf ( fic , "%d\n" , *nb_col );
  if ((retour == 0)||(retour1 == 0))
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
  }
/* Nombre de lignes */
  retour=fprintf ( fic , "NBLIGNES ");
  retour1=fprintf ( fic , "%d\n" , *nb_lig );
  if ((retour == 0)||(retour1 == 0))
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
  }

/* Nombre de plans */
  retour=fprintf ( fic , "NBPLANS ");
  retour1=fprintf ( fic , "%d\n" , *nb_can);
  if ((retour == 0)||(retour1 == 0))
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
  }

/* TYPE DE CODAGE */
  retour=fprintf ( fic , "TYPECODAGE ");
  retour1=fprintf ( fic , "%s\n" , typcode );
  if ((retour == 0)||(retour1 == 1))
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
  }
 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

/* TYPE DE CODAGE */
  retour=fprintf ( fic , "SENSCODAGE ");

   if (strcmp(type_machine,"BE")==0)
 	retour=fprintf ( fic , "IEEE\n");
   else
	retour=fprintf ( fic , "INTEL\n");
   if (retour == 0)
   {
      	sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",
					nom_fic);
      	goto ERREUR;
    }

/*...............................................................
  Pour chaque canal de l'image,constitution du nom de fichier correspondant
  Verification qu'il n'existe pas deja
  Ouverture du fichier en ecriture
  .................................................................*/
  pch0=cai_En_minuscule( typcode );

  for (can=0;can<*nb_can;can++)
  {
       nom_fic[0]='\0';
       if (*nb_can==1)
         sprintf(nom_fic,"%s/%s.%s",repert,nom_image,pch0);
       else
          sprintf(nom_fic,"%s/%s_%d.%s",repert,nom_image,can+1,pch0);

       retour = stat ( nom_fic , &buf_stat );

       if ( retour == 0 )
       { 
		unlink(nom_fic);
       }
       num = open(nom_fic,O_RDWR|O_CREAT|O_BINARY,PERMS); 
       if (num == -1)
       {
     		sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",
				nom_fic);
     		goto ERREUR;
       } 
       image1->ACCES_CANAL_INT[can]=num;
       image1->POS_OCTET[can]=0;
       if (*nb_can==1)
       {
          retour1=fprintf ( fic , "%s.%s\n" , nom_image,pch0);
	  snprintf(image1->NOM_FIC_CAN,100,"%s.%s",nom_image,pch0);
	 *(image1->NOM_FIC_CAN+99)='\0';
       }
       else
       {
          retour1=fprintf ( fic , "%s_%d.%s\n" , nom_image,can+1,pch0);
	  snprintf(image1->NOM_FIC_CAN+can*100,100,"%s_%d.%s",nom_image,can+1,pch0);
	 *(image1->NOM_FIC_CAN+can*100+99)='\0';
       }
       if (retour1 == 1)
       {
      		sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",
					nom_fic);
      		goto ERREUR;
  	}
  }

/*************************************************/
/* Fermeture du fichier                          */
/*************************************************/

  fclose ( fic );
  return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_radar                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image radar                */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_radar(image1, canal,      */
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
/*     cai_lecture_canal_radar     (S) (CAI_OK_KO) : = CAI_KO si pb          */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_lecture_canal_radar(CAI_IMAGE *image1 ,  
                                int *canal ,           
    		          	int *premiere_ligne  ,   
     		          	int *premiere_colonne,
                         	int *nombre_lignes_lire ,
                          	int *nombre_colonnes_lire,
                          	int *pas_ligne ,          
                          	int *pas_colonne,
		          	unsigned char *data_image )        

{

 int   num,i,j,k;         /* variables de travail                          */
 long int pt_oct;	  /* position courante dans le fichier image       */
 long int no_oct;         /* numero du premier octet a lire                */
 int iret;                /* indicateur d'erreur                           */
 long int lig;            /* indice de boucle sur les lignes               */
 long int oct_pix,octpix; /* nb octets par pixels                          */
 long int nb_oct;         /* nb oct =1er octet a lire - position courante  */
 long int oct_lig;        /* nb oct. par ligne en considerant taille pixel */
 unsigned char *buf_lec;  /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch; /* pointeur chaine pour transfert  buffer sortie */
 long int n;
 int nbpix;
/*...........................
  Initialisations diverses
  ..........................*/

   iret=CAI_OK;
   num=(int)image1->ACCES_CANAL_INT[(*canal)-1];
   oct_pix=image1->NBOCT_PIX;;
   oct_lig=oct_pix*(image1->NB_COL);

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
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
             sprintf(CAI_ERREUR,"Erreur lecture donnee image RADAR  %s",
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
       
	  if ((image1->INV_OCTETS==1) && (oct_pix!=1))
          {
            nbpix = (*nombre_lignes_lire)*(*nombre_colonnes_lire);
	    cai_inverser_tableau_complet(data_image,(int)nbpix,(int)oct_pix);
          }

/*****************************************************************************/
/* MOD : VERSION : 1.2 : FA : IMA-FA-011 pb inversion des octets complexes   */
/*****************************************************************************/
	  else if (image1->INV_OCTETS==2)
	  {
            nbpix = (*nombre_lignes_lire)*(*nombre_colonnes_lire)*2;
	    octpix = oct_pix/2;
	    cai_inverser_tableau_complet(data_image,(int)nbpix,(int)octpix);	 
          }

        } 
	free(buf_lec);
    }
        return(iret);
 }
/*
  Fin de l'operation cai_lecture_canal_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_radar                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un plan d'une image radar                 */
/* -----    								     */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_OK_KO cai_ecriture_canal_radar(image1, canal,  */
/* ------------------                                 premiere_ligne,        */
/*                                                    nombre_lignes_ecrire,  */
/*                                                    nombre_colonnes_ecrire,*/
/*                 		                      data_image )           */
/*									     */
/*     image1      (E) (CAI_IMAGE *)   : pointeur sur descripteur de fichier */
/*     canal            (E) (int)      : numero du canal a ecrire            */
/*     premiere_ligne   (E)  (int)     : premiere ligne a ecrire ds l'image  */
/*     nombre_lignes_ecrire  (E) (int) : nombre de lignes a ecrire           */
/*     nombre_colonnes_ecrire(E) (int) : nombre de lignes a ecrire           */
/*                                                                           */
/*     data_image  (S) (unsigned char): tableau des pixels ecris alloue et   */
/*                                      libere par l'appelant                */
/*									     */
/*     cai_ecriture_canal_radar        (S) (CAI_OK_KO) : = CAI_KO si pb      */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_radar (CAI_IMAGE *image1 ,  
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
              sprintf(CAI_ERREUR,"Erreur ecriture donnee image RADAR  %s",
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
	    sprintf( CAI_ERREUR , "Erreur ecriture canal image radar_cnes  %s",
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
  Fin de l'operation cai_ecriture_canal_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_radar                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format radar et de         */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_radar(image1)                     */
/* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_radar        (S) (CAI_OK_KO) : = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_radar (CAI_IMAGE *image1)

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
  Fin de l'operation cai_ferme_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_radar                                         */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en radar et de       */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_radar(repertoire,nom_ima)      */
/* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_radar     (S) (CAI_OK_KO) : = CAI_KO si pb               */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_dest_ima_radar ( char *repertoire , 
                     	     char *nom_ima )

{
int	i_can;			/* compteur de canaux */
long int     iret;              /* code retour fonction fermeture lecture */
char	nom_fic[1024];		/* nom du fichier entete a detruire */
long int	cr_stat;	/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 

CAI_IMAGE   *image1;            /* structure ouverture image */
int         nb_can;	/* nombre de canaux de l'image */
int         nboct_pix;	/* nombre d'octets par pixel image */
int         nb_col;	/* nombre de colonnes de l'image */
int         nb_lig;        /* nombre de lignes de l'image */
char *pch;

  if ((long int)strlen(repertoire)!= 0)
       sprintf(repertoire,"./");

/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_radar			*/
/************************************************/

  image1 = cai_ouvre_lecture_radar ( repertoire,nom_ima, &nb_can ,&nboct_pix ,
				   &nb_col , &nb_lig );	
  if ( image1 == NULL) goto ERREUR;

  image1->NB_CANAUX=nb_can;
  image1->NB_COL=nb_col;
  image1->NB_LIG=nb_lig;
  image1->NBOCT_PIX=nboct_pix;

 
/************************************************/
/* Constitution un a un des noms des fichiers	*/
/* canaux et destruction 			*/
/************************************************/
pch = image1->NOM_FIC_CAN;
for ( i_can = 0 ; i_can < nb_can ; i_can++ )
{
	sprintf(nom_fic,"%s/%s",repertoire,pch);
	cr_stat = stat ( nom_fic , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic );
        pch=pch+100;
}

  iret=cai_ferme_radar(image1);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/
sprintf(nom_fic ,"%s/%s.RAD", repertoire,nom_ima);
cr_stat = stat ( nom_fic , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic );
else
{
  sprintf(nom_fic ,"%s/%s.rad", repertoire,nom_ima);
  cr_stat = stat ( nom_fic , &buf_stat );
  if ( cr_stat == 0 )
	unlink ( nom_fic );
}

return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/*
 Fin de l'operation cai_dest_ima_radar
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_radar                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     radar et de tous les fichiers qui la composent                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_modifie_radar (nom_image,     */
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
/*     cai_ouvre_modifie_radar     (S) (CAI_IMAGE *)  : = NULL si pb         */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_modifie_radar(char *repert,
				     char *nom_image,
				     int  *nb_can,
				     int  *nboct_pix,
				     int  *nb_col,
				     int  *nb_lig)


{
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   char label[80];		/* contient un enregistrement du fichier    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   char *deb,*pch0;	        /* pointeurs sur chaine de caracteres       */
   int ind,num;			/* indice 				    */
   int nboct;			/* variables de travail			    */
   int ind_typ;			/* Indice du type de codage radar           */
   int is_Cplx;                 /* Indique si type complexe ou pas          */
   char *type_machine;		/* Type de codage de la machine             */
   int cmp;			/* variable de travail                      */

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix = 0;
   pch0=NULL;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   if (image1==NULL)
   {
      strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_modifie");
      goto ERREUR;
   }
/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)== 0)
	sprintf(repert,"./");
   sprintf(nom_fic,"%s/%s.rad",repert,nom_image);
   sprintf(image1->NOM_IMAGE,"%s/%s",repert,nom_image);

  /*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"rw");
   if (fic_in==NULL)
   {
/* verification si extension en majuscule */
      nom_fic[0]='\0';
      sprintf(nom_fic,"%s/%s.RAD",repert,nom_image);
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
          strcpy(CAI_ERREUR,"Erreur ouverture fichier entete RADAR-CNES");
          goto ERREUR;
     }
   }
 /*....................................
    Decodage du nombre de colonnes
  ............................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"NBCOLONNES");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes NBCOLONNES dans %s",
		nom_fic);
	goto ERREUR;
   }
   retour=fscanf(fic_in,"%d",nb_col);


 /*....................................
    Decodage du nombre de lignes
  ............................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"NBLIGNES");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes NBLIGNES dans %s",
		nom_fic);
	goto ERREUR;
   }
  retour=fscanf(fic_in,"%d",nb_lig);

 /*........................................
    Decodage du nombre de plans
  .........................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"NBPLANS");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes NBPLANS dans %s",
		nom_fic);
	goto ERREUR;
   }
  retour=fscanf(fic_in,"%d",&nbplan);
  *nb_can=nbplan;

 /*........................................
   Recuperation du type de codage du plan   .........................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"TYPECODAGE");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes TYPECODAGE dans %s",
		nom_fic);
	goto ERREUR;
   }

   retour=fscanf(fic_in,"%s",label);
   pch0=cai_En_majuscule( label );

/* Verification si type de codage reconnu et recuperation de la taille pixel*/
   nboct = cai_istyp_radar(pch0,&ind_typ,&is_Cplx);
   if (nboct == -1)
   {
      sprintf(CAI_ERREUR,"Type de codage du plan %s non reconnu ",
							nom_fic_canal);
      goto ERREUR;
    }
    *nboct_pix=nboct;
    sprintf(image1->TYPE_COD,"%s",pch0);
/* ...........................................................
   Determination du sens de codage de la machine de travail
  .............................................................*/
    type_machine = cai_test_pFpf();

/*.....................................................
   Recuperation du type de codage du plan (INTEL ou IEEE)  .......................................................*/
   retour=fscanf(fic_in,"%s",label);
   pch0=strstr(label,"SENSCODAGE");
   if (pch0==NULL)
   {
        sprintf(CAI_ERREUR,"Informations incoherentes SENSCODAGE dans %s",
		nom_fic);
	goto ERREUR;
   }

   retour=fscanf(fic_in,"%s\n",label);
   retour=strncmp(label,"IEEE",4);
   if (retour==0)	
	sprintf(image1->COD_PIX,"BE");
   else
	sprintf(image1->COD_PIX,"LE");
   cmp = strncmp(type_machine,image1->COD_PIX,2);
   if (cmp!=0)
   {
/*****************************************************************************/
/* MOD : VERSION : 1.2 : FA : IMA-FA-011 pb inversion des octets complexes   */
/*****************************************************************************/
	if (is_Cplx==0)
	  image1->INV_OCTETS =1;
	else
	  image1->INV_OCTETS =2;
   }
   else
   {
	  image1->INV_OCTETS =0;
   }
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Maj du tag COD_PIX lorsque l'image est codée  */
/*                                            en reel  a partir du TYPE_CODE */
/*****************************************************************************/    sprintf(image1->COD_PIX,"%s",image1->TYPE_COD);

/*...............................................
	Pour chaque plan , decodage des caracteristiques du plans
..................................................................*/
   deb=image1->NOM_FIC_CAN;
   for (ind=0;ind<nbplan;ind++)
   {
/* Recuperation nom du plan */
        retour=fscanf(fic_in,"%s",label);
	sprintf(deb,"%s",label);
        sprintf(nom_fic_canal,"%s/%s",repert,label);
	deb = deb+100;
  
/* Verification de l'existence du plan et ouverture */
        num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
        if (num!= -1)
        {
 		image1->ACCES_CANAL_INT[ind]=num;
          	image1->POS_OCTET[ind]=0;   
        }
	else
	{
           	sprintf(CAI_ERREUR,"Impossible d'ouvrir le plan %s",
							nom_fic_canal);
	   	goto ERREUR;
	}
   }  
	
 /*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in!=NULL) fclose(fic_in);


/* sortie nominale */
     return(image1);

ERREUR:
/* sortie en erreur */
     FREE(image1);
     return (NULL);
}

/* 
   Fin de l'operation cai_ouvre_modifie_radar
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_istyp_radar                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de verification si pixels de type radar ou non         */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    retour = cai_istyp_radar(type_code)                 */
/*  ---------------- 		                                             */
/*                                                                           */
/*     type_code    (E) (char *) : type de codage a verifier                 */
/*     ind_typ      (S) (int *)  : indice du type des pixels                 */
/*     is_Cplx      (S) (int *)  : 1 si type complexe, 0 si non              */
/*     retour       (S) (int)    : nb d'octets par pixel si type reconnu     */ 
/*				   -1 si non reconnu			     */
/*                                                                           */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
int cai_istyp_radar(char *type_code,int *ind_typ,int *is_Cplx )
{
	int ind;	 /* indice de boucle sur les types reconnus */
	int trouve,icr;  /* indice pour la recherche                */

/* Initialisations diverses */
	trouve = 0;
	ind = 0;
	while ((trouve==0)&&(ind< CAI_NB_RADAR))
	{
	  icr=strncmp(type_code,CAI_RADAR_TYP[ind],strlen(type_code));
	  if (icr==0) 
		trouve=1;
	  else
	       ind=ind+1;
	}
	
	if (trouve ==1)
	{
	      *ind_typ=ind; 
	      ind=CAI_RADAR_LONG[ind];
	}
	else
	{
		*ind_typ=-1; 
	        ind=-1;
	}

	if (type_code[0]=='C')
	    *is_Cplx=1;
	else
	    *is_Cplx=0;

/* sortie normale */
	return(ind);
}
/* 
   Fin de l'operation cai_istyp_radar
*/
