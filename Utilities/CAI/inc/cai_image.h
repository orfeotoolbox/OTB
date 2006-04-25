
/* fichier include de toutes les informations necessaires au bon 
    fonctionnement de la bibliotheque Couche Acces Image */

/* MOD:VERSION:2:DM optimisation */
/* VERSION : $Header: cai_image.h,v 1.12 01/02/15 14:24:43 cmf Exp $ */
/* Octobre 1996 : prise en compte du format HDF pour QIV */
/* Septembre 1999 : prise en compte du format GESSIMU    */
/* Septembre 1999 : prise en compte du format GDB et GDR */
/* Regine CHAISEMARTIN (CISI)                            */
/*****************************************************************************/
/* MOD : VERSION : 4.2 : DM :           prototypage des fonctions            */
/*****************************************************************************/                       /* MOD : VERSION : 4.3 : DM : integration des formats SPOT1a4 et SPOT5       */
/*****************************************************************************/
/* MOD : VERSION : 4.4 : DM : suppression du format multiscope, modif SPOT5  */
/*****************************************************************************/
/* MOD : VERSION : 4.6 : DM : integration du format RADAR_CNES               */
/*****************************************************************************/
/* MOD : VERSION : 5.2 : FA : PB de reconnaissance automatique RADAR         */
/*****************************************************************************/
/*****************************************************************************/
/* MOD : VERSION : 5.2 : DM :  prise en compte de l'information INTEL ou IEEE*/
/*                             et des types non signes                       */
/*****************************************************************************/
/* MOD : VERSION : 5.3 : DM :  portage de la couche CAI dans plateforme      */
/*                             Microsof Visual C++ V6.0                      */
/*****************************************************************************/
/* MOD : VERSION : 5.3 : DM :  prise en compte du format bandeau             */
/*****************************************************************************/


#ifndef CAI_IMAGE_H
#define CAI_IMAGE_H


#ifndef MAIN
#define WHERE	extern 
#else
#define WHERE 
#endif


/* Code à ajouter si visual c++ */
#ifdef _MSC_VER
  #define _HDF16
  #ifdef CAI_DLL_EXPORTS
    #define EXPORT __declspec(dllexport)
    #define WHERE 
  #else 
   #define EXPORT __declspec(dllimport)
  #endif
  #define LIGNES_PAVE NB_LIG
  #define COLONNES_PAVE NB_COL
  #define F_OK 0
  #define PATH_MAX 4096
  #define strcasecmp(a,b) _stricmp(a,b) 
  #define ftruncate(a,b) _chsize(a,b)
  #define strncasecmp(a,b,c) _strnicmp(a,b,c)
  #define snprintf _snprintf
#else
  #define EXPORT
#endif



#ifndef IN_CAI_HDF
#define WHERE_HDF	extern EXPORT
#else
#define WHERE_HDF EXPORT
#endif

#ifndef IN_CAI_BSQ
#define WHERE_BSQ	extern EXPORT
#else
#define WHERE_BSQ EXPORT
#endif

#ifndef IN_CAI_BIP 
#define WHERE_BIP	extern EXPORT
#else
#define WHERE_BIP EXPORT
#endif

#ifndef IN_CAI_INRIA
#define WHERE_INRIA	extern EXPORT
#else
#define WHERE_INRIA EXPORT
#endif

#ifndef IN_CAI_ONERA
#define WHERE_ONERA	extern EXPORT
#else
#define WHERE_ONERA EXPORT
#endif

#ifndef IN_CAI_TIFF
#define WHERE_TIFF	extern EXPORT
#else
#define WHERE_TIFF EXPORT
#endif

#ifndef IN_CAI_BIL
#define WHERE_BIL	extern EXPORT
#else
#define WHERE_BIL EXPORT
#endif

#ifndef IN_CAI_LUM
#define WHERE_LUM	extern EXPORT
#else
#define WHERE_LUM EXPORT
#endif

#ifndef IN_CAI_GESSIMU
#define WHERE_GESSIMU	extern EXPORT
#else
#define WHERE_GESSIMU EXPORT
#endif

#ifndef IN_CAI_RASTER
#define WHERE_RASTER	extern EXPORT
#else
#define WHERE_RASTER EXPORT
#endif

#ifndef IN_CAI_JPEG
#define WHERE_JPEG	extern EXPORT
#else
#define WHERE_JPEG EXPORT
#endif

#ifndef IN_CAI_GRIL
#define WHERE_GRIL	extern EXPORT
#else
#define WHERE_GRIL EXPORT
#endif

#ifndef IN_CAI_DA
#define WHERE_DA	extern EXPORT
#else
#define WHERE_DA EXPORT
#endif

#ifndef IN_CAI_FONC
#define WHERE_FONC	extern EXPORT
#else
#define WHERE_FONC EXPORT
#endif

#ifndef IN_CAI_SPOT
#define WHERE_SPOT	extern EXPORT
#else
#define WHERE_SPOT EXPORT
#endif

#ifndef IN_CAI_RADAR
#define WHERE_RADAR	extern EXPORT
#else
#define WHERE_RADAR EXPORT
#endif

#ifndef IN_CAI_BANDEAU
#define WHERE_BANDEAU	extern EXPORT
#else
#define WHERE_BANDEAU EXPORT
#endif

#include "tiffio.h"
#include "jpeglib.h"

typedef char CAI_EXTENS[5];
typedef char CH3[3];
typedef char CH50[50];

#define CAI_NB_MAX_IMA 200
#define CAI_NB_MAX_CAN 250
#define CAI_SUFFIXE_DA		".aux"
#define CAI_NB_ALLOC		100
#define CAI_TAILLE_MAX_CLEDA	70
#define CAI_TAILLE_MAX_LIGDA	8192
#define	CAI_NB_MAX_CLE		100
#define CAI_TAILLE_MAX_ASCII	600
#define CAI_JPEG_LECTURE        0
#define CAI_JPEG_ECRITURE       1
#define CAI_JPEG_MODIFIER       2

/* cette taille doit etre paire et superieur a 2 fois la taille du plus
   grand mot cle du fichier */	
#define CAI_TAILLE_BUFFER	8192 
/* */



/* definition de types */

typedef enum{CAI_LECTURE,CAI_ECRITURE} CAI_MODE_OUVERTURE;
typedef char 	  CAI_MOT_CLE[CAI_TAILLE_MAX_CLEDA];
typedef char 	  CAI_CHAINE[1024];
typedef long int  CAI_TYPE;
typedef long int  CAI_OK_KO;


typedef struct {
    CAI_CHAINE nom_fichier;    /* nom du fichier ouvert */		
    int  descr;	       /* file descriptor du fichier de DA */ 
    char *buffer;	       /* buffer de donnee ASCII */
    long  taille_buffer;   /* taille du buffer ASCII demande */  
    long  taille_buffer_lu; /* taille du buffer ASCII lu */
    long  num_buffer;       /*( -1 si non lu ou si ajout ou modif DA) */ 
    CAI_MOT_CLE *liste_mot_cle;/* liste des mot cle ASCII presents ds fichier*/
    long int *ad_mot_cle;       /* adresse des mot cles ASCII dans le fichier */
    long int *ad_donnee;        /* adr debut de donnee apres chaque mot cle */   
    int  nb_mot_cle;       /* nombre de mot cles presents ( -1 si non alloue ou si ajout ou modif DA) */
    CAI_MOT_CLE dernier_mot_cle;/* dernier mot cle lu dans le fichier ( oper cai_caractDA ) */
    long int ad_dern_mot_cle;  /* adresse du dernier  mot cle ASCII lu dans le fichier 
				(-1 si non alloue ou ajout ou modif )*/ 
     long int ad_dern_donnee;    /* adresse du debut de la donne apres le dernier mot cle lu */ 
        char REMPLI[128];
}CAI_FILE_DA;


typedef struct {
	char TYPE_IMAGE[20];
        char NOM_IMAGE[1024];
        char COD_PIX[5];
	int  INV_OCTETS;
        int  NB_CANAUX;
        int  NB_LIG;
        int  NB_COL;
        char COMMENT[80];
        int  NB_BITS_PIX;
        int  NUM_LIG_DEB;
        int  NUM_COL_DEB;
	int  PAS_GRIL_LIG;
        int  PAS_GRIL_COL;
	int  DECAL_LIG;
	int  DECAL_COL;
        char CAR_GRIL[20];
        char TYPE_COD[5];
        char NIVEAU[3];	   /* Niveau de traitement de l'image (SPOT)       */ 
	int  TYPE_SPOT;    /* =1 SPOT 1a4 ; =2 SPOT5 TIFF ; =3  SPOT5  BIL */
        int  ACCES_SPOT[2];/* 0 pour le leader et 1 pour le volume         */
	char SENS_COD[3];
        int   NBOCT_PIX;
        TIFF *ACCES_CANAL_TIFF[CAI_NB_MAX_CAN];
        struct jpeg_decompress_struct *ACCES_CANAL_JPEG_LEC;
        struct jpeg_compress_struct *ACCES_CANAL_JPEG_ECR;
        JSAMPLE *image_JPEG;	
        FILE *ACCES_FILE_JPEG;
        int  ACCES_CANAL_INT[CAI_NB_MAX_CAN];
        int  POS_OCTET[CAI_NB_MAX_CAN];
	char NOM_FIC_CAN[100*CAI_NB_MAX_CAN];
	char NOM_UTIL_CAN[100*CAI_NB_MAX_CAN];

        int  COLORMAP;
        int  *TABCOLOR;
        int  HISTO[CAI_NB_MAX_CAN][256];
        int  NB_PIX_ECRITS[CAI_NB_MAX_CAN];
        CAI_MODE_OUVERTURE MODE;
}CAI_IMAGE;


#ifdef _CAI_IMAGE

#ifdef _HDF16

#define CAI_NB_FORMAT 18
#define CAI_NB_EXTENSIONS 18

static long int CAI_NUMEROS[CAI_NB_EXTENSIONS]=
                         { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
static char *CAI_SUFFIXES[CAI_NB_EXTENSIONS]=
 { ".xs",".inf",".hd",".bil",".tif",".srf",".bip",".hdf",".lum",
  ".jpg",".pfe",".gdb",".gdr"," "," "," ",".rad",".band"};
                                          
static char *CAI_NOM_FORMAT[CAI_NB_FORMAT]=
{"INRIA","ONERA","BSQ","BIL","TIFF","RASTERFILE","BIP","HDF","LUM","JPEG","GESSIMU","GDB","GDR","SPOT1A4","SPOT5TIF","SPOT5BIL","RADAR","BANDEAU"};



/* include necessaire pour le bon fonctionnement des fonctions d'acces des
	donnees auxiliaires */

static char * CAI_SUF_FORMAT[2][CAI_NB_FORMAT]=
      {{ ".xs",".inf",".hd",".bil",".tif",".srf",".bip",".hdf",".lum",
	".jpg",".pfe",".gdb",".gdr"," "," "," ",".rad",".band"},
       { ".xs",".inf",".hd",".bil",".tif",".srf",".bip",".hdf",".lum",
	 ".jpg",".pfe",".gdb",".gdr"," "," "," ",".rad",".band"}};

WHERE_HDF  long int cai_liste_ima_hdf16(char *repert,char ***tab_ima);
WHERE_HDF CAI_OK_KO cai_dest_ima_hdf16(char *repertoire,char *nom_ima);
WHERE_HDF CAI_IMAGE *cai_ouvre_lecture_hdf16(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_HDF CAI_IMAGE *cai_ouvre_modifie_hdf16(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_HDF CAI_OK_KO cai_ferme_hdf16(CAI_IMAGE *image1);
WHERE_HDF CAI_IMAGE *cai_ouvre_creation_hdf16(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_HDF CAI_OK_KO cai_ecriture_canal_hdf16(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_HDF CAI_OK_KO cai_lecture_canal_hdf16(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
WHERE_HDF CAI_OK_KO cai_rename_fichiers_hdf16(CAI_IMAGE *image1,
					      CAI_EXTENS *TabExtens);

#endif

#ifdef _NOHDF16
#define CAI_NB_FORMAT 17
#define CAI_NB_EXTENSIONS 17

static long int CAI_NUMEROS[CAI_NB_EXTENSIONS]=
                         { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
static char *CAI_SUFFIXES[CAI_NB_EXTENSIONS]=
 { ".xs",".inf",".hd",".bil",".tif",".srf",".bip",".lum",".jpg",".pfe",".gdb",".gdr"," "," ",",",".rad",".band"};
                                          
static char *CAI_NOM_FORMAT[CAI_NB_FORMAT]=
{"INRIA","ONERA","BSQ","BIL","TIFF","RASTERFILE","BIP","LUM","JPEG","GESSIMU","GDB","GDR","SPOT1A4","SPOT5TIF","SPOT5BIL","RADAR","BANDEAU"};


/* include necessaire pour le bon fonctionnement des fonctions d'acces des
	donnees auxiliaires */

static char * CAI_SUF_FORMAT[2][CAI_NB_FORMAT]=
      {{ ".xs",".inf",".hd",".bil",".tif",".srf",".bip",".lum",
	".jpg",".pfe",".gdb",".gdr"," "," "," ",".rad",".band"},
       { ".xs",".inf",".hd",".bil",".tif",".srf",".bip",".lum",
	 ".jpg",".pfe",".gdb",".gdr"," "," "," ",".rad",".band"}};

#endif

WHERE_BSQ  long int cai_liste_ima_bsq(char *repert,char ***tab_ima);
WHERE_BSQ CAI_OK_KO cai_dest_ima_bsq(char *repertoire,char *nom_ima);
WHERE_BSQ CAI_IMAGE *cai_ouvre_lecture_bsq(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BSQ CAI_IMAGE *cai_ouvre_modifie_bsq(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BSQ CAI_OK_KO cai_ferme_bsq(CAI_IMAGE *image1);
WHERE_BSQ CAI_IMAGE *cai_ouvre_creation_bsq(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_BSQ CAI_OK_KO cai_ecriture_canal_bsq(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_BSQ CAI_OK_KO cai_lecture_canal_bsq(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
WHERE_BSQ CAI_OK_KO cai_modif_nbbitspix_bsq(CAI_IMAGE *image1,
					    int  NbBitsPix);


WHERE_BIP long int cai_liste_ima_bip(char *repert,char ***tab_ima);
WHERE_BIP CAI_OK_KO cai_dest_ima_bip(char *repertoire,char *nom_ima);
WHERE_BIP CAI_IMAGE *cai_ouvre_lecture_bip(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BIP CAI_IMAGE *cai_ouvre_modifie_bip(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BIP CAI_OK_KO cai_ferme_bip(CAI_IMAGE *image1);
WHERE_BIP CAI_IMAGE *cai_ouvre_creation_bip(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_BIP CAI_OK_KO cai_ecriture_canal_bip(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_BIP CAI_OK_KO cai_lecture_canal_bip(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
WHERE_BIP CAI_OK_KO cai_modif_nbbitspix_bip(CAI_IMAGE *image1,
					int  NbBitsPix);

WHERE_INRIA long int cai_liste_ima_inria(char *repert,char ***tab_ima);
WHERE_INRIA CAI_OK_KO cai_dest_ima_inria(char *repertoire,char *nom_ima);
WHERE_INRIA CAI_IMAGE *cai_ouvre_lecture_inria(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_INRIA CAI_IMAGE *cai_ouvre_modifie_inria(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_INRIA CAI_OK_KO cai_ferme_inria(CAI_IMAGE *image1);
WHERE_INRIA CAI_IMAGE *cai_ouvre_creation_inria(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_INRIA CAI_OK_KO cai_ecriture_canal_inria(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_INRIA CAI_OK_KO cai_lecture_canal_inria(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);


WHERE_ONERA long int cai_liste_ima_onera(char *repert,char ***tab_ima);
WHERE_ONERA CAI_OK_KO cai_dest_ima_onera(char *repertoire,char *nom_ima);
WHERE_ONERA CAI_IMAGE *cai_ouvre_lecture_onera(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_ONERA CAI_IMAGE *cai_ouvre_modifie_onera(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_ONERA CAI_OK_KO cai_ferme_onera(CAI_IMAGE *image1);
WHERE_ONERA CAI_IMAGE *cai_ouvre_creation_onera(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_ONERA CAI_OK_KO cai_ecriture_canal_onera(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_ONERA CAI_OK_KO cai_lecture_canal_onera(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);


WHERE_TIFF long int cai_liste_ima_tiff(char *repert,char ***tab_ima);
WHERE_TIFF CAI_OK_KO cai_dest_ima_tiff(char *repertoire,char *nom_ima);
WHERE_TIFF CAI_IMAGE *cai_ouvre_lecture_tiff(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_TIFF CAI_IMAGE *cai_ouvre_modifie_tiff(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_TIFF CAI_OK_KO cai_ferme_tiff(CAI_IMAGE *image1);
WHERE_TIFF CAI_IMAGE *cai_ouvre_creation_tiff(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_TIFF CAI_OK_KO cai_ecriture_canal_tiff(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_TIFF CAI_OK_KO cai_lecture_canal_tiff(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);

WHERE_BIL long int cai_liste_ima_bil(char *repert,char ***tab_ima);
WHERE_BIL CAI_OK_KO cai_dest_ima_bil(char *repertoire,char *nom_ima);
WHERE_BIL CAI_IMAGE *cai_ouvre_lecture_bil(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BIL CAI_IMAGE *cai_ouvre_modifie_bil(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BIL CAI_OK_KO cai_ferme_bil(CAI_IMAGE *image1);
WHERE_BIL CAI_IMAGE *cai_ouvre_creation_bil(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_BIL CAI_OK_KO cai_ecriture_canal_bil(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_BIL CAI_OK_KO cai_lecture_canal_bil(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
WHERE_BIL CAI_OK_KO cai_modif_nbbitspix_bil(CAI_IMAGE *image1,
					         int  NbBitsPix);


WHERE_LUM long int cai_liste_ima_lum(char *repert,char ***tab_ima);
WHERE_LUM CAI_OK_KO cai_dest_ima_lum(char *repertoire,char *nom_ima);
WHERE_LUM CAI_IMAGE *cai_ouvre_lecture_lum(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_LUM CAI_IMAGE *cai_ouvre_modifie_lum(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_LUM CAI_OK_KO cai_ferme_lum(CAI_IMAGE *image1);
WHERE_LUM CAI_IMAGE *cai_ouvre_creation_lum(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_LUM CAI_OK_KO cai_ecriture_canal_lum(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_LUM CAI_OK_KO cai_lecture_canal_lum(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);


WHERE_RASTER long int cai_liste_ima_sun_raster(char *repert,char ***tab_ima);
WHERE_RASTER CAI_OK_KO cai_dest_ima_sun_raster(char *repertoire,char *nom_ima);
WHERE_RASTER CAI_IMAGE *cai_ouvre_lecture_sun_raster(char *repert,
		char *nom_image,int  *nb_can, int  *nboct_pix,
		int  *nb_col, int  *nb_lig);
WHERE_RASTER CAI_IMAGE *cai_ouvre_modifie_sun_raster(char *repert,
		char *nom_image, int  *nb_can, int  *nboct_pix,
		int  *nb_col, int  *nb_lig);
WHERE_RASTER CAI_OK_KO cai_ferme_sun_raster(CAI_IMAGE *image1);
WHERE_RASTER CAI_IMAGE *cai_ouvre_creation_sun_raster(char *repert,
		char *nom_image, int  *nb_can, int  *nboct_pix,
		int  *nb_col, int  *nb_lig, char* label);
WHERE_RASTER CAI_OK_KO cai_ecriture_canal_sun_raster(CAI_IMAGE *image1,
		 int *canal,  int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_RASTER CAI_OK_KO cai_lecture_canal_sun_raster(CAI_IMAGE *image1,
		 int *canal,int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);


WHERE_JPEG long int cai_liste_ima_jpeg(char *repert,char ***tab_ima);
WHERE_JPEG CAI_OK_KO cai_dest_ima_jpeg(char *repertoire,char *nom_ima);
WHERE_JPEG CAI_IMAGE *cai_ouvre_lecture_jpeg(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_JPEG CAI_IMAGE *cai_ouvre_modifie_jpeg(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_JPEG CAI_OK_KO cai_ferme_jpeg(CAI_IMAGE *image1);
WHERE_JPEG CAI_IMAGE *cai_ouvre_creation_jpeg(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_JPEG CAI_OK_KO cai_ecriture_canal_jpeg(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_JPEG CAI_OK_KO cai_lecture_canal_jpeg(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);


WHERE_GESSIMU long int cai_liste_ima_gessimu(char *repert,char ***tab_ima);
WHERE_GESSIMU CAI_OK_KO cai_dest_ima_gessimu(char *repertoire,char *nom_ima);
WHERE_GESSIMU CAI_IMAGE *cai_ouvre_lecture_gessimu(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_GESSIMU CAI_IMAGE *cai_ouvre_modifie_gessimu(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_GESSIMU CAI_OK_KO cai_ferme_gessimu(CAI_IMAGE *image1);
WHERE_GESSIMU CAI_IMAGE *cai_ouvre_creation_gessimu(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_GESSIMU CAI_OK_KO cai_ecriture_canal_gessimu(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_GESSIMU CAI_OK_KO cai_lecture_canal_gessimu(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);


WHERE_GRIL long int cai_liste_ima_grilles(char typgrille,char *repert,
					  char ***tab_ima);
WHERE_GRIL CAI_OK_KO cai_dest_ima_grilles(char typgrille,char *repertoire,
					  char *nom_ima);
WHERE_GRIL CAI_IMAGE *cai_ouvre_lecture_grilles(char typgrille,char *repert,
					  char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_GRIL CAI_IMAGE *cai_ouvre_modifie_grilles(char typgrille,char *repert,
					  char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_GRIL CAI_OK_KO cai_ferme_grilles(char typgrille,CAI_IMAGE *image1);
WHERE_GRIL CAI_IMAGE *cai_ouvre_creation_grilles(char typgrille,
				          char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_GRIL CAI_OK_KO cai_ecriture_canal_grilles(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_GRIL CAI_OK_KO cai_lecture_canal_grilles(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
WHERE_GRIL CAI_OK_KO cai_modif_nbbitspix_grilles(CAI_IMAGE *image1,
					         int  NbBitsPix);

WHERE_SPOT long int cai_liste_ima_spot(char typspot,char *repert,
					  char ***tab_ima);
WHERE_SPOT CAI_OK_KO cai_dest_ima_spot(char typspot,char *repertoire,
					  char *nom_ima);
WHERE_SPOT CAI_IMAGE *cai_ouvre_lecture_spot(char typspot,char *repert,
					  char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_SPOT CAI_IMAGE *cai_ouvre_modifie_spot(char typspot,char *repert,
					  char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_SPOT CAI_OK_KO cai_ferme_spot(char typspot,CAI_IMAGE *image1);
WHERE_SPOT CAI_IMAGE *cai_ouvre_creation_spot(char typspot,
				          char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_SPOT CAI_OK_KO cai_ecriture_canal_spot(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_SPOT CAI_OK_KO cai_lecture_canal_spot(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
WHERE_SPOT CAI_OK_KO cai_creat_leader_spot5( CAI_IMAGE *desc_im ,
							char *repertoire);


WHERE_RADAR long int cai_liste_ima_radar(char *repert,char ***tab_ima);
WHERE_RADAR CAI_OK_KO cai_dest_ima_radar(char *repertoire,char *nom_ima);
WHERE_RADAR CAI_IMAGE *cai_ouvre_lecture_radar(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_RADAR CAI_IMAGE *cai_ouvre_modifie_radar(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_RADAR CAI_OK_KO cai_ferme_radar(CAI_IMAGE *image1);
WHERE_RADAR CAI_IMAGE *cai_ouvre_creation_radar(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
WHERE_RADAR CAI_OK_KO cai_ecriture_canal_radar(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_RADAR CAI_OK_KO cai_lecture_canal_radar(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);

WHERE_RADAR int cai_istyp_radar(char *type_code,int *ind_typ, int *is_Cplx);

WHERE_BANDEAU long int cai_liste_ima_bandeau(char *repert,char ***tab_ima);
WHERE_BANDEAU CAI_OK_KO cai_dest_ima_bandeau(char *repertoire,char *nom_ima);
WHERE_BANDEAU CAI_IMAGE *cai_ouvre_lecture_bandeau(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
WHERE_BANDEAU CAI_OK_KO cai_ferme_bandeau(CAI_IMAGE *image1);
WHERE_BANDEAU CAI_OK_KO cai_lecture_canal_bandeau(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
#endif 

#define CAI_MAX_LG_FORMAT 20
#define CAI_MAX_LG_IMAGE  1024

#define PERMS 0666
#define CAI_NB_RADAR 28
#define CAI_NB_TYPE 8
#define CAI_NB_LUM 8

WHERE  char CAI_ERREUR[1024]; 
WHERE  CAI_IMAGE *FBANDEAU[20];

static int CAI_RADAR_LONG[CAI_NB_RADAR]=
{1,1,2,2,4,4,8,1,1,1,1,1,1,1,1,2,4,8,8,16,2,2,2,2,2,2,2,2};
static char *CAI_RADAR_TYP[CAI_NB_RADAR]=
{"OCT","PHA","I2","UI2","I4","R4","R8","1B","2B","3B","4B","5B","6B","7B","8B",
"COCT","CI2","CI4","CR4","CR8","C1B","C2B","C3B","C4B","C5B",
"C6B","C7B","C8B"};
static char *CAI_TYPE_CONNU[CAI_NB_TYPE]=
     {"OCT","UOCT","I2","UI2","I4","UI4","R4","R8"};
static char *CAI_LUM_TYP[CAI_NB_LUM]=
     {"BI","LI","BU","LU","FLOA","FLOL","R8B","R8L"};

#define CAI_OK				 0
#define CAI_KO		 		 1
#define CAI_IMPOS			 2
#define CAI_EOF         		-1
#define CAI_ERREUR_OUV  		-2
#define CAI_PB_LSEEK    		-3
#define CAI_DEB_NEG     		-4
#define CAI_FIN_DEP     		-5
#define CAI_ECRASEMENT  		-6
#define CAI_PB_READ 			-7
#define CAI_PB_WRITE    		-8
#define CAI_PB_FTRUNC   		-9
#define CAI_ERR_FORME_FIC 		-10
#define CAI_ERR_DO      		-11
#define CAI_ERR_TYP			-12
#define CAI_ERR_FIN_DO     		-13
#define CAI_ERR_ASCII   		-14
#define CAI_ERR_ALLOC   		-15
#define CAI_ERR_REEL    		-16
#define CAI_ERR_ENTIER  		-17
#define CAI_ERR_DIM			-18
#define CAI_ERREUR_RECHERCHE_MOT	-19
#define CAI_MOT_DEJA_EXISTE		-20
#define CAI_ERREUR_ENCODE		-21
#define CAI_ERR_ADRESSE			-22 
#define CAI_CLE_TROP_LONG		-23
#define CAI_MC_INCONNU			-24
#define CAI_PAS_MC			-24
#define CAI_ERR_CROCHET			-25
#define CAI_TROUVE	         	-26
#define CAI_NON_TROUVE       		-27


/* constantes relatives aux types de donnees */
#define CAI_INCONNU -1
#define CAI_ENTIER4 1
#define CAI_REEL4   2
#define CAI_REEL8   3
#define CAI_ASCII   4 
#define CAI_BLOC    5


/* prototype des fonctions */

WHERE_DA CAI_FILE_DA * cai_openDA(char *nom_fichier, char *type_image, 	
	    			  int nb_can,CAI_MODE_OUVERTURE mode_ouv);
WHERE_DA CAI_OK_KO cai_closeDA(CAI_FILE_DA *fd);
WHERE_DA CAI_OK_KO cai_removeDA(char *nom_fichier,char *type_image,int nb_can);
WHERE_DA CAI_OK_KO cai_lirecaractDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE *type,int *nb_lig,int *nb_col, 	
  	        	            long *taille_cell);
WHERE_DA CAI_OK_KO cai_lireDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE *type,int *nb_lig,int *nb_col, 	
  	        	            long *taille_cell,void *donnees);
WHERE_DA CAI_OK_KO cai_modifieDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE type,int nb_lig,int nb_col, 	
  	        	            long taille_cell,void *donnees);
WHERE_DA CAI_OK_KO cai_detruitDA(CAI_FILE_DA *fd,char *mot_cle);
WHERE_DA CAI_OK_KO cai_listeDA(CAI_FILE_DA *fd);
WHERE_DA CAI_OK_KO cai_ajouteDA(CAI_FILE_DA *fd,char *mot_cle,
				    CAI_TYPE type,int nb_lig,int nb_col, 	
  	        	            long taille_cell,void *donnees);
WHERE_DA CAI_OK_KO cai_lecture_colormap(CAI_IMAGE  *fdima);
WHERE_DA CAI_OK_KO cai_ecriture_colormap(CAI_IMAGE  *fdima);


WHERE_FONC long int cai_liste_image(char *repert,char *type_image,
				    char ***tab_ima);
WHERE_FONC long int cai_liste_format(char ***tab_format,
				     char **(tab_suffixe[2]));
WHERE_FONC CAI_OK_KO cai_destruction_image(char *format,char *nom_ima);
WHERE_FONC CAI_IMAGE *cai_ouvre_lecture_image(char *nom_image,char *type_image,
					 int  *nb_can, int  *nboct_pix,
					 int  *nb_col, int  *nb_lig);
WHERE_FONC CAI_IMAGE *cai_ouvre_modifie_image(char *nom_image,char *type_image,
					 int  *nb_can, int  *nboct_pix,
					 int  *nb_col, int  *nb_lig);
WHERE_FONC CAI_OK_KO cai_ferme_image(CAI_IMAGE *image1);
WHERE_FONC CAI_IMAGE *cai_ouvre_creation_image(char *nom_image,char *type_image,
					  int nb_can, int  nboct_pix,
					  int  nb_col, int  nb_lig,
					 char* label);
WHERE_FONC CAI_OK_KO cai_ecriture_canal(CAI_IMAGE *image1, int canal, 
         	 int premiere_ligne, int nombre_lignes_ecrire,
		 int nombre_colonnes_ecrire,unsigned char *data_image);
WHERE_FONC CAI_OK_KO cai_lecture_canal(CAI_IMAGE *image1, int canal,
         	 int premiere_ligne, int premiere_colonne,
		 int nombre_lignes_lire, int nombre_colonnes_lire,
		 int pas_lig, int pas_col,unsigned char *data);
WHERE_FONC CAI_OK_KO cai_modif_nbbitspix(char 	*format,CAI_IMAGE *image,
					 int NbBitsPix);

WHERE_FONC CAI_OK_KO cai_modif_nbbitspix(char 	*format,CAI_IMAGE *image,
					 int NbBitsPix);
WHERE_FONC void cai_inverser_tableau(short *BufL,short *BufE,int Nb);
WHERE_FONC void cai_inverser_tableau_complet(unsigned char *data_image,
					     int nbpix, int oct_pix);
WHERE_FONC char* cai_test_pFpf();
WHERE_FONC char* cai_En_majuscule( char *mot );
WHERE_FONC char* cai_En_minuscule( char *mot );
WHERE_FONC int   cai_istyp_connu(char *type_code);
WHERE_FONC   CAI_OK_KO cai_lead4to5spot ( CAI_IMAGE *desc_im ,char *repertoire);

#endif
#ifndef _WIN
#define O_BINARY 0
#endif
