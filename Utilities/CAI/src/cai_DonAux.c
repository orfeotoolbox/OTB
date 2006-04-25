/*****************************************************************************/
/*                                                                           */
/* PROJET:   COUCHE ACCES IMAGE                                              */
/*                                                                           */
/*                                                                           */
/* NOM DU MODULE    :   CAI_DONAUX.c                                         */
/*                                                                           */
/* DATE DE CREATION:  Janvier 95                                             */
/* AUTEUR:  jl PASCAL   m RAYNAL       (CISI)                                */
/*                                                                           */
/* ROLE: ce module est charge de toutes les operations de lecture/ecriture   */
/*       sur les fichiers de donnees auxiliaires.			     */
/*	  il permet en outre de creer un nouveau fichier,ou de detruire un   */
/*       fichier existant                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* LANGAGE: C                                                                */
/* VERSION: V1                                                               */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM : modifications pour compatibilite HP64bits      */
/*****************************************************************************/
/* MOD : VERSION : 4.3 : DM : prototypage des fonctions                      */
/*****************************************************************************/
/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>   
#include <fcntl.h>
#include <malloc.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers de CONSTANTES (.cct)                */
/*---------------------------------------------------------------------------*/
#define _CAI_IMAGE
#define IN_CAI_DA

#include "cai_image.h"

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/
static char *rcs_id="$Header: cai_DonAux.c,v 1.10 01/01/15 15:24:40 cmf Exp $";

/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers de TYPES (.cty)                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                Inclusion des AUTRES fichiers                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                Definition des CONSTANTES propres                          */
/*---------------------------------------------------------------------------*/
#define maxi(a,b) ((a) > (b) ? (a) : (b))
#define mini(a,b) ((a) > (b) ? (b) : (a))
/*---------------------------------------------------------------------------*/
/*                Definition des TYPES propres                               */
/*---------------------------------------------------------------------------*/
typedef enum { CAI_DEBUT = 0 , CAI_ETAT31 = 1 } CAI_ETAT3;
typedef enum { CAI_INFERIEUR = 0 , CAI_SUPERIEUR = 1 , CAI_FIN = 2 , CAI_AUTRES = 3 } CAI_EVT3;
typedef enum { CAI_DEBUTDEC = 0 , CAI_ETAT11 = 1 , CAI_ETAT12 = 2 , CAI_ETAT13 = 3 ,
		 CAI_ETAT14 = 4 , CAI_FINDEC = 5 , CAI_BLOCVIDE=6 , CAI_ERR11 = 11 , CAI_ERR13 =13} CAI_ETAT1;
typedef enum { CAI_PAR_OUV = 0 , CAI_PAR_FERM = 1 , CAI_DONNEE = 2 , CAI_INF = 3 ,
		 CAI_BLANC = 4 } CAI_EVT1;
typedef enum { CAI_DEBUTYPE = 0 , CAI_ETAT21 = 1 , CAI_ETAT22 = 2 , CAI_ETAT23 = 3 , 
		 CAI_ENTIER = 4 , CAI_ASC = 5 , CAI_REEL = 6 , CAI_ERR21 = 11 } CAI_ETAT2;
typedef enum { CAI_POINT = 0 , CAI_GUILL = 1 , CAI_ESPACE = 2 , CAI_CHIFFRE = 3 ,
		 CAI_SIGNE = 4 , CAI_AUTRE = 5 } CAI_EVT2;



/*---------------------------------------------------------------------------*/
/*                Declaration des OPERATIONS INTERNES (static)               */
/*---------------------------------------------------------------------------*/
static CAI_OK_KO cai_lirecleDA(CAI_FILE_DA *fd,long  ad_deb,char * mot_cle, 
			       long *ad_suiv,long * ad_deb_mcc);
 static CAI_OK_KO cai_liredonneeDA(CAI_FILE_DA *fd,long ad_deb, void **don_lue, 
				  CAI_TYPE *type_lu,int *nb_lig_lu, 
  			          int *nb_col_lu, long *taille_cell_lu );
 static CAI_OK_KO cai_deplaceficDA(CAI_FILE_DA *fd ,long debut,long deplacement);
 static CAI_OK_KO cai_lireficDA(CAI_FILE_DA *fd,long  ad_deb,char *buff);
 static CAI_OK_KO cai_ecrireficDA(CAI_FILE_DA *fd,long ad_deb,
				  long taille, char **buff );
 static CAI_OK_KO cai_decodedonDA(long taille_buf, char buff[],long ad_deb,
			   CAI_TYPE *type_lu,int *nb_lig_lu,int *nb_elt_lu,
			   int *nb_elt_alloc,long *taille_cell_lu, 
  			   void  **don_lue, CAI_ETAT1 *etat1,long *ad_fin_dec);
 static CAI_OK_KO cai_analyseDA(char car, CAI_EVT1 *evt2);
 static CAI_OK_KO cai_typeDA(char car, CAI_EVT2 *evt2);
 static CAI_OK_KO cai_recherchecleDA(long taille_buf,char *buff,
				    long ad_deb,char *mot_cle,long *ad_suiv); 
 static CAI_OK_KO cai_mot_cleDA(char car, CAI_EVT3 *evt3);
 static CAI_OK_KO cai_ecritDA_buf(CAI_FILE_DA *fd,char *mot_cle,
				 CAI_TYPE type,int nb_lig,int nb_col,
			         long taille_cell, void *donnee,
				 int indic_ecrit,char **da_ascii);


/*---------------------------------------------------------------------------*/
/*                Definition des RESSOURCES IMPORTEES (extern)               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                Definition des RESSOURCES EXPORTEES                        */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                Definition des RESSOURCES PROPRES (static)                 */
/*---------------------------------------------------------------------------*/

static int autom1[6][5]={{ 1,11,11, 6, 0},
			 { 2,11,11,13, 1},
			 {11, 4, 3,13, 2},
			 {11, 4, 3,13, 3},
			 { 2, 5,11,13, 4}};

static int autom2[4][6]={{ 3, 2,11, 1, 1,11},
			 { 3,11, 4, 1,11,11},
			 { 2, 5, 2, 2, 2, 2},
			 { 3,11, 6, 3, 3, 3}}; 
   
static int autom3[2][4]={{  1, 0,  -1, 0},
			 { -25, 2, -24, 1}};

/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_openDA                                                     */
/*                                                                           */
/* ROLE:  Cette operation externe  est charge d'ouvrir le fichier de donnees */
/*	auxiliaires .Elle retourne une structure de type CAI_FILE_DA qui permet  */
/*      par la suite de manipuler le contenu du fichier .		     */
/*	mode d'ouverture  CAI_ECRITURE ou CAI_LECTURE            	     */	  
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*									     */
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO						     */
/*	" erreur a l'ouverture du fichier %s "				     */
/*	" erreur d'allocation memoire "					     */
/*****************************************************************************/
CAI_FILE_DA *cai_openDA(char *nom_fichier, char *type_image, 	
	    			  int nb_can,CAI_MODE_OUVERTURE mode_ouv)
{
int	flags;   /* flag utilise pour la fonction open */
int	mode;    /* valeur du umask utilise pou la fonction open */
int	fd;      /* descripteur du fichier ouvert */
CAI_FILE_DA *donaux; /* structure descriptive du fichier de donnees auxiliaires */
CAI_CHAINE fichier_da; /*fichier de donnees auxiliaires */
int	i;	 /* indice de boucle */
int 	canal;   /* flag indiquant si je suis en mono ou en multispectral */
int    trouve;	 /* flag indiquant si le format a ete reconnu */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
donaux = NULL;
strcpy(fichier_da,nom_fichier);
/*-- on concatene a la fin le CAI_SUF_FORMAT en fonction du format de l'image traitee --*/
if (nb_can ==1 )
	{
/*--- cas monospectral ---*/
	canal=0;
	}
else
	{
/*--- cas multispectral ---*/
	canal=1;
	}

/* reconnaissance du format  */
trouve=0;
for( i = 0 ; i< CAI_NB_FORMAT ; i++)
      {

       if(strcmp(type_image,CAI_NOM_FORMAT[i]) == 0 )
	       {
		strcat(fichier_da,CAI_SUF_FORMAT[canal][i]);		
		trouve=1;
	       }
      }
if (trouve == 0)
	{
	sprintf(CAI_ERREUR,"Erreur: format de l'image non reconnu ( %s )",type_image);
	goto ERREUR;
	}

/*-- on concatene a la fin du nom de fichier le CAI_SUF_FORMAT des donnees auxiliaires --*/
strcat(fichier_da,CAI_SUFFIXE_DA);

/*CD allocation dynamique de la structure de donnees auxiliaires */
donaux = (CAI_FILE_DA *)malloc(sizeof(CAI_FILE_DA));
if (donaux == NULL)
	{
	sprintf(CAI_ERREUR,"Erreur d'allocation a l'ouverture du fichier de donnees auxiliaires %s",fichier_da);
	goto ERREUR;
	}

/*CD en fonction du mode d'ouverture , ouverture du fichier		*/
if (mode_ouv == CAI_LECTURE) 
	{
	flags = O_RDONLY;
	fd = open(fichier_da,flags);
	}
else
	{
	flags = O_RDWR|O_CREAT;
	mode  = PERMS;
	fd = open(fichier_da,flags,mode);	
	}
/*CD si erreur on sort avec messsage " erreur a l'ouverture 		*/
if (fd == -1)
	{
	sprintf(CAI_ERREUR,"Erreur a l'ouverture du fichier de donnees auxiliaires %s",fichier_da);
         if (donaux != NULL)
	 {
	  free(donaux);
          donaux = NULL;

	  }

	goto ERREUR;
	}
else
	{
/*CD initialisations des pointeurs de la structure CAI_FILE_DA     	*/
/*CD ( buffers de lecture , liste de mots cles , longueurs ...)		*/ 
	strcpy(donaux->nom_fichier,fichier_da);
	donaux->descr = fd;
	donaux->buffer=NULL;
	donaux->taille_buffer = CAI_TAILLE_BUFFER;
	donaux->taille_buffer_lu=0;
	donaux->num_buffer=-1;
	donaux->liste_mot_cle=NULL;
	donaux->ad_mot_cle=NULL;
	donaux->ad_donnee=NULL;
	donaux->nb_mot_cle=-1;
	donaux->ad_dern_mot_cle=-1;
/*CD allocation dynamique du buffer */
	donaux->buffer = (char *)malloc(CAI_TAILLE_BUFFER + 1);
	if (donaux->buffer == NULL)
	{
		sprintf(CAI_ERREUR,"Erreur d'allocation a l'ouverture du fichier de donnees auxiliaires %s",fichier_da);
               if (donaux != NULL)
	       {
		free(donaux);
        	donaux = NULL;

	       }

	    goto ERREUR;
	}
	
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(donaux);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
  return(NULL);
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_closeDA                                                    */
/*                                                                           */
/* ROLE: Cette operation externe est chargee de fermer le fichier de donnees */
/* auxiliaires precedenment ouvert ( operation cai_openDA )		     */
/* la zone memoire reservee lors des operations precedentes dans la structure*/
/* CAI_FILE_DA 								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO						     */
/*	" erreur a la fermeture du fichier %s "				     */
/*****************************************************************************/
CAI_OK_KO cai_closeDA(CAI_FILE_DA *fd)

{
CAI_OK_KO	retour;		/* code retour appel fonction */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
retour = CAI_OK;

if (fd ==NULL)
	{
	retour = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Erreur a la fermeture d'un fichier de donnees auxiliaires ");
		goto ERREUR;
	}
close(fd->descr);
/*CD fermeture du fichier 						     */
/* desallocation des buffer alloues */
/*CD si le pointeurs de la structure CAI_FILE_DA sont <> NULL		     */
/*CD	la memoire allouee est liberee 					     */
if (fd->buffer != NULL)
	{
	free(fd->buffer);
        fd->buffer = NULL;
	}
if (fd->liste_mot_cle != NULL)
	{
	free(fd->liste_mot_cle);
        fd->liste_mot_cle = NULL;
   	}
if (fd->ad_mot_cle != NULL)
	{
	free(fd->ad_mot_cle);
        fd->ad_mot_cle= NULL;
	}
if (fd->ad_donnee != NULL)
	{
	free(fd->ad_donnee);
        fd->ad_donnee = NULL;
	}

free(fd);
fd = NULL;
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(retour);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas degrade  				     */
/*---------------------------------------------------------------------------*/
ERREUR:

return(retour);
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_removeDA                                                   */
/*                                                                           */
/* ROLE: Cette operation externe est charge de detruire un fichier de donnees*/
/*   auxiliaires.                                                            */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO				             */
/*	" erreur a la destrucution du fichier %s "			     */
/*****************************************************************************/
CAI_OK_KO cai_removeDA(char *nom_fichier,char *type_image,int nb_can)

{
int	cr; /* code retour */
CAI_CHAINE fichier_da; /*fichier de donnees auxiliaires */
int	i;	 /* indice de boucle */
int 	canal;   /* flag indiquant si je suis en mono ou en multispectral */
int    trouve;	 /* flag indiquant si le format a ete reconnu */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	         		     */
/*---------------------------------------------------------------------------*/
strcpy(fichier_da,nom_fichier);
/*-- on concatene a la fin le CAI_SUF_FORMAT en fonction du format de l'image traitee --*/
if (nb_can ==1 )
	{
/*--- cas monospectral ---*/
	canal=0;
	}
else
	{
/*--- cas multispectral ---*/
	canal=1;
	}

/* reconnaissance du format  */
trouve=0;
for( i = 0 ; i< CAI_NB_FORMAT ; i++)
      {
       if(strcmp(type_image,CAI_NOM_FORMAT[i]) == 0 )
	       {
		strcat(fichier_da,CAI_SUF_FORMAT[canal][i]);		
		trouve=1;
	       }
      }
if (trouve == 0)
	{
	sprintf(CAI_ERREUR,"Erreur: format de l'image non reconnu ( %s )",type_image);
	goto ERREUR;
	}

/*-- on concatene a la fin du nom de fichier le CAI_SUF_FORMAT des donnees auxiliaires --*/
strcat(fichier_da,CAI_SUFFIXE_DA);
cr=remove(fichier_da);
if (cr != 0)
	{
	sprintf(CAI_ERREUR,"destruction du fichier de donnees auxiliaires %s impossible",fichier_da);
	goto ERREUR;	
	}
return(CAI_OK);
ERREUR:
return(CAI_KO);
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_modifieDA                                                 */
/*                                                                           */
/* ROLE:  Cette operation modifie le contenu d'une donnee auxiliaire issue du*/
/*	fichier de DA.							     */
/*	Toutes les autres donnees sont conservees dans le fichier .Si le mot */
/*	cle n'est pas trouve, une erreur est detectee.			     */
/*      NB : le fichier doit au prealable avoir ete ouvert en mode CAI_ECRITURE  */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO						     */
/* "erreur fichier ouvert en mode lecture "				     */ 
/*****************************************************************************/
CAI_OK_KO cai_modifieDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE type,int nb_lig,int nb_col, 	
  	        	            long taille_cell,void *donnee)
{
int	i;	/* indice de boucle */
int	indic;  /* indicateur de recherche du mot cle a modifier */
long	p1;	/* pointeur sur le mot cle a modifier */
long	p2;	/* pointeur sur le mot cle suivant */
int	num_mot_cle; /* numero du mot cle a modifier */ 
CAI_OK_KO   cr;	/* code retour */
off_t   offset;	  	
off_t   offreel;
long	taille_init; /* taille en octets de la donnee a modifier */
long	taille_fin; /* taille en octets de la donnee a modifier */
int	indic_ecrit; /* indicateur d'ecriture dans un fichier */
char  * da_ascii;  /* pointeur sur la donnee formatee a ecrire */
long	deplacement; /* valeur du trou a faire dans le fichier */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
da_ascii=NULL;
/*CD on verifie au prealable que le fichier est effectivement ouvert */
/*CD SINON on sort en erreur */
if ( fd ==NULL ) 
	{
	cr = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Erreur ouverture du fichier de donnees auxiliaires");
	goto ERREUR;
	}
/*---------------------------------------------------------------------------*/
/*CD on appelle alors l'operation de liste de tous les mots cles 	     */
/*---------------------------------------------------------------------------*/
/*CD	si la liste n'existe pas					     */
if (fd->nb_mot_cle == -1)
	{
	cr=cai_listeDA(fd);
	if (cr != CAI_OK )
		{
		goto ERREUR;
		}		
	}

i=0;
indic=CAI_NON_TROUVE;

/*---------------------------------------------------------------------------*/
/*CD recherche du mot cle */
/*---------------------------------------------------------------------------*/
while (indic == CAI_NON_TROUVE && i < fd->nb_mot_cle)
	{
	if (strncmp(mot_cle,(char *)(fd->liste_mot_cle + i),CAI_TAILLE_MAX_CLEDA) == 0 )
		{
		indic = CAI_TROUVE; 
		num_mot_cle=i;
		}
	i++;
	}
/*---------------------------------------------------------------------------*/
/*CD SI le mot cle n'existe pas 					     */
/*---------------------------------------------------------------------------*/
if (indic == CAI_NON_TROUVE)
	{
/*CD	ALORS on sort en erreur        					     */
	cr=CAI_MC_INCONNU;
	sprintf(CAI_ERREUR,"Erreur : le mot cle %s n'existe pas dans le fichier",mot_cle);
	goto ERREUR; 
	}
else
	{
/*---------------------------------------------------------------------------*/
/*CD	SINON on recupere le pointeur sur le mot cle dans le fichier (p1)    */	
/*---------------------------------------------------------------------------*/
	p1 = *(fd->ad_mot_cle + num_mot_cle);
/*---------------------------------------------------------------------------*/
/*CD		ainsi que le pointeur sur le mot cle suivant (p2)	     */
/*---------------------------------------------------------------------------*/
/*CD	SI le mot cle a modifier est le dernier mot cle			     */
	if ( num_mot_cle == fd->nb_mot_cle -1)
		{
/*CD on se positionne sur la fin du fichier ( rewind ) 		     */
		offset=0;
		offreel=lseek(fd->descr,offset,SEEK_END);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
		if ( offreel == -1)
			{
			cr=CAI_PB_LSEEK;
			sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
			goto ERREUR;
			}
		offreel=lseek(fd->descr,offset,SEEK_CUR);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
		if ( offreel == -1)
			{
			cr=CAI_PB_LSEEK;
			sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
			goto ERREUR;
			}
		p2=offreel;
		}
	else
/*CD	SINON */
		{
		p2= *(fd->ad_mot_cle + num_mot_cle+1);
		}
/*CD	FIN SI le mot cle a modifier est le dernier mot cle		     */
/*---------------------------------------------------------------------------*/
/*CD		la taille de la donnee a modifier est p2 - p1 octets ti      */
/*---------------------------------------------------------------------------*/
	taille_init = p2 - p1;
/*---------------------------------------------------------------------------*/
/*CD		on formate dans une chaine de caracteres  la donnee a ecrire */
/*CD		(operation interne)( terminee par un \0)		     */
/*---------------------------------------------------------------------------*/
	indic_ecrit=0;
	cr=cai_ecritDA_buf(fd,		/* [E] file descriptor du fichier de DA 	*/
		   	mot_cle,	/* [E] mot cle associe aux donnees auxiliaires 	*/
		        type,		/* [E] type des donnees a ecrire		*/
		        nb_lig,		/* [E] nombre de lignes du tableau a ecrire	*/
		        nb_col,		/* [E] nombre de colonnes du tableau a ecrire	*/
		        taille_cell,	/* [E] taille maxi en octets d'un element	*/
					/*     (significatif si type = ascii)		*/
		   	donnee,	        /* [E] pointeur sur la zone memoire a ecrire	*/
		        indic_ecrit,	/* [E] indicateur si ecriture du buffer dans 	*/
					/*     le fichier (=1) sinon (=0)	        */
		   	&da_ascii	/* buffer de la DA en ascii (sous forme 	*/
					/* de x fois CAI_TAILLE_MAX_LIGDA caracteres)	*/
		  );
	if (cr != CAI_OK)
		{
		goto ERREUR;
		}
/*CD		la taille de la donnee a ecrire est la taille de la chaine tf*/
	taille_fin=(long)strlen(da_ascii);	
/*---------------------------------------------------------------------------*/
/*CD		on cree alors un trou dans le fichier de DA de taille  tf-ti */
/*CD		en commencant a l'adresse p2 ( operation interne )	     */
/*---------------------------------------------------------------------------*/
	deplacement= taille_fin -taille_init;
	cr=cai_deplaceficDA( 
				fd  ,	  /* [E] file descripteur du fichier ouvert en lecture ecriture  */
			        p2,     /* [E] adresse de debut de la zone a deplacer */
			        deplacement/* [E] valeur deplacement <0 on racourcit le fichier  */
					  /*     valeur deplacement >0 on allonge le fichier    */
			   );
	if (cr != CAI_OK)
		{
		goto ERREUR;
		}

/*---------------------------------------------------------------------------*/
/*CD		on ecrit alors la donnee a ecrire a partir de l'adresse p1   */
/*CD			(operation interne qui ecrit directement la chaine ) */
/*CD			de caracteres precedente)			     */
/*---------------------------------------------------------------------------*/
	cr=cai_ecrireficDA(
			     fd,      /* [E] structure decrivant le fichier de donnees auxiliaires  */
			     p1,      /* [E] adresse dans le fichier ou ecrire le buffer */ 
			     taille_fin,      /* [E] taille de la chaine ascii a ecrire */
         	      	     &da_ascii        /* [E] chaine a ecrire*/
		   );
	if (cr != CAI_OK)
		{
		goto ERREUR;
		}
/*CD		liberation de la chaine de caracteres allouee		     */
	if ( da_ascii != NULL)
		{
		free(da_ascii);
	        da_ascii = NULL;
		}
	}
/*CD    FINSI le mot cle existe						     */		
/*---------------------------------------------------------------------------*/
/*CD on met a jour la structure CAI_FILE_DA ( buffer , liste ...)		     */ 
/*---------------------------------------------------------------------------*/
fd->num_buffer=-1;
fd->nb_mot_cle=-1;
if (fd->liste_mot_cle != NULL)
	{
	free(fd->liste_mot_cle);
        fd->liste_mot_cle=NULL;
	}
if (fd->ad_mot_cle != NULL)
	{
	free(fd->ad_mot_cle);
	fd->ad_mot_cle= NULL;
	}
if (fd->ad_donnee != NULL)
	{
	free(fd->ad_donnee);
        fd->ad_donnee= NULL;
	}
fd->ad_dern_mot_cle=-1;
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
if (da_ascii != NULL)
	{
	free(da_ascii);
         da_ascii=NULL;
	}
/*---------------------------------------------------------------------------*/
/*CD on met a jour la structure CAI_FILE_DA ( buffer , liste ...)		     */ 
/*---------------------------------------------------------------------------*/
fd->num_buffer=-1;
fd->nb_mot_cle=-1;
if (fd->liste_mot_cle != NULL)
	{
	free(fd->liste_mot_cle);
        fd->liste_mot_cle = NULL;
	}
if (fd->ad_mot_cle != NULL)
	{
	free(fd->ad_mot_cle);
	fd->ad_mot_cle=NULL;
	}
if (fd->ad_donnee != NULL)
	{
	free(fd->ad_donnee);
	fd->ad_donnee=NULL;
	}
fd->ad_dern_mot_cle=-1;
return(cr);
/*---------------------------------------------------------------------------*/
/*		 	  FIN  DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_detruitDA                                                 */
/*                                                                           */
/* ROLE:  Cette operation detruit le contenu d'une donnee auxiliaire issue du*/
/*	fichier de DA.							     */
/*	Toutes les autres donnees sont conservees dans le fichier .	     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO					     */
/* "erreur fichier ouvert en mode lecture "				     */ 
/*****************************************************************************/
CAI_OK_KO cai_detruitDA(CAI_FILE_DA *fd,char *mot_cle)

{
int	i;	/* indice de boucle */
int	indic;  /* indicateur de recherche du mot cle a modifier */
long	p1;	/* pointeur sur le mot cle a modifier */
long	p2;	/* pointeur sur le mot cle suivant */
int	num_mot_cle; /* numero du mot cle a modifier */ 
CAI_OK_KO   cr;	/* code retour */
off_t   offset;	  	
off_t   offreel;
long	taille_init; /* taille en octets de la donnee a modifier */
long	deplacement; /* valeur du trou a faire dans le fichier */ 
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD on verifie au prealable que le fichier est effectivement ouvert */
/*CD SINON on sort en erreur */
if ( fd ==NULL ) 

	{
	cr = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Erreur ouverture du fichier de donnees auxiliaires");
	goto ERREUR;
	}
/*---------------------------------------------------------------------------*/
/*CD on appelle alors l'operation de liste de tous les mots cles 	     */
/*---------------------------------------------------------------------------*/
/*CD	si la liste n'existe pas					     */
if (fd->nb_mot_cle == -1)
	{
	cr=cai_listeDA(fd);
	if (cr != CAI_OK )
		{
		goto ERREUR;
		}		
	}

i=0;
indic=CAI_NON_TROUVE;
/*---------------------------------------------------------------------------*/
/*CD recherche du mot cle */
/*---------------------------------------------------------------------------*/
while (indic == CAI_NON_TROUVE && i < fd->nb_mot_cle)
	{
	if (strncmp(mot_cle,(char *)(fd->liste_mot_cle + i),CAI_TAILLE_MAX_CLEDA) == 0 )
		{
		indic = CAI_TROUVE; 
		num_mot_cle=i;
		}
	i++;
	}
/*---------------------------------------------------------------------------*/
/*CD SI le mot cle n'existe pas 					     */
/*---------------------------------------------------------------------------*/
if (indic == CAI_NON_TROUVE)
	{
/*CD	ALORS on sort en erreur        					     */
	cr=CAI_MC_INCONNU;
	sprintf(CAI_ERREUR,"Erreur : le mot cle %s n'existe pas dans le fichier",mot_cle);
	goto ERREUR; 
	}
else
	{
/*---------------------------------------------------------------------------*/
/*CD	SINON on recupere le pointeur sur le mot cle dans le fichier (p1)    */	
/*---------------------------------------------------------------------------*/
	p1 = *(fd->ad_mot_cle + num_mot_cle);
/*---------------------------------------------------------------------------*/
/*CD		ainsi que le pointeur sur le mot cle suivant (p2)	     */
/*---------------------------------------------------------------------------*/
/*CD	SI le mot cle a detruire est le dernier mot cle			     */
	if ( num_mot_cle == fd->nb_mot_cle -1)
		{
/*CD on se positionne sur la fin du fichier ( rewind ) 		     */
		offset=0;
		offreel=lseek(fd->descr,offset,SEEK_END);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
		if ( offreel == -1)
			{
			cr=CAI_PB_LSEEK;
			sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
			goto ERREUR;
			}
		offreel=lseek(fd->descr,offset,SEEK_CUR);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
		if ( offreel == -1)
			{
			cr=CAI_PB_LSEEK;
			sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
			goto ERREUR;
			}
		p2=offreel;
		}
	else
/*CD	SINON */
		{
		p2= *(fd->ad_mot_cle + num_mot_cle+1);
		}
/*CD	FIN SI le mot cle a detruire est le dernier mot cle		     */
/*---------------------------------------------------------------------------*/
/*CD		la taille de la donnee a detruire est p2 - p1 octets ti      */
/*CD		on cree alors un trou dans le fichier de DA de taille ti(<0) */
/*CD		en commencant a l'adresse p2 ( operation interne )	     */
/*---------------------------------------------------------------------------*/
	taille_init = p2 - p1;
	deplacement= -taille_init;
	cr=cai_deplaceficDA( 
				fd  ,	  /* [E] file descripteur du fichier ouvert en lecture ecriture  */
			        p2,     /* [E] adresse de debut de la zone a deplacer */
			        deplacement/* [E] valeur deplacement <0 on racourcit le fichier  */
					  /*     valeur deplacement >0 on allonge le fichier    */
			   );
	if (cr != CAI_OK)
		{
		goto ERREUR;
		}

	}
/*CD    FINSI le mot cle existe						     */		
/*---------------------------------------------------------------------------*/
/*CD on met a jour la structure CAI_FILE_DA ( buffer , liste ...)		     */ 
/*---------------------------------------------------------------------------*/
fd->num_buffer=-1;
fd->nb_mot_cle=-1;
if (fd->liste_mot_cle != NULL)
	{
	free(fd->liste_mot_cle);
        fd->liste_mot_cle=NULL;
	}
if (fd->ad_mot_cle != NULL)
	{
	free(fd->ad_mot_cle);
	fd->ad_mot_cle=NULL;
	}
if (fd->ad_donnee != NULL)
	{
	free(fd->ad_donnee);
	fd->ad_donnee=NULL;
	}
fd->ad_dern_mot_cle=-1;
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
fd->num_buffer=-1;
fd->nb_mot_cle=-1;
if (fd->liste_mot_cle != NULL)
	{
	free(fd->liste_mot_cle);
        fd->liste_mot_cle=NULL;

	}
if (fd->ad_mot_cle != NULL)
	{
	free(fd->ad_mot_cle);
	fd->ad_mot_cle=NULL;

	}
if (fd->ad_donnee != NULL)
	{
	free(fd->ad_donnee);
	fd->ad_donnee=NULL;

	}
fd->ad_dern_mot_cle=-1;
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
return(cr);
/*---------------------------------------------------------------------------*/
/*		 	  FIN  DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_caractDA                                                  */
/*                                                                           */
/* ROLE:  cette operation externe est chargee de trouver le type et la taille*/
/*   d'une donnee auxiliaire lue dans un fichier precedenment ouvert.        */
/*   Il s'agit donc apres localisation du mot cle de lire le type de la donne*/
/*    puis de compter le nombres de cellules de ce type dans la ligne.       */
/*   Dans le cas d'une donnee de type CAI_ASCII la taille maxi de la cellule est */
/*   aussi indiquee a l'appelant.                                            */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/* cette operation doit etre appele apres ouverture prealable du fichier     */
/* de donnees auxiliaires.						     */  
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO						     */
/*   si CAI_KO l'erreur peut etre affiche via la variable globale CAI_ERREUR     */
/*   " fichier de donnees auxiliaires %s non ouvert"			     */ 
/*   " mot cle non present dans le fichier de donnees auxiliaires %s"        */
/*   " aucun mot cle dans le fichier %s"			             */
/*   " taille du mot cle lue %s superieure a la limite %d ou egale a 0"      */ 
/*   " erreur a la lecture du fichier de donnees auxiliaires %s"             */
/*   " parenthese ouvrante non detectee (mot cle %s )                        */ 
/*   " parenthese fermante  non detectee (mot cle %s )                       */
/*   " nombre de niveaux de parentheses superieur a 2 ( mot cle %s )         */ 	
/*   " % dimension du tableau non constante (mot cle %s)                     */
/*   " % dimension du tableau = 0     (mot cle %s)                           */
/*   " type de la donnee lue %s non reconnue (mot cle %s)                    */
/*   " erreur lors du decodage de la donnee lue %s" (mot cle)                */
/*****************************************************************************/
CAI_OK_KO cai_lirecaractDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE *type,int *nb_lig,int *nb_col, 	
  	        	            long *taille_cell)
{
CAI_MOT_CLE mot_cle_lu;    /* mot cle courant lu dans le fichier */
int	indic_mot_cle; /* indicateur de recherche du mot cle */
int	num_mot_cle;   /* numero du mot cle recherche */
long	ad_mot_cle;    /* adresse du debut du mot cle courant */	
long    ad_donnee;     /* adresse de la donnee apres le mot cle trouve */ 		
long	adresse_deb;   /* adresse de debut de recherche du mot cle */
CAI_OK_KO	cr;	       /* code retour des operations */		
void	*don_lue;      /* donnees lue en interne par l'operation cai_liredonneeDA */
int	i;	       /* indice de boucle */	
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD initialisations  */
indic_mot_cle = CAI_NON_TROUVE;
num_mot_cle=0;
i=0;
ad_donnee=0;
cr=CAI_OK;
don_lue=NULL;

/*CD on verifie au prealable que le fichier est effectivement ouvert */
/*CD SINON on sort en erreur */
if ( fd ==NULL ) 

	{
	cr = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Erreur ouverture du fichier de donnees auxiliaires");
	goto ERREUR;
	}

/*CD SI la liste des mots cles est en memoire 			*/
if ( fd->nb_mot_cle != -1 ) 
	{
/*---------------------------------------------------------------------------*/
/*CD ALORS on scrute cette liste jusqu'a trouver le mot cle */
/*---------------------------------------------------------------------------*/
	while (i< fd->nb_mot_cle && indic_mot_cle == CAI_NON_TROUVE )
		{
		if ( strncmp(mot_cle,(char *)(fd->liste_mot_cle + i),CAI_TAILLE_MAX_CLEDA) == 0 )
			{
			indic_mot_cle = CAI_TROUVE;
			num_mot_cle    = i;
/*CD	   on recupere alors le pointeur sur la zone fichier apres ce mot cle */
			ad_donnee=*(fd->ad_donnee + i);
			strcpy(mot_cle_lu,(char *)(fd->liste_mot_cle +i));
			ad_mot_cle=*(fd->ad_mot_cle + i);
			}
		i++;
		}
	}
else
	{
/*CD SINON							*/	
/*CD   on se positionne au debut du fichier ( adresse_deb = 0)  */
	adresse_deb = 0;	
/*---------------------------------------------------------------------------*/
/*CD   on cherche le premier mot cle a partir de cette adresse (operation interne basee sur un automate) */
/*---------------------------------------------------------------------------*/
	cr = cai_lirecleDA(fd,     /* [E] file descriptor du fichier a lire  */
        	      	   adresse_deb,   /* [E] adresse de debut de recherche */
	        	   mot_cle_lu,  /* [S] mot cle suivant lu ( termine par \0)*/
		           &ad_donnee, /* [S] adresse du premier caractere suivant le mot cle */
		           &ad_mot_cle /* [S] adresse du premier caractere du mot cle */
 		   	  );
/*CD    si erreur on sort */
	if (cr != CAI_OK && cr != CAI_EOF) {
			goto ERREUR;
			}	
/*CD   si aucun mot cle trouve on sort en erreur "aucun mot cle dans le fichier" */  
	if (cr == CAI_EOF)
		{
		sprintf(CAI_ERREUR,"Erreur fichier sans mot cle( %s) ",fd->nom_fichier);
		goto ERREUR;
		}
/*CD   TANT QUE  mot_cle non trouve &  pas erreur */
	while ( cr == CAI_OK  && indic_mot_cle == CAI_NON_TROUVE )
		{
/*CD   	  si le mot cle est egal au mot cle demande alors mot_cle trouve */
		if ( strcmp(mot_cle,mot_cle_lu) == 0)
			{
			indic_mot_cle = CAI_TROUVE;
			}
		else
			{
			adresse_deb = ad_donnee;
/*---------------------------------------------------------------------------*/
/*CD      si mot_cle non trouve on cherche le mot cle suivant (operation interne basee sur un automate) */
/*CD      cette operation necessite : un pointeur sur la zone fichier ou l'on recherche le mot cle suivant */
/*CD      cette operation ramene : le mot cle suivant et le pointeur sur la zone fichier apres ce mot cle */
/*---------------------------------------------------------------------------*/
			cr = cai_lirecleDA(fd,     /* [E] file descriptor du fichier a lire  */
        	      	   		   adresse_deb,   /* [E] adresse de debut de recherche */
	        	   		   mot_cle_lu,  /* [S] mot cle suivant lu (termine par \0)*/
		           		   &ad_donnee, /* [S] adresse du premier caractere suivant le mot cle */
		           		   &ad_mot_cle/* [S] adresse du premier caractere du mot cle */
 		   	  		   );
			}
/*CD   FIN TANT QUE pas erreur & mot_cle non trouve */
		}
	}
/*CD FINSI */
if (cr != CAI_OK && cr != CAI_EOF) 
	{
	goto ERREUR;
	}
/*CD 	SI fin de fichier on sort en erreur "mot cle non present */
if ( indic_mot_cle == CAI_NON_TROUVE ) 
	{
	sprintf(CAI_ERREUR,"Mot cle :%s non trouve dans le fichier :%s",mot_cle,fd->nom_fichier);
	goto ERREUR;
	}
/*CD on sauvegarde alors dans la structure fd les info sur le dernier mot cle */
strcpy(fd->dernier_mot_cle,mot_cle_lu);
fd->ad_dern_mot_cle = ad_mot_cle;
fd->ad_dern_donnee  = ad_donnee;
/*---------------------------------------------------------------------------*/
/*CD a partir du mot cle trouve on analyse syntaxiquement la ligne jusqu'au prochain mot cle */
/*CD (operation interne effectuant le decodage et donnant : le type de donnee , le nombres de lignes */
/*CD le nombre de colonnes , la taille maxi d'un element , un pointeur sur le tableau lu */
/*CD ( ce tableau a ete alloue dynamiquement par cette operation) 		*/
/*---------------------------------------------------------------------------*/
cr=cai_liredonneeDA(
			fd,         /* [E] file descriptor du fichier a lire  */
        	      	ad_donnee,  /* [E] adresse de debut de recherche */
	        	&don_lue,    /* [S] donnee lue */
			type,    /* [S] type de la donnee lue (CAI_ENTIER4,CAI_REEL8,CAI_ASCII,CAI_BLOC)*/
			nb_lig,  /* [S] nombre de lignes du tableau lu  */
			nb_col,  /* [S] nombre de colonnes du tableau lu  */  
		        taille_cell/* [S] taille maxi en octet d'un element du tableau lu ( significatif
					 pourla donnee CAI_ASCII )*/   
		      );

/*CD si erreur de decodage on sort en erreur */
if (cr != CAI_OK ) {
	if (cr == CAI_EOF ){
		sprintf(CAI_ERREUR,"erreur fichier de DA incomplet (mot cle %s)",mot_cle);
		}
	goto ERREUR;
	}
/*CD liberation place memoire alloue par l'operation interne precedente  */
if (don_lue != NULL)
	{
	free(don_lue);
	don_lue=NULL;
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
/*CD liberation place memoire alloue par l'operation interne precedente  */
if (don_lue != NULL)
	{
	free(don_lue);
	don_lue=NULL;
	}
return(cr);

/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_lireDA                                                    */
/*                                                                           */
/* ROLE:  Cette operation externe est chargee de lire une donne associee a un*/
/*	mot cle dans le fichier de donnees auxiliaires precedenment ouvert.  */
/*        Il s'agit donc apres localisation du mot cle de lire la donnee et  */
/*    toutes les informations associees(nb_lig,nb_col,taille_cell).	     */
/*    	  On verifie alors la coherence entre les informations lues et celles*/
/*    issues des arguments donnes par l'appelant.			     */
/*	  Si les 3 informations sont coherentes la donnee est recopiee dans  */
/*    la zone memoire reservee par l'appelant.				     */ 	    		
/* 	                                                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/* cette operation doit etre appele apres ouverture prealable du fichier     */
/* de donnees auxiliaires.						     */
/* pour une souplesse d'utilisation maximum il est utilise d'appeler au      */
/* prealable l'operation  cai_lirecaractDA puis de reserver la place memoire */
/* necessaire                           				     */ 
/*                                                                           */
/* VERSION:                                                                  */
/* CODES RETOUR: CAI_OK ou CAI_KO				             */
/*   si CAI_KO l'erreur peut etre affiche via la variable globale CAI_ERREUR */
/*   " fichier de donnees auxiliaires %s non ouvert"			     */ 
/*   " mot cle non present dans le fichier de donnees auxiliaires %s"        */
/*   " aucun mot cle dans le fichier %s"			             */
/*   " taille du mot cle lue %s superieure a la limite %d ou egale a 0"      */ 
/*   " erreur a la lecture du fichier de donnees auxiliaires %s"             */
/*   " parenthese ouvrante non detectee (mot cle %s )                        */ 
/*   " parenthese fermante  non detectee (mot cle %s )                       */
/*   " nombre de niveaux de parentheses superieur a 2 ( mot cle %s )         */ 	
/*   " % dimension du tableau non constante (mot cle %s)                     */
/*   " % dimension du tableau = 0     (mot cle %s)                           */
/*   " type de la donnee lue %s non reconnue (mot cle %s)                    */
/*   " erreur lors du decodage de la donnee lue %s" (mot cle %s)             */
/*   " incoherence entre nblig lue (%d) et nblig specifiee (%d) (mot cle %s) */  	
/*   " incoherence entre nbcol lue (%d) et nbcol specifiee (%d) (mot cle %s) */ 
/*   " incoherence entre taille_cell lue (%d) et taille_cell  specifiee (%d) */
/*            (mot cle %s) 						     */  	
/*****************************************************************************/
CAI_OK_KO cai_lireDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE *type,int *nb_lig,int *nb_col, 	
  	        	            long *taille_cell,void *donnee)
{
int	i;	       /* indice de boucle */
int     j;	       /* indice de boucle */	 	
CAI_MOT_CLE mot_cle_lu;    /* mot cle courant lu dans le fichier */
int	indic_mot_cle; /* indicateur de recherche du mot cle */
int	num_mot_cle;   /* numero du mot cle recherche */
long	ad_mot_cle;    /* adresse du debut du mot cle courant */	
long    ad_donnee;     /* adresse de la donnee apres le mot cle trouve */ 		
long	adresse_deb;   /* adresse de debut de recherche du mot cle */
CAI_OK_KO	cr;	       /* code retour des operations */		
void	*don_lue;      /* donnees lue en interne par l'operation cai_liredonneeDA */	
char    *don_asc;      /* donnee ascii lue */
long int *don_ent;     /* donnee entiere lue */				
double   *don_ree;     /* donnee reelle lue */
CAI_TYPE	type_lu;       /* type de la donnee lue CAI_ENTIER4 ou CAI_REEL8 ou CAI_BLOC*/
int	nb_lig_lu;     /* nombre de lignes lues */
int     nb_col_lu;     /* nombre de colonnes lues */
long	taille_cell_lu;/* taille de la cellule CAI_ASCII maxi lue */
char    *donnee_a;     /* donnee ascii lue */
long int  *donnee_li;  /* donnee entiere a renseigner*/				
double    *donnee_d;   /* donnee reelle  a renseigner*/
float     *donnee_f;   /* donnee reelle  a renseigner*/
double    reel;	       /* variable de travail */	 
long int  entier;      /* variable de travail */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD initialisations */
indic_mot_cle = CAI_NON_TROUVE;
num_mot_cle=0;
i=0;
j=0;
ad_donnee=0;

cr=CAI_OK;
don_lue=NULL;
nb_lig_lu=0;
nb_col_lu=0;
type_lu= CAI_INCONNU;
taille_cell_lu=0;
/*CD on verifie que le type fait partie des types reconnus */
if ( *type != CAI_ENTIER4 && *type != CAI_REEL4 && *type != CAI_REEL8 && *type  != CAI_ASCII && *type !=CAI_BLOC)
	{
	cr = CAI_ERR_TYP;
	sprintf(CAI_ERREUR,"type :%d non reconnu ( 1:CAI_ENTIER4 2:CAI_REEL4 3:CAI_REEL8 4:CAI_ASCII 5:CAI_BLOC)",*type);
	goto ERREUR;
	} 
/*CD on verifie au prealable que le fichier est effectivement ouvert */
/*CD SINON on sort en erreur */
if ( fd ==NULL ) 

	{
	cr = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Erreur ouverture du fichier de donnees auxiliaires");
	goto ERREUR;
	}

/*---------------------------------------------------------------------------*/
/*CD SI le dernier mot cle lu dans CAI_FILE_DA n'existe pas ou  n'est pas egal au mot cle recherche */ 
/*---------------------------------------------------------------------------*/
if (fd->ad_dern_mot_cle == -1 || strncmp(mot_cle,fd->dernier_mot_cle,CAI_TAILLE_MAX_CLEDA) != 0 )
	{ 
/*CD SI la liste des mots cles est en memoire 			*/
	if ( fd->nb_mot_cle != -1 ) 
		{
/*---------------------------------------------------------------------------*/
/*CD ALORS on scrute cette liste jusqu'a trouver le mot cle */
/*---------------------------------------------------------------------------*/
		while (i< fd->nb_mot_cle && indic_mot_cle == CAI_NON_TROUVE )
			{
			if ( strncmp(mot_cle,(char *)(fd->liste_mot_cle +i),CAI_TAILLE_MAX_CLEDA) == 0 )
				{
				indic_mot_cle = CAI_TROUVE;
				num_mot_cle    = i;
/*CD	   on recupere alors le pointeur sur la zone fichier apres ce mot cle */
				ad_donnee=*(fd->ad_donnee + i);
				strcpy(mot_cle_lu,(char *)(fd->liste_mot_cle +i));
				ad_mot_cle=*(fd->ad_mot_cle + i);
				}
			i++;
			}
		}
	else
		{
/*CD SINON	*/
/*CD   on se positionne au debut du fichier ( adresse_deb = 0)*/
		adresse_deb = 0;	
/*---------------------------------------------------------------------------*/
/*CD   on cherche le premier mot cle a partir de cette adresse (operation interne basee sur un automate) */
/*---------------------------------------------------------------------------*/
		cr = cai_lirecleDA(fd,     /* [E] file descriptor du fichier a lire  */
        	      	   adresse_deb,   /* [E] adresse de debut de recherche */
	        	   mot_cle_lu,  /* [S] mot cle suivant lu ( termine par \0)*/
		           &ad_donnee, /* [S] adresse du premier caractere suivant le mot cle */
		           &ad_mot_cle /* [S] adresse du premier caractere du mot cle */
 		   	  );
/*CD    si erreur on sort */
		if (cr != CAI_OK && cr != CAI_EOF) {
			goto ERREUR;
			}	
/*CD   si aucun mot cle trouve on sort en erreur "aucun mot cle dans le fichier" */  
		if ( cr == CAI_EOF)
			{
			sprintf(CAI_ERREUR,"Erreur fichier sans mot cle( %s)",fd->nom_fichier);
			goto ERREUR;
			}
/*CD   TANT QUE  mot_cle non trouve &  pas erreur */
		while (cr == CAI_OK && indic_mot_cle == CAI_NON_TROUVE )
			{
/*CD   	  si le mot cle est egal au mot cle demande alors mot_cle trouve */
			if ( strcmp(mot_cle,mot_cle_lu) == 0)
				{
				indic_mot_cle = CAI_TROUVE;
				}
			else
				{
				adresse_deb = ad_donnee;
/*---------------------------------------------------------------------------*/
/*CD      si mot_cle non trouve on cherche le mot cle suivant (operation interne basee sur un automate) */
/*CD      cette operation necessite : un pointeur sur la zone fichier ou l'on recherche le mot cle suivant */
/*CD      cette operation ramene : le mot cle suivant et le pointeur sur la zone fichier apres ce mot cle */
/*---------------------------------------------------------------------------*/
				cr = cai_lirecleDA(fd,     /* [E] file descriptor du fichier a lire  */
        	      	   		   adresse_deb,   /* [E] adresse de debut de recherche */
	        	   		   mot_cle_lu,  /* [S] mot cle suivant lu (termine par \0)*/
		           		   &ad_donnee, /* [S] adresse du premier caractere suivant le mot cle */
		           		   &ad_mot_cle/* [S] adresse du premier caractere du mot cle */
 		   	  		   );
				}
/*CD    FIN TANT QUE pas erreur & mot_cle non trouve */
			}
/*CD	FIN SI LISTE MOT CLE presente en memoire*/	
		}
/*CD FIN SI le dernier mot cle lu dans CAI_FILE_DA n'existe pas ou  n'est pas egal au mot cle recherche */ 
	if (cr != CAI_OK && cr != CAI_EOF) 
		{
		goto ERREUR;
		}
/*CD 	SI fin de fichier on sort en erreur "mot cle non present */
	if ( indic_mot_cle == CAI_NON_TROUVE ) 
		{
		sprintf(CAI_ERREUR,"Mot cle :%s non trouve dans le fichier :%s",mot_cle,fd->nom_fichier);
		goto ERREUR;
		}
/*CD on sauvegarde alors dans la structure fd les info sur le dernier mot cle */
	strcpy(fd->dernier_mot_cle,mot_cle_lu);
	fd->ad_dern_mot_cle = ad_mot_cle;
	fd->ad_dern_donnee  = ad_donnee;
	}

/*CD SINON	*/
else
	{
	indic_mot_cle = CAI_TROUVE;
	ad_donnee = fd->ad_dern_donnee;	
	ad_mot_cle= fd ->ad_dern_mot_cle;
	strcpy(mot_cle_lu,fd->dernier_mot_cle);
	}


/*---------------------------------------------------------------------------------------*/
/*CD a partir du mot cle trouve on analyse syntaxiquement la ligne jusqu'au prochain mot cle */
/*CD (operation interne effectuant le decodage et donnant : le type de donnee (entier ou reel ou CAI_ASCII),*/
/*CD le nombres de lignes */
/*CD le nombre de colonnes , la taille maxi d'un element , un pointeur sur le tableau lu */
/*CD ( ce tableau a ete alloue dynamiquement par cette operation) 	       		 */
/*---------------------------------------------------------------------------------------*/
cr=cai_liredonneeDA(
			fd,         /* [E] file descriptor du fichier a lire  */
        	      	ad_donnee,  /* [E] adresse de debut de recherche */
	        	&don_lue,    /* [S] donnee lue */
			&type_lu,    /* [S] type de la donnee lue (CAI_ENTIER4,CAI_REEL8,CAI_ASCII,CAI_BLOC)*/
			&nb_lig_lu,  /* [S] nombre de lignes du tableau lu  */
			&nb_col_lu,  /* [S] nombre de colonnes du tableau lu  */  
		        &taille_cell_lu/* [S] taille maxi en octet d'un element du tableau lu ( significatif
					 pourla donnee CAI_ASCII )*/   
		      );
/*CD si erreur de decodage on sort en erreur */
if (cr != CAI_OK ) {
	if (cr == CAI_EOF){
		sprintf(CAI_ERREUR,"erreur fichier de DA incomplet (mot cle %s)",mot_cle);
		}
	goto ERREUR;
	}


/*---------------------------------------------------------------------------------------*/
/*CD on effectue les controles de coherence : */
/*CD type_lu = type  ( entier reel ou CAI_ASCII)*/
/*CD nblig_lu = nblig */
/*CD nbcol_lu = nb_col */
/*CD taille_cell_lu <= taille_cell */
/*CD SI difference on sort en erreur "incoherence entre " */  
/*CD NB : une lecture en reel4 de reel ecrit en reel8 n'est pas une erreur */
/*---------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------*/
/* donnee de type entiere */
/*---------------------------------------------------------------------------------------*/
if ( type_lu == CAI_ENTIER4) 
	{
 	if (*type != CAI_ENTIER4)
		{
		cr = CAI_KO;
		sprintf(CAI_ERREUR,"type lu (%d) incoherent avec le type specifie ( %d) ",type_lu,*type);
		goto ERREUR;
		}
	if (nb_lig_lu != *nb_lig || nb_col_lu != *nb_col)
		{
		cr = CAI_KO;
		sprintf(CAI_ERREUR,"dim lue incoherente avec la dimension specifiee");
		goto ERREUR;
		} 
/*CD le pointeur sur le tableau alloue par l'appelant est caste au type donne par l'appelant  */
/*CD le pointeur sur le tableau alloue par l'operation de decodage est caste au type_lu */
	don_ent   = (long int *)don_lue;
	donnee_li = (long int *)donnee;
	i=0;
/*CD TANT QUE lig <= nblig_lu et non erreur */ 
	while ( i < nb_lig_lu && cr ==CAI_OK)
		{
		j=0;
/*CD 	TANT QUE col <= nbcol_lu et non erreur */ 
		while ( j < nb_col_lu && cr ==CAI_OK)
			{
			entier = *(don_ent + i*nb_col_lu + j);
			*(donnee_li +i*nb_col_lu + j) = entier;
			j++;			
			}
/*CD 	FIN TANT QUE col <= nbcol_lu et non erreur */ 
		i++;
		}
/*CD 	FIN TANT QUE lig <= nblig_lu et non erreur */ 
	if (cr != CAI_OK )
		{
		goto ERREUR;
		}
	}

/*---------------------------------------------------------------------------------------*/
/* donnee de type reelle */
/*---------------------------------------------------------------------------------------*/
if ( type_lu == CAI_REEL8) 
	{
	if (*type != CAI_REEL4 && *type != CAI_REEL8)
		{
		cr=CAI_KO;
		sprintf(CAI_ERREUR,"type lu (%d) incoherent avec le type specifie ( %d)",type_lu,*type);
		goto ERREUR;
		}
	if (nb_lig_lu != *nb_lig || nb_col_lu != *nb_col)
		{
		cr=CAI_KO;
		sprintf(CAI_ERREUR,"dim lue incoherente avec la dimension specifiee ");
		goto ERREUR;
		} 
/*CD le pointeur sur le tableau alloue par l'appelant est caste au type donne par l'appelant  */
/*CD le pointeur sur le tableau alloue par l'operation de decodage est caste au type_lu */
	don_ree = (double *)don_lue;
	donnee_f = (float *)donnee;
	donnee_d = (double *)donnee;
	i=0;
/*CD TANT QUE lig <= nblig_lu et non erreur */ 
	while ( i < nb_lig_lu)
		{
		j=0;
/*CD 	TANT QUE col <= nbcol_lu et non erreur */ 
		while ( j < nb_col_lu)
			{
			reel = *(don_ree + i*nb_col_lu + j);
			if (*type == CAI_REEL4)
				{
				*(donnee_f + i*nb_col_lu + j) = reel;
				}
			else
				{
				*(donnee_d + i*nb_col_lu + j) = reel;
				}
			j++;			
			}
/*CD 	FIN TANT QUE col <= nbcol_lu et non erreur */
		i++; 
		}
/*CD 	FIN TANT QUE lig <= nblig_lu et non erreur */ 
	}


/*---------------------------------------------------------------------------------------*/
/* donnee de type CAI_ASCII */
/*---------------------------------------------------------------------------------------*/
if (type_lu == CAI_ASCII )
	{
	if (*type != CAI_ASCII )
		{
		cr=CAI_KO;
		sprintf(CAI_ERREUR,"type lu (%d) incoherent avec le type specifie ( %d)",type_lu,*type);
		goto ERREUR;
		}
	if (nb_lig_lu != *nb_lig || nb_col_lu != *nb_col)
		{
		cr=CAI_KO;
		sprintf(CAI_ERREUR,"dim lue incoherente avec la dimension specifiee ");
		goto ERREUR;
		} 
	if (taille_cell_lu > *taille_cell)
		{
		cr=CAI_KO;
		sprintf(CAI_ERREUR,"longueur max des donnees CAI_ASCII lues (%d) incoherente avec la taille specifie (%d)"
			,taille_cell_lu,*taille_cell);
		goto ERREUR;
		}
/*CD le pointeur sur le tableau alloue par l'appelant est caste au type donne par l'appelant  */
/*CD le pointeur sur le tableau alloue par l'operation de decodage est caste au type_lu */
	don_asc = (char *)don_lue;
	donnee_a = (char *)donnee;
	i=0;
	j=0;
/*CD TANT QUE lig <= nblig_lu et non erreur */ 
	while ( i < nb_lig_lu)
		{
		j=0;
/*CD 	TANT QUE col <= nbcol_lu et non erreur */ 
		while ( j < nb_col_lu)
			{
			strcpy(donnee_a + (i*nb_col_lu + j)* (*taille_cell),don_asc + (i*nb_col_lu + j)*CAI_TAILLE_MAX_ASCII);
			j++;
			}
/*CD 	FIN TANT QUE col <= nbcol_lu et non erreur */
		i++;
		}
/*CD 	FIN TANT QUE lig <= nblig_lu et non erreur */ 
	}
/*CD liberation place memoire alloue par l'operation interne precedente  */
if (don_lue != NULL)
	{
	free(don_lue);
	don_lue=NULL;
	}
/*---------------------------------------------------------------------------------------*/
/* donnee de type CAI_BLOC */
/*---------------------------------------------------------------------------------------*/
if (type_lu == CAI_BLOC)
	{
	if (*type != CAI_BLOC )
		{
		sprintf(CAI_ERREUR,"type lu (%d) incoherent avec le type specifie ( %d)",type_lu,*type);
		goto ERREUR;
		}
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
/*CD liberation place memoire alloue par l'operation interne precedente  */
if (don_lue != NULL)
	{
	free(don_lue);
	don_lue=NULL;
	}
return(cr);	  
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}

/********************************************************************************/
/*                                                                           	*/
/* OPERATION: cai_listeDA                                                    	*/
/*                                                                           	*/
/* ROLE:  Cette operation recherche la liste des mots cles presents dans un 	*/
/*	  fichier de Donnees Auxiliaires                                        */
/*                                                                           	*/
/* ATTENTION : cette fonction s'alloue de l'espace memoire qu'il conviendra de 	*/
/* liberer a la fin du traitement                                               */
/*                                                                           	*/
/* CONTEXTE D'APPEL:                                                         	*/
/*                                                                          	*/
/* VERSION:                                                                  	*/
/* CODE RETOUR : CAI_OK ou CAI_KO							*/
/*	" taille d'un mot cle lu superieure a la limite "			*/
/*	" erreur a la lecture du fichier de donnees auxiliaires "		*/
/********************************************************************************/
CAI_OK_KO cai_listeDA(CAI_FILE_DA *fd)
	 
{
long	adr_deb;	/* adresse dans le fichier de DA */
long	ad_suiv;	/* adresse du premier caractere suivant un mot cle */
long	ad_deb_mcc;	/* adresse du premier caractere des donnees du mot cle */
long	*ad;		/* adresse pour copie */
int	nb_mot_cle;	/* nombre de mots cle trouves dans le fichier */
size_t	taille;		/* taille du buffer a allouer */
CAI_OK_KO	retour;		/* code retour appel fonctions */
CAI_MOT_CLE mot_cle;	/* mot cle trouve*/
char 	*adr_copie;	/* adresse pour copie de mots cle */
int	nb_bloc;	/* nombre de realloc realises +1 */
off_t   offset;	  	
off_t   offreel;


/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*CD initialisation a zero du nombre de mots cle trouves 		     	*/
/*------------------------------------------------------------------------------*/
nb_mot_cle = 0;
nb_bloc = 1;

/*------------------------------------------------------------------------------*/
/*CD on verifie au prealable que le fichier est effectivement ouvert	 	*/
/*CD SINON on sort en erreur 							*/
/*------------------------------------------------------------------------------*/
if ( fd == NULL ) 
	{
	retour = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Dans listeDA, fichier de donnees auxiliaires non ouvert");
	goto ERREUR;
	}

/*------------------------------------------------------------------------------*/
/*CD Positionnement en debut de fichier						*/
/*------------------------------------------------------------------------------*/
/*CD on se positionne sur le debut du fichier ( rewind ) 		     */
offset=0;
offreel=lseek(fd->descr,offset,SEEK_SET);


/*------------------------------------------------------------------------------*/
/*CD Allocation d'un espace memoire contenant la liste des mots cles trouves 	*/
/*CD Allocation d'espaces memoire contenant les adresses des mots cles		*/
/*CD et les adresses des donnees associees a chaque mot cle 			*/
/*CD (il faut essayer d'allouer un maximum de place representative afin 	*/
/*CD d'eviter un trop grand nombre de realloc)					*/
/*------------------------------------------------------------------------------*/
taille = CAI_TAILLE_MAX_CLEDA * CAI_NB_MAX_CLE;
fd->liste_mot_cle = (CAI_MOT_CLE *)malloc (taille);
if (fd->liste_mot_cle == NULL) 
	{
	retour = CAI_ERR_ALLOC;
	sprintf (CAI_ERREUR," erreur allocation memoire pour liste mots cle ");
	goto ERREUR;
	}

taille = CAI_NB_MAX_CLE * sizeof(int);
fd->ad_mot_cle = (long *)malloc (taille);
if (fd->ad_mot_cle == NULL) 
	{
	retour = CAI_ERR_ALLOC;
	sprintf (CAI_ERREUR," erreur allocation memoire pour adresse mots cle ");
	goto ERREUR;
	}

taille = CAI_NB_MAX_CLE * sizeof(int);
fd->ad_donnee = (long *)malloc (taille);
if (fd->ad_donnee == NULL) 
	{
	retour = CAI_ERR_ALLOC;
	sprintf (CAI_ERREUR," erreur allocation memoire pour adresse donnees d'un mots cle ");
	goto ERREUR;
	}

/*------------------------------------------------------------------------------*/
/*CD Adr_deb = adresse debut de fichier						*/
/*------------------------------------------------------------------------------*/
adr_deb = 0;

/*------------------------------------------------------------------------------*/
/*CD Recherche du premier mot cle du fichier(cai_lirecleDA)			*/
/*CD Si erreur de recherche, sortie en erreur					*/
/*------------------------------------------------------------------------------*/
retour = cai_lirecleDA (fd, 		/* structure du fichier a lire */
			adr_deb, 	/* adresse du debut de recherche dans le fichier */
			mot_cle,	/* mot cle trouve */
			&ad_suiv, 	/* adresse dans le fichier du premier caractere suivant le mot cle */
			&ad_deb_mcc) ;	/* adresse dans le fichier du premier caractere du mot cle */

if (retour != CAI_OK && retour != CAI_EOF) 
	{
	retour = CAI_ERREUR_RECHERCHE_MOT;
	sprintf (CAI_ERREUR," erreur recherche du premier mot cle ");
	goto ERREUR;
	}

/*------------------------------------------------------------------------------*/
/*CD TANT QUE il existe des mots cles dans le fichier				*/
/*------------------------------------------------------------------------------*/
while (retour != CAI_EOF)
	{
/*------------------------------------------------------------------------------*/
/*CD 	si la liste des mots cle est saturee alors reallocation du double de 	*/
/*CD	   place pour les mots cle et les adresse de ces mots			*/
/*------------------------------------------------------------------------------*/
	if (nb_mot_cle == (CAI_NB_MAX_CLE * nb_bloc))
		{
		nb_bloc = nb_bloc + 1;

		taille = CAI_TAILLE_MAX_CLEDA * CAI_NB_MAX_CLE * nb_bloc;
		fd->liste_mot_cle = (CAI_MOT_CLE *)realloc (fd->liste_mot_cle, taille);
		if (fd->liste_mot_cle == NULL) 
			{
			retour = CAI_ERR_ALLOC;
			sprintf (CAI_ERREUR," erreur re-allocation memoire pour liste mots cle - bloc %d ", nb_bloc);
			goto ERREUR;
			}

		taille = CAI_NB_MAX_CLE * sizeof(int) * nb_bloc;
		fd->ad_mot_cle = (long *)realloc (fd->ad_mot_cle, taille);
		if (fd->ad_mot_cle == NULL) 
			{
			retour = CAI_ERR_ALLOC;
			sprintf (CAI_ERREUR," erreur re-allocation memoire pour adresse mots cle - bloc %d ", nb_bloc);
			goto ERREUR;
			}

		taille = CAI_NB_MAX_CLE * sizeof(int) * nb_bloc;
		fd->ad_donnee = (long *)realloc (fd->ad_donnee, taille);
		if (fd->ad_donnee == NULL) 
			{
			retour = CAI_ERR_ALLOC;
			sprintf (CAI_ERREUR," erreur re-allocation memoire pour donnees d un mots cle - bloc %d", nb_bloc);
			goto ERREUR;
			}
		}
		
/*------------------------------------------------------------------------------*/
/*CD	sauvegarde du mot cle trouve dans liste_mot_cle				*/
/*------------------------------------------------------------------------------*/
	adr_copie =(char *)(fd->liste_mot_cle + nb_mot_cle);
	strncpy (adr_copie, mot_cle, CAI_TAILLE_MAX_CLEDA);

/*------------------------------------------------------------------------------*/
/*CD	sauvegarde de l'adresse dans le fichier du mot cle trouve tab_adresse	*/
/*CD 	et de l'adresse du premier caractere des donnees associees au mot cle	*/
/*------------------------------------------------------------------------------*/
	ad = (long *)(fd->ad_mot_cle + nb_mot_cle);
	*ad = ad_deb_mcc;
	ad = (long *)(fd->ad_donnee + nb_mot_cle);
	*ad = ad_suiv;

/*------------------------------------------------------------------------------*/
/*CD	incrementation du nombre de mots cle trouves				*/
/*------------------------------------------------------------------------------*/
	nb_mot_cle = nb_mot_cle + 1;

/*------------------------------------------------------------------------------*/
/*CD	recherche du mot cle suivant dans le fichier (cai_lirecleDA)		*/
/*------------------------------------------------------------------------------*/
	adr_deb = ad_suiv;
	retour = cai_lirecleDA (fd, 		/* structure du fichier a lire */
			     	adr_deb, 	/* adresse du debut de recherche dans le fichier */
			        mot_cle,	/* mot cle trouve */
			        &ad_suiv, 	/* adresse dans le fichier du premier caractere suivant le mot cle */
			        &ad_deb_mcc);	/* adresse dans le fichier du premier caractere du mot cle */
	if (retour != CAI_OK && retour != CAI_EOF) 
		{
		retour = CAI_ERREUR_RECHERCHE_MOT;
		sprintf (CAI_ERREUR," erreur recherche d un mot cle ");
		goto ERREUR;
		}

	}	/*CD FIN TANT QUE (plus de mots cle)		*/	

/*------------------------------------------------------------------------------*/
/* CD Sauvegarde du nombre de mots cle trouves					*/
/*------------------------------------------------------------------------------*/
fd->nb_mot_cle = nb_mot_cle;


/*---------------------------------------------------------------------------*/
/* Fin du traitement nominal			        		     */
/*---------------------------------------------------------------------------*/
retour =  CAI_OK;
return (retour);

/*---------------------------------------------------------------------------*/
/* traitement degrade : 			        		     */
/*---------------------------------------------------------------------------*/
ERREUR :

if (fd != NULL)
	{
if (fd->liste_mot_cle != NULL)
	{
	free(fd->liste_mot_cle);
        fd->liste_mot_cle=NULL;

	}
if (fd->ad_mot_cle != NULL)
	{
	free(fd->ad_mot_cle);
	fd->ad_mot_cle=NULL;

	}
if (fd->ad_donnee != NULL)
	{
	free(fd->ad_donnee);
	fd->ad_donnee=NULL;
        }


	fd->nb_mot_cle = -1;
	}
return (retour);
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/********************************************************************************/
/*                                                                           	*/
/* OPERATION:  cai_ajoute DA                                                  	*/
/*                                                                           	*/
/* ROLE:  Cette operation ajoute a la fin du fichier de DA une donnee  		*/
/*        auxiliaire								*/
/*                                                                           	*/
/*                                                                           	*/
/*                                                                           	*/
/* CONTEXTE D'APPEL:                                                         	*/
/*	Cette operation doit etre appelee apres ouverture prealable du fichier	*/
/*	de Donnees Auxiliaires							*/
/*                                                                           	*/
/* VERSION:                                                                  	*/
/*										*/
/* CODE RETOUR : CAI_OK ou CAI_KO						*/
/*Si CAI_KO, l'erreur peut etre affichee via la variable globale CAI_ERREUR	*/
/*	" fichier de donnees auxiliaires %s non ouvert "			*/
/*	" erreur de positionnement en fin de fichier "				*/
/*	" erreur d'ecriture dans le fichier "					*/
/********************************************************************************/
CAI_OK_KO cai_ajouteDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE type,int nb_lig,int nb_col, 	
  	        	            long taille_cell,void *donnee)
{
CAI_OK_KO	retour;	/* code retour des fonctions */
int	i;		/* indice de boucle */
int	indic_ecrit;    /* indic ecritur DA ds fichier(=1 si ecriture = 0 sinon)*/
char	* da_ascii;	/* buffer de la DA en ascii (sous forme 	*/
			/* de x fois CAI_TAILLE_MAX_LIGDA caracteres)	*/
int	indic_mot_cle; 	/* indicateur de recherche du mot cle */



/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
  da_ascii=NULL;
/*------------------------------------------------------------------------------*/
/*CD on verifie au prealable que le fichier est effectivement ouvert	 	*/
/*CD SINON on sort en erreur 							*/
/*------------------------------------------------------------------------------*/

if ( fd == NULL ) 
	{
	retour = CAI_ERREUR_OUV;
	sprintf(CAI_ERREUR,"Dans ajouteDA, fichier de donnees auxiliaires non ouvert");
	goto ERREUR;
	}
/*------------------------------------------------------------------------------*/
/*CD on verifie que le type fait partie des types reconnus 			*/
/*------------------------------------------------------------------------------*/

if ( type != CAI_ENTIER4 && type != CAI_REEL4 && type != CAI_REEL8 && type  != CAI_ASCII & type !=CAI_BLOC)
	{
	retour = CAI_ERR_TYP;
	sprintf(CAI_ERREUR,"type :%d non reconnu ( 1:CAI_ENTIER4 2:CAI_REEL4 3:CAI_REEL8 4:CAI_ASCII 5:CAI_BLOC )",type);
	goto ERREUR;
	} 
/*------------------------------------------------------------------------------*/
/*CD on verifie que la DA n'existe pas deja (cai_listeDA)			*/
/*------------------------------------------------------------------------------*/
if (fd->nb_mot_cle <= 0)
	{
	retour = cai_listeDA (fd);
	if (retour != CAI_OK)  goto ERREUR;
	}
indic_mot_cle = CAI_NON_TROUVE;
i = 0;
while (i< fd->nb_mot_cle && indic_mot_cle == CAI_NON_TROUVE )
	{
	if ( strncmp(mot_cle,(char *)(fd->liste_mot_cle +i), CAI_TAILLE_MAX_CLEDA) == 0 )  
		indic_mot_cle = CAI_TROUVE;
	i = i + 1;
	}

if (indic_mot_cle == CAI_TROUVE)
	{
	retour = CAI_MOT_DEJA_EXISTE;
	sprintf(CAI_ERREUR,"Dans ajouteDA, mot cle %s deja existant ",mot_cle);
	goto ERREUR;
	}

/*------------------------------------------------------------------------------*/
/*CD On ecrit la DA dans un buffer intermediaire et dans le fichier  		*/
/*------------------------------------------------------------------------------*/

indic_ecrit = 1;

retour = cai_ecritDA_buf( 
		   	fd,		/* [E] file descriptor du fichier de DA 	*/
		   	mot_cle,	/* [E] mot cle associe aux donnees auxiliaires 	*/
		   	type,		/* [E] type des donnees a ecrire		*/
		   	nb_lig,		/* [E] nombre de lignes du tableau a ecrire	*/
		   	nb_col,		/* [E] nombre de colonnes du tableau a ecrire	*/
		   	taille_cell,	/* [E] taille maxi en octets d'un element	*/
					/*     (significatif si type = ascii)		*/
		   	donnee,		/* [E] pointeur sur la zone memoire a ecrire	*/
		   	indic_ecrit,	/* [E] indicateur si ecriture du buffer dans 	*/
					/*     le fichier (=1) sinon (=0)		*/
		   	&da_ascii);	/* [S] buffer de la DA en ascii (sous forme 	*/
					/* de x fois CAI_TAILLE_MAX_LIGDA caracteres)	*/

if (retour != CAI_OK) goto ERREUR;


/*---------------------------------------------------------------------------*/
/* Fin du traitement nominal			        		     */
/*---------------------------------------------------------------------------*/
if (da_ascii != NULL)
{
 free(da_ascii);
 }
retour = CAI_OK;
return (retour);

/*---------------------------------------------------------------------------*/
/* traitement degrade : 			        		     */
/*---------------------------------------------------------------------------*/
ERREUR :
if (da_ascii != NULL)
{
 free(da_ascii);
}
return (retour);

/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}

/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS INTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_lirecleDA()                                                */
/*                                                                           */
/* ROLE: cette operation interne recherche le mot cle suivant detecte        */
/* apres une "adresse dans le fichier ".				     */
/* cette operation retourne le mot cle suivant s'il existe ainsi qu'une adresse*/
/* sur le premier caractere suivant ce mot cle                               */
/*  NB : la notion d'adresse ici est relative au fichier :c.a.d              */
/* 	1er octet du fichier = adresse 0                                     */
/* 	2eme octet du fichier = adresse 1 ...                                */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 					     */
/*   " mot cle non present dans le fichier de donnees auxiliaires %s"        */
/*   " taille du mot cle lue %s superieure a la limite %d ou egale a 0"      */ 
/*   " erreur a la lecture du fichier de donnees auxiliaires %s"             */
/*****************************************************************************/
static CAI_OK_KO cai_lirecleDA(CAI_FILE_DA *fd,long  ad_deb,char * mot_cle, 
			       long *ad_suiv,long * ad_deb_mcc)
{
CAI_OK_KO 	cr;	/* code retour */
long	num;	/* numero du buffer courant */
long 	ad_deb_buff;  /* adresse de debut du buffer */
long     ad_deb_rel;   /* adresse de debut de recherche relative au buffer courant */
long     ad_suivante;  /*  adresse du premier caractere suivant le mot cle courant */
long     taille_buff_courant;  /* taille du buffer lu */
char   *buff_courant; /* buffer courant */ 
CAI_MOT_CLE mot_cle_courant;/* mot cle courant */
off_t   offset;	  	
off_t   offreel;
long     nb_buffer;	/* nombre de buffer dans le fichier */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD dans CAI_FILE_DA on prevoit un indicateur qui vaut -1 si aucun buffer       		*/
/*CD n'a pas deja ete lu et qui vaut sinon le numero du buffer lu dans le fichier 	*/
/*CD on prevoit aussi la taille du buffer lu ainsi que l'allocation statique du buffer 	*/  
/*CD et le decalage entre 2 buffer consecutifs ( fixe a  taille_buff/2)		        */ 
/*CD initialisations 								 	*/ 
cr=CAI_OK;
 
/*---------------------------------------------------------------------------*/
/*CD on recherche alors le nombre de buffer dans le fichier */
/*---------------------------------------------------------------------------*/
/*CD on se positionne sur la fin du fichier ( rewind ) 		     */
offset=0;
offreel=lseek(fd->descr,offset,SEEK_END);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
offreel=lseek(fd->descr,offset,SEEK_CUR);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
if ( offreel < ad_deb)
	{
	cr=CAI_ERR_ADRESSE;
	sprintf(CAI_ERREUR,"erreur adresse de recherche en dehors du fichier (%d)",ad_deb);
	goto ERREUR;
	}

nb_buffer = (offreel / (fd->taille_buffer/2) ) + 1; 

/*---------------------------------------------------------------------------*/
/*CD on commence par calculer le premier buffer a lire par : num = 2 * ad_deb/(fd->taille buffer) */
/*---------------------------------------------------------------------------*/
num= (ad_deb/(fd->taille_buffer/2));
ad_deb_buff = num * (fd->taille_buffer /2);

/*CD SI indicateur != num 								*/
if ( num != fd->num_buffer)
	{
/*---------------------------------------------------------------------------*/
/*CD 	lecture du buffer num a partir de l'adresse num*fd->taille_buffer/2 et de taille*/
/*CD     fd->taille_buffer*2 (operation interne )					*/
/*---------------------------------------------------------------------------*/
	cr=cai_lireficDA(
		    fd, /* [E] structure decrivant le fichier de donnees auxiliaires */
	            ad_deb_buff,      /* [E] adresse du premier octet a lire */ 
        	    fd->buffer    /* [E/S] buffer a remplir */
		   );
/*CD    SI erreur on sort 								*/
	if (cr != CAI_OK)
		{
		goto ERREUR;
		}
/* initialisation du numero de buffer */
	fd->num_buffer=num;
	buff_courant = fd->buffer;
	taille_buff_courant = fd->taille_buffer_lu;	
	}
else
	{
/*CD SINON tab = fd ->ad_buffer 							*/
	buff_courant = fd->buffer;
	taille_buff_courant = fd->taille_buffer_lu;	
	}

ad_deb_rel = ad_deb - ad_deb_buff;
/*---------------------------------------------------------------------------*/
/*CD TANT QUE mot_cle non trouve et non EOF						*/
/*---------------------------------------------------------------------------*/
cr=CAI_KO;
while ( cr !=CAI_OK && taille_buff_courant > 0 && cr != CAI_EOF && cr != CAI_PAS_MC)
	{
/*---------------------------------------------------------------------------*/
/*CD 	recherche du mot cle dans le buffer buff on retourne le mot  cle,sa longueur et */
/*CD    l'adresse dans le tableau du caractere suivant le mot cle (operation interne )  */
/*CD NB: attention au cas ou le mot cle peut etre coupe                                 */ 
/*---------------------------------------------------------------------------*/
	cr=cai_recherchecleDA( 
			        taille_buff_courant,/* [E] taille du buffer a scanner en octets  */
        	      	        buff_courant,     /* [E] buffer a scanner */
		                ad_deb_rel,	  /* [E] adresse a partir de laquelle il faut scanner le buffer */
	        	        mot_cle_courant,  /* [S] mot cle suivant lu */
		                &ad_suivante	  /* [S] adresse du premier caractere suivant le mot cle */
				);
/*CD si erreur on sort */
	if (cr != CAI_OK && cr != CAI_EOF && cr != CAI_PAS_MC)
		{
		goto ERREUR;
		}

/*CD  Si mot_cle non trouve et non EOF  on incremente le numero de buffer 		*/
	if ( (cr == CAI_EOF||cr == CAI_PAS_MC) && num+1< nb_buffer)
		{
/* calcul de l'adresse de debut de recherche */
		ad_deb_rel=0;
/*---------------------------------------------------------------------------*/
/*CD 		lecture du buffer num a partir de l'adresse num*fd->taille_buffer/2 et de taille */
/*CD     	fd->taille_buffer*2 (operation interne )				*/
/*---------------------------------------------------------------------------*/
		num=num+1;
		ad_deb_buff = num * (fd->taille_buffer /2);
		cr=cai_lireficDA(
		  	  fd, /* [E] structure decrivant le fichier de donnees auxiliaires */
	          	  ad_deb_buff,      /* [E] adresse du premier octet a lire */ 
        	  	  fd->buffer    /* [E/S] buffer a remplir */
		  	 );
/*CD    SI erreur on sort 								*/
		if (cr != CAI_OK)
			{
			goto ERREUR;
			}
/* initialisation du numero de buffer */
		fd->num_buffer=num;
/*CD       		on met a jour les variables locales  				*/
		buff_courant = fd->buffer;
		taille_buff_courant = fd->taille_buffer_lu;
		cr=CAI_KO;
		} 		
	}
/*---------------------------------------------------------------------------*/
/*CD FIN TANT QUE mot_cle non trouve 							*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*CD SI mot_cle trouve 									*/
/*---------------------------------------------------------------------------*/
if (cr == CAI_OK)
	{
/*CD	le mot cle est recopie dans l'argument de sortie				*/
	strcpy(mot_cle,mot_cle_courant);
/*CD	 l'adresse dans le tableau du caractere suivant le mot cle est convertie en     */
/*CD 	 adresse absolue dans le fichier ad_suiv = num*fd->taille_buff/2 + (ad_suiv-ad_deb)*/
	*ad_suiv = (num * fd->taille_buffer/2) + ad_suivante;
/*CD	 l'adresse dans le tableau du premier caractere du  mot cle est convertie en    */
/*CD 	 adresse absolue dans le fichier ad_deb_mc = ad_suiv -long 			*/
	*ad_deb_mcc=*ad_suiv -strlen(mot_cle) - 2;
	}
/*---------------------------------------------------------------------------*/
/*CD SI EOF plus de mot cle  			*/
/*---------------------------------------------------------------------------*/
if (cr == CAI_PAS_MC) 
	{
	goto ERREUR;
	}
if (taille_buff_courant ==0 || cr == CAI_EOF)
	{
	cr=CAI_EOF;
	goto ERREUR;
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
return(cr);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_liredonneeDA                                               */
/*                                                                           */
/* ROLE: Cette operation interne analyse syntaxiquement les donnees stockees */
/* apres un mot cle jusqu'au prochain mot_cle du fichier ou EOF.	     */
/* Elle permet donc de determiner le type , la taille  de la donnee .        */
/*       ( ENTIER REEL OU CAI_ASCII)            ( 2 dim maxi )		     */
/* Elle s'alloue de plus la place necessaire au stockage de cette donnees.   */ 
/* pour les donnees CAI_ASCII , le stockage se fait de maniere contigue en   */
/* memoire dans un tableau de cellules de taille fixe (taille_max).          */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 					     */
/*   " erreur a la lecture du fichier de donnees auxiliaires %s"             */
/*   " parenthese ouvrante non detectee (mot cle %s )                        */ 
/*   " parenthese fermante  non detectee (mot cle %s )                       */
/*   " nombre de niveaux de parentheses superieur a 2 ( mot cle %s )         */ 	
/*   " % dimension du tableau non constante (mot cle %s)                     */
/*   " % dimension du tableau = 0     (mot cle %s)                           */
/*   " type de la donnee lue %s non reconnue (mot cle %s)                    */
/*   " erreur lors du decodage de la donnee lue %s" (mot cle %s)             */
/*****************************************************************************/
static CAI_OK_KO cai_liredonneeDA(CAI_FILE_DA *fd,long ad_deb, void **don_lue, 
				  CAI_TYPE *type_lu,int *nb_lig_lu, 
  			          int *nb_col_lu, long *taille_cell_lu )
{
CAI_OK_KO 	cr;		/* code retour */
long	num;		/* numero du buffer courant */
long 	ad_deb_buff;    /* adresse de debut du buffer */
long     ad_deb_rel;     /* adresse de debut de recherche relative au buffer courant */
long     taille_buff_courant;  /* taille du buffer lu */
char   *buff_courant;   /* buffer courant */ 
off_t   offset;	  	
off_t   offreel;
long     nb_buffer;	/* nombre de buffer dans le fichier */
int     nb_lig;         /* nombre de lignes */
int     nb_elt;		/* nombre d'elements */
long  	taille_maxi;	/* taille maxi d'une donnee ascii */
CAI_ETAT1   etat1;		/* etat de l'automate de decodage */
CAI_TYPE    type;		/* type de la donnee */
int	nb_elt_alloues;	/* nombre d'elements alloues */
long	ad_fin_decodage; /* adresse de fin du decodage effectif du buffer */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/* dans CAI_FILE_DA on prevoit un indicateur qui vaut zero si aucun buffer       */
/* n'a pas deja ete lu et qui vaut sinon le numero du buffer lu dans le fichier 	*/
/* on prevoit aussi la taille du buffer lu ainsi que l'allocation statique du buffer 	*/  
/* et le decalage entre 2 buffer consecutifs ( fixe a  taille_buff/2)		*/ 
/*CD initialisations 								 	*/ 
cr=CAI_OK;
nb_lig = 1;
nb_elt = 0;
taille_maxi = 0;
etat1  = CAI_DEBUTDEC; 
type=CAI_INCONNU; 
nb_elt_alloues=0;

/*CD on recherche alors le nombre de buffer dans le fichier */
/*CD on se positionne sur la fin du fichier ( rewind ) 		     */
offset=0;
offreel=lseek(fd->descr,offset,SEEK_END);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
offreel=lseek(fd->descr,offset,SEEK_CUR);
/*CD SI erreur on sort "erreur lors d'un lseek du fichier de DA " 	     */
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le  fichier %s",fd->nom_fichier);
	goto ERREUR;

	}
if ( offreel <= ad_deb)
	{
	cr=CAI_ERR_ADRESSE;
	sprintf(CAI_ERREUR,"erreur adresse de recherche en dehors du fichier (%d)",ad_deb);
	goto ERREUR;
	}

nb_buffer = (offreel / (fd->taille_buffer/2) ) + 1; 

/*CD on commence par calculer le premier buffer a lire par : num = 2 * ad_deb/(fd->taille buffer) */
num= (ad_deb/(fd->taille_buffer/2));
ad_deb_buff = num * (fd->taille_buffer /2);

/*CD SI indicateur != num 								*/
if ( num != fd->num_buffer)
	{
/*CD 	lecture du buffer num a partir de l'adresse num*fd->taille_buffer/2 et de taille*/
/*CD     fd->taille_buffer*2 (operation interne )					*/
	cr=cai_lireficDA(
		    fd, /* [E] structure decrivant le fichier de donnees auxiliaires */
	            ad_deb_buff,      /* [E] adresse du premier octet a lire */ 
        	    fd->buffer    /* [E/S] buffer a remplir */
		   );
/*CD    SI erreur on sort 								*/
	if (cr != CAI_OK)
		{
		goto ERREUR;
		}
/* initialisation du numero de buffer */
	fd->num_buffer=num;
	buff_courant = fd->buffer;
	taille_buff_courant = fd->taille_buffer_lu;	
	}
else
	{
/*CD SINON tab = fd ->ad_buffer 							*/
	buff_courant = fd->buffer;
	taille_buff_courant = fd->taille_buffer_lu;	
	}

/*CD	calcul de l'adresse relative du debut de la recherche dans le buffer            */
/*CD ad_deb_buff = ad_deb - num* fd->taille buffer/2             			*/
ad_deb_rel = ad_deb - ad_deb_buff;


/*CD TANT QUE statut <> lu et non EOF         					*/
while ( etat1 != CAI_FINDEC && cr != CAI_EOF && taille_buff_courant > 0)
	{
/*CD 	lecture de la donnee  dans le buffer buff on retourne le tableau des donnees qui*/
/*CD    ont ete lues (tableau eventuellement incomplet) , la taille du tableau lu       */
/*CD	, le type , la dimension1 , le nb elt , la taille maxi , si incomplet le        */
/*CD    statut de l'automate est retourne (operation interne)			        */ 
/*CD    NB: attention au cas ou la donnee peut etre coupe                               */ 
	cr=cai_decodedonDA(
			   taille_buff_courant,   /* [E] taille du buffer a scanner en octets  */
        	      	   buff_courant,          /* [E] buffer a scanner */
		           ad_deb_rel,	          /* [E] adresse a partir de laquelle il faut scanner le buffer */
			   &type,      	          /* [E/S] type de la donnee lue (=CAI_INCONNU lors du premier appel) */
						  /* CAI_ENTIER4 CAI_REEL8 CAI_ASCII EN SORTIE */
			   &nb_lig,    	/* [E/S] nombre de lignes du tableau lu   ( = 0 lors du premier appel) */
			   &nb_elt,     /* [E/S] nombre d'elements du tableau lu  ( = 0 lors du premier appel) */
			   &nb_elt_alloues,     /* [E/S] nombre d'elements du tableau lu ( = 0 lors du premier appel)*/
		           &taille_maxi,/*[E/S] taill max en octet d'un elt du tab lu (=0 lors du premier appel*/
					       /* significatif exclusivement pour des donnees CAI_ASCII */	
	        	   don_lue,      /* [E/S] donnee lue */
			   &etat1,        /* [E/S] etat de l'auto ( = DEBUT_DECODAGE lors du premier appel )*/
		           &ad_fin_decodage     /* [E/S] adresse de fin du decodage effectif du  buffer */
		     );
	
	
/*CD si erreur on sort */
	if (cr != CAI_OK && cr != CAI_EOF)
		{
		goto ERREUR;
		}

/*CD  Si mot_cle non trouve et non EOF  on incremente le numero de buffer 		*/
	if ( cr == CAI_EOF && num+1< nb_buffer)
		{
/*---------------------------------------------------------------------------*/
/*CD 		lecture du buffer num a partir de l'adresse num*fd->taille_buffer/2 et de taille */
/*CD     	fd->taille_buffer*2 (operation interne )				*/
/*---------------------------------------------------------------------------*/
		num=num+1;
		ad_deb_buff = num * (fd->taille_buffer /2);
		cr=cai_lireficDA(
		  	  fd, /* [E] structure decrivant le fichier de donnees auxiliaires */
	          	  ad_deb_buff,      /* [E] adresse du premier octet a lire */ 
        	  	  fd->buffer    /* [E/S] buffer a remplir */
		  	 );
/*CD    SI erreur on sort 								*/
		if (cr != CAI_OK)
			{
			goto ERREUR;
			}
/* initialisation du numero de buffer */
		fd->num_buffer=num;
/*CD       		on met a jour les variables locales  				*/
		buff_courant = fd->buffer;
		taille_buff_courant = fd->taille_buffer_lu;
/*CD		calcul de l'adresse relative du debut de la recherche dans le buffer    */
/*CD 		ad_deb_buff = ad_deb_buff(donnee par l'operation precedente) -          */
/*CD				fd->taille buffer/2  ( decalage entre 2 buffers)	*/
		ad_deb_rel = ad_fin_decodage - (fd->taille_buffer/2);			
		} 		
	}
/*CD FIN TANT QUE statut <> lu et non EOF 						*/

/*CD	si statut <> lu et EOF on sort en erreur "erreur fichier de DA incomplet (mot cle %s) */
if (  etat1 != CAI_FINDEC && etat1 != CAI_DEBUTDEC && (cr == CAI_EOF || taille_buff_courant == 0) )
	{
	cr == CAI_EOF;
	goto ERREUR;
	}
else
	{
	if (etat1 == CAI_DEBUTDEC && (cr == CAI_EOF || taille_buff_courant == 0) )
		{
		type=CAI_BLOC;
		}
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
*nb_lig_lu=nb_lig;
*nb_col_lu= nb_elt/nb_lig;
*type_lu = type;
*taille_cell_lu=taille_maxi;
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
*nb_lig_lu=nb_lig;
*nb_col_lu= nb_elt/nb_lig;
*type_lu = type;
*taille_cell_lu=taille_maxi;
return(cr);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_deplaceficDA                                               */
/*                                                                           */
/* ROLE: Cette operation interne cree un trou ou compacte un fichier de DA.  */
/*	precedenment ouvert en lecture ecriture . 			     */
/*	ex1 : avant [+++++++++++++++++++++++++++]			     */
/*	                         |--> deplacement			     */
/*				 V					     */ 		
/*                             ad_deb                                        */
/*	      apres [+++++++++++++   ++++++++++++++]			     */
/*	                         |--> deplacement			     */
/*				 V					     */ 		
/*                             ad_deb                                        */
/*	ex2 : avant [+++++++++++++++++++++++++++]			     */
/*	           deplacement<--|					     */
/*				 V					     */ 		
/*                             ad_deb                                        */
/*	      apres [++++++++++++++++++++++++]				     */
/*	                      |				     	   	     */
/*			      V						     */ 		
/*                             ad_deb                                        */
/* NB: on lit et on ecrit des buffers de taille fd->taille_buffer	     */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/*****************************************************************************/
static CAI_OK_KO cai_deplaceficDA(CAI_FILE_DA *fd ,long debut,long deplacement)

{
CAI_OK_KO fin_fichier_avant_transfert;	/* fin fichier origine */
long nb_octets_a_transferer;		/* nombre total d'octets a transferer */
long nb_octets_transferes;		/* nombre total d'octets transferes */
long nb_octets_a_lire;			/* longueur courante du buffer a lire */
long nb_octets_lus;			/* longueur du buffer lu */
long adresse_debut_buffer;		/* adresse debut courante a lire */
CAI_OK_KO retour; 
off_t cr; /* code retour des fonctions appelees */
long   cr2;
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
retour =lseek(fd->descr,0L,SEEK_END);
if ( retour == -1) 
	{
	 sprintf(CAI_ERREUR,"erreur lors d'un lseek sur le fichier de DA");
	 retour=CAI_PB_LSEEK ;
	 goto ERREUR; /* pb lseek */
	}
fin_fichier_avant_transfert=retour;
if ( debut < 0) {
	 sprintf(CAI_ERREUR,"erreur debut de la zone a deplacer negative (%d)",debut);
	 retour=CAI_DEB_NEG;
	 goto ERREUR; /* debut negatif */
	}
if ( debut > fin_fichier_avant_transfert) {
	 sprintf(CAI_ERREUR,"erreur debut de la zone a deplacer en dehors du fichier ",debut);
	 retour=CAI_FIN_DEP;
	 goto ERREUR; /* pb debut en dehors du fichier */
	}
if ( debut + deplacement < 0) {
	 sprintf(CAI_ERREUR,"pb de tassement: deplacement de la zone impossible ");
	 retour=CAI_ECRASEMENT;
	 goto ERREUR; /* pb tassement du fichier en tete */
	}
nb_octets_transferes=0;
nb_octets_a_transferer=fin_fichier_avant_transfert - debut;

if ( deplacement >= 0)
	{
	adresse_debut_buffer=maxi(debut,fin_fichier_avant_transfert - fd->taille_buffer);
	}
else
	{
	adresse_debut_buffer=debut;
	}

/*CD TANT QUE tous les octets n'ont pas ete transferes */
while (nb_octets_transferes < nb_octets_a_transferer)
	{
	nb_octets_a_lire = mini(fd->taille_buffer, nb_octets_a_transferer - nb_octets_transferes);
	cr = lseek(fd->descr,adresse_debut_buffer,SEEK_SET);
	if(  cr != adresse_debut_buffer)
		{
	 	sprintf(CAI_ERREUR,"erreur lors d'un lseek sur le fichier de DA");
		retour=CAI_PB_LSEEK;
		goto ERREUR;
		} 
	nb_octets_lus = read(fd->descr,fd->buffer,nb_octets_a_lire);
	if (nb_octets_lus != nb_octets_a_lire)
		{
	 	sprintf(CAI_ERREUR,"erreur de lecture dans le fichier de DA");
		retour=CAI_PB_READ;
		goto ERREUR;
		}
	cr= lseek(fd->descr,adresse_debut_buffer+deplacement,SEEK_SET);

	if ( cr != (adresse_debut_buffer + deplacement))
		{
	 	sprintf(CAI_ERREUR,"erreur lors d'un lseek sur le fichier de DA");
		retour=CAI_PB_LSEEK;
		goto ERREUR;
		}
	cr2=write(fd->descr,fd->buffer,nb_octets_lus);
	if ( cr2 != nb_octets_lus )
		{
	 	sprintf(CAI_ERREUR,"erreur d'ecriture dans le fichier de DA");
		retour=CAI_PB_WRITE;
		goto ERREUR;		
		}
	nb_octets_transferes = nb_octets_transferes + nb_octets_lus;
	if ( deplacement >= 0)
		{
		adresse_debut_buffer=maxi( adresse_debut_buffer - fd->taille_buffer,debut);
		}
	else
		{
		adresse_debut_buffer=mini( adresse_debut_buffer + fd->taille_buffer,fin_fichier_avant_transfert);
		}
	}
/*CD FIN TANT QUE tous les octets n'ont pas ete transferes */
if (deplacement < 0 )
	{
	retour=ftruncate(fd->descr,(size_t)(fin_fichier_avant_transfert+deplacement));
	if (retour != 0)
		{
	 	sprintf(CAI_ERREUR,"erreur lors d'un ftruncate sur le fichier de DA");
		retour=CAI_PB_FTRUNC;
		goto ERREUR;
		}
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
return(retour);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_lireficDA                                                  */
/*                                                                           */
/* ROLE: Cette operation interne lit dans le fichier de DA un buffer a partir*/
/*    d'une adresse specifiee par l'utilisateur d'un nombre d'octets specifie*/
/*      par l'utilisateur .                      		             */
/*         ( utilisation de  lseek , read ) 	     	        	     */
/*         ( buffer probable de 4096 octets)                                 */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 						     */
/*   " erreur lors du rewind sur le fichier de DA "			     */
/*   " erreur lors du fseek sur le fichier de DA ( ad_deb =%d) "	     */ 	
/*   " erreur a la lecture du fichier de donnees auxiliaires %s"             */
/*****************************************************************************/
static CAI_OK_KO cai_lireficDA(CAI_FILE_DA *fd,long  ad_deb,char *buff)

{
off_t offset;  /* offset dans le fichier de donnees auxiliaires */
off_t offreel; /* offset reel obtenu apres lseek */
long   nb_lus;   /* nombres d'octets lus */
CAI_OK_KO cr;      /* code retour */	
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
cr=CAI_OK;
/*CD on se positionne sur le debut du fichier ( rewind ) 		     */
offset=0;
offreel=lseek(fd->descr,offset,SEEK_SET);
/*CD SI erreur on sort "erreur lors du rewind du fichier de DA " 	     */
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du rewind du fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
/*CD on se deplace de ad_deb octets dans le fichier (fseek) 		     */
offset=ad_deb;
offreel=lseek(fd->descr,offset,SEEK_SET);
/*CD SI erreur on sort " erreur lors du lseek du fichier de DA (ad_deb=%d)"  */  
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
/*CD SI erreur on sort "erreur lors du fseek du fichier de DA " 	     */
/*CD on lit le buffer ( read )			 	             */
nb_lus=read(fd->descr,buff,fd->taille_buffer);
if (nb_lus == -1)
	{
	sprintf(CAI_ERREUR,"erreur lors du read sur le fichier %s",fd->nom_fichier);
	cr=CAI_PB_READ;
	goto ERREUR;
	}
*(buff + nb_lus)= '\0';
/*CD SI taille_ecrite <> taille buff on sort " erreur a l'ecriture du fichier de donnee ..."       */
if ( nb_lus != fd->taille_buffer)
	{
	fd->taille_buffer_lu=nb_lus;
	}
else
	{
	fd->taille_buffer_lu=fd->taille_buffer;
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
return(cr);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_ecrireficDA                                                */
/*                                                                           */
/* ROLE: Cette operation interne ecrit dans un fichier de DA temporaire un buffer a partir*/
/* 	d'une adresse specifiee par l'utilisateur d'un nombre d'octets specifie*/
/*      par l'utilisateur .                      		             */
/*         ( utilisation de rewind , fseek , write ) 			     */
/*         ( buffer probable de 4096 octets)                                 */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 						     */
/*   " erreur lors du rewind sur le fichier de DA "			     */
/*   " erreur lors du fseek sur le fichier de DA ( ad_deb =%d) "	     */ 	
/*   " erreur a l'ecriture du fichier de donnees auxiliaires %s"             */
/*****************************************************************************/
static CAI_OK_KO cai_ecrireficDA(CAI_FILE_DA *fd,long ad_deb,
				  long taille, char **buff )

{
off_t offset;  /* offset dans le fichier de donnees auxiliaires */
off_t offreel; /* offset reel obtenu apres lseek */
long   nb_ecrits;   /* nombres d'octets ecrit */
CAI_OK_KO cr;      /* code retour */	
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
cr=CAI_OK;
/*CD on se positionne sur le debut  du fichier ( lseek ) 		     */
offset=0;
offreel=lseek(fd->descr,offset,SEEK_SET);
/*CD SI erreur on sort "erreur lors du rewind du fichier de DA " 	     */
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
/*CD on se deplace de ad_deb octets dans le fichier (fseek) 		     */
offset=ad_deb;
offreel=lseek(fd->descr,offset,SEEK_SET);
/*CD SI erreur on sort " erreur lors du lseek du fichier de DA (ad_deb=%d)"  */  
if ( offreel == -1)
	{
	cr=CAI_PB_LSEEK;
	sprintf(CAI_ERREUR,"erreur lors du lseek sur le fichier %s",fd->nom_fichier);
	goto ERREUR;
	}
/*CD on ecrit le buffer ( write )			 	             */
nb_ecrits=write(fd->descr,*buff,taille);
/*CD SI taille_ecrite <> taille buff on sort " erreur a l'ecriture du fichier de donnee ..."       */
if ( nb_ecrits != taille)
	{
	sprintf(CAI_ERREUR,"erreur lors du write sur le fichier %s",fd->nom_fichier);
	cr=CAI_PB_WRITE;
	goto ERREUR;
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(CAI_OK);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
return(cr);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_decodedonDA                                                */
/*                                                                           */
/* ROLE:  Cette operation interne  decode une donnee sous la forme           */
/* ENTIER: (( 1 2 24 ) ( 4 5 6 )) ou REEL: ((1. 2. 24.) (4. 5. 6.))          */
/* ASCII: (( "1" "2" "24" ) ( "4" "5" "6" ))                                 */
/* BLOC ( donnee vide )							     */
/* cette operation commence par rechercher la premiere donnee pour determiner*/
/* le type de donnee puis decode  chaque donnee , alloue la place necessaire */
/* et parcourt ainsi tout le buffer					     */		  
/*   NB: Si la donnee est coupee on rend a l'appelant le statut "en cours"   */
/* ainsi qu'un pointeur sur le caractere suivant la derniere donnee lus et   */
/* l'etat de l'automate .			                             */
/* la place est alloue par cette operation mais l'appelant se charge de la   */
/* liberer si besoin.							     */ 
/*                                                                           */
/*                                                                           */
/* ETAT = ( DEBUT DECODAGE <=> ATTENTE ((	0			     */	
/*          ATTENTE (				1			     */
/*	    ATTENTE DONNEE			2			     */	
/*	    ATTENTE DONNEE ou )			3			     */					
/*	    ATTENTE ) ou (   			4			     */	
/* 	    FIN decodage   			5			     */	
/*	    BLOC				6			     */
/*	    ERREUR parenthese	                11			     */	
/*	    ERREUR fin de donne prematuree      13			     */	
/* CARACTERE = (							     */
/*		)							     */	
/*		AUTRE							     */
/*		<       						     */
/*		BLANC ou TAB ou \N				             */
/*	etat: CARACTERE  (    )    AUTRE <     BLANC ou TAB ou \n   	     */
/*	0		 1   11   11     6        0			     */
/*	1		 2   11   11    13  	  1			     */
/*	2		11    4    3    13        2			     */
/*	3		11    4    3    13        3			     */
/*	4		 2    5   11    13 	  4 			     */
/*									     */
/* dans les etats 2 et 3 si l'evenement  = AUTRE on passe a un nouvel        */
/*   automate	( remarque :si 2 dimensions on passe de l'etat 4 a l'etat 2  */
/*									     */	
/* ETAT1 =  Debut decodage donnee			0		     */
/*	    attente point(reel) ou blanc (entier)	1		     */
/*          attente " 					2		     */	
/*          attente BLANC ou TAB ou \n			3		     */		   
/*	    ENTIER                	 		4	             */
/*	    chaine de caracteres			5		     */ 	
/*          REEL 					6		     */
/*	    erreur donnee				11		     */
/* CARACTERE1 = .							     */
/*              "							     */
/*		BLANC ou TAB ou \n					     */
/*		CHIFFRE							     */
/*		SIGNE							     */	
/*              AUTRE							     */	
/*									     */
/*	etat :caractere1 . 	" BLANC ou TAB ou \n ou ) CHIFFRE SIGNE AUTRE*/
/*	0	         3 	2          11              1      1     11   */	
/*      1		 3     11           4              1     11     11   */
/*	2                2      5           2 		   2      2      2   */	
/*	3		 3     11           6              3      3      3   */  
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 						     */
/*	"ERREUR donnee non presente ( mot cle %s)"			     */
/*	"erreur parenthese ( mot cle %s)"				     */
/*	"erreur fin de donnee prematuree ( mot cle %s)			     */
/*      "erreur type de donne non reconnu"  				     */
/*	"erreur de decodage de la donne (mot cle %s) "			     */	 
/*****************************************************************************/
static CAI_OK_KO cai_decodedonDA(long taille_buf, char buff[],long ad_deb,
			   CAI_TYPE *type_lu,int *nb_lig_lu,int *nb_elt_lu,
			   int *nb_elt_alloc,long *taille_cell_lu, 
  			   void  **don_lue, CAI_ETAT1 *etat1,long *ad_fin_dec)
{
long 	indice;     /* indice de parcourt du buffer */
CAI_TYPE type;      /* type de la donnee decodee */
int	nb_lig;     /* nombre de lignes de la donnee */
int 	nb_elt;     /* nombre d'elements de la donnee */
long	taille_max; /* taille max d'une cellule CAI_ASCII */
int	cr;	    /* code retour de la fonction */
CAI_OK_KO   code;       /* code retour */  
char    car;	    /* caractere courant traite */
CAI_ETAT1	etat1_prec; /* etat precedent de niveau 1 */
CAI_EVT1    evt1;       /* evenement de niveau 1 detectee */
CAI_ETAT2   etat2;      /* etat de niveau 2 ( determination du type ) */
CAI_EVT2    evt2;       /* evenement de niveau2			  */
long    ad_deb_do;   /* adresse de debut de la donnee courante */
long int *entier;   /* pointeur sur donnees entieres */
double   *reel;     /* pointeur sur donnees reelles */
char     *asc;      /* pointeur sur donnees ascii */ 
int    nb_elt_alloues;/* nombres d'elements alloues */ 
long	taille;	    /* taille d'une donnee a decoder */		
int	nb_col;	    /* nombre de colonnes */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT  DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/	
/* initialisations 							     */

cr=CAI_OK;
indice=ad_deb;
type= *type_lu;
nb_lig= *nb_lig_lu;
nb_elt= *nb_elt_lu;
taille_max = *taille_cell_lu;
nb_elt_alloues= *nb_elt_alloc;
taille=0;
if (type == CAI_ASCII)
	{
	asc=(char *) *don_lue;
	}
else
	{
	if (type == CAI_REEL8)
		{
		reel=(double *) *don_lue;
		}
	else
		{
		if (type == CAI_ENTIER4)
			{
			entier=(long int *) *don_lue;
			}
		else
			{
			asc = (char *)malloc(CAI_TAILLE_MAX_ASCII);
			if (asc == NULL )
				{
				sprintf(CAI_ERREUR,"Erreur d'allocation lors de la lecture de la donnees");
				cr=CAI_ERR_ALLOC;
				goto ERREUR;
				}
			reel=(double *)asc;
			entier=(long int *)asc;
			}
		}
	}

/*---------------------------------------------------------------------------*/
/* TANT QUE  non erreur et non fin decodage et indice < taille buffer 		*/
/*---------------------------------------------------------------------------*/
while( *etat1 >= 0 && *etat1 != CAI_FINDEC && indice < taille_buf)
	{
/* 	car = buff[indice]						        */
/* 	etat1_prec=etat1							*/
	car = buff[indice];	
	etat1_prec=*etat1;
/*---------------------------------------------------------------------------*/
/* 	fonction interne testant ( ) < BLANC ou TAB ou n AUTRE 		*/
/*---------------------------------------------------------------------------*/
	code=cai_analyseDA(
			car,          /* [E] caractere a tester */
			&evt1         /* [S] evenement detecte */ 
		        );

/*	etat1 = automate1[etat1][evt1]						*/
	*etat1 = autom1[*etat1][evt1];
/*      si erreur on affiche le messsage associe et on sort			*/	
	if (*etat1 > 10)
		{
		switch (*etat1) {
			case CAI_ERR11:cr = CAI_ERR_FORME_FIC;
				sprintf(CAI_ERREUR,"erreur: forme donnee non coherente");  
				break;
			case CAI_ERR13:cr = CAI_ERR_FIN_DO;
				sprintf(CAI_ERREUR,"erreur: fin de donnee prematuree"); 
				break;
			default:cr = CAI_KO;
				sprintf(CAI_ERREUR,"erreur automate1 non reconnue");
				break; 
			}
		goto ERREUR;
		}
/*	si etat1= 2 et etat1_prec=4 			*/
/*			dim2 = dim2 + 1			*/
	if ( *etat1 == CAI_ETAT12 && etat1_prec == CAI_ETAT14)
		{
		nb_lig=nb_lig + 1;
		}
/* sauvegarde ad_deb_do */
	ad_deb_do=indice+1;
	if ( *etat1 == CAI_BLOCVIDE )
		{
		type=CAI_BLOC;
		nb_lig=1;
		nb_elt=0;
		*etat1 = CAI_FINDEC;
		}
/*	SI (etat1=3)					*/
	if ( *etat1 == CAI_ETAT13)
		{
/*		initialisation etat2					        */
		etat2= CAI_DEBUTYPE;
/*		sauvegarde ad_deb_do						*/ 
		ad_deb_do=indice;
/*---------------------------------------------------------------------------*/
/*		TANT QUE etat2 non REEL et non ENTIER et non ASCII et indice < taille buffer */
/*---------------------------------------------------------------------------*/
		while ( etat2 != CAI_REEL && etat2 != CAI_ENTIER && etat2 != CAI_ASC && indice <taille_buf )
			{
/*---------------------------------------------------------------------------*/
/*		  fonction interne testant . " CHIFFRE SIGNE BLANC AUTRE        */
/*---------------------------------------------------------------------------*/
			code=cai_typeDA(
			     car,       /* [E] caractere a tester */
			     &evt2         /* [S] evenement detecte */ 
		        	);
/*		  etat2 = automate2[etat2][evt2]			        */
			etat2=autom2[etat2][evt2];
/*      si erreur on affiche le messsage associe et on sort			*/	
			if (etat2 > 10)
				{
				switch (etat2) {
					case CAI_ERR21:cr = CAI_ERR_TYP;
					sprintf(CAI_ERREUR,"erreur: type de donnee non coherente");  
					break;
				default:cr = CAI_KO;
					sprintf(CAI_ERREUR,"erreur automate2 non reconnue");
					break; 
					}
				goto ERREUR;
				}
/*		  l'indice est incremente					*/
			indice ++;
			car = buff[indice];	
			}
/*---------------------------------------------------------------------------*/
/*		FIN TANT QUE non erreur et non REEL et non ENTIER et non ASCII  */
/*---------------------------------------------------------------------------*/

/* SI on arrive en fin de buffer sans avoir decode la donnee */
		if ( indice == taille_buf && (etat2 != CAI_REEL || etat2 != CAI_ENTIER || etat2 != CAI_ASC))
		    {
			cr= CAI_EOF;
		    }
		else
		   {
  		   if (etat2 == CAI_ASC || type == CAI_ASCII )
			{
			if (etat2 != CAI_ASC || (type != CAI_ASCII && type != CAI_INCONNU))
				{
				sprintf(CAI_ERREUR,"Erreur: melange de type dans une meme donnee");
				cr=CAI_ERR_ASCII;
				goto ERREUR;
				}
			type=CAI_ASCII;
/*---------------------------------------------------------------------------*/
/*		si CAI_ASCII  allocation et decodage entre ad_deb+1 et ad_fin -1	*/
/*	     ( verification que la taille est inferieure a la taille max CAI_ASCII) */
/*---------------------------------------------------------------------------*/
			taille= (indice) - (ad_deb_do) -2 + 1;
			if (taille  >= CAI_TAILLE_MAX_ASCII)
				{
				sprintf(CAI_ERREUR,"donnee ascii de taille sup a CAI_TAILLE_MAX_ASCII (%d)",taille);
				cr=CAI_ERR_ASCII;
				goto ERREUR;
				} 
/* 			allocation de CAI_NB_ALLOC element reel entier et ASCII */
/*			par un realloc d'un ascii ( taille max) et sscanf(%s)	*/
			if (nb_elt_alloues <= nb_elt)
				{
				asc = (char *)realloc(asc,CAI_TAILLE_MAX_ASCII*(CAI_NB_ALLOC + nb_elt_alloues));
				if (asc != NULL )
					{
					nb_elt_alloues = CAI_NB_ALLOC + nb_elt_alloues;
					}
				else
					{
					sprintf(CAI_ERREUR,"Erreur d'allocation lors de la lecture de la donnees");
					cr=CAI_ERR_ALLOC;
					goto ERREUR;
					}
				}

				strncpy(asc + (nb_elt*CAI_TAILLE_MAX_ASCII),buff+ad_deb_do+1,taille-1);
				*(asc + (nb_elt)*CAI_TAILLE_MAX_ASCII +taille -1 )='\0';
/*			   taille_max = sup (taille_max,taille)     */
			taille_max=maxi(taille,taille_max);

/*		nbelt = nbelt + 1						*/
			nb_elt=nb_elt +1;
			}
		   else
			{
			 indice--;
/*---------------------------------------------------------------------------*/
/*		si REEL allocation et decodage entre ad_deb et ad_fin-1   	*/
/*			par un realloc d'un reel et sscanf(%lf)			*/
/*---------------------------------------------------------------------------*/
			if (etat2 == CAI_REEL || type == CAI_REEL8)
				{
				if (etat2 != CAI_REEL || (type != CAI_REEL8 && type != CAI_INCONNU))
					{
					sprintf(CAI_ERREUR,"Erreur: melange de types dans une meme donnee");
					cr=CAI_ERR_REEL;
					goto ERREUR;
					}
				type = CAI_REEL8;				
/* 			allocation de CAI_NB_ALLOC element reel  */
				if (nb_elt_alloues <= nb_elt)
					{
					reel=(double *)realloc(reel,sizeof(double)*(CAI_NB_ALLOC + nb_elt_alloues));
					if (reel != NULL )
						{
						nb_elt_alloues = CAI_NB_ALLOC + nb_elt_alloues;
						}
					else
						{
						sprintf(CAI_ERREUR,"Erreur d'allocation lors de la lecture de la donnees");
						cr=CAI_ERR_ALLOC;
						goto ERREUR;
						}
					}
				code=sscanf(buff+ad_deb_do," %lf",reel+nb_elt);
				if (code != 1)
					{
					sprintf(CAI_ERREUR,"Erreur de decodage de la donnee reelle (element %d)",nb_elt);
					cr=CAI_ERR_REEL;
					goto ERREUR;
					}
				}
			else							
				{
				if (etat2 != CAI_ENTIER || (type != CAI_ENTIER4 && type != CAI_INCONNU))
					{
					sprintf(CAI_ERREUR,"Erreur: melange de type dans une meme donnee");
					cr=CAI_ERR_ENTIER;
					goto ERREUR;
					}
				type = CAI_ENTIER4;
/*---------------------------------------------------------------------------*/
/*		si ENTIER allocation et decodage entre ad_deb et ad_fin -1	*/
/*			par un realloc d'un entier4 et sscanf(%d)		*/
/* 			allocation de CAI_NB_ALLOC element entier  */
/*---------------------------------------------------------------------------*/
				if (nb_elt_alloues <= nb_elt)
					{
					entier=(long int *)realloc(entier,sizeof(long int)*(CAI_NB_ALLOC+nb_elt_alloues));
					if (entier != NULL )
						{
						nb_elt_alloues = CAI_NB_ALLOC + nb_elt_alloues;
						}
					else
						{
						sprintf(CAI_ERREUR,"Erreur d'allocation lors de la lecture de la donnees");
						cr=CAI_ERR_ALLOC;
						goto ERREUR;
						}
					}
				code =sscanf(buff+ad_deb_do," %ld",
						entier+nb_elt);
				if (code != 1)
					{
					sprintf(CAI_ERREUR,"Erreur de decodage de la donnee entier(element %d)",nb_elt);
					cr=CAI_ERR_ENTIER;
					goto ERREUR;
					}
				}
/*		nbelt = nbelt + 1						*/			
			nb_elt=nb_elt +1;
			}
/* FIN SI on arrive en fin de buffer sans avoir decode la donnee */
/* on se positionne sur le dernier caractere de la donnee 	*/
		   indice--;
		   }
		*etat1 = CAI_ETAT12;
/* FIN SI (etat1=3)					*/
		}
/*---------------------------------------------------------------------------*/
/*	FIN SI									*/
/*---------------------------------------------------------------------------*/
/*      l'indice est incremente						  	*/
	indice++;
	}
/*---------------------------------------------------------------------------*/
/* FIN TANT QUE  non erreur et non fin decodage et indice < taille buffer 	*/
/*---------------------------------------------------------------------------*/
if (indice == taille_buf)
	{
	cr=CAI_EOF;
	}
if (*etat1 == CAI_FINDEC)
	{ 
/* nb_col = nbelt  nb_lig						  	*/
	nb_col = (nb_elt/nb_lig);
/* SI nb_lig . nb_col <> nbelt alors erreur dans le nombre d'elements d'une ligne*/
	if ( (nb_lig * nb_col) != nb_elt)
		{
		sprintf(CAI_ERREUR,"Nombre d'elements decodes incoherent (nb_elt:%d nb_lig:%d)",nb_elt,nb_lig);
		cr=CAI_ERR_DIM;
		goto ERREUR;
		}
	}
/*---------------------------------------------------------------------------*/
/* nb_elt_lu = nb_elt							     */
/* nb_lig_lu = dim2 							     */
/* etat1 = etat1							     */
/* type_lu  = type							     */
/* taille_cell_lu= CAI_REEL8 ou CAI_ENTIER4 ou CAI_ASCII( taille max ) 	             */
/* ad_fin_dec= ad_fin  ( pour lecture du prochain buffer si besoin) 	     */  
/*---------------------------------------------------------------------------*/
*type_lu=type;
*nb_lig_lu=nb_lig;
*nb_elt_lu=nb_elt;
*taille_cell_lu=taille_max;
*ad_fin_dec=ad_deb_do;
*nb_elt_alloc=nb_elt_alloues;
if (type == CAI_ASCII)
	{
	*don_lue=(void *)asc;
	}
else
	{
	if (type == CAI_REEL8)
		{
		*don_lue=(void *)reel;
		}
	else
		{
		*don_lue=(void *)entier;
		}
	}
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas nominal  				     */
/*---------------------------------------------------------------------------*/
return(cr);
/*---------------------------------------------------------------------------*/
/*CD fin de l'operation en cas d'erreur 				     */
/*---------------------------------------------------------------------------*/
ERREUR:
/* affectation pour pouvoir liberer la place allouee ( par l'appelant )*/
if (type == CAI_ASCII)
	{
	*don_lue=(void *)asc;
	}
else
	{
	if (type == CAI_REEL8)
		{
		*don_lue=(void *)reel;
		}
	else
		{
		*don_lue=(void *)entier;
		}
	}
return(cr);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_analyseDA                                                  */
/*                                                                           */
/* ROLE: Cette operation interne est un automate qui teste les caracteres    */
/*       ( , ) , AUTRE , <  , BLANC ou TAB ou  \N		             */
/* 									     */
/*      								     */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 						     */
/*****************************************************************************/
static CAI_OK_KO cai_analyseDA(char car, CAI_EVT1 *evt1)

{
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD si  char = '('  evt1 = 0 					*/
if (car == '(' )
	{
	*evt1 = CAI_PAR_OUV;
	}
else
	{
/*CD sinon si  char = ')' evt1 = 1 				*/
	if (car == ')' )
		{
		*evt1 = CAI_PAR_FERM;
		}
	else
		{
/*CD 	   sinon si char = '<' evt1 = 3 			*/
		if (car == '<' )
			{
			*evt1=CAI_INF;
			}
		else
			{
/*CD 		 sinon si char = BLANC ou TAB ou n evt1= 4 	*/
			if (car==' ' || car=='	' || car=='\n')
				{
				*evt1=CAI_BLANC;
				}
			else
				{		
/*CD		       sinon evt1 = 2 				*/
				*evt1=CAI_DONNEE;
				}
/*CD			fin si					*/
			}
/*CD		 fin si						*/
		}
/*CD	   fin si						*/
	}
/*CD fin si							*/	
        return(CAI_OK);	
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_typeDA                                                     */
/*                                                                           */
/* ROLE: Cette operation interne est un automate qui teste les caracteres    */
/*       . , " , BLANC ou TAB ou \n ou ), CHIFFRE	, SIGNE	, AUTRE	     */	
/* 									     */
/*      								     */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 					     */
/*****************************************************************************/
static CAI_OK_KO cai_typeDA(char car, CAI_EVT2 *evt2)

{
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD si  char = '.'  evt1 = 0 					*/
if (car == '.' )
	{
	*evt2=CAI_POINT;
	}
else
	{
/*CD sinon si  char = '"' evt1 = 1 				*/
	if (car=='"' )
		{
		*evt2=CAI_GUILL;
		}
	else
		{
/*CD 	   sinon si char = BLANC ou TAB ou \n ou ) evt1 = 2 	*/
		if (car==' ' || car=='	' || car=='\n' || car==')' )
			{
			*evt2=CAI_ESPACE;
			}
else
			{
/*CD 		 sinon si char = CHIFFRE  (isdigit) evt1= 3       	*/
			if ( isdigit(car) != 0)
				{
				*evt2=CAI_CHIFFRE;
				}
			else
				{
/*CD		       sinon si char = SIGNE  evt1= 4	        */
				if ( car == '+' || car =='-')
					{
					*evt2=CAI_SIGNE;
					}
				else
					{
/*CD			     sinon evt1= 5			*/	
					*evt2=CAI_AUTRE;
					}
				}
/*CD		       fin si					*/
			}
/*CD		 fin si						*/
		}
/*CD	   fin si						*/
	}
/*CD fin si							*/

        return(CAI_OK);
		
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/********************************************************************************/
/*                                                                           	*/
/* OPERATION: cai_recherchecleDA                                               	*/
/*                                                                           	*/
/* ROLE: Cette operation interne recherche un mot cle dans le buffer buff.   	*/
/*  (de taille specifiee dans CAI_FILE_DA). on retourne le mot  cle, sa longueur  	*/
/*  et l'adresse dans le tableau du caractere suivant le mot cle .           	*/
/* NB: attention au cas ou le mot cle est coupe on renvoie un statut 		*/
/*  "non trouve" comme si rien n'avait pu etre detecte                          */
/*                                                                           	*/
/* ETAT = ( DEBUT DECODAGE <=> ATTENTE <	0				*/
/*          ATTENTE >				1				*/
/*          FIN DECODAGE			2	|			*/
/*          ERREUR Syntaxe (<...<) 		CAI_ERR_CROCHET	| status de	*/
/*          ERREUR Donnee non presente		CAI_EOF| retour			*/
/*          ERREUR fin de donnee prematuree	CAI_PAS_MC	|		*/
/*										*/
/* CARACTERE = <  >  AUTRE  FIN_FICHIER						*/
/*										*/
/*	Etat : CARACTERE     	<	>	AUTRE	FIN_FICHIER		*/
/*	0			 1	 0	0	-1			*/
/*	1			-25	 2	1	-24			*/
/*                                                                           	*/
/*                                                                           	*/
/* CONTEXTE D'APPEL:                                                         	*/
/*                                                                           	*/
/* VERSION:                                                                  	*/
/*										*/
/********************************************************************************/
static CAI_OK_KO cai_recherchecleDA(long taille_buf,char *buff,
				    long ad_deb,char *mot_cle,long *ad_suiv)

{
long	retour;			/* code retour */
char *	indice;		     	/* indice de parcourt du buffer */
int	nb_car;			/* nombre de caracteres du mot cle */
char	*ad;			/* adresse d'un caractere */
long	adr_deb_mot;		/* adresse debut de mot cle */
CAI_ETAT3	etat;			/* etat de niveau 3 */
CAI_EVT3	evt3;			/* evenement de niveau 3 */
char	c;			/* caractere courant traite */
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD Initialisation : On se positionne a l'adresse a partir de laquelle il 	*/
/*CD		      faut scanner le buffer 					*/
/*CD On recherche le premier caractere < (strchr)				*/
/*------------------------------------------------------------------------------*/

indice =(char *) strchr ((char *)(buff+ad_deb), '<');

/*CD Si le code retour de strchr est NULL (< non trouve), erreur rencontree,	*/
if (indice == NULL)
	{
	retour = CAI_EOF;
	sprintf(CAI_ERREUR,"erreur: pas de mots cle a partir de l adresse %d", ad_deb); 
	goto ERREUR;
	}
 
/*------------------------------------------------------------------------------*/
/*CD On sauvegarde l'adresse relative  de debut du mot cle			*/
/*------------------------------------------------------------------------------*/
adr_deb_mot = (long)(indice - buff) + 1;

/*------------------------------------------------------------------------------*/
/*CD On utilise un automate pour analyser les caracteres (en les comptant) ;	*/
/*CD	on sort de l'analyse avec :  2 fin decodage ok				*/
/*CD				     -1 erreur syntaxe ( <    < )		*/
/*CD				     -3 On n'a pas assez de donnees pour 	*/
/*CD					analyser integralement le mot cle       */
/*------------------------------------------------------------------------------*/
etat = 0;
nb_car = 0;

while (etat != 2 && etat != CAI_PAS_MC && etat != CAI_ERR_CROCHET)
	{
	ad = (char *)(indice + nb_car);
	c = *ad;
	nb_car = nb_car + 1;
	retour =cai_mot_cleDA(c,       		/* [E] caractere a tester */
			      &evt3);         	/* [S] evenement detecte */ 
	etat = autom3 [etat] [evt3];
	}
nb_car = nb_car -2;

/*CD si etat erreur : envoi message */
if (etat == CAI_PAS_MC)
	{
	retour = CAI_PAS_MC;
	sprintf (CAI_ERREUR,"le mot cle n'est pas inclu entierement dans le buffer");
	goto ERREUR;
	}
if (etat == CAI_ERR_CROCHET )
	{
	retour = CAI_ERR_CROCHET;
	sprintf (CAI_ERREUR,"2 crochets ouvrant < < ont ete detecte dans le mot cle");
	goto ERREUR;
	}

/*CD si mot cle trop long, erreur */
if (nb_car > CAI_TAILLE_MAX_CLEDA)
	{
	retour = CAI_CLE_TROP_LONG;
	sprintf (CAI_ERREUR,"le mot cle est trop long");
	goto ERREUR;
	}

/*CD si decodage ok et si le mot cle n'est pas trop long,			*/
if (etat ==2 )
	{
/*CD 	sauvegarde du mot cle, de sa longueur, et de l'adresse du premier	*/
/*CD	caractere suivant le mot cle						*/
	memcpy (mot_cle,(indice + 1), nb_car);
	*(mot_cle + nb_car) = '\0';
	*ad_suiv = adr_deb_mot + nb_car + 1;
	}

/*---------------------------------------------------------------------------*/
/* Fin du traitement nominal			        		     */
/*---------------------------------------------------------------------------*/
retour = CAI_OK;
return (retour);

/*---------------------------------------------------------------------------*/
/* traitement degrade : 			        		     */
/*---------------------------------------------------------------------------*/
ERREUR :
return (retour);

/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION: cai_decodeautom3DA                                             */
/*                                                                           */
/* ROLE: Cette operation interne est un automate qui teste les caracteres    */
/*       < > fin_buffer blanc (' ' '\n') autre				     */	
/* 									     */
/*      								     */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/* CODE RETOUR : CAI_OK ou CAI_KO 						     */
/*****************************************************************************/
static CAI_OK_KO cai_mot_cleDA(char car, CAI_EVT3 *evt3)
 
{
/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD si  char = ' ' ou \n  	evt3 = ESPACE					*/
/*CD si  char = '<' 		evt3 = INFERIEUR				*/
/*CD si char = '>' 		evt3 = SUPERIEUR				*/
/*CD si char = \0  		evt3 = FIN     					*/
/*CD sinon 			evt3 = AUTRES  		      			*/

switch (car)
	{
	case (' ') :
		*evt3 = CAI_ESPACE;
		break;
	case  ('\n') :
		*evt3 = CAI_ESPACE;
		break;

	case '<' :
		*evt3 = CAI_INFERIEUR;
		break;
	case '>' :
		*evt3 = CAI_SUPERIEUR;
		break;
	case '\0' :
		*evt3 = CAI_FIN;
		break;

	default:
		*evt3 = CAI_AUTRES;
		break;
	}
return (CAI_OK);
/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_ecritDA_buf                                               */
/*                                                                           */
/* ROLE:  Cette operation ecrit une DA dans un buffer			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*	Cette operation doit etre appelee apres ouverture prealable fichier  */
/*	de Donnees Auxiliaires						     */
/*                                                                           */
/* VERSION:                                                                  */
/*									     */
/* CODE RETOUR : CAI_OK ou CAI_KO					     */
/*  si CAI_KO, l'erreur peut etre affichee via la variable globale CAI_ERREUR*/
/*	" fichier de donnees auxiliaires %s non ouvert "		     */
/*	" erreur de positionnement en fin de fichier "			     */
/*	" erreur d'ecriture dans le fichier "				     */
/*****************************************************************************/
static CAI_OK_KO cai_ecritDA_buf(CAI_FILE_DA *fd,char *mot_cle,
				 CAI_TYPE type,int nb_lig,int nb_col,
			         long taille_cell, void *donnee,
				 int indic_ecrit,char **da_ascii)
{
CAI_OK_KO	retour;				/* code retour des fonctions */
char	don_ascii[CAI_TAILLE_MAX_LIGDA];/* buffer contenant une donnee en ascii */
char	* pt_don;			/* pointeur sur don_ascii utilise pour l'ascii */
char 	* pt_ascii;			/* pointe sur da_ascii a l'emplacement du premier caractere libre*/
int	lig;				/* indice de boucle sur les lignes */
int	col;				/* indice de boucle sur les colonnes */	
char	* pt_deb_lig;			/* pointe sur da_ascii a l'adresse de debut de derniere ligne 	*/
int	nb_bloc;			/* nombre de blocs de CAI_TAILLE_BUFFER alloues pour da_ascii */
long	*dbentier;			/* pointeur sur variable entiere double */
float	*reel;				/* pointeur sur variable reelle simple precision */
double	*dbreel;			/* pointeur sur variable double precision */
long	longueur;			/* longueur significative d'un buffer */
long	sauv_diff_deb;			/* difference d'adresse entre derniere ligne du buffer et debut du buffer */
long	sauv_diff_cour;			/* difference d'adresse entre premier element libre du buffer et debut du buffer */
char *    tempo;

/*---------------------------------------------------------------------------*/
/*		 	DEBUT DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
/*CD initialisation des types */
dbentier = (long *) donnee;
reel = (float *) donnee;
dbreel = (double *) donnee;
/*------------------------------------------------------------------------------*/
/*CD Allocation d'un espace memoire pour le buffer d'ecriture de la donnee 	*/
/*CD auxiliaire									*/
/*CD (il faut essayer d'allouer un maximum de place representative afin 	*/
/*CD d'eviter un trop grand nombre de realloc)					*/
/*------------------------------------------------------------------------------*/
*da_ascii = NULL;
*da_ascii = (char *)malloc (CAI_TAILLE_BUFFER);
if (*da_ascii== NULL) 
	{
	retour = CAI_ERR_ALLOC;
	sprintf (CAI_ERREUR," erreur allocation memoire pour DA ascii ");
	goto ERREUR;
	}
         nb_bloc = 1;

/*------------------------------------------------------------------------------*/
/*CD On ecrit dans le buffer, avec la syntaxe desiree le mot cle suivi d'un \n 	*/
/*------------------------------------------------------------------------------*/
sprintf (*da_ascii, "<%s>\n", mot_cle);
pt_ascii = *da_ascii + strlen (*da_ascii);
pt_deb_lig = pt_ascii;

/*------------------------------------------------------------------------------*/
/*CD Initialisation d'une ligne de caracteres ascii, si des donnees sont 	*/
/*CD a ecrire									*/
/*------------------------------------------------------------------------------*/

if ((nb_lig != 0 && nb_col != 0 ) || type == CAI_BLOC)
	{
	if (type == CAI_BLOC)
		{
		nb_lig=0;
		}
	else
		{
		*pt_deb_lig = '(';
		pt_ascii = pt_ascii + 1;
		}
	}
/*------------------------------------------------------------------------------*/
/*CD POUR chaque ligne des donnees						*/
/*------------------------------------------------------------------------------*/
for (lig=0; lig < nb_lig; lig++)
	{

/*------------------------------------------------------------------------------*/
/*CD	Initialisation ( pour chaque ligne					*/
/*------------------------------------------------------------------------------*/
	strcpy (pt_ascii ,"( ");
	pt_ascii = pt_ascii + 2;
	
/*------------------------------------------------------------------------------*/
/*CD	POUR chaque colonne de donnees						*/
/*------------------------------------------------------------------------------*/
	for (col=0; col < nb_col; col++)
		{

/*------------------------------------------------------------------------------*/
/*CD 		Suivant le type des donnees (entier  double, 		        */
/*CD		reel simple ou double, ascii), on ecrit dans le buffer 		*/
/*CD		DON_ASCII dans un format donne (%d, %f, %lf, %s) la donnee	*/
/*CD		suivie d'un blanc						*/
/*------------------------------------------------------------------------------*/
		
		switch (type)
			{
			case CAI_ENTIER4:
				sprintf(don_ascii, "%d ",
				    *(dbentier+(nb_col*lig + col)) );
				break;
			case CAI_REEL4:
				sprintf(don_ascii, "%#.6g ", *(reel+(nb_col*lig + col)));
				break;
			case CAI_REEL8:
				sprintf(don_ascii, "%#.14g ", *(dbreel+(nb_col*lig + col)));
				break;
			case CAI_ASCII:
				pt_don = don_ascii;
				sprintf(pt_don, "\"");
				pt_don = pt_don + 1;
				tempo=((char *)donnee+(nb_col*lig + col)*taille_cell);
				longueur = strlen (tempo);
				memcpy (pt_don, tempo, longueur);
				pt_don = pt_don + longueur;
				sprintf(pt_don,"\" \0");
				break;
			default:
				retour = CAI_ERREUR_ENCODE;
				sprintf (CAI_ERREUR," erreur ecriture en ascii de donnee ");
				goto ERREUR;
			}

/*--------------------------------------------------------------------------------------*/
/*CD		On determine la longueur significative de ce buffer DON_ASCII (sans \0)	*/
/*--------------------------------------------------------------------------------------*/
		longueur = strlen (don_ascii);

/*--------------------------------------------------------------------------------------*/
/*CD		SI cette longueur + taille significative de da_ascii + \n))\n\0 est 	*/
/*CD		superieure a la taille allouee pour da_ascii, on se realloue		*/
/*CD		de la place 								*/
/*--------------------------------------------------------------------------------------*/
		if ( (longueur+5 + (long) pt_ascii - (long)&(**da_ascii)) > (CAI_TAILLE_BUFFER*nb_bloc) )
			{
			nb_bloc = nb_bloc + 1;

			sauv_diff_deb = (long) pt_deb_lig - (long)&(**da_ascii);
			sauv_diff_cour = (long) pt_ascii - (long)&(**da_ascii);

			*da_ascii = (char *)realloc (*da_ascii, (CAI_TAILLE_BUFFER*nb_bloc));
			if (*da_ascii== NULL) 
				{
				retour = CAI_ERR_ALLOC;
				sprintf (CAI_ERREUR,
		" erreur re-allocation memoire pour DA ascii-bloc %d ",
			 nb_bloc);
				goto ERREUR;
				}

			pt_deb_lig = *da_ascii + sauv_diff_deb;
			pt_ascii =  *da_ascii + sauv_diff_cour;
			}

/*--------------------------------------------------------------------------------------------------------------*/
/*CD		SI avec cette longueur + taille de ligne est superieure a CAI_TAILLE_MAX_LIGDA caracteres	*/
/*CD			A l'adresse courante  on ecrit \n dans le buffer da_ascii	                        */
/*--------------------------------------------------------------------------------------------------------------*/
		if ( (longueur + (long)pt_ascii - (long)pt_deb_lig) >= CAI_TAILLE_MAX_LIGDA)
			{
			*pt_ascii = '\n';
			pt_ascii = pt_ascii + 1;
			pt_deb_lig = pt_ascii;

			}

/*------------------------------------------------------------------------------*/
/*CD		On recopie les donnees significatives de DON_ASCII dans le 	*/
/*CD		buffer da_ascii a l'adresse pointeur				*/
/*------------------------------------------------------------------------------*/
		strcpy (pt_ascii, don_ascii);
		pt_ascii = pt_ascii + longueur;
		}	/*CD 	FIN POUR (col)					*/

/*------------------------------------------------------------------------------*/
/*CD	Initialisation ) apres les donnees de la ligne				*/
/*CD	SI longueur de la ligne < CAI_TAILLE_MAX_LIGDA alors			*/
/*CD		on ecrit \n							*/
/*CD	SINON 									*/
/*CD		on ecrit \n)\n							*/
/*------------------------------------------------------------------------------*/
	if (lig != (nb_lig-1))
		{
		if ((pt_ascii - pt_deb_lig + 2) < (CAI_TAILLE_MAX_LIGDA) )
			{
			strcpy (pt_ascii ,")\n");
			pt_ascii = pt_ascii + 2;
			pt_deb_lig = pt_ascii;
			}
		else
			{
			strcpy (pt_ascii ,"\n)\n");
			pt_ascii = pt_ascii + 3;
			pt_deb_lig = pt_ascii;
			}
		}
	else		/*CD on traite le cas de la derniere ligne */
		{
		if ((pt_ascii - pt_deb_lig + 3) < (CAI_TAILLE_MAX_LIGDA) )
			{
			strcpy (pt_ascii ,"))\n");
			pt_ascii = pt_ascii + 3;
			pt_deb_lig = pt_ascii;
			}
		else 
			{
			if ((pt_ascii - pt_deb_lig + 2) < (CAI_TAILLE_MAX_LIGDA) )
				{
				strcpy (pt_ascii ,")\n)\n");
				pt_ascii = pt_ascii + 4;
				pt_deb_lig = pt_ascii;
				}
			else
				{
				strcpy (pt_ascii ,"\n))\n");
				pt_ascii = pt_ascii + 4;
				pt_deb_lig = pt_ascii;
				}
			}

		}



}	/*CD FIN POUR (lig)					*/

/*------------------------------------------------------------------------------*/
/*CD On ecrit \0 en fin de buffer						*/
/*------------------------------------------------------------------------------*/
*pt_ascii='\0';

/*------------------------------------------------------------------------------*/
/*CD Si l'utilisateur le desire on ecrit en fin de fichier le buffer 		*/
/*------------------------------------------------------------------------------*/
if (indic_ecrit == 1)
	{
/*CD 	On se positionne a la fin du fichier de DA.				*/
	retour = lseek(fd->descr, 0L, SEEK_END);
	if (retour == -1)
		{
		retour = CAI_PB_LSEEK;
		sprintf(CAI_ERREUR,"Dans ajouteDA, erreur positionnement en fin de fichier");
		goto ERREUR;
		}

/*CD 	On ecrit le buffer dans le fichier */
	longueur = (long) strlen(*da_ascii);
	write (fd->descr, *da_ascii, longueur);

	} 	/* fin ecriture fichier */

/*------------------------------------------------------------------------------*/
/*CD re-initialisations des pointeurs de la structure CAI_FILE_DA     		*/
/*CD ( buffers de lecture , liste de mots cles , longueurs ...)			*/ 
/*------------------------------------------------------------------------------*/
if (indic_ecrit == 1) {   
	fd->num_buffer=-1;
	fd->nb_mot_cle=-1;
	if (fd->liste_mot_cle != NULL)
	{
	   free(fd->liste_mot_cle);
          fd->liste_mot_cle=NULL;

	}
	if (fd->ad_mot_cle != NULL)
	{
	  free(fd->ad_mot_cle);
	  fd->ad_mot_cle=NULL;

	}
	if (fd->ad_donnee != NULL)
	{
	   free(fd->ad_donnee);
	   fd->ad_donnee=NULL;
        }

}			


/*---------------------------------------------------------------------------*/
/* Fin du traitement nominal			        		     */
/*---------------------------------------------------------------------------*/
retour = CAI_OK;
return (retour);

/*---------------------------------------------------------------------------*/
/* traitement degrade : 			        		     */
/*---------------------------------------------------------------------------*/
ERREUR :
        fd->num_buffer=-1;
        fd->nb_mot_cle=-1;
	if (fd->liste_mot_cle != NULL)
	{
	   free(fd->liste_mot_cle);
          fd->liste_mot_cle=NULL;

	}
	if (fd->ad_mot_cle != NULL)
	{
	  free(fd->ad_mot_cle);
	  fd->ad_mot_cle=NULL;

	}
	if (fd->ad_donnee != NULL)
	{
	   free(fd->ad_donnee);
	   fd->ad_donnee=NULL;
        }

return (retour);

/*---------------------------------------------------------------------------*/
/*		 	FIN DE L'OPERATION	        		     */
/*---------------------------------------------------------------------------*/
}
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_lecture_colormap                                          */
/*                                                                           */
/* ROLE:  lecture de la colormap dans les DA             		     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/*									     */
/* CODE RETOUR : CAI_OK ou CAI_KO					     */
/*  si CAI_KO, l'erreur peut etre affichee via la variable globale CAI_ERREUR*/
/*									     */
/*****************************************************************************/
 CAI_OK_KO cai_lecture_colormap(CAI_IMAGE  *fdima)

{
  CAI_MODE_OUVERTURE  mode;  /* mode de lecture du fichier de DA   	*/
  char  mot_cle[9];	     /*  mot cle associe aux donnees colormap   */
  CAI_TYPE   type;           /* type des donnees a lire                 */
  int	  nb_lig;            /* nombre de lignes du tableau a lire 	*/
  int	  nb_col;	     /* nombre de colonnes du tableau a lire	*/
  long	  taille_cell;	     /* taille maxi en octets d'un element	*/
			     /*     (significatif si type = ascii)	*/
  CAI_OK_KO icr;	     /* compte-rendu de la fonction             */
  CAI_FILE_DA  *fd;          /* Descripteur du fichier DA               */
  char fic_DA[1024];	     /* nom fichier de donnees auxillaires      */

  strcpy(fic_DA,fdima->NOM_IMAGE);

/*** Ouverture du fichier de donnees auxilliaires ***/
   
  mode = CAI_LECTURE;
  fd=cai_openDA(fic_DA,fdima->TYPE_IMAGE,fdima->NB_CANAUX,mode);	
  if (fd == NULL)
  {
    /*--- il n'y a pas de donnees Colormap ---*/
    fdima->COLORMAP=0;

  }
  else
  {
/*** Recherche des caracteristiques des donnees Colormap ***/
   
     strcpy(mot_cle,"COLORMAP");
     mot_cle[8]='\0';
     icr=cai_lirecaractDA(fd,mot_cle,&type,&nb_lig,&nb_col,&taille_cell);
     if (icr != CAI_OK)
       /*--- il n'y a pas de donnees Colormap ---*/
       fdima->COLORMAP=0;
     else
     {
/***      Lecture des donnees de Colormap ***/

          fdima->TABCOLOR = (int *) malloc(sizeof(int)*768);
	  icr = cai_lireDA(fd,mot_cle,&type,&nb_lig,&nb_col,
			   &taille_cell,fdima->TABCOLOR);
          if (icr == CAI_KO)
               goto ERREUR;
	  fdima->COLORMAP = nb_col*nb_lig;
      }
  
/*** Fermeture du fichier de Donnees Auxilliaires ***/
     icr = cai_closeDA(fd);
   }
   return (CAI_OK);

ERREUR:
   return(CAI_KO);
}
/*
	Fin de l'operation cai_lecture_colormap
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION:  cai_ecriture_colormap                                          */
/*                                                                           */
/* ROLE:  ecriture de la colormap dans les DA             		     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL:                                                         */
/*                                                                           */
/* VERSION:                                                                  */
/*									     */
/* CODE RETOUR : CAI_OK ou CAI_KO					     */
/*  si CAI_KO, l'erreur peut etre affichee via la variable globale CAI_ERREUR*/
/*									     */
/*****************************************************************************/
 CAI_OK_KO cai_ecriture_colormap(CAI_IMAGE  *fdima)


{
  CAI_MODE_OUVERTURE  mode;  /* mode de lecture du fichier de DA   	*/
  char  mot_cle[13];	     /*  mot cle associe aux donnees colormap   */
  CAI_TYPE   type;           /* type des donnees a ecrire               */
  int	  nb_lig;            /* nombre de lignes du tableau a ecrire	*/
  int	  nb_col;	     /* nombre de colonnes du tableau a ecrire  */
  CAI_OK_KO icr;	     /* compte-rendu de la fonction             */
  CAI_FILE_DA  *fd;          /* Descripteur du fichier DA               */
  int Infos[3];              /* infos colormap pour attribution R V B   */
  int taille_cell;
  char fic_DA[1024];

  strcpy(fic_DA,fdima->NOM_IMAGE);
 
/*** Ouverture du fichier de donnees auxilliaires ***/

  mode = CAI_ECRITURE;

  fd=cai_openDA(fic_DA,fdima->TYPE_IMAGE,fdima->NB_CANAUX,mode);	
  if (fd == NULL)
     goto ERREUR;
  else
  {
/*** Initialisation et ecriture du champ infos colormap ***/

     strcpy(mot_cle,"COLORMAPINFO");
     mot_cle[12]='\0';
     nb_lig=1;
     nb_col=3;
     type = CAI_ENTIER4;
     taille_cell= sizeof (int);
     Infos[0]=1;
     Infos[1]=1;
     Infos[2]=1;
     icr = cai_ajouteDA(fd,mot_cle,type,nb_lig,
			nb_col,taille_cell,Infos);
     if (icr == CAI_KO)
        goto ERREUR;
  

/*** Initialisation des caracteristiques des donnees Colormap ***/
   
     strcpy(mot_cle,"COLORMAP");
     mot_cle[8]='\0';
     nb_lig=3;
     nb_col=256;
     type = CAI_ENTIER4;
     taille_cell= sizeof (int);

/*** Ecriture des donnees de Colormap ***/

     icr = cai_ajouteDA(fd,mot_cle,type,nb_lig,
			nb_col,taille_cell,fdima->TABCOLOR);
     if (icr == CAI_KO)
        goto ERREUR;
  
/*** Fermeture du fichier de Donnees Auxilliaires ***/
     icr = cai_closeDA(fd);
   }
   return (CAI_OK);

ERREUR:
   return(CAI_KO);
}
/*
	Fin de l'operation cai_ecriture_colormap
*/

