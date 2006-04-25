/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_inria.c                                                   */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format inria                                      */
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
/* MOD : VERSION : 3.3 : DM:optimisation des acces                           */
/*****************************************************************************/
/* MOD : VERSION : 4.0 : DM: integration du module  ouverture en modification*/
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA: Rajout des tests sur les retours d'allocation    */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : verification que l'image est codee sur un ou 2 */
/*octets par pixel (contrainte du format)                                    */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : modifications  open binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA: Rajout des tests sur les retours d'allocation   */
/*			     memoire                                         */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA:  Correction  "PIXSIZE= %d bits\n"  au lieu de   */
/*			       "PIXSIZE= bits\n" dans ouvre_creation         */
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
#include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <malloc.h>

#define _CAI_IMAGE
#define IN_CAI_INRIA

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_inria.c,v 1.13 01/05/31 10:47:04 cmf Exp $";

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
/* OPERATION :    cai_liste_ima_inria                                        */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format inria                  */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_inria(repert,tab_ima)           */
/* -----------------                                                         */
/*                                                                           */
/*     repert         (E) (char *)   : repertoire a explorer     	     */
/*     tab_ima        (S) (char ***) : pointeur sur tableau  images listees  */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_inria     (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



long int cai_liste_ima_inria(char *repert,
		       char ***tab_ima)

{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */

struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char *pch2;          /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images inria identifiees           */
   int i;		/* indice de boucle                            */
   char nom_prec[100];  /* contient le nom du dernier fichier inria vu */
   char pch3[100];      /* permet le transfert du nom de fichier trouve*/

/*........................................
   initialisation et ouverture du repertoire
  .........................................*/

   nb_ima=0;

   dirp=opendir(repert);

   if(dirp!=NULL)
   {


/*......................................
   premiere passe pour comptabiliser les images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE INRIA
      (EXTENSION =.xs)
     ..........................................*/

    strcpy(nom_prec,"11");

    while((dp=readdir(dirp))!=NULL)
    {
     car='.';
     pch0=strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='x';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='s';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
           {
              *pch0='\0';
              *pch1=' ';
	      *pch2=' ';
              pch2=pch2+1;
	      *pch2=' ';
              pch2=pch2+1;
              *pch2=' ';
	      pch2=pch2+1;
	      *pch2='\0';
              strcpy(pch3,dp->d_name);
              i=strcmp(nom_prec,pch3);
              if (i!=0)
              {
                 strcpy(nom_prec,pch3);
	         nb_ima=nb_ima+1;

              }
           }
         
       }
     }
   }

    (void)closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT INRIA
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE INRIA
      (EXTENSION =.xs)
      SI C'EST LE CAS STOCKAGE DE L'IMAGE DANS LA LISTE DES IMAGES INRIA
   ..........................................*/
  dirp=opendir(repert);

   nb_ima=0;

    strcpy(nom_prec,"11");

    while((dp=readdir(dirp))!=NULL)
    {
     car='.';
     pch0=strrchr(dp->d_name,car);
     if (pch0 != NULL)
     {
       car='x';
       pch1=strrchr(dp->d_name,car);
       if ((pch1 != NULL)&&(pch1==pch0+1))
       {
         car='s';
         pch2=strrchr(dp->d_name,car);
         if ((pch2 != NULL)&&(pch2==pch1+1))
           {
              *pch0='\0';
              *pch1=' ';
	      *pch2=' ';
              pch2=pch2+1;
	      *pch2=' ';
              pch2=pch2+1;
              *pch2=' ';
	      pch2=pch2+1;
	      *pch2='\0';
              strcpy(pch3,dp->d_name);
              i=strcmp(nom_prec,pch3);
              if (i!=0)
              {
                 strcpy(nom_prec,pch3);
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
     nb_ima=-1;
   }

    return(nb_ima);

}
/* Fin de l'operation cai_liste_ima_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_inria                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format inria     */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_lecture_inria (nom_image,       */
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
/*     cai_ouvre_lecture_inria   (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_inria(char *repert,
				   char *nom_image,
				   int  *nb_can,
				   int  *nboct_pix,
				   int  *nb_col,
				   int  *nb_lig)
{
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   long  iret,k,n;		/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];	/* nom du fichier canal a ouvrir	    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[3];			/* indice du canal traite     		    */
   int i,num;	                /* indice 				    */
   char *buf_lec ;              /* buffer lecture entete inria              */
   int nb_bits;		        /* nombre de bits par pixels                */
   char str_no;                 /* variable de lecture                      */
   short int n1;		/* variable de travail de lecture           */


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
/* MOD : VERSION : 4.6: FA Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_lecture");
    }
   else
   {

/*.......................................
  INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
  ET LE NOM UTILISATEUR DE CHAQUE CANAL et ouverture
  de ces fichiers
  Recherche du nombre de canaux de l'image
  .....................................................*/
  
  deb = image1->NOM_FIC_CAN;
  deb1= image1->NOM_UTIL_CAN;

   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.xs",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.xs",nom_image);
   }

   for (i=0;i<50;i=i+1)
   {
         nom_fic_canal[0]='\0';
         sscanf(nom_fic,"%s",nom_fic_canal);
         sprintf(car,"%d",i);
         strcat(nom_fic_canal,car);   
         num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);

         if (num != -1)
         {

          if (*nb_can<CAI_NB_MAX_CAN)
          {
          sscanf(nom_image,"%s",deb);
          strcat(deb,".xs");
          strcat(deb,car);
          deb=deb+100;
          sscanf(nom_image,"%s",deb1);
          strcat(deb1,".xs");
          strcat(deb1,car);
          deb1=deb1+100;
          image1->ACCES_CANAL_INT[*nb_can]=num;
	  *nb_can=*nb_can+1;


          }
        
         }
     }
     if ((*nb_can) == 0)
     {
        iret = 0;
     }
     else
     {
/*...................................
     Lecture du fichier entete
  ............................................*/

      buf_lec=(char *) calloc(256,sizeof(char));
      if (buf_lec==NULL) 
      {
        iret=0;
        strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
      }
      else
      {

     	num=image1->ACCES_CANAL_INT[0];

     	k=read(num,buf_lec,256);
     	if (k!=256)
     	{
       		iret=0;
       		strcpy(CAI_ERREUR,"Erreur lecture entete Inria");
    	 }
    	 else
     	 {
/*.................................
       Decodage de l'enregistrement (nouveu format inria)
  .........................................*/
        	if (buf_lec[10]=='4')
        	{

              		sscanf(&buf_lec[19],"%4d",nb_col);
              		sscanf(&buf_lec[29],"%4d",nb_lig);
	      		sscanf(&buf_lec[78],"%2d",&nb_bits);

	      		/* le nombre recupere represente le nombre de */
	      		/* bits ou le nombre d'octets codant un pixel */
	      		/* cette information est transmise via le     */
	      		/* parametre nboct_pix inutilise sur SUN   */
	      		if ( (nb_bits==8) || (nb_bits==16) )
	      			*nboct_pix = nb_bits/8;
	      		else if ( (nb_bits==1) || (nb_bits==2) )
				*nboct_pix =  nb_bits;
              		image1->NB_BITS_PIX= nb_bits;
              		for (i=0;i<*nb_can;i=i+1)
              		{
                   		image1->POS_OCTET[i]=1024; 
               		}
          	}                        
         	else
         	{
/*.................................
       Decodage de l'enregistrement (ancien format inria)
  .........................................*/

                 	lseek(num,0L,0);
                 	n=read(num,(char*)&n1,2);
                 	if (n<=0)
	          	{
        	    		 iret=0;
          	    		 strcpy(CAI_ERREUR,
					"Erreur lecture entete inria");
        	  	}
                 	*nb_col=(int)n1;

                 	n=read(num,(char*)&n1,2);
                 	if (n<=0)
	          	{
        	    		iret=0;
          	     		strcpy(CAI_ERREUR,
					"Erreur lecture entete inria");
        	  	}
                 	*nb_lig=(int)n1;

                 	n=read(num,&str_no,1);
                 	if (n<=0)
	          	{
        	     		iret=0;
          	     		strcpy(CAI_ERREUR,
					"Erreur lecture entete inria");
        	  	}
		  	nb_bits = (int)str_no;

	      		/* le nombre recupere represente le nombre de */
	      		/* bits ou le nombre d'octets codant un pixel */
	      		/* cette information est transmise via le     */
	      		/* parametre nboct_pix inutilise sur SUN   */
	      		if ( (nb_bits==8) || (nb_bits==16) )
	      			*nboct_pix = nb_bits/8;
	      		else if ( (nb_bits==1) || (nb_bits==2) )
				*nboct_pix = nb_bits;
              
              		for (i=0;i<*nb_can;i=i+1)
              		{
                  		 image1->POS_OCTET[i]=256;  
              		} 
        	}   
        	if (*nboct_pix ==1)
			sprintf(image1->COD_PIX,"OCT");
        	else
			sprintf(image1->COD_PIX,"I2");      
      	 }
      	free(buf_lec);        
      }
     }
  }
  if (iret == 0)
  { 
	return (NULL);
   }
   else
   {
	return (image1);
   }

}

/* 
   Fin de l'operation cai_ouvre_lecture_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_inria                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format inria et de tous les fichiers qui la composent        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_inria  (nom_image,       */
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
/*     cai_ouvre_creation_inria   (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_IMAGE *cai_ouvre_creation_inria(char *repert,
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
   char car[2];			/* indice du canal traite     		    */
   int i,num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   int i_oct,remplissage;       /* variables de travail                     */

/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : verification que l'image est codee sur un ou 2 */
/*octets par pixel (contrainte du format)                                    */
/*****************************************************************************/
   if ((*nboct_pix != 1)&&(*nboct_pix != 2))
   {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
	 "Une image INRIA ne peutetre codee que sur un ou 2 octets par pixel");
	goto ERREUR;
   }

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   image1 = NULL;
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

/*************************************************/
/* constitution du nom des fichiers a ecrire     */
/* - si un canal : image.xs0			 */
/* - si +sieurs canaux : image.xsi avec i=1->99	 */
/* et insertion dans les tableaux                */
/*************************************************/
	
/*************************************************/
/* SI l'image est panchromatique		 */
/*************************************************/
  if (* nb_can == 1 )
  {
/*************************************************/
/*	constitution du nom du fichier avec	 */
/*	l'extension .xs0 verif existence	 */
/*************************************************/

   nom_fic_canal[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic_canal,"%s/%s.xs0",repert,nom_image);
   }
   else
   {
       sprintf(nom_fic_canal,"%s.xs0",nom_image);
   }

   retour = stat ( nom_fic_canal , &buf_stat );
   if ( retour == 0 )
    { 
	sprintf ( CAI_ERREUR , "L'image %s INRIA existe \n" , nom_image );
	goto ERREUR;
    }

/*************************************************/
/*	ecriture de l'entete dans le fichier     */
/*	l'extension .xs0			 */
/*************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
   fic = fopen(nom_fic_canal,"wb");
   if (fic == NULL)
    {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
     goto ERREUR;
   }
   retour= fprintf ( fic , "#INRIMAGE-4#{\n" );
   if ( retour == 0 )
   {
	sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n",
	nom_fic_canal );
	goto ERREUR;
   }

   fprintf ( fic , "XDIM=%-4d\n" , *nb_col);
   fprintf ( fic , "YDIM=%-4d\n" , *nb_lig );
   fprintf ( fic , "ZDIM=%-2d\n" , 1 );
   fprintf ( fic , "VDIM=%-2d\n" , 1 );
   fprintf ( fic , "TYPE=unsigned fixed\n" );
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA:  Correction  "PIXSIZE= %d bits\n"  au lieu de   */
/*			       "PIXSIZE= bits\n" dans ouvre_creation         */
/*****************************************************************************/

   fprintf ( fic , "PIXSIZE= %d bits\n" , (*nboct_pix)*8 );
   if (*nboct_pix == 1)
   {
	fprintf ( fic , "SCALE=1**0\n" );
	remplissage = 916;
    }
   else
   {
	fprintf ( fic , "SCALE=2**16\n" );
	remplissage = 915;
    }
    fprintf ( fic , "CPU=sun\n" );
    for ( i_oct = 0 ; i_oct < remplissage ; i_oct++ )
    {
	fprintf ( fic , "\n" );
    }
    fprintf ( fic , "##}\n" );
    if (fic!=NULL) fclose(fic);

/*************************************************/
/*  ouverture pour ecriture radiometrie	         */
/*************************************************/
  
   num = open(nom_fic_canal,O_RDWR|O_BINARY,0);
   if (num == -1)
   {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
     goto ERREUR;
   }
   image1->ACCES_CANAL_INT[0]=num;
   image1->POS_OCTET[0]=1024;
  }

/*************************************************/
/* SINON 					 */
/*************************************************/
  else
  {  
/*************************************************/
/*	POUR chacun des canaux			 */
/*************************************************/
   for ( i= 0 ; i < *nb_can ; i=i+1)
   {
/**************************************************/
/*  constitution du nom du fichier extension .xsi */
/**************************************************/
    nom_fic[0]='\0';
    if ((long int)strlen(repert)!=0)
    {
       sprintf(nom_fic_canal,"%s/%s.xs%d",repert,nom_image,i+1);
    }
   else
   {
       sprintf(nom_fic_canal,"%s.xs%d",nom_image,i+1);
   }

    retour = stat ( nom_fic_canal , &buf_stat );
    if ( retour == 0 )
    { 
	sprintf ( CAI_ERREUR , "L'image %s INRIA existe \n" , nom_image );
	goto ERREUR;
    }
    
/*************************************************/
/*	ecriture de l'entete dans le fichier     */
/*	l'extension .xsi+1			 */
/*************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
    fic = fopen(nom_fic_canal,"wb");
    if (fic == NULL)
    {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
     goto ERREUR;
    }
    retour= fprintf ( fic , "#INRIMAGE-4#{\n" );
    if ( retour == 0 )
    {
	sprintf ( CAI_ERREUR , "Erreur : ecriture impossible dans fichier %s\n",
	nom_fic_canal );
	goto ERREUR;
    }

    fprintf ( fic , "XDIM=%-4d\n" , *nb_col);
    fprintf ( fic , "YDIM=%-4d\n" , *nb_lig );
    fprintf ( fic , "ZDIM=%-2d\n" , 1 );
    fprintf ( fic , "VDIM=%-2d\n" , 1 );
    fprintf ( fic , "TYPE=unsigned fixed\n" );
    fprintf ( fic , "PIXSIZE=%d bits\n" , (*nboct_pix)*8 );
    if (*nboct_pix == 1)
    {
	fprintf ( fic , "SCALE=1**0\n" );
	remplissage = 916;
     }
     else
     {
	fprintf ( fic , "SCALE=2**16\n" );
	remplissage = 915;
     }
     fprintf ( fic , "CPU=sun\n" );
     for ( i_oct = 0 ; i_oct < remplissage ; i_oct++ )
     {
	fprintf ( fic , "\n" );
     }
     fprintf ( fic , "##}\n" );
     fclose(fic);

/*************************************************/
/*  ouverture pour ecriture radiometrie	         */
/*************************************************/
    num = open(nom_fic_canal,O_RDWR|O_BINARY,0);
    if (num == -1)
   {
     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
     goto ERREUR;
   }
   image1->ACCES_CANAL_INT[i]=num;
   image1->POS_OCTET[i]=1024;
  }
 }

  return (image1);

ERREUR :
  if (image1 !=NULL) free(image1);
  return(NULL);
}
/* 
  Fin de l'operation cai_ouvre_creation_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_inria                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image inria                */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_inria(image1, canal,      */       		      /* ------------------                              premiere_ligne,           */
/*                                                 premiere_colonne,         */
/*                                                 nombre_lignes_lire ,      */
/*                                                 nombre_colonnes_lire,     */
/*                                                 pas_ligne ,pas_colonne,   */
/*                 		                   data_image )              */
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
/*     cai_lecture_canal_inria   (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_lecture_canal_inria(CAI_IMAGE *image1 ,  
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
 long no_oct;               /* numero du premier octet a lire                */
 long iret,k;               /* indicateur d'erreur                           */
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

/*.......................................*/
/* verification si lecture image entiere */
/*.......................................*/
  if ((*nombre_lignes_lire==image1->NB_LIG)&&(*nombre_colonnes_lire==image1->NB_COL)
       &&(*pas_ligne==1)&&(*pas_colonne==1))
  {
       oct_lig=oct_pix*(image1->NB_COL)*(image1->NB_LIG);

    /* positionnement en debut de fichier */
       pt_oct=(long int)lseek(num,0L,0);

   /* lecture du canal complet */
      n=read(num,data_image,oct_lig);
      if (n<=0)
      {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image INRIA");
       }
        else
             iret=CAI_OK;
   }
  else
  {

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
        while ((i <= *nombre_lignes_lire)&&(iret==CAI_OK))
        {  
/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
            pt_oct=lseek(num,0L,1);

/*................................
            Calcul du numero du 1er octet a lire
  ....................................*/
            no_oct=image1->POS_OCTET[*canal-1]+((lig-1)*oct_lig);

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
             strcpy(CAI_ERREUR,"Erreur lecture donnee image INRIA");
             }
            else
            { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/           
             deb=buf_lec+((*premiere_colonne)-1)*oct_pix;
             j=1;
             while (j <= *nombre_colonnes_lire)
	     {  
               for (k=0;k<oct_pix;k=k+1)
                {
                  *(pch+k)=*(deb+k);
                 }
              j=j+1;
              deb=deb+((*pas_colonne)*oct_pix);
	      pch=pch+oct_pix;
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
  Fin de l'operation cai_lecture_canal_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_inria                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image inria                */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    CAI_OK_KO cai_ecriture_canal_inria(image1, canal,   */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_inria      (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ecriture_canal_inria (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image )    
{
long	position_first_lig;	/* position de la 1ere ligne a ecrire    */
long	position_courante;	/* position courante dans le fichier     */
long	deplacement;		/* deplacement a effectuer		 */
int	i_ligne;		/* compteur de lignes 			 */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire  */
long	cr_write;		/* code retour de write 		 */
int num;                        /* descripteur de fichier		 */
int nboct_pix;			/* nombre d'octets par pixel             */
int tot_pix;                    /* nb de pixels total a ecrire           */

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
      if (cr_write != nboct_pix)
      {
              strcpy(CAI_ERREUR,"Erreur ecriture donnee image INRIA");
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
               ((*premiere_ligne) - 1) *(*nombre_colonnes_ecrire) * nboct_pix );


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

	buff_lig = data_image +(*nombre_colonnes_ecrire) * nboct_pix * i_ligne;

/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/

	cr_write = write ( num , buff_lig ,(*nombre_colonnes_ecrire) * nboct_pix );
	if ( cr_write !=(*nombre_colonnes_ecrire) * nboct_pix )
	{
		strcpy ( CAI_ERREUR , "Erreur ecriture canal image inria\n" );
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
  Fin de l'operation cai_ecriture_canal_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_inria                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format inria et de         */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_inria(image1)                     */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_inria      (S) (CAI_OK_KO) :   = CA_KO si pb                */
/*                                              = CAI_OK si OK               */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_ferme_inria (CAI_IMAGE *image1)


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
  Fin de l'operation cai_ferme_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_inria                                         */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en inria et de       */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_inria(format,nom_ima)          */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_inria   (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO  cai_dest_ima_inria ( char *repertoire , 
                       		char *nom_ima )

{
int	i_can;			/* compteur de canaux */
long     iret;                  /* code retour fonction fermetrue lecture */
char	str_num_canal[3];	/* suffixe numero du canal */
char	nom_fic[1024];		/* nom du fichier entete a detruire */
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
if ((long int)strlen(repertoire)!= 0)
{
  sprintf ( nom_ima_complet, "%s/%s",repertoire,nom_ima );

}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}
sprintf(nom_fic ,"%s.xs", nom_ima_complet);

/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_inria			*/
/************************************************/
  image1 = cai_ouvre_lecture_inria ( repertoire,nom_ima, &nb_can ,&nboct_pix ,
				     &nb_col , &nb_lig );	
  if ( image1 == NULL) goto ERREUR;

  image1->NB_CANAUX=nb_can;
  image1->NB_COL=nb_col;
  image1->NB_LIG=nb_lig;
  image1->NBOCT_PIX=nboct_pix;
 
  iret=cai_ferme_inria(image1);

  image1 = NULL;

/************************************************/
/* Recherche de tous les fichiers au nom 	*/
/* de l'image comportant une extension  	*/
/*.xsi avec i = 0 -> nb_can et destruction	*/
/************************************************/
	
for ( i_can = 0 ; i_can < nb_can+1 ; i_can++ )
{
	nom_fic_canal[0] = '\0';
	sscanf ( nom_fic , "%s" , nom_fic_canal );
	sprintf ( str_num_canal , "%d" , i_can );
	strcat ( nom_fic_canal , str_num_canal );
	nom_fic_canal[strlen(nom_ima_complet)+3+strlen(str_num_canal)]='\0';

	cr_stat = stat ( nom_fic_canal , &buf_stat );
	if ( cr_stat == 0 )
	{
		/* le fichier existe => destruction */
		unlink ( nom_fic_canal );
	}
}



return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/*
 Fin de l'operation cai_dest_ima_inria
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_inria                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     inria et de tous les fichiers qui la composent                  */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :   CAI_IMAGE *cai_ouvre_modifie_inria (nom_image,       */
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
/*     cai_ouvre_modifie_inria   (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_modifie_inria(char *repert,
				   char *nom_image,
				   int  *nb_can,
				   int  *nboct_pix,
				   int  *nb_col,
				   int  *nb_lig)
{
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   int  iret;			/* indicateur de retour ok ou pas           */
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];	/* nom du fichier canal a ouvrir	    */
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   char car[3];			/* indice du canal traite     		    */
   int i,num;	                /* indice 				    */
   char *buf_lec ;              /* buffer lecture entete inria              */
   int nb_bits;		        /* nombre de bits par pixels                */
   char str_no;                 /* variable de lecture                      */
   short int n1;		/* variable de travail de lecture           */
   long k,n;


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
/* MOD : VERSION : 4.1: FA Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     iret=0;
     strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_modifie");
    }
   else
   {
/*.......................................
  INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
  ET LE NOM UTILISATEUR DE CHAQUE CANAL et ouverture
  de ces fichiers
  Recherche du nombre de canaux de l'image
  .....................................................*/
  
  deb = image1->NOM_FIC_CAN;
  deb1= image1->NOM_UTIL_CAN;

   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
       sprintf(nom_fic,"%s/%s.xs",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.xs",nom_image);
   }

   for (i=0;i<50;i=i+1)
   {
         nom_fic_canal[0]='\0';
         sscanf(nom_fic,"%s",nom_fic_canal);
         sprintf(car,"%d",i);
         strcat(nom_fic_canal,car);   
         num=open(nom_fic_canal,O_RDWR|O_BINARY,0);


         if (num != -1)
         {

          if (*nb_can<100)
          {
          sscanf(nom_image,"%s",deb);
          strcat(deb,".xs");
          strcat(deb,car);
          deb=deb+100;


          sscanf(nom_image,"%s",deb1);
          strcat(deb1,".xs");
          strcat(deb1,car);
          deb1=deb1+100;
          image1->ACCES_CANAL_INT[*nb_can]=num;
	  *nb_can=*nb_can+1;
          }
        
         }
     }

     if ((*nb_can) == 0)
     {
        iret = 0;
     }
     else
     {
/*...................................
     Lecture du fichier entete
  ............................................*/

      buf_lec=(char *) calloc(256,sizeof(char));
      if (buf_lec==NULL) 
      {
        iret=0;
        strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_modifie");
      }
      else
      {

     	num=image1->ACCES_CANAL_INT[0];

     	k=read(num,buf_lec,256);
     	if (k!=256)
     	{
       		iret=0;
       		strcpy(CAI_ERREUR,"Erreur lecture entete Inria");
    	 }
    	 else
     	 {

/*.................................
       Decodage de l'enregistrement (nouveu format inria)
  .........................................*/
        	if (buf_lec[10]=='4')
        	{

              		sscanf(&buf_lec[19],"%4d",nb_col);
              		sscanf(&buf_lec[29],"%4d",nb_lig);
	      		sscanf(&buf_lec[78],"%2d",&nb_bits);

	      		/* le nombre recupere represente le nombre de */
	      		/* bits ou le nombre d'octets codant un pixel */
	      		/* cette information est transmise via le     */
	      		/* parametre nboct_pix inutilise sur SUN   */
	      		if ( (nb_bits==8) || (nb_bits==16) )
	      			*nboct_pix = nb_bits/8;
	      		else if ( (nb_bits==1) || (nb_bits==2) )
				*nboct_pix =  nb_bits;
              		image1->NB_BITS_PIX= nb_bits;
              		for (i=0;i<*nb_can;i=i+1)
              		{
                   		image1->POS_OCTET[i]=1024; 
               		}
          	}                        
         	else
         	{
/*.................................
       Decodage de l'enregistrement (ancien format inria)
  .........................................*/

                 	lseek(num,0L,0);
                 	n=read(num,(char*)&n1,2);
                 	if (n<=0)
	          	{
        	    		 iret=0;
          	    		 strcpy(CAI_ERREUR,
					"Erreur lecture entete inria");
        	  	}
                 	*nb_col=(int)n1;

                 	n=read(num,(char*)&n1,2);
                 	if (n<=0)
	          	{
        	    		iret=0;
          	     		strcpy(CAI_ERREUR,
					"Erreur lecture entete inria");
        	  	}
                 	*nb_lig=(int)n1;

                 	n=read(num,&str_no,1);
                 	if (n<=0)
	          	{
        	     		iret=0;
          	     		strcpy(CAI_ERREUR,
					"Erreur lecture entete inria");
        	  	}
		  	nb_bits = (int)str_no;

	      		/* le nombre recupere represente le nombre de */
	      		/* bits ou le nombre d'octets codant un pixel */
	      		/* cette information est transmise via le     */
	      		/* parametre nboct_pix inutilise sur SUN   */
	      		if ( (nb_bits==8) || (nb_bits==16) )
	      			*nboct_pix = nb_bits/8;
	      		else if ( (nb_bits==1) || (nb_bits==2) )
				*nboct_pix = nb_bits;
              
              		for (i=0;i<*nb_can;i=i+1)
              		{
                  		 image1->POS_OCTET[i]=256;  
              		} 
        	}   
        	if (*nboct_pix ==1)
			sprintf(image1->COD_PIX,"OCT");
        	else
			sprintf(image1->COD_PIX,"I2");      
      	 }
      	free(buf_lec);        
      }
     }
  }

  if (iret == 0)
  { 
	return (NULL);
  }
  else
  {
	return (image1);
  }

}
/* 
   Fin de l'operation cai_ouvre_modifie_inria
*/

