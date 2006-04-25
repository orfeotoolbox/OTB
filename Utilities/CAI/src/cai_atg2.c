/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_atg2.c                                                    */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format atg2                                       */ 
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
static char *rcs_id="$Header: cai_atg2.c,v 1.5 98/10/19 10:29:06 cmf Exp $";


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
/* OPERATION :    cai_liste_ima_atg2                                         */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format atg2                   */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_atg2(repert,tab_ima)            */
/* -----------------                                                         */
/*                                                                           */
/*     repert        (E) (char *)   : repertoire a explorer		     */
/*     tab_ima       (S) (char ***) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_atg2   (S) (int)     : = -1 repertoire inexistant       */                                    /*					    = > 0 nombre d'images listees    */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

int cai_liste_ima_atg2(repert,
		       tab_ima)

char *repert;
char ***tab_ima;

{
  DIR *dirp;		     /* pointeur sur structure ouverture repertoire */

struct dirent *dp;   /* structure resultat de la fonction readdir   */
struct dirent *cr_readdir; /* structure resultat de la fonction readdir   */

  int nb_ima;         	     /* nombre d'images atg2 identifiees            */
  DIR *ptr_sub_dir;          /* pointeur sur struct. ouverture repert. ima  */
  char *str_sub_dir;         /* repertoire image atg2 a ouvrir              */
  int cr_stat;               /* compte-rendu de la fonction stat            */
  struct stat buf_stat;      /* structure retour de la fonction stat        */

  int i;                     /* indice de boucle                            */

   


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
      TEST POUR SAVOIR SI CE FICHIER EST UN REPERTOIRE
      SI C'EST  LE CAS : 
        OUVERTURE DU REPERTOIRE
	VERIFICATION QU'IL CONTIENT UN FICHIER D'EXTENSION .gen 
    ..........................................*/

   while((dp=readdir(dirp))!=NULL)
   {

      /* ne se traite pas lui-meme ni le repertoire pere */
      if ((strcmp(dp->d_name,".")==0) || (strcmp(dp->d_name,"..")==0))
         continue;

      /* construction du nom complet de l'objet */
      str_sub_dir = (char *) malloc(strlen(repert)+1+strlen(dp->d_name)+1);
      if (str_sub_dir == NULL)
      {
        /* Erreur : probleme d'allocation memoire */
          nb_ima=-3;
          return(nb_ima);
      }

      strcpy(str_sub_dir,repert);
      strcat(str_sub_dir,"/");
      strcat(str_sub_dir,dp->d_name);
      str_sub_dir[strlen(repert)+1+strlen(dp->d_name)]='\0';

      /* on verifie que l'objet est un repertoire */
       cr_stat = stat(str_sub_dir,&buf_stat);

       if  ((cr_stat == -1) || ((buf_stat.st_mode & S_IFMT) != S_IFDIR))
       {

        /* acces impossible a l'objet ou l'objet n'est pas un repertoire */
          free(str_sub_dir);
          continue;
       }

       /* on verifie que l'objet est un repertoire ATG2 */
       /* c'est a dire contenant un fichier ".gen"      */

       ptr_sub_dir = opendir(str_sub_dir);
       if(ptr_sub_dir!=NULL)
       {   
         while ((cr_readdir=readdir(ptr_sub_dir)) != NULL)
         {

          if (strcmp(".gen",cr_readdir->d_name) == 0)
          {
             /* c'est un repertoire ATG2 */
	      nb_ima=nb_ima+1;
          }
         }
         closedir(ptr_sub_dir);
        }
        else
        {

        /* acces impossible a l'objet  */
          free(str_sub_dir);
          continue;

        }

       free(str_sub_dir);
   }
   closedir(dirp);

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT ATG2
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne
   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UN REPERTOIRE
      SI C'EST  LE CAS : 
        OUVERTURE DU REPERTOIRE
	VERIFICATION QU'IL CONTIENT UN FICHIER D'EXTENSION .gen 
        SI C'EST LE CAS STOCKAGE DE L'IMAGE DANS LA LISTE DES IMAGES ATG2 
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
      /* ne se traite pas lui-meme ni le repertoire pere */
      if ((strcmp(dp->d_name,".")==0) || (strcmp(dp->d_name,"..")==0))
         continue;

      /* construction du nom complet de l'objet */
      str_sub_dir = (char *) malloc(strlen(repert)+1+strlen(dp->d_name)+1);
      if (str_sub_dir == NULL)
      {
        /* Erreur : probleme d'allocation memoire */
          nb_ima=-3;
          return(nb_ima);
      }

      strcpy(str_sub_dir,repert);
      strcat(str_sub_dir,"/");
      strcat(str_sub_dir,dp->d_name);
      str_sub_dir[strlen(repert)+1+strlen(dp->d_name)]='\0';

      /* on verifie que l'objet est un repertoire */
       cr_stat = stat(str_sub_dir,&buf_stat);
       if  ((cr_stat == -1) || ((buf_stat.st_mode & S_IFMT) != S_IFDIR))
       {

        /* acces impossible a l'objet ou l'objet n'est pas un repertoire */
          free(str_sub_dir);
          continue;
       }

       /* on verifie que l'objet est un repertoire ATG2 */
       /* c'est a dire contenant un fichier ".gen"      */

       ptr_sub_dir = opendir(str_sub_dir);
       if(ptr_sub_dir!=NULL)
       {  
         while ((cr_readdir=readdir(ptr_sub_dir)) != NULL)
         {
          if (strcmp(".gen",cr_readdir->d_name) == 0)
          {
             /* c'est un repertoire ATG2 */
              strcpy((*tab_ima)[nb_ima],dp->d_name);
	      nb_ima=nb_ima+1;
          }
         }
         closedir(ptr_sub_dir);

        }
        else
        {

        /* acces impossible a l'objet  */
          free(str_sub_dir);
          continue;

        }

       free(str_sub_dir);
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
/* Fin de l'operation cai_liste_ima_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_atg2                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format atg2      */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_atg2(nom_image,          */
/*  ---------------- 		                         nb_can,             */
/*				                         nboct_pix,	     */	
/*				                         nb_col,             */
/*				                         nb_lig)             */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_atg2  (S) (CAI_IMAGE *)    : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


typedef struct {
   char   rty[80];
   char   rid[80];
   char   prt[8];
   char   nam[80];
   int    str;
   double lod;
   double lad;
   char   uniloa[8];
   char   mbu[8];
   int    cov;
   int    fec;
   int    poc;
   int    lic;
   int    alc;
   int    sgc;
   int    zna;
   double psp;
   char   imr[4];
   int    arv;
   int    brv;
   double lso;
   double pso;
   char   txt[80];
   int    nul;
   int    nus;
   int    nll;
   int    nls;
   int    nfl;
   int    nfc;
   int    pnc;
   int    pnl;
   int    cod;
   int    rod;
   int    por;
   int    pcb;
   int    pvb;
   char   bad[80];
   char   tif[2];
   char   fmt[8];
   char   pty[2];
   int    ofs;
   double mpl;
   double mpc;
   int    rol[100];
   int    roc[100];
} Fsro_format_data;


CAI_IMAGE *cai_ouvre_lecture_atg2(repert,
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
   int fic;			/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[2];			/* indce du canal traite     		    */
   int num,k,n;		        /* indice 				    */
   Fsro_format_data fsro;       /* structure decrivant l'entete atg2        */

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
 if (strlen(repert)!= 0)
 {
   sscanf(repert,"%s",nom_fic);
   strcat(nom_fic,"/");
   strcat(nom_fic,nom_image);
   strcat(nom_fic,"/");
   strcat(nom_fic,".gen");
   nom_fic[strlen(repert)+1+strlen(nom_image)+5]='\0';
  }
 else
 {
   sscanf(nom_image,"%s",nom_fic);
   strcat(nom_fic,"/");
   strcat(nom_fic,".gen");
   nom_fic[strlen(nom_image)+5]='\0';
  }
/*........................
   Ouverture du fichier entete
  ..........................................*/
   num=open(nom_fic,O_RDONLY,0);

   if (num == -1)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete ATG2");
    }
   else
   {
     
/*....................................
       Lecture du fichier entete
  ............................................*/

          n=read( num , &fsro , sizeof(Fsro_format_data));
	  if ( n <= 0 )
	  {
           sprintf(CAI_ERREUR,"Erreur lecture entete ATG2 %s\n",nom_fic),
	   iret=0;
	   return (NULL);
	  }

/*.........................................
	  Fermeture du fichier entete ATG2
  ..........................................*/
	  close ( num );

/*.........................................
          Recuperation du nombre d'octets par pixel .
  ....................................................*/
	  if (fsro.pvb>16)
  	  {
	      iret=0;
	      sprintf(CAI_ERREUR,"Taille du pixel non traitable %d\n",fsro.pvb);
	      return (NULL);
	  }
	  else
	  {
            if ((fsro.pvb%8)==0)
	       *nboct_pix=fsro.pvb/8;
            else
	       *nboct_pix=fsro.pvb/8+1;
            image1->NB_BITS_PIX = fsro.pvb;
	  }

/*.........................................
          Recuperation de la taille d'un pave en lignes
  ....................................................*/
	  image1->LIGNES_PAVE=fsro.pnl;
/*.........................................
          Recuperation de la taille d'un pave en colonnes
  ....................................................*/
	  image1->COLONNES_PAVE=fsro.pnc;

/*.........................................
          Calcul du nombre de lignes : taille du pave*nombre de pave en ligne
  ....................................................*/
	  *nb_lig=fsro.nfl*image1->LIGNES_PAVE;
/*.........................................
          Calcul du nombre de colonnes: taille  pave*nombre  pave en colonnes
  ....................................................*/
	  *nb_col=fsro.nfc*image1->COLONNES_PAVE;
/*.........................................................
         Recuperation du nombre de canaux en fonction du nom des fichiers .gds
	 1 - test sur l'existence du fichier PAN.gds monochrome
  ...........................................................*/
        nom_fic[0]='\0';
       if (strlen(repert)!= 0)
       {
          sscanf(repert,"%s",nom_fic);
          strcat(nom_fic,"/");
          strcat(nom_fic,nom_image);
          strcat(nom_fic,"/PAN.gds");
          nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
        }
        else
        {
          sscanf(nom_image,"%s",nom_fic);
          strcat(nom_fic,"/PAN.gds");
          nom_fic[strlen(nom_image)+8]='\0';
        }
        num=open(nom_fic,O_RDONLY,0);
        if (num<=0)
	{

/*	  2 - test sur l'existence du fichier XS1.gds monochrome
  ...........................................................*/

          nom_fic[0]='\0';
          if (strlen(repert)!= 0)
          {
            sscanf(repert,"%s",nom_fic);
            strcat(nom_fic,"/");
            strcat(nom_fic,nom_image);
            strcat(nom_fic,"/XS1.gds");
            nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
          }
          else
          {
            sscanf(nom_image,"%s",nom_fic);
            strcat(nom_fic,"/XS1.gds");
            nom_fic[strlen(nom_image)+8]='\0';
          }
 
          fic=open(nom_fic,O_RDONLY,0);
          if (fic<=0)
	  {
	     iret=0;
	     sprintf(CAI_ERREUR,"Erreur nombre de canaux incorrect ATG2\n");
	     return (NULL);
	  }
	  else
          {   
	     close(fic);
	     *nb_can=3;
           }
         }
        else
        {
	  close(num);
	  *nb_can=1;
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

     if (*nb_can==1)
     {
	 sscanf(nom_image,"%s",deb);
	 strcat(deb,"/PAN.gds");
	 sscanf(nom_image,"%s",deb1);
	 strcat(deb1,"/PAN.gds");
         nom_fic[0]='\0';
         if (strlen(repert)!=0)
         {
          sscanf(repert,"%s",nom_fic);
          strcat(nom_fic,"/");
          strcat(nom_fic,nom_image);
          strcat(nom_fic,"/PAN");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
         }
         else
         {
          sscanf(nom_image,"%s",nom_fic);
          strcat(nom_fic,"/PAN");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(nom_image)+8]='\0';
         }
         fic=open(nom_fic,O_RDONLY,0);
         image1->ACCES_CANAL_INT[0]=fic;
         image1->POS_OCTET[0]=0;

        
     }
     else
     {

       k=1;    
       while((k<=*nb_can)&&(k<CAI_NB_MAX_CAN))
       {
          sprintf(car,"%d",k);
          sscanf(nom_image,"%s",deb);
          strcat(deb,"/XS");
          strcat(deb,car);
          strcat(deb,".gds");
          deb=deb+100;
          sscanf(nom_image,"%s",deb1);
          strcat(deb1,".XS");
          strcat(deb1,car);
          strcat(deb1,".gds");
          deb1=deb1+100;
          nom_fic[0]='\0';
         if (strlen(repert)!=0)
         {
          sscanf(repert,"%s",nom_fic);
          strcat(nom_fic,"/");
          strcat(nom_fic,nom_image);
          strcat(nom_fic,"/XS");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
         }
         else
         {
          sscanf(nom_image,"%s",nom_fic);
          strcat(nom_fic,"/XS");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(nom_image)+8]='\0';
         }
          fic=open(nom_fic,O_RDONLY,0);
          if (fic != -1) 
          {
           image1->ACCES_CANAL_INT[k-1]=fic;
           image1->POS_OCTET[k-1]=0;
           k=k+1;
          }
	  else
          {
	    iret=0;
            strcpy(CAI_ERREUR,"incoherence au niveau du nb fichiers canaux");
	    return(NULL);
          }
       }
     }

     return (image1);
   }
   else
   {
     return(NULL);
    }

}

/* 
   Fin de l'operation cai_ouvre_lecture_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_atg2                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format atg2 et de tous les fichiers qui la composent         */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_atg2 (nom_image,         */
/*  ---------------- 		                          nb_can,            */
/*				                          nboct_pix,	     */
/*				                          nb_col,            */
/*				                          nb_lig,            */
/*				                          label)             */
/*                                                                           */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_atg2    (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_atg2(repert,
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
   int fic;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char repertoire_ima[1024];	/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   int retour;			/* retour de fonction 			    */ 
   char car[3];			/* indice du canal traite     		    */
   int i,num,can;		/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   Fsro_format_data fsro;	/* structure informations format image      */

   char	nom_fic_tim[1024];	/* nom de fichier de travail 		   */
   char	nom_fic_his[1024];	/* nom de fichier de travail		   */
   char	nom_fic_bid[1024];	/* nom de fichier de travail 		   */
   char	**tab_nom_tim;		/* tableau contenant les Tile index Maps   */ 
   char	**tab_nom_his;		/* tableau contenant les histogrammes      */
   char	**tab_nom_bid;		/* tableau contenant les caracteristiques des 
				   plans */
   FILE	*ptr_file_tim;		/* flot de fichier tim 			   */
   FILE	*ptr_file_his;		/* flot de fichier his			   */
   FILE	*ptr_file_bid;		/* flot de fichier bid			   */
   unsigned char *ptr_buff_tim;	/* pointeur sur buffer tim 		   */
   int	offset;			/* offset a ecrire dans fichier tim        */

   char	str_cmd[100];		/* chaine de commande pour appels systeme  */
   int	nb_blocs_ns;		/* nombre de blocs N/S			   */
   int	nb_blocs_we;		/* nombre de blocs W/E 			   */


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
 image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

/*************************************************/
/* Construction du nom de la sous-directory 	 */
/* d'accueil des fichiers			 */
/*************************************************/
 if (strlen(repert)!= 0)
 {
   strcpy ( repertoire_ima , repert );
   strcat ( repertoire_ima , "/" );
   strcat ( repertoire_ima , nom_image );
  repertoire_ima[strlen(repert)+1+strlen(nom_image)]='\0';
  }
 else
 {
   strcpy ( repertoire_ima , nom_image );
   repertoire_ima[strlen(nom_image)]='\0';
 }

/*************************************************/
/* verification que cette sous-directory         */
/* n'existe pas deja				 */
/*************************************************/

retour = stat ( repertoire_ima , &buf_stat );
if (( retour != -1 ) && ( (buf_stat.st_mode & S_IFMT) == S_IFDIR ))
{ 
	sprintf ( CAI_ERREUR , "L'image %s atg2 existe \n" , nom_image );
	goto ERREUR;
 }
 
/*************************************************/
/* Creation de la sous-directory d'accueil des	 */
/* fichiers					 */
/*************************************************/

sprintf(str_cmd,"mkdir %s",repertoire_ima);
system(str_cmd);

/*************************************************/
/* Creation du fichier caracteristiques : .gen	 */
/*************************************************/
nom_fic[0]='\0';
strcpy ( nom_fic, repertoire_ima );
strcat ( nom_fic, "/.gen" );
nom_fic[strlen(repertoire_ima)+5]='\0';
fic = creat ( nom_fic,PERMS);
if ( fic == -1 )
{
   sprintf ( CAI_ERREUR , "Erreur : creation fichier %s impossible\n",nom_fic );
   goto ERREUR;
}


/*************************************************/
/* ecriture de l'enregistrement d'entete	 */
/*************************************************/

memset(&fsro,'\0',sizeof(Fsro_format_data));

/* Record in field */

for (i=0;i<80;i++)
   fsro.rty[i]='\0';
strcpy(fsro.rty,"GIN");
for (i=0;i<80;i++)
   fsro.rid[i]='\0';
strcpy(fsro.rid,"1");

/* Data set id field */

for (i=0;i<8;i++)
   fsro.prt[i]='\0';
strcpy(fsro.prt,"IMAGE");
for (i=0;i<80;i++)
   fsro.nam[i]='\0';
strcpy(fsro.nam,nom_image);

/* General information field */

fsro.str = 3;
fsro.lod = 0.0;
fsro.lad = 0.0;
for (i=0;i<8;i++)
   fsro.uniloa[i]='\0';
strcpy(fsro.uniloa,"");
for (i=0;i<8;i++)
   fsro.mbu[i]='\0';
strcpy(fsro.mbu,"");
fsro.cov = 0;
fsro.fec = 0;
fsro.poc = 0;
fsro.lic = 0;
fsro.alc = 0;
fsro.sgc = 0;
fsro.zna = 0;
fsro.psp = 0.0;
for (i=0;i<4;i++)
   fsro.imr[i]='\0';
strcpy(fsro.imr,"no");
fsro.arv = 0;
fsro.brv = 0;
fsro.lso = 0.0;
fsro.pso = 0.0;
for (i=0;i<80;i++)
fsro.txt[i]='\0';
strcpy(fsro.txt,"");

/* Data set parameters fields */

fsro.nul = 0;
fsro.nus = 0;
fsro.nll = 0;
fsro.nls = 0;
/* nombre de blocs N/S */
if ( (*nb_lig) % 256 == 0 )
        fsro.nfl = (*nb_lig) / 256;
else
        fsro.nfl = (*nb_lig) / 256 + 1;
/* nombre de blocs W/E */
if ( (*nb_col) % 256 == 0 )
        fsro.nfc = (*nb_col) / 256;
else
        fsro.nfc = (*nb_col) / 256 + 1;
/* taille des blocs */
fsro.pnc = 256;
fsro.pnl = 256;
fsro.cod = 0;
fsro.rod = 1;
fsro.por = 0;
fsro.pcb = 0;
fsro.pvb = (*nboct_pix)*8;
for (i=0;i<80;i++)
   fsro.bad[i]='\0';
strcpy(fsro.bad,nom_image);
for (i=0;i<2;i++)
   fsro.tif[i]='\0';
/* nombre de blocs W/S */
strcpy(fsro.tif,"y");

/* Non Digest fiel - Specific ATG2/MULTISCOPE */

for (i=0;i<8;i++)
   fsro.fmt[i]='\0';
strcpy(fsro.fmt,"gds");
for (i=0;i<2;i++)
   fsro.pty[i]='\0';
strcpy(fsro.pty,"S ");
fsro.ofs = 0;
fsro.mpl = 0.0;
fsro.mpc = 0.0;
for (i=0;i<100;i++)
   fsro.rol[i]=0;
for (i=0;i<100;i++)
   fsro.roc[i]=0;
retour = write ( fic , &fsro , sizeof(Fsro_format_data));
if ( retour != sizeof(Fsro_format_data) )
{
        sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n",
        nom_fic );
        close(fic);
        unlink(nom_fic);
	goto ERREUR;
}

close ( fic );

/*************************************************/
/* Creation des fichiers :      		 */
/* - Tile index Map				 */
/* - Histo					 */
/* - Caracterisitiques du plan			 */	
/*************************************************/

tab_nom_tim = (char **) malloc ( (*nb_can) * sizeof(char *) );
tab_nom_his = (char **) malloc ( (*nb_can) * sizeof(char *) );
tab_nom_bid = (char **) malloc ( (*nb_can) * sizeof(char *) );

for (can=0; can<*nb_can; can=can+1 )
{
	strcpy ( nom_fic_tim , repertoire_ima );
	strcpy ( nom_fic_his , repertoire_ima );
	strcpy ( nom_fic_bid , repertoire_ima );
	if (*nb_can == 1)
	{
		strcat ( nom_fic_tim , "/PAN.tim" );
		nom_fic_tim[strlen(repertoire_ima)+8]='\0';
		strcat ( nom_fic_his , "/PAN.his" );
		nom_fic_his[strlen(repertoire_ima)+8]='\0';
		strcat ( nom_fic_bid , "/PAN.bid" );
		nom_fic_bid[strlen(repertoire_ima)+8]='\0';
	}
	else
	{
		strcat ( nom_fic_tim , "/XS" );
		sprintf ( car , "%d" , can+1 );
		strcat ( nom_fic_tim , car );
		strcat ( nom_fic_tim , ".tim" );
		nom_fic_tim[strlen(repertoire_ima)+3+strlen(car)+4]='\0';
		strcat ( nom_fic_his , "/XS" );
		sprintf ( car , "%d" ,can+1 );
		strcat ( nom_fic_his , car );
		strcat ( nom_fic_his , ".his" );
		nom_fic_his[strlen(repertoire_ima)+3+strlen(car)+4]='\0';
		strcat ( nom_fic_bid , "/XS" );
		sprintf ( car , "%d" , can+1 );
		strcat ( nom_fic_bid , car );
		strcat ( nom_fic_bid , ".bid" );
		nom_fic_bid[strlen(repertoire_ima)+3+strlen(car)+4]='\0';
	}

	tab_nom_tim[can] = (char *) malloc ( strlen(nom_fic_tim) + 1 );
	strcpy ( tab_nom_tim[can] , nom_fic_tim );
	tab_nom_his[can] = (char *) malloc ( strlen(nom_fic_his) + 1 );
	strcpy ( tab_nom_his[can] , nom_fic_his );
	tab_nom_bid[can] = (char *) malloc ( strlen(nom_fic_bid) + 1 );
	strcpy ( tab_nom_bid[can] , nom_fic_bid );
  }
		
/* calcul du nombre de blocs */

/* Blocs N/S */
if ( (*nb_lig) % 256 == 0 )
	nb_blocs_ns = (*nb_lig) / 256;
else
	nb_blocs_ns = (*nb_lig) / 256 + 1;
/* Blocs W/E */
if ( (*nb_col) % (256/(*nboct_pix)) == 0 )
	nb_blocs_we = (*nb_col) / (256/(*nboct_pix));
else
	nb_blocs_we = (*nb_col) / (256/(*nboct_pix)) + 1;

ptr_buff_tim = (unsigned char *) malloc ( nb_blocs_ns*nb_blocs_we*sizeof(int) );
if ( ptr_buff_tim == NULL )
{
	sprintf ( CAI_ERREUR , "Erreur : probleme d'allocation \n" );
	unlink(nom_fic);
        free ( ptr_buff_tim );
        free ( tab_nom_tim );
        free ( tab_nom_his );
        free ( tab_nom_bid );
	goto ERREUR;
}
for ( can = 0 ; can < (*nb_can) ; can++ )
{
	ptr_file_tim = fopen ( tab_nom_tim[can] ,"w");
	ptr_file_his = fopen ( tab_nom_his[can] ,"w");
	ptr_file_bid = fopen ( tab_nom_bid[can] ,"w");
	if ((ptr_file_tim == NULL) || (ptr_file_his == NULL) || (ptr_file_bid == NULL))
	{
		sprintf (CAI_ERREUR,"Erreur : creation fichier %s impossible\n",tab_nom_tim[can] );
		unlink(nom_fic);
		for(i=0;i<can;i++)
		{
			unlink(tab_nom_tim[i]);
			unlink(tab_nom_his[i]);
			unlink(tab_nom_bid[i]);
		}

                free ( ptr_buff_tim );
                free ( tab_nom_tim );
                free ( tab_nom_his );
                free ( tab_nom_bid );
		goto ERREUR;
	}

	for ( i = 0 ; i < nb_blocs_ns*nb_blocs_we ; i++ )
	{
		offset = 256*256*i + 1;
		memcpy(&ptr_buff_tim[i*4],&offset,4);
	}
	retour = fwrite ( ptr_buff_tim , nb_blocs_ns*nb_blocs_we , sizeof(int) , ptr_file_tim );
	if ( retour == 0 )
	{
		sprintf ( CAI_ERREUR ,"Erreur : ecriture impossible dans %s\n ",tab_nom_tim[can] );
		unlink(nom_fic);
		for(i=0;i<=can;i++)
		{
			unlink(tab_nom_tim[i]);
			unlink(tab_nom_his[i]);
			unlink(tab_nom_bid[i]);
		}
                free ( ptr_buff_tim );
                free ( tab_nom_tim );
                free ( tab_nom_his );
                free ( tab_nom_bid );
		goto ERREUR;
	}

	fclose ( ptr_file_tim );
	fclose ( ptr_file_his );
	fclose ( ptr_file_bid );
}


/*************************************************/
/* Constitution du nom des fichiers a ecrire	 */
/* et insertion dans les tableaux		 */
/*************************************************/


for (can=0; can<*nb_can; can ++ )
{
    nom_fic_canal[0]='\0';
    strcpy ( nom_fic_canal , repertoire_ima );
    if (*nb_can == 1)
    {
	strcat ( nom_fic_canal , "/PAN.gds" );
	nom_fic_canal[strlen(repertoire_ima)+8]='\0';
    }
    else
    {
    	strcat ( nom_fic_canal , "/XS" );
  	sprintf ( car , "%d" , can+1 );
	strcat ( nom_fic_canal , car );
	strcat ( nom_fic_canal , ".gds" );
        nom_fic_canal[strlen(repertoire_ima)+3+strlen(car)+4]='\0';
      }

      num = creat(nom_fic_canal,PERMS);
      if (num == -1)
      {
        sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
	unlink(nom_fic);
	for(i=0;i<=can;i++)
	{
			unlink(tab_nom_tim[i]);
			unlink(tab_nom_his[i]);
			unlink(tab_nom_bid[i]);
	}

        goto ERREUR;
       }
        image1->ACCES_CANAL_INT[can]=num;
        image1->POS_OCTET[can]=0;
  }



free ( ptr_buff_tim );
free ( tab_nom_tim );
free ( tab_nom_his );
free ( tab_nom_bid );
  
return (image1);

ERREUR :
  return(NULL);
}


/* 
  Fin de l'operation cai_ouvre_creation_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_atg2                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image atg2                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_atg2(image1, canal,       */       		      /* ------------------                             premiere_ligne,            */
/*                                                premiere_colonne,          */
/*                                                nombre_lignes_lire ,       */
/*                                                nombre_colonnes_lire,      */
/*                                                pas_ligne ,pas_colonne,    */
/*                 		                  data_image )               */
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
/*     cai_lecture_canal_atg2 (S) (CAI_OK_KO) :    = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_lecture_canal_atg2(image1 ,  
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

 int   num,i,j,k,n,l,p;     /* variables de travail                          */
 int pt_oct;		    /* position courante dans le fichier image       */
 int no_oct;                /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int   oct_pix;             /* nb octets par pixels                          */
 long  nb_oct;              /* nb oct =1er octet a lire - position courante  */
 unsigned char *buf_lec;    /* buffer lecture d'un pave                      */
 unsigned char *buffer;     /* buffer lecture de taille_pave lignes completes*/
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 long nb_oct_pave;          /* taille du pave en octets                      */
 unsigned char *ind_lec;    /* pointeur de travai pour transfert pave        */
 unsigned char *ind_tab;    /* pointeur de travai pour transfert pave        */
 int pave;                  /* nombre de paves colonnes dans une ligne       */
 int nbcol_tot;             /* nb pave*taille colonnes du pave               */
 int taille;                /* nbcol_tot*taille lignes pave                  */
 int bloc;		    /* indice de boucle sur les paves                */
 int nolig;		    /* indice de boucle sur les lignes               */
 int numlig;                /* numero de la ligne reconstituee               */


/*.........................
  Initialisations
  ........................*/
   iret=CAI_OK;
   num=image1->ACCES_CANAL_INT[*canal-1];
   oct_pix=image1->NBOCT_PIX;
   nb_oct_pave=oct_pix*image1->LIGNES_PAVE*image1->COLONNES_PAVE;
   lig=*premiere_ligne;
   i=0;
   pch=data_image;

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
   pave=image1->NB_COL/image1->COLONNES_PAVE;
   if ((image1->NB_COL%image1->COLONNES_PAVE)!=0)
   {
      pave=pave+1;
   }
   nbcol_tot=pave*image1->COLONNES_PAVE;
   taille=nbcol_tot*image1->LIGNES_PAVE*oct_pix;

   buf_lec = (unsigned char *) malloc(nb_oct_pave);
   if (buf_lec == NULL)
   {
      iret=CAI_KO;
      strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
      return(iret);
   }
   else
   {
      buffer = (unsigned char *) malloc(taille);
      if (buffer == NULL)
      {
         iret=CAI_KO;
         strcpy(CAI_ERREUR,"Probleme d'allocation memoire");
         free(buf_lec);
         return(iret);
      }
    }


/*.....................
     Pour chaque ligne a lire
  .......................................*/
     while ((i < (*nombre_lignes_lire))&&(iret==CAI_OK))
    {  
/*............................................
       Recherche de la position courante dans le fichier
  ............................................*/
       pt_oct=lseek(num,0L,1);

/*................................
       Calcul du numero du 1er octet a lire
  ....................................*/
       nolig=((lig-1)/image1->LIGNES_PAVE)*image1->LIGNES_PAVE;  
       no_oct=image1->POS_OCTET[*canal-1]+(nolig*nbcol_tot*oct_pix);

/*..................................
       Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
       nb_oct=(long)(no_oct-pt_oct);
       lseek(num,nb_oct,1);

/*...........................................
       lecture de image1->LIGNES_PAVE lignes completes de nolig+1
       a nolig+image1->LIGNES_PAVE
  ................................................*/
        for (bloc=1;bloc<=pave;bloc=bloc+1)
	{
/*        lecture d'un bloc de nb_oct_pave octets consecutifs */
          n=read(num,buf_lec,nb_oct_pave);
          if (n<=0)
          {
            iret=CAI_KO;
            strcpy(CAI_ERREUR,"Erreur lecture donnee image atg2");
          }
          else
          { 
/*	   stockage du pave dans le tableau de image1->LIGNES_PAVE lignes 
		completes */
           ind_tab=buffer+(bloc-1)*image1->COLONNES_PAVE*oct_pix;
	   ind_lec=buf_lec;

	    for (l=1;l<=image1->LIGNES_PAVE;l=l+1)
            {
	      for (p=1;p<=image1->COLONNES_PAVE*oct_pix;p=p+1)
	      {
		*ind_tab=*ind_lec;
		ind_tab=ind_tab+1;
		ind_lec=ind_lec+1;
	       }
	       ind_tab=ind_tab+(nbcol_tot-image1->COLONNES_PAVE)*oct_pix;
	     }
	   }
        }
  
/*......................................
        Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/       
  
	if (iret == CAI_OK)
	{
         numlig=lig-nolig;
	 deb=buffer+(numlig-1)*nbcol_tot*oct_pix+
	            ((*premiere_colonne)-1)*oct_pix;
  
	 while ((numlig<=image1->LIGNES_PAVE)&&(i<(*nombre_lignes_lire)))
	 {
           j=1;
           while (j<=(*nombre_colonnes_lire))
           {
	     for (k=0;k<oct_pix;k=k+1)
	     {
		 *(pch+k)=*(deb+k);
 	     }
	     j=j+1;
             pch=pch+oct_pix;
	     deb=deb+((*pas_colonne)*oct_pix);
	    }
            numlig=numlig+(*pas_ligne);
            deb=buffer+((numlig-1)*nbcol_tot*oct_pix)+
               ((*premiere_colonne)-1)*oct_pix;           
            lig=lig+(*pas_ligne);
            i=i+1;
	    }
	  }
   }
	free(buffer);
        free(buf_lec);

 	return(iret);

}

/*
  Fin de l'operation cai_lecture_canal_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_atg2                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image atg2                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_OK_KO  cai_ecriture_canal_atg2 (image1, canal,     */       		      /* ------------------                               premiere_ligne,          */
/*                                                  nombre_lignes_ecrire,    */
/*                                                  nombre_colonnes_ecrire,  */
/*                 		                    data_image )             */
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
/*     cai_ecriture_canal_atg2       (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_atg2(image1 ,  
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
int	nb_colon_correc;	/* nombre de colonnes multiple de 256         */
int	nb_blocs;		/* nombre de paquets de 256 pixels            */
int	i_bloc;			/* compteur de blocs			      */
int	nb_last_pix;		/* nb initial de pixels dans le dernier bloc  */
int	nb_blocs_prec_first_lig;/* nb de blocs avant 1ere ligne a ecrire      */
int	nb_blocs_prec_lig;	/* nb de blocs avant ligne courante	      */
int	pos_first_lig_ds_bloc;	/* position 1ere ligne a ecrire dans son bloc */
int	pos_lig_ds_bloc;	/* position ligne courante dans son bloc      */
long	position_first_oct;	/* position du 1er octet a ecrire 	      */
long	position_courante;	/* position courante dans le fichier          */
long	deplacement;		/* deplacement a effectuer		      */
long	depla_bl;		/* deplacement a effectuer 		      */
int	i_ligne;		/* compteur de lignes			      */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire       */
unsigned char *buff_bloc;	/* tableau contenant un bloc a ecrire	      */
int	taille_wr;		/* nombre d'elements du buffer a ecrire       */
int	cr_write;		/* code retour de write			      */
unsigned char *completion;	/* caractere completant les blocs 	      */
int num;                        /* descripteur de fichier                     */
int nboct_pix;			/* nombre d'octets par pixel                  */
unsigned char *pch;		/* pointeur chaine servant au transfert       */
int i;                          /* indice de boucle                           */



/* initialisations */

i_ligne = 0;
num = image1->ACCES_CANAL_INT[*canal-1];
nboct_pix = image1->NBOCT_PIX;



/********************************************************/
/* Calcul du nombre de colonnes multiple de 256 de 	*/
/* l'image a ecrire					*/
/********************************************************/

if ( (*nombre_colonnes_ecrire) % 256 == 0 )
	nb_colon_correc = (*nombre_colonnes_ecrire);
else
	nb_colon_correc = (*nombre_colonnes_ecrire) - ((*nombre_colonnes_ecrire)%256) + 256;

/********************************************************/
/* Calcul du nombre de blocs complets de 256 pixels 	*/
/* sur une meme ligne (eventuellement completes par 0)	*/
/********************************************************/

nb_blocs = nb_colon_correc / 256;


/********************************************************/
/* Calcul du nombre de pixels presents initialement 	*/
/* dans le dernier bloc d'une ligne			*/
/********************************************************/

nb_last_pix = 256 - ( nb_colon_correc - (*nombre_colonnes_ecrire) );


/********************************************************/
/* Calcul du nombre de blocs de 256 lignes precedant la	*/
/* premiere ligne a ecrire				*/
/* Calcul de la position de la premiere ligne a ecrire 	*/
/* au sein d'un bloc de 256 lignes			*/
/********************************************************/

if ( *premiere_ligne <= 256 )
{
	nb_blocs_prec_first_lig = 0;
	pos_first_lig_ds_bloc = *premiere_ligne;
}
else if ( (*premiere_ligne - 1) % 256 == 0 )
{
	nb_blocs_prec_first_lig = (*premiere_ligne - 1) / 256;
	pos_first_lig_ds_bloc = 1;
}
else
{
	nb_blocs_prec_first_lig =
	( (*premiere_ligne- 1 ) - (*premiere_ligne - 1)%256 ) / 256;
	pos_first_lig_ds_bloc = 1 + (*premiere_ligne - 1)%256;
}



/********************************************************/
/* Initialisations					*/
/********************************************************/

pos_lig_ds_bloc = pos_first_lig_ds_bloc;
nb_blocs_prec_lig = nb_blocs_prec_first_lig;


/********************************************************/
/* POUR	chacune des lignes a ecrire	               	*/
/********************************************************/
for ( i_ligne = 0 ; i_ligne < *nombre_lignes_ecrire ; i_ligne++ )
{
/********************************************************/
/*	calcul de la position du premier octet de la	*/
/*	ligne a ecrire					*/
/********************************************************/
   
	position_first_oct = (long) ( image1->POS_OCTET[*canal-1]
				+ 256 * nboct_pix * (pos_lig_ds_bloc - 1) 
	        + 256 * nb_blocs_prec_lig * nb_colon_correc * nboct_pix );

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

	buff_lig = data_image +(*nombre_colonnes_ecrire) * nboct_pix * i_ligne;

/********************************************************/
/*	initialisation du decalage de 255*256 pixels	*/
/********************************************************/

	depla_bl = 0;

/********************************************************/
/*	POUR	chacun des blocs de 256 pixels		*/
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

		buff_bloc = buff_lig + i_bloc * 256 * nboct_pix;

/********************************************************/
/*		determination du nombre d'octets a 	*/
/*		ecrire					*/
/********************************************************/
		if ( i_bloc == nb_blocs - 1 )
                 {
			taille_wr = nb_last_pix*nboct_pix;
                 }
		else
                 {
			taille_wr = 256 * nboct_pix;
		  }

/********************************************************/
/*		ecriture dans le fichier		*/
/********************************************************/
		cr_write = write ( num , buff_bloc , taille_wr );
		if ( cr_write != taille_wr )
		{
			strcpy ( CAI_ERREUR , "Erreur ecriture canal image atg2\n" );
			goto ERREUR;
		}
 
/********************************************************/
/*		completion eventuelle du dernier bloc	*/
/********************************************************/

		if ((i_bloc == nb_blocs - 1) && ((256 - nb_last_pix) > 0))
		{
			taille_wr = (256 - nb_last_pix)*nboct_pix;
		        completion=(unsigned char *) malloc(taille_wr);
                        pch=completion;
		        for (i=0;i<taille_wr;i=i+1)
                        {
			    *pch='\0';
                             pch=pch+1;
			 }
			cr_write = write ( num ,  completion  , taille_wr );
			if ( cr_write != taille_wr )
			{

				strcpy ( CAI_ERREUR , "Erreur ecriture canal image atg2\n" );
				goto ERREUR;
			}
			free(completion);
		}

/********************************************************/
/*		mise a jour du decalage de 255*256 pixels*/
/********************************************************/

		depla_bl = 255*256*nboct_pix;

/********************************************************/
/*	FIN POUR                                        */ 
/********************************************************/
	}

/********************************************************/
/*	mise a jour de la position de la ligne dans le	*/
/*	bloc, et du nombre de blocs precedant la ligne	*/
/********************************************************/

	if ( pos_lig_ds_bloc == 256 )
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


return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/* 
  Fin de l'operation cai_ecriture_canal_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_atg2                                             */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format atg2 et de          */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_atg2(image1)                      */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_atg2       (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_atg2 (image1)

CAI_IMAGE *image1;

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
  Fin de l'operation cai_ferme_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_atg2                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en atg2 et de        */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_atg2(format,nom_ima)           */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_atg2    (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_dest_ima_atg2 ( repertoire , 
                              nom_ima )

char	*repertoire;		/* repertoire de stockage de l'image */
char	*nom_ima;		/* nom de l'image */

{
int     iret ;                   /* code retour fonction de fermeture */
int	i_can;			/* compteur de canaux */
char	nom_fic_courant[1024];	/* nom du fichier entete a detruire */
char    repertoire_ima[1024];   /* repertoire + nom de l'image */
char    str_num_fic[6];         /* chaine numero du canal */
int	numero_fic;		/* numero du fichier canal */
char	str_cmd[100];		/* chaine de commande pour appels systeme */

int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 

CAI_IMAGE   *image1;             /* structure ouverture image */
int         nb_can;	        /* nombre de canaux de l'image */
int         nboct_pix;		/* nombre d'octets par pixel image */
int         nb_col;	        /* nombre de colonnes de l'image */
int         nb_lig;             /* nombre de lignes de l'image */

/*************************************************/
/* Construction du nom de la sous-directory      */
/* qui contient l'image ATG2                     */
/*************************************************/
if (strlen(repertoire) != 0)
{
 strcpy ( repertoire_ima , repertoire );
 strcat ( repertoire_ima, "/" );
 strcat ( repertoire_ima , nom_ima );
 repertoire_ima[strlen(repertoire)+1+strlen(nom_ima)]='\0';
}
else
{
 strcpy ( repertoire_ima , nom_ima );
 repertoire_ima[strlen(nom_ima)]='\0';
}

/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_atg2			*/
/************************************************/

  image1 = cai_ouvre_lecture_atg2 ( repertoire,nom_ima, &nb_can ,&nboct_pix ,
				    &nb_col , &nb_lig );	
  if ( image1 == NULL) goto ERREUR;

  image1->NB_CANAUX=nb_can;
  image1->NB_COL=nb_col;
  image1->NB_LIG=nb_lig;
  image1->NBOCT_PIX=nboct_pix;

  iret=cai_ferme_atg2(image1);



/************************************************/
/* Verification de l'existence de la sous-  	*/
/* directory					*/
/************************************************/

cr_stat = stat ( repertoire_ima , &buf_stat );
if ( cr_stat == -1 )
{
	sprintf ( CAI_ERREUR ,
	"Erreur : acces impossible a %s\n" , repertoire_ima );
	goto ERREUR;
}
if ( (buf_stat.st_mode & S_IFMT) != S_IFDIR )
{
	sprintf ( CAI_ERREUR ,
	"Erreur : %s n'est pas un repertoire\n" , repertoire_ima );
	goto ERREUR;
}

/************************************************/
/* Constitution du nom des fichiers et 		*/
/* destruction			 		*/
/************************************************/
 
strcpy ( nom_fic_courant , repertoire_ima );
strcat ( nom_fic_courant , "/.gen" );
nom_fic_courant[strlen(repertoire_ima)+5]='\0';
cr_stat = stat ( nom_fic_courant , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_courant );

if ( nb_can == 1 )
{
	strcpy ( nom_fic_courant , repertoire_ima );
	strcat ( nom_fic_courant , "/PAN.bid" );
	nom_fic_courant[strlen(repertoire_ima)+8]='\0';
	cr_stat = stat ( nom_fic_courant , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_courant );

	strcpy ( nom_fic_courant , repertoire_ima );
	strcat ( nom_fic_courant , "/PAN.his" );
	nom_fic_courant[strlen(repertoire_ima)+8]='\0';
	cr_stat = stat ( nom_fic_courant , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_courant );
	
	strcpy ( nom_fic_courant , repertoire_ima );
	strcat ( nom_fic_courant , "/PAN.gds" );
	nom_fic_courant[strlen(repertoire_ima)+8]='\0';
	cr_stat = stat ( nom_fic_courant , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_courant );

	strcpy ( nom_fic_courant , repertoire_ima );
	strcat ( nom_fic_courant , "/PAN.tim" );
	nom_fic_courant[strlen(repertoire_ima)+8]='\0';
	cr_stat = stat ( nom_fic_courant , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fic_courant );
}
else
{
	i_can=1;
	numero_fic=1;
	while ( (i_can<=nb_can) && (numero_fic<10000) )
	{
		strcpy ( nom_fic_courant , repertoire_ima );
		strcat ( nom_fic_courant , "/XS" );
                sprintf ( str_num_fic , "%d" , numero_fic );
                strcat ( nom_fic_courant , str_num_fic );
                strcat ( nom_fic_courant , ".bid" );
nom_fic_courant[strlen(repertoire_ima)+3+strlen(str_num_fic)+4]='\0';
		cr_stat = stat ( nom_fic_courant , &buf_stat );
		if ( cr_stat == 0 )
			unlink ( nom_fic_courant );

		strcpy ( nom_fic_courant , repertoire_ima );
		strcat ( nom_fic_courant , "/XS" );
                sprintf ( str_num_fic , "%d" , numero_fic );
                strcat ( nom_fic_courant , str_num_fic );
                strcat ( nom_fic_courant , ".his" );
                nom_fic_courant[strlen(repertoire_ima)+3+strlen(str_num_fic)+4]='\0';
		cr_stat = stat ( nom_fic_courant , &buf_stat );
		if ( cr_stat == 0 )
			unlink ( nom_fic_courant );

		strcpy ( nom_fic_courant , repertoire_ima );
		strcat ( nom_fic_courant , "/XS" );
                sprintf ( str_num_fic , "%d" , numero_fic );
                strcat ( nom_fic_courant , str_num_fic );
                strcat ( nom_fic_courant , ".gds" );
                nom_fic_courant[strlen(repertoire_ima)+3+strlen(str_num_fic)+4]='\0';
		cr_stat = stat ( nom_fic_courant , &buf_stat );
		if ( cr_stat == 0 )
			unlink ( nom_fic_courant );

		strcpy ( nom_fic_courant , repertoire_ima );
		strcat ( nom_fic_courant , "/XS" );
                sprintf ( str_num_fic , "%d" , numero_fic );
                strcat ( nom_fic_courant , str_num_fic );
                strcat ( nom_fic_courant , ".tim" );
                nom_fic_courant[strlen(repertoire_ima)+3+strlen(str_num_fic)+4]='\0';
		cr_stat = stat ( nom_fic_courant , &buf_stat );
		if ( cr_stat == 0 )
			unlink ( nom_fic_courant );

		i_can++;
		numero_fic++;
        }


}

sprintf(str_cmd,"rmdir %s",repertoire_ima);
system(str_cmd);
 
return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/*
  Fin de l'operation cai_dest_ima_atg2
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_atg2                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture ecriture d'une image au format  */
/* -----     atg2 et de tous les fichiers qui la composent                   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_atg2(nom_image,          */
/*  ---------------- 		                         nb_can,             */
/*				                         nboct_pix,	     */	
/*				                         nb_col,             */
/*				                         nb_lig)             */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_atg2  (S) (CAI_IMAGE *)    : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_atg2(repert,
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
   int fic;			/* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[2];			/* indce du canal traite     		    */
   int num,k,n;		        /* indice 				    */
   Fsro_format_data fsro;       /* structure decrivant l'entete atg2        */

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
 if (strlen(repert)!= 0)
 {
   sscanf(repert,"%s",nom_fic);
   strcat(nom_fic,"/");
   strcat(nom_fic,nom_image);
   strcat(nom_fic,"/");
   strcat(nom_fic,".gen");
   nom_fic[strlen(repert)+1+strlen(nom_image)+5]='\0';
  }
 else
 {
   sscanf(nom_image,"%s",nom_fic);
   strcat(nom_fic,"/");
   strcat(nom_fic,".gen");
   nom_fic[strlen(nom_image)+5]='\0';
  }
/*........................
   Ouverture du fichier entete
  ..........................................*/
   num=open(nom_fic,O_RDONLY,0);

   if (num == -1)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete ATG2");
    }
   else
   {
     
/*....................................
       Lecture du fichier entete
  ............................................*/

          n=read( num , &fsro , sizeof(Fsro_format_data));
	  if ( n <= 0 )
	  {
           sprintf(CAI_ERREUR,"Erreur lecture entete ATG2 %s\n",nom_fic),
	   iret=0;
	   return (NULL);
	  }

/*.........................................
	  Fermeture du fichier entete ATG2
  ..........................................*/
	  close ( num );

/*.........................................
          Recuperation du nombre d'octets par pixel .
  ....................................................*/
	  if (fsro.pvb>16)
  	  {
	      iret=0;
	      sprintf(CAI_ERREUR,"Taille du pixel non traitable %d\n",fsro.pvb);
	      return (NULL);
	  }
	  else
	  {
            if ((fsro.pvb%8)==0)
	       *nboct_pix=fsro.pvb/8;
            else
	       *nboct_pix=fsro.pvb/8+1;
            image1->NB_BITS_PIX = fsro.pvb;
	  }

/*.........................................
          Recuperation de la taille d'un pave en lignes
  ....................................................*/
	  image1->LIGNES_PAVE=fsro.pnl;
/*.........................................
          Recuperation de la taille d'un pave en colonnes
  ....................................................*/
	  image1->COLONNES_PAVE=fsro.pnc;

/*.........................................
          Calcul du nombre de lignes : taille du pave*nombre de pave en ligne
  ....................................................*/
	  *nb_lig=fsro.nfl*image1->LIGNES_PAVE;
/*.........................................
          Calcul du nombre de colonnes: taille  pave*nombre  pave en colonnes
  ....................................................*/
	  *nb_col=fsro.nfc*image1->COLONNES_PAVE;
/*.........................................................
         Recuperation du nombre de canaux en fonction du nom des fichiers .gds
	 1 - test sur l'existence du fichier PAN.gds monochrome
  ...........................................................*/
        nom_fic[0]='\0';
       if (strlen(repert)!= 0)
       {
          sscanf(repert,"%s",nom_fic);
          strcat(nom_fic,"/");
          strcat(nom_fic,nom_image);
          strcat(nom_fic,"/PAN.gds");
          nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
        }
        else
        {
          sscanf(nom_image,"%s",nom_fic);
          strcat(nom_fic,"/PAN.gds");
          nom_fic[strlen(nom_image)+8]='\0';
        }
        num=open(nom_fic,O_RDONLY,0);
        if (num<=0)
	{

/*	  2 - test sur l'existence du fichier XS1.gds monochrome
  ...........................................................*/

          nom_fic[0]='\0';
          if (strlen(repert)!= 0)
          {
            sscanf(repert,"%s",nom_fic);
            strcat(nom_fic,"/");
            strcat(nom_fic,nom_image);
            strcat(nom_fic,"/XS1.gds");
            nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
          }
          else
          {
            sscanf(nom_image,"%s",nom_fic);
            strcat(nom_fic,"/XS1.gds");
            nom_fic[strlen(nom_image)+8]='\0';
          }
 
          fic=open(nom_fic,O_RDONLY,0);
          if (fic<=0)
	  {
	     iret=0;
	     sprintf(CAI_ERREUR,"Erreur nombre de canaux incorrect ATG2\n");
	     return (NULL);
	  }
	  else
          {   
	     close(fic);
	     *nb_can=3;
           }
         }
        else
        {
	  close(num);
	  *nb_can=1;
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

     if (*nb_can==1)
     {
	 sscanf(nom_image,"%s",deb);
	 strcat(deb,"/PAN.gds");
	 sscanf(nom_image,"%s",deb1);
	 strcat(deb1,"/PAN.gds");
         nom_fic[0]='\0';
         if (strlen(repert)!=0)
         {
          sscanf(repert,"%s",nom_fic);
          strcat(nom_fic,"/");
          strcat(nom_fic,nom_image);
          strcat(nom_fic,"/PAN");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
         }
         else
         {
          sscanf(nom_image,"%s",nom_fic);
          strcat(nom_fic,"/PAN");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(nom_image)+8]='\0';
         }
         fic=open(nom_fic,O_RDWR,0);
         image1->ACCES_CANAL_INT[0]=fic;
         image1->POS_OCTET[0]=0;

        
     }
     else
     {

       k=1;    
       while((k<=*nb_can)&&(k<CAI_NB_MAX_CAN))
       {
          sprintf(car,"%d",k);
          sscanf(nom_image,"%s",deb);
          strcat(deb,"/XS");
          strcat(deb,car);
          strcat(deb,".gds");
          deb=deb+100;
          sscanf(nom_image,"%s",deb1);
          strcat(deb1,".XS");
          strcat(deb1,car);
          strcat(deb1,".gds");
          deb1=deb1+100;
          nom_fic[0]='\0';
         if (strlen(repert)!=0)
         {
          sscanf(repert,"%s",nom_fic);
          strcat(nom_fic,"/");
          strcat(nom_fic,nom_image);
          strcat(nom_fic,"/XS");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(repert)+1+strlen(nom_image)+8]='\0';
         }
         else
         {
          sscanf(nom_image,"%s",nom_fic);
          strcat(nom_fic,"/XS");
          strcat(nom_fic,car);
          strcat(nom_fic,".gds");
          nom_fic[strlen(nom_image)+8]='\0';
         }
          fic=open(nom_fic,O_RDWR,0);
          if (fic != -1) 
          {
           image1->ACCES_CANAL_INT[k-1]=fic;
           image1->POS_OCTET[k-1]=0;
           k=k+1;
          }
	  else
          {
	    iret=0;
            strcpy(CAI_ERREUR,"incoherence au niveau du nb fichiers canaux");
	    return(NULL);
          }
       }
     }

     return (image1);
   }
   else
   {
     return(NULL);
    }

}
/* 
  Fin de l'operation cai_ouvre_modifie_atg2
*/
