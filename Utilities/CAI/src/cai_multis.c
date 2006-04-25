/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_multis.c                                                  */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format multiscope                                 */ 
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
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : modifications pour compatibilite HP64bits      */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>
# include <sys/types.h>
#include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>

#define _CAI_IMAGE
# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_multis.c,v 1.8 01/05/31 10:47:08 cmf Exp $";


extern char *getenv();


static void inverser (char * s , int nboct );
static void calculer_histo(CAI_IMAGE *image1, unsigned char *buffer ,
				int nb_pix , int canal );

/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_ima_multis                                       */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format multiscope             */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_multis (repert,tab_ima)         */
/* -----------------                                                         */
/*                                                                           */
/*     repert        (E) (char *)   : repertoire a explorer		     */
/*     tab_ima       (S) (char ***) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_multis    (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


long int cai_liste_ima_multis (char *repert,
		          char ***tab_ima)
 

{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */

struct dirent *dp;   /* structure resultat de la fonction readdir   */

   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2,*pch3;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images multiscope identifiees      */
   int i;		/* indice de boucle			       */

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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE MULTISCOPE
      (EXTENSION =.img)
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
         car='m';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
	      car='g';
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

    (void)closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT MULTISCOPE
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE MULTISCOPE
      (EXTENSION =.img)
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
         car='m';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
         {
	      car='g';
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

/* Fin de l'operation cai_liste_ima_multis
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_multis                                   */
/* ----------								     */
/*                                                                           */
/* ROLE : fonction d'ouverture en lecture d'une image au format multiscope   */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_multis (nom_image,       */
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
/*     cai_ouvre_lecture_multis  (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_multis(char *repert,
				    char *nom_image,
				    int *nb_can,
				    int *nboct_pix,
				    int *nb_col,
				    int *nb_lig)

{
   FILE *fic;			/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];	/* nom du fichier canal			    */
   char nom_util_canal[1024];	/* nom du fichier utilisateur		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int num,retour,lon;	        /*  variables de travail                    */
   short int type,fin;          /* variables de trvail			    */

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
 
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   *nb_can=0; 
   iret=1;
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      strcat(nom_fic,".img");
      nom_fic[strlen(repert)+1+strlen(nom_image)+4]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".img");
      nom_fic[strlen(nom_image)+4]='\0';
   }
   
/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic=fopen(nom_fic,"rb");
   if (fic == NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete MULTISCOPE");
    }
   else
   {
     
/*....................................
          positionnement sur le nombre de colonnes
*/
	  fseek(fic,8,0);
/*
	  recuperation du nombre de colonnes */
	  fread(nb_col,4,1,fic);
/*
	  inversion des octets pour compatibilite SUN */
	  lon=sizeof(*nb_col)-1;
	  inverser((char *)nb_col,lon);

/*....................................
	  recuperation du nombre de lignes
*/
	  fread(nb_lig,4,1,fic);
/*
	  inversion des octets pour compatibilite SUN */
	  lon=sizeof(*nb_lig)-1;
	  inverser((char *)nb_lig,lon);

/*......................................
	  initialisation du nombre d'octets par pixel
*/
	  *nboct_pix=1;
          image1->NB_BITS_PIX=8;
	  sprintf(image1->COD_PIX,"OCT");
/*.................................
	  positionnement sur le premier objet
*/
	  fseek(fic,114,1);
	  retour=0;
          deb = image1->NOM_FIC_CAN;
          deb1= image1->NOM_UTIL_CAN;


	  while (retour == 0)
	  {
/*...........Recuperation de tous les objets jusqu'a l'eof
	     nom fichier,nom utilisateur et type de l'objet(avec inversion)
*/
	     fread(nom_fic_canal,14,1,fic);
	     fread(nom_util_canal,40,1,fic);
	     fread(&type,2,1,fic);
	     lon=sizeof(type)-1;
	     inverser((char *)&type,lon);
	     if (type == 3)
	      {
/*.......................................
                 Initialisation des tableaux contenant le nom de fichier
                 et le nom utilisateur de chaque canal
  .....................................................*/


          	if (*nb_can<CAI_NB_MAX_CAN)
          	{
		  nom_fic_canal[0] = tolower(nom_fic_canal[0]);
		  nom_fic_canal[1] = tolower(nom_fic_canal[1]);
		  nom_fic_canal[2] = tolower(nom_fic_canal[2]);
		  nom_fic_canal[9] = tolower(nom_fic_canal[9]);
		  nom_fic_canal[10] = tolower(nom_fic_canal[10]);
		  nom_fic_canal[11] = tolower(nom_fic_canal[11]);
  	          nom_fic[0]='\0';
                  if ((long int)strlen(repert) != 0)
                  {
                    sscanf(repert,"%s",nom_fic);
                    strcat(nom_fic,"/");
                    strcat(nom_fic,nom_fic_canal);
                    nom_fic[strlen(repert)+1+strlen(nom_fic_canal)]='\0';
       		   }
       		  else
       		  {
                     sscanf(nom_fic_canal,"%s",nom_fic);
                     nom_fic[strlen(nom_fic_canal)]='\0';
    		  }
        	  sscanf(nom_fic_canal,"%s",deb);
	          deb=deb+100;
        	  sscanf(nom_util_canal,"%s",deb1);
	          deb1=deb1+100;

                  num=open(nom_fic,O_RDONLY,0);
                  if (num==-1)
                  {
                    iret=0;
                    strcpy(CAI_ERREUR,"Erreur ouverture fi canal Multiscope");
       		   }
       		  else
       		  {
                    image1->ACCES_CANAL_INT[*nb_can]=num;
		    image1->POS_OCTET[*nb_can]=1280;
	            image1->NB_PIX_ECRITS[*nb_can]=0;
                  }

        	}
	        *nb_can=*nb_can+1;
	      }

/*............Positionnement en fin de bloc */
	      fseek(fic,70,1);
/*
  ............Test fin de fichier */
	      fread(&fin,2,1,fic);
	      retour=feof(fic);
	  }      
/*..................................
          Fermeture du fichier 
  ......................................*/
          fclose(fic);
       }

 
   if (iret != 0)
      return (image1);
    else
     return(NULL);

}

/* 
   Fin de l'operation cai_ouvre_lecture_multis
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_multis                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format multiscope et de tous les fichiers qui la composent   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_multis (nom_image,       */
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
/*     cai_ouvre_creation_multis  (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_IMAGE *cai_ouvre_creation_multis(char *repert,
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
   char fichier[1024];          /* variable intermediaire		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   long int retour;		/* retour de fonction 			    */ 
   char car3[3],car6[6];	/* indice du canal traite     		    */
   int i,num,numero_fic;	/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   unsigned char *buff ;        /* buffer a ecrire                          */
   int taille,nb;	        /* variables de travail sur taille fichier  */
   char *deb;			/* pointeur sur chaine pour transfert       */
   int	col_pix_1;		/* colonne du premier pixel		    */
   int	lig_pix_1;		/* ligne du premier pixel 		    */
   char	*nom_fic_carac;		/* nom relatif du fichier caracteristiques  */	
   long int taille_nom_dos_vue; /* taille de fichier 			    */
   char	*nom_dos_vue;		/* nom DOS de la vue			    */
   long int taille_nom_dos_ima;	/* taille de fichier		            */
   char	*nom_dos_ima;		/* nom DOS de l'image			    */
   int	flag_sauvegarde;	/* flag de sauvegarde			    */
   char	*nom_dos_obj;		/* nom DOS de l'objet			    */
   char	nom_util[100+1];	/* intitule ou nom utilisateur		    */
   int	flag_suppression;	/* flag de suppression 			    */
   int	type_obj;		/* type de l'objet		            */
   int	version;		/* version 				    */
   int	filler;			/* filler				    */
   int	dim_bloc;		/* dimension d'un bloc 			    */
   int	presence_histo;		/* indicateur de presence des histogrammes  */
   int	int_tempo;		/* entier utilise pour inversion d'octets   */
   int	lon;			/* longueur de l'entier - 1                 */
   int i_can;			/* indice de boucle sur les canaux          */
   long li;

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      strcat(nom_fic,".img");
      nom_fic[strlen(repert)+1+strlen(nom_image)+4]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".img");
      nom_fic[strlen(nom_image)+4]='\0';
   }
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s MULTISCOPE existe \n" , nom_image );
	goto ERREUR;
   }

/*...............................................................
  Pour chaque canal de l'image,constitution du nom de fichier correspondant
  Verification qu'il n'existe pas deja
  Ouverture du fichier en ecriture
  .................................................................*/

  deb =(char *) image1->NOM_FIC_CAN;
  i=0;
  numero_fic=0;
  while ((i<*nb_can)&& (numero_fic<10000))
  {
   nom_fic_canal[0]='\0';
   fichier[0]='\0';
   sprintf ( car6 , "%05d" , numero_fic );
   strcat ( fichier,"fdo");
   strcat ( fichier , car6);
   strcat ( fichier , ".per" );
   fichier[strlen(car6)+7]='\0';

   if ((long int)strlen(repert) != 0)
   {
     sscanf(repert,"%s",nom_fic_canal);
     strcat(nom_fic_canal,"/fdo");
     strcat ( nom_fic_canal , car6 );
     strcat ( nom_fic_canal , ".per" );
     nom_fic_canal[strlen(repert)+4+strlen(car6)+4]='\0';
    }
    else
    {
     sscanf(fichier,"%s",nom_fic_canal);
     nom_fic_canal[strlen(car6)+7]='\0';
    }
   retour = stat ( nom_fic_canal , &buf_stat );
   if ( retour != 0 )  /* si ce fichier n'existe pas il peut etre creer */
   {    
      sscanf(fichier,"%s",deb);
      i=i+1;
      deb=deb+100;
    }
    numero_fic=numero_fic+1;

   }

/*..................................................................
   ecriture du fichier img et de l'entete des fichiers canaux
  .................................................................*/
  fic = fopen(nom_fic,"w");
  if ( fic == NULL )
   { 
	sprintf ( CAI_ERREUR , "Creation image multiscope impossible \n" ,
		   nom_image );
	goto ERREUR;
   }
/* la taille du fichier entete doit etre multiple de 512 */
   taille = 128*(1+(*nb_can));
   nb=taille;
   if ( taille % 512 == 0 )
	taille = taille;
   else
	taille = taille - (taille%512) + 512;


/*************************************************/
/* Ecriture des enregistrements d'entete et de   */
/* description des objets :		         */
/* 128 + n*128 octets avec n = nombre d'objets   */
/*************************************************/

/*************************************************/
/* ecriture de l'enregistrement d'entete	 */
/*************************************************/
   buff = (unsigned char *) calloc ( 128,sizeof(unsigned char) );
   if ( buff == NULL )
   {
	sprintf ( CAI_ERREUR , "Erreur : probleme d'allocation \n" );
	unlink(nom_fic);
	goto ERREUR;
    }

    col_pix_1 = 0;
    int_tempo = col_pix_1;
    lon = sizeof(int_tempo)-1;
    inverser((char *)&int_tempo,lon);
    memcpy ( buff , &int_tempo , 4 );

    lig_pix_1 = 0;
    int_tempo = lig_pix_1;
    lon = sizeof(int_tempo)-1;
    inverser((char *)&int_tempo,lon);
    memcpy ( &buff[4] , &int_tempo , 4 );

    int_tempo = (*nb_col);
    lon = sizeof(int_tempo)-1;
    inverser((char *)&int_tempo,lon);
    memcpy ( &buff[8] , &int_tempo , 4 );

    int_tempo = (*nb_lig);
    lon = sizeof(int_tempo)-1;
    inverser((char *)&int_tempo,lon);
    memcpy ( &buff[12] , &int_tempo , 4 );
/* construction du nom relatif du fichier */
/* caracteristiques : nom_image.img */
    nom_fic_carac = (char *) strrchr ( nom_fic , '/' ) + 1;


   taille_nom_dos_vue =
   ( (long int)(strlen(nom_fic_carac))-3 > 15 )? 
	(long int)(strlen(nom_fic_carac))-3 : 15;
     nom_dos_vue = (char *) malloc ( taille_nom_dos_vue*sizeof(char *) );
    strncpy ( nom_dos_vue , nom_fic_carac , strlen(nom_fic_carac)-4 );
    for ( li = (long int)(strlen(nom_fic_carac)-4) ; li < 14 ; li++ )
    {
	nom_dos_vue[li]=' ';
    }
    nom_dos_vue[14]='\0';
   /* passage en majuscules */
   for ( i = 0 ; i < 14 ; i++ )
   {
	nom_dos_vue[i] = toupper(nom_dos_vue[i]);
    }
/*    memcpy ( &buff[16] , nom_dos_vue , 14 ); */


    taille_nom_dos_ima =
         ( (long int)(strlen(nom_fic_carac))-3 > 15 )? 
	   (long int)(strlen(nom_fic_carac))-3 : 15;
    nom_dos_ima = (char *) malloc ( taille_nom_dos_ima*sizeof(char *) );
    strncpy ( nom_dos_ima , nom_fic_carac , strlen(nom_fic_carac)-4 );
    nom_dos_ima[strlen(nom_fic_carac)-4]='\0';
    /* le nom DOS de l'image ne doit pas depasser 10 caracteres */
    if ( (long int)(strlen(nom_dos_ima)) > 10 )
	nom_dos_ima[10]='\0';
    /* passage en majuscule */
    for ( li = 0 ; li < (long int)(strlen(nom_dos_ima)) ; li++ )
    {
	nom_dos_ima[li] = toupper(nom_dos_ima[li]);
     }
     li = (long int)strlen(nom_dos_ima)+1 ;
     memcpy ( &buff[30] , nom_dos_ima , li);
 
/*     memcpy ( &buff[30+li] , "IMG" , 3 );
     for ( li = 30+(long int)strlen(nom_dos_ima)+1+3 ; li < 44 ; li++ )
     {
	buff[li] = 0;
      }*/
 
     flag_sauvegarde = 0;
     int_tempo = flag_sauvegarde;
     lon = sizeof(int_tempo)-1;
     inverser((char *)&int_tempo,lon);
     memcpy ( &buff[44] , &int_tempo , 2 );

/*    for ( i = 46 ; i < 128 ; i++ )
    {
	buff[i] = 0;
    }*/
   if ((*nb_can)==1)
   	buff[46]='I';
   retour = fwrite ( buff , 128 , 1 , fic );
   if ( retour == 0 )
   {
    sprintf ( CAI_ERREUR ,"Ecriture impossible dans fichier %s\n" ,nom_fic);
    unlink(nom_fic);
    goto ERREUR;
    }
    free ( nom_dos_vue );
    free ( nom_dos_ima );
    free ( buff );

/*************************************************/
/* POUR	    chacun des objets			 */
/*************************************************/
   deb=image1->NOM_FIC_CAN;
   for ( i_can= 0 ; i_can< *nb_can ; i_can++ )
   {

    
     buff = (unsigned char *) calloc ( 128,sizeof(unsigned char) );
     if ( buff == NULL )
     {
	sprintf ( CAI_ERREUR , "Erreur : probleme d'allocation \n" );
	unlink(nom_fic);
	goto ERREUR;
      }
  
/*************************************************/
/*   ecriture de la description de l'objet 	 */
/*************************************************/

     flag_suppression = 1;
     int_tempo = flag_suppression;
     lon = sizeof(int_tempo)-1;
     inverser((char *)&int_tempo,lon);
     memcpy ( buff , &int_tempo , 2 );

 
     nom_fic_canal[0]='\0';
     sscanf(deb,"%s",nom_fic_canal);
     deb=deb+100;
     nom_dos_obj = (char *) calloc ( 15,sizeof(char *) );
     strcpy ( nom_dos_obj , nom_fic_canal );
     for ( li = (long int)strlen(nom_fic_canal) ; li < 14 ; li++ )
     {
		nom_dos_obj[li]='\0';
      }
      nom_dos_obj[14]='\0';
      nom_dos_obj[0] = toupper(nom_dos_obj[0]);
      nom_dos_obj[1] = toupper(nom_dos_obj[1]);
      nom_dos_obj[2] = toupper(nom_dos_obj[2]);
      nom_dos_obj[9] = toupper(nom_dos_obj[9]);
      nom_dos_obj[10] = toupper(nom_dos_obj[10]);
      nom_dos_obj[11] = toupper(nom_dos_obj[11]);
      memcpy ( &buff[2] , nom_dos_obj , 14 );

      for ( i = 6 ; i < 40 ; i++ )
      {
		nom_util[i]='\0';
       }
      nom_util[40]='\0';
      sprintf(nom_util,"CANAL/PLAN%d",i_can+1);
      memcpy ( &buff[16] , nom_util , 40 );

      type_obj = 3;
      int_tempo = type_obj;
      lon = sizeof(int_tempo)-1;
      inverser((char *)&int_tempo,lon);
      memcpy ( &buff[56] , &int_tempo , 2 );

      memcpy ( &buff[58] , "01-04-01 10:10" , 14 );

      for ( i = 72 ; i < 128 ; i++ )
      {
		buff[i] = 0;
      }

     retour = fwrite ( buff , 128 , 1 , fic );
     if ( retour == 0 )
     {
       sprintf ( CAI_ERREUR ,"Ecriture impossible dans fichier %s\n" ,nom_fic);
       unlink(nom_fic);
      goto ERREUR;
      }
      free(buff);
/*..........................................................
   Ecriture de 0 jusqu'a la taille du fichier
  .........................................................*/
    if (nb != taille)
    {
       buff = (unsigned char *) calloc ( taille-nb,sizeof(unsigned char) );
       if ( buff == NULL )
       {
	sprintf ( CAI_ERREUR , "Erreur : probleme d'allocation \n" );
	unlink(nom_fic);
	goto ERREUR;
        }
       retour = fwrite ( buff , taille-nb , 1 , fic );
       if ( retour == 0 )
       {
        sprintf ( CAI_ERREUR ,"Ecriture impossible dans fichier %s\n" ,nom_fic);
        unlink(nom_fic);
        goto ERREUR;
        }
       free(buff);
      }
    }
    fclose(fic);

  /*************************************************/
  /* POUR      chacun des fichiers canaux	   */
  /*************************************************/
  deb=image1->NOM_FIC_CAN;
  for ( i_can = 0 ; i_can< *nb_can ; i_can++ )
  {

/*************************************************/
/*	ouverture du fichier            	 */
/*************************************************/
       fichier[0]='\0';
      if ((long int)strlen(repert)!=0)
      {
       sscanf(repert,"%s",fichier);
       strcat(fichier,"/");
       strcat(fichier,deb);
       }
       else
       {
         sscanf(deb,"%s",fichier);
        }
       deb=deb+100;

	fic = fopen ( fichier , "w" );
	if ( fic == NULL )
	{
	   sprintf ( CAI_ERREUR , "Erreur : ouverture fichier %s impossible \n",
		     fichier);
		goto ERREUR ;
	 }


/*************************************************/
/*	ecriture de l'enregistrement d'entete	 */
/*	du filler et des enregistrements	 */
/*	histogrammes	 			 */
/* 	24 + 232 + 4*256 octets 		 */
/*************************************************/

/*************************************************/
/*	ecriture de l'enregistrement d'entete	 */
/*************************************************/

	buff = (unsigned char *) calloc ( 24 ,sizeof(unsigned char));
	if ( buff == NULL )
	{
		strcpy ( CAI_ERREUR , "Erreur : probleme d'allocation\n" );
		goto ERREUR;
	}

	version = 1;
	int_tempo = version;
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( buff , &int_tempo , 2 );

	type_obj = 3;
	int_tempo = type_obj;
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( &buff[2] , &int_tempo , 2 );

/*	memcpy ( &buff[4] , "*CAI**" , 6 ); */

	filler = 0;
	int_tempo = filler;
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( &buff[10] , &int_tempo , 2 );

	dim_bloc = 16;
	int_tempo = dim_bloc;
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( &buff[12] , &int_tempo , 2 );

	int_tempo = (*nb_lig);
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( &buff[14] , &int_tempo , 4 );

	int_tempo = (*nb_col);
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( &buff[18] , &int_tempo , 4 );

	presence_histo = 0;
	int_tempo = presence_histo;
	lon = sizeof(int_tempo)-1;
	inverser((char *)&int_tempo,lon);
	memcpy ( &buff[22] , &int_tempo , 2 );
	retour = fwrite ( buff , 24 , 1 , fic );
	if ( retour == 0 )
	{
	  sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n", fichier );
	  goto ERREUR;
	}

	free ( buff );


/*************************************************/
/*	ecriture du filler a 0 			 */
/*************************************************/

	buff = (unsigned char *) calloc ( 232 ,sizeof(unsigned char));
	if ( buff == NULL )
	{
		strcpy ( CAI_ERREUR , "Erreur : probleme d'allocation\n" );
		goto ERREUR;
	}

	retour = fwrite ( buff , 232 , 1 , fic );
	if ( retour == 0 )
	{
	  sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n", fichier );
		goto ERREUR;
	}

	free ( buff );

/*************************************************/
/*	ecriture des histogrammes a 0          	 */
/*************************************************/

	/* initialisations de variables necessaire au calcul d'histogramme */

	for ( i=0 ; i<256 ; i++ )
	{
		image1->HISTO[i_can][i]=0;
	}

	retour = fwrite ( image1->HISTO[i_can] , (sizeof(int)*256) , 1 , fic );
	if ( retour == 0 )
	{
	  sprintf (CAI_ERREUR  , "Erreur : ecriture impossible dans fichier %s\n", fichier );
		goto ERREUR;
	}


/*************************************************/
/*	fermeture du fichier canal 		 */
/*************************************************/

	fclose ( fic );

/*................................................
         Ouverture du fichier canal pour ecriture radiometrie
  .............................................................*/
      num = open(fichier,O_RDWR,0);
      image1->ACCES_CANAL_INT[i_can]=num;
      image1->POS_OCTET[i_can]=24 +232+ 4*256;

    }

   
    free ( nom_dos_obj );


  return (image1);

ERREUR :
  return(NULL);
}

/* 
  Fin de l'operation cai_ouvre_creation_multis
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_multis                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image multiscope           */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_multis(image1, canal,     */       		      /* ------------------                               premiere_ligne,          */
/*                                          	    premiere_colonne,        */
/*                                           	    nombre_lignes_lire ,     */
/*                                           	    nombre_colonnes_lire,    */
/*                                           	    pas_ligne ,pas_colonne,  */
/*                 		             	    data_image )             */
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
/*     cai_lecture_canal_multis  (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_multis(CAI_IMAGE *image1 ,  
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
 long int pt_oct,n;	    /* position courante dans le fichier image       */
 int no_oct;                /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int l,p;		    /* variables de travail			     */
 long  nb_oct;              /* nb oct =1er octet a lire - position courante  */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 unsigned char *buffer;     /* buffer de lecture d'un pave 16*16             */
 int pave;                  /* nombre de paves colonnes dans une ligne       */
 int taille;                /* nbcol_tot*taille lignes pave                  */
 int bloc;		    /* indice de boucle sur les paves                */
 int nolig;		    /* indice de boucle sur les lignes               */
 int numlig;                /* numero de la ligne reconstituee               */
 unsigned char *ind_lec;    /* pointeur de travai pour transfert pave        */
 unsigned char *ind_tab;    /* pointeur de travai pour transfert pave        */
 int nbcol16;               /* nb pave*taille colonnes du pave  (16)         */

/*.........................
  Initialisations
  ........................*/
   iret=CAI_OK;
   num=image1->ACCES_CANAL_INT[*canal-1];
   lig=*premiere_ligne;
   i=0;
   pch=data_image;

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
   pave=image1->NB_COL/16;
   if ((image1->NB_COL%16)!=0)
   {
      pave=pave+1;
   }
   nbcol16=pave*16;
   taille=nbcol16*16;
   buf_lec = (unsigned char *) malloc(256);
   if (buf_lec == NULL)
   {
      iret=CAI_KO;
      strcpy(CAI_ERREUR,"Probleme allocation memoire");
   }
   else
   {
      buffer = (unsigned char *) malloc(taille);
      if (buffer == NULL)
      {
         iret=CAI_KO;
         strcpy(CAI_ERREUR,"Probleme allocation memoire");
      }
      else
      {
        lig=*premiere_ligne;
        i=0;
        pch=data_image;
/*.....................
         Pour chaque ligne a lire
  .......................................*/
        while ((i < *nombre_lignes_lire)&&(iret==CAI_OK))
        {  
/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
            pt_oct=lseek(num,0L,1);

/*................................
            Calcul du numero du 1er octet a lire
  ....................................*/
  	    nolig=((lig-1)/16)*16;
            no_oct=image1->POS_OCTET[*canal-1]+(nolig*nbcol16);

/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
            nb_oct=(long)(no_oct-pt_oct);

            lseek(num,nb_oct,1);
/*...........................................
            lecture de 16 lignes completes de nolig+1 a nolig+16
  ................................................*/
	    for (bloc=1;bloc<=pave;bloc=bloc+1)
	    {
/*	      lecture d'un bloc de 256 octets consecutifs */
              n=read(num,buf_lec,256);
              if (n<=0)
              {
                iret=CAI_KO;
                strcpy(CAI_ERREUR,"Erreur lecture donnee image multis");
              }
              else
              { 
/*	        stockage du pave dans le tableau de 16 lignes completes */
		ind_tab=buffer+(bloc-1)*16;
		ind_lec=buf_lec;

	        for (l=1;l<=16;l=l+1)
                {
		  for (p=1;p<=16;p=p+1)
		  {
			*ind_tab=*ind_lec;
			ind_tab=ind_tab+1;
			ind_lec=ind_lec+1;
	          }
		  ind_tab=ind_tab+nbcol16-16;
		}
	       }
              }

/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/   
       
	     if (iret==CAI_OK)
	     {
		numlig=lig-nolig;
		deb=buffer+(numlig-1)*nbcol16+(*premiere_colonne)-1;
	        while ((numlig<=16)&&(i<*nombre_lignes_lire))
		{
		  j=1;
		  while (j<=*nombre_colonnes_lire)
		  {
		    *pch=*deb;
		    pch=pch+1;
		    j=j+1;
		    deb=deb+(*pas_colonne);
	          }
		  numlig=numlig+(*pas_ligne);
	          deb=buffer+(numlig-1)*nbcol16+(*premiere_colonne)-1;
		  lig=lig+(*pas_ligne);
		  i=i+1;
	        }
	      }
        }

	free(buffer);
      }
      free(buf_lec);
   }

   return(iret);
}
/*
  Fin de l'operation cai_lecture_canal_multis
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_multis                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image multiscope           */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_multis(image1, canal,    */       		      /* ------------------                                premiere_ligne,         */
/*                                                   nombre_lignes_ecrire,   */
/*                                                   nombre_colonnes_ecrire, */
/*                 		                     data_image )            */
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
/*     cai_ecriture_canal_multis     (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_multis (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image )  
{
int num;                        /* descripteur de fichier		     */
int nboct_pix;			/* nombre d'octets par pixel                 */
int	nb_colon_correc;	/* nombre de colonnes multiple de 16 	     */
int	nb_blocs;		/* nombre de paquets de 16 pixels	     */
int	i_bloc;			/* compteur de blocs			     */
int	nb_last_pix;		/* nb initial de pixels dans le dernier bloc */
int	nb_blocs_prec_first_lig;/* nb de blocs avant 1ere ligne a ecrire     */
int	nb_blocs_prec_lig;	/* nb de blocs avant ligne courante 	     */
int	pos_first_lig_ds_bloc;	/* position 1ere ligne a ecrire dans son bloc*/
int	pos_lig_ds_bloc;	/* position ligne courante dans son bloc     */
long	position_first_oct;	/* position du 1er octet a ecrire 	     */
long	position_courante;	/* position courante dans le fichier 	     */
long	deplacement;		/* deplacement a effectuer		     */
long	depla_bl;		/* deplacement a effectuer		     */
int	i_ligne;		/* compteur de lignes 			     */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire      */
unsigned char *buff_bloc;	/* tableau contenant un bloc a ecrire        */
int	taille_wr,octwr;	/* nombre d'elements du buffer a ecrire      */
long int cr_write;		/* code retour de write 		     */
int	i,can;			/* indice de boucle     		     */
int Nb1; 		        /* Nombre d'octets de completion             */
int NbL,NbC,i_can;
unsigned char *buf;
 
/* initialisations */

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


/********************************************************/
/* Calcul du nombre de colonnes multiple de 16 de 	*/
/* l'image a ecrire					*/
/********************************************************/

if ( (*nombre_colonnes_ecrire) % 16 == 0 )
	nb_colon_correc = (*nombre_colonnes_ecrire);
else
	nb_colon_correc = (*nombre_colonnes_ecrire) - ((*nombre_colonnes_ecrire)%16) + 16;

/********************************************************/
/* Calcul du nombre de blocs complets de 16 pixels 	*/
/* sur une meme ligne (eventuellement completes par 0)	*/
/********************************************************/

nb_blocs = nb_colon_correc / 16;

/********************************************************/
/* Calcul du nombre de pixels presents initialement 	*/
/* dans le dernier bloc d'une ligne			*/
/********************************************************/

nb_last_pix = 16 - ( nb_colon_correc - (*nombre_colonnes_ecrire) );


/********************************************************/
/* Calcul du nombre de blocs de 16 lignes precedant la	*/
/* premiere ligne a ecrire				*/
/* Calcul de la position de la premiere ligne a ecrire 	*/
/* au sein d'un bloc de 16 lignes			*/
/********************************************************/
if ( (*premiere_ligne) <= 16 )
{
	nb_blocs_prec_first_lig = 0;
	pos_first_lig_ds_bloc = (*premiere_ligne);
}
else if ( ((*premiere_ligne) - 1) % 16 == 0 )
{
	nb_blocs_prec_first_lig = ((*premiere_ligne) - 1) / 16;
	pos_first_lig_ds_bloc = 1;
}
else
{
	nb_blocs_prec_first_lig =
	( ((*premiere_ligne) - 1 ) - ((*premiere_ligne) - 1)%16 ) / 16;
	pos_first_lig_ds_bloc = 1 + ((*premiere_ligne) - 1)%16;
}



/********************************************************/
/* Initialisations					*/
/********************************************************/

pos_lig_ds_bloc = pos_first_lig_ds_bloc;
nb_blocs_prec_lig = nb_blocs_prec_first_lig;


/********************************************************/
/* POUR	chacune des lignes a ecrire	               	*/
/********************************************************/
for ( i_ligne = 0 ; i_ligne < (*nombre_lignes_ecrire) ; i_ligne++ )
{

/********************************************************/
/*	calcul de la position du premier octet de la	*/
/*	ligne a ecrire					*/
/********************************************************/

	position_first_oct = (long) ( image1->POS_OCTET[*canal-1] 
			   + 16 * (pos_lig_ds_bloc - 1) 
			   + 16 * nb_blocs_prec_lig * nb_colon_correc );
/********************************************************/
/*	recuperation de la position courante		*/
/********************************************************/

	position_courante = lseek ( num , 0L , 1 );

/********************************************************/
/*	deplacement jusqu'au premier octet a ecrire	*/
/********************************************************/

	deplacement = position_first_oct - position_courante;
	lseek ( num , deplacement , 1 ); 

/********************************************************/
/*	extraction de la ligne du tableau 		*/
/********************************************************/

	buff_lig = data_image + (*nombre_colonnes_ecrire) * i_ligne;

/********************************************************/
/*	initialisation du decalage de 15*16 octets	*/
/********************************************************/

	depla_bl = 0;

/********************************************************/
/*	POUR	chacun des blocs de 16 pixels		*/
/********************************************************/
	for ( i_bloc = 0 ; i_bloc < nb_blocs ; i_bloc++ )
	{

/********************************************************/
/*		deplacement 				*/
/********************************************************/

		lseek ( num , depla_bl , 1 );

/********************************************************/
/*		extraction d'un bloc de la ligne	*/
/********************************************************/

		buff_bloc = buff_lig + i_bloc * 16;

/********************************************************/
/*		determination du nombre de pixels a 	*/
/*		ecrire					*/
/********************************************************/

		if ( i_bloc == nb_blocs - 1 )
			taille_wr = (int) nb_last_pix;
		else
			taille_wr = 16;

                image1->NB_PIX_ECRITS[(*canal)-1] = 
                      image1->NB_PIX_ECRITS[(*canal)-1] + taille_wr;

/********************************************************/
/*		ecriture dans le fichier		*/
/********************************************************/
		cr_write = write ( num , buff_bloc , taille_wr );
		if ( cr_write != taille_wr )
		{
			strcpy ( CAI_ERREUR ,
			"Erreur ecriture canal image multiscope\n" );
			goto ERREUR;
		}
/*	        ecriture des colonnes de completion si necessaire */
		octwr = 16 - taille_wr;
                buf = (unsigned char *) calloc(octwr,sizeof(unsigned char));
                if (buf==NULL)
                {
	          printf(CAI_ERREUR,"Erreur completion multiscope");
                  return(CAI_KO);
                }

 		cr_write = write ( num , buf , octwr );
		if ( cr_write != octwr )
		{
			strcpy ( CAI_ERREUR ,
			"Erreur ecriture canal image multiscope\n" );
			goto ERREUR;
		}
                free(buf);
		
 
/********************************************************/
/*		calcul de l'histogramme 		*/
/********************************************************/
		can=(*canal)-1;
		calculer_histo (image1, buff_bloc , taille_wr , can);
/********************************************************/
/*		mise a jour du decalage de 15*16 octets	*/
/********************************************************/

		depla_bl = 15*16;

/********************************************************/
/*	FIN POUR                                        */ 
/********************************************************/
	}

/********************************************************/
/*	mise a jour de la position de la ligne dans le	*/
/*	bloc, et du nombre de blocs precedant la ligne	*/
/********************************************************/

	if ( pos_lig_ds_bloc == 16 )
	{
		pos_lig_ds_bloc = 1;
		nb_blocs_prec_lig++;
	}
	else
		pos_lig_ds_bloc++;

/********************************************************/
/* FIN POUR    						*/
/********************************************************/
}


/********************************************************/
/* SI  on a ecrit le dernier pixel du canal             */
/********************************************************/

if ( image1->NB_PIX_ECRITS[*canal-1] == image1->NB_COL*image1->NB_LIG )
{
/* Ecriture des 0 de completions si necessaires */

  NbL= image1->NB_LIG;
  NbC= image1->NB_COL;
  if ((NbL%16) !=0)
  	NbL = ((NbL/16)+1)*16;

  if ((NbC%16) !=0)
  	NbC = ((NbC/16)+1)*16;

  Nb1 = (NbL*NbC)+1280;
  if (Nb1 !=0)
  {
        position_courante = lseek ( num , 0L , 1 );
        deplacement = Nb1 - position_courante;
        buf = (unsigned char *) calloc(deplacement,sizeof(unsigned char));
        if (buf==NULL)
        {
	   printf(CAI_ERREUR,"Erreur ecriture multiscope");
           return(CAI_KO);
        }

 
   /* ecriture des octets a 0 pour bloc de 16 */
        num=image1->ACCES_CANAL_INT[(*canal)-1];
        cr_write = write ( num , buf , deplacement );

        free(buf);
   }

/********************************************************/
/* 	on ecrit l'histogramme dans le fichier          */
/*	apres positionnement dans le fichier		*/
/********************************************************/

        lseek ( num , 0L , 0 );
        lseek ( num , 256L , 0 );

	/* inversion des octets */
	for ( i=0 ; i<256 ; i++ )
	{
		inverser((char *)&(image1->HISTO[(*canal)-1][i]),
				(sizeof(int)-1));
	}

	cr_write = write ( num , image1->HISTO[(*canal)-1] , 
				(sizeof(int)*256) );
	if ( cr_write != (sizeof(int)*256) )
	{
		strcpy ( CAI_ERREUR ,
			 "Erreur ecriture canal image multiscope\n" );
		goto ERREUR;
	}
}


return (CAI_OK);

ERREUR :

return (CAI_KO);


}

/* 
  Fin de l'operation cai_ecriture_canal_multis
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_multis                                           */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format multiscope et de    */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_multis(image1)                    */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_multis     (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_multis (CAI_IMAGE *image1)

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
     close(num);
    }


  return (iret);
}
/*
  Fin de l'operation cai_ferme_multis
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_multis                                        */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en multiscope et de  */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_multis(format,nom_ima)         */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_multis  (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_dest_ima_multis (char *repertoire , 
                               char *nom_ima )

{
int	i_can;			/* compteur de canaux			    */
long     iret;                  /* code retour fonction fermetrue lecture   */
char	nom_fichier[1024];	/* nom du fichier entete a detruire	    */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image 		    */
char	nom_fic_canal[1024];	/* nom du fichier canal a detruire 	    */
char    chemin[1024];           /* repertoire + "/"			    */
char    *ptr_nom_fic_can;       /* pointeur sur nom fichier canal traite    */
int	cr_stat;		/* code retour de fonction status de fichier*/
struct stat buf_stat;		/* structure decrivant status de fichier    */ 
CAI_IMAGE   *image1;             /* structure ouverture image 		    */
int         nb_can;	        /* nombre de canaux de l'image 		    */
int         nboct_pix;		/* nombre d'octets par pixel image          */
int         nb_col;	        /* nombre de colonnes de l'image	    */
int         nb_lig;             /* nombre de lignes de l'image  	    */

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/
nom_ima_complet[0]='\0';
if ((long int)strlen(repertoire)!= 0)
{
  strcpy ( nom_ima_complet , repertoire );
  strcat ( nom_ima_complet, "/" );
  strcpy ( chemin,nom_ima_complet );
  strcat ( nom_ima_complet , nom_ima );
 chemin[strlen(repertoire)+1] = '\0';
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
  chemin[0]='\0';
}
strcpy(nom_fichier , nom_ima_complet);
strcat ( nom_fichier , ".img" );
nom_fichier[strlen(nom_ima_complet)+4]='\0';


/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* et noms des fichiers fdoxxxxx.per            */
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_multis			*/
/************************************************/
  image1 = cai_ouvre_lecture_multis ( repertoire,nom_ima, &nb_can ,&nboct_pix ,
				      &nb_col, &nb_lig );	
  if ( image1 == NULL) goto ERREUR;

image1->NB_CANAUX=nb_can;
image1->NB_COL=nb_col;
image1->NB_LIG=nb_lig;
image1->NBOCT_PIX=nboct_pix;

ptr_nom_fic_can = image1->NOM_FIC_CAN;

for ( i_can = 0 ; i_can < nb_can ; i_can++ )
{
	strncpy ( nom_fic_canal , ptr_nom_fic_can , 49 );
	nom_fic_canal[49] = '\0';
	strcpy ( nom_fichier , chemin );
	strcat ( nom_fichier, nom_fic_canal );
	nom_fichier[strlen(chemin)+12] = '\0';
	cr_stat = stat ( nom_fichier , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fichier );
	ptr_nom_fic_can = ptr_nom_fic_can + 100;
}

 iret=cai_ferme_multis(image1);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/
nom_fichier[0]='\0';
strcpy(nom_fichier , nom_ima_complet);
strcat ( nom_fichier , ".img" );
nom_fichier[strlen(nom_ima_complet)+4]='\0';

cr_stat = stat ( nom_fichier , &buf_stat );
if ( cr_stat == 0 )
unlink ( nom_fichier );

return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/*
 Fin de l'operation cai_dest_ima_multis
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
/* OPERATION :    inverser                                                   */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    Inversion des octets poids forts,poids faibles                  */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      void inverser((char *)s,nboct)                            */
/* -----------------                                                         */
/*                                                                           */
/*     s              (E/S) (char *) : chaine a inverser                     */
/*     nboct          (E)   (int)    : longueur de la chaine                 */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

static void inverser ( char *s , int nboct )
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
/* Fin de l'operation inverser */

/*****************************************************************************/
/*                                                                           */
/* OPERATION :   calculer_histo                                              */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    Inversion des octets poids forts,poids faibles                  */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      void calculer_histo(s,nboct)                      */
/* -----------------                                                         */
/*                                                                           */
/*     s              (E/S) (char *) : chaine a inverser                     */
/*     nboct          (E)   (int)    : longueur de la chaine                 */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

static void calculer_histo (CAI_IMAGE *image1, unsigned char *buffer , 
						int nb_pix , int canal )
{
   int i_pix;     /* indice de boucle sur les pixels   */
   int val_pix;   /* valeur du pixel traite	       */

   for ( i_pix=0 ; i_pix<nb_pix ; i_pix++ )
   {
      val_pix = (int) buffer[i_pix];
      image1->HISTO[canal][val_pix]++;
   }
}
/*
 Fin de l'operation calculer_histo    
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_multis                                   */
/* ----------								     */
/*                                                                           */
/* ROLE : fonction d'ouverture en lecture-ecriture d'une image au format     */
/* -----     multiscope et de tous les fichiers qui la composent             */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_multis (nom_image,       */
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
/*     cai_ouvre_modifie_multis  (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_modifie_multis(char *repert,
				    char *nom_image,
				    int  *nb_can,
				    int  *nboct_pix,
				    int  *nb_col,
				    int  *nb_lig)


{
   FILE *fic;			/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];	/* nom du fichier canal			    */
   char nom_util_canal[1024];	/* nom du fichier utilisateur		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int num,retour,lon;	        /*  variables de travail                    */
   short int type,fin;          /* variables de trvail			    */

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
 
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   *nb_can=0; 
   iret=1;
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      strcat(nom_fic,".img");
      nom_fic[strlen(repert)+1+strlen(nom_image)+4]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".img");
      nom_fic[strlen(nom_image)+4]='\0';
   }
   
/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic=fopen(nom_fic,"rb");
   if (fic == NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete MULTISCOPE");
    }
   else
   {
     
/*....................................
          positionnement sur le nombre de colonnes
*/
	  fseek(fic,8,0);
/*
	  recuperation du nombre de colonnes */
	  fread(nb_col,4,1,fic);
/*
	  inversion des octets pour compatibilite SUN */
	  lon=sizeof(*nb_col)-1;
	  inverser((char *)nb_col,lon);

/*....................................
	  recuperation du nombre de lignes
*/
	  fread(nb_lig,4,1,fic);
/*
	  inversion des octets pour compatibilite SUN */
	  lon=sizeof(*nb_lig)-1;
	  inverser((char *)nb_lig,lon);

/*......................................
	  initialisation du nombre d'octets par pixel
*/
	  *nboct_pix=1;
          image1->NB_BITS_PIX=8;
	  sprintf(image1->COD_PIX,"OCT");
/*.................................
	  positionnement sur le premier objet
*/
	  fseek(fic,114,1);
	  retour=0;
          deb = image1->NOM_FIC_CAN;
          deb1= image1->NOM_UTIL_CAN;


	  while (retour == 0)
	  {
/*...........Recuperation de tous les objets jusqu'a l'eof
	     nom fichier,nom utilisateur et type de l'objet(avec inversion)
*/
	     fread(nom_fic_canal,14,1,fic);
	     fread(nom_util_canal,40,1,fic);
	     fread(&type,2,1,fic);
	     lon=sizeof(type)-1;
	     inverser((char *)&type,lon);
	     if (type == 3)
	      {
/*.......................................
                 Initialisation des tableaux contenant le nom de fichier
                 et le nom utilisateur de chaque canal
  .....................................................*/


          	if (*nb_can<CAI_NB_MAX_CAN)
          	{
		  nom_fic_canal[0] = tolower(nom_fic_canal[0]);
		  nom_fic_canal[1] = tolower(nom_fic_canal[1]);
		  nom_fic_canal[2] = tolower(nom_fic_canal[2]);
		  nom_fic_canal[9] = tolower(nom_fic_canal[9]);
		  nom_fic_canal[10] = tolower(nom_fic_canal[10]);
		  nom_fic_canal[11] = tolower(nom_fic_canal[11]);
  	          nom_fic[0]='\0';
                  if ((long int)strlen(repert) != 0)
                  {
                    sscanf(repert,"%s",nom_fic);
                    strcat(nom_fic,"/");
                    strcat(nom_fic,nom_fic_canal);
                    nom_fic[strlen(repert)+1+strlen(nom_fic_canal)]='\0';
       		   }
       		  else
       		  {
                     sscanf(nom_fic_canal,"%s",nom_fic);
                     nom_fic[strlen(nom_fic_canal)]='\0';
    		  }
        	  sscanf(nom_fic_canal,"%s",deb);
	          deb=deb+100;
        	  sscanf(nom_util_canal,"%s",deb1);
	          deb1=deb1+100;

                  num=open(nom_fic,O_RDWR,0);
                  if (num==-1)
                  {
                    iret=0;
                    strcpy(CAI_ERREUR,"Erreur ouverture fi canal Multiscope");
       		   }
       		  else
       		  {
                    image1->ACCES_CANAL_INT[*nb_can]=num;
		    image1->POS_OCTET[*nb_can]=1280;
	            image1->NB_PIX_ECRITS[*nb_can]=0;
                  }

        	}
	        *nb_can=*nb_can+1;
	      }

/*............Positionnement en fin de bloc */
	      fseek(fic,70,1);
/*
  ............Test fin de fichier */
	      fread(&fin,2,1,fic);
	      retour=feof(fic);
	  }      
/*..................................
          Fermeture du fichier 
  ......................................*/
          fclose(fic);
       }

 
   if (iret != 0)
      return (image1);
    else
     return(NULL);

}
/* 
   Fin de l'operation cai_ouvre_modifie_multis
*/
