/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_bip.c                                                     */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format bip                                        */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    Mai 1996                                            */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V2                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 3.3 : DM : optimisation des acces                         */
/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA:Rajout des tests sur les retours d'allocation    */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : FA : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
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
#include <errno.h>

#define _CAI_IMAGE
#define IN_CAI_BIP

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_bip.c,v 1.11 01/05/31 10:46:56 cmf Exp $";


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
/* OPERATION :    cai_liste_ima_bip                                          */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format bip                    */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_bip(repert,tab_ima)             */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_bip       (S) (int)     : = -1 repertoire inexistant    */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


long int cai_liste_ima_bip (char *repert,
		           char ***tab_ima)

{

   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */ 
   int nb_ima;          /* nombre d'images bip identifiees             */
   int i;               /* indice de boucle                            */
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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE BIP_CNES
      (EXTENSION =.bip)
   ..........................................*/
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".bip");
     if (pch0 != NULL)
     {
        ll = (long int)strlen(dp->d_name);
        dif = (long int)(dp->d_name-pch0);
        dif=labs(dif);
        pch1=pch0+4;
        if (((ll-4)==dif)||((*pch1)==blanc))
	     nb_ima=nb_ima+1;
     
     }
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
	 else
         {
    		pch0=strstr(dp->d_name,".BIP");
     	        if (pch0 != NULL)
     	        {
        	    ll = strlen(dp->d_name);
        	    dif = (long int)(dp->d_name-pch0);
        	    dif=labs(dif);
        	    pch1=pch0+4;
        	    if (((ll-4)==dif)||((*pch1)==blanc))
	     		nb_ima=nb_ima+1;
     	        }
    	 }

   }

    (void)closedir(dirp);

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT BIP
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE BIP_CNES
      (EXTENSION =.bip)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".bip");
     if (pch0 != NULL)
     {
        ll = (long int)strlen(dp->d_name);
        dif = (long int)(dp->d_name-pch0);
        dif=labs(dif);
        pch1=pch0+4;
        if (((ll-4)==dif)||((*pch1)==blanc))
        {
            *pch0='\0';
            strcpy((*tab_ima)[nb_ima],dp->d_name);
	    nb_ima=nb_ima+1;
        }
     
     }
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
     else
     {
    	 pch0=strstr(dp->d_name,".BIP");
     	 if (pch0 != NULL)
     	 {
        	    ll = strlen(dp->d_name);
        	    dif = (long int)(dp->d_name-pch0);
        	    dif=labs(dif);
        	    pch1=pch0+4;
        	    if (((ll-4)==dif)||((*pch1)==blanc))
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

/* Fin de l'operation cai_liste_ima_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_bip                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format bip       */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_lecture_bip (nom_image,       */
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
/*     cai_ouvre_lecture_bip     (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_bip(char *repert,
				    char * nom_image,
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
   char *deb,*deb1,*pch0;	/* pointeurs sur chaine de caracteres       */
   int i,num;			/* indice 				    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   char COD_PIX[80];	        /* type de codage du pixel (optionnel)      */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int cmp;			/* retour de comparaison de chaine          */
   int reconnaissance_type;

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1: FA Rajout des tests sur les retours d'allocation     */
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
   if ((long int)strlen(repert)!= 0)
   {
      sprintf(nom_fic,"%s/%s.bip_hd",repert,nom_image);
   }
   else
   {
     sprintf(nom_fic,"%s.bip_hd",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      nom_fic[0]='\0';
      if((long int)strlen(repert)!=0)
      {
          sprintf(nom_fic,"%s/%s.BIP_HD",repert,nom_image);
      }
      else
      {
         sprintf(nom_fic,"%s.BIP_HD",nom_image);
       }
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
         iret=0;
         strcpy(CAI_ERREUR,"Erreur ouverture fichier entete BIP-CNES");
      }
    }
    if (iret !=0)
    {
/*....................................
     Decodage du commentaire
  ......................................*/
     fscanf(fic_in,"%s\n",label);
     retour=strncmp(label,"COMMENT",7);
     if (retour!=0)
     {
	strcpy(CAI_ERREUR,"Informations incoherentes ligne COMMENT");
	iret=0;
      }
     else
     {
        fgets(image1->COMMENT,80,fic_in);


 /*........................................
        Decodage du nombre de canaux
  .........................................*/
        fscanf(fic_in,"%s\n",label);
	retour=strncmp(label,"CHANNELS",8);
	if (retour!=0)
	{
	   strcpy(CAI_ERREUR,"Informations incoherentes ligne CHANNELS");
	   iret=0;
	 }
	else
	{
	   fscanf(fic_in,"%d\n",&nbplan);
           *nb_can=nbplan;

/*.......................................
	   Decodage du nombre de lignes
  ........................................*/
	    fscanf(fic_in,"%s\n",label);
	    retour=strncmp(label,"LINES",5);
	    if (retour!=0)
	    {
	       strcpy(CAI_ERREUR,"Informations incoherentes ligne LINES");
	       iret=0;
	     }
	     else
	     {
	       fscanf(fic_in,"%d\n",nb_lig);


/*.......................................
	  Decodage du nombre de colonnes
  ........................................*/
	       fscanf(fic_in,"%s\n",label);
	       retour=strncmp(label,"COLUMNS",7);
	       if (retour!=0)
	       {
	    	 strcpy(CAI_ERREUR,"Informations incoherentes ligne COLUMNS");
	    	 iret=0;
	  	}
	       else
	       {

		 fscanf(fic_in,"%d\n",nb_col);

/*.......................................
	  Decodage du nombre de bits par pixel
  ........................................*/
	  	 fgets(label,80,fic_in);
	  	 retour=strncmp(label,"BITS PER PIXEL",14);
		 if (retour!=0)
	  	 {
	    	   strcpy(CAI_ERREUR,"Informations incoherentes ligne BITS ");
	    	   iret=0;
	  	  }
	  	 else
	    	 {
		   fscanf(fic_in,"%d\n",&nbbit);

/*........................................................
	  Decodage du type de codage du pixel (optionnel)
  ........................................................*/
     		   fscanf(fic_in,"%s\n",label);
     		   retour=strncmp(label,"TYPE",4);
     		   if (retour==0)
                   {
      		      fscanf(fic_in,"%s\n",COD_PIX);
		      retour=1;
                   }
		   else
		     sprintf(COD_PIX,"UND");
        	   pch0=cai_En_majuscule( COD_PIX );

/* 	Verification si type de codage reconnu */
         	   reconnaissance_type = cai_istyp_connu(pch0);
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
/*****************************************************************************/
		   if (retour !=0) fscanf(fic_in,"%s\n",label);
		   retour=strncmp(label,"SENSCODAGE",10);
	 	   if (retour!=0)
	 	   {
/*      Si absent, pas d'inversion      */    
			image1->INV_OCTETS =0;
			sprintf((char *)image1->SENS_COD,type_machine);
	 	    }
		    else
		    {
  			 fscanf(fic_in,"%s\n",label);
  			 retour=strncmp(label,"IEEE",4);
   			 if (retour==0)	
				sprintf((char *)image1->SENS_COD,"BE");
   			 else
				sprintf((char *)image1->SENS_COD,"LE");
   			 cmp = strncmp(type_machine,image1->SENS_COD,2);
   			 if (cmp!=0)
 	  			image1->INV_OCTETS =1;
   			 else
	 			image1->INV_OCTETS =0;
                     }	 	
		  }
		}
	      }
	  }
      }
    }
/*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in !=NULL) fclose(fic_in);

   if (iret !=0)
   {
/*..................................
     Initialisation du nombre d'octets par pixel
  ........................................................*/
     if ((nbbit%8)==0)
	*nboct_pix=(int)(nbbit/8);
     else
	*nboct_pix=(int)((nbbit/8)+1);
     image1->NB_BITS_PIX=nbbit;

     if (reconnaissance_type != -1)
     {
	    	sprintf(image1->COD_PIX,"%s",COD_PIX);
     }
     else
     {
     	  sprintf(image1->COD_PIX,"UND");

          if (*nboct_pix==1)
              sprintf(image1->COD_PIX,"OCT");
          else if (*nboct_pix==2)
	      sprintf(image1->COD_PIX,"I2");
          else if (*nboct_pix==4)
          {
	     retour=strncmp(COD_PIX,"I4",2);
             if (retour==0)
	        sprintf(image1->COD_PIX,"I4");
             else
             {
	 	retour=strncmp(COD_PIX,"R4",2);
          	if (retour==0)
	    	  sprintf(image1->COD_PIX,"R4");
             }
           }
           else if (*nboct_pix==8)
           {
		sprintf(image1->COD_PIX,"R8");	   
           }
       }

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     if ((long int)strlen(repert)!= 0)
     {
       sprintf(nom_fic_canal,"%s/%s.bip",repert,nom_image);
      }
      else
      {
       sprintf(nom_fic_canal,"%s.bip",nom_image);
      }
/*****************************************************************************/
/* MOD : VERSION : 4.4 : FA : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
     retour = stat ( nom_fic_canal , &buf_stat );
     if ( retour != 0 )
     { 
       if((long int)strlen(repert)!=0)
       {
         sprintf(nom_fic_canal,"%s/%s.BIP",repert,nom_image);
       }
       else
       {
        sprintf(nom_fic_canal,"%s.BIP",nom_image);
       }
     }
 
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;
     num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",deb);
  	   deb=deb+100;
           sscanf(nom_image,"%s",deb1);
  	   deb1=deb1+100;
           image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=0;
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
   Fin de l'operation cai_ouvre_lecture_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_bip                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format bip et de tous les fichiers qui la composent          */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    CAI_IMAGE *cai_ouvre_creation_bip (nom_image,       */
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
/*     cai_ouvre_creation_bip     (S) (CAI_IMAGE *) : =NULL si pb            */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_bip(char *repert,
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
   int i,num,Coher;		/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int reconnaissance_type;

/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1: FA Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_creation");
     goto ERREUR;
   }

/*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if ((long int)strlen(repert)!= 0)
   {
       sprintf(nom_fic,"%s/%s.bip_hd",repert,nom_image);
   }
   else
   {
       sprintf(nom_fic,"%s.bip_hd",nom_image);
   }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s BIP existe \n" , nom_image );
	goto ERREUR;
   }

/*...............................................................
  constitution du nom de fichier correspondant a la radiometrie de l'image
  Verification qu'il n'existe pas deja
  Ouverture du fichier en ecriture
  .................................................................*/

   nom_fic_canal[0]='\0';
   sscanf(nom_fic,"%s",nom_fic_canal);
   nom_fic_canal[strlen(nom_fic_canal)-3]='\0';
   retour = stat ( nom_fic_canal , &buf_stat );

   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s BIP existe \n" , nom_image );
	goto ERREUR;
   }

   num = open(nom_fic_canal,O_RDWR|O_CREAT|O_BINARY,PERMS); 
   if (num == -1)
   {

     sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
     goto ERREUR;
   } 

   for (i=0;i<*nb_can;i=i+1)
   {
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
      sprintf(CAI_ERREUR,"Erreur: Creation fichier %s impossible\n",nom_fic);
      goto ERREUR;
   }

  retour=fprintf ( fic , "COMMENT\n");
  if (retour == 0)
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
   }

  fprintf ( fic , "%s\n" , label );
  fprintf ( fic , "CHANNELS\n" );
  fprintf ( fic , "%3d\n" , *nb_can );
  fprintf ( fic , "LINES\n" );
  fprintf ( fic , "%5d\n" , *nb_lig );
  fprintf ( fic , "COLUMNS\n" );
  fprintf ( fic , "%5d\n" , *nb_col );
  fprintf ( fic , "BITS PER PIXEL\n" );
  fprintf ( fic , "%2d\n" , (*nboct_pix)*8 );
  fprintf ( fic , "TYPE\n");
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  Maj du tag TYPE lorsque l'utilisateur le      */
/*                                  precise dans le param  "label"           */
/*****************************************************************************/
/* Verification si type de codage reconnu dans le label */
  reconnaissance_type = cai_istyp_connu(label);
  if (reconnaissance_type == -1)
  {
 	if (*nboct_pix==1)
    		sprintf ( image1->COD_PIX , "OCT" );
  	else  if (*nboct_pix==2)
   		sprintf ( image1->COD_PIX , "I2" );
  	else if (*nboct_pix==4)
  	{
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM :  Maj du tag TYPE lorsque l'image est codée     */
/* en reel  et que l'utilisateur le precise dans le param  "label"           */
/*****************************************************************************/
     		if ((strcasecmp(label,"R4") == 0)||
			(strcasecmp(label,"FLOAT")== 0)||
				(strcasecmp(label,"REEL") == 0))
       			sprintf ( image1->COD_PIX , "R4" );
     		else
       			sprintf ( image1->COD_PIX , "I4" );     
  	}
  	else if (*nboct_pix==8)
		sprintf ( image1->COD_PIX , "R8" );
        else
   		sprintf ( image1->COD_PIX , "UND" );
    	fprintf ( fic , "%s\n",image1->COD_PIX );
   }
   else
   {
/* Verification de coherence entre le nb d'octets par pixel et le type donne */
        Coher=1;
	if(*nboct_pix == 1) 
	    if (reconnaissance_type > 1) Coher=0;       
	else if(*nboct_pix == 2) 
	    if ((reconnaissance_type != 2) || (reconnaissance_type != 3)) Coher=0;
	else if(*nboct_pix == 4) 
	    if ((reconnaissance_type <4) || (reconnaissance_type > 6)) Coher=0;
	else if(*nboct_pix == 8) 
	    if (reconnaissance_type != 7)  Coher=0;
        else 
	   Coher = 0;
        if (Coher == 0) 
        {      	
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }    
	else
        {
     		sprintf ( image1->COD_PIX , "%s" ,label);
     		fprintf ( fic , "%s\n", label );
        }
   }
  fprintf ( fic , "SENSCODAGE\n" );
  retour=strncmp(type_machine,"BE",2);
  sprintf((char *)image1->SENS_COD,"%s",type_machine);
  if (retour == 0)
     fprintf ( fic , "IEEE\n"  );
  else
     fprintf ( fic , "INTEL\n"  );

/*************************************************/
/* Fermeture du fichier                          */
/*************************************************/

  fclose ( fic );
  return (image1);

ERREUR :

   if (fic !=NULL) cai_dest_ima_bip ( repert ,  nom_image );
   return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_bip                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image bip                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_bip(image1, canal,        */       		      /* ------------------                            premiere_ligne,             */
/*                                               premiere_colonne,           */
/*                                               nombre_lignes_lire ,        */
/*                                               nombre_colonnes_lire,       */
/*                                               pas_ligne ,pas_colonne,     */
/*                 		                data_image )                 */
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
/*     cai_lecture_canal_bip     (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_lecture_canal_bip(CAI_IMAGE *image1 ,  
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
 long pt_oct;		    /* position courante dans le fichier image       */
 long no_oct;               /* numero du premier octet a lire                */
 int iret;                  /* indicateur d'erreur                           */
 int lig;                   /* indice de boucle sur les lignes               */
 int   oct_pix;             /* nb octets par pixels                          */
 long   nb_oct;             /* nb oct =1er octet a lire - position courante  */
 int   oct_lig;             /* nb oct. par ligne en considerant taille pixel */
 int  nbcan;                /* nombre de canaux de l'image                   */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 unsigned char *data_image2;/* pointeur donnees image pour inversion octets  */
 int taille_image,Nb;       /* taille en octets de l'image                   */
 long n;

/*...........................
  Initialisations diverses
  ..........................*/

   iret=CAI_OK;
   num=image1->ACCES_CANAL_INT[(*canal)-1];
   oct_pix=image1->NBOCT_PIX;
   nbcan = image1->NB_CANAUX;
   oct_lig=oct_pix*nbcan*(image1->NB_COL);
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
        while ((i <= (*nombre_lignes_lire))&&(iret==CAI_OK))
        {  

/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
            pt_oct=(long int)lseek(num,0L,1);

/*................................
            Calcul du numero du 1er octet a lire
  ....................................*/
            no_oct=image1->POS_OCTET[(*canal)-1]+
				((lig-1)*oct_lig);
/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
            nb_oct=(long int)(no_oct-pt_oct);
            lseek(num,nb_oct,1);

/*...........................................
            lecture d'une ligne image
  ................................................*/
            n=read(num,buf_lec,oct_lig);
            if (n<=0)
            {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image BIP");
            }
            else
            { 
/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/   
             deb=buf_lec+((*premiere_colonne)-1)*oct_pix*nbcan+
			 ((*canal-1)*oct_pix);
             j=1;
             while (j <= (*nombre_colonnes_lire))
	     {  

               for (k=0;k<oct_pix;k=k+1)
                {
                  *(pch+k)=*(deb+k);
                 }
              pch=pch+oct_pix;
              j=j+1;
              deb=deb+((*pas_colonne)*oct_pix*nbcan);
             }   
          
            }
          i=i+1;
          lig=lig+(*pas_ligne);

        }        

	free(buf_lec);
   }

/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
/*****************************************************************************/

   if ( (iret!= CAI_KO) && (image1->INV_OCTETS==1) && (oct_pix!=1) )
   {
        taille_image = (*nombre_lignes_lire)*oct_pix*(*nombre_colonnes_lire);
        data_image2 = NULL;
        data_image2 = (unsigned char *) calloc(taille_image,
						sizeof(unsigned char));    
        if (data_image2 == NULL)
        {
           strcpy(CAI_ERREUR,"Erreur allocation memoire cai_lecture_canal");
           iret=CAI_KO;
        }
	else
        {
	    /*modification ordre selon nb octets par pixel*/
       	    swab( (void*)(data_image), (void*)(data_image2), taille_image);
		
	   if (oct_pix!=2)
           { 
              Nb = taille_image/2;
	      cai_inverser_tableau(data_image, data_image2,Nb);
	   }
	   else 
	     for (i=0; i<taille_image;i++)
	     {
	        /*stockage dans data_image*/
	        data_image[i] = data_image2[i];
	     }	

           if (data_image2!= NULL) free(data_image2);
        }   	   
    }

   return(iret);
 }
/*
  Fin de l'operation cai_lecture_canal_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_bip                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image bip                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      CAI_OK_KO cai_ecriture_canal_bip(image1, canal,   */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_bip        (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_bip (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image )

{

long	position_first_lig;	/* position de la 1ere ligne a ecrire    */
long	position_courante;	/* position courante dans le fichier     */
long	deplacement;		/* deplacement a effectuer               */
int	i_ligne;		/* compteur de lignes                    */
unsigned char *buff_lig;	/* tableau contenant une ligne a ecrire  */
long	cr_write;		/* code retour de write                  */
int num;                        /* descripteur de fichier                */
int nboct_pix;			/* nombre d'octets par pixel             */
int nbcan;                      /* nombre de canaux de l'image           */
int nboctet;                    /* nombre d'octets d'une ligne multi     */
unsigned char *deb,*pch;	/* pointeur sur chaine pour transfert    */
int i,j,k;			/* indice de boucle sur les colonnes     */
long cr_read;			/* code retour de la fonction read       */

/* initialisations */

i_ligne = 0;
num = image1->ACCES_CANAL_INT[*canal-1];
nboct_pix = image1->NBOCT_PIX;
nbcan = image1->NB_CANAUX;
nboctet = (*nombre_colonnes_ecrire)*nbcan*nboct_pix;

/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

position_first_lig =(long) ( image1->POS_OCTET[(*canal)-1]+ 
			    (( *premiere_ligne - 1) *nboctet));
		

/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

position_courante = lseek ( num , 0L , 1 );


/********************************************************/
/* Deplacement jusqu'a la premiere ligne a ecrire	*/
/********************************************************/

deplacement = position_first_lig - position_courante;
lseek ( num , deplacement , 1 ); 

buff_lig=(unsigned char *)(calloc(nboctet,sizeof(unsigned char)));
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
   cr_read=read(num,buff_lig,nboctet);
   if (cr_read <0)
	   {  
	    strcpy ( CAI_ERREUR , "Erreur ecriture canal image BIP\n" );
	    goto ERREUR;
	    }
   else
	   {
             if (cr_read==0)
  	     {
  		 for (i=0;i<nboctet;i++)
     		   *(buff_lig+i)=0;
                 deplacement=0;
  	     }
             else
		deplacement= - nboctet;
           }
/************************************************************************/
/*    Redeplacement jusqu'au premier pixel de la premiere ligne a ecrire*/
/************************************************************************/
      lseek ( num , deplacement , 1 ); 

/*************************************************************************/
/*   chargement de la ligne entrelacant les pixels par rapport aux canaux*/
/*************************************************************************/

   deb=buff_lig+((*canal)-1)*nboct_pix;
   for (j=0;j<(*nombre_colonnes_ecrire);j=j+1)
   {
        for (k=0;k<nboct_pix;k=k+1)
        {
            *(deb+k)=*(pch+k);
         }
      
	pch=pch+nboct_pix;
	deb=deb + (nboct_pix*nbcan);
   }
/********************************************************/
/*	ecriture de la ligne				*/
/********************************************************/
   cr_write = write ( num , buff_lig , nboctet );
 
   if ( cr_write != nboctet )
   {  
	 strcpy ( CAI_ERREUR , "Erreur ecriture canal image BIP\n" );
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

free(buff_lig);
return (CAI_KO);


}

/* 
  Fin de l'operation cai_ecriture_canal_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_bip                                              */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format bip et de           */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_bip(image1)                       */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_bip        (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_bip (CAI_IMAGE *image1)

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
  Fin de l'operation cai_ferme_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_bip                                           */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en bip et de         */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_bip(repertoire,nom_ima)        */       		      /* ------------------                                                        */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_bip     (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_dest_ima_bip ( char *repertoire , 
                     	     char *nom_ima )

{

char	nom_fic_entete[1024];	/* nom du fichier entete a detruire */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image */

int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier */ 

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/

if ((long int)strlen(repertoire)!= 0)
{

  sprintf ( nom_ima_complet ,"%s/%s", repertoire ,nom_ima);
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}
  sprintf(nom_fic_entete ,"%s.bip_hd", nom_ima_complet);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/

cr_stat = stat ( nom_fic_entete , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_entete );

else
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
{
  sprintf(nom_fic_entete ,"%s.BIP_HD", nom_ima_complet);
  cr_stat = stat ( nom_fic_entete , &buf_stat );
  if ( cr_stat == 0 )
	unlink ( nom_fic_entete );

}
 
/************************************************/
/* Destruction du fichier radiometrie		*/
/************************************************/
  sprintf(nom_fic_entete ,"%s.bip", nom_ima_complet);
  cr_stat = stat ( nom_fic_entete , &buf_stat );
  if ( cr_stat == 0 )
	unlink ( nom_fic_entete );
  else
 {
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/

  sprintf(nom_fic_entete ,"%s.BIP", nom_ima_complet);
  cr_stat = stat ( nom_fic_entete , &buf_stat );
  if ( cr_stat == 0 )
	unlink ( nom_fic_entete );

}



return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_modif_nbbitspix_bip                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    permet de modifier le nombre de bits significatifs par pixel    */
/* -----     dans l'entete de fichier (.hd)				     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_modif_nbbitspix_bip(image1,NbBitsPix)   */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     NbBitsPix   (E) (int)          : nb de bits significatifs par pixel   */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_modif_nbbitspix_bip(CAI_IMAGE *image1,int NbBitsPix)

{
  int Can;		/* indice de boucle sur les canaux */
  char fichierE[1024];  /* fichier a traiter               */
  char Command[1024];   /* Commande a envoyer au systeme   */
  FILE *fic;		/* pointeur descripteur fichier    */
  int pt_oct;		/* position courante ds le fichier */
  int *Ref;             /* Reference du SDS des fichiers   */
  char label[80];       /* contient un enregist  fichier   */
  char repert[1024];    /* repertoire de stockage image    */
  char image[1024];     /* nom de l'image a ouvrir         */
  char car;		/* variable de travail             */
  int icr,tempo,retour; /* code retour fonction            */
  CAI_CHAINE *NomIma;

        
/* Ouverture du fichier d'entete */

   fichierE[0]='\0';
   sprintf(fichierE,"%s.bip_hd",image1->NOM_IMAGE);
   fic = fopen(fichierE,"w");
   if (fic == NULL)
   {
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      fichierE[0]='\0';
      sprintf(fichierE,"%s.BIP_HD",image1->NOM_IMAGE);
      fic = fopen(fichierE,"w");
      if (fic == NULL)
      {
         sprintf(CAI_ERREUR,
	   "Erreur: Modification fichier %s impossible\n",fichierE);
          goto ERREUR;
      }
   }

/* Modification du fichier d'entete */
   rewind(fic);
   fprintf ( fic , "COMMENT\n");
   fprintf ( fic , "%s\n" ,image1->COMMENT);
   fprintf ( fic , "CHANNELS\n" );
   fprintf ( fic , "%3d\n" , image1->NB_CANAUX);
   fprintf ( fic , "LINES\n" );
   fprintf ( fic , "%5d\n" , image1->NB_LIG);
   fprintf ( fic , "COLUMNS\n" );
   fprintf ( fic , "%5d\n" , image1->NB_COL );
   fprintf ( fic , "BITS PER PIXEL\n" );
   fprintf ( fic , "%2d\n" , NbBitsPix );
   fprintf ( fic , "TYPE\n");
   fprintf ( fic , "%s\n",image1->COD_PIX );  
   fprintf ( fic,  "SENSCODAGE\n");
   retour=strncmp((char *)image1->SENS_COD,"BE",2);
   if (retour == 0)
     fprintf ( fic , "IEEE\n"  );
   else
     fprintf ( fic , "INTEL\n"  );


  fclose ( fic );

  return(CAI_OK);

ERREUR :
  return(CAI_KO);
}
/*
 Fin de l'operation cai_modif_nbbitspix_bip
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_bip                                      */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture/ecriture d'une image au format  */
/* -----     bip  et de tous les fichiers qui la composent                   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_modifie_bip (nom_image,       */
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
/*     cai_ouvre_modifie_bip     (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_modifie_bip(char *repert,
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
   char *deb,*deb1,*pch0;	/* pointeurs sur chaine de caracteres       */
   int i,num;			/* indice 				    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   char COD_PIX[80];	        /* type de codage du pixel (optionnel)      */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   char *type_machine;          /* ordre rangement octets machine de travail*/
   int cmp;			/* retour de comparaison de chaine          */
   int reconnaissance_type;

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;

 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/ 
   type_machine = cai_test_pFpf();

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1: FA Rajout des tests sur les retours d'allocation     */
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
   if ((long int)strlen(repert)!= 0)
   {
      sprintf(nom_fic,"%s/%s.bip_hd",repert,nom_image);
   }
   else
   {
     sprintf(nom_fic,"%s.bip_hd",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
      nom_fic[0]='\0';
      if((long int)strlen(repert)!=0)
      {
          sprintf(nom_fic,"%s/%s.BIP_HD",repert,nom_image);
      }
      else
      {
         sprintf(nom_fic,"%s.BIP_HD",nom_image);
       }
      fic_in=fopen(nom_fic,"r");
      if (fic_in==NULL)
      {
         iret=0;
         strcpy(CAI_ERREUR,"Erreur ouverture fichier entete BIP-CNES");
      }
    }
    if (iret !=0)
    {
/*....................................
     Decodage du commentaire
  ......................................*/
     fscanf(fic_in,"%s\n",label);
     retour=strncmp(label,"COMMENT",7);
     if (retour!=0)
     {
	strcpy(CAI_ERREUR,"Informations incoherentes ligne COMMENT");
	iret=0;
      }
     else
     {
        fgets(image1->COMMENT,80,fic_in);


 /*........................................
        Decodage du nombre de canaux
  .........................................*/
        fscanf(fic_in,"%s\n",label);
	retour=strncmp(label,"CHANNELS",8);
	if (retour!=0)
	{
	   strcpy(CAI_ERREUR,"Informations incoherentes ligne CHANNELS");
	   iret=0;
	 }
	else
	{
	   fscanf(fic_in,"%d\n",&nbplan);
           *nb_can=nbplan;

/*.......................................
	   Decodage du nombre de lignes
  ........................................*/
	    fscanf(fic_in,"%s\n",label);
	    retour=strncmp(label,"LINES",5);
	    if (retour!=0)
	    {
	       strcpy(CAI_ERREUR,"Informations incoherentes ligne LINES");
	       iret=0;
	     }
	     else
	     {
	       fscanf(fic_in,"%d\n",nb_lig);


/*.......................................
	  Decodage du nombre de colonnes
  ........................................*/
	       fscanf(fic_in,"%s\n",label);
	       retour=strncmp(label,"COLUMNS",7);
	       if (retour!=0)
	       {
	    	 strcpy(CAI_ERREUR,"Informations incoherentes ligne COLUMNS");
	    	 iret=0;
	  	}
	       else
	       {

		 fscanf(fic_in,"%d\n",nb_col);

/*.......................................
	  Decodage du nombre de bits par pixel
  ........................................*/
	  	 fgets(label,80,fic_in);
	  	 retour=strncmp(label,"BITS PER PIXEL",14);
		 if (retour!=0)
	  	 {
	    	   strcpy(CAI_ERREUR,"Informations incoherentes ligne BITS ");
	    	   iret=0;
	  	  }
	  	 else
	    	 {
		   fscanf(fic_in,"%d\n",&nbbit);

/*........................................................
	  Decodage du type de codage du pixel (optionnel)
  ........................................................*/
     		   fscanf(fic_in,"%s\n",label);
     		   retour=strncmp(label,"TYPE",4);
     		   if (retour==0)
                   {
      		      fscanf(fic_in,"%s\n",COD_PIX);
		      retour=1;
                   }
		   else
		     sprintf(COD_PIX,"UND");
        	   pch0=cai_En_majuscule( COD_PIX );

/* 	Verification si type de codage reconnu */
         	   reconnaissance_type = cai_istyp_connu(pch0);
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             en lecture et ecriture dans l'entete .hd      */
/*****************************************************************************/
		   if (retour !=0) fscanf(fic_in,"%s\n",label);
		   retour=strncmp(label,"SENSCODAGE",10);
	 	   if (retour!=0)
	 	   {
/*      Si absent, pas d'inversion      */    
			image1->INV_OCTETS =0;
			sprintf((char *)image1->SENS_COD,type_machine);
	 	    }
		    else
		    {
  			 fscanf(fic_in,"%s\n",label);
  			 retour=strncmp(label,"IEEE",4);
   			 if (retour==0)	
				sprintf((char *)image1->SENS_COD,"BE");
   			 else
				sprintf((char *)image1->SENS_COD,"LE");
   			 cmp = strncmp(type_machine,image1->SENS_COD,2);
   			 if (cmp!=0)
 	  			image1->INV_OCTETS =1;
   			 else
	 			image1->INV_OCTETS =0;
                     }	 	
		  }
		}
	      }
	  }
      }
    }
/*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in !=NULL) fclose(fic_in);

   if (iret !=0)
   {
/*..................................
     Initialisation du nombre d'octets par pixel
  ........................................................*/
     if ((nbbit%8)==0)
	*nboct_pix=(int)(nbbit/8);
     else
	*nboct_pix=(int)((nbbit/8)+1);
     image1->NB_BITS_PIX=nbbit;

     if (reconnaissance_type != -1)
     {
	    	sprintf(image1->COD_PIX,"%s",COD_PIX);
     }
     else
     {
     	  sprintf(image1->COD_PIX,"UND");

          if (*nboct_pix==1)
              sprintf(image1->COD_PIX,"OCT");
          else if (*nboct_pix==2)
	      sprintf(image1->COD_PIX,"I2");
          else if (*nboct_pix==4)
          {
	     retour=strncmp(COD_PIX,"I4",2);
             if (retour==0)
	        sprintf(image1->COD_PIX,"I4");
             else
             {
	 	retour=strncmp(COD_PIX,"R4",2);
          	if (retour==0)
	    	  sprintf(image1->COD_PIX,"R4");
             }
           }
           else if (*nboct_pix==8)
           {
		sprintf(image1->COD_PIX,"R8");	   
           }
       }

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     if ((long int)strlen(repert)!= 0)
     {
       sprintf(nom_fic_canal,"%s/%s.bip",repert,nom_image);
      }
      else
      {
       sprintf(nom_fic_canal,"%s.bip",nom_image);
      }
/*****************************************************************************/
/* MOD : VERSION : 4.4 : FA : prise en compte des  extensions en majuscule   */
/*****************************************************************************/
     retour = stat ( nom_fic_canal , &buf_stat );
     if ( retour != 0 )
     { 
       if((long int)strlen(repert)!=0)
       {
         sprintf(nom_fic_canal,"%s/%s.BIP",repert,nom_image);
       }
       else
       {
        sprintf(nom_fic_canal,"%s.BIP",nom_image);
       }
     }
 
     deb = image1->NOM_FIC_CAN;
     deb1= image1->NOM_UTIL_CAN;
     num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",deb);
  	   deb=deb+100;
           sscanf(nom_image,"%s",deb1);
  	   deb1=deb1+100;
           image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=0;
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
   Fin de l'operation cai_ouvre_modifie_bip
*/
