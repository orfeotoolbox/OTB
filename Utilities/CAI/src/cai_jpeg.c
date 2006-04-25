/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_jpeg.c                                                    */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format jpeg                                       */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    Octobre 1998                                        */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V4.0                                                            */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION :2 : DM :optimisation                                       */
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 :FA:Verification que le nombre d'octets par pixel est */
/* egal a 1 et le nombre de canaux est 1 ou 3. jpeg est un format de         */
/* restitution							             */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : modifiation fermeture avant decompression      */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>

#define _CAI_IMAGE
#define IN_CAI_JPEG

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_jpeg.c,v 1.4 01/05/31 10:47:32 cmf Exp $";


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
/* OPERATION :    cai_liste_ima_jpeg                                         */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format jpeg                   */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_jpeg(repert,tab_ima)            */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_jpeg       (S) (int)     : = -1 repertoire inexistant   */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


long int cai_liste_ima_jpeg (char *repert,
		               char ***tab_ima)


{

   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */ 
   int nb_ima;          /* nombre d'images jpeg identifiees             */
   long int i;          /* indice de boucle                            */
   long int ll,dif;     /* variables de travail                        */
   char blanc;          /* caractere blanc                             */


/*........................................
   INITIALISATION et ouverture du repertoire choisi
  .........................................*/
   nb_ima=0;
   dirp=opendir(repert);
   blanc = ' ';

   if(dirp!=NULL)
   {


/*......................................

   premiere passe pour comptabiliser les images du format donne
   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE JPEG
      (EXTENSION =.jpg)
   ..........................................*/
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".jpg");
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
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT JPEG
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE JPEG
      (EXTENSION =.jpeg)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".jpg");
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

    (void)closedir(dirp);


   }
   else
   {
    /* repertoire inexistant */
     nb_ima=-1;
     }
    return(nb_ima);

}

/* Fin de l'operation cai_liste_ima_jpeg
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_jpeg                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format jpeg      */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_lecture_jpeg (nom_image,      */
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
/*     cai_ouvre_lecture_jpeg     (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_jpeg(char *repert,
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
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour;			/* retour de la fonction comparaison chaines*/ 
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int i,num;			/* indice 				    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   char COD_PIX[80];	        /* type de codage du pixel (optionnel)      */
   struct jpeg_decompress_struct *cinfo;
   static struct jpeg_error_mgr jerr;


   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   cinfo = (struct jpeg_decompress_struct *)calloc(1,
				sizeof(struct jpeg_decompress_struct));
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if ((image1==NULL)||(cinfo==NULL))
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
   if((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.jpg",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.jpg",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"rb");
   if (fic_in==NULL)
   {
     iret=0;
     sprintf(CAI_ERREUR,"Erreur ouverture fichier entete JPEG %s",nom_fic);
     free(cinfo);
     cinfo = NULL;
     image1->ACCES_CANAL_JPEG_LEC=cinfo;
    }
   else
   {
 	cinfo->err=jpeg_std_error(&jerr);
 	jpeg_create_decompress(cinfo);

 	jpeg_stdio_src(cinfo,fic_in);
        (void) jpeg_read_header(cinfo,TRUE);
	jpeg_start_decompress(cinfo);

        *nb_can = cinfo->num_components;
        *nb_lig = cinfo->image_height;
        *nb_col = cinfo->image_width;
        *nboct_pix = 1;
	sprintf(image1->COD_PIX,"OCT");
        image1->ACCES_CANAL_JPEG_LEC=cinfo;
        image1->ACCES_FILE_JPEG=fic_in;
        image1->POS_OCTET[0]=CAI_JPEG_LECTURE;

    }

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

    if((long int)strlen(repert)!=0)
    {
	sprintf(nom_fic_canal,"%s/%s.jpg",repert,nom_image);
    }
    else
    {
 	sprintf(nom_fic_canal,"%s.jpg",nom_image);
    }
  
    deb = image1->NOM_FIC_CAN;
    deb1= image1->NOM_UTIL_CAN;
    for (i=0;i<*nb_can;i=i+1)
    {
           sscanf(nom_image,"%s",deb);
  	   deb=deb+100;
           sscanf(nom_image,"%s",deb1);
  	   deb1=deb1+100;
           image1->POS_OCTET[i]=0;
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
   Fin de l'operation cai_ouvre_lecture_jpeg
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_jpeg                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format jpeg et de tous les fichiers qui la composent         */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    CAI_IMAGE *cai_ouvre_creation_jpeg (nom_image,      */
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
/*     cai_ouvre_creation_jpeg     (S) (CAI_IMAGE *) : =NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_jpeg(char *repert,
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
   int i,nboctet;		/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   struct jpeg_compress_struct *cinfo;
   static struct jpeg_error_mgr jerr;
/*****************************************************************************/
/* MOD : VERSION : 4.2 :FA:Verification que le nombre d'octets par pixel est */
/* egal a 1 et le nombre de canaux est 1 ou 3. jpeg est un format de         */
/* restitution							             */
/*****************************************************************************/
/* Verification sur le nombre d'octets par pixel */

   if (*nboct_pix !=1)
   {
     strcpy(CAI_ERREUR,"Erreur une image jpeg ne peut etre codee que sur un seul octet");
     goto ERREUR;
   }

/* Verification sur le nombre de canaux */
   if ((*nb_can !=1)&&(*nb_can !=3))
   {
     strcpy(CAI_ERREUR,"Erreur une image jpeg a 1 ou 3 canaux exclusivement");
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
   Constitution du nom du fichier  et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if((long int)strlen(repert)!=0)
   {
	sprintf(nom_fic,"%s/%s.jpg",repert,nom_image);
   }
   else
   {
	sprintf(nom_fic,"%s.jpg",nom_image);
    }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s JPEG existe \n" , nom_image );
	goto ERREUR;
   }

/* allocation du tableau contenant l'image entiere */
   nboctet = (*nb_col)*(*nb_lig)*(*nb_can);
   image1->image_JPEG = (JSAMPLE *) calloc(nboctet,sizeof(JSAMPLE));
   if (image1->image_JPEG==NULL)
   {
	sprintf(CAI_ERREUR,"Erreur allocation memoire image JPEG");
	goto ERREUR;
   }

/*...............................................................
   Ouverture du fichier en ecriture avec allocation de la structure
  .................................................................*/
   cinfo = (struct jpeg_compress_struct *)calloc(1,
				sizeof(struct jpeg_compress_struct));
   if (cinfo==NULL)
   {
      strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_creation");
      goto ERREUR;
   }

   cinfo->err=jpeg_std_error(&jerr);
/* Now we can initialize the JPEG compression object. */
   jpeg_create_compress(cinfo);

   fic = fopen(nom_fic,"wb"); 
   if (fic == NULL)
   {
      sprintf(CAI_ERREUR,"Erreur: Creation fichier %s impossible\n",nom_fic);
      goto ERREUR;
   }
   image1->ACCES_FILE_JPEG = fic;;

   jpeg_stdio_dest(cinfo, fic);

  /*  set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
   cinfo->image_width = *nb_col; 	
   cinfo->image_height = *nb_lig;
   cinfo->input_components = *nb_can;	/* # of color components per pixel */
   if (*nb_can==1)
     cinfo->in_color_space = JCS_GRAYSCALE;
   else
     cinfo->in_color_space = JCS_RGB; /* colorspace of input image */

  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
   jpeg_set_defaults(cinfo);

  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
   jpeg_set_quality(cinfo, 90, TRUE /* limit to baseline-JPEG values */);

  /* Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
   jpeg_start_compress(cinfo, TRUE);

   image1->ACCES_CANAL_JPEG_ECR = cinfo;

   image1->POS_OCTET[0]=CAI_JPEG_ECRITURE;
   image1->NBOCT_PIX = 1;

   return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_jpeg
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_jpeg                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image jpeg                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_jpeg(image1, canal,       */       		      /* ------------------                            premiere_ligne,             */
/*                                               premiere_colonne,           */
/*                                               nombre_lignes_lire ,        */
/*                                               nombre_colonnes_lire,       */
/*                                               pas_ligne ,pas_colonne,     */
/*                 		                 data_image )                */
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
/*     cai_lecture_canal_jpeg     (S) (CAI_OK_KO) : = CAI_KO si pb           */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_lecture_canal_jpeg(CAI_IMAGE *image1 ,  
                                int *canal ,           
    		          	int *premiere_ligne  ,   
     		          	int *premiere_colonne,
                         	int *nombre_lignes_lire ,
                          	int *nombre_colonnes_lire,
                          	int *pas_ligne ,          
                          	int *pas_colonne,
		          	unsigned char *data_image )        
{
 JSAMPARRAY buffer;
 struct jpeg_decompress_struct *cinfo;
 static struct jpeg_error_mgr jerr;
 int i,j,k,n;               /* variables de travail                          */
 int pt_oct;		    /* position courante dans le fichier image       */
 int no_oct;                /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int   oct_pix;             /* nb octets par pixels                          */
 long  nb_oct;              /* nb oct =1er octet a lire - position courante  */
 int  oct_lig;             /* nb oct. par ligne en considerant taille pixel */
 int  nbcan;               /* nombre de canaux de l'image                   */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 FILE *infile;               /* acces au fichier jpeg                        */


/*...........................
  Initialisations diverses
  ..........................*/

   iret=CAI_OK;
   cinfo=image1->ACCES_CANAL_JPEG_LEC;
   oct_pix=image1->NBOCT_PIX;
   nbcan = image1->NB_CANAUX;
   oct_lig=nbcan*(image1->NB_COL);

if ((*premiere_ligne)-1 != cinfo->output_scanline)
{
     cinfo->output_scanline = cinfo->image_height;
     jpeg_finish_decompress(cinfo);
     jpeg_destroy_decompress(cinfo);

   infile = image1->ACCES_FILE_JPEG;
   rewind(infile);
   cinfo->err=jpeg_std_error(&jerr);
   jpeg_create_decompress(cinfo);
   jpeg_stdio_src(cinfo,infile);
   (void) jpeg_read_header(cinfo,TRUE);
   jpeg_start_decompress(cinfo); 
}
/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
   buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr)cinfo,
			JPOOL_IMAGE,oct_lig,1);
   if (buffer == NULL)
   {
      iret=CAI_KO;
      strcpy(CAI_ERREUR,"Probleme allocation memoire JPEG");
   }
   else
   {
if ((*premiere_ligne)-1 != cinfo->output_scanline)
{

 
/* positionnement jusqu'a la premiere ligne a lire */
      for (lig=0;lig <(*premiere_ligne)-1;lig++)
      {
     		(void) jpeg_read_scanlines(cinfo,buffer,1);
                
      }
}
/* recuperation des lignes demandees */
       lig=(*premiere_ligne)-1;
       i=1;
       pch = data_image;

       while ((i <= (*nombre_lignes_lire))&&(iret==CAI_OK))
       {  
    	  cinfo->output_scanline = lig;
    	  n = jpeg_read_scanlines(cinfo,buffer,1);             
          if (n<=0)
          {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image JPEG");
          }
          else
          { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/
               deb = *buffer;  
               deb=deb+((*premiere_colonne)-1)*nbcan+(*canal-1);
               j=1;
               while (j <= (*nombre_colonnes_lire))
	       {  

                  *pch=*deb;
                  pch=pch+1;
                  j=j+1;
                  deb=deb+((*pas_colonne)*oct_pix*nbcan);
                }   
          
                i=i+1;
                lig=lig+1;
                j = 1;
		while ((j < (*pas_ligne))&&(iret==CAI_OK)
				&&(lig+1<(image1->NB_LIG)))
                {
    	           cinfo->output_scanline = lig;
    	           n = jpeg_read_scanlines(cinfo,buffer,1);
                
                   if (n<=0)
                   {
                     iret=CAI_KO;
                     strcpy(CAI_ERREUR,"Erreur lecture donnee image JPEG");
                    }
                    else
			lig=lig+1;
		    j = j+1;
                }
          } 
        }        
   }

   return(iret);
 }
/*
  Fin de l'operation cai_lecture_canal_jpeg
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_jpeg                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image jpeg                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      CAI_OK_KO cai_ecriture_canal_jpeg(image1, canal,  */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_jpeg        (S) (CAI_OK_KO) : = CAI_KO si pb       */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_jpeg (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image )
{
int	i_ligne;		/* compteur de lignes                    */
int num;                        /* descripteur de fichier                */
int nboct_pix;			/* nombre d'octets par pixel             */
int nbcan;                      /* nombre de canaux de l'image           */
int nboctet;                    /* nombre d'octets d'une ligne multi     */
int ires,ideb,col,indk;		/* indice 			         */

/* initialisations */
i_ligne = 0;
nboct_pix = image1->NBOCT_PIX;
nbcan = image1->NB_CANAUX;
nboctet = (*nombre_colonnes_ecrire)*nbcan*(nboct_pix);


/********************************************************/
/* TANT QUE il y a des lignes a ecrire                 	*/
/********************************************************/
ires = ( *premiere_ligne - 1) *nboctet+(*canal-1);
ideb = 0;
while (i_ligne < (*nombre_lignes_ecrire))
{

/*************************************************************************/
/*   chargement de la ligne entrelacant les pixels par rapport aux canaux*/
/*************************************************************************/
   for (col=0;col<(*nombre_colonnes_ecrire);col++)
   {
        for (indk=0;indk<nboct_pix;indk++)
        {
	   image1->image_JPEG[ires] = data_image[ideb+indk];
         }
	ires = ires + (nboct_pix*nbcan);
	ideb = ideb + nboct_pix;
   }
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
  Fin de l'operation cai_ecriture_canal_jpeg
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_jpeg                                             */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format jpeg et de          */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_jpeg(image1)                      */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_jpeg        (S) (CAI_OK_KO) :   = CAI_KO si pb              */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_jpeg (CAI_IMAGE *image1)


{
 int iret;     			        /* code retour de l'operation      */
 FILE *infile; 			        /* descripteur du fichier a fermer */
 struct jpeg_decompress_struct *cinfoL; /* structure du fichier a fermer   */
 struct jpeg_compress_struct *cinfoE;   /* structure du fichier a fermer   */
 JSAMPROW row_pointer[1];	        /* pointer to JSAMPLE row[s]       */
 int nboctet;			        /* nb d'octets par ligne           */

/* initialisations */
/*.................*/
  iret=CAI_OK;

if (image1 !=NULL)
{
  nboctet = (image1->NB_COL)*(image1->NB_CANAUX);
/*.............................................
   Fermeture differente si lecture ou ecriture d'images
  .............................................*/
   if (image1->POS_OCTET[0]==CAI_JPEG_LECTURE)
   {
/* Fermeture d'image simple pour une lecture avec liberation des */
/* espaces memoires					         */
     cinfoL = image1->ACCES_CANAL_JPEG_LEC;
     if (cinfoL != NULL)
     {
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : modifiation fermeture avant decompression      */
/*****************************************************************************/
	cinfoL->output_scanline=image1->NB_LIG;

       jpeg_finish_decompress(cinfoL);
       jpeg_destroy_decompress(cinfoL);
       free(cinfoL);
       cinfoL = NULL; 
     }    
   }
   else
   {
/* cas ecriture d'une image au format JPEG : avant de fermer l'image */
/* ecriture compressee du tableau conteant l'image                   */
     cinfoE = image1->ACCES_CANAL_JPEG_ECR;
     if (cinfoE != NULL)
     {
    
         while (cinfoE->next_scanline < cinfoE->image_height)
         {
            /* jpeg_write_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could pass
             * more than one scanline at a time if that's more convenient.
            */
            row_pointer[0] =
			& (image1->image_JPEG[cinfoE->next_scanline * nboctet]);
            (void) jpeg_write_scanlines(cinfoE, row_pointer, 1);
         }
        /* Finish compression */

        jpeg_finish_compress(cinfoE);

       /* After finish_compress, we can close the output file. */

       /*  release JPEG compression object */

       /* This is an important step since it will release a good */
       /* deal of memory. 				         */
       jpeg_destroy_compress(cinfoE);
       free(cinfoE);
       cinfoL = NULL; 
     }
     if (image1->image_JPEG != NULL)
     {
	free(image1->image_JPEG);
        image1->image_JPEG = NULL;
     }
   } 

   infile=image1->ACCES_FILE_JPEG;
   if (infile!=NULL) fclose(infile);
}
  return (iret);
}

/*
  Fin de l'operation cai_ferme_jpeg
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_jpeg                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en jpeg et de        */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_jpeg(repertoire,nom_ima)       */       		      /* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_jpeg     (S) (CAI_OK_KO) : = CAI_KO si pb                */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_dest_ima_jpeg ( char *repertoire , 
                     	     char *nom_ima )

{
char    nom_ima_complet[1024];  /* repertoire + nom de l'image */

int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/

if ((long)strlen(repertoire)!= 0)
{
  sprintf(nom_ima_complet,"%s/%s.jpg",repertoire,nom_ima);
}
else
{
  sprintf(nom_ima_complet,"./%s.jpg",nom_ima);
}

/********************************/
/* Destruction du fichier 	*/
/********************************/

cr_stat = stat ( nom_ima_complet , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_ima_complet );

return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_jpeg
*/    
