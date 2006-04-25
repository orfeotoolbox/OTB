#!/bin/sh
# Version : $Header: COUCHE_IMA_bib.sh,v 1.4 98/05/15 17:40:49 cmf Exp $
# Ce fichier permet de definir l environnement necessaire
# a la bibliotheque COUCHE_IMAGE Couche d'Acces Images


COUCHE_IMA_INC=$RACINE_ATLAS/inc
export COUCHE_IMA_INC

if [ "-$MACHINE_ATLAS" = "-HPUX" ]
then
    
	COUCHE_IMA_BIB=$RACINE_ATLAS/bib/bin_HPUX	

elif [ "-$MACHINE_ATLAS" = "-AIX" ]
then

        COUCHE_IMA_BIB=$RACINE_ATLAS/bib/bin_AIX 
 
elif [ "-$MACHINE_ATLAS" = "-SUNOS" ]
then
	COUCHE_IMA_BIB=$RACINE_ATLAS/bib/bin_SUNOS

elif [ "-$MACHINE_ATLAS" = "-SOLARIS" ]
then
	
	COUCHE_IMA_BIB=$RACINE_ATLAS/bib/bin_SOLARIS

elif [ "-$MACHINE_ATLAS" = "-ITANIUM2" ]
then
	
	COUCHE_IMA_BIB=$RACINE_ATLAS/bib/bin_ITANIUM2
elif [ "-$MACHINE_ATLAS" = "-XEON" ]
then
	
	COUCHE_IMA_BIB=$RACINE_ATLAS/bib/bin_XEON
fi
export COUCHE_IMA_BIB
