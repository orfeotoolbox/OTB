/*---------------------------------------------*/
/* Makefile COUCHE ACCES IMAGE pour AIX	       */
/* Version : $Header: cai_biba_AIX.mk,v 1.3 01/01/15 15:24:12 cmf Exp $ */
/*--------------------------------------------*/

/* Chemins d'acces */

DIRSRC = $(RACINE_ATLAS)/src
DIRBIB = $(RACINE_ATLAS)/bib/bin_AIX
DIRINC = $(RACINE_ATLAS)/inc
TIFFDIR = $(RACINE_ATLAS)/src/tiff
JPEGDIR = $(RACINE_ATLAS)/src/jpeg
RANLIB =  $(RACINE_ATLAS)/src/tiff/RANLIB.csh

/* Option de compilation */

RM= rm
CC = cc
/*CCOPTS	=  -Ae  -D_AIX_SOURCE */
CCOPTS	=	-D_SOLARIS

#ifdef _HDF16
HDFINC = $(RACINE_HDF)/include
DIRHDF = $(RACINE_HDF)
FLAGS = -I$(HDFINC) -I$(JPEGINC) -I$(DIRINC) -D_HDF16
#endif
#ifdef _NOHDF16
HDFINC = 
DIRHDF = 
FLAGS = -I$(DIRINC) -D_NOHDF16
#endif

/*################################*/
/*# Marquage			  */
/*################################*/

PRODUIT       = CAI
VERSION       = 02.00
FIC_TMP       = $(DIRBIB)/version
FIC_VERS_SRC  = $(FIC_TMP).c
FIC_VERS_OBJ  = $(FIC_TMP).o

/* Liste des .o JPEG */

JPEGOBJECTS= $(JPEGDIR)/jcomapi.o $(JPEGDIR)/jutils.o $(JPEGDIR)/jerror.o $(JPEGDIR)/jmemmgr.o \
	$(JPEGDIR)/jcapimin.o $(JPEGDIR)/jcapistd.o $(JPEGDIR)/jctrans.o $(JPEGDIR)/jcparam.o \
	$(JPEGDIR)/jdatadst.o $(JPEGDIR)/jcinit.o $(JPEGDIR)/jcmaster.o \
	$(JPEGDIR)/jcmarker.o $(JPEGDIR)/jcmainct.o $(JPEGDIR)/jcprepct.o \
	$(JPEGDIR)/jccoefct.o $(JPEGDIR)/jccolor.o $(JPEGDIR)/jcsample.o \
	$(JPEGDIR)/jchuff.o $(JPEGDIR)/jcphuff.o $(JPEGDIR)/jcdctmgr.o \
	$(JPEGDIR)/jfdctfst.o $(JPEGDIR)/jfdctflt.o $(JPEGDIR)/jfdctint.o \
	$(JPEGDIR)/jdapimin.o $(JPEGDIR)/jdapistd.o $(JPEGDIR)/jdtrans.o $(JPEGDIR)/jdatasrc.o $(JPEGDIR)/jdmaster.o \
        $(JPEGDIR)/jdinput.o $(JPEGDIR)/jdmarker.o $(JPEGDIR)/jdhuff.o $(JPEGDIR)/jdphuff.o $(JPEGDIR)/jdmainct.o \
	$(JPEGDIR)/jdcoefct.o $(JPEGDIR)/jdpostct.o  $(JPEGDIR)/jddctmgr.o $(JPEGDIR)/jidctfst.o $(JPEGDIR)/jidctflt.o \
	$(JPEGDIR)/jidctint.o $(JPEGDIR)/jidctred.o $(JPEGDIR)/jmemnobs.o\
        $(JPEGDIR)/jdsample.o $(JPEGDIR)/jdcolor.o $(JPEGDIR)/jquant1.o $(JPEGDIR)/jquant2.o $(JPEGDIR)/jdmerge.o

/*# Liste des binaires*/

#ifdef _HDF16

LIBHDF=$(DIRHDF)/lib/libdf.a $(DIRHDF)/lib/libmfhdf.a $(DIRHDF)/lib/libz.a $(DIRHDF)/lib/libjpeg.a

DEPBIN=$(DIRBIB)/cai_fonctions.o \
	$(DIRBIB)/cai_bsq.o \
	$(DIRBIB)/cai_bip.o \
	$(DIRBIB)/cai_hdf.o \
	$(DIRBIB)/cai_inria.o \
	$(DIRBIB)/cai_onera.o \
	$(DIRBIB)/cai_bil.o \
	$(DIRBIB)/cai_lum.o \
	$(DIRBIB)/cai_gessimu.o \
	$(DIRBIB)/cai_grilles.o \
	$(DIRBIB)/cai_tiff.o \
	$(DIRBIB)/cai_jpeg.o \
	$(DIRBIB)/cai_rasterfile.o \
	$(DIRBIB)/cai_spot.o \
	$(DIRBIB)/cai_radar.o \
        $(DIRBIB)/cai_DonAux.o \
	$(DIRBIB)/cai_bandeau.o
#endif
#ifdef _NOHDF16
DEPBIN=$(DIRBIB)/cai_fonctions.o \
	$(DIRBIB)/cai_bsq.o \
	$(DIRBIB)/cai_bip.o \
	$(DIRBIB)/cai_inria.o \
	$(DIRBIB)/cai_onera.o \
	$(DIRBIB)/cai_bil.o \
	$(DIRBIB)/cai_lum.o \
	$(DIRBIB)/cai_gessimu.o \
	$(DIRBIB)/cai_grilles.o \
	$(DIRBIB)/cai_tiff.o \
	$(DIRBIB)/cai_jpeg.o \
	$(DIRBIB)/cai_rasterfile.o \
	$(DIRBIB)/cai_spot.o \
	$(DIRBIB)/cai_radar.o \
        $(DIRBIB)/cai_DonAux.o \
	$(DIRBIB)/cai_bandeau.o
#endif

ALL1=	$(DIRBIB)/cai_couche_ima.a
LIBTIFF = $(DIRBIB)/libtiff.a
LIBJPEG = 


all:	$(LIBTIFF) ${ALL1} 

/*# Generation de la bibliotheque simple*/

${ALL1} : $(DEPBIN) $(DIRINC)/cai_image.h $(JPEGOBJECTS)
	@echo "--> Identification de la bibliotheque simple"
	@$(RM) -f $(FIC_VERS_SRC) $(FIC_VERS_OBJ)
	@echo "static char ident[] = \"@(#)$(PRODUIT) $(VERSION)\";" > $(FIC_VERS_SRC)
	@$(CC) -c  $(FLAGS) -o $(FIC_VERS_OBJ) $(FIC_VERS_SRC)

#ifdef _HDF16
	@echo "--> Extraction objets HDF"
	mkdir $(DIRBIB)/HDF;cd $(DIRBIB)/HDF; ar x  $(DIRHDF)/lib/libdf.a; \
	ar x $(DIRHDF)/lib/libmfhdf.a; ar x $(DIRHDF)/lib/libz.a

	@echo "--> Generation de la bibliotheque simple"
	ar rv $@ $(FIC_VERS_OBJ) $(DEPBIN) $(JPEGOBJECTS) `find $(DIRBIB)/HDF -type f -name '*.o' -print` 
	ranlib $(DIRBIB)/cai_couche_ima.a
	@$(RM) -rf $(DIRBIB)/HDF
#endif
#ifdef _NOHDF16
	@echo "--> Generation de la bibliotheque simple"
	ar rv $@ $(FIC_VERS_OBJ) $(DEPBIN) $(JPEGOBJECTS)
	ranlib $(DIRBIB)/cai_couche_ima.a
#endif
	@$(RM) -f $(FIC_VERS_OBJ) $(FIC_VERS_SRC) $(DEPBIN)  $(JPEGOBJECTS)

/*# Generation des binaires */

$(DIRBIB)/cai_fonctions.o : $(DIRSRC)/cai_fonctions.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_fonctions.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_fonctions.c -o $@

$(DIRBIB)/cai_bsq.o : $(DIRSRC)/cai_bsq.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bsq.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bsq.c -o $@

$(DIRBIB)/cai_bip.o : $(DIRSRC)/cai_bip.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bip.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bip.c -o $@

$(DIRBIB)/cai_inria.o : $(DIRSRC)/cai_inria.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_inria.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_inria.c -o $@

$(DIRBIB)/cai_onera.o : $(DIRSRC)/cai_onera.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_onera.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_onera.c -o $@

$(DIRBIB)/cai_bil.o : $(DIRSRC)/cai_bil.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bil.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bil.c -o $@

$(DIRBIB)/cai_lum.o : $(DIRSRC)/cai_lum.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_lum.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_lum.c -o $@

$(DIRBIB)/cai_gessimu.o : $(DIRSRC)/cai_gessimu.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_gessimu.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_gessimu.c -o $@

$(DIRBIB)/cai_grilles.o : $(DIRSRC)/cai_grilles.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_grilles.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_grilles.c -o $@

$(DIRBIB)/cai_tiff.o : $(DIRSRC)/cai_tiff.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_tiff.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_tiff.c -o $@

$(DIRBIB)/cai_jpeg.o : $(DIRSRC)/cai_jpeg.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_jpeg.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_jpeg.c -o $@

$(DIRBIB)/cai_spot.o : $(DIRSRC)/cai_spot.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_spot.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_spot.c -o $@

$(DIRBIB)/cai_bandeau.o : $(DIRSRC)/cai_bandeau.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bandeau.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bandeau.c -o $@

$(DIRBIB)/cai_radar.o : $(DIRSRC)/cai_radar.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_sp.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_radar.c -o $@

#ifdef _HDF16
$(DIRBIB)/cai_hdf.o : $(DIRSRC)/cai_hdf.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_hdf.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_hdf.c -o $@
#endif

$(DIRBIB)/cai_rasterfile.o : $(DIRSRC)/cai_rasterfile.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_rasterfile.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_rasterfile.c -o $@

$(DIRBIB)/cai_DonAux.o : $(DIRSRC)/cai_DonAux.c
	@echo "--> Generation du binaire cai_DonAux.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_DonAux.c -o $@

$(LIBTIFF):
	( cd $(TIFFDIR) ; make -f Makefile clean ; make -f Makefile CC='$(CC)';cp libtiff.a $(DIRBIB))

$(JPEGOBJECTS):
	( CC='gcc -g'; cd  $(JPEGDIR) ; make -f makefile.cai clean; make -f makefile.cai CC='$(CC)' )



