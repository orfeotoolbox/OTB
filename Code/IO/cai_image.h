/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __cai_image_h
#define __cai_image_h

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

#define CAI_NB_MAX_CAN 250
#define PERMS 0666
#define CAI_NB_TYPE 8
#define CAI_NB_LUM 8
static char CAI_ERREUR[1024]; 

#ifndef _WIN
#define O_BINARY 0
#endif

typedef enum{CAI_LECTURE,CAI_ECRITURE}  CAI_MODE_OUVERTURE;
typedef long int                        CAI_OK_KO;

static char *CAI_TYPE_CONNU[CAI_NB_TYPE]= {"OCT","UOCT","I2","UI2","I4","UI4","R4","R8"};
static char *CAI_LUM_TYP[CAI_NB_LUM]= {"BI","LI","BU","LU","FLOA","FLOL","R8B","R8L"};

namespace otb
{

/** \class cai_image 
 * tous les attributs sont déclarés PUBLIC pour assurer une compatibilité avec les fonctions de la CAI
 * et permettre une facilité de mise à jour
 *
 *
 */
class CAI_IMAGE 
{

public:

  /** Construtor.*/
  CAI_IMAGE() {};
  /** Destructor.*/
  ~CAI_IMAGE() {};

       
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
        FILE *ACCES_FILE_JPEG;
        int  ACCES_CANAL_INT[CAI_NB_MAX_CAN];
        int  POS_OCTET[CAI_NB_MAX_CAN];
	char NOM_FIC_CAN[100*CAI_NB_MAX_CAN];
	char NOM_UTIL_CAN[100*CAI_NB_MAX_CAN];

        int  HISTO[CAI_NB_MAX_CAN][256];
        int  NB_PIX_ECRITS[CAI_NB_MAX_CAN];
        CAI_MODE_OUVERTURE MODE;

  
protected:
  
};

} // end namespace otb

#endif // CAI_IMAGE__H
