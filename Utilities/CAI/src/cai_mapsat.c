/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_mapsat.c                                                  */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format mapsat                                     */ 
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
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>

#define _CAI_IMAGE
# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_mapsat.c,v 1.4 98/07/21 14:54:07 cmf Exp $";

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
/* OPERATION :    cai_liste_ima_mapsat                                       */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format mapsat                 */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_mapsat (repert,tab_ima)         */
/* -----------------                                                         */
/*                                                                           */
/*     repert        (E) (char *)   : repertoire a explorer		     */
/*     tab_ima       (S) (char ***) : pointeur sur tableau des images listees*/
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_mapsat   (S) (int)     :  = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



int cai_liste_ima_mapsat (repert,
		          tab_ima)

char *repert;
char ***tab_ima;

{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */

struct dirent *dp;   /* structure resultat de la fonction readdir   */

   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2;          /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images mapsat  identifiees         */
   int i;               /* indice de boucle                            */

  /*........................................
   INITIALISATION et ouverture du repertoire choisi
  .........................................*/
   nb_ima=0;
   dirp=opendir(repert);

   if(dirp!=NULL)
   {


/*......................................
   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE MAPSAT
      (EXTENSION =.im ou .it)
   ..........................................*/

   while((dp=readdir(dirp))!=NULL)
   {
     car='.';
     pch0=(char *)strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='i';
       pch1=(char *)strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='m';
         pch2=(char *)strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
           {
              *pch0='\0';
              *pch1=' ';
	      *pch2=' ';
	      pch2++;
	      if (*pch2 == '\0' )
	      {
	       nb_ima=nb_ima+1;
	      }
           }
	 else         
          {
              car='t';
              pch2=(char *)strrchr(dp->d_name,car);
              if ((pch2 != NULL)&&(pch2==pch1+1))
              {
                *pch0='\0';
                *pch1=' ';
	        *pch2=' ';
	      	pch2++;
	      	*pch2='\0';
	         nb_ima=nb_ima+1;
               }
	   }
        }
      }
    }

    (void)closedir(dirp);

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT MAPSAT
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne
   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE MAPSAT
      (EXTENSION =.im ou .it)
      SI C'EST LE CAS STOCKAGE DE L'IMAGE DANS LA LISTE DES IMAGES MAPSAT
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
     car='.';
     pch0=(char *)strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='i';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='m';
         pch2=(char *)strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
           {
              *pch0='\0';
              *pch1=' ';
	      *pch2=' ';
	      pch2++;
	      if (*pch2 == '\0' )
	      {
               strcpy((*tab_ima)[nb_ima],dp->d_name);
	       nb_ima=nb_ima+1;
	      }
           }
	 else         
          {
              car='t';
              pch2=(char *)strrchr(dp->d_name,car);
              if ((pch2 != NULL)&&(pch2==pch1+1))
              {
                *pch0='\0';
                *pch1=' ';
	        *pch2=' ';
	      	pch2++;
	      	*pch2='\0';
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

/*
 Fin de l'operation cai_liste_ima_mapsat
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_mapsat                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format mapsat    */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_mapsat (nom_image,       */
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
/*     cai_ouvre_lecture_mapsat  (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_mapsat(repert,
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
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[4];			/* indice du canal traite     		    */
   int i,fic;			/* indice 				    */
   int RVB,MONO;                /* indique le nombre de canaux              */
   char type_inf[5];            /* type fichier lu dans l'entete            */
   char label[64];		/* label lu dans l'entete		    */
   int lig,col;                 /* variables intermediaires                 */

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;
   *nb_lig=0;
   *nb_col=0;
   *nboct_pix=0;
   iret=1;
   RVB=3;
   MONO=1;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*.................................................
   Constitution du nom du fichier MAPSAT .it
   et test sur son existence
  .................................................*/
   nom_fic[0]='\0';
   if (strlen(repert)!=0)
   {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      strcat(nom_fic,".it");
      nom_fic[strlen(repert)+1+strlen(nom_image)+3]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".it");
      nom_fic[strlen(nom_image)+3]='\0';
   }
   fic_in=fopen(nom_fic,"r");
   *nb_can=RVB;
   strncpy(car,".it",3);
   car[3]='\0';
   if (fic_in==NULL)
    {
/*...........................................................
     Test sur l'existence du fichier MAPSAT avec extension .im
   ..........................................................*/
     nom_fic[0]='\0';
     if (strlen(repert)!=0)
     {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      strcat(nom_fic,".im");
      nom_fic[strlen(repert)+1+strlen(nom_image)+3]='\0';
     }
     else
     {
      sscanf(nom_image,"%s",nom_fic);
      strcat(nom_fic,".im");
      nom_fic[strlen(nom_image)+3]='\0';
     }
     fic_in=fopen(nom_fic,"r");
     *nb_can=MONO;
     car[0]='\0';
     strncpy(car,".im",3);
     car[3]='\0';
     if (fic_in==NULL)
     {
       iret=0;
       strcpy(CAI_ERREUR,"Erreur Ouverture Fichier MAPSAT");
       return (NULL);
      }
     }
/*..........................................
    verification que le fichier est de type mapsat
  ...................................................*/
    fread(type_inf,4,1,fic_in);
/*...
    pour une image trichrome
  ......*/
    if (*nb_can==RVB)
    {
      if (strncmp(type_inf,"ITRI",4) !=0)
	{
	   iret=0;
	   strcpy(CAI_ERREUR,"l'image n'est pas au format MAPSAT");
           return (NULL);
         }
      }

/*.....
     pour une image monochrome
  .....*/
     if (*nb_can==MONO)
     {
       if (strncmp(type_inf,"IMON",4) !=0)
	{
	   iret=0;
	   strcpy(CAI_ERREUR,"l'image n'est pas au format MAPSAT");
           return (NULL);
         }
      }

/*.....
     positionnement sur les infos a recuperer
  .....*/
     fread(label,64,1,fic_in);
     fread(nb_lig,4,1,fic_in);
     fread(nb_col,4,1,fic_in);
     *nboct_pix=1;
     image1->NB_BITS_PIX=8;
     lig=*nb_lig;
     col=*nb_col;
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
     fclose (fic_in);
     fic=open(nom_fic,O_RDONLY,0);
  
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;

     for (i=0;i<*nb_can;i=i+1)
     {
           sscanf(nom_image,"%s",deb);
	   strcat(deb,car);
  	   deb=deb+100;
           sscanf(nom_image,"%s",deb1);
           strcat(deb1,car);
           deb1=deb1+100;
           image1->POS_OCTET[i]=col+(i*lig*col);
           image1->ACCES_CANAL_INT[i]=fic;


      }

 
      return(image1);
  
}

/* 
   Fin de l'operation cai_ouvre_lecture_mapsat
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_mapsat                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format mapsat et de tous les fichiers qui la composent       */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_mapsat (nom_image,       */
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
/*     cai_ouvre_creation_mapsat  (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_IMAGE *cai_ouvre_creation_mapsat(repert,
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
   unsigned char *ptr;          /* pointeur sur buffer fichier              */
   int	taille_entete;		/* taille de l'entete en octets		    */
   int pas_echantil;            /* pas d'echantillonnage de l'image         */
   int taille_buff_label;	/* taille en octets du label a ecrire       */
   char *buff_label;		/* pointeur sur la chaine label a ecrire    */
   


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja .Il n'y a qu'un fichier .it si multispectral
   .im si monospectral
  .................................................*/
   nom_fic[0]='\0';
   if (strlen(repert)!=0)
   {
      sscanf(repert,"%s",nom_fic);
      strcat(nom_fic,"/");
      strcat(nom_fic,nom_image);
      if (*nb_can == 1)
       strcat(nom_fic,".im");
      else
       strcat(nom_fic,".it");
      nom_fic[strlen(repert)+1+strlen(nom_image)+3]='\0';
   }
   else
   {
      sscanf(nom_image,"%s",nom_fic);
      if (*nb_can == 1)
       strcat(nom_fic,".im");
     else
       strcat(nom_fic,".it"); 
      nom_fic[strlen(nom_image)+3]='\0';
   }


   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s MAPSAT existe \n" , nom_image );
	goto ERREUR;
   }

/*************************************************/
/* determination de la taille de l'entete :	 */
/* taille de l'entete = taille d'une ligne, soit */
/* nb_col et au minimum 128 octets	 	 */
/* ecriture de l'entete                          */
/*************************************************/

   taille_entete = ( *nb_col > 128 )? (*nb_col) : 128;
   fic = fopen(nom_fic,"w");
   if (fic == NULL)
   {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic);
     goto ERREUR;
   }
   ptr = (unsigned char *) malloc ( taille_entete );
   if ( ptr == NULL )
   {
  	strcpy ( CAI_ERREUR , "Erreur : probleme d'allocation MAPSAT\n" );
	goto ERREUR;
    }
   if (*nb_can == 1) 
   {
	memcpy ( ptr , "IMON" , 4 );
   }
   else
   {
	memcpy ( ptr , "ITRI" , 4 );
   }
   taille_buff_label = ((int)(strlen(label))+1 > 65 )? 
			 (int)(strlen(label))+1 : 65;
   buff_label = (char *) malloc ( taille_buff_label*sizeof(char *) );
   strcpy( buff_label , label );
   for ( i = strlen(label) ; i < 64 ; i++ )
   {
	buff_label[i]=' ';
   }
   buff_label[64]='\0';

   memcpy ( &ptr[4] , buff_label , 64 );
   memcpy ( &ptr[68] , nb_lig , 4 );
   memcpy ( &ptr[72] , nb_col , 4 );
   memcpy ( &ptr[76] , nb_can , 4 );
   pas_echantil = 0;
   memcpy ( &ptr[80] , &pas_echantil , 4 );
   for ( i = 84 ; i < taille_entete - 1 ; i++ )
   {
	ptr[i] = 0;
   }

   retour = fwrite ( ptr , taille_entete , 1 , fic );
  if ( retour == 0 )
  {
    sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n",
	nom_fic );
    goto ERREUR;
  }

  fclose ( fic );
  free (ptr);

/*.................................................
   Ouverture du fichier pour ecriture radiometrie
  ...................................................*/
   num = open(nom_fic,O_RDWR,0);
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
  Fin de l'operation cai_ouvre_creation_mapsat
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_mapsat                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image mapsat               */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_mapsat(image1, canal,     */       		      /* ------------------                               premiere_ligne,          */
/*                                           	    premiere_colonne,        */
/*                                           	    nombre_lignes_lire ,     */
/*                                          	    nombre_colonnes_lire,    */
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
/*     cai_lecture_canal_mapsat (S) (CAI_OK_KO) :  = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

 CAI_OK_KO cai_lecture_canal_mapsat(image1 ,  
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

 int   num,i,j,n;           /* variables de travail                          */
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
   lig=*premiere_ligne;
   i=1;
   pch=data_image;

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
            no_oct=image1->POS_OCTET[*canal-1]+(lig-1)*image1->NB_COL;

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
             strcpy(CAI_ERREUR,"Erreur lecture donnee image mapsat");
            }
            else
            { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/           
             deb=buf_lec+(*premiere_colonne)-1;
             j=1;
             while (j <= *nombre_colonnes_lire)
	     {                         
              *pch=*deb;
              pch=pch+1;
              j=j+1;
              deb=deb+(*pas_colonne);
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
  Fin de l'operation cai_lecture_canal_mapsat
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_mapsat                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image mapsat               */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_mapsat (image1, canal,   */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_mapsat     (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_ecriture_canal_mapsat (image1 ,  
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

long	position_first_lig;	/* position de la 1ere ligne a ecrire	 */
long	position_courante;	/* position courante dans le fichier     */
long	deplacement;		/* deplacement a effectuer 		 */
int	i_ligne;		/* compteur de lignes 			 */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire  */
int	cr_write;		/* code retour de write			 */
int num;                        /* descripteur de fichier                */
int nboct_pix;			/* nombre d'octets par pixel             */

/*...............
  Initialisations 
 ................*/

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
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

position_first_lig = (long) ( image1->POS_OCTET[*canal-1] +
	             (*canal - 1) * image1->NB_LIG* (*nombre_colonnes_ecrire)
		     + ((*premiere_ligne) - 1) * (*nombre_colonnes_ecrire) );


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
while (i_ligne < *nombre_lignes_ecrire)
{

/********************************************************/
/*	extraction de la ligne du tableau 		*/
/********************************************************/

	buff_lig = data_image + (*nombre_colonnes_ecrire) * i_ligne;

/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/

	cr_write = write ( num , buff_lig , (*nombre_colonnes_ecrire) );
	if ( cr_write != (*nombre_colonnes_ecrire) )
	{
		strcpy ( CAI_ERREUR , "Erreur ecriture canal image mapsat\n" );
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


return (CAI_OK);

ERREUR :

return (CAI_KO);


}

/* 
  Fin de l'operation cai_ecriture_canal_mapsat
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_mapsat                                           */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format mapsat et de        */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_mapsat (image1)                   */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_mapsat     (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_mapsat (image1)

CAI_IMAGE *image1;

{
 int iret;    /* code retour de l'operation      */
 int num;     /* descripteur du fichier a fermer */

 iret=CAI_OK;
/*.............................................
   Fermeture de tous les canaux qui sont tous contenus
   dans un seul fichier .im ou .it suivant le nb de canaux
  .............................................*/


   num=image1->ACCES_CANAL_INT[0];
   close(num);


  return (iret);
}

/*
  Fin de l'operation cai_ferme_mapsat
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_mapsat                                        */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en mapsat et de      */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_mapsat (format,nom_ima)        */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_mapsat  (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_dest_ima_mapsat ( repertoire , 
                          	nom_ima )

char	*repertoire;		/* repertoire de stockage de l'image */
char	*nom_ima;		/* nom de l'image */

{
int     iret;                   /* code retour fonction fermetrue lecture */
char	nom_fic[1024];		/* nom du fichier entete a detruire */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image */

CAI_IMAGE   *image1;             /* structure ouverture image */
int         nb_can;	        /* nombre de canaux de l'image */
int         nboct_pix;		/* nombre d'octets par pixel image */
int         nb_col;	        /* nombre de colonnes de l'image */
int         nb_lig;             /* nombre de lignes de l'image */

/************************************************/
/* Constitution du nom du fichier 		*/
/************************************************/
if (strlen(repertoire)!= 0)
{
  strcpy ( nom_ima_complet , repertoire );
  strcat ( nom_ima_complet, "/" );
  strcat ( nom_ima_complet , nom_ima );
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}

/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_mapsat			*/
/************************************************/

  image1 = cai_ouvre_lecture_mapsat (repertoire, nom_ima, &nb_can ,&nboct_pix ,
				      &nb_col , &nb_lig);
  if ( image1 == NULL) goto ERREUR;

  image1->NB_CANAUX=nb_can;
  image1->NB_COL=nb_col;
  image1->NB_LIG=nb_lig;
  image1->NBOCT_PIX=nboct_pix;
 
  iret=cai_ferme_mapsat(image1);

/************************************************/
/* constitution du fichier + extension en       */
/* fonction du  nombre de canaux                */
/************************************************/
  nom_fic[0]='\0';
  strcpy(nom_fic,nom_ima_complet);
  if (nb_can == 1) 
   {
     strcat ( nom_fic , ".im" );
     nom_fic[strlen(nom_ima_complet)+3]='\0';
   }
   else 
   {
     strcat ( nom_fic , ".it" );
     nom_fic[strlen(nom_ima_complet)+3]='\0';
   }


/************************************************/
/* Destruction du fichier image			*/
/************************************************/

  unlink ( nom_fic );


  return (CAI_OK);

ERREUR:

return (CAI_KO);


}
/*
 Fin de l'operation cai_dest_ima_mapsat
*/
