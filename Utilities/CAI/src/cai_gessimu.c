/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_gessimu.c                                                 */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format gessimu (format Alcatel monocanal)         */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    Septembre 1999                                      */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA : Rajout des tests sur les retours d'allocation  */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : verification que l'image est codee sur un seul */
/*octet par pixel (contrainte du format)                                     */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : modifications open  binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
#include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>

#define _CAI_IMAGE
#define IN_CAI_GESSIMU

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_gessimu.c,v 1.6 01/05/31 10:47:35 cmf Exp $";


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
/* OPERATION :    cai_liste_ima_gessimu                                      */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format gessimu                */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_gessimu(repert,tab_ima)         */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*   cai_liste_ima_gessimu   (S) (int)     :   = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_ima_gessimu (char *repert,
		           char ***tab_ima)

{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images gessimu identifiees         */
   long int i;          /* indice de boucle                            */
   long int ll,dif;     /* variables de travail                        */
   char blanc;          /* caractere blanc                             */


/*........................................
   INITIALISATION et ouverture du repertoire choisi
  .........................................*/
   nb_ima=0;
   blanc=' ';
   dirp=opendir(repert);

   if(dirp!=NULL)
   {
/*......................................

   premiere passe pour comptabiliser les images du format donne


   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE GESSIMU
      (EXTENSION =.pfe)
     ..........................................*/

      while((dp=readdir(dirp))!=NULL)
      {
    	 pch0=strstr(dp->d_name,".pfe");
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

    (void)closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT GESSIMU
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE GESSIMU
       (EXTENSION =.pfe)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
 
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".pfe");
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
/* Fin de l'operation cai_liste_ima_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_gessimu                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format gessimu   */
/* -----    					                             */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_gessimu(nom_image,       */
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
/*     cai_ouvre_lecture_gessimu (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_gessimu(char *repert,
				     char *nom_image,
				     int *nb_can,
				     int *nboct_pix,
				     int *nb_col,
				     int *nb_lig)

{
   FILE *fic_in;		/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   int i,num;			/* indice 				    */
   char type_cod[5];		/* type de codage du pixel		    */
   int detec1,detecF;           /* 1er et dernier detecteur dans l'entete   */
   int Tail_lig;	        /* taille d'une ligne sur disque (multi 4)  */
 
   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
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
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.pfe",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.pfe",nom_image);
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
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete GESSIMU");
    }
   else
   {
/*....................................
    positionnement sur le nombre de colonnes 
  .............................................*/
        fseek(fic_in,16,0);

/*..................................
   recuperation du numero du premier detecteur
  ...........................................*/
	fread(&detec1,4,1,fic_in);
/*..................................
   recuperation du dernier detecteur
  ......................................*/
	fread(&detecF,4,1,fic_in);
/*..................................
   recuperation du nombre de lignes 
  ......................................*/
        fseek(fic_in,12,1);
	fread(nb_lig,4,1,fic_in);
/*..................................
   calcul du nombre de colonne 
  ......................................*/
	*nb_col = detecF - detec1 ;
/*..................................
   initialisation du type de codage des pixels 
  ......................................*/
    	image1->NB_BITS_PIX=8;
    	image1->NBOCT_PIX=1;
	sprintf(image1->COD_PIX,"OCT");
	*nboct_pix = image1->NBOCT_PIX;

/*..................................
   initialisation du nb de canaux (toujours monocanal)
  ......................................................*/
	*nb_can = 1;

/*..................................
   Calcul de la taille d'une ligne sur disque
  ......................................................*/
	Tail_lig = (((*nb_col-1)/4)+1)*4;

  }
/*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in!=NULL) fclose(fic_in);

   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     if ((long int)strlen(repert)!=0)
     {
       sprintf(nom_fic_canal,"%s/%s.pfe",repert,nom_image);
      }
      else
      {
       sprintf(nom_fic_canal,"%s.pfe",nom_image);
      }
  
     num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
           sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
            image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=Tail_lig;
        }
      }
      else
         iret=0;
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
   Fin de l'operation cai_ouvre_lecture_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_gessimu                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format gessimu et de tous les fichiers qui la composent      */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_gessimu(nom_image,       */
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
/*     cai_ouvre_creation_gessimu (S) (CAI_IMAGE *) :     =NULL si pb        */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_gessimu(char *repert,
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
   long retour;			/* retour de fonction 			    */ 
   int i,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   unsigned char *Header;       /* entete du fichier gessimu 	            */
   int taille_entete;	        /* taille de l'entete		            */
   int detec1,detecF;           /* numero du 1er et dernier detecteur       */
   double h_moyen;

/*********************************************/
/* Verification qu'un seul canal est demande */
/*********************************************/
   if (*nb_can != 1)
   {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image GESSIMU ne peut contenir qu'un seul canal");
	goto ERREUR;
   }
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : verification que l'image est codee sur un seul */
/*octet par pixel (contrainte du format)                                     */
/*****************************************************************************/
   if (*nboct_pix != 1)
   {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image GESSIMU ne peutetre codee que sur un seul octet par pixel");
	goto ERREUR;
   }

    
/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA:Rajout des tests sur les retours d'allocation     */
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
       sprintf(nom_fic,"%s/%s.pfe",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.pfe",nom_image);
    }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s GESSIMU existe \n" , nom_image );
	goto ERREUR;
   }

/*......................................................
  Ouverture du fichier entete en ecriture 
  Ecriture du contenu
  ..........................................................*/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/

  fic = fopen(nom_fic,"wb");
  if (fic == NULL)
  {
      sprintf(CAI_ERREUR,"Erreur: Creation fichier %s impossible\n",nom_fic);
      goto ERREUR;
   }
   taille_entete = (((*nb_col-1)/4)+1)*4;
   Header = (unsigned char *) calloc(taille_entete,sizeof( unsigned char));
   if (Header == NULL)
   {
      sprintf(CAI_ERREUR,"Erreur alloc: Creation fichier %s impossible\n",
				nom_fic);
      goto ERREUR;
   }

/*******************************************/
/* ecriture du 1ER detecteur de l'image    */
/*******************************************/
   detec1 = 1;
   memcpy(&Header[16],&detec1 , 4 );

/************************************************/
/* ecriture du dernier detecteur de l'image     */
/************************************************/
   detecF = *nb_col;
   memcpy(&Header[20],&detecF , 4 );

/*********************************************/
/* ecriture du nombre de colonnes de l'image */
/*********************************************/
   memcpy(&Header[24],nb_col , 4 );

/*********************************************/
/* ecriture du nombre de lignes de l'image */
/*********************************************/
   memcpy(&Header[36],nb_lig , 4 );

/***************************************************/
/* ecriture de la taille d'une ligne multiple de 4 */
/***************************************************/
   memcpy(&Header[40],&taille_entete , 4 );

/************************************************/
/* ecriture du h_moyen (double 0)               */
/************************************************/
   h_moyen = 0.;
   memcpy(&Header[44],&h_moyen , 8 );

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

   fclose ( fic );

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
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_gessimu                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image GESSIMU              */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_gessimu(image1, canal,    */       		      /* ------------------                        premiere_ligne,                 */
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
/*     cai_lecture_canal_gessimu (S) (CAI_OK_KO) :     = CAI_KO si pb        */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_gessimu(CAI_IMAGE *image1 ,  
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
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 int Tail_lig;		    /* Taille physique d'une ligne sur disque        */

/*********************************************/
/* Verification qu'un seul canal est demande */
/*********************************************/
 if (*canal != 1)
 {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image GESSIMU ne contient qu'un seul canal");
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
   Tail_lig = (((image1->NB_COL-1)/4)+1)*4;

/*..............................................
   allocation de memoire pour lire une ligne image
 ............................................. */

    buf_lec = (unsigned char *) calloc(Tail_lig,sizeof(unsigned char));
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
               
	      no_oct=image1->POS_OCTET[*canal-1]+(lig-1)*Tail_lig;


/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
              nb_oct=(long)(no_oct-pt_oct);
              lseek(num,nb_oct,1);

/*...........................................
            lecture d'une ligne image
  ................................................*/
              n=read(num,buf_lec,Tail_lig);
 
              if (n<=0)
              {
             	iret=CAI_KO;
             	strcpy(CAI_ERREUR,"Erreur lecture donnee image GESSIMU");
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
                  	 *pch=*deb;
                  	 j=j+1;
                	 pch=pch+1;
                	 deb=deb+(*pas_colonne);
                  }   
             
              }
              i=i+1;
              lig=lig+(*pas_ligne);
           }        
          	    
	   free(buf_lec);
      	}
   }
  return(iret);
}
/*
  Fin de l'operation cai_lecture_canal_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_gessimu                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image gessimu              */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_gessimu(image1, canal,   */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_gessimu    (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ecriture_canal_gessimu (CAI_IMAGE *image1 ,  
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
int Tail_lig;		        /* taille en octet d'une ligne (mult 4)  */
int ind,i;			/* variables de travail			 */

/*********************************************/
/* Verification qu'un seul canal est demande */
/*********************************************/
   if (*canal != 1)
   {
		image1 = NULL;
 		sprintf ( CAI_ERREUR , 
			"Une image GESSIMU ne contient qu'un seul canal");
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
   Tail_lig = (((image1->NB_COL-1)/4)+1)*4;
    buff_lig = NULL;
    buff_lig = (unsigned char *) calloc(Tail_lig,sizeof(unsigned char));
    if (buff_lig == NULL)
    {
       		strcpy(CAI_ERREUR,"Probleme allocation memoire");
		goto ERREUR;
     }
 /********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

     position_first_lig = (long) ( image1->POS_OCTET[*canal-1] + 
 		+ (*premiere_ligne - 1) * Tail_lig);

/********************************************************/
/*  Recuperation de la position courante	        */
/********************************************************/

    position_courante = lseek ( num , 0L , 1 );

/********************************************************/
/* Calcul du deplacement jusqu'a la 1ere ligne a ecrire	*/
/********************************************************/

   deplacement = position_first_lig - position_courante;

/*******************************************************/
/* deplacement jusqu'a la premiere ligne a ecrire      */
/*******************************************************/

   lseek ( num , deplacement , 1 ); 

/***********************************************/
/* ecriture des lignes			       */
/***********************************************/
   nboct_ecrire = *nombre_colonnes_ecrire;
   ind = 0;
   i = 0;

/*.....................
   Pour chaque ligne a lire
  .............................*/
   while (i < *nombre_lignes_ecrire)
   {  
	    memcpy(buff_lig,&data_image[ind] , nboct_ecrire );


	    cr_write = write ( num , buff_lig,Tail_lig);

	    if ( cr_write !=  Tail_lig)
	    {
	    	strcpy ( CAI_ERREUR , 
				"Erreur ecriture canal image gessimu\n" );
	        goto ERREUR;
	    }
            ind = ind + nboct_ecrire;
	    i = i + 1;
    }
if (buff_lig != NULL) free(buff_lig);

return (CAI_OK);

ERREUR :

if (buff_lig != NULL) free(buff_lig);
return (CAI_KO);


}
/* 
  Fin de l'operation cai_ecriture_canal_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_gessimu                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format gessimu et de       */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_gessimu(image1)                   */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_gessimu    (S) (CAI_OK_KO) :       = CAI_KO si pb           */
/*                                                  = CAI_OK si OK           */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO  cai_ferme_gessimu (CAI_IMAGE *image1)

{

 int iret;    /* code retour de l'operation      */
 int i_can;   /* indice du canal a fermer        */
 int num;     /* descripteur du fichier a fermer */

 iret=CAI_OK;
/*.............................................
   Fermeture du fichier qui contient tous les canaux
  .............................................*/
  num=image1->ACCES_CANAL_INT[0];
  if (num>0) close(num);


  return (iret);
}

/*
  Fin de l'operation cai_ferme_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_gessimu                                       */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en gessimu et de     */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_gessimu(repertoire,nom_ima)    */       		      /* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_gessimu (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_dest_ima_gessimu ( char *repertoire , 
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
  sprintf(nom_fic_entete ,"%s.pfe", nom_ima_complet);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/

cr_stat = stat ( nom_fic_entete , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_entete );


return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_gessimu
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_gessimu                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     gessimu et de tous les fichiers qui la composent                */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_gessimu(nom_image,       */
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
/*     cai_ouvre_modifie_gessimu (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_gessimu(char *repert,
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
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   int i,num;			/* indice 				    */
   char type_cod[5];		/* type de codage du pixel		    */
   int detec1,detecF;           /* 1er et dernier detecteur dans l'entete   */
   int Tail_lig;	        /* taille d'une ligne sur disque (multi 4)  */
 
   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA:Rajout des tests sur les retours d'allocation     */
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
   if ((long int)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.pfe",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.pfe",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/

   fic_in=fopen(nom_fic,"rb");
   if (fic_in==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete GESSIMU");
    }
   else
   {
/*....................................
    positionnement sur le nombre de colonnes 
  .............................................*/
        fseek(fic_in,16,0);
/*..................................
   recuperation du numero du premier detecteur
  ...........................................*/
	fread(&detec1,4,1,fic_in);
/*..................................
   recuperation du dernier detecteur
  ......................................*/
	fread(&detecF,4,1,fic_in);
/*..................................
   recuperation du nombre de lignes 
  ......................................*/
        fseek(fic_in,12,1);
	fread(nb_lig,4,1,fic_in);
/*..................................
   calcul du nombre de colonne 
  ......................................*/
	*nb_col = detecF - detec1 ;
/*..................................
   initialisation du type de codage des pixels 
  ......................................*/
    	image1->NB_BITS_PIX=8;
    	image1->NBOCT_PIX=1;
	sprintf(image1->COD_PIX,"OCT");
	*nboct_pix = image1->NBOCT_PIX;

/*..................................
   initialisation du nb de canaux (toujours monocanal)
  ......................................................*/
	*nb_can = 1;

/*..................................
   Calcul de la taille d'une ligne sur disque
  ......................................................*/
	Tail_lig = (((*nb_col-1)/4)+1)*4;

  }
/*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in!=NULL) fclose(fic_in);

   if (iret !=0)
   {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     if ((long int)strlen(repert)!=0)
     {
       sprintf(nom_fic_canal,"%s/%s.pfe",repert,nom_image);
      }
      else
      {
       sprintf(nom_fic_canal,"%s.pfe",nom_image);
      }
  
     num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
           sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
            image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=Tail_lig;
        }
      }
      else
         iret=0;
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
   Fin de l'operation cai_ouvre_modifie_gessimu
*/

