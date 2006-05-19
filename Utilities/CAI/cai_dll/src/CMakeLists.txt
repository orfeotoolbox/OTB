# Sources of non-templated classes.

SET(CAI_SRCS 
        cai_fonctions.c
        cai_bsq.c
        cai_bip.c
        cai_inria.c
        cai_onera.c
        cai_bil.c
        cai_lum.c
        cai_gessimu.c
        cai_grilles.c
        cai_tiff.c 
        cai_jpeg.c 
        cai_rasterfile.c
        cai_DonAux.c 
        cai_radar.c 
        cai_spot.c 
        cai_bandeau.c 
 )

ADD_LIBRARY(cai ${CAI_SRCS})

INSTALL_TARGETS(/lib/otb cai )
INSTALL_FILES(/include/otb/Utilities/CAI "(\\.h)$")
