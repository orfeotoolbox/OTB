/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_hdf16.c                                                   */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format hdf                                        */ 
/*		Seuls l'acces aux Scientific Data Set est possible, code en  */
/*		8bits non signes ou 16 bits non signes                       */
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    Octobre 1996                                        */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V2                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 3.3 : DM : optimisation du sous-echantillonnage           */
/*****************************************************************************/
/* MOD : VERSION : 4.0 : DM : possibilite de traiter des images en R4 et R8  */
/*			   flottant simple et double precision               */
/*****************************************************************************/
/* MOD : VERSION : 4.1 : FA : Rajout des tests sur les retours d'allocation  */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : pb sur le sous-echantillonnage-prise en compte */
/*			   du nombre d'octets par pixel pour ssech en lignes */
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Identification ds l'entete des SDS par le nom  */
/*			   ou par le numero indifferemment.                  */
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Prise en compte  images codées I4 en lecture   */
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Remplacer les appels systemes pour renommer un */
/*                            fichier par la commande C rename               */
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM : Prise en compte en lecture des images codées en*/
/*                            8, 16 et 32 bits signés                        */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/


/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>

#define _CAI_IMAGE
#define IN_CAI_HDF

# include "cai_image.h"
# include "hdf.h"
/*# include "mfhdf.h"*/

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_hdf.c,v 1.10 01/05/31 10:47:25 cmf Exp $";


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
/* OPERATION :    cai_liste_ima_hdf16                                        */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format hdf                    */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      int cai_liste_ima_hdf16(repert,tab_ima)           */
/* -----------------                                                         */
/*                                                                           */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_hdf16       (S) (int)     : = -1 repertoire inexistant  */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_ima_hdf16 (char *repert,
		       char ***tab_ima)

{

   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images hdf identifiees             */
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
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE HDF
      (EXTENSION =.hdf)
   ..........................................*/
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".hdf");
     if (pch0 != NULL)
     {
        ll = (long int)strlen(dp->d_name);
        dif = (long int)(dp->d_name-pch0);
        dif=labs(dif);
        pch1=pch0+4;

/*****************************************************************************/
/* MOD:VERSION:3.3:DM lecture de toutes les images dont l'extension commence */
/*		    par .hdf. Cette modif permet de prendre en compte .hdf16 */
/*****************************************************************************/
/*ligne supprimee        if (((ll-4)==dif)||((*pch1)==blanc)) */
	     nb_ima=nb_ima+1;
     
     }
   }
    (void)closedir(dirp);

/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT HDF
     ......................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE HDF
      (EXTENSION =.hdf)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,".hdf");
     if (pch0 != NULL)
     {
        ll = strlen(dp->d_name);
        dif = (long int)(dp->d_name-pch0);
        dif=labs(dif);
        pch1=pch0+4;
/*****************************************************************************/
/* MOD:VERSION:3.3:DM lecture de toutes les images dont l'extension commence */
/*		    par .hdf. Cette modif permet de prendre en compte .hdf16 */
/*****************************************************************************/

/* ligne supprimee       if (((ll-4)==dif)||((*pch1)==blanc))
        {*/
            *pch0='\0';
            strcpy((*tab_ima)[nb_ima],dp->d_name);
	    nb_ima=nb_ima+1;
/*  ligne supprimee        }*/
     
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

/* Fin de l'operation cai_liste_ima_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_hdf16                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format hdf       */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_lecture_hdf16 (nom_image,     */
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
/*     cai_ouvre_lecture_hdf16     (S) (CAI_IMAGE *)  : = NULL si pb         */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_lecture_hdf16(char *repert,
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
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour,icr;		/* retour de fonctions                      */ 
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int num;		        /* indice 				    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   int plan;                    /* indice de boucle sur les canaux          */  
   char nom_plan[100];          /* nom des fichiers plan                    */ 
   int Ref,lon,Alpha;
   int sds_id,status,attribute,num_type,nb_dim;
   char name[MAX_NC_NAME];
   int val_dim[MAX_VAR_DIMS];
   int nbdataset,nfileattribut,retn,nbits;
   char refSDS[50], sdsname[50];
   int ind,isds;
   int sds_trouve;

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   fic_in = NULL;

   nbplan=0;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1: FA Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

   if (image1==NULL)
   {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_lecture");
     goto ERREUR;
    }
/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.hdf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.hdf",nom_image);
   }

/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=NULL;
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*****************************************************************************/
/* MOD:VERSION:3.3:DM on essaie avec .hdf16 comme extension (vieille version)*/
/* Cette modif permet de prendre en compte .hdf16 du QIV                     */
/*****************************************************************************/
     strcat(nom_fic,"16");
     fic_in=fopen(nom_fic,"r");
     if (fic_in==NULL)
     {
         strcpy(CAI_ERREUR,"Erreur ouverture fichier entete HDF");
         goto ERREUR;
      }
   }
/*................................................
   Pour chaque plan image lecture du nom du fichier correspondant
   et de la position du SDS dans le fichier
  ..................................................*/
  fgets(label,80,fic_in);
  retour=strncmp(label,"PLANES_ID",9);
  if (retour!=0)
  {
	strcpy(CAI_ERREUR,
	      "Informations incoherentes ligne PLANES_ID ");
        goto ERREUR;
   }
   icr=2;
   retour=1;
   deb = image1->NOM_FIC_CAN;
   deb1= image1->NOM_UTIL_CAN;

   while ((icr==2)&&(retour!=0))
   {
      icr = fscanf(fic_in,"%s %s",nom_plan,refSDS);
      if (icr==1) 
      {
	Ref=0;
        icr=2;
      }
      retour=strncmp(nom_plan,"NB_BITS_PIX",11);
      if ((icr==2)&&(retour!=0))
      {  
        retour=strncmp(nom_plan,"COMMENT",7);
        if ((icr==2)&&(retour!=0))
        {  

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
          nom_fic[0]='\0';
          nom_fic_canal[0]='\0';

          if ((long int)strlen(repert)!=0)
          {
            sprintf(nom_fic_canal,"%s/%s",repert,nom_plan);
           }
          else
          {
            sscanf(nom_plan,"%s",nom_fic_canal);
          }
/*.......................................
       test sur l'existence du fichier plan 
       et ouverture du fichier plan
  .......................................*/
          num=SDstart(nom_fic_canal,DFACC_RDONLY);
          if (num!= -1)
          {
              sscanf(nom_plan,"%s",deb);
  	      deb=deb+100;
              sscanf(nom_plan,"%s",deb1);
  	      deb1=deb1+100;
              image1->ACCES_CANAL_INT[nbplan]=num;

/*         Recuperation des informations generales */
/*.................................................*/
              retn=SDfileinfo(num,&nbdataset,&nfileattribut);
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Identification ds l'entete des SDS par le nom  */
/*			   ou par le numero indifferemment.                  */
/*****************************************************************************/
/* Verification si SDS identifie par son nom ou par son numero */
              lon=strlen(refSDS ); 
              Alpha=0;
              for (ind=0;ind<lon;ind++)
              {
                  if (isalpha(refSDS[ind])!=0) Alpha=Alpha+1;
              }
              if (Alpha !=0)
              {
/*              Identification par nom de SDS boucle sur les SDS */
/*              pour recuperer le numero de SDS                  */
                sds_trouve = 0;
                isds =0;
                while ((isds<nbdataset)&&(sds_trouve==0))
                {
                   sds_id = SDselect(num,isds);
/*                 Recuperation des infos du SDS                 */
                  status = SDgetinfo(sds_id,sdsname,
				        &nb_dim,val_dim,
					&num_type,&attribute);

    		   if ( strcmp ( sdsname, refSDS ) == 0 )
    		   {
 /*      recuperer le numero de SD correspondant et repartir sur la version 
        precedente*/
                     Ref=isds;
                     sds_trouve = 1;
    		   }
                   else
			isds++;

                 }
                 if (sds_trouve == 0)
                 {
		   sprintf( CAI_ERREUR ,"Nom de SDS non trouve %s\0",refSDS);
                   goto ERREUR;
                 }
               }
               else
               {
                  Ref=atoi(refSDS);

/*         Selection du Scientific Data Set contenant les donnees */
/*................................................................*/
                  sds_id = SDselect(num,Ref);
                  status = SDgetinfo(sds_id,sdsname,
				        &nb_dim,val_dim,
					&num_type,&attribute);
              }
/*******************************************************/
/* FIN MOD : VERSION : 5.0 : DM : Identification  SDS  */
/*******************************************************/            
              image1->POS_OCTET[nbplan]=sds_id;

/*         Verification que tous les plans ont la meme taille */
/*...........................................................*/
              if (nbplan!=0)
              {
	        if ((*nb_lig!=val_dim[0])||(*nb_col!=val_dim[1]))
                {            
		   sprintf( CAI_ERREUR ,"Plans de taille differentes !\0");
                   goto ERREUR;
                }
              }
              *nb_lig=val_dim[0];
              *nb_col=val_dim[1];
              nbbit= num_type;  

              nbplan=nbplan+1;

           }
           else
           {
 	     sprintf ( CAI_ERREUR ,
	       "Le fichier plan %s HDF n'existe pas\n" , nom_fic_canal );
             goto ERREUR;
           }
        }
        else
          icr=99;
      }
   }
/*....................................
  Decodage du nombre d'octets par pixel
  ......................................*/
  if ((retour==0)&&(icr!=99))
  {
    if (icr==2) 
       image1->NB_BITS_PIX=Ref;
    else
       image1->NB_BITS_PIX=0;
/*....................................
  Decodage du commentaire
  ......................................*/
     icr = fscanf(fic_in,"%s",label);
     retour=strncmp(label,"COMMENT",7);
     if (retour==0)
		icr=99;
  }
  else
       image1->NB_BITS_PIX=0;



/*....................................
  Decodage du commentaire
  ......................................*/
  if (icr==99)
     icr = fscanf(fic_in,"%s",image1->COMMENT);
  else
      strcpy(image1->COMMENT,"Pas de commentaire\0");

/*...................................*/
/* Initialisation du nombre de plans */
/*...................................*/
  *nb_can=nbplan;

/*..................................
   Initialisation du nombre d'octets par pixel
  ........................................................*/
/*****************************************************************************/
/* MOD : VERSION : 5.1 : DM : Prise en compte en lecture des images codées en*/
/*                            8, 16 et 32 bits signes                        */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/

   if (nbbit==DFNT_INT8)
   {
	*nboct_pix=1;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=8;
	sprintf(image1->COD_PIX,"OCT");
   }
   else if (nbbit==DFNT_UINT8)
   {
	*nboct_pix=1;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=8;
	sprintf(image1->COD_PIX,"UOCT");

   }
   else if (nbbit==DFNT_INT16)
   {
	*nboct_pix=2;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=16;
	sprintf(image1->COD_PIX,"I2");
   }
   else if (nbbit==DFNT_UINT16)
   {
	*nboct_pix=2;
        if (image1->NB_BITS_PIX==0) image1->NB_BITS_PIX=16;
	sprintf(image1->COD_PIX,"UI2");
   }

/* MOD : 5.0 Prise en compte des images codées en I4 non signe*/
   else if (nbbit==DFNT_INT32)
   {
	*nboct_pix=4;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=32;
	sprintf(image1->COD_PIX,"I4");
   }
   else if (nbbit==DFNT_UINT32)
   {
	*nboct_pix=4;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=32;
	sprintf(image1->COD_PIX,"UI4");
   }
   else if (nbbit==DFNT_FLOAT)
   {
	*nboct_pix=4;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=32;
	sprintf(image1->COD_PIX,"R4");

   }
   else if (nbbit==DFNT_DOUBLE)
   {
	*nboct_pix=8;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=64;
	sprintf(image1->COD_PIX,"R8");

   }
   else
   {
 	 sprintf ( CAI_ERREUR ,
	   "Type de donnees non supporte par la Couche Acces Image\n" );
         goto ERREUR;
    }
/*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in!=NULL) fclose(fic_in);

/*......................
    sortie normale 
  ....................*/
      return(image1);

ERREUR:

/*......................
    sortie degradee 
  ....................*/
   if (fic_in!=NULL)    fclose(fic_in);
   for (plan=0;plan<nbplan;plan++)
   {
     num=image1->ACCES_CANAL_INT[plan];
     if (num!=-1) retn=SDend(num);
   }
   return (NULL);

}

/* 
   Fin de l'operation cai_ouvre_lecture_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_hdf16                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format hdf et de tous les fichiers qui la composent          */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :    CAI_IMAGE *cai_ouvre_creation_hdf16 (nom_image,     */
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
/*     cai_ouvre_creation_hdf16     (S) (CAI_IMAGE *) : =NULL si pb          */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_hdf16(char *repert,
				  char *nom_image,
				  int  *nb_can,
				  int  *nboct_pix,
				  int  *nb_col,
				  int  * nb_lig,
				  char *label)

{
   FILE *fic;		        /* pointeur sur descripteur de fichier      */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image */ 
   char nom_fic[1024];		/* nom du fichier en-tete		    */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence*/
   int retour;			/* retour de fonction 			    */ 
   char car[4];			/* indice du canal traite     		    */
   int num;			/* indice 				    */
   struct stat	buf_stat;	/* structure decrivant status de fichier    */
   int plan;                    /* indice de boucle sur les canaux          */
   char nom_plan[100];          /* nom des fichiers plan                    */ 
   int dims[2],rank,sds_id; 
   int nbbit,Ref,Coher;
   char NomSDS[80];  
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

   NomSDS[0]='\0';
   Ref=0;

/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  Maj du  TYPE lorsque l'utilisateur le         */
/*                                  precise dans le param  "label"           */
/*****************************************************************************/
/* Verification si type de codage reconnu dans le label */
    reconnaissance_type = cai_istyp_connu(label);
    if (reconnaissance_type == -1)
    {
     	if (*nboct_pix==1)
       		 nbbit=DFNT_UINT8;
      	else if (*nboct_pix==2)
       		 nbbit=DFNT_UINT16;
      	else if (*nboct_pix==4)
        	nbbit=DFNT_FLOAT;
      	else if (*nboct_pix==8)
        	nbbit=DFNT_DOUBLE;
     }
     else
     {
/* Verification de coherence entre le nb d'octets par pixel et le type donne */
        Coher=1;
        if(*nboct_pix == 1) 
        {
	  nbbit=DFNT_INT8;
	  if (reconnaissance_type > 1) Coher=0;  
          else if (reconnaissance_type ==1)  nbbit=DFNT_UINT8;
        }    
	else if(*nboct_pix == 2) 
        {
          if (reconnaissance_type ==2 )  nbbit=DFNT_INT16;
          else if (reconnaissance_type ==3 )  nbbit=DFNT_UINT16;
          else Coher=0;  	  
        }  	  
	else if(*nboct_pix == 4) 
        {
          if (reconnaissance_type ==4 )  nbbit=DFNT_INT32;
          else if (reconnaissance_type ==5 )  nbbit=DFNT_UINT32;
          else if (reconnaissance_type ==6 )  nbbit=DFNT_FLOAT;
          else Coher=0;  	  
        }  
	else if(*nboct_pix == 8) 
        {
	  if (reconnaissance_type == 7)  nbbit=DFNT_DOUBLE;
	  else Coher = 0;
        }
        else 
	   Coher = 0;
        if (Coher == 0) 
        {      	
	  sprintf(CAI_ERREUR,"incoherence entre TYPE et Nb bits par pixels\n");
	  goto ERREUR;
        }    
    }
/*.................................................
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.hdf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.hdf",nom_image);
   }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s HDF existe \n" , nom_image );
	goto ERREUR;
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

  fprintf ( fic , "PLANES_ID\n" );
/*...............................................................
  Pour chaque canal de l'image,constitution du nom de fichier correspondant
  Verification qu'il n'existe pas deja
  Ouverture du fichier en ecriture
  .................................................................*/

  for (plan=0;plan<*nb_can;plan++)
  {
     nom_plan[0]='\0';
     nom_fic_canal[0]='\0';     
     sprintf(nom_plan,"%s.b%d",nom_image,plan);
     fprintf ( fic , "%s %d\n" , nom_plan,Ref );
     if ((long int)strlen(repert)!=0)
     {
	sprintf(nom_fic_canal,"%s/%s",repert,nom_plan);

     }
     else
     {
    	 sprintf(nom_fic_canal,"%s",nom_plan);
     }
        retour = stat ( nom_fic_canal , &buf_stat );

     if ( retour == 0 )
     { 
	sprintf ( CAI_ERREUR , "L'image %s HDF existe \n" , nom_fic_canal );
	goto ERREUR;
     }
/* ouverture du fichier */
     num=SDstart(nom_fic_canal,DFACC_CREATE);
     if (num == -1)
     {
        sprintf(CAI_ERREUR,"Erreur: Creation %s impossible\n",nom_fic_canal);
        goto ERREUR;
      } 
      image1->ACCES_CANAL_INT[plan]=num;

/* ouverture et creation du SDS */
      rank=2;
      dims[0]=*nb_lig;
      dims[1]=*nb_col;
    
      if((long int)strlen(repert)!=0)
        strcpy(NomSDS,label);
      else
        strcpy(NomSDS,"SDS1");

      sds_id=SDcreate(num,NomSDS,nbbit,rank,dims);

      image1->POS_OCTET[plan]=sds_id;
  }
  retour=fprintf ( fic , "NB_BITS_PIX\n");
  retour=fprintf ( fic , "%d\n" , (*nboct_pix)*8 );

  retour=fprintf ( fic , "COMMENT\n");
  retour=fprintf ( fic , "%s\n" , label );
  if (retour == 0)
  {
      sprintf(CAI_ERREUR,"Erreur: Ecriture %s impossible\n",nom_fic);
      goto ERREUR;
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
  Fin de l'operation cai_ouvre_creation_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_hdf16                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image hdf                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_hdf16(image1, canal,      */       		      /* ------------------                            premiere_ligne,             */
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
/*     cai_lecture_canal_hdf16     (S) (CAI_OK_KO) : = CAI_KO si pb          */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_lecture_canal_hdf16(CAI_IMAGE *image1 ,  
                                int *canal ,           
    		          	int *premiere_ligne  ,   
     		          	int *premiere_colonne,
                         	int *nombre_lignes_lire ,
                          	int *nombre_colonnes_lire,
                          	int *pas_ligne ,          
                          	int *pas_colonne,
		          	unsigned char *data_image )        
{

 int   num,n,ilig,k,j;      /* variables de travail                          */
 int iret;                  /* indicateur d'erreur                           */
 int edges[2],strides[2],start[2];
 unsigned char *buf,*deb,*pch;
 int nboct;

/*...........................
  Initialisations diverses
  ..........................*/

   iret=CAI_OK;
   num=image1->POS_OCTET[(*canal)-1];

    if ((*pas_ligne==1)&&(*pas_colonne==1))
    {
       start[0]=(*premiere_ligne)-1;
       start[1]=(*premiere_colonne)-1;
       edges[0]=*nombre_lignes_lire;
       edges[1]=*nombre_colonnes_lire;
       strides[0]=*pas_ligne;
       strides[1]=*pas_colonne;
       n= SDreaddata(num,start,strides,edges,(VOIDP *)data_image);
       if (n<0)
       {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image HDF");
        }
    }
    else
    {
	nboct = (*nombre_lignes_lire)*(*pas_ligne)*
				(image1->NB_COL)*(image1->NBOCT_PIX);
	buf = (unsigned char *) calloc(nboct,sizeof(unsigned char));
/*...........................................
            lecture des lignes images
  ................................................*/

       start[0]=(*premiere_ligne)-1;
       start[1]=0;
       edges[0]=(((*nombre_lignes_lire)-1)*(*pas_ligne))+1;
       edges[1]=image1->NB_COL;
       strides[0]=1;
       strides[1]=1;
       n= SDreaddata(num,start,strides,edges,(VOIDP *)buf);
       if (n<0)
       {
             iret=CAI_KO;
             strcpy(CAI_ERREUR,"Erreur lecture donnee image HDF");
        }
        pch=data_image;

/*......................................
             Sauvegarde des donnees lues entenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/  
        for (ilig=0;ilig<*nombre_lignes_lire;ilig++)
        {
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : pb sur le sous-echantillonnage-prise en compte */
/*			   du nombre d'octets par pixel pour ssech en lignes */
/*****************************************************************************/

             deb=buf+((ilig*(image1->NB_COL)*(*pas_ligne))+
			((*premiere_colonne)-1))*image1->NBOCT_PIX;
             j=1;
             while (j <= (*nombre_colonnes_lire))
	     {  
               for (k=0;k<image1->NBOCT_PIX;k=k+1)
                {
                  *(pch+k)=*(deb+k);
                 }
              pch=pch+image1->NBOCT_PIX;
              j=j+1;
              deb=deb+((*pas_colonne)*image1->NBOCT_PIX);
             }   
         }
          if (buf!=NULL) free(buf);
    }
 
   return(iret);
 }

/*
  Fin de l'operation cai_lecture_canal_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_hdf16                                   */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image hdf                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :      CAI_OK_KO cai_ecriture_canal_hdf16(image1, canal, */       		      /* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_hdf16        (S) (CAI_OK_KO) : = CAI_KO si pb      */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ecriture_canal_hdf16 (CAI_IMAGE *image1 ,  
                                      int *canal ,           
    		                      int *premiere_ligne  ,   
                                      int *nombre_lignes_ecrire ,
                                      int *nombre_colonnes_ecrire,
		                      unsigned char *data_image )
{

int num,n;                        /* descripteur de fichier                */
int edges[2],strides[2],start[2];


/* initialisations */

     num = image1->POS_OCTET[*canal-1];
     start[0]=(*premiere_ligne)-1;
     start[1]=0;
     edges[0]=*nombre_lignes_ecrire;
     edges[1]=*nombre_colonnes_ecrire;
     n= SDwritedata(num,start,NULL,edges,(VOIDP *)data_image);
     if (n<0)
     {
             strcpy(CAI_ERREUR,"Erreur lecture donnee image HDF");
	     return (CAI_KO);
      }

return (CAI_OK);

}

/* 
  Fin de l'operation cai_ecriture_canal_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_hdf16                                            */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format hdf et de           */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_hdf16(image1)                     */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_hdf16        (S) (CAI_OK_KO) :   = CAI_KO si pb             */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO cai_ferme_hdf16 (CAI_IMAGE *image1)

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
   num = image1->POS_OCTET[i_can];
   iret=SDendaccess(num);
   num=image1->ACCES_CANAL_INT[i_can];
   iret=SDend(num);
 }
  return (iret);
}

/*
  Fin de l'operation cai_ferme_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_hdf16                                         */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en hdf et de         */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_hdf16(format,nom_ima)          */       		      /* ------------------                                                        */
/*     format       (E) (char *) : format de l'image a detruire              */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_hdf16     (S) (CAI_OK_KO) : = CAI_KO si pb               */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/



CAI_OK_KO cai_dest_ima_hdf16 (char *repertoire , 
                     	      char *nom_ima )

{
int	i_can;			/* compteur de canaux                        */
long    iret;                   /* code retour fonction fermetrue lecture    */
char	str_num_canal[3];	/* suffixe numero du canal                   */
char	nom_fic_entete[1024];	/* nom du fichier entete a detruire          */
char    nom_ima_complet[1024];  /* repertoire + nom de l'image               */
char	nom_fic_canal[1024];	/* nom du fichier canal a detruire           */
int	cr_stat;		/* code retour de fonction status de fichier */
struct stat buf_stat;		/* structure decrivant status de fichier     */ 
CAI_IMAGE   *image1;            /* structure ouverture image                 */
int         nb_can;	        /* nombre de canaux de l'image               */
int         nboct_pix;		/* nombre d'octets par pixel image           */
int         nb_col;	        /* nombre de colonnes de l'image             */
int         nb_lig;             /* nombre de lignes de l'image               */
char    ptr_nom_fic_can[100*CAI_NB_MAX_CAN];  /* nom fichier canal traite    */
char	nom_fichier[1024];	/* nom du fichier entete a detruire	     */
char    chemin[1024];           /* repertoire + "/"			     */
char *ptr,*deb;

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/
chemin[0]='\0';
ptr_nom_fic_can[0]='\0';

if ((long int)strlen(repertoire)!=0)
{
  sprintf(nom_ima_complet,"%s/%s",repertoire,nom_ima);
  sprintf ( chemin ,"%s/", repertoire );
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
  strcpy ( chemin , "./");
}
  strcpy(nom_fic_entete , nom_ima_complet);
  strcat ( nom_fic_entete , ".hdf\0" );

/************************************************/
/* Extraction des caracteristiques de l'image 	*/
/* et noms des fichiers plans                   */
/* du fichier entete par l'appel a la fonction	*/
/* cai_ouvre_lecture_image			*/
/************************************************/
  image1 = cai_ouvre_lecture_image ( nom_ima_complet,"HDF", &nb_can ,
				     &nboct_pix ,&nb_col, &nb_lig );
	
  if ( image1 == NULL) goto ERREUR;

  ptr = ptr_nom_fic_can;
  deb = image1->NOM_FIC_CAN;
  for ( i_can = 0 ; i_can < nb_can ; i_can++ )
  {
      sscanf(deb,"%s",ptr);
      deb=deb+100;
      ptr=ptr+100;
  }

  ptr = ptr_nom_fic_can;

 iret=cai_ferme_image(image1);
 image1=NULL;
/************************************************/
/* Destruction des fichiers radiometrie		*/
/************************************************/

  for ( i_can = 0 ; i_can < nb_can ; i_can++ )
  {
	strncpy ( nom_fic_canal , ptr , 100 );
	nom_fic_canal[99] = '\0';
	strcpy ( nom_fichier , chemin );
	strcat ( nom_fichier, nom_fic_canal );
	nom_fichier[strlen(chemin)+strlen(nom_fic_canal)+1] = '\0';
	cr_stat = stat ( nom_fichier , &buf_stat );
	if ( cr_stat == 0 )
		unlink ( nom_fichier );
	ptr = ptr + 100;
  }

 
/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/
cr_stat = stat ( nom_fic_entete , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_entete );
else
{
/*****************************************************************************/
/* MOD:VERSION:3.3:DM on essaie avec .hdf16 comme extension (vieille version)*/
/* Cette modif permet de prendre en compte .hdf16 du QIV                     */
/*****************************************************************************/
     strcat(nom_fic_entete,"16");
     cr_stat = stat ( nom_fic_entete , &buf_stat );
     if ( cr_stat == 0 )
	unlink ( nom_fic_entete );
}
 
return (CAI_OK);
ERREUR:
  return((CAI_OK_KO)NULL);
}
/*
 Fin de l'operation cai_dest_ima_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_rename_fichiers_hdf16                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    permet de modifier l'extension des plans d'une image HDF        */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_rename_fichiers_hdf16(image1,TabExtens) */       		      /* ------------------                                                        */
/*									     */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*     TabExtens   (E) (CAI_EXTENS *) : tableau des nouvelles extensions     */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_rename_fichiers_hdf16(CAI_IMAGE *image1,CAI_EXTENS *TabExtens)

{
  int Can;		/* indice de boucle sur les canaux */
  char fichierE[1024];  /* fichier a traiter               */
  char fichierS[1024];  /* nouveau nom du fichier          */
  FILE *fic;		/* pointeur descripteur fichier    */
  int pt_oct;		/* position courante ds le fichier */
  int *Ref;             /* Reference du SDS des fichiers   */
  char label[80];       /* contient un enregist  fichier   */
  char repert[1024];    /* repertoire de stockage image    */
  char image[1024];     /* nom de l'image a ouvrir         */
  char *pch,*deb;       /* pointeurs de travail            */
  long nb1,nb2;		/* indice de longueur significative*/
  char car;		/* variable de travail             */
  int icr,tempo;        /* code retour fonction            */
  CAI_CHAINE *NomIma;

/* Separation du repertoire de stockage de l'image et du nom propre */

      repert[0]='\0';
      image[0]='\0';
      car='/';
      pch=(char *)strrchr(image1->NOM_IMAGE,car);
      if (pch == NULL) 
      {
         /* le repertoire est celui courant */
           sscanf(image1->NOM_IMAGE,"%s",image);
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';        
      }
      else
      {
  
           nb1=(long int)(pch)-(long int)(image1->NOM_IMAGE);
           strncpy(repert,image1->NOM_IMAGE,nb1);
           repert[nb1]='\0';
           deb=image1->NOM_IMAGE+nb1+1;
           nb2=strlen(image1->NOM_IMAGE)-(nb1+1);
	   strncpy(image,deb,nb2);
           image[nb2]='\0';
           car='.';
           pch=(char *)strrchr(image,car);
           if (pch != NULL)
               *pch='\0';          

     }
        
/* Ouverture du fichier d'entete */

   fichierE[0]='\0';
   sprintf(fichierE,"%s.hdf",image1->NOM_IMAGE);
   fic = fopen(fichierE,"r+");
   if (fic == NULL)
   {
/*****************************************************************************/
/* MOD:VERSION:3.3:DM on essaie avec .hdf16 comme extension (vieille version)*/
/* Cette modif permet de prendre en compte .hdf16 du QIV                     */
/*****************************************************************************/
         strcat(fichierE,"16");
         fic = fopen(fichierE,"r+");
         if (fic == NULL)
         {

            sprintf(CAI_ERREUR,
	       "Erreur: Modification fichier %s impossible\n",fichierE);
            goto ERREUR;
         }
   }

/* Lecture des references de chaque plan */

  Ref = (int *) calloc(image1->NB_CANAUX,sizeof(int));
  fgets(label,80,fic);
  icr=strncmp(label,"PLANES_ID",9);
  if (icr!=0)
  {
	strcpy(CAI_ERREUR,
	      "Informations incoherentes ligne PLANES_ID ");
        goto ERREUR;
   }

/*................................................*/
/* Rename de chaque bande spectrale si necessaire */
/*................................................*/
  NomIma = NULL;
  NomIma = (CAI_CHAINE *)calloc(image1->NB_CANAUX,sizeof(CAI_CHAINE));
  if (NomIma == NULL) goto ERREUR;
  for(Can=0;Can<image1->NB_CANAUX;Can++)
  {
    icr = fscanf(fic,"%s%d",label,&tempo);
    if (icr==1) 
	Ref[Can]=0;
    else
       Ref[Can]=tempo;
    if (TabExtens != NULL)
    {
      fichierE[0]='\0';
      fichierS[0]='\0';  
      sprintf(fichierE,"%s/%s",repert,label);
      sprintf(fichierS,"%s%s",image1->NOM_IMAGE,TabExtens[Can]);
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Remplacer les appels systemes pour renommer un */
/*                            fichier par la commande C rename               */
/*****************************************************************************/
      rename(fichierE,fichierS);
    }
    else
    {
	strcpy(NomIma[Can],label);
    }
  }   

/* Modification du fichier d'entete */
   rewind(fic);
   fprintf ( fic , "PLANES_ID\n" );
   for(Can=0;Can<image1->NB_CANAUX;Can++)
   {
       tempo=Ref[Can];
    if (TabExtens != NULL)
       fprintf ( fic , "%s%s %d\n",image,TabExtens[Can],tempo);
    else
       fprintf ( fic , "%s %d\n",NomIma[Can],tempo);


   }
  fprintf ( fic , "NB_BITS_PIX\n");
  fprintf ( fic , "%d\n" , image1->NB_BITS_PIX );

  icr=fprintf ( fic , "COMMENT\n");
  icr=fprintf ( fic , "%s\n" , image1->COMMENT );

  fclose ( fic );

  if (NomIma != NULL) free(NomIma);

  return(CAI_OK);

ERREUR :
  return(CAI_KO);
}
/*
 Fin de l'operation cai_rename_fichiers_hdf16
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_hdf16                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     hdf et de tous les fichiers qui la composent                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :     CAI_IMAGE *cai_ouvre_modifie_hdf16 (nom_image,     */
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
/*     cai_ouvre_modifie_hdf16     (S) (CAI_IMAGE *)  : = NULL si pb         */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_modifie_hdf16(char *repert,
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
   int nbbit;			/* nb de bits par pixel du fichier en-tete  */
   int retour,icr;		/* retour de fonctions                      */ 
   char *deb,*deb1;		/* pointeurs sur chaine de caracteres       */
   int num;			/* indice 				    */
   int nbplan;			/* nombre de canaux dans le fichier en-tete */
   int plan;                    /* indice de boucle sur les canaux          */
   char nom_plan[100];          /* nom des fichiers plan                    */ 
   int Ref;
   int sds_id,status,attribute,num_type,nb_dim;
   char name[MAX_NC_NAME];
   int val_dim[MAX_VAR_DIMS];
   int nbdataset,nfileattribut,retn,nbits;

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   fic_in = NULL;

   nbplan=0;

   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
/*****************************************************************************/
/* MOD : VERSION : 4.1: FA Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/

 if (image1==NULL)
 {
     strcpy(CAI_ERREUR,"Erreur allocation memoirem dans cai_ouvre_modifie");
     goto ERREUR;
 }

/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long int)strlen(repert)!=0)
   {
      sprintf(nom_fic,"%s/%s.hdf",repert,nom_image);
   }
   else
   {
      sprintf(nom_fic,"%s.hdf",nom_image);
    }

/*........................
   Ouverture du fichier entete
  ..........................................*/
   fic_in=fopen(nom_fic,"r");
   if (fic_in==NULL)
   {
/*****************************************************************************/
/* MOD:VERSION:3.3:DM on essaie avec .hdf16 comme extension (vieille version)*/
/* Cette modif permet de prendre en compte .hdf16 du QIV                     */
/*****************************************************************************/
     strcat(nom_fic,"16");
     fic_in=fopen(nom_fic,"r");
     if (fic_in==NULL)
     {
         strcpy(CAI_ERREUR,"Erreur ouverture fichier entete HDF");
         goto ERREUR;
      }
    }

/*................................................
   Pour chaque plan image lecture du nom du fichier correspondant
   et de la position du SDS dans le fichier
  ..................................................*/
  fgets(label,80,fic_in);
  retour=strncmp(label,"PLANES_ID",9);
  if (retour!=0)
  {
	strcpy(CAI_ERREUR,
	      "Informations incoherentes ligne PLANES_ID ");
        goto ERREUR;
   }
   icr=2;
   retour=1;
   deb = image1->NOM_FIC_CAN;
   deb1= image1->NOM_UTIL_CAN;

   while ((icr==2)&&(retour!=0))
   {
      icr = fscanf(fic_in,"%s%d",nom_plan,&Ref);
      if (icr==1) 
      {
	Ref=0;
        icr=2;
      }
      retour=strncmp(nom_plan,"NB_BITS_PIX",11);
      if ((icr==2)&&(retour!=0))
      {  
        retour=strncmp(nom_plan,"COMMENT",7);
        if ((icr==2)&&(retour!=0))
        {  

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/
          nom_fic[0]='\0';
          nom_fic_canal[0]='\0';

          if ((long int)strlen(repert)!=0)
          {
            sprintf(nom_fic_canal,"%s/%s",repert,nom_plan);
            }
          else
          {
            sscanf(nom_plan,"%s",nom_fic_canal);
          }
/*.......................................
       test sur l'existence du fichier plan 
       et ouverture du fichier plan
  .......................................*/
          num=SDstart(nom_fic_canal,DFACC_RDWR);
          if (num!= -1)
          {
              sscanf(nom_plan,"%s",deb);
  	      deb=deb+100;
              sscanf(nom_plan,"%s",deb1);
  	      deb1=deb1+100;
              image1->ACCES_CANAL_INT[nbplan]=num;

/*         Recuperation des informations generales */
/*.................................................*/
              retn=SDfileinfo(num,&nbdataset,&nfileattribut);

/*         Selection du Scientific Data Set contenant les donnees */
/*................................................................*/
              sds_id = SDselect(num,Ref);
              image1->POS_OCTET[nbplan]=sds_id;

/*         Recherche des attributs (taille, nb oct pix) */
/*......................................................*/

              status = SDgetinfo(sds_id,name,&nb_dim,val_dim,
					&num_type,&attribute);

/*         Verification que tous les plans ont la meme taille */
/*...........................................................*/
              if (nbplan!=0)
              {
	        if ((*nb_lig!=val_dim[0])||(*nb_col!=val_dim[1]))
                {            
		   sprintf( CAI_ERREUR ,"Plans de taille differentes !\0");
                   goto ERREUR;
                }
              }
              *nb_lig=val_dim[0];
              *nb_col=val_dim[1];
              nbbit= num_type;  

              nbplan=nbplan+1;

           }
           else
           {
 	     sprintf ( CAI_ERREUR ,
	       "Le fichier plan %s HDF n'existe pas\n" , nom_fic_canal );
             goto ERREUR;
           }
        }
        else
          icr=99;
      }
   }
/*....................................
  Decodage du nombre d'octets par pixel
  ......................................*/
  if ((retour==0)&&(icr!=99))
  {
    if (icr==2) 
       image1->NB_BITS_PIX=Ref;
    else
       image1->NB_BITS_PIX=0;
/*....................................
  Decodage du commentaire
  ......................................*/
     icr = fscanf(fic_in,"%s",label);
     retour=strncmp(label,"COMMENT",7);
     if (retour==0)
		icr=99;
  }
  else
       image1->NB_BITS_PIX=0;



/*....................................
  Decodage du commentaire
  ......................................*/
  if (icr==99)
     icr = fscanf(fic_in,"%s",image1->COMMENT);
  else
      strcpy(image1->COMMENT,"Pas de commentaire\0");

/*...................................*/
/* Initialisation du nombre de plans */
/*...................................*/
  *nb_can=nbplan;

/*..................................
   Initialisation du nombre d'octets par pixel
  ........................................................*/
/* MOD : VERSION : 5.2 : DM :  prise en compte des I2,I4,UI2,UI4 et R8       */
/*                             en lecture et ecriture                        */
/*****************************************************************************/

   if (nbbit==DFNT_INT8)
   {
	*nboct_pix=1;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=8;
	sprintf(image1->COD_PIX,"OCT");
   }
   else if (nbbit==DFNT_UINT8)
   {
	*nboct_pix=1;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=8;
	sprintf(image1->COD_PIX,"UOCT");

   }
   else if (nbbit==DFNT_INT16)
   {
	*nboct_pix=2;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=16;
	sprintf(image1->COD_PIX,"I2");
   }
   else if (nbbit==DFNT_UINT16)
   {
	*nboct_pix=2;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=16;
	sprintf(image1->COD_PIX,"UI2");
   }

/* MOD : 5.0 Prise en compte des images codées en I4 non signe*/
   else if (nbbit==DFNT_INT32)
   {
	*nboct_pix=4;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=32;
	sprintf(image1->COD_PIX,"I4");
   }
   else if (nbbit==DFNT_UINT32)
   {
	*nboct_pix=4;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=32;
	sprintf(image1->COD_PIX,"UI4");
   }
   else if (nbbit==DFNT_FLOAT)
   {
	*nboct_pix=4;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=32;
	sprintf(image1->COD_PIX,"R4");

   }
   else if (nbbit==DFNT_DOUBLE)
   {
	*nboct_pix=8;
        if (image1->NB_BITS_PIX==0)
           image1->NB_BITS_PIX=64;
	sprintf(image1->COD_PIX,"R8");

   }
   else
   {
 	 sprintf ( CAI_ERREUR ,
	   "Type de donnees non supporte par la Couche Acces Image\n" );
         goto ERREUR;
    }

 /*..................................
   Fermeture du fichier 
  ......................................*/
   if (fic_in!=NULL) fclose(fic_in);

/*......................
    sortie normale 
  ....................*/
      return(image1);

ERREUR:

/*......................
    sortie degradee 
  ....................*/
   if (fic_in!=NULL)    fclose(fic_in);
   for (plan=0;plan<nbplan;plan++)
   {
     num=image1->ACCES_CANAL_INT[plan];
     if (num!=-1) retn=SDend(num);
   }
   return (NULL);

}
/* 
   Fin de l'operation cai_ouvre_modifie_hdf16
*/

