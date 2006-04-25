/*****************************************************************************/
/* PROJET :    COUCHE ACCES IMAGE                                            */
/* -------								     */
/*                                                                           */
/* MODULE :    cai_spot.c                                                    */
/* -------								     */
/*                                                                           */
/* ROLE   :    Ce module rassemble toutes les fonctions d'acces aux images   */
/* -------     stockees au format spot                                       */ 
/*                                                                           */
/* AUTEUR :       (CS-SI)                                                    */
/* -------								     */
/*                                                                           */
/* DATE DE CREATION :  Octobre 2000                                          */
/* -----------------						             */
/*                                                                           */
/* LANGAGE : C                                                               */
/* --------                                                                  */
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : prise en compte  ecriture SPOT5 TIF et BIL     */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : test sur  fclose  pour PC/LINUX                */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : initialisation de image1=NULL a l'ouverture    */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : anomalie sur sous-echantillonnage en lecture   */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'orthographe sur LATITUDE_ARGUMENT     */
/*			      et Look_Angles				     */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'ecriture detector_id lookangle pour   */
/*                            PAN                                            */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur conversion jour julien dans CAI         */
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : Initialisation du champ COD_PIX (OCT,I2 ou UND)*/
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'ecriture detector_id lookangle pour   */
/*                            PAN                                            */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : indice ind_tab_gain non initialise dans boucle */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : correction anomalie ecriture SOURCE_ID         */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : pas d'ecriture <badlines> si pas de badlines   */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : erreur calcul de la datation des attitudes     */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA : erreur initialisation BYTEORDER en creation    */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA :  mot cle  lignes perdues LOST au lieu de LOSS  */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM :  LEAD_XX.dat devient METADATA.DIM              */
/*			       IMAG_XX.bil (tif) devient IMAGERY.BIL (.TIF)  */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA :  Prise en compte date de la scene non shiftée */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'attribut href=""                   */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'attribut version="5.2"             */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element PIXEL_ORIGIN               */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Deplacement de Simplified_Location_Model      */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element lc_List                    */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element pc_List                    */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout attribut href="http://www.spotimage.fr" */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element DATA_TYPE                  */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement de SATURATE par SATURATED        */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement 255 par 1 dans les element LEVEL */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement de BAND par BAND_DESCRIPTION     */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : ajout de l'unite dans PHYSICAL_UNIT           */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM :  Ajout de l'element  Bad_Line_List            */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM :  Ds les dates, utilisation du . au lieu de ,  */
/*			       Separation de la date et heure par "T"       */
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Look_Angles_List            */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Angles_List                 */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Angular_Speeds_List         */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Instrument_look_Angles_List */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA : SPECIAL_VALUE_TEXT  NODATA a la place NO_DATA  */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Version DIMAP passe de 2.0 à 2.9               */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Rajouter ALPHA_LEVEL unqiement pour SPOT4 MIR  */
/*****************************************************************************/ 
/* MOD : VERSION : 4.8 : FA : Rajout SCENE au début de chaine DATASET_NAME   */
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour FRAME_ROW, FRAME_COL   */
/*			      et pour TIE_POINTS_DATA_X et TIE_POINTS_DATA_Y */
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Blindage de la reconnaissance du fichier image */
/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Remplacer les appels systemes pour renommer un */
/*                            fichier par la commande C rename               */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : DIMAP Ajouter un espace entre SCENE et numsat  */
/*                            dans le DATASET_NAME                           */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : DIMAP dans le DATASET_FRAME, inversion des deux*/
/*                            derniers vertex : à mettre dans l'ordre        */
/*				NO,NE,SE,SO                                  */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : DIMAP remplissage des tags HORIZONTAL_CS_CODE  */
/*                            et HORIZONTAL_CS_NAME avec des valeurs défauts */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : DIMAP tag LOOK_ANGLE_LIST valeurs de PSI_X et  */
/*                            PSI_Y 1er et dernier detecteur inverses        */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : Prise en compte de la lecture sur systeme LE   */
/*                            pour inversion des octets SPOT1A4              */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : Correction sprintf(nom_fic,"IMAGERY") au lieu  */
/*                                de sprintf(nom_fic,"IMAGERY",nom_rep)      */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : Correction  de                                 */
/*            sprintf(valeur,"%d", (ind_tab_gain+1+1500*i)) au lieu de       */
/*		sprintf(valeur,"%f", (ind_tab_gain+1+1500*i))                */
/*****************************************************************************/
/* MOD : VERSION : 5.3 : DM :  portage de la couche CAI dans plateforme      */
/*                             Microsof Visual C++ V6.0                      */
/*****************************************************************************/




/*---------------------------------------------------------------------------*/
/*                Inclusion des fichiers STANDARDS (.h)                      */
/*---------------------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# ifndef _MSC_VER
# include <strings.h>
# endif
#include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <malloc.h>
# include <math.h>
# include <unistd.h>
# include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define _CAI_IMAGE
#define IN_CAI_SPOT

# include "cai_image.h"

/*----------------------------------------------------------------------------*/
/*                 Definition des CONSTANTES PROPRES                          */
/*----------------------------------------------------------------------------*/

#define FREE(A)        { if ((A)!=NULL) { free(A); (A)=NULL; } }
#define NOMBRE_DE_BITS "NBITS"
#define NOMBRE_DE_LIGNES "NROWS"
#define NOMBRE_DE_COLONNES "NCOLS"
#define NOMBRE_DE_CANAUX "NBANDS"
#define ORDRE_LECTURE_OCTETS "BYTEORDER"
#define NB_OCTETS_A_SAUTER "SKIPBYTES"
#define NB_SEC_PAR_JOUR 86400
#define NB_MIN_PAR_JOUR 1440
#define NB_HEURE_PAR_JOUR 24
#define NB_JOURS_AN_BISSEX 366
#define NB_JOURS_AN_NON_BISSEX 365
#define NB_CENTIEME_PAR_JOUR 8640000
#define ANNEE_DEBUT_JULIEN 1950
#define PI 3.141592653589

/*----------------------------------------------------------------------------*/
/*                 Declaration des OPERATION INTERNES (static)                */
/*----------------------------------------------------------------------------*/

static void cai_DMS_To_Float ( char DegMinSec [17] , double *Degre );
static CAI_OK_KO cai_WriteElementXML ( FILE *desc , char *motCle ,
                                             char *valeur );
static CAI_OK_KO cai_WriteOpenTagXML ( FILE *des , char *TagName );
static CAI_OK_KO cai_WriteCloseTagXML ( FILE *des , char *TagName );
static CAI_OK_KO cai_WriteNulTagXML ( FILE *des , char *TagName );
static CAI_OK_KO cai_WriteEnteteXML ( FILE *des );
static CAI_OK_KO cai_WriteElementXMLWithAttribute ( FILE *desc ,
                                                          char *motCle ,
							  char *valeur,
							  char *attribut,
							  char *valat ,
							  int type);

static void cai_JulToGreg ( long nbjours , double nbsec , char* dategreg );
static double cai_DegreeToRadian ( double angle_degree );
static int cai_addtime ( char ret[26] , char temps[18] , double milisec );
static int cai_recup_nom_spot(char typspot,char * str_repertoire ,
						char * str_debut ,
					        char **str_fichier );
static CAI_OK_KO cai_lire_dimensions_spot5 ( char *fich_xml , int *nbcan ,
					     int *nboct , int *nbcol ,
					     int *nblig , char *byteorder ,
					     int *skipbytes );
static CAI_IMAGE *cai_ouvre_creation_spot1a4(char *repert,
				   		char *nom_image,
				   		int *nbca,
				   		int *nboct_pix,
				   		int *nbcol,
				   		int *nblig,
				   		char *label);
static CAI_IMAGE *cai_ouvre_creation_spot5TIF(char *repert,
				   		char *nom_image,
				   		int *nbca,
				   		int *nboct_pix,
				   		int *nbcol,
				   		int *nblig,
				   		char *label);
static CAI_IMAGE *cai_ouvre_creation_spot5BIL(char *repert,
				   		char *nom_image,
				   		int *nbca,
				   		int *nboct_pix,
				   		int *nbcol,
				   		int *nblig,
				   		char *label);
static char* cai_spot5_gettoken ( FILE *MyFile );

/*----------------------------------------------------------------------------*/
/* Variables globales                                                         */
/*----------------------------------------------------------------------------*/

char *type_machine;         /* ordre rangement octets machine de travail     */
static char Command[1024];  /* commande envoye au shell			     */

/*----------------------------------------------------------------------------*/
/* Variable pour la  gestion en configuration                                 */
/*----------------------------------------------------------------------------*/

static char *rcs_id="$Header: cai_spot.c,v 1.3 01/05/31 10:47:53 cmf Exp $";


/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*                Definition des OPERATIONS EXTERNES                         */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_liste_ima_spot                                         */
/* ----------                                                                */
/*                                                                           */
/* ROLE :    fonction de determination des images au format spot             */
/* ------                                                                    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  int cai_liste_ima_spot(typspot,repert,tab_ima)        */
/* -----------------                                                         */
/*                                                                           */
/*     typspot      (E) (char)     : type spot (1=1a4,b=5bil ou t=5tif)      */
/*     repert       (E) (char *)   : repertoire a explorer		     */
/*     tab_ima      (S) (char ***) : pointeur sur tableau des images listees */
/*                                    alloue par la fonction,il doit etre    */
/*                                    LIBERE PAR L'APPELANT                  */
/*									     */
/*     cai_liste_ima_spot   (S) (int)     :     = -1 repertoire inexistant   */                                    /*					       = > 0 nombre d'images listees */
/*                                                                           */
/* VERSION :  V2                                                             */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

long int cai_liste_ima_spot (char typspot, char *repert,
		             char ***tab_ima)

 {
   DIR *dirp;		    /* pointeur sur structure ouverture repertoire   */
   struct dirent *dp;       /* structure resultat de la fonction readdir     */
   struct stat buf_stat;    /* structure contenant resultat stat             */
   char *nom_entete_spot;   /* nom fichier entete image SPOT                 */
   char *nom_trailer_spot;  /* nom fichier trailer image SPOT                */
   char *nom_fic_spot;      /* nom fichier image SPOT                        */
   char *nom_vol_spot;      /* nom fichier pour volume repertoire image SPOT */
   char *nom_NULL_spot;     /* nom fichier volume NULL repertoire image SPOT */ 
   int nb_ima;              /* nombre d'images lum identifiees               */                    

   int cr,i;                /* compte rendu appel fonction                   */ 
   int v1,v2;               /* comparaison nom repertoire avec . et ..       */
   char path_rep[PATH_MAX]; /* chemin acces fichiers ds repertoire courant   */
   
/*........................................
   INITIALISATION et ouverture du repertoire courant choisi
  .........................................*/
  
   nb_ima=0;
     
   dirp=opendir(repert);

   if(dirp!=NULL)
   {
/*......................................

   premiere passe pour comptabiliser les images du format donne

   POUR CHAQUE REPERTOIRE DU REPERTOIRE COURANT,
      TEST POUR SAVOIR SI CE REPERTOIRE CONTIENT 
      	TOUS LES FICHIER PROPRES AU FORMAT SPOT 
     ..........................................*/
     
    while( (dp=readdir(dirp))!=NULL )
    {
    
    /********************************************/
    /*  on ne doit pas tenir compte des . et .. */
    /*      sous le repertoire courant          */
    /*******************************************/
    
    	v1 = strcmp(dp->d_name, ".");
	v2 = strcmp(dp->d_name, "..");
	
        if ( (v1 != 0) && (v2 != 0) )
	{
	   sprintf(path_rep, "%s/%s", repert, dp->d_name);
		
	   /*--------------------------------*/		
	   /*test sur exitence du repertoire */
	   /*--------------------------------*/
		
	   cr = stat(path_rep,&buf_stat);
			
    	   if ( (cr!=-1) && ((buf_stat.st_mode&S_IFMT) == S_IFDIR) )
    	   {	     		
 /* verification existence des fichiers propres au format SPOT 1 a 4*/
	     if (typspot == '1')
             {
    			if ( 
	(cai_recup_nom_spot(typspot,path_rep,"lea",&nom_entete_spot) == 0) && 
        (cai_recup_nom_spot(typspot,path_rep,"tra",&nom_trailer_spot) == 0) &&
        (cai_recup_nom_spot(typspot,path_rep,"ima",&nom_fic_spot) == 0) &&
        (cai_recup_nom_spot(typspot,path_rep,"vol",&nom_vol_spot) == 0) && 
	( cai_recup_nom_spot(typspot,path_rep,"NUL",&nom_NULL_spot) == 0) )
    			{
				/*image SPOT1 4*/
				nb_ima=nb_ima+1;
    			}
              }      
	      else if (typspot == 'B')
              {
 /* verification existence des fichiers propres au format SPOT5 BIL */
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/
		 if ( 
	(cai_recup_nom_spot(typspot,path_rep,"METADATA.DIM",&nom_entete_spot) == 0) &&
	(cai_recup_nom_spot(typspot,path_rep,"im",&nom_fic_spot) == 0) )
			{
			        /*image SPOT5 BIL*/
				nb_ima=nb_ima+1;
			}
              }
	      else if (typspot == 'T')
              {
 /* verification existence des fichiers propres au format SPOT5 TIF */
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/
		 if ( 
	(cai_recup_nom_spot(typspot,path_rep,"METADATA.DIM",&nom_entete_spot) == 0) &&
	(cai_recup_nom_spot(typspot,path_rep,"im",&nom_fic_spot) == 0) )
			{
			        /*image SPOT5 TIF*/
				nb_ima=nb_ima+1;
			}
	      }
     	   } /*fin if cr*/
		
     	} /*fin if (v1 v2)*/
	
     } /*fin while*/
     
     /*fermeture du repertoire*/
     (void)closedir(dirp);
    
/*.....................................
     ALLOCATION DU TABLEAU CONTENANT LA LISTE DES IMAGES DU FORMAT SPOT
     ......................................*/
    *tab_ima = (char **) calloc( nb_ima,sizeof(char *) );
    for (i=0;i<nb_ima;i=i+1)
      (*tab_ima)[i]=(char *)calloc(1,100);

/*......................................

   deuxieme passe pour constituer la liste des images du format donne

  POUR CHAQUE REPERTOIRE DU REPERTOIRE COURANT,
      TEST POUR SAVOIR SI CE REPERTOIRE CONTIENT 
      	TOUS LES FICHIER PROPRES AU FORMAT SPOT 
   SI C`EST LE CAS,STOCKAGE DANS LA LISTE
   ..........................................*/ 
	 
   dirp=opendir(repert);

   nb_ima=0;
 
   while((dp=readdir(dirp))!=NULL)
   {
   
   /**********************************************/
   /*   on ne doit pas tenir compte des . et ..  */
   /*       sous le repertoire courant           */
   /**********************************************/
    
      v1 = strcmp(dp->d_name, ".");
      v2 = strcmp(dp->d_name, "..");
	
      if ( (v1 != 0) && (v2 != 0) )
      {		
	sprintf(path_rep, "%s/%s", repert,dp->d_name);	
		
	/*-------------------------------------*/
	/*   test sur exitence du repertoire   */
	/*-------------------------------------*/
		
	cr = stat(path_rep,&buf_stat);
			
    	if ( (cr!=-1) && ((buf_stat.st_mode&S_IFMT) == S_IFDIR) )
    	{	     		
    	 /* verification existence des fichiers propres au format SPOT 1 a 4*/
	     if (typspot == '1')
             {
    			if ( 
	(cai_recup_nom_spot(typspot,path_rep,"lea",&nom_entete_spot) == 0) && 
        (cai_recup_nom_spot(typspot,path_rep,"tra",&nom_trailer_spot) == 0) &&
        (cai_recup_nom_spot(typspot,path_rep,"ima",&nom_fic_spot) == 0) &&
        (cai_recup_nom_spot(typspot,path_rep,"vol",&nom_vol_spot) == 0) && 
	( cai_recup_nom_spot(typspot,path_rep,"NUL",&nom_NULL_spot) == 0) )
    			{
				/*image SPOT1 4*/
				nb_ima=nb_ima+1;
				strcpy( (*tab_ima)[nb_ima-1],dp->d_name );
    			}
              }      
	      else if (typspot == 'B')
              {
 /* verification existence des fichiers propres au format SPOT5 BIL */
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/
		 if ( 
	(cai_recup_nom_spot(typspot,path_rep,"METADATA.DIM",&nom_entete_spot) == 0) &&
	(cai_recup_nom_spot(typspot,path_rep,"im",&nom_fic_spot) == 0) )
			{
			        /*image SPOT5 BIL*/
				nb_ima=nb_ima+1;
				strcpy( (*tab_ima)[nb_ima-1],dp->d_name );
			}
              }
	      else if (typspot == 'T')
              {
 /* verification existence des fichiers propres au format SPOT5 TIF */
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/
		 if ( 
	(cai_recup_nom_spot(typspot,path_rep,"METADATA.DIM",&nom_entete_spot) == 0) &&
	(cai_recup_nom_spot(typspot,path_rep,"im",&nom_fic_spot) == 0) )
			{
			        /*image SPOT5 TIF*/
				nb_ima=nb_ima+1;
				strcpy( (*tab_ima)[nb_ima-1],dp->d_name );
			}
	      }
  	   } /*fin if cr*/
			
       	}/*fin if (v1 v2)*/	

   }/*fin while*/
     
    /*fermeture du repertoire*/ 
   (void)closedir(dirp);

   }
   else
   {
     /* Erreur : Repertoire inexistant sous repertoire courant */
      nb_ima=-1;
   }

    return(nb_ima);

}
/* Fin de l'operation cai_liste_ima_spot*/

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_lecture_spot                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture d'une image au format spot      */
/* -----     et de tous les fichiers qui la composent                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_lecture_spot(typspot,nom_image,  */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*                                                                           */
/*     typspot        (E) (char)   : type spot (1=1a4,b=5bil ou t=5tif)      */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_lecture_spot (S) (CAI_IMAGE *)  : = NULL si pb              */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */                                                                           /*                                                                           */  /* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_lecture_spot(char typspot,char *repert,
				  char *nom_image,
				  int *nb_can,
				  int *nboct_pix,
				  int *nb_col,
				  int *nb_lig)

{
DIR *dirp;		    /* pointeur sur structure ouverture repertoire   */
CAI_IMAGE *image1;          /* pointeur su descripteur de fichier image      */ 
struct dirent *dp;          /* structure resultat de la fonction readdir     */
char fichier_clefs[1024];   /* Nom du fichier des donnees auxiliaires        */
int i,j,k;		    /* Variables de boucle                           */
int descrip_leader;	    /* Descripteur du fichier LEADER                 */
int descrip_imagery;	    /* Descripteur du fichier IMAGERY                */
int descrip_volume;	    /* Descripteur du fichier VOLUME                 */
long cr_lseek;		    /* Code retour de lseek                          */
long cr_read;		    /* Code retour de read                           */
long lg_saut;		    /* Nombre d'octets a sauter                      */
int  lg_enreg,lgtemp;	    /* longueur d'un enregistrement                  */
int NbCan;                  /* nombre de canaux de l'image                   */
int test1,test2,test3,test4,test5;/* test sur nom de fichiers                */
int lg_entete;	            /* longueur de l'entete fichier IMAGERY          */
int num,trouve;		    /* variable de test                              */
char nom_fic[1024];         /* nom du fichier en-tete		             */
char nom_fic_canal[1024];   /* nom fichier canal dont on teste existence     */
char nom_rep[1024];         /* nom du repertoire image SPOT		     */
char nom_fic_ima[1024];     /* nom du fichier imagery.dat	             */
char nom_leader[1024];      /* nom du fichier leader		             */
char path_rep[PATH_MAX];    /* chemin acces fichiers ds repertoire courant   */
char nom_fic_spot5[PATH_MAX];/* chemin acces fichier image tiff ou BIL SPOT5 */
char nom_ima_spot5[PATH_MAX];/* nom du fichier image spot5                   */
char *pch0,*pch1;            /* pointeur sur chaine de caracteres            */
char car[2],carpoint;           /* variable de travail                          */
int type_spot;               /* type d'image spot                            */
char byteorder[1];           /* Ordre de codage intel (LE) ou Motorola (BE)  */
int skipbytes;               /* Taille entete pour les fichiers BIL SPOT5    */
int Trouve;
CAI_OK_KO icr;


/* Donnees auxiliaires */

char TABLEAU[1][257];	/* Variable intermediaire */

/* premiere variable = pour le nombre d'elements = nbre de colonnes */
/* deuxieme element = pour la taille de l'element */


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
  descrip_leader = 0;
  descrip_imagery = 0;
  descrip_volume = 0;
  *nb_can=0;
  *nb_lig=0;
  *nb_col=0;
  *nboct_pix=1;
  type_spot=0;
  trouve=0;
  car[0]='/';

#ifdef _WIN
      car[0]='\\';
#endif

/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : initialisation de image1=NULL a l'ouverture    */
/*****************************************************************************/
   image1= NULL;   
 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/   
   
  type_machine = cai_test_pFpf();

/******************************************************************************/
/*	Determination du type d'image : SPOT 1 a 4 ou SPOT 5                  */
/******************************************************************************/
sprintf(nom_rep, "%s%s%s", repert,car, nom_image);

dirp=opendir(nom_rep);

 
if(dirp!=NULL)
 {
  while( ((dp=readdir(dirp))!=NULL )&& (trouve ==0))
  {
   
  /*---------------------------------------------*/
  /*  on ne doit pas tenir compte des . et ..    */
  /*        sous le repertoire courant           */
  /*---------------------------------------------*/
    
   test1 = strcmp(dp->d_name, ".");
   test2 = strcmp(dp->d_name, "..");
	
   if ( (test1 != 0) && (test2 != 0) )
   {
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Blindage de la reconnaissance du fichier image */
/*****************************************************************************/
     
  /*---------------------------------------------*/
  /*  on recherche le fichier Image              */
  /*---------------------------------------------*/
     test3 = strncasecmp(dp->d_name,"IMAG", 4);
     if (test3 == 0)
     {
        test4 = strcasecmp(dp->d_name,"imagery.tif");
        if (test4==0)  /* format SPOT5TIF */
        {
                type_spot = 2;
		sprintf (nom_fic_spot5, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
		sprintf (nom_ima_spot5, "%s", dp->d_name);
		trouve=1;
        }
        else 
        {
	    test4 = strcasecmp(dp->d_name,"imagery.bil");
            if (test4==0)  /* format SPOT5BIL */
            {
                type_spot = 3;
		sprintf (nom_fic_spot5, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
		sprintf (nom_ima_spot5, "%s", dp->d_name);
		trouve=1;
             }
	     else /* format SPOT 1 a 4*/
             {
		carpoint = '.';
                pch0 = strchr (dp->d_name, carpoint);
		if  (pch0 == NULL)
                {
			type_spot = 1;     /* format SPOT 1 a 4*/
     		        sprintf(nom_fic_ima, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
			trouve=1;
                }
                else if (strcasecmp(pch0,".DAT")== 0)
                {
			type_spot = 1;     /* format SPOT 1 a 4*/
     		        sprintf(nom_fic_ima, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
			trouve=1;
                }

             }		
        }
     }
    }

  }
  closedir(dirp); 
 }


if (type_spot == 0)
{
	sprintf (CAI_ERREUR,"Erreur : type d'image SPOT inconnu");
	goto ERREUR;
}
/*........................................................................*/
/* Verication ed la coherence du type spot annonce et du type spot trouve */
/*........................................................................*/
if (((typspot == '1')&&(type_spot != 1))||
		((typspot == 'T')&&(type_spot != 2))||
		((typspot == 'B')&&(type_spot != 3)))
{
	    sprintf (CAI_ERREUR,
			"Erreur : L'image n'est pas du type SPOT annonce");
	    goto ERREUR;
}

/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 1 A 4 */
/*=============================================*/
if (type_spot == 1)
{
   image1= NULL;    
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   image1->ACCES_SPOT[0]=0;
   image1->ACCES_SPOT[1]=0;

/*****************************************************************************/
/* tests sur les retours d'allocation  memoire                               */
/*****************************************************************************/

   if (image1==NULL)
   {
       strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
       goto ERREUR;
    }

/******************************************************************************/
/*	Ouverture des fichiers LEADER et IMAGERY en lecture		      */
/******************************************************************************/

    sprintf(nom_rep, "%s%s%s", repert,car, nom_image);
    dirp=opendir(nom_rep);
 
    if(dirp!=NULL)
    {
      while( (dp=readdir(dirp))!=NULL )
      {
   
  /*---------------------------------------------*/
  /*  on ne doit pas tenir compte des . et ..    */
  /*        sous le repertoire courant           */
  /*---------------------------------------------*/
    
        test1 = strcmp(dp->d_name, ".");
        test2 = strcmp(dp->d_name, "..");
	
        if ( (test1 != 0) && (test2 != 0) )
        {
     
     	   test3 = strncasecmp(dp->d_name,"LEAD", 4);
     	   test4 = strncasecmp(dp->d_name,"IMAG", 4); 
     	   test5 = strncasecmp(dp->d_name,"VOL", 3);   
    
     /********************************************************/
     /* tests de determination du fichier LEADER sous repert */
     /********************************************************/
     
           if (test3==0)
           {
     		 sprintf(path_rep, "%s%s%s%s%s", repert, car,nom_image, 
								car,dp->d_name);
      
      /*ouverture du fichier LEADER*/
            
      		descrip_leader = open (path_rep, O_RDONLY |O_BINARY);
      		if (descrip_leader == -1)
      		{
      	           strcpy(CAI_ERREUR,
			"Erreur d'ouverture du fichier LEADER SPOT");
       		   goto ERREUR;
      		}        
        /*---------------------------------------------------------*/
	/* Lecture de la taille d'un enregistrement dans LEADER    */
	/*---------------------------------------------------------*/

		cr_lseek = lseek ( descrip_leader , 8L , 1 );
		if ( cr_lseek == -1 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
		  goto ERREUR;
		}

		cr_read = read ( descrip_leader , &lg_enreg , 4 );
		if ( cr_read == -1 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}
	        if (strcmp(type_machine,"LE")==0)
                {
		  swab( (void*)&lg_enreg, (void*)&lgtemp,4 );
		  cai_inverser_tableau((short *)&lg_enreg, (short *)&lgtemp,2);
                }
	
	/*------------------------------------------------------*/
	/* Retour au debut du fichier LEADER		        */
	/*------------------------------------------------------*/

		cr_lseek = lseek ( descrip_leader , 0L , 0 );
		if ( cr_lseek == -1 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}

        /*------------------------------------------------------*/
	/* Saut de lg_enreg + 996 octets dans LEADER		*/
	/*------------------------------------------------------*/

		lg_saut = (long)(lg_enreg + 996);
		cr_lseek = lseek ( descrip_leader , lg_saut , 0 );
		if ( cr_lseek == -1 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}
	/*------------------------------------------------------*/
	/* Lecture du nombre de colonnes dans LEADER	        */
	/*------------------------------------------------------*/

  		cr_read = read (descrip_leader, TABLEAU[0], 16);
		if ( cr_read != 16 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}
        	TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
        /* On met un masque sur le 8ieme bit de chaque octet : */

  		for (i=0;i<16;i++)
  		{
			TABLEAU[0][i] = TABLEAU[0][i] & 0177;
  		}

 		sscanf ( TABLEAU[0], "%d", nb_col);
	/*------------------------------------------------------*/
	/* Lecture du nombre de lignes dans LEADER		*/
	/*------------------------------------------------------*/

  		cr_read = read (descrip_leader, TABLEAU[0], 16);
		if ( cr_read != 16 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}
       		TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
        /* On met un masque sur le 8ieme bit de chaque octet : */

  		for (i=0;i<16;i++)
  		{
			TABLEAU[0][i] = TABLEAU[0][i] & 0177;
  		}

 		sscanf ( TABLEAU[0], "%d", nb_lig);

	/*------------------------------------------------------*/
	/* Saut de 16 octets dans LEADER			*/
	/*------------------------------------------------------*/

		cr_lseek = lseek ( descrip_leader , 16L , 1 );
		if ( cr_lseek == -1 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}
	
	/*------------------------------------------------------*/
	/* Lecture du nombre de canaux	dans LEADER		*/
	/*------------------------------------------------------*/

 		cr_read = read (descrip_leader, TABLEAU[0], 16);
		if ( cr_read != 16 )
		{
			strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT");
			goto ERREUR;
		}
        	TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
        /* On met un masque sur le 8ieme bit de chaque octet : */

  		for (i=0;i<16;i++)
  		{
			TABLEAU[0][i] = TABLEAU[0][i] & 0177;
  		}

 		sscanf ( TABLEAU[0], "%d", nb_can);	
   	   } /*fin test3 (v==0)*/
     
          else  if (test5==0)
          {
    /********************************************************/
    /* tests de determination du fichier VOLUME sous repert */
    /********************************************************/

      		sprintf(path_rep, "%s%s%s%s%s", repert, car,nom_image, 
							car,dp->d_name);      
      	/*ouverture du fichier VOLUME*/
            
      		descrip_volume = open (path_rep, O_RDONLY|O_BINARY );
      		if (descrip_volume == -1)
      		{
       		     strcpy(CAI_ERREUR,
			"Erreur d'ouverture du fichier VOLUME SPOT");
       		     goto ERREUR;
                 }
           }
    /********************************************************/
    /* tests de determination du fichier IMAGERY sous repert */
    /********************************************************/
    	       
           else if (test4==0)
           {   
     
      /*ouverture du fichier IMAGERY*/
      
      		descrip_imagery = open (nom_fic_ima, O_RDONLY|O_BINARY );
      		if (descrip_imagery == -1)
      		{
      		   strcpy(CAI_ERREUR,
			"Erreur d'ouverture du fichier IMAGERY SPOT");
       	           goto ERREUR;
      		}
                  
      /*------------------------------------------------------*/
      /* Lecture de la taille de l'entete dans IMAGERY	      */
      /*------------------------------------------------------*/

  		cr_lseek = lseek ( descrip_imagery , 8L , 1);
  		if ( cr_lseek == -1 )
  		{
			strcpy(CAI_ERREUR,
			  "Erreur acces au fichier contenant donnees SPOT");
			goto ERREUR;
  		}	

  		cr_read = read ( descrip_imagery , &lg_entete , 4 );
  		if ( cr_read == -1 )
  		{
			strcpy(CAI_ERREUR,
			  "Erreur acces au fichier contenant donnees SPOT");
			goto ERREUR;
  		}
	        if (strcmp(type_machine,"LE")==0)
                {
		  swab( (void*)&lg_entete, (void*)&lgtemp,4 );
		  cai_inverser_tableau((short *)&lg_entete, (short *)&lgtemp,2);
                }
      
    	  } /*fin else if test4*/
    	       
        } /*fin else test1 test2*/
   
     }  /*fin while*/
                   
     closedir(dirp); 
 
   }  /*fin if (dirp=!NULL)*/


/* verification que tous les fichiers necessaires (lead,imag et vol ont ete trouves */
  if (descrip_leader == 0)
  {
      sprintf(CAI_ERREUR,"Erreur : Fichier Leader SPot1a4 absent");
      goto ERREUR;
  }
  if (descrip_imagery == 0)
  {
      sprintf(CAI_ERREUR,"Erreur : Fichier imagery SPot1a4 absent");
      goto ERREUR;
  }
  if (descrip_volume == 0)
  {
      sprintf(CAI_ERREUR,"Erreur : Fichier volume SPot1a4 absent");
      goto ERREUR;
  }
/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

  nom_fic[0]='\0';
  nom_fic_canal[0]='\0';
     
  sprintf(nom_fic_canal,"%s",nom_fic_ima);   

  for (i=0;i<*nb_can;i=i+1)
  {
    sscanf(nom_image,"%s",image1->NOM_FIC_CAN);
    sscanf(nom_image,"%s",image1->NOM_UTIL_CAN);
    image1->ACCES_CANAL_INT[i]=descrip_imagery;
    image1->ACCES_SPOT[0]=descrip_leader;
    image1->ACCES_SPOT[1]=descrip_volume;
    image1->POS_OCTET[i]=lg_entete;	   
  }
    image1->TYPE_SPOT = type_spot;
    image1->NBOCT_PIX = *nboct_pix;
      
 }  /* fin TYPE IMAGE SPOT 1 a 4 */ 
    /*===========================*/
 
/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 5 TIF */
/*=============================================*/
 else if (type_spot == 2)
 {

/* Ouverture de l'image */
    image1  = cai_ouvre_lecture_image (nom_fic_spot5, "TIFF", 
                                        nb_can,nboct_pix,nb_col,nb_lig);

  if (image1 == NULL)
  {
  	strcpy(CAI_ERREUR,
		"Erreur acces au fichier image TIFF contenant donnees SPOT");
	goto ERREUR;
  }

   image1->NB_CANAUX = *nb_can  ;
   image1->NBOCT_PIX = *nboct_pix  ;
   image1->NB_COL =  *nb_col  ;
   image1->NB_LIG = *nb_lig ;
   image1->TYPE_SPOT = type_spot;
   image1->ACCES_SPOT[0]=0;
   image1->ACCES_SPOT[1]=0;


 }  /* fin TYPE IMAGE SPOT 5 TIFF */ 
    /*============================*/

/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 5 BIL */
/*=============================================*/
 else
 {

   image1= NULL;    
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   image1->ACCES_SPOT[0]=0;
   image1->ACCES_SPOT[1]=0;
  
/*****************************************************************************/
/* tests sur les retours d'allocation  memoire                               */
/*****************************************************************************/

   if (image1==NULL)
   {
       strcpy(CAI_ERREUR,"Erreur allocation memoire dans cai_ouvre_lecture");
       goto ERREUR;
    }

   /* Recherche du leader Dimap */
     dirp=opendir(nom_rep);
     Trouve=0; 
     if(dirp!=NULL)
     {
        while( (dp=readdir(dirp))!=NULL )
        {
   
  /*---------------------------------------------*/
  /*  on ne doit pas tenir compte des . et ..    */
  /*        sous le repertoire courant           */
  /*---------------------------------------------*/
    
   	   test1 = strcmp(dp->d_name, ".");
   	   test2 = strcmp(dp->d_name, "..");
	
  	   if ( (test1 != 0) && (test2 != 0) )
    	   {
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/     
     	     test3 = strncasecmp(dp->d_name,"METADATA.DIM", 12);
    
     /**************************************************************/
     /* tests de determination du fichier METADATA.DIM sous repert */
     /**************************************************************/
     	     if (test3==0)
     	     {
      	          sprintf(nom_leader, "%s%s%s%s%s", repert,car, nom_image, 
							   car, dp->d_name);
      

/* Extraction du nombre de lig col can et nboct du fichier DIMAP */
/*...............................................................*/
    		   icr = cai_lire_dimensions_spot5(nom_leader,nb_can,nboct_pix ,
                               			   nb_col , nb_lig,byteorder,
						   &skipbytes );
	           if (icr != CAI_OK)
                   {
			goto ERREUR;
                   }
		   else
			Trouve=1;
             }
            } /*fin else test1 test2*/
   
         }  /*fin while*/
                   
         closedir(dirp); 
 
      }  /*fin if (dirp=!NULL)*/

      if (Trouve==0)
      {
	 sprintf(CAI_ERREUR,
	  "Erreur Le fichier METADATA.DIM n'existe pas ou erreur de lecture");
	 goto ERREUR;
       }


/*   INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic[0]='\0';
     nom_fic_canal[0]='\0';

     sprintf(nom_fic_canal,"%s",nom_fic_spot5);

     num=open(nom_fic_canal,O_RDONLY|O_BINARY,0);
     if (num!= -1)
     {
       for (i=0;i<*nb_can;i=i+1)
       {
    		sscanf(nom_ima_spot5,"%s",image1->NOM_FIC_CAN);
    		sscanf(nom_ima_spot5,"%s",image1->NOM_UTIL_CAN);
    		image1->ACCES_CANAL_INT[i]=num;
           	image1->POS_OCTET[i]=(*nb_col)*i*(*nboct_pix)+skipbytes;
        }
        image1->TYPE_SPOT = type_spot;
        image1->NBOCT_PIX = *nboct_pix;
        if (byteorder[0]=='M')
        {
	   if (strcmp(type_machine,"BE")==0)
		image1->INV_OCTETS=0;
	   else
		image1->INV_OCTETS=1;
        }
        else
        {
	   if (strcmp(type_machine,"LE")==0)
		image1->INV_OCTETS=0;
	   else
		image1->INV_OCTETS=1;
        }

      }
      else
      {
  	strcpy(CAI_ERREUR,
		"Erreur ouverture fichier BIL contenant donnees SPOT");
	goto ERREUR;
      }

 }  /* fin TYPE IMAGE SPOT 5 BIL */ 

    /*===========================*/


/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : Initialisation du champ COD_PIX (OCT,I2 ou UND)*/
/*****************************************************************************/
 if (*nboct_pix==1) 
	sprintf(image1->COD_PIX,"OCT");
 else if (*nboct_pix==2) 
	sprintf(image1->COD_PIX,"I2");
 else 
	sprintf(image1->COD_PIX,"UND");


 return(image1);


ERREUR:
  if (descrip_leader !=0) close(descrip_leader);
  if (descrip_imagery !=0) close(descrip_imagery);
  if (descrip_volume !=0) close(descrip_volume);
  if (image1!=NULL) free(image1);
  return(NULL);
}
/* 
   Fin de l'operation cai_ouvre_lecture_spot
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_spot                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format spot et de tous les fichiers qui la composent         */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_spot(typspot,nom_image,  */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     typspot        (E) (char )  : type d'images spot 91a4,5tif ou 5bil)   */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_spot (S) (CAI_IMAGE *) :     =NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_IMAGE *cai_ouvre_creation_spot(char typspot,char *repert,
				   char *nom_image,
				   int *nbca,
				   int *nboct_pix,
				   int *nbcol,
				   int *nblig,
				   char *label)

{

   CAI_IMAGE *image1;	     /* pointeur su descripteur de fichier image     */
   int icr;		     /* Code retour de fonctions                     */ 

/*........................................
   INITIALISATIONS et allocation de place pour la structure CAI_IMAGE
  .........................................*/
  icr = CAI_OK;


/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : initialisation de image1=NULL a l'ouverture    */
/*****************************************************************************/
   image1= NULL; 

/*-------------------------------------------------------------*/
/* test du type de machine sur laquelle est constituee l'image */
/*-------------------------------------------------------------*/
  type_machine = cai_test_pFpf();

  if (typspot=='1')
	image1 = cai_ouvre_creation_spot1a4(repert,nom_image,nbca,
				   nboct_pix,nbcol,nblig,label);
  else if (typspot=='B')
	image1 = cai_ouvre_creation_spot5BIL(repert,nom_image,nbca,
				   nboct_pix,nbcol,nblig,label);
  else if (typspot=='T')
	image1 = cai_ouvre_creation_spot5TIF(repert,nom_image,nbca,
				   nboct_pix,nbcol,nblig,label);
  if (image1==NULL) icr=CAI_KO;

  if (icr != CAI_OK)
  {
      if (image1!=NULL) free(image1);
      image1 = NULL;
  }
  return(image1);

}

/* 
  Fin de l'operation cai_ouvre_creation_spot
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_lecture_canal_spot                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de lecture d'un canal d'une image LUM                  */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_lecture_canal_spot(image1, canal,       */ 
/* ------------------                        premiere_ligne,                 */
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
/*     cai_lecture_canal_spot (S) (CAI_OK_KO) :     = CAI_KO si pb           */
/*                                                 = CAI_OK si OK            */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_lecture_canal_spot(CAI_IMAGE *image1,int *canal, 
				 int *premiere_ligne, int *premiere_colonne, 
		      		 int *nb_lig_lire, int *nb_col_lire, 
				 int *pas_lig, int *pas_col, 
				 unsigned char *data_image  )        
  
    
{
DIR *dirp;		    /* pointeur sur structure ouverture repertoire   */
struct dirent *dp;          /* structure resultat de la fonction readdir     */
int descrip_imagery;	    /* descripteur du fichier imagery                */
int nb_can;                 /* nombre de canaux de l'image SPOT              */
int num,lig,i,j,k;          /* variables de travail                          */
long  nb_oct;               /* nb oct =1er octet a lire - position courante  */
long  oct_lig;              /* nb oct. par ligne en considerant taille pixel */
long int offset;	    /* Deplacement dans le fichier                   */
long cr_lseek;		    /* Code retour de lseek                          */
long cr_read;		    /* Code retour de read                           */
int lg_prefixe;	            /* longueur du prefixe d'une ligne               */
int lg_suffixe;             /* longueur du suffixe d'une ligne               */
int lg_entete ;             /* longueur de l'entete d'une ligne              */
char *nom_fic;              /* nom contenant le chemin de l'image            */
char path_rep[PATH_MAX];    /* chemin acces fichiers ds repertoire courant   */
unsigned char *buf_lec;     /* buffer lecture d'une  ligne                   */
long pt_oct;		    /* position courante dans le fichier image       */
unsigned char *pch, *deb;   /* pointeur chaine pour transfert  buffer sortie */
int no_oct;                 /* numero du premier octet a lire                */
CAI_OK_KO icr;              /* code retour fonction                          */
int taille_image;           /* Taille en octets de la zone a lire            */
unsigned char *data_image2; /* pointeur donnees image pour inversion octets  */


data_image2 = NULL;
if ( (image1->TYPE_SPOT < 1) || (image1->TYPE_SPOT > 3) )
 {
	sprintf (CAI_ERREUR,"Erreur : type d'image SPOT inconnu");
	goto ERREUR;
 }
/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 1 A 4 */
/*=============================================*/
else if (image1->TYPE_SPOT == 1)
 {
/*...........................
  Initialisations diverses
  ..........................*/
  
  lg_prefixe = 32;
  descrip_imagery = image1->ACCES_CANAL_INT[*canal-1];
  nb_can = image1->NB_CANAUX;

/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : anomalie sur sous-echantillonnage en lecture   */
/*****************************************************************************/
/* Une ligne modifiee */
  oct_lig = image1->NBOCT_PIX*(image1->NB_COL);
              
  /*..............................................
        allocation de memoire pour lire une ligne image
 ............................................. */

   buf_lec = (unsigned char *) calloc(oct_lig,sizeof(unsigned char));
   if (buf_lec == NULL)
   {     
      sprintf(CAI_ERREUR,"ERREUR:Probleme allocation memoire\n");
      goto ERREUR;
   }
   
   else
   {
    /*....................
        Initialisation
    ......................*/
        lig=*premiere_ligne;
        i=1;
        pch=data_image;
	lg_entete=image1->POS_OCTET[*canal-1];

/*.....................
         Pour chaque ligne a lire
  .......................................*/
        while (i <= *nb_lig_lire)
        {  
/*............................................
	    Recherche de la position courante dans le fichier
  ............................................*/
            pt_oct = lseek(descrip_imagery,0L,1);

	    if ( pt_oct == -1 )
    		{
       	 	 strcpy(CAI_ERREUR,"Erreur acces au fichier contenant donnees SPOT");
	  	 goto ERREUR;
   		}	

/*................................
            Calcul du numero du 1er octet a lire
	    no_oct=(*nblig-1)*lg_entete*nbcanaux+lg_entete*(canal-1)+lg_prefixe+(*canal-1)*lg_entete
  ....................................*/
               
	   no_oct = (lig-1)*lg_entete*nb_can+image1->POS_OCTET[*canal-1]+lg_prefixe+(*canal-1)*lg_entete;
	          
/*..................................
            Saut d'octets pour etre positionne sur la ligne a lire
  .......................................*/
            nb_oct=(long)(no_oct-pt_oct);
            lseek(descrip_imagery,nb_oct,1);

/*...........................................
            lecture d'une ligne image
  ................................................*/
            cr_read=read(descrip_imagery,buf_lec,oct_lig);

            if (cr_read==-1)
            {             
             strcpy(CAI_ERREUR,"Erreur lecture donnee image SPOT");
	     goto ERREUR;
            }
            else
            { 
/*......................................
             Sauvegarde des donnees lues en tenant compte du pas 
             d'echantillonnage en colonne
  ........................................*/           
             deb = buf_lec+(*premiere_colonne-1)*image1->NBOCT_PIX;
             j=1;
	     while (j <= *nb_col_lire)
	     {  
               for (k=0;k<image1->NBOCT_PIX;k=k+1)
                {
                  *(pch+k)=*(deb+k);
                }
              j=j+1;
              pch=pch+image1->NBOCT_PIX;
              deb=deb+(*pas_col*image1->NBOCT_PIX);
	      
             }/*fin while j*/ 
                           
            }/*fin else*/
	    
          i=i+1;
          lig=lig+(*pas_lig);

        } /*fin while i*/        

	free(buf_lec);
   } /*fin else*/
   
 
      
 }  /* fin TYPE IMAGE SPOT 1 a 4 */ 
    /*===========================*/
 
/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 5 TIF */
/*=============================================*/
 else if (image1->TYPE_SPOT == 2)
 {

	icr = cai_lecture_canal_tiff (image1, canal, 
					premiere_ligne, premiere_colonne, 
				 	nb_lig_lire, nb_col_lire, pas_lig, 
					pas_col, data_image);
	if (icr != CAI_OK)
                 goto ERREUR;

 }  /* fin TYPE IMAGE SPOT 5 TIFF */ 
    /*============================*/
 
/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 5 BIL */
/*=============================================*/
 else 
 {
      
	icr = cai_lecture_canal_bil (image1, canal, premiere_ligne, 
				     premiere_colonne, nb_lig_lire, nb_col_lire,
				      pas_lig, pas_col, data_image);
	if (icr != CAI_OK)
                 goto ERREUR;
        else
       {
/* Si necessaire inversion des octets */
          if ((image1->INV_OCTETS==1)&&(image1->NBOCT_PIX !=1))
          {
   	      taille_image = (*nb_lig_lire)*(*nb_col_lire)*(image1->NBOCT_PIX);
  	      data_image2 = (unsigned char *) calloc(taille_image,
				sizeof(unsigned char));

              if (data_image2 == NULL)
              {
      		strcpy(CAI_ERREUR,
			"Erreur allocation memoire dans cai_lecture_canal");
      		goto ERREUR;
   	      }
      	      swab( (void*)(data_image), (void*)(data_image2), taille_image);
	      for (i=0; i<taille_image;i++)
	      {
	   /*stockage dans data_image*/
	         data_image[i] = data_image2[i];
	       }	
              free(data_image2);
          }	     
       }

 }  /* fin TYPE IMAGE SPOT 5 BIL */ 
    /*===========================*/
 

/* Sortie normale */
  return (CAI_OK);

/* Sortie degradee */
ERREUR:
  if (descrip_imagery>0) close(descrip_imagery);
  return(NULL);

}

/*
  Fin de l'operation cai_lecture_canal_spot
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ecriture_canal_spot                                    */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ecriture d'un canal d'une image spot                 */
/* -----    								     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ecriture_canal_spot(image1, canal,      */ 
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
/*     cai_ecriture_canal_spot    (S) (CAI_OK_KO) : = CAI_KO si pb           */
/*                                                     = CAI_OK si OK        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_ecriture_canal_spot (CAI_IMAGE *image1,
					int *canal ,         
					int *premiere_ligne  ,  
					int *nombre_lignes_ecrire ,
					int *nombre_colonnes_ecrire,
					unsigned char *data_image )        

{
  long	cr_write;		/* code retour de write                  */
  unsigned char *buf_lec;	/* Buffer des radiometries               */
  int descrip_imagery;	        /* Descripteur du fichier IMAGERY        */
  int lgrec;
  int i_ligne, j, i;
  CAI_OK_KO icr;
  int nb_can, nboct_pix,nboct,compl;
  int no_oct,nblig,nbcol;
  long deplacement;		/* deplacement a effectuer               */
  long	position_courante;	/* position courante dans le fichier     */

/* Variables intermediaires */

  unsigned char BYTE[4];
  unsigned char COMP[5300];
  int VAR_LONG[2];
  short int TAB_SHORT;
  char TAB[3];

/************************************************/
/* TRAITEMENT DIFFERENT SUIVANT LE TYPE DE SPOT */
/************************************************/
icr = CAI_OK;
if (image1->TYPE_SPOT ==1)
{
/*--------------------*/
/*   IMAGE SPOT1A4    */
/*--------------------*/

  /*...........................
    Initialisations diverses
    ..........................*/
  
   i_ligne = 0;   
   lgrec =  image1->POS_OCTET[*canal-1];  /*long entete image SPOT*/
   descrip_imagery = image1->ACCES_CANAL_INT[*canal-1];
   nb_can = image1->NB_CANAUX;
   nboct_pix = image1->NBOCT_PIX;
   nboct= (*nombre_colonnes_ecrire) * nboct_pix ;
   compl= lgrec-nboct-100;
   for (i=0;i<5300;i++) COMP[i]=0;
   nbcol = image1->NB_COL;
 
/*---------------------------------------------------------------------------*/
/*		ECRITURE DU FICHIER IMAGERY				     */
/*---------------------------------------------------------------------------*/

  /*......................................................*/
  /* Calcul de la position du premier octet a ecrire      */ 
  /*......................................................*/

   no_oct = lgrec+(*premiere_ligne-1)*lgrec*nb_can+lgrec*(*canal-1);
  
  /*......................................................*/
  /* Recuperation de la position courante		  */
  /*......................................................*/

   position_courante = lseek ( descrip_imagery , 0L , 1 );

  /*.......................................................*/
  /* Calcul du deplacement jusqu'a la 1ere ligne a ecrire  */
  /*.......................................................*/

   deplacement = no_oct - position_courante;
  
  /********************************************************/
  /* TANT QUE il y a des lignes a ecrire                  */
  /********************************************************/
   buf_lec=data_image;
   while (i_ligne < (*nombre_lignes_ecrire))
   {
      nblig=(*premiere_ligne)+i_ligne;
 /*.............................................*/
  /*	deplacement jusqu'a la ligne a ecrire   */
  /*............................................*/

      lseek ( descrip_imagery , deplacement , 1 ); 
  
  /******************************/
  /* Pour le fichier IMAGE DATA */
  /******************************/

  /*initialisation des infos de l'entete de l'image dest*/
      BYTE[0] = 237;
      BYTE[1] = 237;
      BYTE[2] = 18;
      BYTE[3] = 18;
      VAR_LONG[1] = lgrec;
      VAR_LONG[0] = i_ligne+1;
   
  /*ecriture dans le fichier image dest*/  
      cr_write = write ( descrip_imagery , VAR_LONG , 4); 
      if (cr_write != 4)
      {
        icr = CAI_KO;
	goto FIN;
      }

      cr_write = write ( descrip_imagery , BYTE , 4);
      if (cr_write != 4)
      {
        icr = CAI_KO;
	goto FIN;
      }

      cr_write = write ( descrip_imagery , &VAR_LONG[1] , 4);
      if (cr_write != 4)
      {
        icr = CAI_KO;
	goto FIN;
      }

   /* Ecriture des prefixes : */

      cr_write = write ( descrip_imagery, &nblig, 4);
      if (cr_write != 4)
      {
        icr = CAI_KO;
	goto FIN;
      }

      strcpy ( TAB,"  ");
      cr_write = write ( descrip_imagery , TAB, 2);
      if (cr_write != 2)	
      {
        icr = CAI_KO;
	goto FIN;
      }

      TAB_SHORT = (short)(*canal);

      cr_write = write ( descrip_imagery , &TAB_SHORT  , 2); 
      if (cr_write !=2)
      {
        icr = CAI_KO;
	goto FIN;
      }

      cr_write = write ( descrip_imagery , COMP  , 12); 
      if (cr_write != 12)
      {
        icr = CAI_KO;
	goto FIN;
      }
 
  /* Ecriture radiometries */

      cr_write = write ( descrip_imagery , buf_lec ,nboct  );
      if (cr_write != nboct)
      {
        icr = CAI_KO;
	goto FIN;
      }

  /* ecriture des octets de completion */
      cr_write = write ( descrip_imagery , COMP ,compl  );
      if (cr_write != compl)
      {
        icr = CAI_KO;
	goto FIN;
      }
  
  /* Ecriture des suffixes : */

      cr_write = write ( descrip_imagery , COMP , 24); 
      if (cr_write != 24)
      {
        icr = CAI_KO;
	goto FIN;
      }

      cr_write = write ( descrip_imagery , &nbcol , 4);
      if (cr_write != 4)
      {
        icr = CAI_KO;
	goto FIN;
      }

      cr_write = write ( descrip_imagery , COMP, 40);
      if (cr_write != 40)
      {
        icr = CAI_KO;
	goto FIN;
      }

  /********************************************************/
  /*	mise a jour du deplacement jusqu'a la ligne	*/
  /*	suivante                                   	*/
  /********************************************************/

      deplacement = ( nb_can - 1 ) * lgrec;
      buf_lec = buf_lec+nboct;

  /********************************************************/
  /*	incrementation du nombre de lignes		*/
  /********************************************************/

       i_ligne++;

     } /*fin while*/
}
else if (image1->TYPE_SPOT ==2)
{
/*--------------------*/
/*   IMAGE SPOT5 TIFF */
/*--------------------*/
     icr = cai_ecriture_canal_tiff (image1,canal,premiere_ligne,  
					nombre_lignes_ecrire ,
					nombre_colonnes_ecrire,data_image);
}
else if (image1->TYPE_SPOT ==3)
{
/*--------------------*/
/*   IMAGE SPOT5 BIL  */
/*--------------------*/
     icr = cai_ecriture_canal_bil(image1,canal,premiere_ligne,  
					nombre_lignes_ecrire ,
					nombre_colonnes_ecrire,data_image);

}  

FIN :
  if (icr != CAI_OK)
	  strcpy(CAI_ERREUR,"Erreur ecriture du fichier Imagery");

  return (icr);

}

/* 
  Fin de l'operation cai_ecriture_canal_spot
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ferme_spot                                             */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de fermeture d'une image au format spot et de          */
/* -----     tous les fichiers qui la composent 			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_OK_KO cai_ferme_spot(typspot,image1)              */  /* ------------------                                                        */
/*									     */
/*     typspot     (E) (char )        : =1 spot1a4, =T spot5Tif,=B spot5Bil  */
/*     image1      (E) (CAI_IMAGE *)  : pointeur sur descripteur de fichier  */
/*									     */
/*     cai_ferme_spot    (S) (CAI_OK_KO) :       = CAI_KO si pb              */
/*                                              = CAI_OK si OK               */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/


CAI_OK_KO  cai_ferme_spot (char typspot,CAI_IMAGE *image1)

{

 CAI_OK_KO iret;           /* code retour de l'operation         */
 int num;                  /* descripteur du fichier a fermer    */
 int temp;                 /* variable temporaire                */
 long pt_oct;		   /* position courante ds fichier image */

/*--------------------------*/
/* Initialisations diverses */
/*--------------------------*/
 iret=CAI_OK;

/************************************************/
/* TRAITEMENT DIFFERENT SUIVANT LE TYPE DE SPOT */
/************************************************/
 if (typspot == '1')
 {
/* SPOT1a4 */

    num=image1->ACCES_CANAL_INT[0];

/*--------------------------------------------------*/
/* Mise a jour de l'entete si ouverture en creation */
/*--------------------------------------------------*/
   if (image1->MODE == CAI_ECRITURE)
   {
  	 pt_oct=lseek(num,0L,0);
 
/*........................................*/
/* Mise a jour du niveau de pretraitement */
/*........................................*/
   	 pt_oct=lseek(num,53L,1); 
   	 pt_oct = write ( num,image1->NIVEAU , 2 );
   	 if ( pt_oct == 0 )
    	 {
      	  sprintf ( CAI_ERREUR ,
       	   "Erreur : MAJ impossible entete a la fermeture fichier SPOT1a4\n");
          iret=CAI_KO;
          goto ERREUR;
    	}

    }
/*.............................................
   Fermeture du fichier qui contient tous les canaux
  .............................................*/
    if (num>0) close(num);

    if (image1->ACCES_SPOT[0]!=0) close(image1->ACCES_SPOT[0]);
    if (image1->ACCES_SPOT[1]!=0) close(image1->ACCES_SPOT[1]);
   }
   else if (typspot == 'T')
   {
/*   SPOT5TIF */
     iret = cai_ferme_tiff(image1);
   }
   else if (typspot == 'B')
   {
/*   SPOT5BIL */
     iret = cai_ferme_bil(image1);
   }



  return (iret);

ERREUR:
  return(CAI_KO);

}

/*
  Fin de l'operation cai_ferme_spot
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_dest_ima_spot                                          */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de destruction d'une image stockee en spot et de       */
/* -----     tous les fichiers qui la composent  			     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_OK_KO cai_dest_ima_spot(typspot,repertoire,nom_ima)*/ /* ------------------                                                        */
/*     typspot     (E) (char )        : =1 spot1a4, =T spot5Tif,=B spot5Bil  */
/*     repertoire   (E) (char *) : repertoire de l'image a detruire          */
/*     nom_image    (E) (char *) : nom de l'image a detruire avec repertoire */
/*									     */
/*     cai_dest_ima_spot (S) (CAI_OK_KO) : = CAI_KO si pb                    */
/*                                            = CAI_OK si OK                 */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_OK_KO cai_dest_ima_spot (char typspot, char *repertoire , 
                              char *nom_ima )

{
char	nom_fic[1024];	/* nom du repertoire a detruire */

/************************************************/
/* Constitution du nom du repertoire image      */
/************************************************/

if ((long)strlen(repertoire)!= 0)
{
  sprintf( nom_fic, "%s/%s" ,repertoire,nom_ima);
}
else
{
  strcpy ( nom_fic , nom_ima);
}

/*****************************************************/
/* Destruction du repertoire et de tous ses fichiers */
/*****************************************************/

     sprintf(Command,"rm -R %s",nom_fic);
     system(Command);

return (CAI_OK);

}
/*
 Fin de l'operation cai_dest_ima_spot
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_modifie_spot                                     */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction d'ouverture en lecture-ecriture d'une image au format  */
/* -----     spot et de tous les fichiers qui la composent                   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL :  CAI_IMAGE *cai_ouvre_modifie_spot(typspot,nom_image,  */
/*  ---------------- 		                            nb_can,          */
/*				                            nboct_pix,	     */	
/*				                            nb_col,          */
/*				                            nb_lig)          */
/*                                                                           */
/*     typspot        (E) (char)   : type spot (1=1a4,b=5bil ou t=5tif)      */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir avec repertoire */
/*									     */
/*     nb_can         (S) (int *)  : nombre de canaux de l'image             */
/*     nboct_pix      (S) (int *)  : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (S) (int *)  : nombre de lignes de l'image             */
/*     nb_col         (S) (int *)  : nombre de colonnes de l'image           */
/*                                                                           */
/*     cai_ouvre_modifie_spot (S) (CAI_IMAGE *)  : = NULL si pb              */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */                                                                            /*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/

CAI_IMAGE *cai_ouvre_modifie_spot(char typspot,char *repert,
				  char *nom_image,
				  int  *nb_can,
				  int  *nboct_pix,
				  int  *nb_col,
				  int  *nb_lig)

{
DIR *dirp;		    /* pointeur sur structure ouverture repertoire   */
CAI_IMAGE *image1;          /* pointeur su descripteur de fichier image      */ 
struct dirent *dp;          /* structure resultat de la fonction readdir     */
char fichier_clefs[1024];   /* Nom du fichier des donnees auxiliaires        */
int i,j,k;		    /* Variables de boucle                           */
int descrip_leader;	    /* Descripteur du fichier LEADER                 */
int descrip_imagery;	    /* Descripteur du fichier IMAGERY                */
int descrip_volume;	    /* Descripteur du fichier VOLUME                 */
long cr_lseek;		    /* Code retour de lseek                          */
long cr_read;		    /* Code retour de read                           */
long lg_saut;		    /* Nombre d'octets a sauter                      */
int  lg_enreg,lgtemp;	    /* longueur d'un enregistrement                  */
int NbCan;                  /* nombre de canaux de l'image                   */
int test1,test2,test3,test4,test5;/* test sur nom de fichiers                */
int lg_entete;	            /* longueur de l'entete fichier IMAGERY          */
int num,trouve;		    /* variable de test                              */
char nom_fic[1024];         /* nom du fichier en-tete		             */
char nom_fic_canal[1024];   /* nom fichier canal dont on teste existence     */
char nom_rep[1024];         /* nom du repertoire image SPOT		     */
char nom_fic_ima[1024];     /* nom du fichier imagery.dat	             */
char nom_leader[1024];      /* nom du fichier leader		             */
char path_rep[PATH_MAX];    /* chemin acces fichiers ds repertoire courant   */
char nom_fic_spot5[PATH_MAX];/* chemin acces fichier image tiff ou BIL SPOT5 */
char nom_ima_spot5[PATH_MAX];/* nom du fichier image spot5                   */
char *pch0;                  /* pointeur sur chaine de caracteres            */
char car[2];                 /* variable de travail                          */
int type_spot;               /* type d'image spot                            */
char byteorder[1];           /* Ordre de codage intel (LE) ou Motorola (BE)  */
int skipbytes;               /* Taille entete pour les fichiers BIL SPOT5    */
CAI_OK_KO icr;
int Trouve;


/* Donnees auxiliaires */

char TABLEAU[1][257];	/* Variable intermediaire */

/* premiere variable = pour le nombre d'elements = nbre de colonnes */
/* deuxieme element = pour la taille de l'element */


/*........................................
   INITIALISATION et allocation de place pour la structure CAI_IMAGE
  .........................................*/
*nb_can=0;
*nb_lig=0;
*nb_col=0;
*nboct_pix=1;
 type_spot=0;
 trouve=0;
 car[0]='/';

#ifdef _WIN
      car[0]='\\';
#endif

/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : initialisation de image1=NULL a l'ouverture    */
/*****************************************************************************/
   image1= NULL;   

 /*..................................
   determination ordre de rangement 
   des octets sur machine de travail
  ......................................*/   
   
  type_machine = cai_test_pFpf();

/******************************************************************************/
/*	Determination du type d'image : SPOT 1 a 4 ou SPOT 5                  */
/******************************************************************************/
sprintf(nom_rep, "%s%s%s", repert,car, nom_image);

dirp=opendir(nom_rep);

 
if(dirp!=NULL)
 {
  while( ((dp=readdir(dirp))!=NULL )&& (trouve ==0))
  {
   
  /*---------------------------------------------*/
  /*  on ne doit pas tenir compte des . et ..    */
  /*        sous le repertoire courant           */
  /*---------------------------------------------*/
    
   test1 = strcmp(dp->d_name, ".");
   test2 = strcmp(dp->d_name, "..");
	
   if ( (test1 != 0) && (test2 != 0) )
{
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Blindage de la reconnaissance du fichier image */
/*****************************************************************************/
     
  /*---------------------------------------------*/
  /*  on recherche le fichier Image              */
  /*---------------------------------------------*/
     test3 = strncasecmp(dp->d_name,"IMAG", 4);
     if (test3 == 0)
     {
        test4 = strcasecmp(dp->d_name,"imagery.tif");
        if (test4==0)  /* format SPOT5TIF */
        {
                type_spot = 2;
		sprintf (nom_fic_spot5, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
		sprintf (nom_ima_spot5, "%s", dp->d_name);
		trouve=1;
        }
        else 
        {
	    test4 = strcasecmp(dp->d_name,"imagery.bil");
            if (test4==0)  /* format SPOT5BIL */
            {
                type_spot = 3;
		sprintf (nom_fic_spot5, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
		sprintf (nom_ima_spot5, "%s", dp->d_name);
		trouve=1;
             }
	     else /* format SPOT 1 a 4*/
             {
		car[0] = '.';
                pch0 = strchr (dp->d_name, car[0]);
		if  (pch0 == NULL)
                {
			type_spot = 1;     /* format SPOT 1 a 4*/
     		        sprintf(nom_fic_ima, "%s/%s/%s", repert, 
						nom_image, dp->d_name);
			trouve=1;
                }
                else if (strcasecmp(pch0,".DAT")== 0)
                {
			type_spot = 1;     /* format SPOT 1 a 4*/
     		        sprintf(nom_fic_ima, "%s%s%s%s%s", repert, car,
						nom_image, car,dp->d_name);
			trouve=1;
                }

             }		
        }
     }
    }
  }
  closedir(dirp); 
 }


if (type_spot == 0)
{
	sprintf (CAI_ERREUR,"Erreur : type d'image SPOT inconnu");
	goto ERREUR;
}
/*........................................................................*/
/* Verication ed la coherence du type spot annonce et du type spot trouve */
/*........................................................................*/
	if (((typspot == '1')&&(type_spot != 1))||
		((typspot == 'T')&&(type_spot != 2))||
		((typspot == 'B')&&(type_spot != 3)))
	{
	    sprintf (CAI_ERREUR,
			"Erreur : L'image n'est pas du type SPOT annonce");
	    goto ERREUR;
	}

/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 1 A 4 */
/*=============================================*/
if (type_spot == 1)
{
   image1= NULL;    
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   image1->ACCES_SPOT[0]=0;
   image1->ACCES_SPOT[1]=0;

/*****************************************************************************/
/* tests sur les retours d'allocation  memoire                               */
/*****************************************************************************/

   if (image1==NULL)
   {
       strcpy(CAI_ERREUR,
	"Erreur allocation memoire dans cai_ouvre_modifie_image spot1a4");
       goto ERREUR;
    }

/******************************************************************************/
/*	Ouverture des fichiers LEADER et IMAGERY en lecture		      */
/******************************************************************************/

    sprintf(nom_rep, "%s/%s", repert, nom_image);

    dirp=opendir(nom_rep);
 
    if(dirp!=NULL)
    {
      while( (dp=readdir(dirp))!=NULL )
      {
   
  /*---------------------------------------------*/
  /*  on ne doit pas tenir compte des . et ..    */
  /*        sous le repertoire courant           */
  /*---------------------------------------------*/
    
        test1 = strcmp(dp->d_name, ".");
        test2 = strcmp(dp->d_name, "..");
	
        if ( (test1 != 0) && (test2 != 0) )
        {
     
     	   test3 = strncasecmp(dp->d_name,"LEAD", 4);
     	   test4 = strncasecmp(dp->d_name,"IMAG", 4); 
     	   test5 = strncasecmp(dp->d_name,"VOL", 3);   
    
     /********************************************************/
     /* tests de determination du fichier LEADER sous repert */
     /********************************************************/
     
           if (test3==0)
           {
    		 sprintf(path_rep, "%s%s%s%s%s", repert, car,nom_image, 
								car,dp->d_name);      
      /*ouverture du fichier LEADER*/
            
      		descrip_leader = open (path_rep, O_RDWR|O_BINARY );
      		if (descrip_leader == -1)
      		{
      		  strcpy(CAI_ERREUR,
				"Erreur d'ouverture du fichier LEADER SPOT1a4");
       		  goto ERREUR;
      		}
            
        /*---------------------------------------------------------*/
	/* Lecture de la taille d'un enregistrement dans LEADER    */
	/*---------------------------------------------------------*/

		cr_lseek = lseek ( descrip_leader , 8L , 1 );
		if ( cr_lseek == -1 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}

		cr_read = read ( descrip_leader , &lg_enreg , 4 );
		if ( cr_read == -1 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}
	        if (strcmp(type_machine,"LE")==0)
                {
		   swab( (void*)&lg_enreg, (void*)&lgtemp,4 );
		   cai_inverser_tableau((short *)&lg_enreg, (short *)&lgtemp,2);
                }
	
	/*------------------------------------------------------*/
	/* Retour au debut du fichier LEADER		        */
	/*------------------------------------------------------*/

		cr_lseek = lseek ( descrip_leader , 0L , 0 );
		if ( cr_lseek == -1 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}

        /*------------------------------------------------------*/
	/* Saut de lg_enreg + 996 octets dans LEADER		*/
	/*------------------------------------------------------*/

		lg_saut = (long)(lg_enreg + 996);
		cr_lseek = lseek ( descrip_leader , lg_saut , 0 );
		if ( cr_lseek == -1 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}
	/*------------------------------------------------------*/
	/* Lecture du nombre de colonnes dans LEADER	        */
	/*------------------------------------------------------*/

  		cr_read = read (descrip_leader, TABLEAU[0], 16);
		if ( cr_read != 16 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}
        	TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
        /* On met un masque sur le 8ieme bit de chaque octet : */

  		for (i=0;i<16;i++)
  		{
			TABLEAU[0][i] = TABLEAU[0][i] & 0177;
  		}

 		sscanf ( TABLEAU[0], "%d", nb_col);

	/*------------------------------------------------------*/
	/* Lecture du nombre de lignes dans LEADER		*/
	/*------------------------------------------------------*/

  		cr_read = read (descrip_leader, TABLEAU[0], 16);
		if ( cr_read != 16 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}
       		TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
        /* On met un masque sur le 8ieme bit de chaque octet : */

  		for (i=0;i<16;i++)
  		{
			TABLEAU[0][i] = TABLEAU[0][i] & 0177;
  		}

 		sscanf ( TABLEAU[0], "%d", nb_lig);

	/*------------------------------------------------------*/
	/* Saut de 16 octets dans LEADER			*/
	/*------------------------------------------------------*/

		cr_lseek = lseek ( descrip_leader , 16L , 1 );
		if ( cr_lseek == -1 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}
	
	/*------------------------------------------------------*/
	/* Lecture du nombre de canaux	dans LEADER		*/
	/*------------------------------------------------------*/

 		cr_read = read (descrip_leader, TABLEAU[0], 16);
		if ( cr_read != 16 )
		{
		  strcpy(CAI_ERREUR,"Erreur acces au fichier leader SPOT1a4");
		  goto ERREUR;
		}
        	TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
        /* On met un masque sur le 8ieme bit de chaque octet : */

  		for (i=0;i<16;i++)
  		{
			TABLEAU[0][i] = TABLEAU[0][i] & 0177;
  		}

 		sscanf ( TABLEAU[0], "%d", nb_can);	
   	   } /*fin test3 (v==0)*/
     
          else  if (test5==0)
          {
    /********************************************************/
    /* tests de determination du fichier VOLUME sous repert */
    /********************************************************/

     		sprintf(path_rep, "%s%s%s%s%s", repert, car,nom_image, 
							car,dp->d_name); 
      
      	/*ouverture du fichier VOLUME*/
            
      		descrip_volume = open (path_rep, O_RDWR|O_BINARY );
      		if (descrip_volume == -1)
      		{
       		   strcpy(CAI_ERREUR,
			"Erreur d'ouverture du fichier VOLUME SPOT1a4");
       		   goto ERREUR;
                 }
           }
    /********************************************************/
    /* tests de determination du fichier IMAGERY sous repert */
    /********************************************************/
    	       
           else if (test4==0)
           {   
       
      /*ouverture du fichier IMAGERY*/
      
      		descrip_imagery = open (nom_fic_ima, O_RDWR |O_BINARY);
      		if (descrip_imagery == -1)
      		{
      		  strcpy(CAI_ERREUR,
			"Erreur d'ouverture du fichier IMAGERY SPOT1a4");
       		  goto ERREUR;
      		}
                  
      /*------------------------------------------------------*/
      /* Lecture de la taille de l'entete dans IMAGERY	      */
      /*------------------------------------------------------*/

  		cr_lseek = lseek ( descrip_imagery , 8L , 1);
  		if ( cr_lseek == -1 )
  		{
		  strcpy(CAI_ERREUR,
			"Erreur acces au fichier contenant donnees SPOT1a4");
		  goto ERREUR;
  		}	

  		cr_read = read ( descrip_imagery , &lg_entete , 4 );
  		if ( cr_read == -1 )
  		{
			strcpy(CAI_ERREUR,
				"Erreur acces au fichier contenant donnees SPOT");
			goto ERREUR;
  		}
	        if (strcmp(type_machine,"LE")==0)
                {
		   swab( (void*)&lg_entete, (void*)&lgtemp,4 );
		   cai_inverser_tableau((short *)&lg_entete, 
				(short *)&lgtemp,2);
                }
      
    	  } /*fin else if test4*/
    	       
        } /*fin else test1 test2*/
   
     }  /*fin while*/
                   
     closedir(dirp); 
 
   }  /*fin if (dirp=!NULL)*/

/*.......................................
     INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

  for (i=0;i<*nb_can;i=i+1)
  {
    image1->ACCES_CANAL_INT[i]=descrip_imagery;
    image1->ACCES_SPOT[0]=descrip_leader;
    image1->ACCES_SPOT[1]=descrip_volume;
    image1->POS_OCTET[i]=lg_entete;	   
  }
    image1->TYPE_SPOT = type_spot;
    image1->NBOCT_PIX = *nboct_pix;
      
 }  /* fin TYPE IMAGE SPOT 1 a 4 */ 
    /*===========================*/
 
/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 5 TIF */
/*=============================================*/
 else if (type_spot == 2)
 {

/* Ouverture de l'image */
    image1  = cai_ouvre_modifie_image (nom_fic_spot5, "TIFF", 
                                        nb_can,nboct_pix,nb_col,nb_lig);

  if (image1 == NULL)
  {
    strcpy(CAI_ERREUR,
	"Erreur acces au fichier image TIFF contenant donnees SPOT5TIF");
	goto ERREUR;
  }

   image1->NB_CANAUX = *nb_can  ;
   image1->NBOCT_PIX = *nboct_pix  ;
   image1->NB_COL =  *nb_col  ;
   image1->NB_LIG = *nb_lig ;
   image1->TYPE_SPOT = type_spot;
   image1->ACCES_SPOT[0]=0;
   image1->ACCES_SPOT[1]=0;


 }  /* fin TYPE IMAGE SPOT 5 TIFF */ 
    /*============================*/

/*=============================================*/
/* TRAITEMENT D'UNE IMAGE AU FORMAT SPOT 5 BIL */
/*=============================================*/
 else
 {

   image1= NULL;    
   image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));
   image1->ACCES_SPOT[0]=0;
   image1->ACCES_SPOT[1]=0;
  
/*****************************************************************************/
/* tests sur les retours d'allocation  memoire                               */
/*****************************************************************************/

   if (image1==NULL)
   {
       strcpy(CAI_ERREUR,
	"Erreur allocation memoire dans cai_ouvre_modifie_image SPOT5TIF");
       goto ERREUR;
    }

   /* Recherche du leader Dimap */
     dirp=opendir(nom_rep);
     Trouve=0;
     if(dirp!=NULL)
     {
        while( (dp=readdir(dirp))!=NULL )
        {
   
  /*---------------------------------------------*/
  /*  on ne doit pas tenir compte des . et ..    */
  /*        sous le repertoire courant           */
  /*---------------------------------------------*/
    
   	   test1 = strcmp(dp->d_name, ".");
   	   test2 = strcmp(dp->d_name, "..");
	
  	   if ( (test1 != 0) && (test2 != 0) )
    	   {
     
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/     
     	     test3 = strncasecmp(dp->d_name,"METADATA.DIM", 12);
    
     /**************************************************************/
     /* tests de determination du fichier METADATA.DIM sous repert */
     /**************************************************************/
     	     if (test3==0)
     	     {
      	          sprintf(nom_leader, "%s%s%s%s%s", repert,car, nom_image, 
							   car, dp->d_name);      

/* Extraction du nombre de lig col can et nboct du fichier DIMAP */
/*...............................................................*/
    		   icr = cai_lire_dimensions_spot5(nom_leader,nb_can,nboct_pix ,
                               			   nb_col , nb_lig,byteorder,
						   &skipbytes );
	           if (icr != CAI_OK)
                   {
			goto ERREUR;
                   }
		   else
			Trouve=1;
             }
           } /*fin else test1 test2*/
   
         }  /*fin while*/
                   
         closedir(dirp); 
 
      }  /*fin if (dirp=!NULL)*/

      if (Trouve==0)
      {
	  sprintf(CAI_ERREUR,
	  "Erreur Le fichier METADATA.DIM n'existe pas ou erreur de lecture");
	  goto ERREUR;
       }


/*   INTIALISATION DES TABLEAUX CONTENANT LE NOM DE FICHIER
      ET LE NOM UTILISATEUR DE CHAQUE CANAL
  .....................................................*/

     nom_fic_canal[0]='\0';

     sprintf(nom_fic_canal,"%s",nom_fic_spot5);

     num=open(nom_fic_canal,O_RDWR|O_BINARY,0);
     if (num!= -1)
     {
       for (i=0;i<*nb_can;i=i+1)
       {
    		image1->ACCES_CANAL_INT[i]=num;
           	image1->POS_OCTET[i]=(*nb_col)*i*(*nboct_pix)+skipbytes;
        }
        image1->TYPE_SPOT = type_spot;
        image1->NBOCT_PIX = *nboct_pix;
        if (byteorder[0]=='M')
        {
	   if (strcmp(type_machine,"BE")==0)
		image1->INV_OCTETS=0;
	   else
		image1->INV_OCTETS=1;
        }
        else
        {
	   if (strcmp(type_machine,"LE")==0)
		image1->INV_OCTETS=0;
	   else
		image1->INV_OCTETS=1;
        }

      }
      else
      {
  	strcpy(CAI_ERREUR,
		"Erreur ouverture fichier BIL contenant donnees SPOT5");
	goto ERREUR;
      }

 }  /* fin TYPE IMAGE SPOT 5 BIL */ 
    /*===========================*/

 return(image1);


ERREUR:
  if (type_spot==1)
  {
     if (descrip_leader >0) close(descrip_leader);
     if (descrip_imagery >0) close(descrip_imagery);
     if (descrip_volume >0) close(descrip_volume);
  } 
  else 
     icr = cai_ferme_spot(typspot,image1);

  if (image1!=NULL) free(image1);
  return(NULL);
}

/*
  Fin de l'operation cai_ouvre_modifie_spot
*/
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_lead4to5spot                                               */
/*                                                                            */
/*ROLE : Cette operation genère un fichier XML de donnees auxiliaires SPOT5   */
/*       a partir du fichier Leader donne en parametre                        */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/*RETOUR : CAI_OK si tout va bien , CAI_KO sinon                              */
/*                                                                            */
/******************************************************************************/

/* MOD : VERSION : 4.5 : FA : erreur d'ecriture detector id lookangle pour PAN*/

CAI_OK_KO cai_lead4to5spot ( CAI_IMAGE *desc_im  , char *repertoire )
{
  CAI_OK_KO cr;
  /* Variables destinees aux noms des fichiers manipules */
  
  FILE *des;
  char chemin_descripteur[1024] ;
  /* Variables destinees aux controles de la lecture des fichiers */
  /*  int desc_lead;*/
  
  int BadFlag[4],Bad,type;
  int desc_lead , desc_vol , desc_img;  
  int detect_id;
  int lg_enreg ;  /* longueur de l'enregistrement et retour de read */
  long cr_lseek , lg_saut;  /* retour de lseek et saut demande                */
  long cr_read;

  int i , x , j , ind_gains;/* indices de boucle                              */
  int can;
  char TABLEAU[4][257];   /* variable destinee a la lecture des octets      */

  /* Variables stockant les donnees auxiliaires lues */
  
  int nb_col , nb_lig;        /* nbre de lignes et de colonnes       */
  int K , J , nb_bs;          /* valeurs K,J,decalage et nbre canaux */
  int ind_max;
  int anneecal , moiscal , jourcal;

  char scene_id[17]   ;         /* identifiant de scene                */
  char grille_ref[8]  ;        /* grille de reference K J             */
  char shift_val[2]   ;         /* shift value                         */
  char latc[17] , lonc[17] ;  /* lat lon au centre de scene          */
  
  char latse[17] , latso[17]; /* lat lon aux 4 coins                 */
  char latne[17] , latno[17]; /*                                     */
  char lonse[17] , lonso[17]; /*                                     */
  char lonne[17] , lonno[17]; /*                                     */
  char al[17] , bl[17] , cl[17];
  char ap[17] , bp[17] , cp[17];

  double lat_c  ,lon_c;       /* valeurs en double des latitudes et  */
  double lat_se, lat_so;     /* longitudes au centre de scene et    */
  double lon_se , lon_so;     /* 4 coins de scene                    */
  double lat_ne , lat_no;
  double lon_ne , lon_no;
  double lat_nad , long_nad;
  double yawdeg , yawrad;  
  
  char colc1[17] , ligc1[17]; /* Lignes et colonnes aux 4 coins       */
  char colc2[17] , ligc2[17];
  char colc3[17] , ligc3[17];
  char colc4[17] , ligc4[17];
  int nbl_c , nbc_c; /* x y ligne et colonne centre de scene */
  int ivaleur;
  char Valeur[17];

  char decal_lat[17];       /* Decalage latitude          */
  char decal_lon[17];       /* Decalage longitude         */
  char longnad[17];         /* latitude nadir             */
  char latnad[17];          /* longitude nadir            */
  char datecalib[11];
  char datecentre[17];
  char datecal[9];
  char relativedatecalib[16][20];
  int relativeannee  , relativemois  , relativejour ;
  char mapproj[33] ;         /* Projection cartographique  */
  char gains_a[4][9];       /* coefficient de calibration */
  char gains_b[4][9];       /* offset de calibration      */
  char nb_gains[4][5];      /* nombre de gains            */
  char date[11] ;             /* date  d'acquisition        */
  char stepnum[5] ;
  char heure[10] ;           /* heure d'acquisition        */
  char coupled[3]  , sec[3] ;     /* minute , seconde           */
  char niv_trait[17] ;       /* niveau de traitement       */
  char direct_playback[17] ;
  char compression[17] ;
  char instrument[6] ;      /* instrument HRV             */
  char time_angle[73][27];
  char timeyaw[3][27];
  float gain_value[16][1500];
  float dark_value[16][1500];
  float anglin ;
  short int val,valtemp ;
  float val_float ;
  int type_gain , ind_gain_val  , ind_dark_val ;
  int  instrument_index  , ind_tab_gain  ;
  int line[73] , line_raw[2] ;
  char yaw[73][20] ,pitch[73][20] ,roll[73][20] ;
  char yaw_angle[2][20] , pitch_angle[2][20] , roll_angle[2][20];
  double psi_x_first[3] ,psi_y_first[3] ,psi_x_last[3] ,psi_y_last[3];
  double psideg  , periodeligne ;
  char lineperiod[13] , outofrange[3] ;
  char ins_ind[2] ;
  char sat_ind[2] ;
  char mode_spect[17];      /* mode spectral              */
  char satellite[5];       /* nom du satellite           */
  int satellite_index ;
  char revolution[17];      /* revolution number          */
  char band_layout[17];
  char spect_ind[17];       /* indicateur de bandes       */
  char sat_alt[9];          /* altitude du satellite      */
  char nb_leader[9];        /* nb enregistrements leader  */
  char lg_leader[9] ;        /* lg enregistrements leader  */
  char nb_imagery[9] ;       /* nb enregistrements imagery */
  char lg_imagery[9] ;       /* lg enregistrements imagery */
  char nbima_imagery[9] ;    /* nb enregist imagery image  */
  char nb_trailer[9] ;       /* nb enregistrements trailer */
  char lg_trailer[9] ;       /* lg enregistrements trailer */
  char fpixraw[9] , flineraw[9] , fpix1b[9] , fline1b[9];
  char taille_pixlig[17];   /* taille pixel sur les lignes   */
  char taille_pixcol[17];   /* taille pixel sur les colonnes */
  char x0[33] , y0[33];     /* coordonnees en pixel 1,1      */
  char lgx[17] , lgy [17];  /* longueur image en X et Y      */
  char im[17] , deltai[17] , jm[17] ,deltaj[17];
  char lm[17] , deltal[17] , pm[17] , deltap[17];
  char valeur[100];
  char prod_info[31];
  char mean[9];
  char lowtresh[4][9];
  char uptresh[4][9];
  char firstwave[4][20];
  double unwave ;
  double wavelong ;
  char wavelengthstep[4][20] , solar_irradiance[4][5];
  char spectral_sensitivitie[4][64][6];
  char shval[9] , segmentstart[9] , scenestart[9] , segmentend[9];
  char resampling[3];
  int utdatej , utdates;
  char p7[17] ,p8[17] ,p9[17] ,p10[17] ;
  char axis[13] , ascension[13] , latarg[13];
  char ex[17] , ey[17] , inclin[13];
  char coeffi[17] , ai[17] , bi[17] , ci[17] , di[17] , ei[17];
  char coeffj[17] , aj[17] , bj[17] , cj[17];
  char coeff_a[17] , aa[17] , ba[17] , ca[17] , da[17];
  char coeff_b[17] , ab[17] , bb[17] , cb[17] , db[17] , eb[17];
  char coeff_l[17] , a_l[17] , b_l[17] , d_l[17] , e_l[17] , c_l[17];
  char coeff_p[17] , a_p[17] , b_p[17] , c_p[17];
  char alswir[17] , blswir[17] , clswir[17] , apswir[17] , bpswir[17];
  char cpswir[17],car[2];
  int jourjul;
  int datejul;
  char clockvalue[13] ;
  char clockperiod[13] ;
  double period  , pico ;
  char boardtime[13];
  double X_loc[9] , Y_loc[9] , Z_loc[9];
  double X_vel[9] , Y_vel[9] , Z_vel[9];
  char time[9][28];
  char dorisused[3];
  char time_scenecenter[31];
  char timecenter[18];
  char angle_in[17] , angle_o[17]; /* angles incidence et orientation Nord */
  char sunazim[17], sunelev[17];   /* azimut et elevation solaire          */
  char *profile;

  int signe , swir ;                /* signe des lat lon ( +1 ou -1 )       */
  char mirror_step[17] ;     /* nombre de pas mirroir                */
  int nbpix ;                /* nombre de pixels */
  char satn , insn;
  char insid, codecapt;
  char a0 , a1 , m0 , m1 , j0 , j1 , h0 , h1 , mi0 , mi1 , s0 , s1 ;
  char aprim0 , aprim1 , mprim0 , mprim1 , jprim0 , jprim1 , hprim0 , hprim1 , miprim0 , miprim1 , sprim0 , sprim1 ;
  char datasetname[40];
  char datasetname_centre[40];
  char dsetname[25];
  char dsetname_centre[25];
  char toto[32] , coeffa[17] , coeffb[17], coeffc[17] , coeffd[17];
  char coeffe[17] , coefff[17] , coeffap[17] , coeffbp[17] ;
  char coeffcp[17] , coeffdp[17] , coeffep[17] , coefffp[17];
  char rlma[17] , rlmb[17], rlmc[17] , rlmd[17];
  char rlme[17] , rlmf[17] , rlmap[17] , rlmbp[17] ;
  char rlmcp[17] , rlmdp[17] , rlmep[17] , rlmfp[17];
  long jours[9];
  long jourscenter ;
  int nb_passage = 0;
  double secondescenter ;
  double secondes[9];
  double milisecondes[9];
  
  /* variables destinees a lire dans le voldir */

  char job_id[17] , produser[13] , product_type[17];
  char software_name[13] , processing_center[9];
  int product_sec , product_min , product_heure;
  int product_annee , product_jour , product_mois;
  char copyright[100]  , source_description[200];

  /* variables destinees a lire dans le imagery */

  unsigned char bad_line[10000][4];

/* Initialisations diverses */
/*--------------------------*/
  for (i=0 ; i<16 ; i++)
  {
     for (j=0 ; j<1500 ; j++)
     {
	gain_value[i][j] = 0.0;
	dark_value[i][j] = 0.0;
     }
  }
  signe=1;
  desc_lead = desc_im->ACCES_SPOT[0];
  desc_vol = desc_im->ACCES_SPOT[1];
  desc_img = desc_im->ACCES_CANAL_INT[0];

      car[0]='/';

#ifdef _WIN
      car[0]='\\';
#endif


  /* construction du chemin complet du fichier xml a generer */
  sprintf ( chemin_descripteur , "%s%sSPOT_DIMAP.xml" , repertoire, car);
  
  /********************************************************/
  /*             LECTURE DU FICHIER LEADER                */
  /********************************************************/
 
  cr_lseek = lseek ( desc_lead , 8L , 1 );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  
  /*------------------------------------------------------*/
  /*   lecture de la longueur de l'enregistrement 	  */
  /*------------------------------------------------------*/
  lg_enreg = 0;
  cr_read = read ( desc_lead , &lg_enreg , 4 );
  
  if ( cr_read == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_read );
    goto ERREUR;
    }
  /*------------------------------------------------------*/
  /* Retour au debut du fichier		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 0L , 0 );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Saut de 52 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 52 , SEEK_SET );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /********************************************************/
  /*           LEADER FILE DESCRIPTOR RECORD              */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Lecture du code capteur             		  */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 13 );
  
  if ( cr_read != 13 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  TABLEAU[0][13] = '\0';
  
  for ( i = 0 ; i < 13 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  codecapt = '\0';
  toto[0] ='\0';
  sscanf ( TABLEAU[0] , "%c%s" , &codecapt , toto );

  /*------------------------------------------------------*/
  /* Retour au debut du fichier		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 0L , 0 );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Saut de lg_enreg + 20 octets 			  */
  /*------------------------------------------------------*/

  lg_saut = (long)( 3960 + 20);
  
  cr_lseek = lseek ( desc_lead , lg_saut , SEEK_SET );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /********************************************************/
  /*                LEADER HEADER RECORD                  */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Lecture des coeeficients K et J  			  */
  /*------------------------------------------------------*/
  
  for ( x = 0 ; x < 2 ; x++ )
    {
    cr_read = read ( desc_lead, TABLEAU[x], 3 );
    if ( cr_read != 3 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    TABLEAU[x][3] = '\0';
    
    for ( i = 0 ; i < 3 ; i++ )
      {
      TABLEAU[x][i] = TABLEAU[x][i] & 0177;
      }
    }
  K=0;
  J=0;
  sscanf ( TABLEAU[0], "%d", &K );
  sscanf ( TABLEAU[1], "%d", &J );
  sprintf ( grille_ref , "%.3d%.3d" , K , J );

  /*------------------------------------------------------*/
  /* Lecture de shift value        			  */
  /*------------------------------------------------------*/
  
  /* sauter le slash */
  
  cr_lseek = lseek ( desc_lead , 1 , SEEK_CUR );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  
  cr_read = read ( desc_lead , TABLEAU[0] , 1 );

  if ( cr_read != 1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  TABLEAU[0][1] = '\0';
    
  for ( i = 0 ; i < 1 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  shift_val[0]='\0';
  sscanf ( TABLEAU[0] , "%s" , shift_val );

  /*------------------------------------------------------*/
  /* Saut de 8 octets 			                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 8 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Lecture de l'identifiant de scene    		  */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
  
  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  TABLEAU[0][16] = '\0';
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  
  sprintf ( scene_id , "\0" );
  sscanf ( TABLEAU[0] , "%s" , scene_id );

/****************************************************************************/
/* MOD : VERSION : 4.7 : FA :  Prise en compte date de la scene non shiftée */
/****************************************************************************/  strcpy ( datecentre , scene_id );

  /*------------------------------------------------------*/
  /* Lecture de shift along track ( decal_lat )		  */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16 );
  
  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  TABLEAU[0][16] = '\0';
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( decal_lat , "\0" );
  sscanf ( TABLEAU[0] , "%s" , decal_lat );

  /*------------------------------------------------------*/
  /* Lecture de shift along track ( decal_lon )		  */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16 );
  
  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
  TABLEAU[0][16] = '\0';
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( decal_lon, "\0" );
  sscanf ( TABLEAU[0] , "%s" , decal_lon );

  /*------------------------------------------------------*/
  /* Lecture de la latitude du centre de la scene         */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( latc , "\0" );
  lat_c=0;
  sscanf ( TABLEAU[0] , "%s" , latc );
  cai_DMS_To_Float ( latc , &lat_c );

  /*------------------------------------------------------*/
  /* Lecture de la longitude du centre de la scene        */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( lonc , "\0" );
  lon_c=0;
  sscanf ( TABLEAU[0] , "%s" , lonc );
  cai_DMS_To_Float ( lonc , &lon_c );
  
  /*------------------------------------------------------*/
  /* lecture des coordonnees lignes au centre de scene    */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  nbc_c = 0;
  sscanf ( TABLEAU[0] , "%d" , &nbc_c );
  
  /*------------------------------------------------------*/
  /* lecture des coordonnees colonnes au centre de scene  */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead , TABLEAU[0] , 16 );

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  nbl_c = 0;
  sscanf ( TABLEAU[0] , "%d" , &nbl_c );
  /*------------------------------------------------------*/
  /* Lecture de la latitude NO                            */  
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( latno , "\0" );
  
  lat_no=0;
  
  sscanf ( TABLEAU[0] , "%s" , latno );
  cai_DMS_To_Float ( latno , &lat_no );

  /*------------------------------------------------------*/
  /* Lecture de la longitude NO                           */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( lonno , "\0" );
  lon_no=0;
  sscanf ( TABLEAU[0] , "%s" , lonno );
  cai_DMS_To_Float ( lonno , &lon_no );
  
  /*------------------------------------------------------*/
  /* Lecture ligne coin NO                                */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ligc1 , "\0" );
  sscanf ( TABLEAU[0] , "%s" ,ligc1  );

  /*------------------------------------------------------*/
  /* Lecture colonne coin NO                              */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( colc1 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , colc1 );

  /*------------------------------------------------------*/
  /* Lecture de la latitude NE                            */  
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( latne , "\0" );
  sscanf ( TABLEAU[0] , "%s" , latne );
  cai_DMS_To_Float ( latne , &lat_ne );
  
  /*------------------------------------------------------*/
  /* Lecture de la longitude NE                           */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( lonne , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lonne );
  cai_DMS_To_Float ( lonne , &lon_ne );
  
  /*------------------------------------------------------*/
  /* Lecture ligne coin NE                                */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( ligc2 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ligc2 );

  /*------------------------------------------------------*/
  /* Lecture colonne coin NE                              */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( colc2 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , colc2 );

  /*------------------------------------------------------*/
  /* Lecture de la latitude SO                            */  
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( latso , "\0" );
  sscanf ( TABLEAU[0] , "%s" , latso );
  cai_DMS_To_Float ( latso , &lat_so );

  /*------------------------------------------------------*/
  /* Lecture de la longitude SO                           */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';  
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( lonso , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lonso );
  cai_DMS_To_Float ( lonso , &lon_so );
  
  /*------------------------------------------------------*/
  /* Lecture ligne coin SO                                */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ligc3 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ligc3 );

  /*------------------------------------------------------*/
  /* Lecture colonne coin SO                              */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( colc3 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , colc3 );

  /*------------------------------------------------------*/
  /* Lecture de la latitude SE                            */  
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( latse , "\0" );
  sscanf ( TABLEAU[0] , "%s" , latse );
  cai_DMS_To_Float ( latse , &lat_se );
  
  /*------------------------------------------------------*/
  /* Lecture de la longitude SE                           */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( lonse , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lonse );
  cai_DMS_To_Float ( lonse , &lon_se );

  /*------------------------------------------------------*/
  /* Lecture ligne coin SE                                */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf (ligc4 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ligc4 );

  /*------------------------------------------------------*/
  /* Lecture colonne coin SE                              */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( colc4 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , colc4 );
  
  /*------------------------------------------------------*/
  /* Lecture latitude nadir centre                        */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( latnad , "\0" );
  lat_nad = 0;
  sscanf ( TABLEAU[0] , "%s" , latnad );
  cai_DMS_To_Float ( latnad , &lat_nad );
  
  /*------------------------------------------------------*/
  /* Lecture longitude nadir centre                       */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( longnad , "\0" );
  long_nad = 0;
  sscanf ( TABLEAU[0] , "%s" , longnad );
  cai_DMS_To_Float ( longnad , &long_nad );
  
  /*------------------------------------------------------*/
  /* Lecture de l'angle d'orientation                     */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead , TABLEAU[0] , 16 );

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0'; 
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( angle_o , "\0" );
  sscanf ( TABLEAU[0] , "%s" , angle_o );

  /*------------------------------------------------------*/
  /* Lecture de l'angle d'incidence                       */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead , TABLEAU[0] , 16 );

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';  /* Pour ne pas lire les char suivants */
  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( angle_in , "\0" );
  sscanf ( TABLEAU[0] , "%s" , angle_in );

  /*------------------------------------------------------*/
  /* Lecture de l'azimut solaire                          */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( sunazim , "\0" );
  sscanf ( TABLEAU[0] , "%s" , sunazim );

  /*------------------------------------------------------*/
  /* Lecture de l'elevation solaire                       */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( sunelev , "\0" );
  sscanf ( TABLEAU[0] , "%s" , sunelev );

  /*------------------------------------------------------*/
  /* Lecture de l'altitude du satellite                   */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );

  if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( sat_alt , "\0" );
  sscanf ( TABLEAU[0] , "%s" , sat_alt );

  /*------------------------------------------------------*/
  /* Saut de 72 octets 			                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 72 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Lecture de la date                                   */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );

  if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  date[0]='\0';
  mi0='\0';
  mi1='\0';
  a0='\0';
  a1='\0';
  m0='\0';
  m1='\0';
  j0='\0';
  j1='\0';
  miprim0='\0';
  miprim1='\0';
  aprim0='\0';
  aprim1='\0';
  mprim0='\0';
  mprim1='\0';
  jprim0='\0';
  jprim1='\0';

  sscanf ( TABLEAU[0] , "%s" , date );
  sscanf ( date , "%c%c%c%c%c%c%c%c" , &mi0 , &mi1 , &a0 ,
           &a1 , &m0 , &m1 , &j0 , &j1 );
  sprintf ( date , "%c%c%c%c-%c%c-%c%c" , mi0 , mi1 , a0 , a1 , m0 , m1 , j0 , j1 );

  /*------------------------------------------------------*/
  /* Lecture de l'heure                                   */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 9 );

  if ( cr_read != 9 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][9] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 9 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  heure[0]='\0';
  toto[0]='\0';
  h0='\0';
  h1='\0';
  insid='\0';
  insn='\0';
  s1='\0';
  s0='\0';
  hprim0='\0';
  hprim1='\0';
  sprim1='\0';
  sprim0='\0';

  satn='\0';
  sscanf ( TABLEAU[0] , "%s" , heure );

  sscanf ( heure , "%c%c%c%c%c%c%s" , &h0 , &h1 , &mi0 , &mi1 , &s0 , &s1 , toto );
  
  sprintf ( heure , "%c%c:%c%c:%c%c" , h0 , h1 , mi0 , mi1 , s0 , s1 );
  sscanf ( scene_id , "S%c%c%c%s" , &satn , &insid , &insn ,toto );

  sprintf ( datasetname , "%c %.3d-%.3d %c%c/%c%c/%c%c %c%c:%c%c:%c%c %c %c\0" ,
            satn , K , J , a0 , a1 , m0 , m1 , j0 , j1 , h0 ,
	    h1 , mi0 , mi1 , s0 , s1 , insn , codecapt );

  sscanf ( &datecentre[15] , "%c" , &sprim1 );
  datecentre[15] = '\0' ;
  sscanf ( &datecentre[14] , "%c" , &sprim0 );
  datecentre[14] = '\0' ;
  sscanf ( &datecentre[13] , "%c" , &miprim1 );
  datecentre[13] = '\0' ;
  sscanf ( &datecentre[12] , "%c" , &miprim0 );
  datecentre[12] = '\0' ;
  sscanf ( &datecentre[11] , "%c" , &hprim1 );
  datecentre[11] = '\0' ;
  sscanf ( &datecentre[10] , "%c" , &hprim0 );
  datecentre[10] = '\0' ;
  sscanf ( &datecentre[9] , "%c" , &jprim1 );
  datecentre[9] = '\0' ;
  sscanf ( &datecentre[8] , "%c" , &jprim0 );
  datecentre[8] = '\0' ;
  sscanf ( &datecentre[7] , "%c" , &mprim1 );
  datecentre[7] = '\0' ;
  sscanf ( &datecentre[6] , "%c" , &mprim0 );
  datecentre[6] = '\0' ;
  sscanf ( &datecentre[5] , "%c" , &aprim1 );
  datecentre[5] = '\0' ;
  sscanf ( &datecentre[4] , "%c" , &aprim0 );
  datecentre[4] = '\0' ;
/*****************************************************************************/ 
/* MOD : VERSION : 4.8 : FA : Rajout SCENE au début de chaine DATASET_NAME   */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : DIMAP Ajouter un espace entre SCENE et numsat  */
/*                            dans le DATASET_NAME                           */
/*****************************************************************************/
  sprintf ( datasetname_centre , "SCENE %c %.3d-%.3d %c%c/%c%c/%c%c %c%c:%c%c:%c%c %c %c\0" ,
            satn , K , J , aprim0 , aprim1 , mprim0 , mprim1 , jprim0 , jprim1 ,hprim0 ,
	    hprim1 , miprim0 , miprim1 , sprim0 , sprim1 , insn , codecapt );
  sprintf ( dsetname , "%c%.3d%.3d%c%c%c%c%c%c%c%c%c%c%c%c%c%c\0" ,
            satn , K , J , a0 , a1 , m0 , m1 , j0 , j1 , h0 ,
	    h1 , mi0 , mi1 , s0 , s1 , insn , codecapt );

  sprintf ( dsetname_centre , "%c%.3d%.3d%c%c%c%c%c%c%c%c%c%c%c%c%c%c\0" ,
            satn , K , J , aprim0 , aprim1 , mprim0 , mprim1 , jprim0 , jprim1 , hprim0 ,
            hprim1 , miprim0 , miprim1 , sprim0 , sprim1 , insn , codecapt );


  /*------------------------------------------------------*/
  /* Saut de 15 octets 			                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 15 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Lecture du nom de satellite                          */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 4);

  if ( cr_read != 4 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][4] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 4 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( satellite , "\0" );
  sscanf ( TABLEAU[0] , "%s" , satellite );
  
  /*------------------------------------------------------*/
  /* Lecture du num de satellite                          */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );

  if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  satellite_index = 0;
  sscanf ( TABLEAU[0] ,"%d" , &satellite_index ); 

  /*------------------------------------------------------*/
  /* Lecture du nom d'instrument                          */
  /*------------------------------------------------------*/
  
  if ( satellite_index != 4 )
    {
    cr_read = read ( desc_lead, TABLEAU[0], 3 );

    if ( cr_read != 3 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    TABLEAU[0][3] = '\0';
  
    /* Pour ne pas lire les char suivants */  
    /* On met un masque sur le 8ieme bit de chaque octet : */

    for ( i = 0 ; i < 3 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
   sprintf ( instrument , "\0" );
   sscanf ( TABLEAU[0] , "%s" , instrument );
    
    cr_read = read ( desc_lead, TABLEAU[0], 13 );

    if ( cr_read != 13 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    TABLEAU[0][13] = '\0';
  
    /* Pour ne pas lire les char suivants */  
    /* On met un masque sur le 8ieme bit de chaque octet : */

    for ( i = 0 ; i < 13 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
     instrument_index=0;      
     sscanf ( TABLEAU[0] , "%d" , &instrument_index );      
    }
  else
    {
    cr_read = read ( desc_lead, TABLEAU[0], 5 );

    if ( cr_read != 5 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    TABLEAU[0][5] = '\0';
  
    /* Pour ne pas lire les char suivants */  
    /* On met un masque sur le 8ieme bit de chaque octet : */

    for ( i = 0 ; i < 5 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
     
     sprintf ( instrument , "\0" );   
     sscanf ( TABLEAU[0] , "%s" , instrument );
    
    cr_read = read ( desc_lead, TABLEAU[0], 11 );

    if ( cr_read != 11 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    TABLEAU[0][11] = '\0';
  
    /* Pour ne pas lire les char suivants */  
    /* On met un masque sur le 8ieme bit de chaque octet : */

    for ( i = 0 ; i < 11 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
      instrument_index=0;      
     sscanf ( TABLEAU[0] , "%d" , &instrument_index );      
    }
    
  /*------------------------------------------------------*/
  /* Lecture du mode spectral                             */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
    sprintf ( mode_spect , "\0" );
    sscanf ( TABLEAU[0] , "%s" , mode_spect );

  /*------------------------------------------------------*/
  /* Lecture du revolution number                         */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( revolution , "\0" );
  sscanf ( TABLEAU[0] , "%s" , revolution );

  /*------------------------------------------------------*/
  /* Lecture du nombre de pas mirroir                     */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( mirror_step , "\0" );
  sscanf ( TABLEAU[0] , "%s" , mirror_step );

  /*------------------------------------------------------*/
  /* Lecture du mode de compression                       */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( compression , "\0" );
  sscanf ( TABLEAU[0] , "%s" , compression );

  /*------------------------------------------------------*/
  /* Lecture du direct playback                           */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( direct_playback , "\0" );
  sscanf ( TABLEAU[0] , "%s" , direct_playback );

  /*------------------------------------------------------*/
  /* Lecture du nombre_de_gains                           */
  /*------------------------------------------------------*/

  for ( j=0  ; j < 4 ; j++ )
  {
    cr_read = read ( desc_lead, TABLEAU[0], 4 );

    if ( cr_read != 4 )
    {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
    }

    TABLEAU[0][4] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

    for ( i=0  ; i < 4 ; i++ )
    {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    sprintf(nb_gains[j],"\0");
    sscanf ( TABLEAU[0] , "%s" , nb_gains[j] );
  }

  /*------------------------------------------------------*/
  /* Lecture du step_num                                  */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 4 );

  if ( cr_read != 4 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][4] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i = 0 ; i < 4 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( stepnum , "\0" );
  sscanf ( TABLEAU[0] , "%s" , stepnum );

  /*------------------------------------------------------*/
  /* Lecture du coupled mode                              */
  /*------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 2 );

  if ( cr_read != 2 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i=0  ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( coupled , "\0" );
 
  sscanf ( TABLEAU[0] , "%s" , coupled );

  /*------------------------------------------------------*/
  /* Saut de 250 octets			                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 250 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Lecture du nombre de colonnes			  */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16 );

  if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */  
  /* On met un masque sur le 8ieme bit de chaque octet : */

  for ( i=0  ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  nb_col=0;
  sscanf ( TABLEAU[0], "%d", &nb_col );

  /*------------------------------------------------------*/
  /* Lecture du nombre de lignes			  */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i=0  ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  nb_lig=0;
  sscanf ( TABLEAU[0], "%d", &nb_lig );

  /*------------------------------------------------------*/
  /* Lecture de bands layout                              */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16);
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i=0  ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( band_layout, "\0" );
  sscanf ( TABLEAU[0] , "%s" , band_layout );
  
  /*------------------------------------------------------*/
  /* Lecture du nombre de Bandes spectrales               */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16);
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  nb_bs=0;
  sscanf ( TABLEAU[0], "%d", &nb_bs );
  
  /*------------------------------------------------------*/
  /* Lecture indicateur de Bandes spectrales              */
  /*------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   sprintf ( spect_ind , "\0" );
  strcpy ( spect_ind , TABLEAU[0] );
  
  /*------------------------------------------------------*/
  /* Saut de 240 octets                                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 240 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture du niveau de traitement                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( niv_trait , "\0" );

  sscanf ( TABLEAU[0] , "%s" , niv_trait );
  if (strlen(niv_trait)==0) sprintf ( niv_trait , "0" );

  /*------------------------------------------------------*/
  /* Saut de 32 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 32 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
 
  /*-------------------------------------------------------*/
  /* Lecture de resampling method                          */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( resampling , "\0" );
  sscanf ( TABLEAU[0] , "%s" , resampling );

  /*-------------------------------------------------------*/
  /* Lecture de la taille des pixels sur les lignes        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( taille_pixlig , "\0" );
  sscanf ( TABLEAU[0] , "%s" , taille_pixlig );

  /*-------------------------------------------------------*/
  /* Lecture de la taille des pixels sur les colonnes      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( taille_pixcol , "\0" );
  sscanf ( TABLEAU[0] , "%s" , taille_pixcol );
  
  /*-------------------------------------------------------*/
  /* Lecture de la projection cartographique               */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 32);
 
   if ( cr_read != 32 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
   goto ERREUR;
    }
    
  TABLEAU[0][32] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 32 ; i++)
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( mapproj , "\0" );
  sscanf ( TABLEAU[0] , "%s" , mapproj );
  
  /*-------------------------------------------------------*/
  /* Lecture longueur image en Y                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( lgy , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lgy );

  /*-------------------------------------------------------*/
  /* Lecture longueur image en X                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16);
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( lgx , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lgx );
  
  /*-------------------------------------------------------*/
  /* Lecture Mean_Rectification_Elevation (2A)             */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 9 );
 
   if ( cr_read != 9 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][9] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 9 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( mean , "\0" );
  sscanf ( TABLEAU[0] , "%s" , mean );
  
  /*------------------------------------------------------*/
  /* Saut de 143 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 143 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture coordonnees X pixel 1,1                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12);
 
   if ( cr_read != 12 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( x0 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , x0 );
  
  /*-------------------------------------------------------*/
  /* Lecture coordonnees Y pixel 1,1                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12);
 
   if ( cr_read != 12 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( y0, "\0" );
  sscanf ( TABLEAU[0] , "%s" , y0 );
  
  /*------------------------------------------------------*/
  /* Saut de 96 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 96 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture date de calibration                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead , TABLEAU[0] , 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( datecal , "\0" );

  jourcal=0;
  moiscal=0;
  anneecal=0;
  sscanf ( TABLEAU[0] , "%s" , datecal );
  if (strlen(datecal) != 0)
  {
    sscanf ( &datecal[6] , "%d" , &jourcal );
    datecal[6] = '\0';
    sscanf ( &datecal[4] , "%d" , &moiscal );

    datecal[4] = '\0';
    sscanf ( datecal, "%d" , &anneecal );
  }
  sprintf ( datecalib , "%.4d-%.2d-%.2d" , anneecal , moiscal , jourcal );

  /*-------------------------------------------------------*/
  /* Lecture coeff de calibration                          */
  /*-------------------------------------------------------*/
  
  for ( j = 0; j < 4 ; j++ )
  {
    cr_read = read ( desc_lead , TABLEAU[j] , 8 );
 
     if ( cr_read != 8 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[j][8] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 8 ; i++ )
      {
      TABLEAU[j][i] = TABLEAU[j][i] & 0177;
      }
      sprintf(gains_a[j],"\0");

    sscanf ( TABLEAU[j] , "%s" , gains_a[j] );
  }
      
  /*------------------------------------------------------*/
  /* Saut de 480 octets                                   */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 480 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture offset de calibration                         */
  /*-------------------------------------------------------*/
  
  for ( j = 0; j < 4 ; j++ )
  {
    cr_read = read ( desc_lead, TABLEAU[j], 8 );
 
    if ( cr_read != 8 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[j][8] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 8 ; i++ )
      {
      TABLEAU[j][i] = TABLEAU[j][i] & 0177;
      }
   
    sprintf ( gains_b[j] , "\0" );

    sscanf ( TABLEAU[j] , "%s" , gains_b[j] );
  }
      
  /*------------------------------------------------------*/
  /* Saut de 1191 octets                                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 608 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture shift val                                     */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( shval , "\0" );
  sscanf ( TABLEAU[0] , "%s" , shval );

  /*------------------------------------------------------*/
  /* Saut de 100 octets                                   */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 100 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture SWIR REGISTRATION FLAG                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  swir=0;
  sscanf ( TABLEAU[0] , "%d" , &swir );

  /*------------------------------------------------------*/
  /* Saut de 459 octets                                   */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 459 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture coeff a                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( coeffa , "\0" );

  sscanf ( TABLEAU[0] , "%s" , coeffa );

  /*-------------------------------------------------------*/
  /* Lecture coeff b                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeffb , "\0" ) ;

  sscanf ( TABLEAU[0] , "%s" , coeffb );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff c                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( coeffc , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeffc );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff d                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
   sprintf ( coeffd , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeffd );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff e                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( coeffe , "\0" );

  sscanf ( TABLEAU[0] , "%s" , coeffe );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff f                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( coefff , "\0" );

  sscanf ( TABLEAU[0] , "%s" , coefff );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff ap                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeffap , "\0" );

  sscanf ( TABLEAU[0] , "%s" , coeffap );

  /*-------------------------------------------------------*/
  /* Lecture coeff bp                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeffbp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeffbp );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff cp                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( coeffcp , "\0" ) ;

  sscanf ( TABLEAU[0] , "%s" , coeffcp );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff dp                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeffdp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeffdp );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff ep                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( coeffep , "\0" );

  sscanf ( TABLEAU[0] , "%s" , coeffep );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff fp                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coefffp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coefffp );
  
  /*------------------------------------------------------*/
  /* Saut de 269 octets                                   */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 269 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /********************************************************/
  /*         LEADER EPHEMERIS/ATTITUDE RECORD             */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Saut de 3384 octets                                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 20 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     printf ( " erreur69\n" );
    goto ERREUR;
    }
  
  for ( j = 0; j < 9 ; j++ )
    {
    /*-------------------------------------------------------*/
    /* Lecture X , Y , Z location points                     */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
     if ( cr_read != 12 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][12] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 12 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    X_loc[j] =0.0;
    sscanf ( TABLEAU[0] , "%lf" , &X_loc[j] );

    cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
     if ( cr_read != 12 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][12] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 12 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    Y_loc[j]= 0.0;
    sscanf ( TABLEAU[0] , "%lf" , &Y_loc[j] );
    
    cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
     if ( cr_read != 12 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][12]='\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 12 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    Z_loc[j] =0.0;
    sscanf ( TABLEAU[0] , "%lf" , &Z_loc[j] );
  
    cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
     if ( cr_read != 12 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][12] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 12 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    X_vel[j] =0.0;
    sscanf ( TABLEAU[0] , "%lf" , &X_vel[j] );

    cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
     if ( cr_read != 12 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][12] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 12 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    Y_vel[j] =0.0;
    sscanf ( TABLEAU[0] , "%lf" , &Y_vel[j] );
  
    cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
     if ( cr_read != 12 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][12] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 12 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    Z_vel[j] =0.0;
    sscanf ( TABLEAU[0] , "%lf" , &Z_vel[j] );

    /*-------------------------------------------------------*/
    /* Lecture satellite time                                */
    /*-------------------------------------------------------*/

    cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
     if ( cr_read != 5 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][5]='\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 5 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    jours[j] =0 ;
    sscanf ( TABLEAU[0] , "%ld" , &jours[j] );

    cr_read = read ( desc_lead, TABLEAU[0], 23 );
 
     if ( cr_read != 23 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][23] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 23 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    sscanf ( TABLEAU[0] , "%s" , toto );
    secondes[j] = (double) atof ( toto );
    
    cai_JulToGreg ( jours[j] , secondes[j] , time[j] );
    
    }
    
  /*-------------------------------------------------------*/
  /* Lecture DORIS FLAG                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 2 );
 
   if ( cr_read != 2 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   dorisused[0]='\0';
  sscanf ( TABLEAU[0] , "%s" , dorisused );
  
  /*------------------------------------------------------*/
  /* Saut de 24 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 24 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture line period                                   */
  /*-------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( lineperiod , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lineperiod );
  periodeligne = atof ( lineperiod ) * 0.001;
  
  /*-------------------------------------------------------*/
  /* Lecture outofrange                                    */
  /*-------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 2 );
 
   if ( cr_read != 2 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( outofrange , "\0" );
  sscanf ( TABLEAU[0] , "%s" , outofrange );
  
  /*------------------------------------------------------*/
  /* Saut de 4 octets                                     */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 4 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture timescenecenter                               */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 30 );
 
   if ( cr_read != 30 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][30] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 30 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( timecenter , "\0" );
  sprintf ( time_scenecenter , "\0" );
  sscanf ( TABLEAU[0] , "%s" , time_scenecenter );
  if (time_scenecenter[0]!='\0')
  {
      strcpy ( timecenter , time_scenecenter );
     secondescenter = atof ( &time_scenecenter[5] );
     time_scenecenter[5] = '\0';
     jourscenter = 0;
     sscanf ( time_scenecenter , "%ld" , &jourscenter );
     cai_JulToGreg ( jourscenter , secondescenter , time_scenecenter );
  }
  /*------------------------------------------------------*/
  /* Saut de 6 octets                                     */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 6 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  for ( j = 0; j < 73 ; j++ )
    {
    /*-------------------------------------------------------*/
    /* Lecture attitude line number                          */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 4 );
 
     if ( cr_read != 4 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][4] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 4 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    line[j] = 0 ;
    sscanf ( TABLEAU[0] , "%d" , &line[j] );
   
/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : erreur calcul de la datation des attitudes     */
/*****************************************************************************/
 
    cai_addtime ( time_angle[j] ,  timecenter ,
                                  ((double) ( line[j] - nbc_c ))*periodeligne );
    
    /*-------------------------------------------------------*/
    /* Lecture rotation yaw axis                             */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
     if ( cr_read != 5 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][5] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 5 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
    
    yawdeg = atof ( TABLEAU[0] );
    yawrad = cai_DegreeToRadian ( yawdeg ) * 0.000001;
    
    if ( j == 72 )
      {
      strcpy ( yaw[j] , "" );
      sscanf ( TABLEAU[0] , "%s" , yaw[j] );
      if ( strcmp ( yaw[j] , "" ) !=0)
        sprintf ( yaw[j] , "%.12lf" , yawrad );
      }
    else      
      sprintf ( yaw[j] , "%.12lf" , yawrad );

    /*-------------------------------------------------------*/
    /* Lecture rotation roll axis                            */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
     if ( cr_read != 5 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][5] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 5 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    yawdeg = atof ( TABLEAU[0] ) * 0.000001;
    yawrad = cai_DegreeToRadian ( yawdeg );
    sprintf ( roll[j] , "%.12lf" , yawrad );

    /*-------------------------------------------------------*/
    /* Lecture rotation pitch axis                           */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
     if ( cr_read != 5 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][5] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 5 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    yawdeg = atof ( TABLEAU[0] ) * 0.000001;
    yawrad = cai_DegreeToRadian ( yawdeg );

    sprintf ( pitch[j] , "%.12lf" , yawrad );

    /*------------------------------------------------------*/
    /* Saut de 1 octet                                      */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 1 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    }

  /*-------------------------------------------------------*/
  /* Lecture psi_x_first                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  /*  sscanf ( TABLEAU[0] , "%s" , psi_x_first );*/
  
  /*-------------------------------------------------------*/
  /* Lecture psi_x_last                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  /*  sscanf ( TABLEAU[0] , "%s" , psi_x_last );*/
  
  /*-------------------------------------------------------*/
  /* Lecture psi_y_first                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  /*  sscanf ( TABLEAU[0] , "%s" , psi_y_first );*/
  
  /*-------------------------------------------------------*/
  /* Lecture psi_y_last                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  /*  sscanf ( TABLEAU[0] , "%s" , psi_x_last );*/
  
  /*------------------------------------------------------*/
  /* Saut de 508 octets                                   */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 508 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  for ( j = 0; j < 2 ; j++ )
  {
  /*-------------------------------------------------------*/
  /* Lecture line number raw scene                         */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  line_raw[j]= 0 ;
  sscanf ( TABLEAU[0] , "%d" , &line_raw[j] );

/*****************************************************************************/
/* MOD : VERSION : 4.6 : FA : erreur calcul de la datation des attitudes     */
/*****************************************************************************/

  cai_addtime ( timeyaw[j] ,  timecenter ,
                             ((double) ( line_raw[j] - nbc_c ))*periodeligne );
  
  /*-------------------------------------------------------*/
  /* Lecture yaw_angle                                     */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  yawdeg = atof ( TABLEAU[0] ) * 0.000001;
  yawrad = cai_DegreeToRadian ( yawdeg );

  sprintf ( yaw_angle[j] , "%.12lf" , yawrad );
  
  /*-------------------------------------------------------*/
  /* Lecture roll_angle                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  yawdeg = atof ( TABLEAU[0] ) * 0.000001;
  yawrad = cai_DegreeToRadian ( yawdeg );

  sprintf ( roll_angle[j] , "%.12lf" , yawrad );
  
  /*-------------------------------------------------------*/
  /* Lecture pitch_angle                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  yawdeg = atof ( TABLEAU[0] ) * 0.000001;
  yawrad = cai_DegreeToRadian ( yawdeg );

  sprintf ( pitch_angle[j] , "%.12lf" , yawrad );
  
  }
  
  if ( nb_bs == 1 )
    ind_max = nb_bs;
  else
  if ( nb_bs == 3 )
    ind_max = 2;
  else
    ind_max = 3;
    
  for ( j = 0; j < ind_max ; j++ )
  {

  /*-------------------------------------------------------*/
  /* Lecture psi_x_first                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  cai_DMS_To_Float ( TABLEAU[0] , &psideg );
  psi_x_first[j] = cai_DegreeToRadian ( psideg );
  
  /*-------------------------------------------------------*/
  /* Lecture psi_x_last                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  cai_DMS_To_Float ( TABLEAU[0] , &psideg );
  psi_x_last[j] = cai_DegreeToRadian ( psideg );  
    
  /*-------------------------------------------------------*/
  /* Lecture psi_y_first                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  cai_DMS_To_Float ( TABLEAU[0] , &psideg );
  psi_y_first[j] = cai_DegreeToRadian ( psideg );
  
  /*-------------------------------------------------------*/
  /* Lecture psi_y_last                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  psideg = 0.0;
  cai_DMS_To_Float ( TABLEAU[0] , &psideg );
  psi_y_last[j] = cai_DegreeToRadian ( psideg );
  }

  /*-------------------------------------------------------*/
  /* saut different selon le nombre d'angles lus           */
  /*-------------------------------------------------------*/
  
  if ( ind_max == 1 )
    {
    /* saut de 8*12 octets */
    
    /*------------------------------------------------------*/
    /* Saut de 96 octets                                    */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 96 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    }
  else
  if ( ind_max == 2 )
    {
    /* saut de 4*12 octets */
    
    /*------------------------------------------------------*/
    /* Saut de 48 octets                                    */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 48 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    }

  /*-------------------------------------------------------*/
  /* Lecture date julienne                                 */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 7 );
 
   if ( cr_read != 7 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][7] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 7 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  jourjul=0;
  sscanf ( TABLEAU[0] , "%d" , &jourjul );
  
  cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
   if ( cr_read != 5 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][5] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 5 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  datejul=0;
  sscanf ( TABLEAU[0] , "%d" , &datejul );
  
  /*-------------------------------------------------------*/
  /* Lecture major axis                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( axis , "\0" );
  sscanf ( TABLEAU[0] , "%s" , axis );
  
  /*-------------------------------------------------------*/
  /* Lecture ex                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ex , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ex );
  
  /*-------------------------------------------------------*/
  /* Lecture ey                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ey , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ey );
  
  /*-------------------------------------------------------*/
  /* Lecture inclinaison                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( inclin , "\0" );
  sscanf ( TABLEAU[0] , "%s" , inclin );
  
  /*-------------------------------------------------------*/
  /* Lecture ascension                                     */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ascension , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ascension );
  
  /*-------------------------------------------------------*/
  /* Lecture latitude argument                             */
  /*-------------------------------------------------------*/

  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( latarg , "\0" );
  sscanf ( TABLEAU[0] , "%s" , latarg );
  
  /*-------------------------------------------------------*/
  /* Lecture p7                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( p7 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , p7 );
  
  /*-------------------------------------------------------*/
  /* Lecture p8                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( p8 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , p8 );
  
  /*-------------------------------------------------------*/
  /* Lecture p9                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( p9 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , p9 );
  
  /*-------------------------------------------------------*/
  /* Lecture p10                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( p10 , "\0" );
  sscanf ( TABLEAU[0] , "%s" , p10 );
  
  /*-------------------------------------------------------*/
  /* Lecture segmentstart                                  */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( segmentstart , "\0" );
  sscanf ( TABLEAU[0] , "%s" , segmentstart );
  
  /*-------------------------------------------------------*/
  /* Lecture scenestart                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( scenestart , "\0" );
  sscanf ( TABLEAU[0] , "%s" , scenestart );
  
  /*-------------------------------------------------------*/
  /* Lecture segmentend                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( segmentend , "\0" );
  sscanf ( TABLEAU[0] , "%s" , segmentend );
  
  /*-------------------------------------------------------*/
  /* Lecture UT date                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 7 );
 
   if ( cr_read != 7 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][7] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 7 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  utdatej=0;
  sscanf ( TABLEAU[0] , "%d" , &utdatej );

  cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
   if ( cr_read != 5 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][5] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 5 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  utdates=0;
  sscanf ( TABLEAU[0] , "%d" , &utdates );
  
  /*-------------------------------------------------------*/
  /* Lecture clockvalue                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( clockvalue , "\0" );
  sscanf ( TABLEAU[0] , "%s" , clockvalue );
  
  /*-------------------------------------------------------*/
  /* Lecture clockperiod                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( clockperiod , "\0" );
  sscanf ( TABLEAU[0] , "%s" , clockperiod );  
  
  /*-------------------------------------------------------*/
  /* Lecture boardtime                                     */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 12 );
 
   if ( cr_read != 12 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( boardtime , "\0" );
  sscanf ( TABLEAU[0] , "%s" , boardtime );
  
  /*------------------------------------------------------*/
  /* Saut de 528 octets                                   */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 528 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /********************************************************/
  /*        LEADER RADIOMETRIC CALIBRATION RECORD         */
  /********************************************************/
  ind_gain_val = 0;
  ind_dark_val = 0;

  for ( ind_gains = 0 ; ind_gains < 16 ; ind_gains++ )
    {

    /*------------------------------------------------------*/
    /* Saut de 24 octets                                    */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 24 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    /*------------------------------------------------------*/
    /* Lecture du flag indiquant si c'est un gains ou dark  */
    /*------------------------------------------------------*/

    cr_read = read ( desc_lead, TABLEAU[0], 4 );
 
    if ( cr_read != 4 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][4] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 4 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
    type_gain =0;
    sscanf ( TABLEAU[0] , "%d" , &type_gain );

    /*------------------------------------------------------*/
    /* Saut de 20 octets                                    */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 20 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }

    /*------------------------------------------------------*/
    /* Lecture date calibration de cette bande              */
    /*------------------------------------------------------*/

    cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
    if ( cr_read != 8 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][8] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 8 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
    relativeannee=0;
    sscanf ( &(TABLEAU[0][4]) , "%d" , &relativeannee );
    TABLEAU[0][4] = '\0';
    relativemois=0;
    sscanf ( &(TABLEAU[0][2]) , "%d" , &relativemois );
    TABLEAU[0][2] = '\0';
    relativejour=0;
    sscanf ( TABLEAU[0] , "%d" , &relativejour );
    
    sprintf ( relativedatecalib[ind_gains] , "%.4d-%.2d-%.2d 00:00:00" , relativeannee ,
              relativemois , relativejour );

    /*------------------------------------------------------*/
    /* Saut de 4 octets                                     */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 4 , SEEK_CUR );
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    if ( type_gain == 1 )
      {
      for ( ind_tab_gain = 0 ; ind_tab_gain < 1500 ; ind_tab_gain++ )
        {

        /*------------------------------------------------------*/
        /* Lecture des 1500 gains                               */
        /*------------------------------------------------------*/

        cr_read = read ( desc_lead, &val , 2 );
 
        if ( cr_read != 2 )
          {
          sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
          goto ERREUR;
          }
         if (strcmp(type_machine,"LE")==0) 
         { 
	     swab((void*)&val,(void*)&valtemp,2);
             val=valtemp;
         }
	 val_float = (float) val/10000.0;
	 gain_value[ind_gain_val][ind_tab_gain] = val_float ;
        }
      ind_gain_val++;
      }
    else if ( type_gain == 2 )
    {
      for ( ind_tab_gain =0 ; ind_tab_gain < 1500 ; ind_tab_gain++ )
        {

        /*------------------------------------------------------*/
        /* Lecture des 1500 darks                               */
        /*------------------------------------------------------*/

        cr_read = read ( desc_lead, &val, 2 );
        if ( cr_read != 2 )
          {
          sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
          goto ERREUR;
          }
         if (strcmp(type_machine,"LE")==0) 
         { 
	     swab((void*)&val,(void*)&valtemp,2);
             val=valtemp;
         }
	 val_float = (float) val/10.0;
	 dark_value[ind_dark_val][ind_tab_gain] = val_float ;
        }
      ind_dark_val++;
     }
     else
     {
/* Valeurs non significatives . Saut de l'enregistrement */
	 cr_lseek = lseek ( desc_lead , 3000 , SEEK_CUR );
     }

    /*------------------------------------------------------*/
    /* Saut de 900 octets                                   */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 900 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }      
    }

  /********************************************************/
  /*             LEADER MODELIZATION RECORD               */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Saut de 16 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 16 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture first pixel of the raw scene                  */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( fpixraw , "\0" );
  sscanf ( TABLEAU[0] , "%s" , fpixraw );

  /*-------------------------------------------------------*/
  /* Lecture first line of the raw scene                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( flineraw , "\0" );
  sscanf ( TABLEAU[0] , "%s" , flineraw );

  /*-------------------------------------------------------*/
  /* Lecture first pixel 1b of the raw scene               */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( fpix1b , "\0" );
  sscanf ( TABLEAU[0] , "%s" , fpix1b );

  /*-------------------------------------------------------*/
  /* Lecture first line 1b of the raw scene                */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
   if ( cr_read != 8 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( fline1b , "\0" );
  sscanf ( TABLEAU[0] , "%s" , fline1b );

  /*------------------------------------------------------*/
  /* Saut de 32 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 32 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture al                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( al , "\0" );
  sscanf ( TABLEAU[0] , "%s" , al );

  /*-------------------------------------------------------*/
  /* Lecture bl                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( bl , "\0" );
  sscanf ( TABLEAU[0] , "%s" , bl );

  /*-------------------------------------------------------*/
  /* Lecture cl                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( cl , "\0" );
  sscanf ( TABLEAU[0] , "%s" , cl );

  /*-------------------------------------------------------*/
  /* Lecture ap                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ap , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ap );

  /*-------------------------------------------------------*/
  /* Lecture bp                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( bp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , bp );

  /*-------------------------------------------------------*/
  /* Lecture cp                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( cp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , cp );

  /*------------------------------------------------------*/
  /* Saut de 16 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 16 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture i_m                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( im , "\0" );
  sscanf ( TABLEAU[0] , "%s" , im );

  /*-------------------------------------------------------*/
  /* Lecture delta_i                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( deltai , "\0" );
  sscanf ( TABLEAU[0] , "%s" , deltai );

  /*-------------------------------------------------------*/
  /* Lecture j_m                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( jm , "\0" );
  sscanf ( TABLEAU[0] , "%s" , jm );

  /*-------------------------------------------------------*/
  /* Lecture delta_j                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( deltaj , "\0" );
  sscanf ( TABLEAU[0] , "%s" , deltaj );

  /*-------------------------------------------------------*/
  /* Lecture l_m                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( lm , "\0" );
  sscanf ( TABLEAU[0] , "%s" , lm );

  /*-------------------------------------------------------*/
  /* Lecture delta_l                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( deltal , "\0" );
  sscanf ( TABLEAU[0] , "%s" , deltal );

  /*-------------------------------------------------------*/
  /* Lecture p_m                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( pm , "\0" );
  sscanf ( TABLEAU[0] , "%s" , pm );

  /*-------------------------------------------------------*/
  /* Lecture delta_p                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( deltap , "\0" );
  sscanf ( TABLEAU[0] , "%s" , deltap );
  
  /*------------------------------------------------------*/
  /* Saut de 16 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 16 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture coeff_i                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( coeffi , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeffi );
  
  /*-------------------------------------------------------*/
  /* Lecture ai                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ai , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ai );
  
  /*-------------------------------------------------------*/
  /* Lecture bi                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  sprintf ( bi  , "\0" );
  sscanf ( TABLEAU[0] , "%s" , bi );
  
  /*-------------------------------------------------------*/
  /* Lecture ci                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( ci , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ci );
  
  /*-------------------------------------------------------*/
  /* Lecture di                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( di , "\0" );
  sscanf ( TABLEAU[0] , "%s" , di );
  
  /*-------------------------------------------------------*/
  /* Lecture ei                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ei , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ei );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff_j                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( coeffj , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeffj );
  
  /*-------------------------------------------------------*/
  /* Lecture aj                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( aj , "\0" );
  sscanf ( TABLEAU[0] , "%s" , aj );
  
  /*-------------------------------------------------------*/
  /* Lecture bj                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( bj , "\0" );
  sscanf ( TABLEAU[0] , "%s" , bj );
  
  /*-------------------------------------------------------*/
  /* Lecture cj                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( cj , "\0" );
  sscanf ( TABLEAU[0] , "%s" , cj );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff_a                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeff_a , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeff_a );
  
  /*-------------------------------------------------------*/
  /* Lecture aa                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( aa , "\0" );
  sscanf ( TABLEAU[0] , "%s" , aa );
  
  /*-------------------------------------------------------*/
  /* Lecture ba                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ba , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ba );
  
  /*-------------------------------------------------------*/
  /* Lecture ca                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ca  , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ca );
  
  /*-------------------------------------------------------*/
  /* Lecture da                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( da  , "\0" );
  sscanf ( TABLEAU[0] , "%s" , da );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff_b                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeff_b , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeff_b );
  
  /*-------------------------------------------------------*/
  /* Lecture ab                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( ab , "\0" );
  sscanf ( TABLEAU[0] , "%s" , ab );
  
  /*-------------------------------------------------------*/
  /* Lecture bb                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( bb , "\0" );
  sscanf ( TABLEAU[0] , "%s" , bb );
  
  /*-------------------------------------------------------*/
  /* Lecture cb                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( cb , "\0" );
  sscanf ( TABLEAU[0] , "%s" , cb );
  
  /*-------------------------------------------------------*/
  /* Lecture db                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( db , "\0" );
  sscanf ( TABLEAU[0] , "%s" , db );
  
  /*-------------------------------------------------------*/
  /* Lecture eb                                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( eb , "\0" );
  sscanf ( TABLEAU[0] , "%s" , eb );
  
  /*------------------------------------------------------*/
  /* Saut de 16 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 16 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture coeff_l                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeff_l , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeff_l );
   /*-------------------------------------------------------*/
  /* Lecture a_l                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( a_l , "\0" );
  sscanf ( TABLEAU[0] , "%s" , a_l );
  
  /*-------------------------------------------------------*/
  /* Lecture b_l                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( b_l , "\0" );
  sscanf ( TABLEAU[0] , "%s" , b_l );
  
  /*-------------------------------------------------------*/
  /* Lecture c_l                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( c_l , "\0" );
  sscanf ( TABLEAU[0] , "%s" , c_l );
  
  /*-------------------------------------------------------*/
  /* Lecture d_l                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( d_l , "\0" );
  sscanf ( TABLEAU[0] , "%s" , d_l );
  
  /*-------------------------------------------------------*/
  /* Lecture e_l                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( e_l , "\0" );
  sscanf ( TABLEAU[0] , "%s" , e_l );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff_p                                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( coeff_p , "\0" );
  sscanf ( TABLEAU[0] , "%s" , coeff_p );
  
  /*-------------------------------------------------------*/
  /* Lecture a_p                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( a_p , "\0" );
  sscanf ( TABLEAU[0] , "%s" , a_p );
  
  /*-------------------------------------------------------*/
  /* Lecture b_p                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( b_p , "\0" );
  sscanf ( TABLEAU[0] , "%s" , b_p );
  
  /*-------------------------------------------------------*/
  /* Lecture c_p                                           */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( c_p , "\0" );
  sscanf ( TABLEAU[0] , "%s" , c_p );
  
  /*------------------------------------------------------*/
  /* Saut de 16 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 16 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture alswir                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( alswir  , "\0" );
  sscanf ( TABLEAU[0] , "%s" , alswir );
  
  /*-------------------------------------------------------*/
  /* Lecture blswir                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   ;
  sprintf ( blswir , "\0" );
  sscanf ( TABLEAU[0] , "%s" , blswir );
  
  /*-------------------------------------------------------*/
  /* Lecture clswir                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  sprintf ( clswir , "\0" );
  sscanf ( TABLEAU[0] , "%s" , clswir );
  
  /*-------------------------------------------------------*/
  /* Lecture apswir                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf ( apswir , "\0" );
  sscanf ( TABLEAU[0] , "%s" , apswir );
  
  /*-------------------------------------------------------*/
  /* Lecture bpswir                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  sprintf ( bpswir  , "\0" );
  sscanf ( TABLEAU[0] , "%s" , bpswir );
  
  /*-------------------------------------------------------*/
  /* Lecture cpswir                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( cpswir , "\0" );
  sscanf ( TABLEAU[0] , "%s" , cpswir );
  
  /*------------------------------------------------------*/
  /* Saut de 16 octets                                    */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 16 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /*     lecture de reverse simplified location model     */
  /*------------------------------------------------------*/

  /*-------------------------------------------------------*/
  /* Lecture coeff rlma                                    */
  /*-------------------------------------------------------*/
 

  TABLEAU[0][0]='\0'; 
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  strcpy ( rlma , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlma );

  /*-------------------------------------------------------*/
  /* Lecture coeff rlmb                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  strcpy ( rlmb , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmb );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmc                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  strcpy ( rlmc , "\0" );
  sscanf ( TABLEAU[0] , "%s" ,  rlmc);
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmd                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
   
  strcpy ( rlmd , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmd );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlme                                    */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  strcpy ( rlme , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlme );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmf                                    */
  /*-------------------------------------------------------*/
  
  TABLEAU[0][0]='\0'; 
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  strcpy ( rlmf , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmf );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmap                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  strcpy ( rlmap , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmap );

  /*-------------------------------------------------------*/
  /* Lecture coeff rlmbp                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  strcpy ( rlmbp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmbp );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmcp                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  sprintf ( rlmcp  , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmcp );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmdp                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  sprintf ( rlmdp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmdp );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmep                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    
  sprintf ( rlmep , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmep );
  
  /*-------------------------------------------------------*/
  /* Lecture coeff rlmfp                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_lead, TABLEAU[0], 16 );
 
   if ( cr_read != 16 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( rlmfp , "\0" );
  sscanf ( TABLEAU[0] , "%s" , rlmfp );
  
  /*------------------------------------------------------*/
  /* Saut de 2792 octets                                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 2792 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /********************************************************/
  /*         LEADER GROUND CONTROL POINT RECORD           */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Saut de 3960 octets                                  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_lead , 3960 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
     sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /********************************************************/
  /*             LEADER HISTOGRAM RECORD                  */
  /********************************************************/

  for ( j = 0; j < nb_bs ; j++ )
    {
    
    /*------------------------------------------------------*/
    /* Saut de 2470 octets                                  */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 2470 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
       sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
       goto ERREUR;
      }

    /*-------------------------------------------------------*/
    /* Lecture lowthresh                                     */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
     if ( cr_read != 8 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][8] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 8 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
    
    sprintf ( lowtresh[j] , "\0" );
    sscanf ( TABLEAU[0] , "%s" , lowtresh[j]  );
    
    /*-------------------------------------------------------*/
    /* Lecture upthresh                                      */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
     if ( cr_read != 8 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][8] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 8 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
    
    sprintf ( uptresh[j] , "\0" );
    sscanf ( TABLEAU[0] , "%s" , uptresh[j] );

    /*-------------------------------------------------------*/
    /* Lecture firstwavelength                               */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 8 );
 
     if ( cr_read != 8 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][8] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 8 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }

    unwave =0.0;
    
    sscanf ( TABLEAU[0] , "%lf" , &unwave );
    sprintf ( firstwave[j] , "%.12lf" , unwave * 0.000001 );

    /*-------------------------------------------------------*/
    /* Lecture wavelengthstep                                */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 4 );
 
     if ( cr_read != 4 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][4] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 4 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
    wavelong=0.0;
    sscanf ( TABLEAU[0] , "%lf" , &wavelong );
    sprintf ( wavelengthstep[j] , "%.12lf" , wavelong * 0.000000001 );

    /*-------------------------------------------------------*/
    /* Lecture des 64 coefficients                           */
    /*-------------------------------------------------------*/

    for ( ind_tab_gain = 0  ; ind_tab_gain < 64 ; ind_tab_gain++ )
      {  
      cr_read = read ( desc_lead, TABLEAU[0], 5 );
 
      if ( cr_read != 5 )
        {
        sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
        goto ERREUR;
        }
    
      TABLEAU[0][5] = '\0';
   
      /* Pour ne pas lire les char suivants */
      /* On met un masque sur le 8ieme bit de chaque octet : */
  
      for ( i = 0 ; i < 5 ; i++ )
        {
        TABLEAU[0][i] = TABLEAU[0][i] & 0177;
        }

      sprintf (  spectral_sensitivitie[j][ind_tab_gain] , "\0" );
      sscanf ( TABLEAU[0] , "%s" , spectral_sensitivitie[j][ind_tab_gain] );
      }
    
    /*-------------------------------------------------------*/
    /* Lecture solar_irradiance                              */
    /*-------------------------------------------------------*/
  
    cr_read = read ( desc_lead, TABLEAU[0], 4 );
 
     if ( cr_read != 4 )
      {
      sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
      goto ERREUR;
      }
    
    TABLEAU[0][4] = '\0';
  
    /* Pour ne pas lire les char suivants */
    /* On met un masque sur le 8ieme bit de chaque octet : */
  
    for ( i = 0 ; i < 4 ; i++ )
      {
      TABLEAU[0][i] = TABLEAU[0][i] & 0177;
      }
     
     sprintf ( solar_irradiance[j] , "\0" );
     sscanf ( TABLEAU[0] , "%s" , solar_irradiance[j] );

    /*------------------------------------------------------*/
    /* Saut de 1138 octets                                  */
    /*------------------------------------------------------*/

    cr_lseek = lseek ( desc_lead , 1138 , SEEK_CUR );
  
    if ( cr_lseek == -1 )
      {
       sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Leader octet %ld" , cr_lseek );
       goto ERREUR;
      }
    }

  /********************************************************/
  /*         LECTURE DU FICHIER VOLUME DIRECTORY          */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Retour au debut du fichier		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 0L , 0 );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Saut de 60 octects 		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 60 , SEEK_SET );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture job_id                                        */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 16 );
 
  if ( cr_read != 16 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
    sprintf ( job_id , "\0" );
  strcpy ( job_id , TABLEAU[0] );

  /*-------------------------------------------------------*/
  /* Lecture product_type                                  */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 16 );
 
  if ( cr_read != 16 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][16] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 16 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  sprintf( product_type, "\0" );
  strcpy ( product_type , TABLEAU[0] );
  
  /*------------------------------------------------------*/
  /* Saut de 20 octects 		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 20 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture product_date                                  */
  /*-------------------------------------------------------*/

  /*-------------------------------------------------------*/
  /* Lecture product_annee                                 */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 4 );
 
  if ( cr_read != 4 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][4] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 4 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  product_annee=0;
  sscanf ( TABLEAU[0] , "%d" , &product_annee );

  /*-------------------------------------------------------*/
  /* Lecture product_mois                                  */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 2 );
 
  if ( cr_read != 2 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  product_mois=0;
  sscanf ( TABLEAU[0] , "%d" , &product_mois );
  
  /*-------------------------------------------------------*/
  /* Lecture product_jour                                  */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 2 );
 
  if ( cr_read != 2 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  product_jour=0;
  sscanf ( TABLEAU[0] , "%d" , &product_jour );
  
  /*-------------------------------------------------------*/
  /* Lecture product_heure                                 */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 2 );
 
  if ( cr_read != 2 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  product_heure =0;
  sscanf ( TABLEAU[0] , "%d" , &product_heure );
  
  /*-------------------------------------------------------*/
  /* Lecture product_min                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 2 );
 
  if ( cr_read != 2 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  product_min=0;
  sscanf ( TABLEAU[0] , "%d" , &product_min );
  
  /*-------------------------------------------------------*/
  /* Lecture product_sec                                   */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 2 );
 
  if ( cr_read != 2 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][2] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 2 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  product_sec=0;
  sscanf ( TABLEAU[0] , "%d" , &product_sec );

  /*------------------------------------------------------*/
  /* Saut de 2 octects 			  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 2 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture produser                                      */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 12 );
 
  if ( cr_read != 12 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
 
  sprintf ( produser , "\0" );
  strcpy ( produser , TABLEAU[0] );

  /*-------------------------------------------------------*/
  /* Lecture processing_center                             */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 8 );
 
  if ( cr_read != 8 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][8] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 8 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( processing_center , "\0" );
  strcpy ( processing_center , TABLEAU[0] );

  /*-------------------------------------------------------*/
  /* Lecture software_name                                 */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 12 );
 
  if ( cr_read != 12 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( software_name , "\0" );
  strcpy ( software_name , TABLEAU[0] );

  /*------------------------------------------------------*/
  /* Saut de 1296 octects 			  	  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 1296 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture source_description                            */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 104 );
 
  if ( cr_read != 104 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][104] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 104 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  
  sprintf ( source_description , "\0" );
  strcpy ( source_description , TABLEAU[0] );

  /*------------------------------------------------------*/
  /* Saut de 18 octects 			  	  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 18 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture copyright                                     */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_vol, TABLEAU[0], 14 );
 
  if ( cr_read != 14 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][14] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 14 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }
  strcpy ( copyright , TABLEAU[0] );
  
  /*------------------------------------------------------*/
  /* Saut de 2 octects 			     	          */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 2 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  cr_read = read ( desc_vol, TABLEAU[0], 12 );
 
  if ( cr_read != 12 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][12] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 12 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  strcat ( copyright , TABLEAU[0] );
  
  /*------------------------------------------------------*/
  /* Saut de 2 octects 			     	          */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_vol , 2 , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
    goto ERREUR;
    }

  cr_read = read ( desc_vol, TABLEAU[0], 10 );
 
  if ( cr_read != 10 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Volume Directory octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][10] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 10 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  strcat ( copyright , TABLEAU[0] );

  /********************************************************/
  /*         LECTURE DU FICHIER IMAGERY                   */
  /********************************************************/

  /*------------------------------------------------------*/
  /* Retour au debut du fichier		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_img , 0L , 0 );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Saut de 186 octects 		    	          */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_img , 186 , SEEK_SET );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*-------------------------------------------------------*/
  /* Lecture longueur enregistrement                       */
  /*-------------------------------------------------------*/
  
  cr_read = read ( desc_img, TABLEAU[0], 6 );
 
  if ( cr_read != 6 )
   {
   sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
   goto ERREUR;
   }
    
  TABLEAU[0][6] = '\0';
  
  /* Pour ne pas lire les char suivants */
  /* On met un masque sur le 8ieme bit de chaque octet : */
  
  for ( i = 0 ; i < 6 ; i++ )
    {
    TABLEAU[0][i] = TABLEAU[0][i] & 0177;
    }

  lg_enreg = 0;
  sscanf ( TABLEAU[0] , "%d" , &lg_enreg );
  
  /*------------------------------------------------------*/
  /* Retour au debut du fichier		  		  */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_img , 0L , SEEK_SET );

  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
    goto ERREUR;
    }

  /*------------------------------------------------------*/
  /* Saut de lg_enreg octects 		    	          */
  /*------------------------------------------------------*/

  cr_lseek = lseek ( desc_img , (long) lg_enreg , SEEK_CUR );
  
  if ( cr_lseek == -1 )
    {
    sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
    goto ERREUR;
    }
  
  for ( j = 0; j < nb_lig ; j++ )
    {
    for ( can = 0 ; can < nb_bs ; can++ )
      {
      /*---------------------------------------------------*/
      /* Saut de lg_enreg octects 		           */
      /*---------------------------------------------------*/
      cr_lseek = lseek ( desc_img , (long) ( lg_enreg - 69 ), SEEK_CUR );
    
      if ( cr_lseek == -1 )
        {
        sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
        goto ERREUR;
        }
      
      /*----------------------------------------------------*/
      /* Lecture loss flag                                  */
      /*----------------------------------------------------*/
  
      cr_read = read ( desc_img , &bad_line[j][can] , 1 );
 
      if ( cr_read != 1 )
        {
        sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
        goto ERREUR;
        }

      /*---------------------------------------------------*/
      /* Saut de 68 octects 		                   */
      /*---------------------------------------------------*/
      
      cr_lseek = lseek ( desc_img , 68 , SEEK_CUR );
    
      if ( cr_lseek == -1 )
        {
        sprintf ( CAI_ERREUR , "Erreur Lecture du fichier Imagery octet %ld" , cr_lseek );
        goto ERREUR;
        }
      }
    }
    
  /********************************************************/
  /*         OUVERTURE DU FICHIER XML PRODUIT             */
  /********************************************************/

  if ( ( des = fopen ( chemin_descripteur , "wb" ) ) == NULL )
    {
    sprintf ( CAI_ERREUR , "Erreur Ouverture du fichier XML" );
    goto ERREUR;
    }

  /********************************************************/
  /*      ECRITURE DES DONNEES XML DANS LE FICHIER        */
  /********************************************************/

  /* Entete du document XML */

  cr = cai_WriteEnteteXML ( des );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur Ecriture du fichier XML" );
    goto ERREUR;
    }
  
  /* Element Dimap_document */
 
  cr = cai_WriteOpenTagXML ( des , "Dimap_Document" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dimap_Document" );
    goto ERREUR;
    }

  /* Sous element Metadata_Id */
  
  cr = cai_WriteOpenTagXML ( des , "Metadata_Id" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Metadata_Id" );
    goto ERREUR;
    }

  /* sous element METADATA_FORMAT */
  /*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Version DIMAP passe de 2.0 à 2.9               */
/*****************************************************************************/
  type=0;
  cr = cai_WriteElementXMLWithAttribute ( des , "METADATA_FORMAT" , "DIMAP" ,
                                          "version" , "2.9" ,type);
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element METADATA_FORMAT" );
    goto ERREUR;
    }

  /* sous element METADATA_PROFILE */

  profile = (char*) NULL;
  profile = (char*) calloc ( strlen ( niv_trait ) + 11 , sizeof (char) );
  
  sprintf ( profile , "SPOTSCENE_%s" , niv_trait );
  
  cr = cai_WriteElementXML ( des , "METADATA_PROFILE" , profile );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element METADATA_PROFILE" );
    goto ERREUR;
    }

  FREE ( profile );
  
 /* Fermeture de l'element Metadata_Id */

  cr = cai_WriteCloseTagXML ( des , "Metadata_Id" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Metadata_Id" );
    goto ERREUR;
    }
  
 /* Sous element Dataset_Id */
  
  cr = cai_WriteOpenTagXML ( des , "Dataset_Id" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dataset_Id" );
    goto ERREUR;
    }

  /* sous element DATASET_NAME */
  
  cr = cai_WriteElementXML ( des , "DATASET_NAME" , datasetname_centre );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_NAME" );
    goto ERREUR;
    }

  /* sous element COPYRIGHT */

  cr = cai_WriteElementXML ( des , "COPYRIGHT" , copyright );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COPYRIGHT" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'attribut href=""                   */
/****************************************************************************/

  /* sous element DATASET_TN_PATH */
  type=1;
  cr = cai_WriteElementXMLWithAttribute ( des , "DATASET_TN_PATH" , "" , "href" , "" ,type);

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_TN_PATH" );
    goto ERREUR;
    }

  /* sous element DATASET_TN_FORMAT */

  cr = cai_WriteElementXML ( des , "DATASET_TN_FORMAT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_TN_FORMAT" );
    goto ERREUR;
    }

  /* sous element DATASET_QL_PATH */
  type=1;
  cr = cai_WriteElementXMLWithAttribute ( des , "DATASET_QL_PATH" , "" , "href" , "",type );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_QL_PATH" );
    goto ERREUR;
    }

  /* sous element DATASET_QL_FORMAT */

  cr = cai_WriteElementXML ( des , "DATASET_QL_FORMAT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_QL_FORMAT" );
    goto ERREUR;
    }

 /* Fermeture de l'element Dataset_Id */

  cr = cai_WriteCloseTagXML ( des , "Dataset_Id" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dataset_Id" );
    goto ERREUR;
    }

 /* Sous element Dataset_Frame */
  
  cr = cai_WriteOpenTagXML ( des , "Dataset_Frame" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dataset_Frame" );
    goto ERREUR;
    }

  /* sous element Vertex */

  cr = cai_WriteOpenTagXML ( des , "Vertex" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

  /* sous element FRAME_LON */
  
  sprintf ( valeur , "%lf" , lon_no );

  cr = cai_WriteElementXML ( des , "FRAME_LON" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LON" );
    goto ERREUR;
    }

  /* sous element FRAME_LAT */

  sprintf ( valeur , "%lf" , lat_no );
  
  cr = cai_WriteElementXML ( des , "FRAME_LAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LAT" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour FRAME_ROW, FRAME_COL   */
/*****************************************************************************/
/* Coin NO */
/***********/
  /* sous element FRAME_ROW */
    sscanf ( ligc1 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "FRAME_ROW" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_ROW" );
    goto ERREUR;
    }
  /* sous element FRAME_COL */
    sscanf ( colc1 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
 
  cr = cai_WriteElementXML ( des , "FRAME_COL" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_COL" );
    goto ERREUR;
    }

 /* Fermeture de l'element Vertex */

  cr = cai_WriteCloseTagXML ( des , "Vertex" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

  /* sous element Vertex */

  cr = cai_WriteOpenTagXML ( des , "Vertex" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

  /* sous element FRAME_LON */
  
  sprintf ( valeur , "%f" , lon_ne );

  cr = cai_WriteElementXML ( des , "FRAME_LON" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LON" );
    goto ERREUR;
    }

  /* sous element FRAME_LAT */
  
  sprintf ( valeur , "%f" , lat_ne );

  cr = cai_WriteElementXML ( des , "FRAME_LAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LAT" );
    goto ERREUR;
    }
  /*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour FRAME_ROW, FRAME_COL   */
/*****************************************************************************/
/* Coin NE */
/***********/
  /* sous element FRAME_ROW */
    sscanf ( ligc2 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "FRAME_ROW" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_ROW" );
    goto ERREUR;
    }
  /* sous element FRAME_COL */
    sscanf ( colc2 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "FRAME_COL" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_COL" );
    goto ERREUR;
    }

 /* Fermeture de l'element Vertex */

  cr = cai_WriteCloseTagXML ( des , "Vertex" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : DIMAP dans le DATASET_FRAME, inversion des deux*/
/*                            derniers vertex : à mettre dans l'ordre        */
/*				NO,NE,SE,SO : inversion de c3 et c4          */
/*****************************************************************************/
/* Coin SE */
/***********/
/* sous element Vertex */

  cr = cai_WriteOpenTagXML ( des , "Vertex" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

  /* sous element FRAME_LON */
  
  sprintf ( valeur , "%lf" , lon_se );

  cr = cai_WriteElementXML ( des , "FRAME_LON" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LON" );
    goto ERREUR;
    }

  /* sous element FRAME_LAT */

  sprintf ( valeur , "%lf" , lat_se );
  
  cr = cai_WriteElementXML ( des , "FRAME_LAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LAT" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour FRAME_ROW, FRAME_COL   */
/*****************************************************************************/

  /* sous element FRAME_ROW */
    sscanf ( ligc4 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
  
  cr = cai_WriteElementXML ( des , "FRAME_ROW" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_ROW" );
    goto ERREUR;
    }
  /* sous element FRAME_COL */
    sscanf ( colc4 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "FRAME_COL" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_COL" );
    goto ERREUR;
    }

 /* Fermeture de l'element Vertex */

  cr = cai_WriteCloseTagXML ( des , "Vertex" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }
/***********/
/* Coin SO */
/***********/ 
  /* sous element Vertex */

  cr = cai_WriteOpenTagXML ( des , "Vertex" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

  /* sous element FRAME_LON */
  
  sprintf ( valeur , "%lf" , lon_so );

  cr = cai_WriteElementXML ( des , "FRAME_LON" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LON" );
    goto ERREUR;
    }

  /* sous element FRAME_LAT */

  sprintf ( valeur , "%lf" , lat_so );
  
  cr = cai_WriteElementXML ( des , "FRAME_LAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LAT" );
    goto ERREUR;
    }
    
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour FRAME_ROW, FRAME_COL   */
/*****************************************************************************/
/* sous element FRAME_ROW */
    sscanf ( ligc3 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
  
  cr = cai_WriteElementXML ( des , "FRAME_ROW" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_ROW" );
    goto ERREUR;
    }
  /* sous element FRAME_COL */
     sscanf ( colc3 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
 
  cr = cai_WriteElementXML ( des , "FRAME_COL" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_COL" );
    goto ERREUR;
    }

 /* Fermeture de l'element Vertex */

  cr = cai_WriteCloseTagXML ( des , "Vertex" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Vertex" );
    goto ERREUR;
    }

  /* sous element Scene_Center */

  cr = cai_WriteOpenTagXML ( des , "Scene_Center" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Scene_Center" );
    goto ERREUR;
    }

  /* sous element FRAME_LON */
  
  sprintf ( valeur , "%lf" , lon_c );
  cr = cai_WriteElementXML ( des , "FRAME_LON" ,  valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LON" );
    goto ERREUR;
    }
  /* sous element FRAME_LAT */
  
  sprintf ( valeur , "%lf" , lat_c );
  
  cr = cai_WriteElementXML ( des , "FRAME_LAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_LAT" );
    goto ERREUR;
    }
  /* sous element FRAME_ROW */
  
  sprintf ( valeur , "%d" , nbc_c );
  cr = cai_WriteElementXML ( des , "FRAME_ROW" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_ROW" );
    goto ERREUR;
    }

  /* sous element FRAME_COL */
  
  sprintf ( valeur , "%d" , nbl_c );
  cr = cai_WriteElementXML ( des , "FRAME_COL" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FRAME_COL" );
    goto ERREUR;
    }

 /* Fermeture de l'element Scene_Center */

  cr = cai_WriteCloseTagXML ( des , "Scene_Center" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Scene_Center" );
    goto ERREUR;
    }

  /* sous element SCENE_ORIENTATION */

  cr = cai_WriteElementXML ( des , "SCENE_ORIENTATION" , angle_o );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SCENE_ORIENTATION" );
    goto ERREUR;
    }

 /* Fermeture de l'element Dataset_Frame */

  cr = cai_WriteCloseTagXML ( des , "Dataset_Frame" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dataset_Frame" );
    goto ERREUR;
    }

  /* sous element Dataset_Sources */
  
  cr = cai_WriteOpenTagXML ( des , "Dataset_Sources" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dataset_Sources" );
    goto ERREUR;
    }

  /* sous element Source_Information */
  
  cr = cai_WriteOpenTagXML ( des , "Source_Information" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Source_Information" );
    goto ERREUR;
    }

  /* sous element SOURCE_TYPE */
  
  cr = cai_WriteElementXML ( des , "SOURCE_TYPE" , "SCENE" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SOURCE_TYPE" );
    goto ERREUR;
    }

  /* sous element SOURCE_ID */
  
  cr = cai_WriteElementXML ( des , "SOURCE_ID" , dsetname_centre );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SOURCE_ID" );
    goto ERREUR;
    }

  /* sous element SOURCE_DESCRIPTION */
  
  cr = cai_WriteElementXML ( des , "SOURCE_DESCRIPTION" , source_description );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SOURCE_DESCRIPTION" );
    goto ERREUR;
    }

  /* sous element Scene_Source */
  
  cr = cai_WriteOpenTagXML ( des , "Scene_Source" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Scene_Source" );
    goto ERREUR;
    }

  /* sous element GRID_REFERENCE */
  
  cr = cai_WriteElementXML ( des , "GRID_REFERENCE" , grille_ref );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element GRID_REFERENCE" );
    goto ERREUR;
    }

  /* sous element SHIFT_VALUE */
  
  cr = cai_WriteElementXML ( des , "SHIFT_VALUE" , shift_val );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SHIFT_VALUE" );
    goto ERREUR;
    }

  /* sous element IMAGING_DATE */
  
  cr = cai_WriteElementXML ( des , "IMAGING_DATE" , date );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element IMAGING_DATE" );
    goto ERREUR;
    }

  /* sous element IMAGING_TIME */
  
  cr = cai_WriteElementXML ( des , "IMAGING_TIME" , heure );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element IMAGING_TIME" );
    goto ERREUR;
    }

  /* sous element MISSION */
  
  cr = cai_WriteElementXML ( des , "MISSION" , satellite );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element MISSION" );
    goto ERREUR;
    }

  /* sous element MISSION_INDEX */
  
  sprintf ( sat_ind , "%d" , satellite_index );

  cr = cai_WriteElementXML ( des , "MISSION_INDEX" , sat_ind );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element MISSION_INDEX" );
    goto ERREUR;
    }

  /* sous element INSTRUMENT */
  
  cr = cai_WriteElementXML ( des , "INSTRUMENT" , instrument );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element INSTRUMENT" );
    goto ERREUR;
    }

  /* sous element INSTRUMENT_INDEX */
  
  sprintf ( ins_ind , "%d" , instrument_index );
  
  cr = cai_WriteElementXML ( des , "INSTRUMENT_INDEX" , ins_ind );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element INSTRUMENT_INDEX" );
    goto ERREUR;
    }

  /* sous element SENSOR_CODE */
  
  sprintf ( valeur , "%c" , codecapt );
  cr = cai_WriteElementXML ( des , "SENSOR_CODE" , valeur );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SENSOR_CODE" );
    goto ERREUR;
    }

  /* sous element SCENE_PROCESSING_LEVEL */
  
  cr = cai_WriteElementXML ( des , "SCENE_PROCESSING_LEVEL" , niv_trait );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SCENE_PROCESSING_LEVEL" );
    goto ERREUR;
    }

  /* sous element INCIDENCE_ANGLE */
  
  if ( angle_in[0] == 'L' )
    {
    sprintf ( valeur , "-%s" , &angle_in[1] );
    }
  else if (angle_in[0] != '\0')
    {
    sprintf ( valeur , "%s" , &angle_in[1] );
    }
   else
	sprintf(valeur, "\0");

  cr = cai_WriteElementXML ( des , "INCIDENCE_ANGLE" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element INCIDENCE_ANGLE" );
    goto ERREUR;
    }

  /* sous element SUN_AZIMUT */
  
  cr = cai_WriteElementXML ( des , "SUN_AZIMUTH" , sunazim );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SUN_AZIMUT" );
    goto ERREUR;
    }

  /* sous element SUN_ELEVATION */
  
  cr = cai_WriteElementXML ( des , "SUN_ELEVATION" , sunelev );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SUN_ELEVATION" );
    goto ERREUR;
    }

  /* sous element Imaging_Parameters */
  
  cr = cai_WriteOpenTagXML ( des , "Imaging_Parameters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Imaging_Parameters" );
    goto ERREUR;
    }

  /* sous element REVOLUTION_NUMBER */
  
  cr = cai_WriteElementXML ( des , "REVOLUTION_NUMBER" , revolution );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element REVOLUTION_NUMBER" );
    goto ERREUR;
    }

  /* sous element COMPRESSION_MODE */
  
  cr = cai_WriteElementXML ( des , "COMPRESSION_MODE" , compression );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COMPRESSION_MODE" );
    goto ERREUR;
    }

  /* sous element DIRECT_PLAYBACK_INDICATOR */
  
  cr = cai_WriteElementXML ( des , "DIRECT_PLAYBACK_INDICATOR" , direct_playback );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DIRECT_PLAYBACK_INDICATOR" );
    goto ERREUR;
    }

  /* sous element REFOCUSING_STEP_NUM */
  
  cr = cai_WriteElementXML ( des , "REFOCUSING_STEP_NUM" , stepnum );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element REFOCUSING_STEP_NUM" );
    goto ERREUR;
    }

  /* sous element COUPLED_MODE_FLAG */
  
  cr = cai_WriteElementXML ( des , "COUPLED_MODE_FLAG" , coupled );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COUPLED_MODE_FLAG" );
    goto ERREUR;
    }

  /* sous element SWATH_MODE */
  
  cr = cai_WriteElementXML ( des , "SWATH_MODE" , "FULL" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SWATH_MODE" );
    goto ERREUR;
    }

 /* Fermeture de l'element Imaging_Parameters */

  cr = cai_WriteCloseTagXML ( des , "Imaging_Parameters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Imaging_Parameters" );
    goto ERREUR;
    }

 /* Fermeture de l'element Scene_Source */

  cr = cai_WriteCloseTagXML ( des , "Scene_Source" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Scene_Source" );
    goto ERREUR;
    }

 /* Fermeture de l'element Source_Information */

  cr = cai_WriteCloseTagXML ( des , "Source_Information" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Source_Information" );
    goto ERREUR;
    }

 /* Fermeture de l'element Dataset_Sources */

  cr = cai_WriteCloseTagXML ( des , "Dataset_Sources" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dataset_Sources" );
    goto ERREUR;
    }

  /* sous element Coordinate_Reference_System */

  cr = cai_WriteOpenTagXML ( des , "Coordinate_Reference_System" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Coordinate_Reference_System" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'attribut version="5.2"              */
/*****************************************************************************/

  /* sous element GEO_TABLES */
    type=0;
  cr = cai_WriteElementXMLWithAttribute ( des , "GEO_TABLES" , "EPSG" , "version" , "5.2",type );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element GEO_TABLES" );
    goto ERREUR;
    }

  /* sous element Horizontal_CS */

  cr = cai_WriteOpenTagXML ( des , "Horizontal_CS" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Horizontal_CS" );
    goto ERREUR;
    }

  /* sous element HORIZONTAL_CS_TYPE */
  
  cr = cai_WriteElementXML ( des , "HORIZONTAL_CS_TYPE" , "GEOGRAPHIC" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element HORIZONTAL_CS_TYPE" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : DIMAP remplissage des tags HORIZONTAL_CS_CODE  */
/*                            et HORIZONTAL_CS_NAME avec des valeurs défauts */
/*****************************************************************************/
  /* sous element HORIZONTAL_CS_CODE */
  
  cr = cai_WriteElementXML ( des , "HORIZONTAL_CS_CODE" , "epsg:4326" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element HORIZONTAL_CS_CODE" );
    goto ERREUR;
    }

  /* sous element HORIZONTAL_CS_NAME */
  
  cr = cai_WriteElementXML ( des , "HORIZONTAL_CS_NAME" , "WGS 84" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element HORIZONTAL_CS_NAME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Horizontal_CS */

  cr = cai_WriteCloseTagXML ( des , "Horizontal_CS" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Horizontal_CS" );
    goto ERREUR;
    }

 /* Fermeture de l'element Coordinate_Reference_System */

  cr = cai_WriteCloseTagXML ( des , "Coordinate_Reference_System" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Coordinate_Reference_System" );
    goto ERREUR;
    }

  /* sous element Raster_CS */

  cr = cai_WriteOpenTagXML ( des , "Raster_CS" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raster_CS" );
    goto ERREUR;
    }

  /* sous element RASTER_CS_TYPE */

  if ( strncmp ( niv_trait , "2A" , 2 ) ==0 )
    {
    cr = cai_WriteElementXML ( des , "RASTER_CS_TYPE" , "CELL" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element RASTER_CS_TYPE" );
      goto ERREUR;
      }
    }
  else
    {
    cr = cai_WriteElementXML ( des , "RASTER_CS_TYPE" , "POINT" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element RASTER_CS_TYPE" );
      goto ERREUR;
      }
    }
    
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element PIXEL_ORIGIN               */
/****************************************************************************/
  
  /* sous element PIXEL_ORIGIN */

  cr = cai_WriteElementXML ( des , "PIXEL_ORIGIN" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PIXEL_ORIGIN" );
    goto ERREUR;
    }
  

  /* Fermeture de l'element Raster_CS */

  cr = cai_WriteCloseTagXML ( des , "Raster_CS" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raster_CS" );
    goto ERREUR;
    }

  /* sous element Geoposition */
  
  cr = cai_WriteOpenTagXML ( des , "Geoposition" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Geoposition" );
    goto ERREUR;
    }

  /* sous element Geoposition_Points */
  
  cr = cai_WriteOpenTagXML ( des , "Geoposition_Points" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Geoposition_Points" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */

  sprintf ( valeur , "%lf" , lon_no );
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */

  sprintf ( valeur , "%lf" , lat_no );
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_X */
/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour TIE_POINTS_DATA_X et   */
/*					TIE_POINTS_DATA_Y                    */
/*****************************************************************************/
     sscanf ( ligc1 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
     sscanf ( colc1 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
 
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */
  
  sprintf ( valeur , "%f" , lon_ne );
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */

  sprintf ( valeur , "%f" , lat_ne );
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }


/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour TIE_POINTS_DATA_X et   */
/*					TIE_POINTS_DATA_Y                    */
/*****************************************************************************/
     sscanf ( ligc2 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
    sscanf ( colc2 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */
  
  sprintf ( valeur , "%lf" , lon_so );

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */
  
  sprintf ( valeur , "%lf" , lat_so );

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }


/*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour TIE_POINTS_DATA_X et   */
/*					TIE_POINTS_DATA_Y                    */
/*****************************************************************************/
 /* sous element TIE_POINTS_DATA_X */

     sscanf ( ligc3 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
    sscanf ( colc3 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);

  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */
  
  sprintf ( valeur , "%lf" , lon_se );

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */
  
  sprintf ( valeur , "%lf" , lat_se );

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }


 /*****************************************************************************/
/* MOD : VERSION : 4.8 : FA : Enlever le signe + pour TIE_POINTS_DATA_X et   */
/*					TIE_POINTS_DATA_Y                    */
/*****************************************************************************/
 /* sous element TIE_POINTS_DATA_X */

     sscanf ( ligc4 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
     sscanf ( colc4 , "%d" ,&ivaleur  );
    sprintf(Valeur,"%d",ivaleur);
 
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , Valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Tie_Point" );
    goto ERREUR;
    }

 /* Fermeture de l'element Geoposition_Points */

  cr = cai_WriteCloseTagXML ( des , "Geoposition_Points" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Geoposition_Points" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Deplacement de Simplified_Location_Model       */
/*****************************************************************************/

  /* sous element Simplified_Location_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Simplified_Location_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Simplified_Location_Model" );
    goto ERREUR;
    }

  /* sous element Direct_Location_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Direct_Location_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Direct_Location_Model" );
    goto ERREUR;
    }

  /* sous element lc_List */

  cr = cai_WriteOpenTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , coeffa );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , coeffb );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , coeffc );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , coeffd );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , coeffe );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , coefff );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* Fermeture de l'element lc_List */

  cr = cai_WriteCloseTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element pc_List */

  cr = cai_WriteOpenTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , coeffap );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , coeffbp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , coeffcp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , coeffdp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , coeffep );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , coefffp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* Fermeture de l'element pc_List */

  cr = cai_WriteCloseTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

 /* Fermeture de l'element Direct_Location_Model */

  cr = cai_WriteCloseTagXML ( des , "Direct_Location_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Direct_Location_Model" );
    goto ERREUR;
    }

  /* sous element Reverse_Location_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Reverse_Location_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Reverse_Location_Model" );
    goto ERREUR;
    }

  /* sous element lc_List */

  cr = cai_WriteOpenTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , rlma );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , rlmb );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , rlmc );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , rlmd );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , rlme );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , rlmf );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element lc" );
    goto ERREUR;
    }

  /* Fermeture de l'element lc_List */

  cr = cai_WriteCloseTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element pc_List */

  cr = cai_WriteOpenTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , rlmap );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , rlmbp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , rlmcp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , rlmdp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , rlmep );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , rlmfp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element pc" );
    goto ERREUR;
    }

  /* Fermeture de l'element pc_List */

  cr = cai_WriteCloseTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

  /* Fermeture de l'element Reverse_Location_Model */

  cr = cai_WriteCloseTagXML ( des , "Reverse_Location_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Reverse_Location_Model" );
    goto ERREUR;
    }

  /* Fermeture de l'element Simplified_Location_Model */

  cr = cai_WriteCloseTagXML ( des , "Simplified_Location_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Simplified_Location_Model" );
    goto ERREUR;
    }

 /* Fermeture de l'element Geoposition */

  cr = cai_WriteCloseTagXML ( des , "Geoposition" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Geoposition" );
    goto ERREUR;
    }

  /* sous element Production */
  
  cr = cai_WriteOpenTagXML ( des , "Production" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Production" );
    goto ERREUR;
    }

  /* sous element DATASET_PRODUCER_NAME */
  
  cr = cai_WriteElementXML ( des , "DATASET_PRODUCER_NAME" , processing_center );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_PRODUCER_NAME" );
    goto ERREUR;
    }
  
/******************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout attribut href="http://www.spotimage.fr"   */
/******************************************************************************/
   
  /* sous element DATASET_PRODUCER_URL */
  type=1;
  cr = cai_WriteElementXMLWithAttribute ( des , "DATASET_PRODUCER_URL" , "" ,
                                  "href" , "http://www.spotimage.fr" ,type);
  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_PRODUCER_URL" );
    goto ERREUR;
    }

  /* sous element DATASET_PRODUCTION_DATE */
  
  sprintf ( valeur , "%.4d-%.2d-%.2d %.2d:%.2d:%.2d" , product_annee ,
            product_mois , product_jour , product_heure , product_min ,
	    product_sec );
  
  cr = cai_WriteElementXML ( des , "DATASET_PRODUCTION_DATE" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATASET_PRODUCTION_DATE" );
    goto ERREUR;
    }

  /* sous element PRODUCT_TYPE */

  cr = cai_WriteElementXML ( des , "PRODUCT_TYPE" , product_type );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PRODUCT_TYPE" );
    goto ERREUR;
    }

  /* sous element PRODUCT_INFO */

  sprintf ( prod_info , "Spot SYSTEM SCENE level %s" , niv_trait );
  
  cr = cai_WriteElementXML ( des , "PRODUCT_INFO" , prod_info );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PRODUCT_INFO" );
    goto ERREUR;
    }

  /* sous element JOB_ID */
  
  cr = cai_WriteElementXML ( des , "JOB_ID" , job_id );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element JOB_ID" );
    goto ERREUR;
    }

  /* sous element Production_Facility */
  
  cr = cai_WriteOpenTagXML ( des , "Production_Facility" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Production_Facility" );
    goto ERREUR;
    }

  /* sous element SOFTWARE_NAME */

  cr = cai_WriteElementXML ( des , "SOFTWARE_NAME" , software_name );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SOFTWARE_NAME" );
    goto ERREUR;
    }

  /* sous element SOFTWARE_VERSION */
  
  cr = cai_WriteElementXML ( des , "SOFTWARE_VERSION" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SOFTWARE_VERSION" );
    goto ERREUR;
    }

  /* sous element PROCESSING_CENTER */
  
  cr = cai_WriteElementXML ( des , "PROCESSING_CENTER" , software_name );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PROCESSING_CENTER" );
    goto ERREUR;
    }

 /* Fermeture de l'element Production_Facility */

  cr = cai_WriteCloseTagXML ( des , "Production_Facility" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Production_Facility" );
    goto ERREUR;
    }

 /* Fermeture de l'element Production */

  cr = cai_WriteCloseTagXML ( des , "Production" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Production" );
    goto ERREUR;
    }
    
  /* sous element Raster_Dimensions */
  
  cr = cai_WriteOpenTagXML ( des , "Raster_Dimensions" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raster_Dimensions" );
    goto ERREUR;
    }

  /* sous element NCOLS */
  
  sprintf ( valeur , "%d" , nb_col );
  cr = cai_WriteElementXML ( des , "NCOLS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element NCOLS" );
    goto ERREUR;
    }

  /* sous element NROWS */
  
  sprintf ( valeur , "%d" , nb_lig );
  cr = cai_WriteElementXML ( des , "NROWS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element NROWS" );
    goto ERREUR;
    }

  /* sous element NBANDS */
  
  sprintf ( valeur , "%d" , nb_bs );
  cr = cai_WriteElementXML ( des , "NBANDS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element NBANDS" );
    goto ERREUR;
    }

 /* Fermeture de l'element Raster_Dimensions */

  cr = cai_WriteCloseTagXML ( des , "Raster_Dimensions" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raster_Dimensions" );
    goto ERREUR;
    }

  /* sous element Raster_Encoding */
  
  cr = cai_WriteOpenTagXML ( des , "Raster_Encoding" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raster_Encoding" );
    goto ERREUR;
    }

  /* sous element NBITS */
  
  cr = cai_WriteElementXML ( des , "NBITS" , "8" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element NBITS" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element DATA_TYPE                  */
/****************************************************************************/

  /* sous element DATA_TYPE */

  cr = cai_WriteElementXML ( des , "DATA_TYPE" , "UNSIGNED" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATA_TYPE" );
    goto ERREUR;
    }

  /* sous element BYTEORDER */
  
  cr = cai_WriteElementXML ( des , "BYTEORDER" , "M" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BYTEORDER" );
    goto ERREUR;
    }

  /* sous element BANDS_LAYOUT */
  
  cr = cai_WriteElementXML ( des , "BANDS_LAYOUT" , band_layout );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BANDS_LAYOUT" );
    goto ERREUR;
    }

   /* sous element SKIPBYTES */
  
  cr = cai_WriteElementXML ( des , "SKIPBYTES" , "0" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SKIPBYTES" );
    goto ERREUR;
    }

 /* sous element COMPRESSION_NAME */
  
  cr = cai_WriteElementXML ( des , "COMPRESSION_NAME" , "NONE" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COMPRESSION_NAME" );
    goto ERREUR;
    }

 /* Fermeture de l'element Raster_Encoding */

  cr = cai_WriteCloseTagXML ( des , "Raster_Encoding" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raster_Encoding" );
    goto ERREUR;
    }

  /* sous element Data_Processing */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Processing" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Processing" );
    goto ERREUR;
    }

  /* sous element PROCESSING_LEVEL */
  
  cr = cai_WriteElementXML ( des , "PROCESSING_LEVEL" , niv_trait );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PROCESSING_LEVEL" );
    goto ERREUR;
    }

  /* sous element GEOMETRIC_PROCESSING */

  if ( strncmp ( niv_trait , "2A" , 2 ) == 0)
    {
    cr = cai_WriteElementXML ( des , "GEOMETRIC_PROCESSING" , "CARTO" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element GEOMETRIC_PROCESSING" );
      goto ERREUR;
      }
    }
  else
  if ( ( strncmp ( niv_trait , "1A" , 2 ) == 0) ||
       ( strncmp ( niv_trait , "0" , 1 ) == 0) )
    {
    cr = cai_WriteElementXML ( des , "GEOMETRIC_PROCESSING" , "RAW" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element GEOMETRIC_PROCESSING" );
      goto ERREUR;
      }
    }
  else
    {
    cr = cai_WriteElementXML ( des , "GEOMETRIC_PROCESSING" , "SYSTEM" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element GEOMETRIC_PROCESSING" );
      goto ERREUR;
      }
    }
  
  /* sous element RADIOMETRIC_PROCESSING */
  
  if ( strncmp ( niv_trait , "0" , 1 ) != 0)
    {
    cr = cai_WriteElementXML ( des , "RADIOMETRIC_PROCESSING" , "SYSTEM" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element RADIOMETRIC_PROCESSING" );
      goto ERREUR;
      }
    }
  else
    {
    cr = cai_WriteElementXML ( des , "RADIOMETRIC_PROCESSING" , "NONE" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element RADIOMETRIC_PROCESSING" );
      goto ERREUR;
      }
    }
  
  /* sous element SPECTRAL_PROCESSING */

  if ( ( codecapt == 'I' ) && ( nb_bs == 3 ) )
    {
    cr = cai_WriteElementXML ( des , "SPECTRAL_PROCESSING" , "X" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element SPECTRAL_PROCESSING" );
      goto ERREUR;
      }
    }
  else
  if ( ( codecapt == 'M' ) && ( nb_bs == 3 ) )
    {
    cr = cai_WriteElementXML ( des , "SPECTRAL_PROCESSING" , "M+X" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element SPECTRAL_PROCESSING" );
      goto ERREUR;
      }
    }
  else
  if ( ( codecapt == 'M' ) && ( nb_bs == 4 ) )
    {
    cr = cai_WriteElementXML ( des , "SPECTRAL_PROCESSING" , "M+I" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element SPECTRAL_PROCESSING" );
      goto ERREUR;
      }
    }
  
  /* sous element Processing_Options */
  
  cr = cai_WriteOpenTagXML ( des , "Processing_Options" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Processing_Options" );
    goto ERREUR;
    }

  /* sous element MEAN_RECTIFICATION_ELEVATION */
  
  if ( strncmp ( niv_trait , "1B" , 2 ) == 0)
    {
    cr = cai_WriteElementXML ( des , "MEAN_RECTIFICATION_ELEVATION" , "0" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element MEAN_RECTIFICATION_ELEVATION" );
      goto ERREUR;
      }
    }
  else
  if ( strncmp ( niv_trait , "2A" , 2 ) == 0)  
    {
    cr = cai_WriteElementXML ( des , "MEAN_RECTIFICATION_ELEVATION" , mean );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element MEAN_RECTIFICATION_ELEVATION" );
      goto ERREUR;
      }
    }
  /* sous element Dynamic_Stretch */
  
  cr = cai_WriteOpenTagXML ( des , "Dynamic_Stretch" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dynamic_Stretch" );
    goto ERREUR;
    }

  for ( i = 0 ; i < nb_bs ; i++ )
  {
  /* sous element Thresholds */
  
  cr = cai_WriteOpenTagXML ( des , "Thresholds" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Thresholds" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
  sprintf ( valeur , "%d" , (i+1) );
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
    goto ERREUR;
    }

  /* sous element LOW_THRESHOLD */
  
  cr = cai_WriteElementXML ( des , "LOW_THRESHOLD" , lowtresh[i] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LOW_THRESHOLD" );
    goto ERREUR;
    }

  /* sous element HIGH_THRESHOLD */
  
  cr = cai_WriteElementXML ( des , "HIGH_THRESHOLD" , uptresh[i] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element HIGH_THRESHOLD" );
    goto ERREUR;
    }

 /* Fermeture de l'element Thresholds */

  cr = cai_WriteCloseTagXML ( des , "Thresholds" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Thresholds" );
    goto ERREUR;
    }
  }
 /* Fermeture de l'element Dynamic_Stretch */

  cr = cai_WriteCloseTagXML ( des , "Dynamic_Stretch" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dynamic_Stretch" );
    goto ERREUR;
    }

  /* sous element LINE_SHIFT */
  
  cr = cai_WriteElementXML ( des , "LINE_SHIFT" , shval );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LINE_SHIFT" );
    goto ERREUR;
    }

  if ( ( strcmp ( niv_trait , "1A" ) != 0) &&
       ( strcmp ( niv_trait , "0" ) != 0) )
  {
  /* sous element RESAMPLING_METHOD */
  
  cr = cai_WriteElementXML ( des , "RESAMPLING_METHOD" , resampling );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element RESAMPLING_METHOD" );
    goto ERREUR;
    }
  
  /* sous element Sampling_Step */
  
  cr = cai_WriteOpenTagXML ( des , "Sampling_Step" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Sampling_Step" );
    goto ERREUR;
    }

  /* sous element SAMPLING_STEP_X */
  
  cr = cai_WriteElementXML ( des , "SAMPLING_STEP_X" , taille_pixlig );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SAMPLING_STEP_X" );
    goto ERREUR;
    }

  /* sous element SAMPLING_STEP_Y */
  
  cr = cai_WriteElementXML ( des , "SAMPLING_STEP_Y" , taille_pixcol );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SAMPLING_STEP_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Sampling_Step */

  cr = cai_WriteCloseTagXML ( des , "Sampling_Step" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Sampling_Step" );
    goto ERREUR;
    }
  }
  /* sous element SWIR_BAND_REGISTRATION_FLAG */
  
  if ( swir == 1 )
    cr = cai_WriteElementXML ( des , "SWIR_BAND_REGISTRATION_FLAG" , "Y" );
  else
    cr = cai_WriteElementXML ( des , "SWIR_BAND_REGISTRATION_FLAG" , "N" );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SWIR_BAND_REGISTRATION_FLAG" );
    goto ERREUR;
    }

  /* Fermeture de l'element Processing_Options */

  cr = cai_WriteCloseTagXML ( des , "Processing_Options" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Processing_Options" );
    goto ERREUR;
    }

  /* Fermeture de l'element Data_Processing */

  cr = cai_WriteCloseTagXML ( des , "Data_Processing" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Processing" );
    goto ERREUR;
    }

  /* sous element Data_Access */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Access" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Access" );
    goto ERREUR;
    }

  /* sous element DATA_FILE_ORGANISATION */
  
  cr = cai_WriteElementXML ( des , "DATA_FILE_ORGANISATION" , "BAND_COMPOSITE" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATA_FILE_ORGANISATION" );
    goto ERREUR;
    }

  /* sous element DATA_FILE_FORMAT */
  
  cr = cai_WriteElementXML ( des , "DATA_FILE_FORMAT" , "RAW" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATA_FILE_FORMAT" );
    goto ERREUR;
    }

  /* sous element Data_File */
  
  cr = cai_WriteOpenTagXML ( des , "Data_File" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_File" );
    goto ERREUR;
    }

  /* sous element DATA_FILE_PATH */
  
  cr = cai_WriteElementXML ( des , "DATA_FILE_PATH" , "./" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATA_FILE_PATH" );
    goto ERREUR;
    }

 /* Fermeture de l'element Data_File */

  cr = cai_WriteCloseTagXML ( des , "Data_File" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_File" );
    goto ERREUR;
    }

 /* Fermeture de l'element Data_Access */

  cr = cai_WriteCloseTagXML ( des , "Data_Access" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Access" );
    goto ERREUR;
    }

  /* sous element Image_Display */
  
  cr = cai_WriteOpenTagXML ( des , "Image_Display" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Image_Display" );
    goto ERREUR;
    }

  /* sous element Band_Display_Order */
  
  cr = cai_WriteOpenTagXML ( des , "Band_Display_Order" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Display_Order" );
    goto ERREUR;
    }

  if ( strncasecmp ( mode_spect , "PM" , 2) == 0)
    {    

    /* sous element RED_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "RED_CHANNEL" , "1" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element RED_CHANNEL" );
      goto ERREUR;
      }    
    /* sous element GREEN_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "GREEN_CHANNEL" , "1" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element GREEN_CHANNEL" );
      goto ERREUR;
      }    
    /* sous element BLUE_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "BLUE_CHANNEL" , "1" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element BLUE_CHANNEL" );
      goto ERREUR;
      }    
    }
  else
  if ( ( strncasecmp ( mode_spect , "XS" , 2) == 0)|| 
       ( strncasecmp ( mode_spect , "XI" , 2) == 0) )
    {
    /* sous element RED_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "RED_CHANNEL" , "3" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element RED_CHANNEL" );
      goto ERREUR;
      }    
    /* sous element GREEN_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "GREEN_CHANNEL" , "2" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element GREEN_CHANNEL" );
      goto ERREUR;
      }    

    /* sous element BLUE_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "BLUE_CHANNEL" , "1" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element BLUE_CHANNEL" );
      goto ERREUR;
      }    
    }
  
 /* Fermeture de l'element Band_Display_Order */

  cr = cai_WriteCloseTagXML ( des , "Band_Display_Order" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Display_Order" );
    goto ERREUR;
    }

  /* sous element Special_Value */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value" );
    goto ERREUR;
    }

  /* sous element SPECIAL_VALUE_INDEX */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_INDEX" , "0" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SPECIAL_VALUE_INDEX" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA : SPECIAL_VALUE_TEXT  NODATA a la place NO_DATA  */
/*****************************************************************************/  
  /* sous element SPECIAL_VALUE_TEXT */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_TEXT" , "NODATA" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SPECIAL_VALUE_TEXT" );
    goto ERREUR;
    }    
   
  /* sous element Special_Value_Color */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value_Color" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value_Color" );
    goto ERREUR;
    }
  
  /* sous element RED_LEVEL */
 
  cr = cai_WriteElementXML ( des , "RED_LEVEL" , "0" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element RED_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element GREEN_LEVEL */
 
  cr = cai_WriteElementXML ( des , "GREEN_LEVEL" , "0" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element GREEN_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element BLUE_LEVEL */
 
  cr = cai_WriteElementXML ( des , "BLUE_LEVEL" , "0" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BLUE_LEVEL" );
    goto ERREUR;
    }    
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Rajouter ALPHA_LEVEL unqiement pour SPOT4 MIR  */
/*****************************************************************************/ 

   if ((nb_bs == 4)&&(satellite_index==4))
   {
  /* sous element ALPHA_LEVEL */
 
      cr = cai_WriteElementXML ( des , "ALPHA_LEVEL" , "0" );
      if ( cr == CAI_KO )
      {
        sprintf ( CAI_ERREUR , "Erreur ecriture Element ALPHA_LEVEL" );
        goto ERREUR;
       }    
   } 

  /* Fermeture de l'element Special_Value_Color */

  cr = cai_WriteCloseTagXML ( des , "Special_Value_Color" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value_Color" );
    goto ERREUR;
    }

  /* Fermeture de l'element Special_Value */

  cr = cai_WriteCloseTagXML ( des , "Special_Value" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value" );
    goto ERREUR;
    }

  /* sous element Special_Value */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value" );
    goto ERREUR;
    }
  
  /* sous element SPECIAL_VALUE_INDEX */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_INDEX" , "255" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SPECIAL_VALUE_INDEX" );
    goto ERREUR;
    }    
 
/******************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement de SATURATE par SATURATED          */
/******************************************************************************/
 
  /* sous element SPECIAL_VALUE_TEXT */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_TEXT" , "SATURATED" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SPECIAL_VALUE_TEXT" );
    goto ERREUR;
    }    
  
  /* sous element Special_Value_Color */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value_Color" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value_Color" );
    goto ERREUR;
    }
 
/******************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement de 255 par 1 dans les element LEVEL*/
/******************************************************************************/
 
  /* sous element RED_LEVEL */
 
  cr = cai_WriteElementXML ( des , "RED_LEVEL" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element RED_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element GREEN_LEVEL */
 
  cr = cai_WriteElementXML ( des , "GREEN_LEVEL" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element GREEN_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element BLUE_LEVEL */
 
  cr = cai_WriteElementXML ( des , "BLUE_LEVEL" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BLUE_LEVEL" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Rajouter ALPHA_LEVEL uniquement pour SPOT4 MIR */
/*****************************************************************************/ 

   if ((nb_bs == 4)&&(satellite_index==4))
   {
  /* sous element ALPHA_LEVEL */
 
      cr = cai_WriteElementXML ( des , "ALPHA_LEVEL" , "1" );
      if ( cr == CAI_KO )
      {
        sprintf ( CAI_ERREUR , "Erreur ecriture Element ALPHA_LEVEL" );
        goto ERREUR;
       }    
   } 

  /* Fermeture de l'element Special_Value_Color */

  cr = cai_WriteCloseTagXML ( des , "Special_Value_Color" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value_Color" );
    goto ERREUR;
    }

  /* Fermeture de l'element Special_Value */

  cr = cai_WriteCloseTagXML ( des , "Special_Value" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Special_Value" );
    goto ERREUR;
    }

  /* Fermeture de l'element Image_Display */

  cr = cai_WriteCloseTagXML ( des , "Image_Display" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Image_Display" );
    goto ERREUR;
    }

  /* sous element Image_Interpretation */
  
  cr = cai_WriteOpenTagXML ( des , "Image_Interpretation" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Image_Interpretation" );
    goto ERREUR;
    }

  for ( j = 0; j < nb_bs ; j++ )
    {
    /* sous element Spectral_Band_Info */
  
    cr = cai_WriteOpenTagXML ( des , "Spectral_Band_Info" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element Spectral_Band_Info" );
      goto ERREUR;
      }

    /* sous element BAND_INDEX */
  
    sprintf ( valeur , "%d" , (j+1) );
  
    cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
      goto ERREUR;
      }
    
    /* sous element BAND_DESCRIPTION */

    if ( nb_bs == 1 )
      sprintf ( valeur , "PAN" );
    else
    if ( ( ( j == 0) || ( j == 1 ) ) || ( j == 2 ) )
      sprintf ( valeur , "XS%d" , (j+1) );
    else
    if ( j == 3 )
      sprintf ( valeur , "SWIR" );
     
/******************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement de BAND par BAND_DESCRIPTION       */
/******************************************************************************/
 
    cr = cai_WriteElementXML ( des , "BAND_DESCRIPTION" , valeur );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_DESCRIPTION" );
      goto ERREUR;
      }
 
   /* sous element PHYSICAL_CALIBRATION_DATE */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_CALIBRATION_DATE" , datecalib );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PHYSICAL_CALIBRATION_DATE" );
    goto ERREUR;
    }

  /* sous element PHYSICAL_BIAIS */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_BIAIS" , gains_b[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PHYSICAL_BIAIS" );
    goto ERREUR;
    }

  /* sous element PHYSICAL_GAIN */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_GAIN" , gains_a[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PHYSICAL_GAIN" );
    goto ERREUR;
    }

/******************************************************************************/
/* MOD : VERSION : 4.7 : DM : ajout de l'unite dans PHYSICAL_UNIT             */
/******************************************************************************/
 
  /* sous element PHYSICAL_UNIT */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_UNIT" , "equivalent radiance (W.m-2.Sr-1.um-1)" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PHYSICAL_UNIT" );
    goto ERREUR;
    }

  /* Fermeture de l'element Spectral_Band_Info */

  cr = cai_WriteCloseTagXML ( des , "Spectral_Band_Info" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Spectral_Band_Info" );
    goto ERREUR;
    }
  }

  /* Fermeture de l'element Image_Interpretation */

  cr = cai_WriteCloseTagXML ( des , "Image_Interpretation" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Image_Interpretation" );
    goto ERREUR;
    }

  /* sous element Data_Strip */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Strip" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Strip" );
    goto ERREUR;
    }

  /* sous element Satellite_Time */
  
  cr = cai_WriteOpenTagXML ( des , "Satellite_Time" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Satellite_Time" );
    goto ERREUR;
    }

  /* sous element UT_DATE */
  
  sprintf ( valeur , "%.7d %.5d.000000" , utdatej , utdates );
  cr = cai_WriteElementXML ( des , "UT_DATE" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element UT_DATE" );
    goto ERREUR;
    }

  /* sous element CLOCK_VALUE */
  
  cr = cai_WriteElementXML ( des , "CLOCK_VALUE" , clockvalue );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element CLOCK_VALUE" );
    goto ERREUR;
    }

  /* sous element CLOCK_PERIOD */
  
  if ( clockperiod[0] != '\0' )
     sscanf ( clockperiod , "%lf" , &period );
  else
	period = 0.0;
  pico =0.000000000001;
  period = period * pico;
  
  sprintf ( valeur , "%.12lf" , period );
  
  cr = cai_WriteElementXML ( des , "CLOCK_PERIOD" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element CLOCK_PERIOD" );
    goto ERREUR;
    }

  /* sous element BOARD_TIME */
  
  cr = cai_WriteElementXML ( des , "BOARD_TIME" , boardtime );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BOARD_TIME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Satellite_Time */

  cr = cai_WriteCloseTagXML ( des , "Satellite_Time" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Satellite_Time" );
    goto ERREUR;
    }

  /* sous element Ephemeris */
  
  cr = cai_WriteOpenTagXML ( des , "Ephemeris" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Ephemeris" );
    goto ERREUR;
    }

  /* sous element SATELLITE_ALTITUDE */
  
  cr = cai_WriteElementXML ( des , "SATELLITE_ALTITUDE" , sat_alt );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SATELLITE_ALTITUDE" );
    goto ERREUR;
    }

  /* sous element NADIR_LAT */
  
  sprintf ( valeur , "%lf" , lat_nad );
  cr = cai_WriteElementXML ( des , "NADIR_LAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element NADIR_LAT" );
    goto ERREUR;
    }

  /* sous element NADIR_LON */
  
  sprintf ( valeur , "%lf" , long_nad );
  cr = cai_WriteElementXML ( des , "NADIR_LON" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element NADIR_LON" );
    goto ERREUR;
    }

  /* sous element Points */
  
  cr = cai_WriteOpenTagXML ( des , "Points" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Points" );
    goto ERREUR;
    }

  for ( j = 0; j < 8 ; j++ )
  {
  /* sous element Point */
  
  cr = cai_WriteOpenTagXML ( des , "Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Point" );
    goto ERREUR;
    }

  /* sous element Location */
  
  cr = cai_WriteOpenTagXML ( des , "Location" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Location" );
    goto ERREUR;
    }

  /* sous element X */
  
  sprintf ( valeur , "%.3lf" , X_loc[j]*1000.0 );
  cr = cai_WriteElementXML ( des , "X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element X" );
    goto ERREUR;
    }

  /* sous element Y */
  
  sprintf ( valeur , "%.3lf" , Y_loc[j]*1000.0 );
  cr = cai_WriteElementXML ( des , "Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Y" );
    goto ERREUR;
    }

  /* sous element Z */
  
  sprintf ( valeur , "%.3lf" , Z_loc[j]*1000.0 );
  cr = cai_WriteElementXML ( des , "Z" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Z" );
    goto ERREUR;
    }

 /* Fermeture de l'element Location */

  cr = cai_WriteCloseTagXML ( des , "Location" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Location" );
    goto ERREUR;
    }

  /* sous element Velocity */
  
  cr = cai_WriteOpenTagXML ( des , "Velocity" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Velocity" );
    goto ERREUR;
    }

  /* sous element X */
  
  sprintf ( valeur , "%.3lf" , X_vel[j]*1000.0 );
  cr = cai_WriteElementXML ( des , "X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element X" );
    goto ERREUR;
    }

  /* sous element Y */
  
  sprintf ( valeur , "%.3lf" , Y_vel[j]*1000.0 );
  cr = cai_WriteElementXML ( des , "Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Y" );
    goto ERREUR;
    }

  /* sous element Z */
  
  sprintf ( valeur , "%.3lf" , Z_vel[j]*1000.0 );
  cr = cai_WriteElementXML ( des , "Z" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Z" );
    goto ERREUR;
    }

 /* Fermeture de l'element Velocity */

  cr = cai_WriteCloseTagXML ( des , "Velocity" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Velocity" );
    goto ERREUR;
    }

  /* sous element TIME */
  
  cr = cai_WriteElementXML ( des , "TIME" , time[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Point */

  cr = cai_WriteCloseTagXML ( des , "Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Point" );
    goto ERREUR;
    }
  }
  
  if ( X_loc[8] != 0.0 )
    {
  /* sous element Point */
  
  cr = cai_WriteOpenTagXML ( des , "Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Point" );
    goto ERREUR;
    }

  /* sous element Location */
  
  cr = cai_WriteOpenTagXML ( des , "Location" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Location" );
    goto ERREUR;
    }

  /* sous element X */
  
  sprintf ( valeur , "%.3lf" , X_loc[8] );
  
  cr = cai_WriteElementXML ( des , "X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element X" );
    goto ERREUR;
    }

  /* sous element Y */
  
  sprintf ( valeur , "%.3lf" , Y_loc[8] );
  cr = cai_WriteElementXML ( des , "Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Y" );
    goto ERREUR;
    }

  /* sous element Z */
  
  sprintf ( valeur , "%.3lf" , Z_loc[8] );
  cr = cai_WriteElementXML ( des , "Z" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Z" );
    goto ERREUR;
    }

 /* Fermeture de l'element Location */

  cr = cai_WriteCloseTagXML ( des , "Location" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Location" );
    goto ERREUR;
    }

  /* sous element Velocity */
  
  cr = cai_WriteOpenTagXML ( des , "Velocity" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Velocity" );
    goto ERREUR;
    }

  /* sous element X */
  
  sprintf ( valeur , "%.3lf" , X_vel[8]*1000.0 );
  cr = cai_WriteElementXML ( des , "X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element X" );
    goto ERREUR;
    }

  /* sous element Y */
  
  sprintf ( valeur , "%lf" , Y_vel[8]*1000.0 );
  cr = cai_WriteElementXML ( des , "Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Y" );
    goto ERREUR;
    }

  /* sous element Z */
  
  sprintf ( valeur , "%.3lf" , Z_vel[8]*1000.0 );
  cr = cai_WriteElementXML ( des , "Z" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Z" );
    goto ERREUR;
    }

  /* Fermeture de l'element Velocity */

  cr = cai_WriteCloseTagXML ( des , "Velocity" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Velocity" );
    goto ERREUR;
    }

  /* sous element TIME */
  
  cr = cai_WriteElementXML ( des , "TIME" , time[8] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Point */

  cr = cai_WriteCloseTagXML ( des , "Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Point" );
    goto ERREUR;
    }

  }
  /* Fermeture de l'element Points */

  cr = cai_WriteCloseTagXML ( des , "Points" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Points" );
    goto ERREUR;
    }

  /* sous element DORIS_USED */
  
  cr = cai_WriteElementXML ( des , "DORIS_USED" , dorisused );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DORIS_USED" );
    goto ERREUR;
    }

  if ( satellite_index == 4 )
    {
  /* sous element Orbital_Bulletin */
  
  cr = cai_WriteOpenTagXML ( des , "Orbital_Bulletin" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Orbital_Bulletin" );
    goto ERREUR;
    }

  /* sous element UT_DATE */
  
  sprintf ( valeur , "%.7d %.5d.000000" , jourjul , datejul );
  cr = cai_WriteElementXML ( des , "UT_DATE" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element UT_DATE" );
    goto ERREUR;
    }

  /* sous element ELLIPSE_AXIS */
  
  cr = cai_WriteElementXML ( des , "ELLIPSE_AXIS" , axis );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ELLIPSE_AXIS" );
    goto ERREUR;
    }

  /* sous element ELLIPSE_ECCENTRICITY_VECTOR_EX */
  
  cr = cai_WriteElementXML ( des , "ELLIPSE_ECCENTRICITY_VECTOR_EX" , ex );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ELLIPSE_ECCENTRICITY_VECTOR_EX" );
    goto ERREUR;
    }

  /* sous element ELLIPSE_ECCENTRICITY_VECTOR_EY */
  
  cr = cai_WriteElementXML ( des , "ELLIPSE_ECCENTRICITY_VECTOR_EY" , ey );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ELLIPSE_ECCENTRICITY_VECTOR_EY" );
    goto ERREUR;
    }

  /* sous element ELLIPSE_INCLINATION */
  
  cr = cai_WriteElementXML ( des , "ELLIPSE_INCLINATION" , inclin );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ELLIPSE_INCLINATION" );
    goto ERREUR;
    }

  /* sous element RIGHT_ASCENSION */
  
  cr = cai_WriteElementXML ( des , "RIGHT_ASCENSION" , ascension );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element RIGHT_ASCENSION" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'orthographe sur LATITUDE_ARGUMENT     */
/*			      et Look_Angles				     */
/*****************************************************************************/

  /* sous element LATITUDE_ARGUMENT */
  
  cr = cai_WriteElementXML ( des , "LATITUDE_ARGUMENT" , latarg );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LATITUDE_ARGUMENT" );
    goto ERREUR;
    }

  /* sous element P7 */
  
  cr = cai_WriteElementXML ( des , "P7" , p7 );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element P7" );
    goto ERREUR;
    }

  /* sous element P8 */
  
  cr = cai_WriteElementXML ( des , "P8" , p8 );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element P8" );
    goto ERREUR;
    }

  /* sous element P9 */
  
  cr = cai_WriteElementXML ( des , "P9" , p9 );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element P9" );
    goto ERREUR;
    }

  /* sous element P10 */
  
  cr = cai_WriteElementXML ( des , "P10" , p10 );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element P10" );
    goto ERREUR;
    }

  /* Fermeture de l'element Orbital_Bulletin */

  cr = cai_WriteCloseTagXML ( des , "Orbital_Bulletin" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Orbital_Bulletin" );
    goto ERREUR;
    }
    
  }

  /* Fermeture de l'element Ephemeris */

  cr = cai_WriteCloseTagXML ( des , "Ephemeris" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Ephemeris" );
    goto ERREUR;
    }

  /* sous element Satellite_Attitudes */
  
  cr = cai_WriteOpenTagXML ( des , "Satellite_Attitudes" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Satellite_Attitudes" );
    goto ERREUR;
    }

  /* sous element Raw_Attitudes */
  
  cr = cai_WriteOpenTagXML ( des , "Raw_Attitudes" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raw_Attitudes" );
    goto ERREUR;
    }

  /* sous element Aocs_Attitude */
  
  cr = cai_WriteOpenTagXML ( des , "Aocs_Attitude" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Aocs_Attitude" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Angles_List                 */
/*****************************************************************************/

  /* sous element Angles_List */
  
  cr = cai_WriteOpenTagXML ( des , "Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angles_List" );
    goto ERREUR;
    }

  for ( j = 0; j < 2 ; j++ )
  {
  /* sous element Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angles" );
    goto ERREUR;
    }

  /* sous element TIME */
  
  cr = cai_WriteElementXML ( des , "TIME" , timeyaw[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIME" );
    goto ERREUR;
    }

  /* sous element YAW */
  
  cr = cai_WriteElementXML ( des , "YAW" , yaw_angle[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element YAW" );
    goto ERREUR;
    }

  /* sous element PITCH */
  
  cr = cai_WriteElementXML ( des , "PITCH" , pitch_angle[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PITCH" );
    goto ERREUR;
    }

  /* sous element ROLL */
  
  cr = cai_WriteElementXML ( des , "ROLL" , roll_angle[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ROLL" );
    goto ERREUR;
    }

  /* sous element OUT_OF_RANGE */
  
  cr = cai_WriteElementXML ( des , "OUT_OF_RANGE" , "N" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element OUT_OF_RANGE" );
    goto ERREUR;
    }

  /* Fermeture de l'element Angles */

  cr = cai_WriteCloseTagXML ( des , "Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angles" );
    goto ERREUR;
    }
  }
 
  /* Fermeture de l'element Angles_List */

  cr = cai_WriteCloseTagXML ( des , "Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angles_List" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Angular_Speeds_List         */
/*****************************************************************************/

  /* sous element Angular_Speeds_List */
 
  cr = cai_WriteOpenTagXML ( des , "Angular_Speeds_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angular_Speeds_List" );
    goto ERREUR;
    }
 
  for ( j = 0; j < 72 ; j++ )
  {
  /* sous element Angular_Speeds */
  
  cr = cai_WriteOpenTagXML ( des , "Angular_Speeds" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angular_Speeds" );
    goto ERREUR;
    }

  /* sous element TIME */
  
  cr = cai_WriteElementXML ( des , "TIME" , time_angle[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIME" );
    goto ERREUR;
    }

  /* sous element YAW */
  
  cr = cai_WriteElementXML ( des , "YAW" , yaw[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element YAW" );
    goto ERREUR;
    }

  /* sous element PITCH */
  
  cr = cai_WriteElementXML ( des , "PITCH" , pitch[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PITCH" );
    goto ERREUR;
    }

  /* sous element ROLL */
  
  cr = cai_WriteElementXML ( des , "ROLL" , roll[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ROLL" );
    goto ERREUR;
    }

  /* sous element OUT_OF_RANGE */
  
  cr = cai_WriteElementXML ( des , "OUT_OF_RANGE" , outofrange );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element OUT_OF_RANGE" );
    goto ERREUR;
    }

  /* Fermeture de l'element Angular_Speeds */

  cr = cai_WriteCloseTagXML ( des , "Angular_Speeds" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angular_Speeds" );
    goto ERREUR;
    }
  }

  if ( strcmp ( yaw[72] , "" ) != 0)
   {
     /* sous element Angular_Speeds */
  
  cr = cai_WriteOpenTagXML ( des , "Angular_Speeds" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angular_Speeds" );
    goto ERREUR;
    }

  /* sous element TIME */
  
  cr = cai_WriteElementXML ( des , "TIME" , time_angle[72] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIME" );
    goto ERREUR;
    }

  /* sous element YAW */
  
  cr = cai_WriteElementXML ( des , "YAW" , yaw[72] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element YAW" );
    goto ERREUR;
    }

  /* sous element PITCH */
  
  cr = cai_WriteElementXML ( des , "PITCH" , pitch[72] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PITCH" );
    goto ERREUR;
    }

  /* sous element ROLL */
  
  cr = cai_WriteElementXML ( des , "ROLL" , roll[72] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element ROLL" );
    goto ERREUR;
    }

  /* sous element OUT_OF_RANGE */
  
  cr = cai_WriteElementXML ( des , "OUT_OF_RANGE" , outofrange );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element OUT_OF_RANGE" );
    goto ERREUR;
    }

  /* Fermeture de l'element Angular_Speeds */

  cr = cai_WriteCloseTagXML ( des , "Angular_Speeds" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angular_Speeds" );
    goto ERREUR;
    }
  }

  /* Fermeture de l'element Angular_Speeds_List */

  cr = cai_WriteCloseTagXML ( des , "Angular_Speeds_List" ); 
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Angular_Speeds_List" );
    goto ERREUR;
    }

   /* Fermeture de l'element Aocs_Attitude */

  cr = cai_WriteCloseTagXML ( des , "Aocs_Attitude" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Aocs_Attitude" );
    goto ERREUR;
    }

  /* Fermeture de l'element Raw_Attitudes */

  cr = cai_WriteCloseTagXML ( des , "Raw_Attitudes" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Raw_Attitudes" );
    goto ERREUR;
    }

  /* Fermeture de l'element Satellite_Attitudes */

  cr = cai_WriteCloseTagXML ( des , "Satellite_Attitudes" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Satellite_Attitudes" );
    goto ERREUR;
    }

  /* sous element Sensor_Configuration */
  
  cr = cai_WriteOpenTagXML ( des , "Sensor_Configuration" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Sensor_Configuration" );
    goto ERREUR;
    }

  /* sous element Time_Stamp */
  
  cr = cai_WriteOpenTagXML ( des , "Time_Stamp" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Time_Stamp " );
    goto ERREUR;
    }

  /* sous element LINE_PERIOD */
  
  sprintf ( valeur , "%.12lf" , periodeligne );
  cr = cai_WriteElementXML ( des , "LINE_PERIOD" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LINE_PERIOD" );
    goto ERREUR;
    }

  /* sous element SCENE_CENTER_TIME */
  
  cr = cai_WriteElementXML ( des , "SCENE_CENTER_TIME" , time_scenecenter );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SCENE_CENTER_TIME" );
    goto ERREUR;
    }

  /* sous element SCENE_CENTER_LINE */
  
  sprintf ( valeur , "%d" , nbc_c );
  cr = cai_WriteElementXML ( des , "SCENE_CENTER_LINE" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SCENE_CENTER_LINE" );
    goto ERREUR;
    }

  /* sous element SCENE_CENTER_COL */
  
  sprintf ( valeur , "%d" , nbl_c );
  cr = cai_WriteElementXML ( des , "SCENE_CENTER_COL" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SCENE_CENTER_COL" );
    goto ERREUR;
    }

  /* Fermeture de l'element Time_Stamp */

  cr = cai_WriteCloseTagXML ( des , "Time_Stamp" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Time_Stamp" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Instrument_look_Angles_List */
/*****************************************************************************/

  /* sous element Instrument_Look_Angles_List */
  
  cr = cai_WriteOpenTagXML ( des , "Instrument_Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles_List" );
    goto ERREUR;
    }

  j = 0;
  while ( ( j < nb_bs ) && ( j < 3 ) )
  {
  /* sous element Instrument_Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Instrument_Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles" );
    goto ERREUR;
    }

  /* sous element VALIDITY_DATE */
  
  cr = cai_WriteElementXML ( des , "VALIDITY_DATE" , datecalib );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element VALIDITY_DATE" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
  sprintf ( valeur , "%d" , (j+1) );
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Look_Angles_List            */
/*****************************************************************************/

  /* sous element Look_Angles_List */

  cr = cai_WriteOpenTagXML ( des , "Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles_List" );
    goto ERREUR;
    }

  /* sous element Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* sous element DETECTOR_ID */

  cr = cai_WriteElementXML ( des , "DETECTOR_ID" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DETECTOR_ID" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : DIMAP tag LOOK_ANGLE_LIST valeurs de PSI_X et  */
/*                            PSI_Y 1er et dernier detecteur inverses        */
/*****************************************************************************/

  /* sous element PSI_X */
  
  sprintf ( valeur , "%lf" , psi_x_last[0] );
  cr = cai_WriteElementXML ( des , "PSI_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_X" );
    goto ERREUR;
    }

  /* sous element PSI_Y */
  
  sprintf ( valeur , "%lf" , psi_y_last[0] );
  cr = cai_WriteElementXML ( des , "PSI_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* sous element Look_Angle */
  
  cr = cai_WriteOpenTagXML ( des , "Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* sous element DETECTOR_ID */

/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'ecriture detector_id lookangle pour   */
/*                            PAN                                            */
/*****************************************************************************/

  if ( nb_bs == 1 ) 
     detect_id = 6000;
  else
     detect_id = 3000;

  sprintf ( valeur , "%d" , detect_id );

  cr = cai_WriteElementXML ( des , "DETECTOR_ID" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DETECTOR_ID" );
    goto ERREUR;
    }

  /* sous element PSI_X */
  
  sprintf ( valeur , "%lf" , psi_x_first[0] );
  cr = cai_WriteElementXML ( des , "PSI_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_X" );
    goto ERREUR;
    }

  /* sous element PSI_Y */
  
  sprintf ( valeur , "%lf" , psi_y_first[0] );
  cr = cai_WriteElementXML ( des , "PSI_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }
 
  /* Fermeture de l'element Look_Angles_List */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles_List" ); 
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles_List" );
    goto ERREUR;
    }
 
  /* Fermeture de l'element Instrument_Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Instrument_Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles" );
    goto ERREUR;
    }
  j++;
  }

  while ( ( nb_bs != 1 ) && ( j < nb_bs + 1 ) )
  {
  /* sous element Instrument_Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Instrument_Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles" );
    goto ERREUR;
    }

  /* sous element VALIDITY_DATE */
  
  cr = cai_WriteElementXML ( des , "VALIDITY_DATE" , datecalib );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element VALIDITY_DATE" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
  sprintf ( valeur , "%d" , (j+1) );
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Look_Angles_List            */
/*****************************************************************************/

  /* sous element Look_Angles_List */

  cr = cai_WriteOpenTagXML ( des , "Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles_List" );
    goto ERREUR;
    }


  /* sous element Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* sous element DETECTOR_ID */

  cr = cai_WriteElementXML ( des , "DETECTOR_ID" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DETECTOR_ID" );
    goto ERREUR;
    }
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM : DIMAP tag LOOK_ANGLE_LIST valeurs de PSI_X et  */
/*                            PSI_Y 1er et dernier detecteur inverses        */
/*****************************************************************************/

  /* sous element PSI_X */
  
  sprintf ( valeur , "%lf" , psi_x_last[j-2] );
  cr = cai_WriteElementXML ( des , "PSI_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_X" );
    goto ERREUR;
    }

  /* sous element PSI_Y */
  
  sprintf ( valeur , "%lf" , psi_y_last[j-2] );
  cr = cai_WriteElementXML ( des , "PSI_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* sous element Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* sous element DETECTOR_ID */

/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'ecriture detector_id lookangle pour   */
/*                            PAN                                            */
/*****************************************************************************/

  if ( nb_bs == 1 )
     detect_id = 6000;
  else
     detect_id = 3000;

  sprintf ( valeur , "%d" , detect_id );

  cr = cai_WriteElementXML ( des , "DETECTOR_ID" , valeur );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DETECTOR_ID" );
    goto ERREUR;
    }

  /* sous element PSI_X */
  
  sprintf ( valeur , "%lf" , psi_x_first[j-2] );
  cr = cai_WriteElementXML ( des , "PSI_X" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_X" );
    goto ERREUR;
    }

  /* sous element PSI_Y */
  
  sprintf ( valeur , "%lf" , psi_y_first[j-2] );
  cr = cai_WriteElementXML ( des , "PSI_Y" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element PSI_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles_List */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles_List" );
    goto ERREUR;
    }

  /* Fermeture de l'element Instrument_Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Instrument_Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles" );
    goto ERREUR;
    }
  j++;
  
  }

  /* Fermeture de l'element Instrument_Look_Angles_List */

  cr = cai_WriteCloseTagXML ( des , "Instrument_Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles_List" );
    goto ERREUR;
    }

  /* sous element Mirror_Position */
  
  cr = cai_WriteOpenTagXML ( des , "Mirror_Position" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Mirror_Position" );
    goto ERREUR;
    }

  /* sous element STEP_COUNT */
  
  cr = cai_WriteElementXML ( des , "STEP_COUNT" , mirror_step );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element STEP_COUNT" );
    goto ERREUR;
    }

  /* Fermeture de l'element Mirror_Position */

  cr = cai_WriteCloseTagXML ( des , "Mirror_Position" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Mirror_Position" );
    goto ERREUR;
    }

  /* Fermeture de l'element Sensor_Configuration */

  cr = cai_WriteCloseTagXML ( des , "Sensor_Configuration" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Sensor_Configuration" );
    goto ERREUR;
    }

  /* sous element Frame_Counters */
  
  cr = cai_WriteOpenTagXML ( des , "Frame_Counters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Frame_Counters" );
    goto ERREUR;
    }

  /* sous element SEGMENT_START */
  
  cr = cai_WriteElementXML ( des , "SEGMENT_START" , segmentstart );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SEGMENT_START" );
    goto ERREUR;
    }

   /* sous element SCENE_START */
  
  cr = cai_WriteElementXML ( des , "SCENE_START" , scenestart );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SCENE_START" );
    goto ERREUR;
    }

  /* sous element SEGMENT_END */
  
  cr = cai_WriteElementXML ( des , "SEGMENT_END" , segmentend );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SEGMENT_END" );
    goto ERREUR;
    }

 /* Fermeture de l'element Frame_Counters */

  cr = cai_WriteCloseTagXML ( des , "Frame_Counters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Frame_Counters" );
    goto ERREUR;
    }

  /* sous element Data_Strip_Coordinates */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Strip_Coordinates" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Strip_Coordinates" );
    goto ERREUR;
    }

  /* sous element FIRST_PIXEL_RAW */
  
  cr = cai_WriteElementXML ( des , "FIRST_PIXEL_RAW" , fpixraw );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FIRST_PIXEL_RAW" );
    goto ERREUR;
    }

   /* sous element FIRST_LINE_RAW */
  
  cr = cai_WriteElementXML ( des , "FIRST_LINE_RAW" , flineraw );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FIRST_LINE_RAW" );
    goto ERREUR;
    }

  /* sous element FIRST_PIXEL_1B */
  
  cr = cai_WriteElementXML ( des , "FIRST_PIXEL_1B" , fpix1b );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FIRST_PIXEL_1B" );
    goto ERREUR;
    }

  /* sous element FIRST_LINE_1B */
  
  cr = cai_WriteElementXML ( des , "FIRST_LINE_1B" , fline1b );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element FIRST_LINE_1B" );
    goto ERREUR;
    }

 /* Fermeture de l'element Data_Strip_Coordinates */

  cr = cai_WriteCloseTagXML ( des , "Data_Strip_Coordinates" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Strip_Coordinates" );
    goto ERREUR;
    }

  /* sous element Models */
  
  cr = cai_WriteOpenTagXML ( des , "Models" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Models" );
    goto ERREUR;
    }

  /* sous element OneB_Model */
  
  cr = cai_WriteOpenTagXML ( des , "OneB_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element OneB_Model" );
    goto ERREUR;
    }

  /* sous element Coordinate_Normalization */
  
  cr = cai_WriteOpenTagXML ( des , "Coordinate_Normalization" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Coordinate_Normalization" );
    goto ERREUR;
    }

  /* sous element LINES_I_M */
  
  cr = cai_WriteElementXML ( des , "LINES_I_M" , im );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LINES_I_M" );
    goto ERREUR;
    }

  /* sous element LINES_DELTA_I */
  
  cr = cai_WriteElementXML ( des , "LINES_DELTA_I" , deltai );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LINES_DELTA_I" );
    goto ERREUR;
    }

  /* sous element COLUMNS_J_M */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_J_M" , jm );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COLUMNS_J_M" );
    goto ERREUR;
    }

  /* sous element COLUMNS_DELTA_J */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_DELTA_J" , deltaj );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COLUMNS_DELTA_J" );
    goto ERREUR;
    }

  /* sous element LINES_L_M */
  
  cr = cai_WriteElementXML ( des , "LINES_L_M" , lm );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LINES_L_M" );
    goto ERREUR;
    }

  /* sous element LINES_DELTA_L */
  
  cr = cai_WriteElementXML ( des , "LINES_DELTA_L" , deltal );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element LINES_DELTA_L" );
    goto ERREUR;
    }

  /* sous element COLUMNS_P_M */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_P_M" , pm );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COLUMNS_P_M" );
    goto ERREUR;
    }

  /* sous element COLUMNS_DELTA_P */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_DELTA_P" , deltap );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element COLUMNS_DELTA_P" );
    goto ERREUR;
    }

  /* Fermeture de l'element Coordinate_Normalization */

  cr = cai_WriteCloseTagXML ( des , "Coordinate_Normalization" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Coordinate_Normalization" );
    goto ERREUR;
    }

  /* sous element Direct_OneB_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Direct_OneB_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Direct_OneB_Model" );
    goto ERREUR;
    }

  /* sous element I */
  
  cr = cai_WriteOpenTagXML ( des , "I" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element I" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , coeffi );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , ai );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , bi );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , ci );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , di );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , ei );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element I */

  cr = cai_WriteCloseTagXML ( des , "I" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element I" );
    goto ERREUR;
    }

  /* sous element J */
  
  cr = cai_WriteOpenTagXML ( des , "J" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element J" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , coeffj );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , aj );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , bj );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , cj );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element J */

  cr = cai_WriteCloseTagXML ( des , "J" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element J" );
    goto ERREUR;
    }

  /* Fermeture de l'element Direct_OneB_Model */

  cr = cai_WriteCloseTagXML ( des , "Direct_OneB_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Direct_OneB_Model" );
    goto ERREUR;
    }

  /* sous element Reverse_OneB_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Reverse_OneB_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Reverse_OneB_Model" );
    goto ERREUR;
    }

  /* sous element L */
  
  cr = cai_WriteOpenTagXML ( des , "L" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element L" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , coeff_l );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , a_l );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , b_l );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , c_l );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , d_l );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , e_l );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element L */

  cr = cai_WriteCloseTagXML ( des , "L" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element L" );
    goto ERREUR;
    }

  /* sous element P */
  
  cr = cai_WriteOpenTagXML ( des , "P" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element P" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , coeff_p );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , a_p );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , b_p );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , c_p );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element P */

  cr = cai_WriteCloseTagXML ( des , "P" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element P" );
    goto ERREUR;
    }

  /* Fermeture de l'element Reverse_OneB_Model */

  cr = cai_WriteCloseTagXML ( des , "Reverse_OneB_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Reverse_OneB_Model" );
    goto ERREUR;
    }

  /* sous element A */
  
  cr = cai_WriteOpenTagXML ( des , "A" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element A" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , coeff_a );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , aa );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , ba );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , ca );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , da );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element A */

  cr = cai_WriteCloseTagXML ( des , "A" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element A" );
    goto ERREUR;
    }

  /* sous element B */
  
  cr = cai_WriteOpenTagXML ( des , "B" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element B" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , coeff_b );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , ab );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , bb );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , cb );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , db );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , eb );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element B */

  cr = cai_WriteCloseTagXML ( des , "B" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element B" );
    goto ERREUR;
    }

  /* Fermeture de l'element OneB_Model */

  cr = cai_WriteCloseTagXML ( des , "OneB_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element OneB_Model" );
    goto ERREUR;
    }

  /* sous element Attitude_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Attitude_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Attitude_Model" );
    goto ERREUR;
    }

  /* sous element D_L */
  
  cr = cai_WriteOpenTagXML ( des , "D_L" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_L" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , al );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , bl );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , cl );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_L */

  cr = cai_WriteCloseTagXML ( des , "D_L" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_L" );
    goto ERREUR;
    }

  /* sous element D_P */
  
  cr = cai_WriteOpenTagXML ( des , "D_P" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_P" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , ap );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , bp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , cp );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_P */

  cr = cai_WriteCloseTagXML ( des , "D_P" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_P" );
    goto ERREUR;
    }

  /* Fermeture de l'element Attitude_Model */

  cr = cai_WriteCloseTagXML ( des , "Attitude_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Attitude_Model" );
    goto ERREUR;
    }

  /* sous element SWIR_Registration_Model */
  
  cr = cai_WriteOpenTagXML ( des , "SWIR_Registration_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SWIR_Registration_Model" );
    goto ERREUR;
    }

  /* sous element D_L */
  
  cr = cai_WriteOpenTagXML ( des , "D_L" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_L" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , alswir );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , blswir );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , clswir );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_L */

  cr = cai_WriteCloseTagXML ( des , "D_L" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_L" );
    goto ERREUR;
    }

  /* sous element D_P */
  
  cr = cai_WriteOpenTagXML ( des , "D_P" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_P" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , apswir );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , bpswir );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , cpswir );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_P */

  cr = cai_WriteCloseTagXML ( des , "D_P" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element D_P" );
    goto ERREUR;
    }

  /* Fermeture de l'element SWIR_Registration_Model */

  cr = cai_WriteCloseTagXML ( des , "SWIR_Registration_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element SWIR_Registration_Model" );
    goto ERREUR;
    }

  /* Fermeture de l'element Models */

  cr = cai_WriteCloseTagXML ( des , "Models" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Models" );
    goto ERREUR;
    }
  /* sous element Sensor_Calibration */
  
  cr = cai_WriteOpenTagXML ( des , "Sensor_Calibration" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Sensor_Calibration" );
    goto ERREUR;
    }

  /* sous element METHOD */
  
  cr = cai_WriteElementXML ( des , "METHOD" , "system" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element METHOD" );

    goto ERREUR;
    }

  /* sous element Calibration */
  
  cr = cai_WriteOpenTagXML ( des , "Calibration" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Calibration" );
    goto ERREUR;
    }

  for ( j = 0; j < nb_bs ; j++ )
  {
  /* sous element Band_Parameters */
  
  cr = cai_WriteOpenTagXML ( des , "Band_Parameters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Parameters" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  sprintf ( valeur , "%d" , (j+1) );
  
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
    goto ERREUR;
    }

  /* sous element Gain_Section */
  
  cr = cai_WriteOpenTagXML ( des , "Gain_Section" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Gain_Section" );
    goto ERREUR;
    }

  /* sous element GAIN_NUMBER */
  
  cr = cai_WriteElementXML ( des , "GAIN_NUMBER" , nb_gains[j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element GAIN_NUMBER" );
    goto ERREUR;
    }

  /* sous element GAIN_ANALOG_VALUE */
  
  cr = cai_WriteElementXML ( des , "GAIN_ANALOG_VALUE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element GAIN_ANALOG_VALUE" );
    goto ERREUR;
    }

  /* sous element Pixel_Parameters */
  
  cr = cai_WriteOpenTagXML ( des , "Pixel_Parameters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Pixel_Parameters" );
    goto ERREUR;
    }

  /* sous element G_DATE */
  
  cr = cai_WriteElementXML ( des , "G_DATE" , relativedatecalib[2*j] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element G_DATE" );
    goto ERREUR;
    }

  /* sous element DC_DATE */
  
  cr = cai_WriteElementXML ( des , "DC_DATE" , relativedatecalib[2*j+nb_bs*2] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DC_DATE" );
    goto ERREUR;
    }

  /* sous element Cells */
  
  cr = cai_WriteOpenTagXML ( des , "Cells" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Cells" );
    goto ERREUR;
    }

  if ( nb_bs == 1 )
  {
    for ( i = 0 ; i < 4 ; i ++ )
    { /* MOD : VERSION 4.6 : FA : initialisation de ind_tab_gain dans la boucle */ 
      for ( ind_tab_gain = 0  ; ind_tab_gain < 1500 ; ind_tab_gain ++ )
        {
	
        /* sous element Cell */
  
        cr = cai_WriteOpenTagXML ( des , "Cell" );
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Cell" );
          goto ERREUR;
          }

        /* sous element G */
    
        sprintf ( valeur , "%5.4f" , gain_value[i][ind_tab_gain] ); 
    
        cr = cai_WriteElementXML ( des , "G" , valeur );
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element G" );
          goto ERREUR;
          }

        /* sous element DARK_CURRENT */
     
        sprintf ( valeur , "%2.1f" , dark_value[i][ind_tab_gain] ); 

        cr = cai_WriteElementXML ( des , "DARK_CURRENT" , valeur );
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element DARK_CURRENT" );
          goto ERREUR;
          }

        /* Fermeture de l'element Cell */
 
        cr = cai_WriteCloseTagXML ( des , "Cell" );  
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Cell" );
          goto ERREUR;
          }
        }
      }
  }
  else
  {
    for ( i = 2*j ; i < 2*j+2 ; i++ )
      {
      for ( ind_tab_gain = 0 ; ind_tab_gain < 1500 ; ind_tab_gain ++ )
        {
        /* sous element Cell */
  
        cr = cai_WriteOpenTagXML ( des , "Cell" );
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Cell" );
          goto ERREUR;
          }

        /* sous element G */
    
        sprintf ( valeur , "%5.4f" , gain_value[i][ind_tab_gain] ); 
    
        cr = cai_WriteElementXML ( des , "G" , valeur );
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element G" );
          goto ERREUR;
          }

        /* sous element DARK_CURRENT */

        sprintf ( valeur , "%2.1f" , dark_value[i][ind_tab_gain] ); 

        cr = cai_WriteElementXML ( des , "DARK_CURRENT" , valeur );
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element DARK_CURRENT" );
          goto ERREUR;
          }

        /* Fermeture de l'element Cell */
 
        cr = cai_WriteCloseTagXML ( des , "Cell" );  
        if ( cr == CAI_KO )
          {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Cell" );
          goto ERREUR;
          }
        }
       }	
    }
  
  /* Fermeture de l'element Cells */

  cr = cai_WriteCloseTagXML ( des , "Cells" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Cells" );
    goto ERREUR;
    }

  /* Fermeture de l'element Pixel_Parameters */

  cr = cai_WriteCloseTagXML ( des , "Pixel_Parameters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Pixel_Parameters" );
    goto ERREUR;
    }

  /* Fermeture de l'element Gain_Section */

  cr = cai_WriteCloseTagXML ( des , "Gain_Section" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Gain_Section" );
    goto ERREUR;
    }

  /* sous element Dead_Detectors */
  
  cr = cai_WriteOpenTagXML ( des , "Dead_Detectors" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dead_Detectors" );
    goto ERREUR;
    }

  /* sous element VALIDITY_DATE */
  
  cr = cai_WriteElementXML ( des , "VALIDITY_DATE" , 
					relativedatecalib[2*j+1] );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element VALIDITY_DATE" );
    goto ERREUR;
    }

  /* sous element Bad_Pixels */
  
  cr = cai_WriteOpenTagXML ( des , "Bad_Pixels" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Pixels" );
    goto ERREUR;
    }

  if ( nb_bs == 1 )
  {
    for ( i = 0 ; i < 4 ; i ++ )
    {
      for ( ind_tab_gain = 0; ind_tab_gain < 1500 ; ind_tab_gain ++ )
      {
         if ( gain_value[i][ind_tab_gain] == (float) 0 )
         {
	    /* sous element Bad_Pixel */
          
	    cr = cai_WriteOpenTagXML ( des , "Bad_Pixel" );
            if ( cr == CAI_KO )
            {
              sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Pixel" );
              goto ERREUR;
            }

            /* sous element PIXEL_INDEX */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : Correction  de                                 */
/*            sprintf(valeur,"%d", (ind_tab_gain+1+1500*i)) au lieu de       */
/*		sprintf(valeur,"%f", (ind_tab_gain+1+1500*i))                */
/*****************************************************************************/

            sprintf ( valeur , "%d" , (ind_tab_gain+1+1500*i) );
            cr = cai_WriteElementXML ( des , "PIXEL_INDEX" , valeur );
            if ( cr == CAI_KO )
            {
              sprintf ( CAI_ERREUR , "Erreur ecriture element PIXEL_INDEX" );
              goto ERREUR;
            }

          /* sous element BAD_PIXEL_STATUS */
  
           cr = cai_WriteElementXML ( des , 
				"BAD_PIXEL_STATUS" , "ABERRANT" );
           if ( cr == CAI_KO )
           {
              sprintf ( CAI_ERREUR , "Erreur ecriture element BAD_PIXEL_STATUS" );
              goto ERREUR;
            }

           /* Fermeture de l'element Bad_Pixel */

          cr = cai_WriteCloseTagXML ( des , "Bad_Pixel" );  
          if ( cr == CAI_KO )
          {
            sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Pixel" );
            goto ERREUR;
          }
        }
      }	  
    }
  }
  else
  {
    nb_passage = 0;
    for ( i = 2*j ; i < j*2 + 2 ; i++ )
    {
      for ( ind_tab_gain = 0 ; ind_tab_gain < 1500 ; ind_tab_gain ++ )
      {
          if ( gain_value[i][ind_tab_gain] == (float) 0 )
          {
	  /* sous element Bad_Pixel */
          
	    cr = cai_WriteOpenTagXML ( des , "Bad_Pixel" );
            if ( cr == CAI_KO )
            {
               sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Pixel" );
               goto ERREUR;
            }

          /* sous element PIXEL_INDEX */
  
            sprintf ( valeur , "%d" , ( ind_tab_gain+1+1500*nb_passage ) );
	    cr = cai_WriteElementXML ( des , "PIXEL_INDEX" , valeur );
            if ( cr == CAI_KO )
            {
              sprintf ( CAI_ERREUR , "Erreur ecriture element PIXEL_INDEX" );
              goto ERREUR;
            }

          /* sous element BAD_PIXEL_STATUS */
  
            cr = cai_WriteElementXML ( des , 
					"BAD_PIXEL_STATUS" , "ABERRANT" );
           if ( cr == CAI_KO )
           {
             sprintf ( CAI_ERREUR , "Erreur ecriture element BAD_PIXEL_STATUS" );
             goto ERREUR;
           }

         /* Fermeture de l'element Bad_Pixel */

           cr = cai_WriteCloseTagXML ( des , "Bad_Pixel" );  
           if ( cr == CAI_KO )
           {
             sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Pixel" );
             goto ERREUR;
           }
	 }
       }
      if ( nb_passage == 0 )
         nb_passage = 1;
      else
         nb_passage = 0;

    }
  }
    
  /* Fermeture de l'element Bad_Pixels */

  cr = cai_WriteCloseTagXML ( des , "Bad_Pixels" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Pixels" );
    goto ERREUR;
    }

   /* Fermeture de l'element Dead_Detectors */

  cr = cai_WriteCloseTagXML ( des , "Dead_Detectors" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dead_Detectors" );
    goto ERREUR;
    }

  /* Fermeture de l'element Band_Parameters */

  cr = cai_WriteCloseTagXML ( des , "Band_Parameters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Parameters" );
    goto ERREUR;
    }
  }
   /* Fermeture de l'element Calibration */

  cr = cai_WriteCloseTagXML ( des , "Calibration" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Calibration" );
    goto ERREUR;
    }

  /* sous element Spectral_Sensitivities */
  
  cr = cai_WriteOpenTagXML ( des , "Spectral_Sensitivities" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Spectral_Sensitivities" );
    goto ERREUR;
    }


  for ( j = 0; j < nb_bs ; j++ )
  {

    /* sous element Band_Spectral_Sensitivities */
   
    cr = cai_WriteOpenTagXML ( des , "Band_Spectral_Sensitivities" );
    if ( cr == CAI_KO )
    {
      sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Spectral_Sensitivities" );
      goto ERREUR;
    }

     sprintf( valeur , "%d" , ( j+1 ) );
     /* sous element BAND_INDEX */
  
     cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
     if ( cr == CAI_KO )
     {
        sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
        goto ERREUR;
     }

  /* sous element FIRST_WAVELENGTH_VALUE */
  
     cr = cai_WriteElementXML ( des , 
			"FIRST_WAVELENGTH_VALUE" , firstwave[j] );
     if ( cr == CAI_KO )
     {
       sprintf ( CAI_ERREUR , "Erreur ecriture element FIRST_WAVELENGTH_VALUE" );
       goto ERREUR;
     }

  /* sous element WAVELENGTH_STEP */
  
     cr = cai_WriteElementXML ( des , 
			"WAVELENGTH_STEP" , wavelengthstep[j] );
     if ( cr == CAI_KO )
     {
        sprintf ( CAI_ERREUR , "Erreur ecriture element WAVELENGTH_STEP" );
        goto ERREUR;
      }

  /* sous element CALIBRATION_DATE */
  
     cr = cai_WriteElementXML ( des , "CALIBRATION_DATE" , "" );
     if ( cr == CAI_KO )
     {
         sprintf ( CAI_ERREUR , "Erreur ecriture element CALIBRATION_DATE" );
         goto ERREUR;
      }
  
  /* sous element Spectral_Sensitivity_Values */
  
     cr = cai_WriteOpenTagXML ( des , "Spectral_Sensitivity_Values" );
     if ( cr == CAI_KO )
     {
        sprintf ( CAI_ERREUR , "Erreur ecriture element Spectral_Sensitivity_Values" );
        goto ERREUR;
     }

    for ( ind_tab_gain = 0  ; ind_tab_gain < 64 ; ind_tab_gain++ )
    {
    
    /* sous element SPECTRAL_SENSITIVITY_VALUE */
  
        cr = cai_WriteElementXML ( des , "SPECTRAL_SENSITIVITY_VALUE" , 
				spectral_sensitivitie[j][ind_tab_gain] );
        if ( cr == CAI_KO )
        {
            sprintf ( CAI_ERREUR , "Erreur ecriture element SPECTRAL_SENSITIVITY_VALUE" );
            goto ERREUR;
         }
     }
     
    /* Fermeture de l'element Spectral_Sensitivity_Values */

     cr = cai_WriteCloseTagXML ( des , "Spectral_Sensitivity_Values" );  
     if ( cr == CAI_KO )
     {
       sprintf ( CAI_ERREUR , "Erreur ecriture element Spectral_Sensitivity_Values" );
       goto ERREUR;
     }

    /* Fermeture de l'element Band_Spectral_Sensitivities */

    cr = cai_WriteCloseTagXML ( des , "Band_Spectral_Sensitivities" );  
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Spectral_Sensitivities" );
      goto ERREUR;
      }
  }  
  
  /* Fermeture de l'element Spectral_Sensitivities */

  cr = cai_WriteCloseTagXML ( des , "Spectral_Sensitivities" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Spectral_Sensitivities" );
    goto ERREUR;
    }

  /* sous element Solar_Irradiance */
  
  cr = cai_WriteOpenTagXML ( des , "Solar_Irradiance" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Solar_Irradiance" );
    goto ERREUR;
    }

  for ( j = 0; j < nb_bs ; j++ )
  {
    sprintf ( valeur , "%d" , ( j+1 ) );
  /* sous element Band_Solar_Irradiance */
  
    cr = cai_WriteOpenTagXML ( des , "Band_Solar_Irradiance" );
    if ( cr == CAI_KO )
    {
       sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Solar_Irradiance" );
       goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
    cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
    if ( cr == CAI_KO )
    {
      sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
      goto ERREUR;
     }

  /* sous element SOLAR_IRRADIANCE_VALUE */
  
     cr = cai_WriteElementXML ( des , "SOLAR_IRRADIANCE_VALUE" , 
			solar_irradiance[j] );
     if ( cr == CAI_KO )
     {
        sprintf ( CAI_ERREUR , "Erreur ecriture element SOLAR_IRRADIANCE_VALUE" );
        goto ERREUR;
      }

  /* Fermeture de l'element Band_Solar_Irradiance */

    cr = cai_WriteCloseTagXML ( des , "Band_Solar_Irradiance" );  
    if ( cr == CAI_KO )
    {
       sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Solar_Irradiance" );
       goto ERREUR;
    }
  }
  /* Fermeture de l'element Solar_Irradiance */

  cr = cai_WriteCloseTagXML ( des , "Solar_Irradiance" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Solar_Irradiance" );
    goto ERREUR;
    }

  /* Fermeture de l'element Sensor_Calibration */

  cr = cai_WriteCloseTagXML ( des , "Sensor_Calibration" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Sensor_Calibration" );
    goto ERREUR;
    }

/* MOD : VERSION : 4.6 : FA : pas d'écriture <badlines> si pas de badlines   */
/* le test s'effectue pour chaque canal   */
   Bad=0;
   for (can=0;can<nb_bs;can++)
      BadFlag[can]=0;
   can=0;
   j=0;
   while ((can<nb_bs)&&(BadFlag[can]==0))
   {	
      j=0;
      while ((j<nb_lig)&&(BadFlag[can]==0))
      {
	 if ( bad_line[j][can] != 0)
	 {
		BadFlag[can]=1; 
		Bad=1;
         }
         j=j+1;
       }
      can=can+1;
   }

 if (Bad==1)
 {

  /* sous element Bad_Lines */
  
  cr = cai_WriteOpenTagXML ( des , "Bad_Lines" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Lines" );
    goto ERREUR;
    }
  }

  for ( can = 0 ; can < nb_bs ; can++ )
  {
    if (BadFlag[can]==1)
    {   

    sprintf ( valeur , "%d" , (can+1) );
  
  /* sous element Band_Bad_Lines */
  
    cr = cai_WriteOpenTagXML ( des , "Band_Bad_Lines" );
    if ( cr == CAI_KO )
    {
      sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Bad_Lines" );
      goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
    cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
    if ( cr == CAI_KO )
    {
      sprintf ( CAI_ERREUR , "Erreur ecriture element BAND_INDEX" );
      goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM :  Ajout de l'element  Bad_Line_List             */
/*****************************************************************************/    

   /* sous element Bad_Line_List */
  
    cr = cai_WriteOpenTagXML ( des , "Bad_Line_List" );
    if ( cr == CAI_KO )
    {
      sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Line_List" );
      goto ERREUR;
    }

    for ( j = 0; j < nb_lig ; j++ )
    {    
      if ( bad_line[j][can] == 1 )
      {
      /* sous element Bad_Line */
  
        cr = cai_WriteOpenTagXML ( des , "Bad_Line" );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Line" );
          goto ERREUR;
        }

      /* sous element BL_INDEX */
  
        sprintf ( valeur , "%d" , ( j+1 ) );
  
        cr = cai_WriteElementXML ( des , "BL_INDEX" , valeur );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element BL_INDEX" );
          goto ERREUR;
        }

       /* sous element BL_STATUS */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA :  mot cle  lignes perdues LOST au lieu de LOSS  */
/*****************************************************************************/    
        cr = cai_WriteElementXML ( des , "BL_STATUS" , "LOST" );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element BL_STATUS" );
          goto ERREUR;
        }

      /* Fermeture de l'element Bad_Line */
 
        cr = cai_WriteCloseTagXML ( des , "Bad_Line" );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Line" );
          goto ERREUR;
        }
      }
   else
   {
      if ( bad_line[j][can] == 2 )
      {
        /* sous element Bad_Line */
  
        cr = cai_WriteOpenTagXML ( des , "Bad_Line" );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Line" );
          goto ERREUR;
        }

      /* sous element BL_INDEX */
    
        sprintf ( valeur , "%d" , ( j+1 ) );
    
        cr = cai_WriteElementXML ( des , "BL_INDEX" , valeur );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element BL_INDEX" );
          goto ERREUR;
        }

       /* sous element BL_STATUS */
  
        cr = cai_WriteElementXML ( des , "BL_STATUS" , "DEGRADED" );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element BL_STATUS" );
          goto ERREUR;
        }
 
      /* Fermeture de l'element Bad_Line */

        cr = cai_WriteCloseTagXML ( des , "Bad_Line" );
        if ( cr == CAI_KO )
        {
          sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Line" );
          goto ERREUR;
        }
      } 
    } 
  } /* Fin boucle sur les lignes */
 
  /* Fermeture de l'element Bad_Line_List */

  cr = cai_WriteCloseTagXML ( des , "Bad_Line_List" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Line_List" );
    goto ERREUR;
    }
 
  /* Fermeture de l'element Band_Bad_Lines */

  cr = cai_WriteCloseTagXML ( des , "Band_Bad_Lines" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Band_Bad_Lines" );
    goto ERREUR;
    }
    
  }
 }
 if (Bad==1)
 {

  /* Fermeture de l'element Bad_Lines */

  cr = cai_WriteCloseTagXML ( des , "Bad_Lines" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Bad_Lines" );
    goto ERREUR;
    }
 
 }
  /* Fermeture de l'element Data_Strip */

  cr = cai_WriteCloseTagXML ( des , "Data_Strip" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Data_Strip" );
    goto ERREUR;
    }

 /* Fermeture de l'element Dimap_Document */

  cr = cai_WriteCloseTagXML ( des , "Dimap_Document" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Dimap_Document" );
    goto ERREUR;
    }

  /* fermeture du fichier descripteur */
  
  if (des!=NULL) fclose ( des );
  return CAI_OK;
  
  ERREUR:
  return CAI_KO;
  }  
/* 
	Fin de l'operation cai_lead4to5spot
*/

/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*                 Definition des OPERATIONS INTERNES                         */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
 
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_ElementWriteXML                                            */
/*                                                                            */
/*ROLE : Cette operation ecrit un element XML dans le fichier donne en        */
/*       parametre                                                            */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : CAI_OK ou CAI_KO                                                  */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_WriteElementXML ( FILE *desc , char *motCle , char *valeur )
 {
  char *BaliseXmlOpen;
  char *BaliseXmlClose;
  
  BaliseXmlOpen = (char*) NULL;
  BaliseXmlOpen = (char*) calloc ( strlen ( motCle ) + 3 , sizeof ( char ) );

  BaliseXmlClose = (char*) NULL;
  BaliseXmlClose = (char*) calloc ( strlen ( motCle ) + 4 , sizeof ( char ) );

  sprintf ( BaliseXmlOpen , "<%s>" , motCle );
  sprintf ( BaliseXmlClose , "</%s>" , motCle );
  
  fprintf ( desc , "%s%s%s\n" , BaliseXmlOpen , valeur , BaliseXmlClose );
  
  FREE ( BaliseXmlClose );
  FREE ( BaliseXmlOpen );
  return CAI_OK;
  
  }
/* 
	Fin de l'operation cai_WriteElementXML
*/
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_ElementWriteXMLWithAttribute                         */
/*                                                                            */
/*ROLE : Cette operation ecrit un element XML dans le fichier donne en        */
/*       parametre                                                            */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : CAI_OK ou CAI_KO                                                  */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_WriteElementXMLWithAttribute ( FILE *desc ,
                                                          char *motCle ,
							  char *valeur,
							  char *attribut,
							  char *valat ,
							  int type)
 {
  char *BaliseXmlOpen;
  char *BaliseXmlClose;
  
  BaliseXmlOpen = (char*) NULL;
  BaliseXmlOpen = (char*) calloc ( strlen ( motCle ) +
                                   strlen ( attribut ) +
				   strlen ( valat ) + 10 , sizeof ( char ) );

  BaliseXmlClose = (char*) NULL;
  BaliseXmlClose = (char*) calloc ( strlen ( motCle ) + 4 , sizeof ( char ) );
  if (type==0)
     sprintf ( BaliseXmlOpen , "<%s %s='%s'>" , motCle , attribut , valat );
  else
     sprintf ( BaliseXmlOpen , "<%s %s=\"%s\">" , motCle , attribut , valat );
  sprintf ( BaliseXmlClose , "</%s>" , motCle );
  
  fprintf ( desc , "%s%s%s\n" , BaliseXmlOpen , valeur , BaliseXmlClose );
  
  FREE ( BaliseXmlClose );
  FREE ( BaliseXmlOpen );
  return CAI_OK;
  
  }

/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_WriteOpenTagXML                                      */
/*                                                                            */
/*ROLE : Cette operation ecrit une balise ouvrante XML dans le fichier donne  */
/*       en parametre                                                         */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : CAI_OK ou CAI_KO                                                  */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_WriteOpenTagXML ( FILE *des , char *TagName )
  {
  fprintf ( des , "<%s>\n" , TagName );
  return CAI_OK;
  }
/* 
	Fin de l'operation cai_WriteOpenTagXML
*/
 
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_WriteCloseTagXML                                     */
/*                                                                            */
/*ROLE : Cette operation ecrit une balise fermante XML dans le fichier donne  */
/*       en parametre                                                         */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : CAI_OK ou CAI_KO                                                  */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_WriteCloseTagXML ( FILE *des , char *TagName )
  {
  fprintf ( des , "</%s>\n" , TagName );
  return CAI_OK;
  }
/* 
	Fin de l'operation cai_WriteCloseTagXML
*/
  
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_WriteNulTagXML                                       */
/*                                                                            */
/*ROLE : Cette operation ecrit une balise vide XML dans le fichier donne      */
/*       en parametre                                                         */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : CAI_OK ou CAI_KO                                                  */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_WriteNulTagXML ( FILE *des , char *TagName )
  {
  fprintf ( des , "<%s/>\n" , TagName );
  return CAI_OK;
  }
/* 
	Fin de l'operation cai_WriteNulTagXML
*/

/******************************************************************************/
/*                                                                            */
/*OPERATION  :  cai_WriteEnteteXML                                      */
/*                                                                            */
/*ROLE : Cette operation ecrit l'entete d'un document XML                     */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : CAI_OK ou CAI_KO                                                  */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_WriteEnteteXML ( FILE *des )
  {
  fprintf ( des , "<?xml version='1.0'?>\n" );
  return CAI_OK;  }
/* 
	Fin de l'operation cai_WriteEnteteXML
*/

/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_DMS_To_Float                                               */
/*                                                                            */
/*ROLE : Cette operation convertie une latitude/longitude exprimee en DDDMMSS */
/*       sous forme de chaine de caracteres en Flotants                       */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/* RETOUR : la donnee exprimee en double                                      */
/*                                                                            */
/******************************************************************************/

static void cai_DMS_To_Float ( char DegMinSec [17] , double *Degre )
 {
  /*-------------------------------*/
  /* debut du code de l'operation  */
  /*-------------------------------*/
  int i;
  int signe = 1;
  long int ValD = 0 , ValM = 0;
  double ValS = 0.0;
  char ValDeg[4] = "";
  char ValMin[3] = "";
  char ValSec[6] = "";

  if (strlen(DegMinSec) != 0)
  {
    /* Traitement du signe */

    if ( (DegMinSec[0] == 'N') || (DegMinSec[0] == 'E') )
       signe = 1;

    if ( (DegMinSec[0] == 'S') || (DegMinSec[0] == 'W') )
       signe = -1;

    if ( (DegMinSec[0] == 'R') || ( DegMinSec[0] == '+') )
       signe = 1;

    if ( (DegMinSec[0] == 'L') || ( DegMinSec[0] == '-') )
      signe = -1;
    
  /* Traitement de la valeur absolue */

    for ( i = 0 ; i < 3 ; i++ )
      ValDeg[i] = DegMinSec[i+1];

    ValDeg[3] = '\0';
    sscanf( ValDeg , "%ld" , &ValD );

    for ( i = 0 ; i < 2 ; i++ )
      ValMin[i] = DegMinSec[i+4];

    ValMin[2] = '\0';
    sscanf( ValMin , "%ld" , &ValM );

    for ( i = 0 ; i < 5 ; i++ )
       ValSec[i] = DegMinSec[i+6];

    ValSec[5] = '\0';
    sscanf( ValSec , "%lf" , &ValS );

    *Degre = (double)signe*((double)ValD + 
			(double)ValM/60.0 + (double)ValS/3600.0);
   }
   else
     *Degre = 0.0;

  /*-----------------------------------------------*/
  /* fin du code de l'operation cai_DMS_To_Float   */
  /*-----------------------------------------------*/
  } 

/******************************************************************************/
/*                                                                            */
/*OPERATION : cai_JulToGreg                                                   */
/*                                                                            */
/*ROLE : Convertir une date julienne en date gregorienne                      */
/*ROLE :                                                                      */
/*                                                                            */
/*RETOUR :                                                                    */
/*                                                                            */
/******************************************************************************/

int nb_jours_mois[12] = { 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 };

static void cai_JulToGreg ( long nbjours , double nbsec , char* dategreg )
 {
  /*-----------------------------------*/
  /* Declaration des variables locales */
  /*-----------------------------------*/
  int  an = ANNEE_DEBUT_JULIEN ;              /* annee debut = 1950 */
  int  nb_jour_an = NB_JOURS_AN_NON_BISSEX ;  /* 1950 = annee non bissextile */
  int  mois, jour, heure, minute, seconde, centieme ;
  long  jourjulien = 0 ;
  double  secjulien = 0. ;
  
  /*---------------------------------------*/
  /* DEBUT DE L'OPERATION   cai_JulToGreg  */
  /*---------------------------------------*/
  jourjulien = nbjours ;
  secjulien = nbsec ;
  
  /*---------------------------------------*/
  /* Calculer l'annee gregorienne          */
  /*---------------------------------------*/
  while ( jourjulien >= (long)nb_jour_an )
    {
    an = an + 1;    
    jourjulien = jourjulien - (long)nb_jour_an ;
    
    /*------------------------------------------------*/
    /* si anne bissextile mettre a jour le nb_jour_an */
    /*------------------------------------------------*/
    if ( ( ((an%4) == 0) && ((an%100) != 0) ) || (an%400 == 0) )
      nb_jour_an = NB_JOURS_AN_BISSEX ;
    else
      nb_jour_an = NB_JOURS_AN_NON_BISSEX ; 
    }
    
  /*---------------------------------------*/
  /* Attention : si l'annee est bissextile */
  /* il faut modifier le mois de fevrier ! */
  /*---------------------------------------*/
  if ( nb_jour_an == NB_JOURS_AN_BISSEX )
    nb_jours_mois[1] = 29 ;
        
  /*---------------------------------------*/
  /* Calculer le mois                      */
  /*---------------------------------------*/
  mois = 0 ;
  while ( jourjulien >= (long)nb_jours_mois[mois] )
    {
    jourjulien = jourjulien - (long)nb_jours_mois[mois] ;
    mois++;
    }
  
  /*---------------------------------------*/
  /* Calculer le jour                      */
  /*---------------------------------------*/
  
/* MOD : VERSION : 4.5 : FA : erreur conversion jour julien dans CAI         */
  jour = (int)jourjulien +1;

  /*---------------------------------------*/
  /* Calculer l'heure                      */
  /*---------------------------------------*/
  heure = (int) ( secjulien / 3600 ) ;
  secjulien = secjulien - ( heure * 3600 ) ;
  
  /*---------------------------------------*/
  /* Calculer les minutes                  */
  /*---------------------------------------*/
  minute = (int) ( secjulien / 60 ) ;
  secjulien = secjulien - ( minute * 60 ) ;
  
  /*---------------------------------------*/
  /* Calculer les secondes                 */
  /*---------------------------------------*/
  seconde = (int)secjulien  ;
  secjulien = secjulien - seconde ;
   
  /*---------------------------------------*/
  /* Calculer les centiemes                */
  /*---------------------------------------*/
  centieme = (int) ( secjulien * 1000000.0 + 0.5 ) ;

  /*---------------------------------------*/
  /* Ecrire le resultat                    */
  /*---------------------------------------*/
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM :  Ds les dates, utilisation du . au lieu de ,   */
/*			       Separation de la date et heure par "T"        */
/*****************************************************************************/
  sprintf ( dategreg , "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.6d\0" , an , (mois +1) ,
            jour , heure , minute , seconde , centieme );

  } 
/* Fin de l'operation cai_JulToGreg */



/*------------------------------------------*/
/* Debut de l'operation cai_DegreeToRadian  */

static double cai_DegreeToRadian ( double angle_degree )
  {
  return ( (angle_degree * (double )PI) / 180.0);
  }
/* Fin de l'operation cai_DegreeToRadian */



/*-----------------------------------*/
/* Debut de l'operation cai_addtime  */

static int cai_addtime ( char ret[26] , char temps[18] , double milisec )
{
double sec;
long lesjours=0;
char retour[25]="";
char variable[18]="";
strcpy ( variable , temps );

sec = atof ( &variable[5] ) + milisec;
if ( sec < 0 )
  {
  sec = -sec;
  lesjours--;
  }
  
variable[5] = '\0';
sscanf ( variable , "%ld" , &lesjours );

while ( sec >= NB_SEC_PAR_JOUR )
  {
  lesjours++;
  sec = sec - NB_SEC_PAR_JOUR ;
  }

cai_JulToGreg ( lesjours , sec , retour );
strcpy ( ret , retour );

return CAI_OK;
}

/* Fin de l'operation cai_addtime */

/****************************************************************/
/* Fonction permettant de retourner le nom d'un fichier a partir*/
/* du nom du repertoire qui le contient et du debut de son nom  */
/*                                                              */
/* PARAMETRES :                                                 */
/* typspot     (E) : type de spot (spot1a4, spot5 tif,spot5bil  */
/* str_repertoire (E) : chaine donnant le nom du repertoire     */
/* str_debut (E) : debut du nom du fichier                      */
/* str_fichier (S) : nom complet du fichier                     */
/*                                                              */
/* VALEUR RETOURNEE :                                           */
/* 0 : ok                                                       */
/* 1 : ko                                                       */
/****************************************************************/

static int cai_recup_nom_spot (char typspot, char * str_repertoire ,
				char * str_debut , char **str_fichier )
{

     DIR *ptr_repertoire;
     struct dirent *cr_readdir;
     long lg_comp;
     char str_comp[20];
     int fic_trouve;
     int retour;


     strcpy(CAI_ERREUR,"");
     fic_trouve = 0;
     retour = 0;
     lg_comp = strlen(str_debut);

/* --------------------------------------------------
  Ouverture du repertoire et listage des fichiers
-------------------------------------------------- */
/**************************************/
/* Traitement suivant le type de spot */
/**************************************/
 if (typspot=='1')
{
/* Format SPOT1a4 */

   ptr_repertoire = opendir(str_repertoire);
   if (ptr_repertoire != NULL)
   {
     while ((cr_readdir=readdir(ptr_repertoire))!=NULL)
     {
       strncpy(str_comp,cr_readdir->d_name,lg_comp);
       str_comp[lg_comp]='\0';  
       if ((strcasecmp(str_comp,str_debut)==0) && (fic_trouve == 0)
							 && (retour == 0))
	 {
	     *str_fichier = (char *) calloc(strlen(str_repertoire)+1+strlen(cr_readdir->d_name)+1, sizeof(char));
	     if (*str_fichier != NULL)
	     {
		strcpy(*str_fichier,str_repertoire);
	        strcat(*str_fichier,"/");
		strcat(*str_fichier,cr_readdir->d_name);
			(*str_fichier)[strlen(str_repertoire)+1+strlen(cr_readdir->d_name)]='\0';
		fic_trouve++;
	     }
	     else
	     {
		strcpy(CAI_ERREUR,"Erreur : Probleme d'allocation memoire");
	        retour = 1;
	     }
          }
	}
	closedir(ptr_repertoire);

	if (fic_trouve == 0)
	{
	    sprintf(CAI_ERREUR,"Erreur : fichier %s/%s... n'existe pas",
			str_repertoire,str_debut);
	   retour = 1;
	}
   }
 }
 else if (typspot=='B')
{
/* Format SPOT5 BIL */

   ptr_repertoire = opendir(str_repertoire);
   if (ptr_repertoire != NULL)
   {
     while ((cr_readdir=readdir(ptr_repertoire))!=NULL)
     {
       strncpy(str_comp,cr_readdir->d_name,lg_comp);
       str_comp[lg_comp]='\0';
       if ((strcasecmp(str_comp,str_debut)==0) && (fic_trouve == 0)
							 && (retour == 0))
       {
         if (((strncasecmp(str_debut,"IM",2)== 0)&&
		(((strstr(cr_readdir->d_name,".BIL")) != 0)||
		  ((strstr(cr_readdir->d_name,".bil")) != 0)))||
	      (strncasecmp(str_debut,"METADATA.DIM",12)== 0))
         {
	     *str_fichier = (char *) calloc(strlen(str_repertoire)+1+strlen(cr_readdir->d_name)+1, sizeof(char));
	     if (*str_fichier != NULL)
	     {
		strcpy(*str_fichier,str_repertoire);
	        strcat(*str_fichier,"/");
		strcat(*str_fichier,cr_readdir->d_name);
			(*str_fichier)[strlen(str_repertoire)+1+strlen(cr_readdir->d_name)]='\0';
		fic_trouve++;
	     }
	     else
	     {
		strcpy(CAI_ERREUR,"Erreur : Probleme d'allocation memoire");
	        retour = 1;
	     }
           }
          }
	}
	closedir(ptr_repertoire);

	if (fic_trouve == 0)
	{
	    sprintf(CAI_ERREUR,"Erreur : fichier %s/%s... n'existe pas",
			str_repertoire,str_debut);
	   retour = 1;
	}
   }

}
else if (typspot=='T')
{
/* Format SPOT5 TIF */

   ptr_repertoire = opendir(str_repertoire);
   if (ptr_repertoire != NULL)
   {
     while ((cr_readdir=readdir(ptr_repertoire))!=NULL)
     {
       strncpy(str_comp,cr_readdir->d_name,lg_comp);
       str_comp[lg_comp]='\0';
       if ((strcasecmp(str_comp,str_debut)==0) && (fic_trouve == 0)
							 && (retour == 0))
       {
         if (((strncasecmp(str_debut,"IM",2)== 0)&&
		(((strstr(cr_readdir->d_name,".TIF")) != 0)||
		  ((strstr(cr_readdir->d_name,".tif")) != 0)))||
	      (strncasecmp(str_debut,"METADATA.DIM",12)== 0))
         {
	     *str_fichier = (char *) calloc(strlen(str_repertoire)+1+strlen(cr_readdir->d_name)+1, sizeof(char));
	     if (*str_fichier != NULL)
	     {
		strcpy(*str_fichier,str_repertoire);
	        strcat(*str_fichier,"/");
		strcat(*str_fichier,cr_readdir->d_name);
			(*str_fichier)[strlen(str_repertoire)+1+strlen(cr_readdir->d_name)]='\0';
		fic_trouve++;
	     }
	     else
	     {
		strcpy(CAI_ERREUR,"Erreur : Probleme d'allocation memoire");
	        retour = 1;
	     }
           }
          }
	}
	closedir(ptr_repertoire);

	if (fic_trouve == 0)
	{
	    sprintf(CAI_ERREUR,"Erreur : fichier %s/%s... n'existe pas",
			str_repertoire,str_debut);
	   retour = 1;
	}
   }

}
else	
	retour=1;
   return(retour);
}

/*
  Fin de l'operation cai_recup_nom_spot
*/
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_lire_dimensions_spot5                                      */
/*                                                                            */
/*ROLE : Cette operation lit dans le leader XML de SPOT5 les caracteristiques */
/*       de l'image a lire                                                    */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/*RETOUR : CAI_OK si tout va bien , CAI_KO sinon                              */
/*                                                                            */
/******************************************************************************/

static CAI_OK_KO cai_lire_dimensions_spot5 ( char *fich_xml , int *nbcan ,
					     int *nboct , int *nbcol ,
					     int *nblig , char *byteorder,
					     int *skipbytes )
{
  /* variables locales */  
  FILE *descripteur;    /* descripteur du fichier XML             */
  char *maval;          /* variable servant a la lecture des mots */
  int trouve = 0;       /* nombre de mot cles trouves             */
  int ok_ncols  = 0;    /* indicateur de presence de NCOLS        */
  int ok_nbits  = 0;    /* indicateur de presence de NBITS        */
  int ok_nbands = 0;    /* indicateur de presence de NBANDS       */
  int ok_nrows  = 0;    /* indicateur de presence de NROWS        */
  int ok_byteorder = 0; /* indicateur de presence de BYTEORDER    */
  int ok_skipbytes = 0; /* indicateur de presence de SKIPBYTES    */
  int nbits;            /* nombre de bits par pixel               */


  /* Initialisation par defaut */
    if (strcmp(type_machine,"BE")==0)
		byteorder[0]='M';
    else
		byteorder[0]='I';

  /* ouvrir le fichier a lire */
   
  if ( ( descripteur = fopen ( fich_xml , "rb" ) ) == NULL )
    {
    sprintf ( CAI_ERREUR , "erreur ouverure de fichier" );
    goto ERREUR;
    }

  /* tant que le fichier n'est pas a la fin */
  
  while ( !feof ( descripteur ) )
    {
    /* lire un mot */
    maval = cai_spot5_gettoken ( descripteur );
    if ( maval == NULL ) goto ERREUR;

    /* comparer le mot lu */    

    if ( strcasecmp ( maval , NOMBRE_DE_BITS ) == 0 )
      {
      /* lire mot suivant */
      maval = cai_spot5_gettoken ( descripteur );
      if ( maval == NULL ) goto ERREUR;
      
      /* en deduire la valeur */
      sscanf ( maval , "%d" , &nbits );
      *nboct = nbits / 8 ;
      
      /* incrementer le nombre de champs trouves */
      trouve++;
      
      /* positionner le flag pour ce champs a 1 */
      ok_nbits = 1;
      }
    else  
    if ( strcasecmp ( maval , NOMBRE_DE_COLONNES ) == 0 )
      {
      /* lire mot suivant */
      maval = cai_spot5_gettoken ( descripteur );
      if ( maval == NULL ) goto ERREUR;
      
      /* en deduire la valeur */
      sscanf ( maval , "%d" , nbcol );
      
      /* incrementer le nombre de champs trouves */
      trouve++;
      
      /* positionner le flag pour ce champs a 1 */
      ok_ncols = 1;
      }
    else
    if ( strcasecmp ( maval , NOMBRE_DE_CANAUX ) == 0 )
      {
      /* lire mot suivant */
      maval = cai_spot5_gettoken ( descripteur );
      if ( maval == NULL ) goto ERREUR;
      
      /* en deduire la valeur */
      sscanf ( maval , "%d" , nbcan );
      
      /* incrementer le nombre de champs trouves */
      trouve++;
      
      /* positionner le flag pour ce champs a 1 */
      ok_nbands = 1;      
      }
    else
    if ( strcasecmp ( maval , NOMBRE_DE_LIGNES ) == 0 )
      {
      /* lire mot suivant */
      maval = cai_spot5_gettoken ( descripteur );
      if ( maval == NULL ) goto ERREUR;
      
      /* en deduire la valeur */
      sscanf ( maval , "%d" , nblig );
      
      /* incrementer le nombre de champs trouves */
      trouve++;
      
      /* positionner le flag pour ce champs a 1 */
      ok_nrows = 1;     
      }
    else
    if ( strcasecmp ( maval , ORDRE_LECTURE_OCTETS ) == 0 )
      {
      /* lire mot suivant */
      maval = cai_spot5_gettoken ( descripteur );
      if ( maval == NULL ) goto ERREUR;
      
      /* en deduire la valeur */
      sscanf ( maval , "%c" , byteorder );
      
      /* incrementer le nombre de champs trouves */
      trouve++;
      
      /* positionner le flag pour ce champs a 1 */
      ok_byteorder = 1;
      }
    else
    if ( strcasecmp ( maval , NB_OCTETS_A_SAUTER ) == 0 )
      {
      /* lire mot suivant */
      maval = cai_spot5_gettoken ( descripteur );
      if ( maval == NULL ) goto ERREUR;
      
      /* en deduire la valeur */
      sscanf ( maval , "%d" , skipbytes );
      
      /* incrementer le nombre de champs trouves */
      trouve++;
      
      /* positionner le flag pour ce champs a 1 */
      ok_skipbytes = 1;
      }
      
    /* liber la ressource */
    FREE ( maval );
    
    /* si les 6 mots a lire ont ete trouves , sortir de la boucle */    
    if ( trouve == 6 )
      break;
    }

  /* traiter les erreur dues au document */
  if ( trouve != 6 )
    {
     if ( ok_ncols == 0 )
      {
      sprintf ( CAI_ERREUR , "manque le tag %s dans le fichier DIMAP" , 
			     NOMBRE_DE_COLONNES );
      goto ERREUR;
      }

    if ( ok_nbits == 0 )
      {
      sprintf ( CAI_ERREUR , "manque le tag %s dans le fichier DIMAP" , 
			     NOMBRE_DE_BITS );
      goto ERREUR;
      }
    if ( ok_nrows == 0 )
      {
      sprintf ( CAI_ERREUR , "manque le tag %s dans le fichier DIMAP" , 
			     NOMBRE_DE_LIGNES );
      goto ERREUR;
      }

    if ( ok_nbands == 0 )
      {
      sprintf ( CAI_ERREUR , "manque le tag %s dans le fichier DIMAP" , 
			     NOMBRE_DE_CANAUX );
      goto ERREUR;
      }

     if ( ok_byteorder == 0 )
     {      sprintf ( CAI_ERREUR , "manque le tag %s dans le fichier DIMAP" , 
			     ORDRE_LECTURE_OCTETS );
      goto ERREUR;
      }

/* le skip byte n'est pas obligatoire */
    if ( ok_skipbytes == 0 )
      {
	*skipbytes = 0;
      }
    }

  /* fermeture du fichier */  
  if (descripteur !=NULL) fclose ( descripteur );
  
  /* liberation des ressources */  
  FREE ( maval );
  
  return CAI_OK;

ERREUR:
   FREE ( maval );
   return CAI_KO;
  }

 /* Fin de l'operation cai_lire_dimensions_spot5 */
 
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_spot5_gettoken                                             */
/*                                                                            */
/*ROLE : Cette operation lit lit un mot dans le fichier donne en parametre    */
/*        etretourne sa valeur en enlevent les eventuels caracteres < et >    */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/*RETOUR :  la valeur si tout va bien , CAI_KO sinon                          */
/*                                                                            */
/******************************************************************************/

static char* cai_spot5_gettoken ( FILE *MyFile )
{
  char valeur[1024] = ""; /* variable servant a stocker le mot lu       */
  char *ret;              /* variable retourne qui contiendra le mot lu */
  char c;                 /* caractere lu                               */
  int i = 0;              /* indice de la chaine de caractere valeur    */
  
  /* lire le premier caractere */
  
  if ( fscanf ( MyFile , "%c" , &c ) == 0 )
    {
    sprintf ( CAI_ERREUR , "erreur lecture xml" );
    return (char*) NULL;
    }
  
  /* tester si le caractere est <,>,retour chariot, espace  */
  /* auquel cas, sauter les blancs < , > , tabulations, etc */
  /* et si on n'est pas en fin de fichier                   */
  
  while ( ( ( ( c == '<' ) || ( c == ' ' ) ) || 
          ( ( c == '>' ) || ( c == '\n' ) ) || 
	  ( ( c == '\r' ) || ( c == '\t' ) ) )
	  && ( !feof ( MyFile ) ) )
    {
    if ( fscanf ( MyFile , "%c" , &c ) == 0 )
      {
      sprintf ( CAI_ERREUR , "erreur lecture xml" );
      return (char*) NULL;
      }
    }


  /* tant que que le mot n'est pas fini, lire les caracteres */
  
  while ( ( ( c != '>' ) && ( c != '<' ) ) &&
          ( ( c != ' ' ) && ( c != '\n' ) ) )
    {
    valeur[i] = c;
    if ( fscanf ( MyFile , "%c" , &c ) == 0 )
      {
      sprintf ( CAI_ERREUR , "erreur lecture xml" );
      return (char*) NULL;
      }
    i++;
    }

  /* terminer la chaine de caractere lue */
  
  valeur[i+1] = '\0';
  
  /* allouer la place pour la chaine qui contiendra le resultat */
  
  ret = (char*) NULL;
  ret = (char*) calloc ( strlen ( valeur ) + 1 , sizeof ( char ) );

  /* copie du mot lu dans la variable resultat */
  
  strcpy ( ret , valeur );

  /* retour du resultat */
  
  return ret;
  }
/* fin de l'operation cai_spot5_gettoken */

/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_spot1a4                                 */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format spot1a4 et de tous les fichiers qui la composent      */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_spot1a4(repert,          */
/*  ---------------- 		                       nom_image,nb_can,     */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     repert	      (E) (char *) : repertoire	de stockage		     */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir                 */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_spot1a4 (S) (CAI_IMAGE *) :  =NULL si pb           */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
static CAI_IMAGE *cai_ouvre_creation_spot1a4(char *repert,
				   		char *nom_image,
				   		int *nbca,
				   		int *nboct_pix,
				   		int *nbcol,
				   		int *nblig,
				   		char *label)
{
   CAI_IMAGE *image1;        /* descripteur image			     */
   char nom_fic[1024];	     /* nom du fichier en-tete		             */
   char nom_rep[1024];	     /* nom du fichier en-tete		             */
   int i,j,num,k;	     /* indice 				             */
   int cr;                   /* compte rendu d'operation                     */
   int retour;               /* compte rendu d'operation                     */
   struct stat	buf_stat;    /* structure decrivant status de fichier        */
   int descrip_leader;	     /* Descripteur du fichier LEADER                */
   int descrip_trailer;	     /* Descripteur du fichier TRAILER               */
   int descrip_volume;	     /* Descripteur du fichier VOLUME DIRECTORY      */
   int descrip_null;	     /* Descripteur du fichier NULL VOLUME DIRECTORY */
   int descrip_imagery;	     /* Descripteur du fichier IMAGERY               */
   long cr_write;	     /* Code retour de write                         */
   CAI_OK_KO icr;	     /* code retour ecriture dans DA                 */
   int nouv_indic;           /* indice 				             */
   char str_int4[5];	     /* Chaines intermediaires                       */
   char str_int5[6];         /* Chaines intermediaires                       */
   char str_int1[2];         /* Chaines intermediaires                       */
   char mode[4];	     /* Mode spectral                                */
   int lgrec;		     /* Longueur des enregistrements                 */
   int nbrec;		     /* Nombre d'enregistrements de l'image          */
   int nb;                   /* taille de l'image                            */
   int Existe;		     /* Indicateur d'existence du repertoire image   */

  char Message[200];
  
  /* Variables intermediaires */
   unsigned char BYTE[4];
   int VAR_LONG[2];

   char TAB[13000];  
   char LEVEL[1];
   char TAMP2[5][9];

   int LgDate;
   char StrDate[24];
   time_t LocalTime;
   int nbre_enreg;
	 

/*........................................
   INITIALISATIONS et allocation de place pour la structure CAI_IMAGE
  .........................................*/
  
  descrip_leader = -1;
  descrip_trailer = -1;
  descrip_volume = -1;
  descrip_null = -1;
  descrip_imagery = -1;
  Existe = 0;
  image1 = NULL;

  image1 = (CAI_IMAGE *)calloc(1,sizeof(CAI_IMAGE));

  if (image1==NULL)
  {
    strcpy(CAI_ERREUR,
		"Erreur allocation memoirem dans cai_ouvre_creation_spot1a4");
    goto ERREUR;
  }

  if ((*nboct_pix) != 1 )
  {
    strcpy(CAI_ERREUR,
	"Seul un octet par pixel est autorise pour l'ecriture SPOT1a4");
    goto ERREUR;
  }

  sprintf(image1->NIVEAU,"0 ");
 
/******************************************************************/
/*   Constitution du nom du fichier LEADER et verification qu'il  */
/*                    n'existe pas deja	                          */				
/******************************************************************/

  nom_fic[0]='\0';
  if ((long)strlen(repert)!= 0)
  {
      sprintf(nom_rep,"%s/%s",repert,nom_image);     
  }
  else
  {
      sprintf(nom_rep,"%s",nom_image);
  }

  retour = stat ( nom_rep , &buf_stat );
    
  /* si le repertoire image n'existe pas, le creer*/  
  if ( retour != 0 )
  {  
      cr = mkdir (nom_rep,0775);
      if (cr !=0)
      {
         sprintf ( CAI_ERREUR , 
		"probleme a la creation du repertoire image %s SPOT1A4 \n" ,
		 nom_image );
         goto ERREUR;
      }
  }
  else
    Existe = 1;

  sprintf(nom_fic,"%s/LEAD_01.DAT",nom_rep);
  
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , "Le fichier %s LEADER SPOT1A4 existe \n" , 
							nom_image );
       goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*	Ouverture du fichier LEADER en creation				     */
/*---------------------------------------------------------------------------*/

  descrip_leader = open (nom_fic,O_RDWR|O_CREAT|O_BINARY, PERMS );
  if (descrip_leader == -1)
  {
	strcpy ( CAI_ERREUR , "Erreur de creation du fichier Leader SPOT1A4");
    	goto ERREUR;
  }

/*---------------------------------------------------------------------------*/
/*		ECRITURE DU FICHIER LEADER				     */
/*---------------------------------------------------------------------------*/


/***********************************/
/* Pour le fichier FILE DESCRIPTOR */
/***********************************/

/* 1 RECORD */

/* SEGMENT FIXE : */

  VAR_LONG[0] = 1;
  BYTE[0] = 63;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;

  cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , &BYTE[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader SPOT1A4");
	goto ERREUR;
  }


  cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  strcpy( TAB , "A   S4-ST-73-1CN 0 0               1");
  cr_write  = write ( descrip_leader , TAB , 36);
  if (cr_write != 36)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  if (*nbca == 1)
  {
	strcpy(mode,"PAN");
        strcpy(TAB,"SP0 P0  LEADBIL ");
  }
  else
  {
    if (*nbca == 3)
    {
	strcpy(mode,"XS ");
        strcpy(TAB,"SP0 X0  LEADBIL ");
    }
    else
    {
     if (*nbca == 4)
     {
	strcpy(mode,"XI ");
        strcpy(TAB,"SP0 I0  LEADBIL ");
     }
    }
   }	

  cr_write = write ( descrip_leader, TAB , 16);
  if (cr_write != 16)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  
  strcpy( TAB , "FSEQ       1   4FTYP       5   4FLGT       9   4YNNN");
  cr_write  = write ( descrip_leader , TAB , 52);
  if (cr_write != 52)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }


  for (i=0;i<13000;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write  = write ( descrip_leader , TAB , 64);
  if (cr_write != 64)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }

/* SEGMENT VARIABLE : */

  strcpy( TAB , "     1  3960    24  3960     1  3960     2    37 16A     2    21 16A");
  cr_write  = write ( descrip_leader , TAB , 68);
  if (cr_write != 68)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }


 for (i=0;i<81;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  strcpy( TAB , "     2   613 16A     2   629 16A     2   581 32A     2    85 32A     2  1317 64A");
  cr_write  = write ( descrip_leader , TAB , 80);
  if (cr_write != 80)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

 for (i=0;i<50;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  strcpy( TAB , "     2  1029 16A     2  1061256A                ");
  cr_write  = write ( descrip_leader , TAB , 48);
  if (cr_write != 48)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

 for (i=0;i<3600;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write ( descrip_leader , TAB , 3584);
  if (cr_write != 3584)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }

/**************************/
/* Pour le fichier HEADER */
/**************************/

/* 1 RECORD */

  VAR_LONG[0] = 2;
  BYTE[0] = 18;
  BYTE[1] = 18;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;


  cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }


  cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  strcpy ( TAB, "   1");
  cr_write  = write ( descrip_leader , TAB , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

 for (i=0;i<630;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

   cr_write = write ( descrip_leader , TAB , 628);
  if (cr_write != 628)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

   /* Ecriture du mode spectral */

   cr_write = write(descrip_leader,mode,3);
   if (cr_write != 3)
   {
	 strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	 goto ERREUR;
   }

    cr_write = write(descrip_leader,TAB,360);
    if (cr_write != 360)
    {
         strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
         goto ERREUR;
    }
 
   /* Initialisation du nombre de colonnes, du nombre de lignes et du nombre 
   de canaux : */
   sprintf(TAB,"%5d" , *nbcol);

   cr_write = write ( descrip_leader , TAB , 5);
   if (cr_write != 5)
   {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
   }
   strcpy ( TAB, "               ");
   cr_write = write ( descrip_leader , TAB , 11);
   if (cr_write != 11)
   {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
   }
   sprintf(TAB,"%5d" , *nblig);
   cr_write = write ( descrip_leader , TAB , 5);
   if (cr_write != 5)
   {
	   strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	   goto ERREUR;
   }
   for (i=0;i<35;i++)
   {
	 strcpy(&TAB[i], " ");
   }
   cr_write = write ( descrip_leader , TAB , 31);
   if (cr_write != 31)
   {
	   strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	   goto ERREUR;
   }
   
   
  sprintf(TAB,"%d",*nbca);
  cr_write = write ( descrip_leader , TAB , 1);
  if (cr_write != 1)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }
   for (i=0;i<2741;i++)
   {
	 strcpy(&TAB[i], " ");
   }

  cr_write = write ( descrip_leader , TAB , 2740);
  if (cr_write != 2740)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }
  
                 
  strcpy ( TAB , "   1            3960              16            3960            ");
  cr_write = write ( descrip_leader , TAB , 64);
  if (cr_write != 64)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

 
  strcpy ( TAB , "   1            3960               4            3960            ");
  cr_write = write ( descrip_leader , TAB , 64);
  if (cr_write != 64)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  strcpy ( TAB , "   1            3960            ");
  cr_write = write ( descrip_leader , TAB , 32);
  if (cr_write != 32)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }



/**************************************/
/* Pour le fichier EPHEMERIS/ATTITUDE */
/**************************************/

/* 1 RECORD */

  VAR_LONG[0] = 3;
  BYTE[0] = 246;
  BYTE[1] = 36;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;


  cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  strcpy ( TAB , "   1");
  cr_write = write ( descrip_leader , TAB , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }
  for (i=0;i<3945;i++)
  {
	 strcpy(&TAB[i], " ");
  }

/* Ecriture dans le fichier Leader */

  cr_write = write ( descrip_leader , TAB , 3944);
  if (cr_write != 3944)
  {
	strcpy(CAI_ERREUR, "Erreur acces au fichier Leader");
	goto ERREUR;
  }



/*******************************************/
/* Pour le fichier RADIOMETRIC CALIBRATION */
/*******************************************/

/* 16 RECORDS */

  BYTE[0] = 63;
  BYTE[1] = 36;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;

  /*image source non SPOT*/

/* Ecriture dans le fichier Leader */

  for (i=0;i<16;i++)	/* Pour 16 records */
  {
	VAR_LONG[0] = 4+i;
	cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
 	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

  	cr_write = write ( descrip_leader , BYTE , 4);
  	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

	cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
 	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

  	cr_write = write ( descrip_leader , TAB , 48);
	if (cr_write != 48)
	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}
        for (j=0;j<3000;j=j+2)
        {
	   strcpy(&TAB[j], " ");
	   strcpy(&TAB[j+1], "0");
        }

	cr_write = write ( descrip_leader , TAB , 3000);
	if ( cr_write != 3000)
	{
	  strcpy(CAI_ERREUR,"Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
	}
        for (j=0;j<900;j++)
        {
	   strcpy(&TAB[j], " ");
        }

  	cr_write = write ( descrip_leader , TAB , 900);
	if (cr_write != 900)
	{
	  	strcpy ( CAI_ERREUR, 
			"Erreur ecriture du fichier Leader Spot1a4");
	  	goto ERREUR;
  	}

  }


/**********************************************/
/* Pour  MODELIZATION et GROUND CONTROL POINTS*/
/**********************************************/

/* 2 RECORD */

for (i=0;i<2;i++)
{
  VAR_LONG[0] = 20+i;
  BYTE[0] = 8;
  BYTE[1] = 21;
  BYTE[2] = 48;
  BYTE[3] = 35;
  VAR_LONG[1] = 3960;

  cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }


  strcpy ( TAB , "   1");
  cr_write = write ( descrip_leader , TAB, 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR , "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  for (i=0;i<3950;i++)
  {
	 strcpy(&TAB[j], " ");
  }

  
  /* Ecriture dans le fichier Leader */

  cr_write = write ( descrip_leader , TAB , 3944);
  if ( cr_write != 3944)
  {
	  strcpy(CAI_ERREUR,"Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }
}

/*****************************/
/* Pour  HISTOGRAM           */
/*****************************/

/* 4 RECORDS */

  BYTE[0] = 192;
  BYTE[1] = 36;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;


  for (i=0;i<*nbca;i++)	/* nbca records */
  {

	VAR_LONG[0] = 22+i;

	cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

  	cr_write = write ( descrip_leader , BYTE , 4);
  	if (cr_write != 4)
 	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

	cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

  	cr_write = write ( descrip_leader , TAB , 3948);
	if (cr_write != 3948)
	{
	  	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  	goto ERREUR;
  	}

  }

  if (*nbca!=4)
  {
    for (i=0;i<(4-(*nbca));i++)
    {
	VAR_LONG[0] = 22+i+(*nbca);

	cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

  	cr_write = write ( descrip_leader , BYTE , 4);
  	if (cr_write != 4)
 	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}


	cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

	if (*nbca == 1)
	{
          sprintf(TAB,"   %d",i+2);
	}
	if (*nbca == 3)
	{
	  strcpy(TAB,"   4");
	}

	cr_write = write ( descrip_leader , TAB , 4);
  	if (cr_write != 4)
  	{
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  	}

  	for (i=0;i<4000;i++)
  	{
		 strcpy(&TAB[j], " ");
  	}

        cr_write = write(descrip_leader, TAB, 3944);
	if (cr_write != 3944)
	{
		strcpy(CAI_ERREUR,"Erreur ecriture du fichier Leader Spot1a4");
		goto ERREUR;
	}
     }
  }


/**********************************/
/* Pour le fichier MAP PROJECTION */
/**********************************/

/* 1 RECORD */


  VAR_LONG[0] = 26;
  BYTE[0] = 36;
  BYTE[1] = 36;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;

  cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  strcpy(TAB, "   1");
  cr_write = write ( descrip_leader , TAB , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  for (i=0;i<4000;i++)
  {
	 strcpy(&TAB[i], " ");
  }

  cr_write = write ( descrip_leader , TAB , 3944);
  if (cr_write != 3944)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }

/*******************************/
/* Pour le fichier ANNOTATIONS */
/*******************************/

/* 1 RECORD */


  VAR_LONG[0] = 27;
  BYTE[0] = 18;
  BYTE[1] = 219;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 3960;

  cr_write = write ( descrip_leader , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  cr_write = write ( descrip_leader , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  strcpy(TAB, "   1   3");
  cr_write = write ( descrip_leader , TAB , 8);
  if (cr_write != 8)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

   strcpy(TAB, "                                             ");

  cr_write = write ( descrip_leader , TAB , 32);
  if (cr_write != 32)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }

  strcpy(TAB, " 180  36VT  ");
  cr_write = write ( descrip_leader , TAB , 12);
  if (cr_write != 12)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

 for (i=0;i<170;i++)
 { 
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write ( descrip_leader , TAB , 168);
  if (cr_write != 168)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }

  strcpy(TAB, " 360  98VZ1 ");
  cr_write = write ( descrip_leader , TAB , 12);
  if (cr_write != 12)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  for (i=0;i<350;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write ( descrip_leader , TAB , 348);
  if (cr_write != 348)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;

  }

  strcpy(TAB, " 540  60VZ2 ");
  cr_write = write ( descrip_leader , TAB , 12);
  if (cr_write != 12)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  for (i=0;i<1120;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write ( descrip_leader , TAB , 1112);
  if (cr_write != 1112)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	  goto ERREUR;
  }

  strcpy(TAB, "   4 8  ");
  cr_write = write ( descrip_leader , TAB , 8);
  if (cr_write != 8)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }

  for (i=0;i<2240;i++)
  {
	 strcpy(&TAB[i], " ");
  }

  cr_write = write ( descrip_leader , TAB , 2236);
  if (cr_write != 2236)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Leader Spot1a4");
	goto ERREUR;
  }
 
/*--------------------------------------------*/
/*       Fermeture du fichier LEADER          */
/*--------------------------------------------*/

  close (descrip_leader);


/******************************************************************/
/*   Constitution du nom du fichier TRAILER et verification qu'il */
/*                    n'existe pas deja	                          */				
/******************************************************************/
  sprintf(nom_fic,"%s/TRAI_01.DAT",nom_rep);
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , "Le fichier %s TRAILER SPOT1A4 existe \n" , 
				nom_image );
       goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*	Ouverture du fichier TRAILER en creation			     */
/*---------------------------------------------------------------------------*/

  descrip_trailer =  open (nom_fic,O_RDWR|O_CREAT|O_BINARY, PERMS );

  if (descrip_trailer == -1)
  {
  	strcpy (CAI_ERREUR,"Erreur ouverture du fichier TRAILER SPOT1A4 ");
  	goto ERREUR;
  }

/*---------------------------------------------------------------------------*/
/*		ECRITURE DU FICHIER TRAILER				     */
/*---------------------------------------------------------------------------*/

/***********************************/
/* Pour le fichier FILE DESCRIPTOR */
/***********************************/

/* 1 RECORD */

/* SEGMENT FIXE : */

  VAR_LONG[0] = 1;
  BYTE[0] = 63;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 1080;

  cr_write = write ( descrip_trailer , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_trailer , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }


  cr_write = write ( descrip_trailer , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "A   S4-ST-73-1CN 0 0               3");
  cr_write = write ( descrip_trailer , TAB , 36);
  if ( cr_write != 36)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  strcpy (TAB,"        TRAIBIL FSEQ       1   4FTYP       5   4FLGT       9   4YNNN");
  cr_write = write ( descrip_trailer , TAB , 68);
  if ( cr_write != 68)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<70;i++)
  {
	 strcpy(&TAB[i], " ");
  }
  
  cr_write = write ( descrip_trailer , TAB , 64);
  if ( cr_write != 64)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }


/* SEGMENT VARIABLE : */

  strcpy ( TAB , "   3    1080");
  cr_write = write ( descrip_trailer , TAB , 12);
  if ( cr_write != 12)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<30;i++)
  {
	 strcpy(&TAB[i], " ");
  }
  
  cr_write = write ( descrip_trailer , TAB , 24);
  if ( cr_write != 24)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	  goto ERREUR;
  }
  strcpy ( TAB , "     2    21  4N     2    35200A");
  cr_write = write ( descrip_trailer , TAB , 32);
  if ( cr_write != 32)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }
  for (i=0;i<835;i++)
  {
	 strcpy(&TAB[i], " ");
  }

  cr_write = write ( descrip_trailer , TAB , 832);
  if ( cr_write != 832)
  {
	  strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	  goto ERREUR;
  }


/***********************************/
/* Pour le fichier TRAILER-QUALITY */
/***********************************/

/* 1 RECORD */

/* SEGMENT FIXE : */

  VAR_LONG[0] = 2;
  BYTE[0] = 18;
  BYTE[1] = 246;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 1080;

  cr_write = write ( descrip_trailer , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_trailer , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_trailer , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "   1    ");
  cr_write = write ( descrip_trailer , TAB , 8);
  if ( cr_write != 8)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }


  /*ecriture dans le fichier trailer*/
  for (i=0;i<215;i++)
  {
	 strcpy(&TAB[i], " ");
  }
  cr_write = write ( descrip_trailer , TAB , 214);
  if ( cr_write != 214)
  {
	strcpy ( CAI_ERREUR , "Erreur ecriture du fichier Trailer SPOT1A4");
 	goto ERREUR;
  }
 
  strcpy ( TAB , "   1  1080");
  cr_write = write ( descrip_trailer , TAB , 10);
  if ( cr_write != 10)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<840;i++)
  {
	 strcpy(&TAB[i], " ");
  }

  cr_write = write ( descrip_trailer , TAB , 836);
  if ( cr_write != 836)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

/*********************************************/
/* Pour le fichier GEOMETRIC TRANSFORMATIONS */
/*********************************************/

/* 1 RECORD */

/* SEGMENT FIXE : */

  VAR_LONG[0] = 3;
  BYTE[0] = 18;
  BYTE[1] = 246;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 1080;

  cr_write = write ( descrip_trailer , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_trailer , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_trailer , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "   1");
  cr_write = write ( descrip_trailer , TAB , 4);
  if ( cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<1070;i++)
  {
	 strcpy(&TAB[i], " ");
  }

  cr_write = write ( descrip_trailer , TAB, 1064);
  if ( cr_write != 1064)
  {
	strcpy ( CAI_ERREUR, "Erreur ecriture du fichier Trailer SPOT1A4");
	goto ERREUR;
  }


/*----------------------------------------------*/
/*        Fermeture du fichier TRAILER          */
/*----------------------------------------------*/

  close (descrip_trailer);


/******************************************************************/
/*   Constitution du nom du fichier VOLUME_DIRECTORY et           */
/*      verification qu'il n'existe pas deja	                  */				
/******************************************************************/
  sprintf(nom_fic,"%s/VOLD_01.DAT",nom_rep);  
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR ,
		 "Le fichier %s VOLUME_DIRECTORY SPOT1A4 existe \n" ,
		 nom_image );
       goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*	Ouverture du fichier VOLUME DIRECTORY en creation		     */
/*---------------------------------------------------------------------------*/

  descrip_volume = open (nom_fic,O_RDWR|O_CREAT|O_BINARY, PERMS );
  if (descrip_volume == -1)
  {
  	strcpy (CAI_ERREUR,
		"Erreur de creation du fichier VOLUME DIRECTORY  SPOT1A4");
  	goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*		ECRITURE DU FICHIER VOLUME DIRECTORY			     */
/*---------------------------------------------------------------------------*/

/*************************************/
/* Pour le fichier VOLUME DESCRIPTOR */
/*************************************/

/* 1 RECORD */

  VAR_LONG[0] = 1;
  BYTE[0] =192;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 360;

  cr_write = write ( descrip_volume , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory  SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory  SPOT1A4");
	goto ERREUR;
  }


  cr_write = write ( descrip_volume , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory  SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "A   CCB-CCT-0002 A AECR_EXT_SPOT");
  cr_write = write ( descrip_volume, TAB, 32);
  if (cr_write != 32)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory  SPOT1A4");
	goto ERREUR;
  }

  
  for (i=0;i<70;i++)
  {
	 strcpy ( &TAB[i]," ");
  }
  cr_write = write ( descrip_volume, TAB, 68);
  if (cr_write != 68)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory  SPOT1A4");
	goto ERREUR;
  }


/* Recuperation de la date systeme */

  LocalTime = time(NULL);	/* Date calendaire */


/* Ecriture dans une chaine de caracteres */

  LgDate = 17;

  strftime ( StrDate , LgDate , "%Y%m%d%H%M%S  ",localtime(&LocalTime));

  cr_write = write ( descrip_volume,StrDate,16);
  if (cr_write != 16)
  {
	  strcpy ( CAI_ERREUR,
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }
 
  strcpy ( &TAB[33], "   3   5");
  cr_write = write ( descrip_volume , TAB , 232);
  if (cr_write != 232)
  {
	strcpy ( CAI_ERREUR,
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }



/************************************************/
/* Pour le fichier FILE POINTER for Leader File */
/************************************************/

/* 1 RECORD */

  VAR_LONG[0] = 2;
  BYTE[0] =219;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 360;

  cr_write = write ( descrip_volume , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB,"A      1SP      LEADBIL LEADER FILE ");
  cr_write = write ( descrip_volume, TAB, 36);
  if (cr_write != 36)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "                                   ");
  cr_write = write ( descrip_volume, TAB, 16);
  if (cr_write != 16)
  {
	  strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }

  strcpy ( TAB,"LEADMIXED BINARY AND ASCII      MBAA      27    3960    3960FIXED LENGHTFIXD    ");
  cr_write = write ( descrip_volume, TAB, 80);
  if (cr_write != 80)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }


  for (i=0;i<217;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write ( descrip_volume, TAB, 216);
  if (cr_write != 216)
  {
	  strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }


/*************************************************/
/* Pour le fichier FILE POINTER for Imagery File */
/*************************************************/

/* 1 RECORD */

  VAR_LONG[0] = 3;
  BYTE[0] =219;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 360;

  cr_write = write ( descrip_volume , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB,"A      2SP      IMGYBIL IMAGERY FIL ");
  cr_write = write ( descrip_volume, TAB, 36);
  if (cr_write != 36)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "                IMGYBINARY ONLY                 BINO");
  cr_write = write ( descrip_volume, TAB, 52);
  if (cr_write != 52)
  {
	  strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }

  strcpy ( TAB ,"000                     FIXED LENGTHFIXD");
  cr_write = write ( descrip_volume, TAB, 40);
  if (cr_write != 40)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }
 
   for (i=0;i<125;i++)
   {
	 strcpy ( &TAB[i]," ");
   }

  cr_write = write ( descrip_volume, TAB, 120);
  if (cr_write != 120)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }
 
  strcpy(TAB,"000     ");
  cr_write = write ( descrip_volume, TAB, 100);
  if (cr_write != 100)
  {
	  strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }


/*************************************************/
/* Pour le fichier FILE POINTER for Trailer File */
/*************************************************/

/* 1 RECORD */

  VAR_LONG[0] = 4;
  BYTE[0] = 219;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 360;

  cr_write = write ( descrip_volume , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB,"A      3SP      TRAIBIL TRAILER FILE");
  cr_write = write ( descrip_volume, TAB, 36);
  if (cr_write != 36)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }
  strcpy(TAB,"                                    ");
  cr_write = write ( descrip_volume, TAB, 16);
  if (cr_write != 16)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB,"TRAIMIXED BINARY AND ASCII      MBAA       3    1080    1080FIXED LENGTHFIXD");
  cr_write = write ( descrip_volume, TAB, 76);
  if (cr_write != 76)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory  SPOT1A4");
	goto ERREUR;
  }

   for (i=0;i<225;i++)
   {
	 strcpy ( &TAB[i]," ");
   }
  cr_write = write ( descrip_volume, TAB, 220);
  if (cr_write != 220)
  {
	  strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }

/************************/
/* Pour le fichier TEXT */
/************************/

/* 1 RECORD */

  VAR_LONG[0] = 5;
  BYTE[0] = 18;
  BYTE[1] = 63;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = 360;

  cr_write = write ( descrip_volume , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_volume , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }


  cr_write = write ( descrip_volume , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,
		 "Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB,"A   PRODUCT:            SPOT");
  cr_write = write ( descrip_volume, TAB, 28);
  if (cr_write != 28)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }


  strcpy ( TAB,"             MODE       BIL  LEVEL    \r\n");
  cr_write = write ( descrip_volume, TAB, 40);
  if (cr_write != 40)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Volume Directory");
	goto ERREUR;
  }

   for (i=0;i<50;i++)
   {
	 strcpy ( &TAB[i]," ");
   }

  cr_write = write ( descrip_volume, TAB, 40);
  if ( cr_write != 40)
  {
	strcpy ( CAI_ERREUR,
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }


  strcpy ( TAB , "PROD. CNES_QTIS   COPYRIGHT CNES   ");
  cr_write = write ( descrip_volume , TAB , 35);
  if (cr_write != 35)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

 
/* Recuperation de la date systeme */

  LocalTime = time(NULL);	/* Date calendaire */

/* Ecriture dans une chaine de caracteres */

  LgDate = 24;

  strftime ( StrDate , LgDate , "%d %m %Y - %HH%MMN%SS" , 
		localtime(&LocalTime));
  cr_write = write ( descrip_volume, StrDate , 23);
  if (cr_write != 23)
  {
	  strcpy ( CAI_ERREUR,
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }
 
  strcpy ( TAB ,"\r\nSCENE               -       THE               ");
  cr_write = write ( descrip_volume , TAB , 48);
  if (cr_write != 48)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB,"\r\nVOL : VOLUME 1 ORDER: ORDRE 1         \r\n");
  cr_write = write ( descrip_volume , TAB , 42);
  if (cr_write != 42)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	goto ERREUR;
  }

   for (i=0;i<95;i++)
   {
	 strcpy ( &TAB[i]," ");
   }

  cr_write = write ( descrip_volume , TAB , 92);
  if (cr_write != 92)
  {
	  strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Volume Directory SPOT1A4");
	  goto ERREUR;
  }

/*----------------------------------------------*/
/*        Fermeture du fichier VOLUME           */
/*----------------------------------------------*/

  close (descrip_volume);


/******************************************************************/
/*   Constitution du nom du fichier NULL_VOLUME_DIRECTORY         */ 
/*          et verification qu'il n'existe pas deja	          */				
/******************************************************************/
  sprintf(nom_fic,"%s/NULL_01.DAT",nom_rep);
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , 
		"Le fichier %s NULL_VOLUME_DIRECTORY  SPOT1A4 existe \n" , 
		nom_image );
       goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*	Ouverture du fichier NULL VOLUME DIRECTORY en creation		     */
/*---------------------------------------------------------------------------*/

  descrip_null = open (nom_fic,O_RDWR|O_CREAT|O_BINARY, PERMS );
  if (descrip_null == -1)
  {
  	strcpy (CAI_ERREUR,
		"Erreur de creation du fichier NULL VOLUME DIRECTORY SPOT1A4");
  	goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*		ECRITURE DU FICHIER NULL VOLUME DIRECTORY		     */
/*---------------------------------------------------------------------------*/

/******************************************/
/* Pour le fichier NULL VOLUME DESCRIPTOR */
/******************************************/

/* 1 RECORD */

  VAR_LONG[0] = 1;
  BYTE[0] =192;
  BYTE[1] = 192;
  BYTE[2] = 63;
  BYTE[3] = 18;
  VAR_LONG[1] = 360;

  cr_write = write ( descrip_null , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Null Volume Directory SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_null , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Null Volume Directory SPOT1A4");
	goto ERREUR;
  }


  cr_write = write ( descrip_null , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Null Volume Directory SPOT1A4");
	goto ERREUR; 
 }

  strcpy ( TAB , "A   CCB-CCT-0002 A A");
  cr_write = write ( descrip_null, TAB, 20);
  if (cr_write != 20)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Null Volume Directory SPOT1A4");
	goto ERREUR; 
 }

  for (i=0;i<340;i++)
  {
	 strcpy ( &TAB[i]," ");
  }
  cr_write = write ( descrip_null, TAB, 328);
  if (cr_write != 328)
  {
	strcpy ( CAI_ERREUR, 
		"Erreur d'ecriture du fichier Null Volume Directory SPOT1A4");
	goto ERREUR; 
 }
 


/*-------------------------------------------*/
/*    Fermeture du fichier NULL VOLUME       */
/*-------------------------------------------*/

  close (descrip_null);


/******************************************************************/
/*   Constitution du nom du fichier IMAGERY et verification qu'il */
/*                    n'existe pas deja	                          */				
/******************************************************************/
  sprintf(nom_fic,"%s/IMAG_01.DAT",nom_rep); 
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , "Le fichier %s IMAGERY SPOT1A4 existe \n" , 
			nom_image );
       goto ERREUR;
  }


/*---------------------------------------------------------------------------*/
/*	Ouverture du fichier IMAGERY en creation		             */
/*---------------------------------------------------------------------------*/

  descrip_imagery = open (nom_fic,O_RDWR|O_CREAT|O_BINARY, PERMS );
  if (descrip_imagery == -1)
  {
	strcpy ( CAI_ERREUR , "Erreur de creation du fichier Imagerie SPOT1A4");
    	goto ERREUR;
  }

/*---------------------------------------------------------------------------*/
/*		ECRITURE DU FICHIER IMAGERY				     */
/*---------------------------------------------------------------------------*/

/***********************************/
/* Pour le fichier FILE DESCRIPTOR */
/***********************************/

/* SEGMENT FIXE : */

  /* Initialisation de nbrec et lgrec et des autres donnees utiles */
   nb = (*nbcol)*(*nboct_pix);
  /* Definition de lgrec : */
  if (nb<=5300) 
	 lgrec = 5400;
  else if ((nb>5300) && (nb<=8540))
	 lgrec = 8640;
  else if ((nb>8540) && (nb<=10880))
	 lgrec = 10980;
  else if ((nb>10880) && (nb<=12140))
	 lgrec = 12240;
  else
  {
	strcpy ( CAI_ERREUR , "Enregistrement trop long pour le fichier imagery");
    	goto ERREUR;
  }


  /* 1 RECORD */
  VAR_LONG[0] = 1;
  BYTE[0] = 63;
  BYTE[1] = 192;
  BYTE[2] = 18;
  BYTE[3] = 18;
  VAR_LONG[1] = lgrec;


  nbrec = (*nbca)*(*nblig);
  if (nbrec<10000)
  {
	 strcpy(&TAMP2[0][0]," ");
	 strcpy(&TAMP2[0][1]," ");
	 sprintf ( str_int4,"%d",nbrec);
  	 for (i=0;i<4;i++)
  	 {
	  strcpy(&TAMP2[0][i+2],&str_int4[i]); 
  	 }
  }
  else
  { 
	 strcpy(&TAMP2[0][0]," ");
	 sprintf (str_int5,"%d",nbrec);
	 for (i=0;i<5;i++)
	 {
		strcpy(&TAMP2[0][i+1],&str_int5[i]);
	 }
  }


  if (lgrec<10000)
  {
	 strcpy(&TAMP2[1][0]," ");
	 strcpy(&TAMP2[1][1]," ");
	 sprintf ( str_int4,"%d",lgrec);
  	 for (i=0;i<4;i++)
  	 {
	  	strcpy(&TAMP2[1][i+2],&str_int4[i]); 
  	 }
  }
  else
  {
	 strcpy(&TAMP2[1][0]," ");
	 sprintf (str_int5,"%d",lgrec);
	 for (i=0;i<5;i++)
	 {
		strcpy(&TAMP2[1][i+1],&str_int5[i]);
	 }
  }


  /* Initialisation du nombre de colonnes et du nombre de lignes : */

  for (i=0;i<3;i++)
  {
	 strcpy ( &TAMP2[2][i]," ");
	 strcpy ( &TAMP2[3][i]," ");
  }

  if (*nblig < 10000)
  {
	 strcpy ( &TAMP2[2][3] , "0");
	 sprintf( str_int4 , "%d" , *nblig);
	 for (j=0;j<4;j++)
	 {
		strcpy(&TAMP2[2][j+4],&str_int4[j]);
	 }
  }
  else
  {
	 sprintf( str_int5, "%d", *nblig);
	 for (j=0;j<5;j++)
	 {
		strcpy ( &TAMP2[2][j+3],&str_int5[j]);
	 }
  }

  if (*nbcol < 10000)
  {
	 strcpy ( &TAMP2[3][3] , "0");
	 sprintf ( str_int4 , "%d" , *nbcol);
	 for (j=0;j<4;j++)
	 {
		strcpy ( &TAMP2[3][j+4],&str_int4[j]);
	 }
  }
  else
  {
	 sprintf ( str_int5 , "%d" , *nbcol);
	 for ( j=0;j<5;j++)
	 {
		strcpy ( &TAMP2[3][j+3],&str_int5[j]);
	 }
  }
 
  for (i=0;i<4;i++)
  {
	  strcpy(&TAMP2[4][i]," "); 
  }


  cr_write = write ( descrip_imagery , &VAR_LONG[0] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  cr_write = write ( descrip_imagery , BYTE , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }


  cr_write = write ( descrip_imagery , &VAR_LONG[1] , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB, "A   S4-ST-73-1CN 0 0               2");
  cr_write = write ( descrip_imagery , TAB , 36);
  if ( cr_write != 36)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }
  
/* Initialisation du mode spectral et du niveau : */
   if ((mode[0]=='X')&&(mode[1]=='I'))
         sprintf(TAB,"    I0  ");
   else
         sprintf(TAB,"    %.1s0  ",&mode[0]);
  

  cr_write = write (descrip_imagery, TAB, 8);
  if (cr_write != 8)
  {
	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  /* Initialisation du niveau de traitement : */

  LEVEL[0]=' ';
  strcpy(TAB, "IMGYBIL FSEQ       1   4FTYP       5   4FLGT       9   4YNYN");
  cr_write = write (descrip_imagery, TAB, 60);
  if (cr_write != 60)
  {
	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<70;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write (descrip_imagery, TAB, 64);
  if (cr_write != 64)
  {
	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }


/* SEGMENT VARIABLE : */

  cr_write = write (descrip_imagery , TAMP2 , 6); 
  if (cr_write != 6)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

 cr_write = write (descrip_imagery , TAMP2[1] , 6); 
  if (cr_write != 6)
  {
	strcpy ( CAI_ERREUR, "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }


  cr_write = write (descrip_imagery, TAB, 24);
  if (cr_write != 24)
  {
 	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  if (*nboct_pix == 1)
  {
	strcpy ( TAB ,"   8");
  }
  else if (*nboct_pix == 2)
  {
	strcpy ( TAB, "  16");
  }
  

  cr_write = write (descrip_imagery, TAB, 4);
  if (cr_write != 4)
  {
 	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

   for (i=0;i<15;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  strcpy(TAB, "   1   1    ");
  cr_write = write (descrip_imagery, TAB, 12);
  if (cr_write != 12)
  {
 	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }


  sprintf(TAB,"%4.d",*nbca);
  cr_write = write ( descrip_imagery , TAB , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  cr_write = write (descrip_imagery, TAMP2[2], 8);
  if (cr_write != 8)
  {
	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<5;i++)
  {
	 strcpy ( &TAB[i]," ");
  }
 

  strcpy ( TAB, "   0");
  cr_write = write ( descrip_imagery , TAB , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }
  
 cr_write = write (descrip_imagery, TAMP2[3], 8);
  if (cr_write != 8)
  {
	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  cr_write = write (descrip_imagery, TAMP2[4], 4);
  if (cr_write != 4)
  {
	strcpy(CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<16;i++)
  {
	 strcpy ( &TAB[i]," ");
  }
 
  strcpy ( TAB, "   0   0BIL  1");
  cr_write = write ( descrip_imagery , TAB , 14);
  if (cr_write != 14)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  sprintf(TAB,"%4.d",*nbca);

  cr_write = write ( descrip_imagery , TAB , 2);
  if (cr_write != 2)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

 
  for (i=0;i<6;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  strcpy ( TAB, "  20");
  cr_write = write ( descrip_imagery , TAB , 4);
  if (cr_write != 4)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  if (lgrec == 5400)
  {
	strcpy(TAB , "    5300");
  }
  else
  {
	if (lgrec == 8640)
	{
	  strcpy(TAB , "    8540");
	}
	else
	{
	  if (lgrec == 10980)
	  {
		strcpy(TAB, "   10880");
	  }
	  else strcpy(TAB, "   12140");
	}
  }
  cr_write = write ( descrip_imagery , TAB , 8);
  if (cr_write != 8)
  {
	strcpy ( CAI_ERREUR , "Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB, "  28       1 4PB   5 4PB");
  cr_write = write ( descrip_imagery , TAB , 24);
  if (cr_write != 24)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }
  strcpy ( TAB, "                              ");
  if ( LEVEL[0] == 'A')
  {
	strcpy(TAB,"   9 4PB");
  }
  if (( LEVEL[0] == 'B') || (LEVEL[0] == ' '))
  {
	strcpy(TAB,"        ");
  }
  cr_write = write ( descrip_imagery , TAB , 8);
  if (cr_write != 8)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB , "  13 4PB  17 4PB");
  cr_write = write ( descrip_imagery , TAB , 16);
  if (cr_write != 16)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  for (i=0;i<40;i++)
  {
	 strcpy ( &TAB[i]," ");
  }

  cr_write = write ( descrip_imagery , TAB , 32);
  if (cr_write != 32)
  {
	  strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	  goto ERREUR;
  }
 
  if ( LEVEL[0] == 'A')
  {
	strcpy(TAB,"   1 2PB");
  }
  if (( LEVEL[0] == 'B')  || ( LEVEL[0] == ' '))
  {
	strcpy(TAB,"        ");
  }
  cr_write = write ( descrip_imagery , TAB , 8);
  if (cr_write != 8)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

 for (i=0;i<60;i++)
  {
	 strcpy ( &TAB[i]," ");
  }  

  cr_write = write ( descrip_imagery , TAB , 56);
  if (cr_write != 56)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }

  strcpy ( TAB, "   0   0     254");
  cr_write = write ( descrip_imagery , TAB , 16);
  if (cr_write != 16)
  {
	strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	goto ERREUR;
  }


/* Mise a blanc de lgrec-180-268 = lgrec-448 octets */

  for (i=0;i<5000;i++)
  {
	 strcpy ( &TAB[i]," ");
  }  
  
  k = lgrec-448;
  cr_write = write ( descrip_imagery , TAB , k);
  if (cr_write != k)
  {
	  strcpy ( CAI_ERREUR,"Erreur d'ecriture du fichier Imagery SPOT1A4");
	  goto ERREUR;
  }

/*------------------------------------*/
/* Ecriture de l'image entiere a vide */
/*------------------------------------*/
  nb=(*nblig)*(*nbca);
  for (i=0;i<lgrec;i++)
  {
	 TAB[i]=0;
  }  
  
  for (i=0;i<nb;i++)
  {
 	cr_write = write ( descrip_imagery , TAB , lgrec);
  	if (cr_write != lgrec)
  	{
	  strcpy ( CAI_ERREUR,
		"Erreur d'ecriture du fichier Imagery SPOT1A4 a vide");	
	  goto ERREUR;
  	}
    }

  /*stockage de la long enregistrement et port */

 for (i=0;i<*nbca;i=i+1)
   {
    image1->ACCES_CANAL_INT[i]=descrip_imagery;
    image1->POS_OCTET[i]=lgrec;
   }
  image1->TYPE_SPOT = 1;

    /* Sortie sans erreur : */
  return(image1);
 
/* Sorties degradees : */

ERREUR:

/* Fermeture des fichiers et destructions */
  if (descrip_leader>0) close(descrip_leader);
  if (descrip_trailer>0) close(descrip_trailer);
  if (descrip_volume>0) close(descrip_volume);
  if (descrip_null>0) close(descrip_null);
  if (descrip_imagery>0) close(descrip_imagery);
  if (Existe==0)
  {
     sprintf(Command,"rm -R %s",nom_rep);
     system(Command);
  }
  else
  {
      sprintf(nom_fic,"%s/LEAD_01.DAT",nom_rep);
      unlink(nom_fic);
      sprintf(nom_fic,"%s/TRAI_01.DAT",nom_rep);
      unlink(nom_fic);
      sprintf(nom_fic,"%s/VOLD_01.DAT",nom_rep);
      unlink(nom_fic);
      sprintf(nom_fic,"%s/IMAG_01.DAT",nom_rep);
      unlink(nom_fic);
      sprintf(nom_fic,"%s/NULL_01.DAT",nom_rep);
      unlink(nom_fic);
  }
     
  return(NULL);

}
/* 
  Fin de l'operation cai_ouvre_creation_spot1a4
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_spot5TIF                                */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format spot5 TIFF et de tous les fichiers qui la composent   */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_spot5TIF(repert,         */
/*  ---------------- 		                       nom_image,nb_can,     */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     repert	      (E) (char *) : repertoire	de stockage		     */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir                 */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_spot5TIF (S) (CAI_IMAGE *) :  =NULL si pb          */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
static CAI_IMAGE *cai_ouvre_creation_spot5TIF(char *repert,
				   		char *nom_image,
				   		int *nbca,
				   		int *nboct_pix,
				   		int *nbcol,
				   		int *nblig,
				   		char *label)

{
   char nom_fic[1024];	     /* nom du fichier leader    	             */
   char nom_ficM[1024];	     /* nom du fichier leader renomme 	             */
   char nom_rep[1024];	     /* nom du repertoire image 	             */
   CAI_OK_KO icr;            /* compte rendu d'operation                     */
   struct stat	buf_stat;    /* structure decrivant status de fichier        */
   CAI_IMAGE *image1;        /* descripteur image			     */
   int Existe;		     /* Indicateur d'existence du repertoire image   */
   int retour,cr;            /* retour de fonction stat                      */
	 
/*........................................
   INITIALISATIONS et allocation de place pour la structure CAI_IMAGE
  .........................................*/
  Existe = 0;

/******************************************************************/
/*   Constitution du repertoire image s'il n'existe pas deja      */
/******************************************************************/

  nom_fic[0]='\0';
  if ((long)strlen(repert)!= 0)
  {
      sprintf(nom_rep,"%s/%s",repert,nom_image);     
  }
  else
  {
      sprintf(nom_rep,"%s",nom_image);
  }

  retour = stat ( nom_rep , &buf_stat );
    
  /* si le repertoire image n'existe pas, le creer*/  
  if ( retour != 0 )
  {  
      cr = mkdir (nom_rep,0775);
      if (cr !=0)
      {
         sprintf ( CAI_ERREUR , 
		"probleme a la creation du repertoire image %s SPOT5TIFF \n" ,
		 nom_image );
         retour =0;
         goto ERREUR;
      }
  }
  else
	Existe = 1;

/******************************************************************/
/*   Constitution du nom du fichier IMAGERY et verification qu'il */
/*                    n'existe pas deja	                          */				/******************************************************************/
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM :  IMAG_01.TIF  devient IMAGERY.TIF  */
/*****************************************************************/
  sprintf(nom_fic,"%s/IMAGERY.TIF",nom_rep); 
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , "Le fichier %s IMAGERY SPOT5TIFF existe \n" , 
			nom_image );
       goto ERREUR;
  }
  sprintf(nom_fic,"IMAGERY"); 
  image1 = cai_ouvre_creation_tiff(nom_rep,nom_fic,nbca,
				   nboct_pix,nbcol,nblig,label);
  if (image1 == NULL)
     goto ERREUR;
/******************************************************************/
/*   fichier LEADER : verification s'il existe pas deja           */
/******************************************************************/
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM : LEAD_01.DAT  devient METADATA.DIM  */
/*****************************************************************/

  sprintf(nom_fic,"%s/METADATA.DIM",nom_rep);
  
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR ,"Le fichier %s METADATA.DIM SPOT5TIFF existe \n" , 
							nom_image );
       goto ERREUR;
  }

/******************************************************************/
/*   fichier METADATA.DIM : constitution                          */
/******************************************************************/
   image1->NB_LIG = (*nblig);
   image1->NB_COL = (*nbcol);
   image1->NB_CANAUX = (*nbca);
   image1->NBOCT_PIX = (*nboct_pix);
   image1->TYPE_SPOT = 2;
   icr = cai_creat_leader_spot5(image1,nom_fic);
   if (icr !=CAI_OK)
   {
       sprintf ( CAI_ERREUR , "Erreur creation METADATA.DIM SPOT5TIFF" );
       goto ERREUR;

   }

/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Remplacer les appels systemes pour renommer un */
/*                            fichier par la commande C rename               */
/*****************************************************************************/
    /* Sortie sans erreur : */
/* renommer l'image en majuscule */
     sprintf(nom_fic,"%s/IMAGERY.tif",nom_rep);
     sprintf(nom_ficM,"%s/IMAGERY.TIF",nom_rep);
     rename(nom_fic,nom_ficM);

  return(image1);
 
/* Sorties degradees : */

ERREUR:
/* destruction du fichier leader et de l'image */
/* et s'il n'existe pas du repertoire */ 
  if (image1 != NULL)
     icr = cai_ferme_tiff (image1);
 
  if (Existe==0)
  {
     sprintf(Command,"rm -R %s",nom_rep);
     system(Command);
  }
  return(NULL);

}
/* 
  Fin de l'operation cai_ouvre_creation_spot5TIF
*/
/*****************************************************************************/
/*                                                                           */
/* OPERATION :    cai_ouvre_creation_spot5BIL                                */
/* ----------								     */
/*                                                                           */
/* ROLE :    fonction de creation et d'ouverture en ecriture d'une image     */
/* -----     au format spot5 BIL et de tous les fichiers qui la composent    */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* CONTEXTE D'APPEL : CAI_IMAGE *cai_ouvre_creation_spot5BIL(repert,         */
/*  ---------------- 		                       nom_image,nb_can,     */
/*				                            nboct_pix,	     */
/*				                            nb_col,          */
/*				                            nb_lig,          */
/*				                            label)           */
/*                                                                           */
/*     repert	      (E) (char *) : repertoire	de stockage		     */
/*     nom_image      (E) (char *) : nom de l'image a ouvrir                 */
/*     nb_can         (E) (int )   : nombre de canaux de l'image             */
/*     nboct_pix      (E) (int )   : nombre d'octets par pixels (1 ou 2)     */
/*     nb_lig         (E) (int )   : nombre de lignes de l'image             */
/*     nb_col         (E) (int )   : nombre de colonnes de l'image           */
/*     label          (E) (char *) : commentaire lie a l'image               */
/*                                                                           */
/*     cai_ouvre_creation_spot5BIL (S) (CAI_IMAGE *) :  =NULL si pb          */
/*                                                    = structure contenant  */ 
/*				toutes les informations necessaires au       */
/*				traitement de l'image                        */ 
/*                                                                           */
/* VERSION : V1                                                              */
/* --------                                                                  */
/*                                                                           */
/*****************************************************************************/
static CAI_IMAGE *cai_ouvre_creation_spot5BIL(char *repert,
				   		char *nom_image,
				   		int *nbca,
				   		int *nboct_pix,
				   		int *nbcol,
				   		int *nblig,
				   		char *label)
{
   char nom_fic[1024];	     /* nom du fichier leader    	             */
   char nom_ficM[1024];	     /* nom du fichier leader renomme	             */
   char nom_rep[1024],*Fic;  /* nom du repertoire image 	             */
   CAI_OK_KO icr;            /* compte rendu d'operation                     */
   struct stat	buf_stat;    /* structure decrivant status de fichier        */
   CAI_IMAGE *image1;        /* descripteur image			     */
   int Existe;		     /* Indicateur d'existence du repertoire image   */
   int retour,cr;            /* retour de fonction stat                      */

/*........................................
   INITIALISATIONS et allocation de place pour la structure CAI_IMAGE
  .........................................*/
  Existe = 0;

/******************************************************************/
/*   Constitution du repertoire image s'il n'existe pas deja      */
/******************************************************************/

  nom_fic[0]='\0';
  if ((long)strlen(repert)!= 0)
  {
      sprintf(nom_rep,"%s/%s",repert,nom_image);     
  }
  else
  {
      sprintf(nom_rep,"%s",nom_image);
  }

  retour = stat ( nom_rep , &buf_stat );
    
  /* si le repertoire image n'existe pas, le creer*/  
  if ( retour != 0 )
  {  
      cr = mkdir (nom_rep,0775);
      if (cr !=0)
      {
          sprintf ( CAI_ERREUR , 
		"probleme a la creation du repertoire image %s SPOT5BIL \n" ,
		 nom_image );
           goto ERREUR;
      }
  }
  else
	Existe = 1;

/******************************************************************/
/*   Constitution du nom du fichier IMAGERY et verification qu'il */
/*                    n'existe pas deja	                          */				/******************************************************************/
/****************************************************************/
/* MOD : VERSION : 4.7 : DM :  IMAG_XX.bil  devient IMAGERY.BIL */
/****************************************************************/
  sprintf(nom_fic,"%s/IMAGERY.BIL",nom_rep); 
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , "Le fichier %s IMAGERY SPOT5BIL existe \n" , 
			nom_image );
       goto ERREUR;
  }
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : Correction sprintf(nom_fic,"IMAGERY") au lieu  */
/*                                de sprintf(nom_fic,"IMAGERY",nom_rep)      */
/*****************************************************************************/
  sprintf(nom_fic,"IMAGERY");
  image1 = cai_ouvre_creation_bil(nom_rep,nom_fic,nbca,
				   nboct_pix,nbcol,nblig,label);
  if (image1 == NULL)
     goto ERREUR;
  else
  {
/* destruction du fichier .bil_hd qui n'a aucune utilite pour Spot5 */
      sprintf(nom_fic,"%s/IMAGERY.bil_hd",nom_rep);
      unlink(nom_fic);
   }
/******************************************************************/
/*   fichier LEADER : verification s'il existe pas deja           */
/******************************************************************/
/*****************************************************************/
/* MOD : VERSION : 4.7 : DM :  LEAD_01.DAT  devient METADATA.DIM */
/*****************************************************************/
  sprintf(nom_fic,"%s/METADATA.DIM",nom_rep);
  
  retour = stat ( nom_fic , &buf_stat );
  if ( retour == 0 )
  { 
       sprintf ( CAI_ERREUR , "Le fichier %s METADATA.DIM SPOT5BIL existe \n" , 
							nom_image );
       goto ERREUR;
  }

/******************************************************************/
/*   fichier METADATA.DIM : constitution                          */
/******************************************************************/
   image1->NB_LIG = (*nblig);
   image1->NB_COL = (*nbcol);
   image1->NB_CANAUX = (*nbca);
   image1->NBOCT_PIX = (*nboct_pix);
   image1->TYPE_SPOT = 3;
   icr = cai_creat_leader_spot5(image1,nom_fic);
   if (icr !=CAI_OK)
   {
       sprintf ( CAI_ERREUR , "Erreur creation METADATA.DIM SPOT5BIL" );
       goto ERREUR;

   }

/*****************************************************************************/
/* MOD : VERSION : 5.0 : DM : Remplacer les appels systemes pour renommer un */
/*                            fichier par la commande C rename               */
/*****************************************************************************/
    /* Sortie sans erreur : */
/* renommer l'image en majuscule */
     sprintf(nom_fic,"%s/IMAGERY.bil",nom_rep);
     sprintf(nom_ficM,"%s/IMAGERY.BIL",nom_rep);
     rename(nom_fic,nom_ficM);

     return(image1);
 
/* Sorties degradees : */

ERREUR:
/* destruction du fichier leader et de l'image */
/* et s'il n'existe pas du repertoire */ 
  if (image1 != NULL)
     icr = cai_ferme_bil (image1);
 
  if (Existe==0)
  {
     sprintf(Command,"rm -R %s",nom_rep);
     system(Command);
  }
  return(NULL);

}
/* 
  Fin de l'operation cai_ouvre_creation_spot5BIL
*/
/******************************************************************************/
/*                                                                            */
/*OPERATION  : cai_creat_leader_spot5                                         */
/*                                                                            */
/*ROLE : Cette operation genère un fichier XML de donnees auxiliaires SPOT5   */
/*       a partir des donnees cai_image passees en parametre                  */
/*                                                                            */
/*CONTEXTE D'APPEL :                                                          */
/*                                                                            */
/*RETOUR : CAI_OK si tout va bien , CAI_KO sinon                              */
/*                                                                            */
/******************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 4.5 : FA : erreur d'orthographe sur LATITUDE_ARGUMENT     */
/*			      et Look_Angles				     */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA : erreur initialisation BYTEORDER en creation    */
/*****************************************************************************/

CAI_OK_KO cai_creat_leader_spot5 ( CAI_IMAGE *desc_im  , char *nom_fich )
 {
  /* Code retour */
  CAI_OK_KO cr;
  /* Fichier XML a generer */
  FILE *des;
  /* Nombre de lignes de l'image */
  int nblig;
  /* Nombre de colonnes */
  int nbcol;
  /* Nombre de canaux */
  int nbcan;
  /* Nombre d'octets par pixel */
  int nboct;
  /* type de SPOT ( TIFF ou BIL ) */
  int typeimg;
  int type;
  
  int i;
  
  char valeur[200];
 
  /* affectation des variables */
  
  nblig = desc_im->NB_LIG;
  nbcol = desc_im->NB_COL;
  nbcan = desc_im->NB_CANAUX;
  nboct = desc_im->NBOCT_PIX;
  typeimg = desc_im->TYPE_SPOT;

  /********************************************************/
  /*         OUVERTURE DU FICHIER XML PRODUIT             */
  /********************************************************/

  if ( ( des = fopen ( nom_fich , "wb" ) ) == NULL )
    {
    sprintf ( CAI_ERREUR , "Impossible de creer le fichier XML %s" , nom_fich );
    goto ERREUR;
    }

  /********************************************************/
  /*      ECRITURE DES DONNEES XML DANS LE FICHIER        */
  /********************************************************/

  /* Entete du document XML */

  cr = cai_WriteEnteteXML ( des );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture entete XML" );
    goto ERREUR;
    }
  
  /* Element Dimap_Document */
 
  cr = cai_WriteOpenTagXML ( des , "Dimap_Document" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dimap_Document" );
    goto ERREUR;
    }

  /* Sous element Metadata_Id */
  
  cr = cai_WriteOpenTagXML ( des , "Metadata_Id" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Metadata_Id" );
    goto ERREUR;
    }

  /* sous element METADATA_FORMAT */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Version DIMAP passe de 2.0 à 2.9               */
/*****************************************************************************/
  type=0;
  cr = cai_WriteElementXMLWithAttribute ( des , "METADATA_FORMAT" , "DIMAP" ,
                                          "version" , "2.9",type );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element METADATA_FORMAT" );
    goto ERREUR;
    }

  /* sous element METADATA_PROFILE */

  cr = cai_WriteElementXML ( des , "METADATA_PROFILE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element METADATA_PROFILE" );
    goto ERREUR;
    }
  
 /* Fermeture de l'element Metadata_Id */

  cr = cai_WriteCloseTagXML ( des , "Metadata_Id" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Metadata_Id" );
    goto ERREUR;
    }
  
 /* Sous element Dataset_Id */
  
  cr = cai_WriteOpenTagXML ( des , "Dataset_Id" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dataset_Id" );
    goto ERREUR;
    }

  /* sous element DATASET_NAME */
  
  cr = cai_WriteElementXML ( des , "DATASET_NAME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_NAME" );
    goto ERREUR;
    }

  /* sous element COPYRIGHT */

  cr = cai_WriteElementXML ( des , "COPYRIGHT" , "COPYRIGHT CNES" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element COPYRIGHT" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'attribut href=""                   */
/****************************************************************************/

  /* sous element DATASET_TN_PATH */
  type=1;
  cr = cai_WriteElementXMLWithAttribute ( des , "DATASET_TN_PATH" , "" , "href" , "" ,type);
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_TN_PATH" );
    goto ERREUR;
    }

  /* sous element DATASET_TN_FORMAT */

  cr = cai_WriteElementXML ( des , "DATASET_TN_FORMAT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_TN_FORMAT" );
    goto ERREUR;
    }

  /* sous element DATASET_QL_PATH */
  type=1;
  cr = cai_WriteElementXMLWithAttribute ( des , "DATASET_QL_PATH" , "" , "href" , "" ,type);
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_QL_PATH" );
    goto ERREUR;
    }

  /* sous element DATASET_QL_FORMAT */

  cr = cai_WriteElementXML ( des , "DATASET_QL_FORMAT" , "" );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_QL_FORMAT" );
    goto ERREUR;
    }

  /* Fermeture de l'element Dataset_Id */

  cr = cai_WriteCloseTagXML ( des , "Dataset_Id" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dataset_Id" );
    goto ERREUR;
    }

  /* sous element Dataset_Sources */
  
  cr = cai_WriteOpenTagXML ( des , "Dataset_Sources" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dataset_Sources" );
    goto ERREUR;
    }

  /* sous element Source_Information */
  
  cr = cai_WriteOpenTagXML ( des , "Source_Information" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Source_Information" );
    goto ERREUR;
    }

  /* sous element SOURCE_TYPE */
  
  cr = cai_WriteElementXML ( des , "SOURCE_TYPE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SOURCE_TYPE" );
    goto ERREUR;
    }

  /* sous element SOURCE_ID */
  
  cr = cai_WriteElementXML ( des , "SOURCE_ID" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SOURCE_ID" );
    goto ERREUR;
    }

  /* sous element SOURCE_DESCRIPTION */
  
  cr = cai_WriteElementXML ( des , "SOURCE_DESCRIPTION" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SOURCE_DESCRIPTION" );
    goto ERREUR;
    }

  /* sous element Scene_Source */
  
  cr = cai_WriteOpenTagXML ( des , "Scene_Source" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Scene_Source" );
    goto ERREUR;
    }

  /* sous element IMAGING_DATE */
  
  cr = cai_WriteElementXML ( des , "IMAGING_DATE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element IMAGING_DATE" );
    goto ERREUR;
    }

  /* sous element IMAGING_TIME */
  
  cr = cai_WriteElementXML ( des , "IMAGING_TIME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element IMAGING_TIME" );
    goto ERREUR;
    }

  /* sous element MISSION */
  
  cr = cai_WriteElementXML ( des , "MISSION" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element MISSION" );
    goto ERREUR;
    }

  /* sous element MISSION_INDEX */
  
  cr = cai_WriteElementXML ( des , "MISSION_INDEX" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element MISSION_INDEX" );
    goto ERREUR;
    }

  /* sous element INSTRUMENT */
  
  cr = cai_WriteElementXML ( des , "INSTRUMENT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element INSTRUMENT" );
    goto ERREUR;
    }

  /* sous element INSTRUMENT_INDEX */
  
  cr = cai_WriteElementXML ( des , "INSTRUMENT_INDEX" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element INSTRUMENT_INDEX" );
    goto ERREUR;
    }

  /* sous element SENSOR_CODE */
  
  cr = cai_WriteElementXML ( des , "SENSOR_CODE" , "" );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SENSOR_CODE" );
    goto ERREUR;
    }

  /* sous element INCIDENCE_ANGLE */
  
  cr = cai_WriteElementXML ( des , "INCIDENCE_ANGLE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element INCIDENCE_ANGLE" );
    goto ERREUR;
    }

  /* sous element SUN_AZIMUT */
  
  cr = cai_WriteElementXML ( des , "SUN_AZIMUTH" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SUN_AZIMUT" );
    goto ERREUR;
    }

  /* sous element SUN_ELEVATION */
  
  cr = cai_WriteElementXML ( des , "SUN_ELEVATION" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SUN_ELEVATION" );
    goto ERREUR;
    }

  /* sous element Imaging_Parameters */
  
  cr = cai_WriteOpenTagXML ( des , "Imaging_Parameters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Imaging_Parameters" );
    goto ERREUR;
    }

  /* sous element REVOLUTION_NUMBER */
  
  cr = cai_WriteElementXML ( des , "REVOLUTION_NUMBER" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element REVOLUTION_NUMBER" );
    goto ERREUR;
    }

  /* sous element COMPRESSION_MODE */
  
  cr = cai_WriteElementXML ( des , "COMPRESSION_MODE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element COMPRESSION_MODE" );
    goto ERREUR;
    }

  /* sous element DIRECT_PLAYBACK_INDICATOR */
  
  cr = cai_WriteElementXML ( des , "DIRECT_PLAYBACK_INDICATOR" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DIRECT_PLAYBACK_INDICATOR" );
    goto ERREUR;
    }

  /* sous element REFOCUSING_STEP_NUM */
  
  cr = cai_WriteElementXML ( des , "REFOCUSING_STEP_NUM" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element REFOCUSING_STEP_NUM" );
    goto ERREUR;
    }

  /* sous element SWATH_MODE */
  
  cr = cai_WriteElementXML ( des , "SWATH_MODE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SWATH_MODE" );
    goto ERREUR;
    }

 /* Fermeture de l'element Imaging_Parameters */

  cr = cai_WriteCloseTagXML ( des , "Imaging_Parameters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Imaging_Parameters" );
    goto ERREUR;
    }

 /* Fermeture de l'element Scene_Source */

  cr = cai_WriteCloseTagXML ( des , "Scene_Source" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Scene_Source" );
    goto ERREUR;
    }

 /* Fermeture de l'element Source_Information */

  cr = cai_WriteCloseTagXML ( des , "Source_Information" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Source_Information" );
    goto ERREUR;
    }

 /* Fermeture de l'element Dataset_Sources */

  cr = cai_WriteCloseTagXML ( des , "Dataset_Sources" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dataset_Sources" );
    goto ERREUR;
    }

  /* sous element Coordinate_Reference_System */

  cr = cai_WriteOpenTagXML ( des , "Coordinate_Reference_System" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Coordinate_Reference_System" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'attribut version="5.2"              */
/*****************************************************************************/

  /* sous element GEO_TABLES */
    type=0;
  cr = cai_WriteElementXMLWithAttribute ( des , "GEO_TABLES" , "" , "version" , "5.2" ,type);
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element GEO_TABLES" );
    goto ERREUR;
    }

  /* sous element Horizontal_CS */

  cr = cai_WriteOpenTagXML ( des , "Horizontal_CS" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Horizontal_CS" );
    goto ERREUR;
    }

  /* sous element HORIZONTAL_CS_TYPE */
  
  cr = cai_WriteElementXML ( des , "HORIZONTAL_CS_TYPE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element HORIZONTAL_CS_TYPE" );
    goto ERREUR;
    }

  /* sous element HORIZONTAL_CS_CODE */
  
  cr = cai_WriteElementXML ( des , "HORIZONTAL_CS_CODE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element HORIZONTAL_CS_CODE" );
    goto ERREUR;
    }

  /* sous element HORIZONTAL_CS_NAME */
  
  cr = cai_WriteElementXML ( des , "HORIZONTAL_CS_NAME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element HORIZONTAL_CS_NAME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Horizontal_CS */

  cr = cai_WriteCloseTagXML ( des , "Horizontal_CS" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Horizontal_CS" );
    goto ERREUR;
    }

  /* Fermeture de l'element Coordinate_Reference_System */

  cr = cai_WriteCloseTagXML ( des , "Coordinate_Reference_System" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Coordinate_Reference_System" );
    goto ERREUR;
    }

  /* sous element Raster_CS */

  cr = cai_WriteOpenTagXML ( des , "Raster_CS" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Raster_CS" );
    goto ERREUR;
    }

  /* sous element RASTER_CS_TYPE */

  cr = cai_WriteElementXML ( des , "RASTER_CS_TYPE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element RASTER_CS_TYPE" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element PIXEL_ORIGIN               */
/****************************************************************************/
  
  /* sous element PIXEL_ORIGIN */

  cr = cai_WriteElementXML ( des , "PIXEL_ORIGIN" , "1" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PIXEL_ORIGIN" );
    goto ERREUR;
    }
  
  /* Fermeture de l'element Raster_CS */

  cr = cai_WriteCloseTagXML ( des , "Raster_CS" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Raster_CS" );
    goto ERREUR;
    }

  /* sous element Geoposition */
  
  cr = cai_WriteOpenTagXML ( des , "Geoposition" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Geoposition" );
    goto ERREUR;
    }

  /* sous element Geoposition_Points */
  
  cr = cai_WriteOpenTagXML ( des , "Geoposition_Points" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Geoposition_Points" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }


  /* sous element TIE_POINTS_DATA_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }
    
/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }


  /* sous element TIE_POINTS_DATA_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element Tie_Point */
  
  cr = cai_WriteOpenTagXML ( des , "Tie_Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_CRS_Y */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_CRS_Y" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element TIE_POINTS_CRS_Z           */
/****************************************************************************/

  /* sous element TIE_POINTS_CRS_Z */

  cr = cai_WriteElementXML ( des , "TIE_POINTS_CRS_Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element TIE_POINTS_CRS_Z" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_X */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_X" );
    goto ERREUR;
    }

  /* sous element TIE_POINTS_DATA_Y */
  
  cr = cai_WriteElementXML ( des , "TIE_POINTS_DATA_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIE_POINTS_DATA_Y" );
    goto ERREUR;
    }

 /* Fermeture de l'element Tie_Point */

  cr = cai_WriteCloseTagXML ( des , "Tie_Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Tie_Point" );
    goto ERREUR;
    }

 /* Fermeture de l'element Geoposition_Points */

  cr = cai_WriteCloseTagXML ( des , "Geoposition_Points" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Geoposition_Points" );
    goto ERREUR;
    }
    
/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Deplacement de Simplified_Location_Model       */
/*****************************************************************************/

  /* sous element Simplified_Location_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Simplified_Location_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Simplified_Location_Model" );
    goto ERREUR;
    }

  /* sous element Direct_Location_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Direct_Location_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Direct_Location_Model" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element lc_List                     */
/*****************************************************************************/

  /* sous element lc_List */

  cr = cai_WriteOpenTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

 /* Fermeture de l'element lc_List */

  cr = cai_WriteCloseTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element pc_List                     */
/*****************************************************************************/

  /* sous element pc_List */

  cr = cai_WriteOpenTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

 /* Fermeture de l'element pc_List */

  cr = cai_WriteCloseTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

 /* Fermeture de l'element Direct_Location_Model */

  cr = cai_WriteCloseTagXML ( des , "Direct_Location_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Direct_Location_Model" );
    goto ERREUR;
    }

  /* sous element Reverse_Location_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Reverse_Location_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Reverse_Location_Model" );
    goto ERREUR;
    }

  /* sous element lc_List */

  cr = cai_WriteOpenTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* sous element lc */
  
  cr = cai_WriteElementXML ( des , "lc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc" );
    goto ERREUR;
    }

  /* Fermeture de l'element lc_List */

  cr = cai_WriteCloseTagXML ( des , "lc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element lc_List" );
    goto ERREUR;
    }

  /* sous element pc_List */

  cr = cai_WriteOpenTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* sous element pc */
  
  cr = cai_WriteElementXML ( des , "pc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc" );
    goto ERREUR;
    }

  /* Fermeture de l'element pc_List */

  cr = cai_WriteCloseTagXML ( des , "pc_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element pc_List" );
    goto ERREUR;
    }

  /* Fermeture de l'element Reverse_Location_Model */

  cr = cai_WriteCloseTagXML ( des , "Reverse_Location_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Reverse_Location_Model" );
    goto ERREUR;
    }

  /* Fermeture de l'element Simplified_Location_Model */

  cr = cai_WriteCloseTagXML ( des , "Simplified_Location_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Simplified_Location_Model" );
    goto ERREUR;
    }

 /* Fermeture de l'element Geoposition */

  cr = cai_WriteCloseTagXML ( des , "Geoposition" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Geoposition" );
    goto ERREUR;
    }

  /* sous element Production */
  
  cr = cai_WriteOpenTagXML ( des , "Production" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Production" );
    goto ERREUR;
    }

  /* sous element DATASET_PRODUCER_NAME */
  
  cr = cai_WriteElementXML ( des , "DATASET_PRODUCER_NAME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_PRODUCER_NAME" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout attribut href="http://www.spotimage.fr" */
/****************************************************************************/
    
  /* sous element DATASET_PRODUCER_URL */
  type=1;
  cr = cai_WriteElementXMLWithAttribute ( des , "DATASET_PRODUCER_URL" , "" ,
                                   "href" , "http://www.spotimage.fr" ,type);
 
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_PRODUCER_URL" );
    goto ERREUR;
    }

  /* sous element DATASET_PRODUCTION_DATE */
  
  cr = cai_WriteElementXML ( des , "DATASET_PRODUCTION_DATE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATASET_PRODUCTION_DATE" );
    goto ERREUR;
    }

  /* sous element PRODUCT_TYPE */

  cr = cai_WriteElementXML ( des , "PRODUCT_TYPE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PRODUCT_TYPE" );
    goto ERREUR;
    }

  /* sous element PRODUCT_INFO */

  cr = cai_WriteElementXML ( des , "PRODUCT_INFO" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PRODUCT_INFO" );
    goto ERREUR;
    }

  /* sous element Production_Facility */
  
  cr = cai_WriteOpenTagXML ( des , "Production_Facility" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Production_Facility" );
    goto ERREUR;
    }

  /* sous element SOFTWARE_NAME */

  cr = cai_WriteElementXML ( des , "SOFTWARE_NAME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SOFTWARE_NAME" );
    goto ERREUR;
    }

  /* sous element SOFTWARE_VERSION */
  
  cr = cai_WriteElementXML ( des , "SOFTWARE_VERSION" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SOFTWARE_VERSION" );
    goto ERREUR;
    }

  /* sous element PROCESSING_CENTER */
  
  cr = cai_WriteElementXML ( des , "PROCESSING_CENTER" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PROCESSING_CENTER" );
    goto ERREUR;
    }

 /* Fermeture de l'element Production_Facility */

  cr = cai_WriteCloseTagXML ( des , "Production_Facility" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Production_Facility" );
    goto ERREUR;
    }

 /* Fermeture de l'element Production */

  cr = cai_WriteCloseTagXML ( des , "Production" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Production" );
    goto ERREUR;
    }

  /* sous element Raster_Dimensions */
  
  cr = cai_WriteOpenTagXML ( des , "Raster_Dimensions" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Raster_Dimension" );
    goto ERREUR;
    }

  /* sous element NCOLS */
  
  sprintf ( valeur , "%d" , nbcol );
  cr = cai_WriteElementXML ( des , "NCOLS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element NCOLS" );
    goto ERREUR;
    }

  /* sous element NROWS */
  
  sprintf ( valeur , "%d" , nblig );
  cr = cai_WriteElementXML ( des , "NROWS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element NROWS" );
    goto ERREUR;
    }

  /* sous element NBANDS */
  
  sprintf ( valeur , "%d" , nbcan );
  cr = cai_WriteElementXML ( des , "NBANDS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element NBANDS" );
    goto ERREUR;
    }

 /* Fermeture de l'element Raster_Dimensions */

  cr = cai_WriteCloseTagXML ( des , "Raster_Dimensions" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Raster_Dimensions" );
    goto ERREUR;
    }

  /* sous element Raster_Encoding */
  
  cr = cai_WriteOpenTagXML ( des , "Raster_Encoding" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Raster_Encoding" );
    goto ERREUR;
    }

  /* sous element NBITS */
  
  sprintf ( valeur , "%d" , nboct*8 );
  cr = cai_WriteElementXML ( des , "NBITS" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element NBITS" );
    goto ERREUR;
    }

/****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element DATA_TYPE                  */
/****************************************************************************/

  /* sous element DATA_TYPE */

  cr = cai_WriteElementXML ( des , "DATA_TYPE" , "UNSIGNED" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element DATA_TYPE" );
    goto ERREUR;
    }

  /* sous element BYTEORDER */
/*****************************************************************************/
/* MOD : VERSION : 4.7 : FA : erreur initialisation BYTEORDER en creation    */
/*****************************************************************************/

  /* Initialisation par defaut */
  if (strcmp(type_machine,"BE")==0)
     cr = cai_WriteElementXML ( des , "BYTEORDER" , "M" );
  else
     cr = cai_WriteElementXML ( des , "BYTEORDER" , "I" );

  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BYTEORDER" );
    goto ERREUR;
    }

  /* sous element BANDS_LAYOUT */
  
  if ( typeimg == 3 )
    sprintf ( valeur , "BIL" );
  else
    sprintf ( valeur , "" );  
  cr = cai_WriteElementXML ( des , "BANDS_LAYOUT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BANDS_LAYOUT" );
    goto ERREUR;
    }

 /* Fermeture de l'element Raster_Encoding */

  cr = cai_WriteCloseTagXML ( des , "Raster_Encoding" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Raster_Encoding" );
    goto ERREUR;
    }

  /* sous element Data_Processing */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Processing" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Processing" );
    goto ERREUR;
    }

  /* sous element PROCESSING_LEVEL */
  
  cr = cai_WriteElementXML ( des , "PROCESSING_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PROCESSING_LEVEL" );
    goto ERREUR;
    }

  /* sous element GEOMETRIC_PROCESSING */

  cr = cai_WriteElementXML ( des , "GEOMETRIC_PROCESSING" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element GEOMETRIC_PROCESSING" );
    goto ERREUR;
    }
  
  /* sous element Processing_Options */
  
  cr = cai_WriteOpenTagXML ( des , "Processing_Options" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Processing_Options" );
    goto ERREUR;
    }

  /* sous element Dynamic_Stretch */
  
  cr = cai_WriteOpenTagXML ( des , "Dynamic_Stretch" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dynamic_Stretch" );
    goto ERREUR;
    }

  for ( i = 0 ; i < nbcan ; i++ )
  {
  /* sous element Thresholds */
  
  cr = cai_WriteOpenTagXML ( des , "Thresholds" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Thresholds" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
  sprintf ( valeur , "%d" , (i+1) );
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND_INDEX" );
    goto ERREUR;
    }

  /* sous element LOW_THRESHOLD */
  
  cr = cai_WriteElementXML ( des , "LOW_THRESHOLD" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element LOW_THRESHOLD" );
    goto ERREUR;
    }

  /* sous element HIGH_THRESHOLD */
  
  cr = cai_WriteElementXML ( des , "HIGH_THRESHOLD" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element HIGH_THRESHOLD" );
    goto ERREUR;
    }

 /* Fermeture de l'element Thresholds */

  cr = cai_WriteCloseTagXML ( des , "Thresholds" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Thresholds" );
    goto ERREUR;
    }
  }
 /* Fermeture de l'element Dynamic_Stretch */

  cr = cai_WriteCloseTagXML ( des , "Dynamic_Stretch" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dynamic_Stretch" );
    goto ERREUR;
    }

 /* Fermeture de l'element Processing_Options */

  cr = cai_WriteCloseTagXML ( des , "Processing_Options" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Processing_Options" );
    goto ERREUR;
    }

 /* Fermeture de l'element Data_Processing */

  cr = cai_WriteCloseTagXML ( des , "Data_Processing" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Processing" );
    goto ERREUR;
    }

  /* sous element Data_Access */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Access" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Access" );
    goto ERREUR;
    }

  /* sous element DATA_FILE_ORGANISATION */
  
  if ( typeimg == 3 )
    sprintf ( valeur , "BAND_COMPOSITE" );
  else
    sprintf ( valeur , "" );  

  cr = cai_WriteElementXML ( des , "DATA_FILE_ORGANISATION" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATA_FILE_ORGANISATION" );
    goto ERREUR;
    }

  /* sous element DATA_FILE_FORMAT */
  
  if ( typeimg == 3 )
    sprintf ( valeur , "RAW" );
  else
    sprintf ( valeur , "TIFF" );
    
  cr = cai_WriteElementXML ( des , "DATA_FILE_FORMAT" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DATA_FILE_FORMAT" );
    goto ERREUR;
    }

 /* Fermeture de l'element Data_Access */

  cr = cai_WriteCloseTagXML ( des , "Data_Access" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Access" );
    goto ERREUR;
    }

  /* sous element Image_Display */
  
  cr = cai_WriteOpenTagXML ( des , "Image_Display" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Image_Display" );
    goto ERREUR;
    }

  /* sous element Band_Display_Order */
  
  cr = cai_WriteOpenTagXML ( des , "Band_Display_Order" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Display_Order" );
    goto ERREUR;
    }

    /* sous element RED_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "RED_CHANNEL" , "" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture Element RED_CHANNEL" );
      goto ERREUR;
      }    
    /* sous element GREEN_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "GREEN_CHANNEL" , "" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture Element GREEN_CHANNEL" );
      goto ERREUR;
      }    
    /* sous element BLUE_CHANNEL */
 
    cr = cai_WriteElementXML ( des , "BLUE_CHANNEL" , "" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture Element BLUE_CHANNEL" );
      goto ERREUR;
      }
  
 /* Fermeture de l'element Band_Display_Order */

  cr = cai_WriteCloseTagXML ( des , "Band_Display_Order" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Display_Order" );
    goto ERREUR;
    }

  /* sous element Special_Value */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value" );
    goto ERREUR;
    }

  /* sous element SPECIAL_VALUE_INDEX */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_INDEX" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SPECIAL_VALUE_INDEX" );
    goto ERREUR;
    }
   
  /* sous element SPECIAL_VALUE_TEXT */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_TEXT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SPECIAL_VALUE_TEXT" );
    goto ERREUR;
    }    
   
  /* sous element Special_Value_Color */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value_Color" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value_Color" );
    goto ERREUR;
    }
  
  /* sous element RED_LEVEL */
 
  cr = cai_WriteElementXML ( des , "RED_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element RED_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element GREEN_LEVEL */
 
  cr = cai_WriteElementXML ( des , "GREEN_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element GREEN_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element BLUE_LEVEL */
 
  cr = cai_WriteElementXML ( des , "BLUE_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BLUE_LEVEL" );
    goto ERREUR;
    }    

   if (nbcan == 4)
   {
  /* sous element ALPHA_LEVEL */
 
      cr = cai_WriteElementXML ( des , "ALPHA_LEVEL" , "" );
      if ( cr == CAI_KO )
      {
        sprintf ( CAI_ERREUR , "Erreur ecriture Element ALPHA_LEVEL" );
        goto ERREUR;
       }    
   } 

  /* Fermeture de l'element Special_Value_Color */

  cr = cai_WriteCloseTagXML ( des , "Special_Value_Color" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value_Color" );
    goto ERREUR;
    }

  /* Fermeture de l'element Special_Value */

  cr = cai_WriteCloseTagXML ( des , "Special_Value" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value" );
    goto ERREUR;
    }

  /* sous element Special_Value */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value" );
    goto ERREUR;
    }
  
  /* sous element SPECIAL_VALUE_INDEX */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_INDEX" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SPECIAL_VALUE_INDEX" );
    goto ERREUR;
    }    
  
  /* sous element SPECIAL_VALUE_TEXT */
 
  cr = cai_WriteElementXML ( des , "SPECIAL_VALUE_TEXT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SPECIAL_VALUE_TEXT" );
    goto ERREUR;
    }    
  
  /* sous element Special_Value_Color */
  
  cr = cai_WriteOpenTagXML ( des , "Special_Value_Color" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value_Color" );
    goto ERREUR;
    }
  
  /* sous element RED_LEVEL */
 
  cr = cai_WriteElementXML ( des , "RED_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element RED_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element GREEN_LEVEL */
 
  cr = cai_WriteElementXML ( des , "GREEN_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element GREEN_LEVEL" );
    goto ERREUR;
    }    
  
  /* sous element BLUE_LEVEL */
 
  cr = cai_WriteElementXML ( des , "BLUE_LEVEL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BLUE_LEVEL" );
    goto ERREUR;
    }    

   if (nbcan == 4)
   {
  /* sous element ALPHA_LEVEL */
 
      cr = cai_WriteElementXML ( des , "ALPHA_LEVEL" , "" );
      if ( cr == CAI_KO )
      {
        sprintf ( CAI_ERREUR , "Erreur ecriture Element ALPHA_LEVEL" );
        goto ERREUR;
       }    
   } 
  
  /* Fermeture de l'element Special_Value_Color */

  cr = cai_WriteCloseTagXML ( des , "Special_Value_Color" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value_Color" );
    goto ERREUR;
    }

  /* Fermeture de l'element Special_Value */

  cr = cai_WriteCloseTagXML ( des , "Special_Value" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Special_Value" );
    goto ERREUR;
    }

  /* Fermeture de l'element Image_Display */

  cr = cai_WriteCloseTagXML ( des , "Image_Display" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Image_Display" );
    goto ERREUR;
    }

  /* sous element Image_Interpretation */
  
  cr = cai_WriteOpenTagXML ( des , "Image_Interpretation" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Image_Interpretation" );
    goto ERREUR;
    }

  for ( i = 0; i < nbcan ; i++ )
    {
    /* sous element Spectral_Band_Info */
  
    cr = cai_WriteOpenTagXML ( des , "Spectral_Band_Info" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture Element Spectral_Band_Info" );
      goto ERREUR;
      }

  /* sous element BAND_INDEX */
  
  sprintf ( valeur , "%d" , (i+1) );
  
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND_INDEX" );
    goto ERREUR;
    }

/******************************************************************************/
/* MOD : VERSION : 4.7 : DM : remplacement de BAND par BAND_DESCRIPTION       */
/******************************************************************************/

    /* sous element BAND_DESCRIPTION */

    cr = cai_WriteElementXML ( des , "BAND_DESCRIPTION" , "" );
    if ( cr == CAI_KO )
      {
      sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND" );
      goto ERREUR;
      }
    
  /* sous element PHYSICAL_CALIBRATION_DATE */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_CALIBRATION_DATE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PHYSICAL_CALIBRATION_DATE" );
    goto ERREUR;
    }

  /* sous element PHYSICAL_BIAIS */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_BIAIS" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PHYSICAL_BIAIS" );
    goto ERREUR;
    }

  /* sous element PHYSICAL_GAIN */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_GAIN" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PHYSICAL_GAIN" );
    goto ERREUR;
    }

  /* sous element PHYSICAL_UNIT */
  
  cr = cai_WriteElementXML ( des , "PHYSICAL_UNIT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PHYSICAL_UNIT" );
    goto ERREUR;
    }

  /* Fermeture de l'element Spectral_Band_Info */

  cr = cai_WriteCloseTagXML ( des , "Spectral_Band_Info" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Spectral_Band_Info" );
    goto ERREUR;
    }

  }
  /* Fermeture de l'element Image_Interpretation */

  cr = cai_WriteCloseTagXML ( des , "Image_Interpretation" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Image_Interpretation" );
    goto ERREUR;
    }

  /* sous element Data_Strip */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Strip" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Strip" );
    goto ERREUR;
    }

  /* sous element Satellite_Time */
  
  cr = cai_WriteOpenTagXML ( des , "Satellite_Time" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Satellite_Time" );
    goto ERREUR;
    }

  /* sous element UT_DATE */
  
  cr = cai_WriteElementXML ( des , "UT_DATE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element UT_DATE" );
    goto ERREUR;
    }

  /* sous element CLOCK_VALUE */
  
  cr = cai_WriteElementXML ( des , "CLOCK_VALUE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element CLOCK_VALUE" );
    goto ERREUR;
    }

  /* sous element CLOCK_PERIOD */
  
  cr = cai_WriteElementXML ( des , "CLOCK_PERIOD" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element CLOCK_PERIOD" );
    goto ERREUR;
    }

  /* sous element BOARD_TIME */
  
  cr = cai_WriteElementXML ( des , "BOARD_TIME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BOARD_TIME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Satellite_Time */

  cr = cai_WriteCloseTagXML ( des , "Satellite_Time" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Satellite_Time" );
    goto ERREUR;
    }

  /* sous element Ephemeris */
  
  cr = cai_WriteOpenTagXML ( des , "Ephemeris" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Ephemeris" );
    goto ERREUR;
    }

  /* sous element SATELLITE_ALTITUDE */
  
  cr = cai_WriteElementXML ( des , "SATELLITE_ALTITUDE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SATELLITE_ALTITUDE" );
    goto ERREUR;
    }

  /* sous element NADIR_LAT */
  
  cr = cai_WriteElementXML ( des , "NADIR_LAT" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element NADIR_LAT" );
    goto ERREUR;
    }

  /* sous element NADIR_LON */
  
  cr = cai_WriteElementXML ( des , "NADIR_LON" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element NADIR_LON" );
    goto ERREUR;
    }

  /* sous element Points */
  
  cr = cai_WriteOpenTagXML ( des , "Points" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Points" );
    goto ERREUR;
    }

  for ( i = 0; i < 8 ; i++ )
  {
  /* sous element Point */
  
  cr = cai_WriteOpenTagXML ( des , "Point" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Point" );
    goto ERREUR;
    }

  /* sous element Location */
  
  cr = cai_WriteOpenTagXML ( des , "Location" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Location" );
    goto ERREUR;
    }

  /* sous element X */
  
  cr = cai_WriteElementXML ( des , "X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element X" );
    goto ERREUR;
    }

  /* sous element Y */
  
  cr = cai_WriteElementXML ( des , "Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Y" );
    goto ERREUR;
    }

  /* sous element Z */
  
  cr = cai_WriteElementXML ( des , "Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Z" );
    goto ERREUR;
    }

 /* Fermeture de l'element Location */

  cr = cai_WriteCloseTagXML ( des , "Location" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Location" );
    goto ERREUR;
    }

  /* sous element Velocity */
  
  cr = cai_WriteOpenTagXML ( des , "Velocity" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Velocity" );
    goto ERREUR;
    }

  /* sous element X */
  
  cr = cai_WriteElementXML ( des , "X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element X" );
    goto ERREUR;
    }

  /* sous element Y */
  
  cr = cai_WriteElementXML ( des , "Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Y" );
    goto ERREUR;
    }

  /* sous element Z */
  
  cr = cai_WriteElementXML ( des , "Z" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Z" );
    goto ERREUR;
    }

 /* Fermeture de l'element Velocity */

  cr = cai_WriteCloseTagXML ( des , "Velocity" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Velocity" );
    goto ERREUR;
    }

  /* sous element TIME */
  
  cr = cai_WriteElementXML ( des , "TIME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element TIME" );
    goto ERREUR;
    }

  /* Fermeture de l'element Point */

  cr = cai_WriteCloseTagXML ( des , "Point" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Point" );
    goto ERREUR;
    }
  }
  
  /* Fermeture de l'element Points */

  cr = cai_WriteCloseTagXML ( des , "Points" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Points" );
    goto ERREUR;
    }

  /* sous element DORIS_USED */
  
  cr = cai_WriteElementXML ( des , "DORIS_USED" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DORIS_USED" );
    goto ERREUR;
    }

  /* Fermeture de l'element Ephemeris */

  cr = cai_WriteCloseTagXML ( des , "Ephemeris" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Ephemeris" );
    goto ERREUR;
    }

  /* sous element Sensor_Configuration */
  
  cr = cai_WriteOpenTagXML ( des , "Sensor_Configuration" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Sensor_Configuration" );
    goto ERREUR;
    }

  /* sous element Time_Stamp */
  
  cr = cai_WriteOpenTagXML ( des , "Time_Stamp" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Time_Stamp" );
    goto ERREUR;
    }

  /* sous element LINE_PERIOD */
  
  cr = cai_WriteElementXML ( des , "LINE_PERIOD" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element LINE_PERIOD" );
    goto ERREUR;
    }

  /* sous element SCENE_CENTER_TIME */
  
  cr = cai_WriteElementXML ( des , "SCENE_CENTER_TIME" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SCENE_CENTER_TIME" );
    goto ERREUR;
    }

  /* sous element SCENE_CENTER_LINE */
  
  cr = cai_WriteElementXML ( des , "SCENE_CENTER_LINE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SCENE_CENTER_LINE" );
    goto ERREUR;
    }

  /* sous element SCENE_CENTER_COL */
  
  cr = cai_WriteElementXML ( des , "SCENE_CENTER_COL" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SCENE_CENTER_COL" );
    goto ERREUR;
    }

  /* Fermeture de l'element Time_Stamp */

  cr = cai_WriteCloseTagXML ( des , "Time_Stamp" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Time_Stamp" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Instrument_look_Angles_List */
/*****************************************************************************/

  /* sous element Instrument_Look_Angles_List */

  cr = cai_WriteOpenTagXML ( des , "Instrument_Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Instrument_Look_Angles_List" );
    goto ERREUR;
    }

  for ( i = 0 ; i < nbcan ; i++ )
    {
  /* sous element Instrument_Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Instrument_Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Instrument_Look_Angles" );
    goto ERREUR;
    }

  /* sous element VALIDITY_DATE */
  
  cr = cai_WriteElementXML ( des , "VALIDITY_DATE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element VALIDITY_DATE" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
  sprintf ( valeur , "%d" , (i+1) );
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND_INDEX" );
    goto ERREUR;
    }

/*****************************************************************************/
/* MOD : VERSION : 4.7 : DM : Ajout de l'element Look_Angles_List            */
/*****************************************************************************/

  /* sous element Look_Angles_List */

  cr = cai_WriteOpenTagXML ( des , "Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture element Look_Angles_List" );
    goto ERREUR;
    }

  /* sous element Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Look_Angles" );
    goto ERREUR;
    }

  /* sous element DETECTOR_ID */

  cr = cai_WriteElementXML ( des , "DETECTOR_ID" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DETECTOR_ID" );
    goto ERREUR;
    }

  /* sous element PSI_X */
  
  cr = cai_WriteElementXML ( des , "PSI_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PSI_X" );
    goto ERREUR;
    }

  /* sous element PSI_Y */
  
  cr = cai_WriteElementXML ( des , "PSI_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PSI_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Look_Angles" );
    goto ERREUR;
    }

  /* sous element Look_Angles */
  
  cr = cai_WriteOpenTagXML ( des , "Look_Angles" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Look_Angles" );
    goto ERREUR;
    }

  /* sous element DETECTOR_ID */

  cr = cai_WriteElementXML ( des , "DETECTOR_ID" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element DETECTOR_ID" );
    goto ERREUR;
    }

  /* sous element PSI_X */
  
  cr = cai_WriteElementXML ( des , "PSI_X" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PSI_X" );
    goto ERREUR;
    }

  /* sous element PSI_Y */
  
  cr = cai_WriteElementXML ( des , "PSI_Y" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element PSI_Y" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Look_Angles" );
    goto ERREUR;
    }

  /* Fermeture de l'element Look_Angles_List */

  cr = cai_WriteCloseTagXML ( des , "Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Look_Angles_List" );
    goto ERREUR;
    }
   
  /* Fermeture de l'element Instrument_Look_Angles */

  cr = cai_WriteCloseTagXML ( des , "Instrument_Look_Angles" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Instrument_Look_Angles" );
    goto ERREUR;
    }
  }

  /* Fermeture de l'element Instrument_Look_Angles_List */

  cr = cai_WriteCloseTagXML ( des , "Instrument_Look_Angles_List" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Instrument_Look_Angles_List" );
    goto ERREUR;
    }

  /* Fermeture de l'element Sensor_Configuration */

  cr = cai_WriteCloseTagXML ( des , "Sensor_Configuration" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Sensor_Configuration" );
    goto ERREUR;
    }

  /* sous element Frame_Counters */
  
  cr = cai_WriteOpenTagXML ( des , "Frame_Counters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Frame_Counters" );
    goto ERREUR;
    }

  /* sous element SEGMENT_START */
  
  cr = cai_WriteElementXML ( des , "SEGMENT_START" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SEGMENT_START" );
    goto ERREUR;
    }

   /* sous element SCENE_START */
  
  cr = cai_WriteElementXML ( des , "SCENE_START" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SCENE_START" );
    goto ERREUR;
    }

  /* sous element SEGMENT_END */
  
  cr = cai_WriteElementXML ( des , "SEGMENT_END" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SEGMENT_END" );
    goto ERREUR;
    }

 /* Fermeture de l'element Frame_Counters */

  cr = cai_WriteCloseTagXML ( des , "Frame_Counters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Frame_Counters" );
    goto ERREUR;
    }

  /* sous element Data_Strip_Coordinates */
  
  cr = cai_WriteOpenTagXML ( des , "Data_Strip_Coordinates" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Strip_Coordinates" );
    goto ERREUR;
    }

  /* sous element FIRST_PIXEL_RAW */
  
  cr = cai_WriteElementXML ( des , "FIRST_PIXEL_RAW" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element FIRST_PIXEL_RAW" );
    goto ERREUR;
    }

   /* sous element FIRST_LINE_RAW */
  
  cr = cai_WriteElementXML ( des , "FIRST_LINE_RAW" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element FIRST_LINE_RAW" );
    goto ERREUR;
    }

  /* sous element FIRST_PIXEL_1B */
  
  cr = cai_WriteElementXML ( des , "FIRST_PIXEL_1B" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element FIRST_PIXEL_1B" );
    goto ERREUR;
    }

  /* sous element FIRST_LINE_1B */
  
  cr = cai_WriteElementXML ( des , "FIRST_LINE_1B" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element FIRST_LINE_1B" );
    goto ERREUR;
    }

 /* Fermeture de l'element Data_Strip_Coordinates */

  cr = cai_WriteCloseTagXML ( des , "Data_Strip_Coordinates" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Strip_Coordinates" );
    goto ERREUR;
    }

  /* sous element Models */
  
  cr = cai_WriteOpenTagXML ( des , "Models" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Models" );
    goto ERREUR;
    }

  /* sous element OneB_Model */
  
  cr = cai_WriteOpenTagXML ( des , "OneB_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element OneB_Model" );
    goto ERREUR;
    }

  /* sous element Coordinate_Normalization */
  
  cr = cai_WriteOpenTagXML ( des , "Coordinate_Normalization" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Coordinate_Normalization" );
    goto ERREUR;
    }

  /* sous element LINES_I_M */
  
  cr = cai_WriteElementXML ( des , "LINES_I_M" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element LINES_I_M" );
    goto ERREUR;
    }

  /* sous element LINES_DELTA_I */
  
  cr = cai_WriteElementXML ( des , "LINES_DELTA_I" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element LINES_DELTA_I" );
    goto ERREUR;
    }

  /* sous element COLUMNS_J_M */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_J_M" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element COLUMNS_J_M" );
    goto ERREUR;
    }

  /* sous element COLUMNS_DELTA_J */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_DELTA_J" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element COLUMNS_DELTA_J" );
    goto ERREUR;
    }

  /* sous element LINES_L_M */
  
  cr = cai_WriteElementXML ( des , "LINES_L_M" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element LINES_L_M" );
    goto ERREUR;
    }

  /* sous element LINES_DELTA_L */
  
  cr = cai_WriteElementXML ( des , "LINES_DELTA_L" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element LINES_DELTA_L" );
    goto ERREUR;
    }

  /* sous element COLUMNS_P_M */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_P_M" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element COLUMNS_P_M" );
    goto ERREUR;
    }

  /* sous element COLUMNS_DELTA_P */
  
  cr = cai_WriteElementXML ( des , "COLUMNS_DELTA_P" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element COLUMNS_DELTA_P" );
    goto ERREUR;
    }

  /* Fermeture de l'element Coordinate_Normalization */

  cr = cai_WriteCloseTagXML ( des , "Coordinate_Normalization" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Coordinate_Normalization" );
    goto ERREUR;
    }


  /* sous element Direct_OneB_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Direct_OneB_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Direct_OneB_Model" );
    goto ERREUR;
    }

  /* sous element I */
  
  cr = cai_WriteOpenTagXML ( des , "I" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element I" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element I */

  cr = cai_WriteCloseTagXML ( des , "I" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element I" );
    goto ERREUR;
    }

  /* sous element J */
  
  cr = cai_WriteOpenTagXML ( des , "J" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element J" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element J */

  cr = cai_WriteCloseTagXML ( des , "J" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element J" );
    goto ERREUR;
    }

  /* Fermeture de l'element Direct_OneB_Model */

  cr = cai_WriteCloseTagXML ( des , "Direct_OneB_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Direct_OneB_Model" );
    goto ERREUR;
    }

  /* sous element Reverse_OneB_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Reverse_OneB_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Reverse_OneB_Model" );
    goto ERREUR;
    }

  /* sous element L */
  
  cr = cai_WriteOpenTagXML ( des , "L" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element L" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element L */

  cr = cai_WriteCloseTagXML ( des , "L" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element L" );
    goto ERREUR;
    }

  /* sous element P */
  
  cr = cai_WriteOpenTagXML ( des , "P" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element P" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element P */

  cr = cai_WriteCloseTagXML ( des , "P" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element P" );
    goto ERREUR;
    }

  /* Fermeture de l'element Reverse_OneB_Model */

  cr = cai_WriteCloseTagXML ( des , "Reverse_OneB_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Reverse_OneB_Model" );
    goto ERREUR;
    }

  /* sous element A */
  
  cr = cai_WriteOpenTagXML ( des , "A" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element A" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element A */

  cr = cai_WriteCloseTagXML ( des , "A" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element A" );
    goto ERREUR;
    }

  /* sous element B */
  
  cr = cai_WriteOpenTagXML ( des , "B" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element B" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* sous element c */
  
  cr = cai_WriteElementXML ( des , "c" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element c" );
    goto ERREUR;
    }

  /* Fermeture de l'element B */

  cr = cai_WriteCloseTagXML ( des , "B" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element B" );
    goto ERREUR;
    }

  /* Fermeture de l'element OneB_Model */

  cr = cai_WriteCloseTagXML ( des , "OneB_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element OneB_Model" );
    goto ERREUR;
    }

  /* sous element Attitude_Model */
  
  cr = cai_WriteOpenTagXML ( des , "Attitude_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Attitude_Model" );
    goto ERREUR;
    }

  /* sous element D_L */
  
  cr = cai_WriteOpenTagXML ( des , "D_L" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_L" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_L */

  cr = cai_WriteCloseTagXML ( des , "D_L" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_L" );
    goto ERREUR;
    }

  /* sous element D_P */
  
  cr = cai_WriteOpenTagXML ( des , "D_P" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_P" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_P */

  cr = cai_WriteCloseTagXML ( des , "D_P" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_P" );
    goto ERREUR;
    }

  /* Fermeture de l'element Attitude_Model */

  cr = cai_WriteCloseTagXML ( des , "Attitude_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Attitude_Model" );
    goto ERREUR;
    }

  /* sous element SWIR_Registration_Model */
  
  cr = cai_WriteOpenTagXML ( des , "SWIR_Registration_Model" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SWIR_Registration_Model" );
    goto ERREUR;
    }

  /* sous element D_L */
  
  cr = cai_WriteOpenTagXML ( des , "D_L" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_L" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_L */

  cr = cai_WriteCloseTagXML ( des , "D_L" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_L" );
    goto ERREUR;
    }

  /* sous element D_P */
  
  cr = cai_WriteOpenTagXML ( des , "D_P" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_P" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* sous element abc */
  
  cr = cai_WriteElementXML ( des , "abc" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element abc" );
    goto ERREUR;
    }

  /* Fermeture de l'element D_P */

  cr = cai_WriteCloseTagXML ( des , "D_P" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element D_P" );
    goto ERREUR;
    }

  /* Fermeture de l'element SWIR_Registration_Model */

  cr = cai_WriteCloseTagXML ( des , "SWIR_Registration_Model" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SWIR_Registration_Model" );
    goto ERREUR;
    }

  /* Fermeture de l'element Models */

  cr = cai_WriteCloseTagXML ( des , "Models" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Models" );
    goto ERREUR;
    }
  /* sous element Sensor_Calibration */
  
  cr = cai_WriteOpenTagXML ( des , "Sensor_Calibration" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Sensor_Calibration" );
    goto ERREUR;
    }

  /* sous element METHOD */
  
  cr = cai_WriteElementXML ( des , "METHOD" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element METHOD" );
    goto ERREUR;
    }

  /* sous element Calibration */
  
  cr = cai_WriteOpenTagXML ( des , "Calibration" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Calibration" );
    goto ERREUR;
    }

  for ( i = 0; i < nbcan ; i++ )
  {
  /* sous element Band_Parameters */
  
  cr = cai_WriteOpenTagXML ( des , "Band_Parameters" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Parameters" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  sprintf ( valeur , "%d" , (i+1) );
  
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND_INDEX" );
    goto ERREUR;
    }

  /* Fermeture de l'element Band_Parameters */

  cr = cai_WriteCloseTagXML ( des , "Band_Parameters" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Parameters" );
    goto ERREUR;
    }
  }
   /* Fermeture de l'element Calibration */

  cr = cai_WriteCloseTagXML ( des , "Calibration" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Calibration" );
    goto ERREUR;
    }

  /* sous element Spectral_Sensitivities */
  
  cr = cai_WriteOpenTagXML ( des , "Spectral_Sensitivities" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Spectral_Sensitivities" );
    goto ERREUR;
    }

  for ( i = 0; i < nbcan ; i++ )
  {
    /* sous element Band_Spectral_Sensitivities */
   
    cr = cai_WriteOpenTagXML ( des , "Band_Spectral_Sensitivities" );
    if ( cr == CAI_KO )
    {
      sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Spectral_Sensitivities" );
      goto ERREUR;
    }

  sprintf( valeur , "%d" , ( i+1 ) );
  /* sous element BAND_INDEX */
  
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND_INDEX" );
    goto ERREUR;
    }

  /* sous element FIRST_WAVELENGTH_VALUE */
  
  cr = cai_WriteElementXML ( des , "FIRST_WAVELENGTH_VALUE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element FIRST_WAVELENGTH_VALUE" );
    goto ERREUR;
    }

  /* sous element WAVELENGTH_STEP */
  
  cr = cai_WriteElementXML ( des , "WAVELENGTH_STEP" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element WAVELENGTH_STEP" );
    goto ERREUR;
    }

  /* sous element CALIBRATION_DATE */
  
  cr = cai_WriteElementXML ( des , "CALIBRATION_DATE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element CALIBRATION_DATE" );
    goto ERREUR;
    }
  
  /* sous element Spectral_Sensitivity_Values */
  
  cr = cai_WriteOpenTagXML ( des , "Spectral_Sensitivity_Values" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Spectral_Sensitivity_Values" );
    goto ERREUR;
    }

  /* sous element SPECTRAL_SENSITIVITY_VALUE */
  
  cr = cai_WriteElementXML ( des , "SPECTRAL_SENSITIVITY_VALUE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SPECTRAL_SENSITIVITY_VALUE" );
    goto ERREUR;
    }

  /* Fermeture de l'element Spectral_Sensitivity_Values */

  cr = cai_WriteCloseTagXML ( des , "Spectral_Sensitivity_Values" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Spectral_Sensitivity_Values" );
    goto ERREUR;
    }

  /* Fermeture de l'element Band_Spectral_Sensitivities */

  cr = cai_WriteCloseTagXML ( des , "Band_Spectral_Sensitivities" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Spectral_Sensitivities" );
    goto ERREUR;
    }

  }  

  /* Fermeture de l'element Spectral_Sensitivities */

  cr = cai_WriteCloseTagXML ( des , "Spectral_Sensitivities" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Spectral_Sensitivities" );
    goto ERREUR;
    }

  /* sous element Solar_Irradiance */
  
  cr = cai_WriteOpenTagXML ( des , "Solar_Irradiance" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Solar_Irradiance" );
    goto ERREUR;
    }

  for ( i = 0; i < nbcan ; i++ )
  {
  sprintf ( valeur , "%d" , ( i+1 ) );
  /* sous element Band_Solar_Irradiance */
  
  cr = cai_WriteOpenTagXML ( des , "Band_Solar_Irradiance" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Solar_Irradiance" );
    goto ERREUR;
    }

  /* sous element BAND_INDEX */
  
  cr = cai_WriteElementXML ( des , "BAND_INDEX" , valeur );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element BAND_INDEX" );
    goto ERREUR;
    }

  /* sous element SOLAR_IRRADIANCE_VALUE */
  
  cr = cai_WriteElementXML ( des , "SOLAR_IRRADIANCE_VALUE" , "" );
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element SOLAR_IRRADIANCE_VALUE" );
    goto ERREUR;
    }

  /* Fermeture de l'element Band_Solar_Irradiance */

  cr = cai_WriteCloseTagXML ( des , "Band_Solar_Irradiance" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Band_Solar_Irradiance" );
    goto ERREUR;
    }
  }
  /* Fermeture de l'element Solar_Irradiance */

  cr = cai_WriteCloseTagXML ( des , "Solar_Irradiance" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Solar_Irradiance" );
    goto ERREUR;
    }

  /* Fermeture de l'element Sensor_Calibration */

  cr = cai_WriteCloseTagXML ( des , "Sensor_Calibration" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Sensor_Calibration" );
    goto ERREUR;
    }

  /* Fermeture de l'element Data_Strip */

  cr = cai_WriteCloseTagXML ( des , "Data_Strip" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Data_Strip" );
    goto ERREUR;
    }

 /* Fermeture de l'element Dimap_Document */

  cr = cai_WriteCloseTagXML ( des , "Dimap_Document" );  
  if ( cr == CAI_KO )
    {
    sprintf ( CAI_ERREUR , "Erreur ecriture Element Dimap_document" );
    goto ERREUR;
    }

  /* fermeture du fichier descripteur */
  
  if (des!=NULL) fclose ( des );

  return CAI_OK;
  
  ERREUR:

  return CAI_KO;
  }  
/* 
  Fin de l'operation cai_creat_leader_spot5
*/
