/*****************************************************************************/
/*                                                                           */
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_grilles.c                                                 */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format gdb (Grille de Decalages Bruts) et au      */
/*	       format gdr (Grille de Decalages Restitues)		     */ 
/*                                                                           */
/* AUTEUR :      R.CHAISEMARTIN     (CISI)                                   */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :    Septembre 99                                        */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.1 :FA:Rajout des tests sur les retours d'allocation     */
/*			   memoire                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM :fonctions de test machine et inversions         */
/*			     deviennent generiques                           */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : modifications fopen binaire pour PC/NT         */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : modifications open binaire pour PC/NT          */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
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
# include <malloc.h>
# include <math.h>
# include <unistd.h>

#define _CAI_IMAGE
#define IN_CAI_GRIL

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_grilles.c,v 1.6 01/05/31 10:47:38 cmf Exp $";


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
/* OPERATION :    cai_liste_ima_grilles                                      */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de listage des images au format gdb                    */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  int cai_liste_ima_grilles(typ_grille,repert,tab_ima)  */
/* -----------------                                                         */
/*                                                                           */
/*     typ_grille   (E) (char)     : type de grille B=Brute, R=rectifiee     */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_grilles   (S) (int)     : = -1 repertoire inexistant    */                                    
/*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_ima_grilles (char typ_grille,char *repert,char ***tab_ima)
       
{
   DIR *dirp;		/* pointeur sur structure ouverture repertoire */
   struct dirent *dp;   /* structure resultat de la fonction readdir   */
   char *pch0,*pch1;    /* pointeurs sur chaine de caracteres          */
   char car;            /* variable de travail                         */  
   int nb_ima;          /* nombre d'images gdb identifiees             */
   long int i;          /* indice de boucle                            */
   long int ll,dif;     /* variables de travail                        */
   char blanc;          /* caractere blanc                             */
   char extens[5];      /* extension recherchee                        */

/*........................................
   INITIALISATION et ouverture du repertoire choisi
  .........................................*/
   extens[0]='\0';
   if (typ_grille=='B')
	strcpy(extens,".gdb");
   else
	strcpy(extens,".gdr");	
   nb_ima=0;
   blanc=' ';
   dirp=opendir(repert);

   if(dirp!=NULL)
   {
/*......................................

   premiere passe pour comptabiliser les images du format donne


   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE GDB ou GDR
      (EXTENSION =.gdb pour Brute ou .gdr pour Rectifiee)
     ......................................................*/

      while((dp=readdir(dirp))!=NULL)
      {
    	 pch0=strstr(dp->d_name,extens);
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

   closedir(dirp);
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT GDB ou GDR
     .......................................................................*/
    *tab_ima = (char **) calloc(nb_ima,sizeof(char *));
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

   POUR CHAQUE FICHIER DU REPERTOIRE ,
      TEST POUR SAVOIR SI CE FICHIER EST UNE IMAGE GDB ou GDR
       (EXTENSION =.gdb ou .gdr)
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/
   dirp=opendir(repert);

   nb_ima=0;
 
   while((dp=readdir(dirp))!=NULL)
   {
     pch0=strstr(dp->d_name,extens);
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
/* Fin de l'operation cai_liste_ima_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_grilles                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image format gdb ou gdr   */
/* -----                            					     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_grilles(typ_grille,      */
/*  ---------------- 		                            nom_image,       */
/*							    nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     typ_grille     (E) (char)   : type de grille B=Brute, R=rectifiee     */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels              */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_grilles (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_grilles(char typ_grille,
				     char *repert,
				     char *nom_image,
				     int  *nb_can,
				     int  *nboct_pix,
				     int  *nb_col,
				     int  *nb_lig)
{
   FILE *fic_in;		/* pointeur sur descripteur de fichier         */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image    */ 
   int  iret;			/* indicateur de retour ok ou pas              */
   char nom_fic[1024];		/* nom du fichier en-tete		       */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence   */
   int nbbit;			/* nb de bits par pixel du fichier en-tete     */
   int retour;			/* retour de la fonction comparaison chaines   */ 
   int i,num;			/* indice 				       */
   char type_cod[5];		/* type de codage du pixel		       */
   int temp;                    /* zone temporaire de lecture                  */
   char extens[5];              /* extension recherchee                        */
   char type_image[3];          /* type code+ordre rangement octets dans entete*/
   char *type_machine;          /* ordre rangement octets machine travail      */
   char  buf[59];               /* buffer contenant entete image               */
   int v;	                /* comparaison type machine-type image         */
   char *v1;                    /* comparaison type image avec chaine vide     */
   int nb_col2, nb_lig2;        /* variables intermediaires inversion octets   */
   int temp2,nb;                /* zone temporaire de lecture                  */

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
   *nb_can=0;

   *nb_lig=0;

   *nb_col=0;

   *nboct_pix=0;
 
   iret=1;
   
   type_machine = (char *) calloc(3,sizeof(char));
   
   if (type_machine == NULL)
   {
      iret=0;
      strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
   }
   
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

      extens[0]='\0';
      if (typ_grille =='B')
	strcpy(extens,".gdb");
      else
	strcpy(extens,".gdr");	

/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
      nom_fic[0]='\0';
      if ((long)strlen(repert)!= 0)
      {
          sprintf(nom_fic,"%s/%s%s",repert,nom_image,extens);
      }
      else
      {
         sprintf(nom_fic,"%s%s",nom_image,extens);
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
        strcpy(CAI_ERREUR,"Erreur ouverture fichier entete grille");
      }
      else
      {
      
    
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  

/*..................................
   recuperation infos completes format.gdb 
  ......................................*/ 
    
   if (typ_grille =='B')
   	fread(buf,36,1,fic_in);
	
/*..................................
   recuperation infos completes format.gdr 
  ......................................*/ 
   else fread(buf,56,1,fic_in);
   
/*..................................
   recuperation du type de codage des pixels 
  ......................................*/
   memcpy(type_cod,&buf[8],4);
	
        if (strncmp(type_cod,"FLOA",4) ==0)
	{
    	   image1->NB_BITS_PIX=32;
    	   image1->NBOCT_PIX=4;
	   sprintf(image1->COD_PIX,"R4");
           strcpy(type_image,"BE");
           sprintf(image1->TYPE_COD,"FLOA");
	}
	
	else if (strncmp(type_cod,"FLOL",4) ==0)
	{
	   image1->NB_BITS_PIX=32;
    	   image1->NBOCT_PIX=4;
	   sprintf(image1->COD_PIX,"R4");
           strcpy(type_image,"LE");
           sprintf(image1->TYPE_COD,"FLOL");
	}
	
	else 
	{
	  iret=0;
     	  sprintf(CAI_ERREUR,"Type de codage GRILLE non reconnu : %s",type_cod);
        }
	
	*nboct_pix = image1->NBOCT_PIX;
              
/*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/   
   
   	type_machine = cai_test_pFpf();  
 
 /*..................................
   determination des autres infos 
   en fonction inversion
  ......................................*/  
   
	v = strncmp(type_machine,type_image,2);
	v1 = strrchr(type_image,'E');
	  	  	
/*..................................
   recuperation du nombre de colonne 
  ......................................*/
	memcpy(nb_col,&buf[0],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/
        nb=2;
	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)nb_col, (void*)&nb_col2,4 );
		cai_inverser_tableau((short *)nb_col, (short *)&nb_col2,nb);
	}

/*..................................
   recuperation du nombre de lignes 
  ......................................*/
	memcpy(nb_lig,&buf[4],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) && (v1!=NULL) )
	{
		swab( (void*)nb_lig, (void*)&nb_lig2,4 );
		cai_inverser_tableau((short *)nb_lig, (short *)&nb_lig2, nb);
	}	      
 
/*.....................................................................
   recuperation du numero de ligne du 1er point de la matrice 
  .....................................................................*/
        
	memcpy(&temp,&buf[12],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
  
	image1->NUM_LIG_DEB = temp;
	
/*.....................................................................
   recuperation du numero de colonne du 1er point de la matrice 
  .....................................................................*/
	       
	memcpy(&temp,&buf[16],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2, nb);
	}
	
	image1->NUM_COL_DEB = temp;

/*.....................................................................
   recuperation du pas de la grille le long des lignes 
  .....................................................................*/
		       
	memcpy(&temp,&buf[20],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
	
	image1->PAS_GRIL_LIG = temp;

/*.....................................................................
   recuperation du pas de la grille le long des colonnes 
    .....................................................................*/
		       
	memcpy(&temp,&buf[24],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
	
	image1->PAS_GRIL_COL = temp;

/*.....................................................................
   recuperation du decalage initial le long des lignes 
  .....................................................................*/	
	       
	memcpy(&temp,&buf[28],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
	
	image1->DECAL_LIG = temp;

/*.....................................................................
   recuperation du decalage initial le long des colonnes  
   .....................................................................*/	
	       
	memcpy(&temp,&buf[32],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp,(short *) &temp2,nb);
	}
	
	image1->DECAL_COL = temp;

/*.....................................................................
   recuperation de la caracteristique de la grille si .gdr
.....................................................................*/
        if (typ_grille == 'R')
        {
	    memcpy(image1->CAR_GRIL,&buf[36],20);
            *nb_can = 2;
        }
        else
	    *nb_can = 3;
     }

/**************************/
/*    fin modifications   */ 
/**************************/    

/*..................................
   Fermeture du fichier 
  ......................................*/
      if (fic_in != NULL) fclose(fic_in);

      if (iret !=0)
      {
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

      
        nom_fic_canal[0]='\0';

        if ((long)strlen(repert)!= 0)
        {
          sprintf(nom_fic_canal,"%s/%s%s",repert,nom_image,extens);
        }
        else
        {
          sprintf(nom_fic_canal,"%s%s",nom_image,extens);
        }
                     
        num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
        if (num!= -1)
        {

          for (i=0;i<*nb_can;i=i+1)
          {
            sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
            sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
            image1->ACCES_CANAL_INT[i]=num;
            image1->POS_OCTET[i]=((i*(*nb_lig))+1)*(*nb_col)*image1->NBOCT_PIX;
          }
	  
	 	
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  
	
	  if ( (v!=0) && (v1!=NULL) )
	  {
	     image1->INV_OCTETS =1;
	  }
	  else
	  {
	     image1->INV_OCTETS =0;
	  }
	   
/**************************/
/*    fin modifications   */ 
/**************************/  
	  	  
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
   Fin de l'operation cai_ouvre_lecture_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_grilles                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format gdb ou gdr (grilles brutes ou restituees)             */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_grilles(typ_grille,      */
/*  ---------------- 		                            nom_image,       */
/*							    nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     typ_grille     (E) (char)   : type de grille B=Brute, R=rectifiee     */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_grilles (S) (CAI_IMAGE *) : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_grilles(char typ_grille,char *repert,
				      char *nom_image,
				      int  *nb_can,
				      int  *nboct_pix,
				      int  *nb_col,
				      int  *nb_lig,
				      char *label)

{
   FILE *fic;		      /* pointeur sur descripteur de fichier            */
   CAI_IMAGE *image1;         /* pointeur su descripteur de fichier image       */ 
   char nom_fic[1024];	      /* nom du fichier en-tete		                */
   long retour;		      /* retour de fonction 		                */ 
   int i,num;		      /* indice 			                */
   struct stat	buf_stat;     /* structure decrivant status de fichier          */
   unsigned char *Header;     /* entete du fichier gdb 	                        */
   int taille_entete;	      /* taille de l'entete	                        */
   int temp;                  /* zone temporaire pour ecriture                  */
   char extens[5];            /* extension recherchee                           */
   int Min_nbcol;             /* nombre de colonnes minimums                    */
   char *type_image;          /* type code+ordre rangement des octets ds entete */

    extens[0]='\0';
    if (typ_grille == 'B')
    {
        Min_nbcol = 9;  
	strcpy(extens,".gdb");

/******************************************************************/
/* Verification que les trois plans sont demandes		  */
/* 	-> plan1 : grille pour les decalages le long des lignes   */
/*	-> plan2 : grille pour les decalages le long des colonnes */
/*	-> plan3 : grille pour les indices de confiance		  */
/******************************************************************/
      if (*nb_can != 3)
      {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image GDB doit contenir trois plans");
	goto ERREUR;
       }
     }
     else
     {
/******************************************************************/
/* Verification que les deux plans sont demandes		  */
/* 	-> plan1 : grille pour les decalages le long des lignes   */
/*	-> plan2 : grille pour les decalages le long des colonnes */
/******************************************************************/
      if (*nb_can != 2)
      {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image GDR doit contenir deux plans");
	goto ERREUR;
       }
	strcpy(extens,".gdr");
        Min_nbcol = 14;  

     }
 
/**************************************************************/   
/* Verification que le nombre minimum de colonnes est demande */
/**************************************************************/
    if ((*nb_col)< Min_nbcol)
    {
	image1 = NULL;
 	sprintf ( CAI_ERREUR , 
		"Une image doit contenir au moins %d colonnes",Min_nbcol);
	goto ERREUR;

    }
    taille_entete = (*nb_col)*(*nboct_pix);
   
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
   Constitution du nom du fichier entete et verification qu'il
   n'existe pas deja
  .................................................*/

   nom_fic[0]='\0';
   if ((long)strlen(repert)!= 0)
   {
       sprintf(nom_fic,"%s/%s%s",repert,nom_image,extens);
   }
   else
   {
      sprintf(nom_fic,"%s%s",nom_image,extens);
    }
   
   retour = stat ( nom_fic , &buf_stat );
   if ( retour == 0 )
   { 
	sprintf ( CAI_ERREUR , "L'image %s GRILLE existe \n" , nom_image );
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
   Header = (unsigned char *) calloc(taille_entete,sizeof( unsigned char));
   if (Header == NULL)
   {
      sprintf(CAI_ERREUR,"Erreur alloc: Creation fichier %s impossible\n",
				nom_fic);
      goto ERREUR;
   }
   
/*********************************************/
/* ecriture du nombre de colonnes de l'image */
/*********************************************/
   memcpy(Header,nb_col , 4 );

/*******************************************/
/* ecriture du nombre de lignes de l'image */
/*******************************************/
   memcpy(&Header[4],nb_lig , 4 );

/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  
	 
/*****************************************/
/* ecriture type de l'image dans la zone */
/*    reservee au type de codage         */
/*****************************************/
   
  /*....................................
  test sur la machine de travail pour 
         typer image en LE ou BE      
  .....................................*/
  
  type_image = cai_test_pFpf();
  


/**************************************/
/* ecriture type de codage de l'image */
/**************************************/
  if (*nboct_pix != 4)
  {
      sprintf ( CAI_ERREUR ,
		 "Erreur : La grille foit etre codee en FLOAT\n");
      goto ERREUR;
   }

  else if ( (*nboct_pix == 4) && (strncmp(type_image,"BE",2)==0) )
  {
        memcpy(&Header[8],"FLOA" , 4 );
    	image1->NBOCT_PIX=4;
        sprintf(image1->TYPE_COD,"FLOA");
  }

  else if ( (*nboct_pix == 4) && (strncmp(type_image,"LE",2)==0) )
  {	
	memcpy(&Header[8],"FLOL" , 4 );
	image1->NBOCT_PIX=4;
        sprintf(image1->TYPE_COD,"FLOL");
   }
   	

/***********************/
/* fin modifications   */
/***********************/


/***************************************************************/
/* ecriture de l'entete ainsi constituee dans le fichier       */
/***************************************************************/
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
     image1->POS_OCTET[i]=((i*(*nb_lig))+1)*(*nb_col)*image1->NBOCT_PIX;
    }			 
  return (image1);

ERREUR :
  return(NULL);
}
  
/* 
  Fin de l'operation cai_ouvre_creation_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_grilles                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image GDB ou GDR           */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_grilles(image1,           */       		      
/* ------------------                        canal,premiere_ligne,           */
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
/*     cai_lecture_canal_grilles (S) (CAI_OK_KO) : = CAI_KO si pb            */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_grilles(CAI_IMAGE *image1 ,  
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
 long  oct_lig;             /* nb oct. par ligne en considerant taille pixel */
 unsigned char *buf_lec;    /* buffer lecture d'une  ligne                   */
 unsigned char *deb,*pch;   /* pointeur chaine pour transfert  buffer sortie */
 unsigned char *data_image2;/* pointeur donnees image pour inversion octets  */
 int taille_image;          /* taille en octets de l'image                   */ 
 int Nb;


/*...........................
  Initialisations diverses
  ..........................*/
   iret=CAI_OK;
   num=image1->ACCES_CANAL_INT[*canal-1];
   oct_pix=image1->NBOCT_PIX;
   taille_image = (*nombre_lignes_lire)*oct_pix*(*nombre_colonnes_lire);
   data_image2 = NULL;
   data_image2 = (unsigned char *) calloc(taille_image,sizeof(unsigned char));

   if (data_image2 == NULL)
   {
      strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_lecture_canal");
      iret=CAI_KO;
      goto FIN;
   }

/*.............................................................*/
/* verification si lecture non sous-echantillonnee par bandeau */
/*.............................................................*/
  if ((*nombre_colonnes_lire==image1->NB_COL)&&(*premiere_colonne==1)&&
      (*pas_ligne==1)&&(*pas_colonne==1))
  {
       oct_lig=(*nombre_lignes_lire)*oct_pix*(image1->NB_COL);
       pch=data_image;

/*............................................
	Recherche de la position courante dans le fichier
  ............................................*/
        pt_oct=lseek(num,0L,1);

/*................................
        Calcul du numero du 1er octet a lire
  ....................................*/
        lig=*premiere_ligne;  
	no_oct=image1->POS_OCTET[*canal-1]+
	          (lig-1)*image1->NB_COL*oct_pix;


/*..................................
        Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
        nb_oct=(long)(no_oct-pt_oct);
        lseek(num,nb_oct,1);

/*...........................................
        lecture des lignes completes
  ................................................*/
         n=read(num,pch,oct_lig);
 
         if (n<=0)
         {
             	iret=CAI_KO;
             	strcpy(CAI_ERREUR,"Erreur lecture donnee image grille");
	        goto FIN;

         }

   }
   else
   {
	oct_lig=oct_pix*image1->NB_COL;

/*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */
        buf_lec=NULL;
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
               
	      no_oct=image1->POS_OCTET[*canal-1]+
	          (lig-1)*image1->NB_COL*oct_pix;


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
             	strcpy(CAI_ERREUR,"Erreur lecture donnee image GRILLE");
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
                	for (k=0;k<oct_pix;k=k+1)
                	{
                  	  *(pch+k)=*(deb+k);
                 	}
                 	j=j+1;
                	 pch=pch+oct_pix;
                	 deb=deb+(*pas_colonne*oct_pix);
                  }   
             
              }
              i=i+1;
              lig=lig+(*pas_ligne);
           }        
          	    
	   free(buf_lec);
           buf_lec=NULL;
      	}
    }
        
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  
  
   Nb = taille_image/2;
     
   if ( (iret!= CAI_KO) && (image1->INV_OCTETS==1) )
   {
	
	   /*modification ordre des 4 octets par pixel*/
       	   swab( (void*)(data_image), (void*)(data_image2), taille_image);
	   cai_inverser_tableau(data_image, data_image2,Nb);
    }
       
/*************************/
/*   fin modifications   */    
/*************************/  
  FIN:  
   if (data_image2!= NULL) free(data_image2);
  return(iret);
}
/*
  Fin de l'operation cai_lecture_canal_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_grilles                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image gdb                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_grilles(image1, canal,   */
/* ------------------                                 premiere_ligne,        */
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
/*     cai_ecriture_canal_grilles    (S) (CAI_OK_KO) : = CAI_KO si pb        */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ecriture_canal_grilles (CAI_IMAGE *image1 ,  
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
int nboct_ecrire;		/* nb d'octets du bandeau a ecrire       */


/* ................
   Initialisations 
  .................*/

	num = image1->ACCES_CANAL_INT[*canal-1];
	nboct_pix = image1->NBOCT_PIX;
	position_first_lig = 0;
	position_courante = 0;
	deplacement = 0;
	nboct_ecrire = 0;

/********************************************************/
/* Calcul de la position de la premiere ligne a ecrire 	*/ 
/********************************************************/

	position_first_lig = (long) ( image1->POS_OCTET[*canal-1] + 
 		+ (*premiere_ligne - 1) * 
		(*nombre_colonnes_ecrire) * nboct_pix );


/********************************************************/
/* Recuperation de la position courante			*/
/********************************************************/

	position_courante = lseek ( num , 0L , 1 );


/********************************************************/
/* Calcul du deplacement jusqu'a la 1ere ligne a ecrire	*/
/********************************************************/

	deplacement = position_first_lig - position_courante;

/*******************************************************/
/*	deplacement jusqu'a la premiere ligne a ecrire*/
/*******************************************************/

	lseek ( num , deplacement , 1 ); 

/***********************************************/
/*	poistion en debut de bandeau a ecrire  */
/***********************************************/

	buff_lig = data_image; 

/********************************************************/
/*	ecriture des lignes				*/
/********************************************************/
	nboct_ecrire = (*nombre_colonnes_ecrire)*nboct_pix*
			(*nombre_lignes_ecrire);

	cr_write = write ( num , buff_lig,nboct_ecrire);

	if ( cr_write !=  nboct_ecrire)
	{
	   strcpy ( CAI_ERREUR , "Erreur ecriture canal image grille\n" );
	   goto ERREUR;
	}
 
return (CAI_OK);

ERREUR :

return (CAI_KO);


}
/* 
  Fin de l'operation cai_ecriture_canal_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_grilles                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format gdb  ou gdr         */
/* -----     					   			     */
/* Mise a Jour de l'entete si image ouverte en creation			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_grilles(typ_grille,image1)        */
/* ------------------                                                        */
/*									     */
/*     typ_grille  (E) (char)     : type de grille B=Brute, R=rectifiee      */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_grilles    (S) (CAI_OK_KO) :   = CAI_KO si pb               */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO  cai_ferme_grilles (char typ_grille,CAI_IMAGE *image1)

{

 int iret;    			/* code retour de l'operation         */
 int i_can;   			/* indice du canal a fermer           */
 int num;     			/* descripteur du fichier a fermer    */
 unsigned char *Header;         /* entete du fichier gdb 	      */
 int taille_entete;		/* taille de l'entete a mettre a jour */
 long pt_oct,tempi;		/* position courante ds fichier image */
 int temp,i;                    /* variable temporaire                */
 char *type_image;              /* ordre rangement octets dans entete */


/****************************/
/* Initialisations diverses */
/****************************/
iret=CAI_OK;
num=image1->ACCES_CANAL_INT[0];

/*--------------------------------------------------*/
/* Mise a jour de l'entete si ouverture en creation */
/*--------------------------------------------------*/

if (image1->MODE == CAI_ECRITURE)
{
   taille_entete = 46; 
   Header = (unsigned char *) calloc(taille_entete,sizeof( unsigned char));
   if (Header == NULL)
   {
      sprintf(CAI_ERREUR,
		"Erreur alloc: Mise a jour entete grille impossible\n");
      goto ERREUR;
   }
 
   pt_oct=lseek(num,0L,0);

/*******************************************************/
/* ecriture nombre de colonnes		               */
/*******************************************************/
   temp = image1->NB_COL;
   memcpy(Header,&temp , 4 );

/*******************************************************/
/* ecriture nombre de ligne			       */
/*******************************************************/
   temp = image1->NB_LIG;
   memcpy(&Header[4],&temp , 4 );

/*****************************************************************/
/* ecriture du type de codage en fonctions de la taille du pixel */
/* et du codage des Poids Forts, poids faibles                   */
/*****************************************************************/
   memcpy(&Header[8],image1->TYPE_COD , 4 );

   tempi = write ( num,Header , 12 );
   if ( tempi == 0 )
   {
      sprintf ( CAI_ERREUR ,
       "Erreur : MAJ entete a la fermeture impossible ds fichier grille\n");
      goto ERREUR;
   }
/***************************************************************/
/* ecriture du numero de ligne du 1er point de la matrice      */
/***************************************************************/
   temp = image1->NUM_LIG_DEB;
   memcpy(Header,&temp , 4 );
   

/***************************************************************/
/* ecriture du numero de ligne du 1er point de la matrice      */
/***************************************************************/
   temp = image1->NUM_COL_DEB;
   memcpy(&Header[4],&temp , 4 );
   

/***************************************************************/
/* ecriture du pas de la grille le long des lignes             */
/***************************************************************/
   temp = image1->PAS_GRIL_LIG;
   memcpy(&Header[8],&temp , 4 );
   

/***************************************************************/
/* ecriture du pas de la grille le long des colonnes           */
/***************************************************************/
   temp = image1->PAS_GRIL_COL;
   memcpy(&Header[12],&temp , 4 );
  

/***************************************************************/
/* ecriture du decalage initial le long des lignes             */
/***************************************************************/
   temp = image1->DECAL_LIG;
   memcpy(&Header[16],&temp , 4 );
   

/***************************************************************/
/* ecriture du decalage initial le long des colonnes           */
/***************************************************************/
   temp = image1->DECAL_COL;
   memcpy(&Header[20],&temp , 4 );
   

/******************************************************************/
/* ecriture de la caracteristique de la grille si gdr (restituee) */  
/******************************************************************/

  if (typ_grille == 'R')
  {
    memcpy(&Header[24],image1->CAR_GRIL, 20 );
      
  }

/***************************************************************/
/* mise a jour de l'entete ainsi constituee dans le fichier    */
/***************************************************************/
   tempi = write ( num, Header , taille_entete );
   free(Header);
   if ( tempi == 0 )
   {
      sprintf ( CAI_ERREUR ,
       "Erreur : MAJ entete a la fermeture impossible ds fichier grille\n");
      goto ERREUR;
   }
 }
/*.............................................
   Fermeture du fichier qui contient tous les canaux
  .............................................*/
  if (num>0) close(num);

  return (iret);

ERREUR:
  return(CAI_KO);
}

/*
  Fin de l'operation cai_ferme_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_grilles                                       */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stocke en gdb ou gdr        */
/* -----     				       			             */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_dest_ima_grilles(typ_grille,repertoire, */
/* ------------------      				nom_ima)             */
/* 									     */
/*     typ_grille   (E) (char)     : type de grille B=Brute, R=rectifiee     */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_grilles (S) (CAI_OK_KO) : = CAI_KO si pb                 */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_dest_ima_grilles ( char typ_grille,char *repertoire , 
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
char        extens[5];          /* extension recherchee         */

/*........................................
   INITIALISATION et ouverture du repertoire choisi
  .........................................*/
   extens[0]='\0';
   if (typ_grille=='B')
	strcpy(extens,".gdb");
   else
	strcpy(extens,".gdr");	

/************************************************/
/* Constitution du nom du fichier d'entete	*/
/************************************************/

if ((long)strlen(repertoire)!=0)
{
  sprintf( nom_ima_complet, "%s/%s" ,repertoire,nom_ima);
}
else
{
  strcpy ( nom_ima_complet , nom_ima);
}
  sprintf(nom_fic_entete ,"%s%s", nom_ima_complet,extens);

/************************************************/
/* Destruction du fichier d'entete		*/
/************************************************/

cr_stat = stat ( nom_fic_entete , &buf_stat );
if ( cr_stat == 0 )
	unlink ( nom_fic_entete );


return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_grilles
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_grilles                                  */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     gdb et de tous les fichiers qui la composent                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_grilles(typ_grille,      */
/*  ---------------- 		                            nom_image,       */
/*							    nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*     typ_grille     (E) (char)   : type de grille B=Brute, R=restituee     */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_grilles (S) (CAI_IMAGE *)  : = NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */   
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_grilles(char typ_grille,char *repert,
				     char *nom_image,
				     int  *nb_can,
				     int  *nboct_pix,
				     int  *nb_col,
				     int  *nb_lig)
{
   FILE *fic_in;		/* pointeur sur descripteur de fichier         */
   CAI_IMAGE *image1;		/* pointeur su descripteur de fichier image    */ 
   int  iret;			/* indicateur de retour ok ou pas              */
   char nom_fic[1024];		/* nom du fichier en-tete		       */
   char nom_fic_canal[1024];    /* nom fichier canal dont on teste existence   */
   int nbbit;			/* nb de bits par pixel du fichier en-tete     */
   int retour;			/* retour de la fonction comparaison chaines   */ 
   int i,num;			/* indice 				       */
   char type_cod[5];		/* type de codage du pixel		       */
   int temp;                    /* zone temporaire de lecture                  */
   char extens[5];
   char type_image[3];          /* type code+ordre rangement octets dans entete*/
   char *type_machine;          /* ordre rangement octets machine de travail   */
   char  buf[59];               /* buffer contenant entete image               */
   int v;                       /* comparaison type machine-type image         */
   char *v1;	                /* comparaison type image avec chaine vide     */
   int nb_col2, nb_lig2;        /* variables intermediaires inversion octets   */ 
   int temp2,nb;                /* zone temporaire de lecture                  */

   /*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
 nb=2;
 *nb_can=0;

 *nb_lig=0;

 *nb_col=0;

 *nboct_pix=0;
 
 iret=1;

 
 type_machine = (char *) calloc(3,sizeof(char));
   
 if (type_machine == NULL)
 {
    iret=0;
    strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_modifie");
 }

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
   extens[0]='\0';
   if (typ_grille == 'B')
      strcpy(extens,".gdb");
   else
      strcpy(extens,".gdr");
      
/*.................................................
   Constitution du nom du fichier entete
  .................................................*/
   nom_fic[0]='\0';
   if ((long)strlen(repert)!= 0)
   {
       sprintf(nom_fic,"%s/%s%s",repert,nom_image,extens);
   }
   else
   {
      sprintf(nom_fic,"%s%s",nom_image,extens);
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
     strcpy(CAI_ERREUR,"Erreur ouverture fichier entete GRILLE");
    }
   else
   {

    
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  
   
   /*..................................
   recuperation infos completes format.gdb 
  ......................................*/ 
    
   if (typ_grille =='B')
   	fread(buf,36,1,fic_in);
	
/*..................................
   recuperation infos completes format.gdr 
  ......................................*/ 
   else fread(buf,56,1,fic_in);
    
   
/*..................................
   recuperation du type de codage des pixels 
  ......................................*/
	memcpy(type_cod,&buf[8],4);
	
        if (strncmp(type_cod,"FLOA",4) ==0)
	{
    	   image1->NB_BITS_PIX=32;
    	   image1->NBOCT_PIX=4;
	   sprintf(image1->COD_PIX,"R4");
           strcpy(type_image,"BE");
           sprintf(image1->TYPE_COD,"FLOA");
	}
	
	else if (strncmp(type_cod,"FLOL",4) ==0)
	{
	   image1->NB_BITS_PIX=32;
    	   image1->NBOCT_PIX=4;
	   sprintf(image1->COD_PIX,"R4");
           strcpy(type_image,"LE");
           sprintf(image1->TYPE_COD,"FLOL");
	}
	
	else 
	{
	  iret=0;
     	  sprintf(CAI_ERREUR,"Type de codage GRILLE non reconnu : %s",type_cod);
        }
	
	*nboct_pix = image1->NBOCT_PIX;
              
/*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/   
   
   	type_machine = cai_test_pFpf();  
 
 /*..................................
   determination des autres infos 
   en fonction inversion
  ......................................*/  
   
	v = strncmp(type_machine,type_image,2);
	v1 = strrchr(type_image,'E');
	  	  		  	  	
/*..................................
   recuperation du nombre de colonne 
  ......................................*/
	memcpy(nb_col,&buf[0],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)nb_col, (void*)&nb_col2,4 );
		cai_inverser_tableau((short *)nb_col, (short *)&nb_col2,nb);
	}

/*..................................
   recuperation du nombre de lignes 
  ......................................*/
	memcpy(nb_lig,&buf[4],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) && (v1!=NULL) )
	{
		swab( (void*)nb_lig, (void*)&nb_lig2,4 );
		cai_inverser_tableau((short *)nb_lig, (short *)&nb_lig2,nb);
	}	      
/*.....................................................................
   recuperation du numero de ligne du 1er point de la matrice 
  .....................................................................*/
        
	memcpy(&temp,&buf[12],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp,(short *) &temp2,nb);
	}
  
	image1->NUM_LIG_DEB = temp;
/*.....................................................................
   recuperation du numero de colonne du 1er point de la matrice 
  .....................................................................*/
	       
	memcpy(&temp,&buf[16],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp,(short *) &temp2,nb);
	}
	
	image1->NUM_COL_DEB = temp;

/*.....................................................................
   recuperation du pas de la grille le long des lignes 
  .....................................................................*/
	
	       
	memcpy(&temp,&buf[20],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
	
	image1->PAS_GRIL_LIG = temp;

/*.....................................................................
   recuperation du pas de la grille le long des colonnes  
   .....................................................................*/
	
	       
	memcpy(&temp,&buf[24],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp,(short *) &temp2,nb);
	}
	
	image1->PAS_GRIL_COL = temp;

/*.....................................................................
   recuperation du decalage initial le long des lignes 
  .....................................................................*/
	
	       
	memcpy(&temp,&buf[28],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
	
	image1->DECAL_LIG = temp;

/*.....................................................................
   recuperation du decalage initial le long des colonnes  
   .....................................................................*/
		       
	memcpy(&temp,&buf[32],4);

/*modification si (type_image et type_machine st differentes) */
/*et si (type_image existe avec BE ou LE)*/

	if ( (v!=0) &&  (v1!=NULL) ) 
	{	
		swab( (void*)&temp, (void*)&temp2,4 );
		cai_inverser_tableau((short *)&temp, (short *)&temp2,nb);
	}
	
	image1->DECAL_COL = temp;

/*.....................................................................
   recuperation de la caracteristique de la grille si .gdr
.....................................................................*/
        if (typ_grille == 'R')
        {
	    memcpy(image1->CAR_GRIL,&buf[36],20);
            *nb_can = 2;
        }
        else
	    *nb_can = 3;
     }

/************************/
/*  fin modifications   */
/************************/   
   

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

     if ((long)strlen(repert)!= 0)
     {
       sprintf(nom_fic_canal,"%s/%s%s",repert,nom_image,extens);
      }
      else
      {
       sprintf(nom_fic_canal,"%s%s",nom_image,extens);
      }
  
     num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
     if (num!= -1)
     {

       for (i=0;i<*nb_can;i=i+1)
       {
           sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
           sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
            image1->ACCES_CANAL_INT[i]=num;
           image1->POS_OCTET[i]=((i*(*nb_lig))+1)*(*nb_col)*image1->NBOCT_PIX;
        }
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications indications Big ou Little Endian */
/*****************************************************************************/  
	
        if ( (v!=0) && (v1!=NULL) )
	{
	   image1->INV_OCTETS =1;
	}
	else
	{
	   image1->INV_OCTETS =0;
	}
	
/***********************/
/*  fin modifications  */        
/***********************/
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
   Fin de l'operation cai_ouvre_modifie_grilles
*/



