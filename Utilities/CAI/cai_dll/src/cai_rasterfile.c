/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_rasterfile.c                                              */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format rasterfile                                 */ 
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
/* MOD : VERSION : 3.3 : DM :optimisation des acces                          */
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 :FA:Verification que le nombre de canaux est 1 ou 3.  */
/* sunrasterfile est un format de  restitution	                             */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : modifications  open binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA :  == au lieu de = simple                        */
/*                                         image1->MODE == CAI_ECRITURE      */
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
#define IN_CAI_RASTER

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_rasterfile.c,v 1.11 01/05/31 10:47:16 cmf Exp $";

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
/* OPERATION :    cai_liste_ima_sun_raster                                   */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format rasterfile             */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_sun_raster(repert,tab_ima)     */
/* -----------------                                                         */
/*                                                                           */
/*     repert        (E) (char *)   : repertoire a explorer		     */
/*     tab_ima       (S) (char ***) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*    cai_liste_ima_sun_raster (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


long int cai_liste_ima_sun_raster (char *repert,
		              	char ***tab_ima)


{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2,*pch3;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images rasterfile identifiees      */
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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE sun_raster_file
      (EXTENSION =.srf)
 ..........................................*/


   while((dp=readdir(dirp))!=NULL)
   {

     car='.';
     pch0=strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='s';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='r';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
	      car='f';
              pch3=strrchr(dp->d_name,car);
              if ((pch3 != NULL)&&(pch3==pch2+1))
	      {

                  *pch0=' ';
              	  *pch1=' ';
	      	  *pch2=' ';
		  *pch3=' ';
	          nb_ima=nb_ima+1;
              }
           }
       }
     }
   }

    (void)closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES 
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE RASTERFILE
      (EXTENSION =.srf)
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
       car='s';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='r';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
	      car='f';
              pch3=strrchr(dp->d_name,car);
              if ((pch3 != NULL)&&(pch3==pch2+1))
	      {

                  *pch0=' ';
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
     /* Erreur:Repertoire inexistant */
      nb_ima=-1;
   }

    return(nb_ima);

}
/* Fin de l'operation cai_liste_ima_sun_raster
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_sun_raster                               */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format rasterfile*/
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_lecture_sun_raster(nom_image,     */
/*  ---------------- 		                              nb_can,        */
/*				                              nboct_pix      */	
/*				                              nb_col,        */
/*				                              nb_lig)        */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*  cai_ouvre_lecture_sun_raster (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */  
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_sun_raster(char *repert,
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
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int i,j,ind;			/* indice 				    */
   int ras_type;                /* type de fichier raster                   */
   int fic;			/* code retour fonction open		    */
   unsigned char *colormap;     /* tableau de colormap                      */
   unsigned char *pch;
   int taille;
   long li;

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
   Constitution du nom du fichier SUN_RASTER srf
   .................................................*/
   nom_fic[0]='\0';
   if((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.srf",repert,nom_image);

   }
   else
   {
      sprintf(nom_fic,"%s.srf",nom_image);
   }
/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=fopen(nom_fic,"rb");
   if (fic_in==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier sun_raster");
   }
   else
   {
/*....................................
    positionnement sur le nombre de colonnes */
    fseek(fic_in,4,0);

/*  recuperation du nombre de colonnes 
............................................*/

    li=fread(nb_col,4,1,fic_in);

 /*....................................
     recuperation du nombre de lignes
............................................*/

     fread(nb_lig,4,1,fic_in);

/*......................................
     recuperation du nombre de canaux
............................................*/
     fread(nb_can,4,1,fic_in);
     if ((*nb_can!=8)&&(*nb_can!=24))
     {
       iret=0;
       strcpy(CAI_ERREUR,"Profondeur encodage ni 8 ni 24. Format non traite");
      }
      else
      {

       *nb_can=(*nb_can)/8;
/*.........................................
       recuperation du type de fichier
  ............................................*/

        fseek(fic_in,4,1);
        fread(&ras_type,4,1,fic_in);
	if (ras_type!=1)
        {
          iret=0;
          strcpy(CAI_ERREUR,"Type de fichier non standard.Non traite");
         }
         else
         {
/*.........................................
	    recuperation du nombre d'octets de colormap
  ............................................*/
            fseek(fic_in,4,1);
            fread(nboct_pix,4,1,fic_in);
            image1->COLORMAP=*nboct_pix;
	    if (image1->COLORMAP != 0)
            {
/*.....................................................
              lecture de la colormap si elle existe
  ......................................................*/
	      colormap = (unsigned char *) malloc(image1->COLORMAP);
              image1->TABCOLOR = (int *)malloc(sizeof(int)*768);
              if (image1->TABCOLOR == NULL)
              {
                iret=CAI_KO;
                strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
               }

              if (colormap == NULL)
              {
                iret=CAI_KO;
                strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
               }
               else
               {
	         li=fread(colormap,image1->COLORMAP,1,fic_in);
                 pch=colormap;
	         taille=(image1->COLORMAP/3);
/*
-------------    recuperation pour chaque couleur R V B ----*/
                 for(j=0;j<3;j++)
                 {
                   for (i=0;i<taille;i++)
                   {
		     ind=i+(j*256);
                     image1->TABCOLOR[ind]=(int)(*pch);
                     pch=pch+1;
                   }
                   for (i=taille;i<256;i++)
                   {
		     ind=i+(j*256);
                     image1->TABCOLOR[ind]=0;

                   }
	          }
                  free(colormap);
                 }
             }
/*.........................................
	    recuperation du nombre d'octets par pixel
  .................................................*/
	    *nboct_pix=1;
            image1->NB_BITS_PIX=1;
	    sprintf(image1->COD_PIX,"OCT");

            }
           }
        }

	if (fic_in!=NULL) fclose(fic_in);
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
     fic=NULL;
     fic=open(nom_fic,O_RDONLY|O_BINARY,0);
     if (fic == -1)
      {
	 iret=0;
         strcpy(CAI_ERREUR,"Erreur ouverture fichier sun_raster");
      }
     else
     {

       deb = image1->NOM_FIC_CAN;
       deb1= image1->NOM_UTIL_CAN;

       for (i=0;i<*nb_can;i=i+1)
       {
	   sprintf(deb,"%s.srf",nom_image);
  	   deb=deb+100;
	   sprintf(deb1,"%s.srf",nom_image);
           deb1=deb1+100;
           image1->POS_OCTET[i]=32+image1->COLORMAP;
           image1->ACCES_CANAL_INT[i]=fic;
        }

      }
    }
    if (iret == 0)
	return (NULL);
    else     
    return(image1);
  
}
/* 
   Fin de l'operation cai_ouvre_lecture_sun_raster
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_sun_raster                              */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format rasterfile et de tous les fichiers qui la composent   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_sun_raster(nom_image,    */
/*  ---------------- 		                               nb_can,       */
/*				                               nboct_pix,    */
/*				                               nb_col,       */
/*				                               nb_lig,       */
/*				                               label)        */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*  cai_ouvre_creation_sun_raster (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


#define RAS_MAGIC 0x59a66a95


CAI_IMAGE *cai_ouvre_creation_sun_raster(char *repert,
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
   long retour;			/* retour de fonction 			    */ 
   int i,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   unsigned char *buff;         /* pointeur sur buffer fichier              */
   int	taille;		        /* taille du fichier en octets		    */
   int     ras_depth;           /* profondeur d'encodage du pixel 8 ou 24   */
   int     ras_magic;           /* nombre magique raster file               */
   int     ras_length;          /* nb octets total de l'image               */
   int     ras_type;		/* type de fichier 1=sun raster standard    */

  
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


/*************************************************/
/* Les donnees doivent etre codees sur 8 bits	 */
/*************************************************/

   if ( *nboct_pix != 1 )
   {
	strcpy ( CAI_ERREUR ,
	   "Erreur : les donnees doivent etre codees sur 8 bits\n");
	goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 4.2 :FA:Verification que le nombre de canaux est 1 ou 3.  */
/* sunrasterfile est un format de  restitution	                             */
/*****************************************************************************/
/* Verification sur le nombre de canaux 1(noir&blanc) ou 3 (RVB) */
   if ((*nb_can !=1)&&(*nb_can !=3))
   {
     strcpy(CAI_ERREUR,"Erreur une image RASTERFILE a 1 ou 3 canaux exclusivement");
     goto ERREUR;
   }

/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja .
  .................................................*/

   nom_fic[0]='\0';
   if((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.srf",repert,nom_image);
    }
   else
   {
      sprintf(nom_fic,"%s.srf",nom_image);
   }

    retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s SUN_RASTER existe \n" , nom_image );
	goto ERREUR;
   }

/*************************************************/
/* creation du fichier et reservation de la      */
/* place necessaire a l'ecriture de l'entete	 */
/*************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/

    fic = fopen ( nom_fic, "wb" );
    if ( nom_fic == NULL )
    {
   	sprintf ( CAI_ERREUR , "Erreur : creation fichier %s impossible\n" ,
	nom_fic );
	goto ERREUR;
     }

     taille=32 ;

     buff = (unsigned char *) malloc (taille);
     if ( buff== NULL )
     {
	strcpy ( CAI_ERREUR , "Erreur : probleme d'allocation \n" );
	unlink(nom_fic);
	goto ERREUR;
      }

       ras_length=(*nb_lig)*(*nb_col)*(*nb_can);
       if ((*nb_col)%2!=0)
         ras_length=ras_length+(*nb_lig);
       ras_magic=RAS_MAGIC;
       ras_depth=(*nb_can)*8;
       ras_type=1;
       memcpy ( &buff[0] , &ras_magic , 4);
       memcpy ( &buff[4] , nb_col , 4 );
       memcpy ( &buff[8] , nb_lig , 4 );
       memcpy ( &buff[12] , &ras_depth , 4 );
       memcpy ( &buff[16] , &ras_length, 4 );
       memcpy ( &buff[20] , &ras_type, 4 );


       for ( i = 24 ; i < taille ; i++ )
       {
	buff[i] = 0;
        }

        retour= fwrite ( buff , taille , 1 , fic );
        if ( retour == 0 )
        {
	sprintf (CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n",
	nom_fic );
	goto ERREUR;
        }



       free ( buff );
       if (fic!=NULL) fclose(fic);

/*.................................................
   Ouverture du fichier pour ecriture radiometrie
  ...................................................*/
   num = open(nom_fic,O_RDWR|O_BINARY,0);
   if (num == -1)
   {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic);
     goto ERREUR;
   }
   for (i=0;i<*nb_can;i=i+1)
   {
     image1->ACCES_CANAL_INT[i]=num;
     image1->POS_OCTET[i]=32;
   }

  return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_sun_raster
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_sun_raster                               */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image rasterfile           */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_OK_KO cai_lecture_canal_sun_raster(image1, canal,  */       		      /* ------------------                                  premiere_ligne,       */
/*                                          	       premiere_colonne,     */
/*                                          	       nombre_lignes_lire ,  */
/*                                          	       nombre_colonnes_lire, */
/*                                           	       pas_ligne ,           */
/*						       pas_colonne,          */
/*                 		             	       data_image )          */
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
/*  cai_lecture_canal_sun_raster (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_lecture_canal_sun_raster( CAI_IMAGE *image1 ,  
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
 long pt_oct,n;		    /* position courante dans le fichier image       */
 int no_oct;                /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int   oct_pix;             /* nb octets par pixels                          */
 long  nb_oct;              /* nb oct =1er octet a lire - position courante  */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 int Arrond;                /* pixels rajoutes pour arrondi 2 octets         */


/*...........................
  Initialisations diverses
  ..........................*/
   iret=CAI_OK;
   strcpy(CAI_ERREUR,"        ");
   lig=*premiere_ligne;
   i=1;
   pch=data_image;
   num = image1->ACCES_CANAL_INT[*canal-1];
   oct_pix = image1->NBOCT_PIX;

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */

   buf_lec = (unsigned char *) malloc(image1->NB_COL*oct_pix*image1->NB_CANAUX);
   if (buf_lec == NULL)
   {
      iret=CAI_KO;
      strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
   }
   else
   {
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
            if ( image1->NB_COL%2 == 0 )
               Arrond=0;
            else
	       Arrond=(lig-1);
            no_oct= image1->POS_OCTET[*canal-1] + Arrond +
		   (lig-1)*(image1->NB_COL)*(oct_pix)*(image1->NB_CANAUX);


/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
            nb_oct=(long)(no_oct-pt_oct);
            lseek(num,nb_oct,1);

/*...........................................
            lecture d'une ligne image
  ................................................*/
            n=read(num,buf_lec,(image1->NB_COL)*(oct_pix)*(image1->NB_CANAUX));

            if (n<=0)
            {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image sun_raster");
            }
            else
            { 
/*......................................
             Sauvegarde des donnees lues en tenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/           
             
            deb=buf_lec+(((*premiere_colonne)-1)*(oct_pix)*(image1->NB_CANAUX))+
		(image1->NB_CANAUX-(*canal));
             j=1;
             while (j <= *nombre_colonnes_lire)
	     {                         
		for(k=0;k<oct_pix;k++)
		{
              	     *(pch+k)=*(deb+k);
		}
              j=j+1;
              pch=pch+oct_pix;
              deb=deb+((*pas_colonne)*(oct_pix)*(image1->NB_CANAUX));
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
  Fin de l'operation cai_lecture_canal_sun_raster
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_sun_raster                              */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image rasterfile           */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_OK_KO cai_ecriture_canal_sun_raster(image1, canal, */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_sun_raster (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_sun_raster (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image ) 
{
long	position_first_lig;	/* position de la 1ere ligne a ecrire   */
long	position_courante;	/* position courante dans le fichie	*/
long	deplacement;		/* deplacement a effectuer		*/
int	i_ligne;		/* compteur de lignes 			*/
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire */
long	cr_write;		/* code retour de write 		*/
int num;                        /* descripteur de fichier 		*/
int nboct_pix;			/* nombre d'octets par pixel            */
int i,j;			/* indice de boucle sur les colonnes    */
long  cr_read;			/* code retour de la fonction read      */
int nb;				/* nombre d'octets lus par le read      */
unsigned char *deb,*pch;	/* pointeur sur chaine pour transfert   */
int nb_canaux;			/* nombre de canaux de l'image          */
int Arrond;                     /* arrondi pour les lignes non paires   */

/* ................
   Initialisations 
  .................*/
i_ligne = 0;
num = image1->ACCES_CANAL_INT[(*canal)-1];
nboct_pix = image1->NBOCT_PIX;
nb_canaux = image1->NB_CANAUX;

/*************************************************/
/* Les donnees doivent etre codees sur 8 bits	 */
/*************************************************/

if ( nboct_pix != 1 )
{
	strcpy ( CAI_ERREUR ,
	    "Erreur : les donnees doivent etre codees sur 8 bits\n");
	goto ERREUR;
}


/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/
Arrond=0;
if (*nombre_colonnes_ecrire%2 !=0)
    Arrond=((*premiere_ligne) - 1);
position_first_lig = (long) ( image1->POS_OCTET[*canal-1] + Arrond +
       ((*premiere_ligne) - 1) *(*nombre_colonnes_ecrire)*nb_canaux );
/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

position_courante = lseek ( num , 0L , 1 );


/************************************************************************/
/* Deplacement jusqu'au premier pixel de la premiere ligne a ecrire 	*/
/************************************************************************/

deplacement = position_first_lig - position_courante;
lseek ( num , deplacement , 1 ); 
nb=(*nombre_colonnes_ecrire)*nb_canaux;
if (*nombre_colonnes_ecrire%2 !=0)
	nb=nb+1;
buff_lig=(unsigned char *)(calloc(nb,sizeof(unsigned char)));
if (buff_lig == NULL)
{
      strcpy(CAI_ERREUR,"Probleme d'allocation memoire ecriture");
      goto ERREUR;
 }

pch=data_image;

/********************************************************/
/* TANT QUE il y a des lignes a ecrire                 	*/
/********************************************************/
while (i_ligne < (*nombre_lignes_ecrire))
{

cr_read=read(num,buff_lig,nb);
if (cr_read <0)
	   {  
	    strcpy ( CAI_ERREUR , "Erreur ecriture canal image sun_raster\n" );
	    goto ERREUR;
	    }
else
	   {
             if (cr_read==0)
  	     {
  		 for (i=0;i<nb;i++)
     		   *(buff_lig+i)=0;
                 deplacement=0;
  	     }
             else
		deplacement= - nb;
           }
/************************************************************************/
/* Redeplacement jusqu'au premier pixel de la premiere ligne a ecrire 	*/
/************************************************************************/
lseek ( num , deplacement , 1 ); 

/*************************************************************************/
/*chargement de la ligne en entrelacant les pixels par rapport aux canaux*/
/*************************************************************************/

deb=buff_lig+(nb_canaux-(*canal));
for (j=0;j<(*nombre_colonnes_ecrire);j=j+1)
{
	*deb=*pch;
	pch=pch+1;
	deb=deb+nb_canaux;
}

/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/

	   cr_write = write ( num , buff_lig , nb );

	   if ( cr_write != nb )
	   {  
	    strcpy ( CAI_ERREUR , "Erreur ecriture canal image sun_raster\n" );
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
free(buff_lig);

return (CAI_OK);

ERREUR :

return (CAI_KO);


}

/* 
  Fin de l'operation cai_ecriture_canal_sun_raster
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_sun_raster                                       */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format rasterfile et de    */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_sun_raster(image1)                */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_sun_raster (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ferme_sun_raster (CAI_IMAGE *image1)


{
 long           iret;	       /* code retour de l'operation      */
 int           num;	       /* descripteur du fichier a fermer */
 unsigned char *colormap;      /* tableau de colormap             */
 unsigned char *pch;           /* pointeur de travail             */
 int            i;	       /* indice de boucle		  */
 long int       pos_cour,li;   /* position courante               */
 long int       deplacement;   /* deplacement a effectuer         */
 unsigned char *buf;           /* buffer de lecture de l'image    */ 
 int taille;                   /* taille totale de l'image (oct)  */ 
 int val;		       /* variable intermediaire          */
   	 

 iret=CAI_OK;
 num=image1->ACCES_CANAL_INT[0];

/*.............................................
   Traitement de la Colormap si ecriture
 .............................................*/
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA :  == au lieu de = simple                        */
/*                                         image1->MODE == CAI_ECRITURE      */
/*****************************************************************************/

  if ((image1->COLORMAP !=0) && (image1->MODE == CAI_ECRITURE))
  {
/*--- recuperation de la position courante ---*/
      pos_cour=lseek(num,0L,1);

/*-- on se positionne sur le debut de la radiometrie  --*/
      deplacement = 32 - pos_cour;
      lseek(num,deplacement,1);

/*--- lecture de l'image rasterfile ---*/
      taille = image1->NB_LIG*image1->NB_COL*image1->NB_CANAUX;
      buf = (unsigned char *)malloc(taille);
      li=read(num,buf,taille);

/*--- recuperation de la position courante ---*/
      pos_cour=lseek(num,0L,1);

/*-- on se positionne sur le debut des caracteristiques COLORMAP  --*/
      deplacement = 24 - pos_cour;
      lseek(num,deplacement,1);

/*-- positionnement de l'indicateur de presence colormap --*/
     val=1;
     li=write(num,(char *)(&val),4);
     
/*-- positionnement de la taille de la colormap --*/
     val=image1->COLORMAP;
     li=write(num,(char *)(&val),4);

/*--- recuperation de la Colormap ---*/
      colormap = (unsigned char *) malloc(image1->COLORMAP);
      pch=colormap;
      for (i=0;i<image1->COLORMAP;i++)
      {
        *pch = (unsigned char )(image1->TABCOLOR[i]);
         pch=pch+1;
      }

/*--- ecriture des octets de colormap dans l'image ---*/
     iret=write(num,colormap,image1->COLORMAP);
     if (iret != image1->COLORMAP)
     {
        strcpy(CAI_ERREUR,"Erreur ecriture Colormap rasterfile ");
        iret=CAI_KO;
     }
     else
     {
/*--- ecriture de l'image ---*/
     iret=write(num,buf,taille);
     }
     free(buf);
     free(colormap);
 

   }

/*.............................................
   Fermeture de tous les canaux qui sont tous contenus
   dans un seul fichier .srf
  .............................................*/

   if (num>0) close(num);

  return (iret);
}

/*
  Fin de l'operation cai_ferme_sun_raster
*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_sun_raster                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en rasterfile et de  */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_sun_raster(format,nom_ima)     */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*   cai_dest_ima_sun_raster (S) (CAI_OK_KO) : = CAI_KO si pb                */
/*                                             = CAI_OK_KO si OK             */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_dest_ima_sun_raster (char * repertoire , 
                              	    char *nom_ima )

{

char	nom_fic[1024];		/* nom du fichier entete a detruire 	     */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image 		     */
int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier     */ 


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
sprintf ( nom_fic , "%s.srf" ,nom_ima_complet);

/************************************************/
/* Destruction du fichier sun_raster 		*/
/************************************************/

cr_stat = stat ( nom_fic , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic );


return (CAI_OK);


}
/* 
 Fin de l'operation cai_dest_ima_sun_raster
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_sun_raster                               */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----    rasterfile et de tous les fichiers qui la composent              */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_modifie_sun_raster(nom_image,     */
/*  ---------------- 		                              nb_can,        */
/*				                              nboct_pix      */	
/*				                              nb_col,        */
/*				                              nb_lig)        */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*  cai_ouvre_modifie_sun_raster (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */  
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_sun_raster(char *repert,
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
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int i,j,ind;			/* indice 				    */
   int ras_type;                /* type de fichier raster                   */
   int fic;			/* code retour fonction open		    */
   unsigned char *colormap;     /* tableau de colormap                      */
   unsigned char *pch;
   int taille;
   long li;

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
   Constitution du nom du fichier SUN_RASTER srf
   .................................................*/
   nom_fic[0]='\0';
   if((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.srf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.srf",nom_image);
   }
/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=fopen(nom_fic,"rb");
   if (fic_in==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier sun_raster");
   }
   else
   {
/*....................................
    positionnement sur le nombre de colonnes */
    fseek(fic_in,4,0);

/*  recuperation du nombre de colonnes 
............................................*/

     li=fread(nb_col,4,1,fic_in);

 /*....................................
     recuperation du nombre de lignes
............................................*/

     fread(nb_lig,4,1,fic_in);

/*......................................
     recuperation du nombre de canaux
............................................*/
     fread(nb_can,4,1,fic_in);
     if ((*nb_can!=8)&&(*nb_can!=24))
     {
       iret=0;
       strcpy(CAI_ERREUR,"Profondeur encodage ni 8 ni 24. Format non traite");
      }
      else
      {

       *nb_can=(*nb_can)/8;
/*.........................................
       recuperation du type de fichier
  ............................................*/

        fseek(fic_in,4,1);
        fread(&ras_type,4,1,fic_in);
	if (ras_type!=1)
        {
          iret=0;
          strcpy(CAI_ERREUR,"Type de fichier non standard.Non traite");
         }
         else
         {
/*.........................................
	    recuperation du nombre d'octets de colormap
  ............................................*/
            fseek(fic_in,4,1);
            fread(nboct_pix,4,1,fic_in);
            image1->COLORMAP=*nboct_pix;
	    if (image1->COLORMAP != 0)
            {
/*.....................................................
              lecture de la colormap si elle existe
  ......................................................*/
	      colormap = (unsigned char *) malloc(image1->COLORMAP);
              image1->TABCOLOR = (int *)malloc(sizeof(int)*768);
              if (image1->TABCOLOR == NULL)
              {
                iret=CAI_KO;
                strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
               }

              if (colormap == NULL)
              {
                iret=CAI_KO;
                strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
               }
               else
               {
	         li=fread(colormap,image1->COLORMAP,1,fic_in);
                 pch=colormap;
	         taille=(image1->COLORMAP/3);
/*
-------------    recuperation pour chaque couleur R V B ----*/
                 for(j=0;j<3;j++)
                 {
                   for (i=0;i<taille;i++)
                   {
		     ind=i+(j*256);
                     image1->TABCOLOR[ind]=(int)(*pch);
                     pch=pch+1;
                   }
                   for (i=taille;i<256;i++)
                   {
		     ind=i+(j*256);
                     image1->TABCOLOR[ind]=0;

                   }
	          }
                  free(colormap);
                 }
             }
/*.........................................
	    recuperation du nombre d'octets par pixel
  .................................................*/
	    *nboct_pix=1;
            image1->NB_BITS_PIX=1;
	    sprintf(image1->COD_PIX,"OCT");
            }
           }
        }

	if (fic_in!=NULL) fclose(fic_in);
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     fic=open(nom_fic,O_RDWR|O_BINARY,0);
     if (fic == -1)
      {
	 iret=0;
         strcpy(CAI_ERREUR,"Erreur ouverture fichier sun_raster");
      }
     else
     {

       deb = image1->NOM_FIC_CAN;
       deb1= image1->NOM_UTIL_CAN;

       for (i=0;i<*nb_can;i=i+1)
       {
	   sprintf(deb,"%s.srf",nom_image);
  	   deb=deb+100;
	   sprintf(deb1,"%s.srf",nom_image);
           deb1=deb1+100;
           image1->POS_OCTET[i]=32+image1->COLORMAP;
           image1->ACCES_CANAL_INT[i]=fic;
        }

      }
   }
   if (iret == 0)
	return (NULL);
   else     
      return(image1);
  
}/* 
   Fin de l'operation cai_ouvre_modifie_sun_raster
*/
