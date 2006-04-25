#\*--------------------------------------------*
#\* Makefile COUCHE ACCES IMAGE pour PC NT *
#\* Version : $Header: cai_biba_NT.mk,v 1.1 01\01\15 15:24:15 cmf Exp $ *
#\*--------------------------------------------*
#\* Chemins d'acces *

DIRSRC = $(RACINE_ATLAS)/src
DIRBIB = $(RACINE_ATLAS)/bib/bin_WIN
DIRINC = $(RACINE_ATLAS)/inc
TIFFDIR = $(RACINE_ATLAS)/src/tiff
JPEGDIR = $(RACINE_ATLAS)/src/jpeg
JPEGINC = $(RACINE_ATLAS)/src/jpeg
RANLIB =  ranlib

#\* Option de compilation *

RM = rm
CC = gcc
CCOPTS	= -D_WIN
AR = ar

 DIRHDF=
 HDFINC =
 FLAGS = -I$(DIRINC) -I$(JPEGINC) -D_NOHDF16

#\*#################################*
#\*## Marquage			  *
#\*#################################*

PRODUIT       = CAI
VERSION       = 05.01
FIC_TMP       = ./version
FIC_VERS_SRC  = $(FIC_TMP).c
FIC_VERS_OBJ  = $(FIC_TMP).o

#\* Liste des .o JPEG *

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

#\* Liste des binaires *

 DEPBIN=cai_fonctions.o \
	cai_bsq.o \
	cai_bip.o \
	cai_inria.o \
	cai_onera.o \
	cai_bil.o \
	cai_lum.o \
	cai_gessimu.o \
	cai_grilles.o \
	cai_tiff.o \
	cai_jpeg.o \
	cai_rasterfile.o \
        cai_DonAux.o \
	cai_radar.o\
	cai_spot.o \
	cai_bandeau.o  

ALL1=	cai_couche_ima.a
LIBTIFF = $(DIRBIB)\libtiff.a
LIBJPEG = 

all:	$(LIBTIFF) ${ALL1}

#\*  DEPLACEMENT .a *
	@echo "--> Deplacement de "cai_couche_ima.a" sous $(DIRBIB)."
	@cp cai_couche_ima.a $(DIRBIB)
	@rm -f cai_couche_ima.a


#\* Generation de la bibliotheque simple *

${ALL1} : $(DEPBIN) $(DIRINC)/cai_image.h $(JPEGOBJECTS)
	@echo "--> Identification de la bibliotheque simple"
	@echo static char ident[] = "@(#)$(PRODUIT) $(VERSION)"; > $(FIC_VERS_SRC)
	@$(CC) -c  $(FLAGS) -o $(FIC_VERS_OBJ) $(FIC_VERS_SRC)

	@echo "--> Generation de la bibliotheque simple"
	${AR} rv $@ $(FIC_VERS_OBJ) $(DEPBIN) $(JPEGOBJECTS) 
	@echo "--> Creation de cai_couche_ima.a"
	${RANLIB} cai_couche_ima.a

	@echo "--> Effacement des .o sous gen et sous src\JPEG."
	@rm -f version.c 
	@rm -f *.o
	@rm -f $(JPEGOBJECTS)

	@echo "--> Effacement des .o sous src\TIFF."
	@rm -f $(TIFFDIR)\*.o

#\* Generation des binaires *

cai_fonctions.o : $(DIRSRC)/cai_fonctions.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_fonctions.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_fonctions.c -o $@

cai_bsq.o : $(DIRSRC)/cai_bsq.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bsq.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bsq.c -o $@

cai_bip.o : $(DIRSRC)/cai_bip.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bip.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bip.c -o $@

cai_inria.o : $(DIRSRC)/cai_inria.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_inria.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_inria.c -o $@

cai_onera.o : $(DIRSRC)/cai_onera.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_onera.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_onera.c -o $@

cai_bil.o : $(DIRSRC)/cai_bil.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bil.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bil.c -o $@

cai_lum.o : $(DIRSRC)/cai_lum.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_lum.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_lum.c -o $@

cai_gessimu.o : $(DIRSRC)/cai_gessimu.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_gessimu.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_gessimu.c -o $@

cai_grilles.o : $(DIRSRC)/cai_grilles.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_grilles.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_grilles.c -o $@

cai_tiff.o : $(DIRSRC)/cai_tiff.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_tiff.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_tiff.c -o $@

cai_jpeg.o : $(DIRSRC)/cai_jpeg.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_jpeg.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_jpeg.c -o $@

cai_rasterfile.o : $(DIRSRC)/cai_rasterfile.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_rasterfile.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_rasterfile.c -o $@

cai_DonAux.o : $(DIRSRC)/cai_DonAux.c
	@echo "--> Generation du binaire cai_DonAux.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_DonAux.c -o $@

cai_spot.o : $(DIRSRC)/cai_spot.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_spot.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_spot.c -o $@

cai_bandeau.o : $(DIRSRC)/cai_bandeau.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_bandeau.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_bandeau.c -o $@

cai_radar.o : $(DIRSRC)/cai_radar.c $(DIRINC)/cai_image.h
	@echo "--> Generation du binaire cai_radar.o"
	$(CC) $(CCOPTS) -c  $(FLAGS) $(DIRSRC)/cai_radar.c -o $@

$(LIBTIFF):
	( cd $(TIFFDIR) ; make -f Makefile.gcc.nt clean ; make -f Makefile.gcc.nt CC='$(CC)' 'CCOPTS=$(CCOPTS)')


$(JPEGOBJECTS):
	(cd  $(JPEGDIR) ;make -f makefile.cai.nt clean;  make -f makefile.cai.nt CC='$(CC)' 'CCOPTS=$(CCOPTS)')

