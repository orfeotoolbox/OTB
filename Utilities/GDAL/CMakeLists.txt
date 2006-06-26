# This is the root GDAL CMakeLists file.
# $Id$
#


# ----------------- MACRO TRY COMPIL d'un FOLDER PORT ----------------------------------------------
# MACRO(GDAL_TRY_COMPIL_FOLDER folder liste_sources)
#  SET( OK 1 )
#  FOREACH(source ${liste_sources} )
#     CONFIGURE_FILE(${source}
#     ${OTB_BINARY_DIR}/Utilities/GDAL/try_compil.cxx COPYONLY IMMEDIATE)
#     FILE(WRITE ${OTB_BINARY_DIR}/Utilities/GDAL/try_compil.cxx "\nint main(int argc, char* argv[])\n{\n   return 1;\n}\n" )

#    CONFIGURE_FILE(${OTB_BINARY_DIR}/Utilities/GDAL/try_compil.cxx 
#    ${OTB_BINARY_DIR}/Utilities/GDAL/try_compil2.cxx
#    IMMEDIATE)

#     TRY_COMPILE(GDAL_TRY_COMPIL
#       ${OTB_BINARY_DIR}/CMakeTmp
#       ${OTB_BINARY_DIR}/Utilities/GDAL/try_compil.cxx
#       )
#      OUTPUT_VARIABLE OUTPUT)
#     IF( NOT GDAL_TRY_COMPIL)
#       SET( OK 0 )
#     ENDIF(NOT GDAL_TRY_COMPIL)
#   ENDFOREACH(source)
#   IF(OK)
#       MESSAGE(STATUS "Checking folder ${folder} -- yes")
#       SET(GDALliste_sources_SRCS ${GDALliste_sources_SRCS} ${liste_sources} )
#   ELSE(OK) 
#       MESSAGE(STATUS "Checking folder ${folder} -- no")
#   ENDIF(OK) 

# ENDMACRO(GDAL_TRY_COMPIL_FOLDER)






#CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/GDAL/port/cpl_config.h.in 
#               ${OTB_BINARY_DIR}/cpl_config.h)
#CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/GDAL/configure.in
#               ${OTB_BINARY_DIR}/configure)

#CHECK_INCLUDE_FILES("assert.h fcntl.h unistd.h dbmalloc.h dlfcn.h stdint.h limits.h locale.h values.h float.h errno.h" HAVE_ERRNO_H)
CHECK_INCLUDE_FILES("errno.h" HAVE_ERRNO_H)
#IF(HAVE_ERRNO_H)
        ADD_DEFINITIONS( -DHAVE_ERRNO_H  )
#ENDIF(HAVE_ERRNO_H)


ADD_DEFINITIONS(-DOGR_ENABLE)
ADD_DEFINITIONS(-DPIC)
ADD_DEFINITIONS(-DHAVE_MITAB)

# ----------------- PORT ----------------------------------------------
SET( GDALport_SRCS
        port/cpl_conv.cpp
        port/cpl_csv.cpp
        port/cpl_dir.cpp
        port/cpl_error.cpp
        port/cpl_findfile.cpp
        port/cpl_getexecpath.cpp
        port/cplgetsymbol.cpp
        port/cpl_list.cpp
        port/cpl_minixml.cpp
        port/cpl_multiproc.cpp
#        port/cpl_odbc.cpp
        port/cpl_path.cpp
        port/cpl_string.cpp
        port/cplstring.cpp
        port/cpl_vsil.cpp
        port/cpl_vsil_unix_stdio_64.cpp
        port/cpl_vsil_win32.cpp
        port/cpl_vsi_mem.cpp
        port/cpl_vsisimple.cpp
#        port/xmlreformat.cpp
)

#GDAL_TRY_COMPIL_FOLDER( port "${GDALport_SRCS}")



# ----------------- GCORE ----------------------------------------------
FILE(GLOB GDALgcore_SRCS "gcore/*.cpp" )

# ----------------- ALG ----------------------------------------------
FILE(GLOB GDALalgC_SRCS "alg/*.c" )
FILE(GLOB GDALalgCpp_SRCS "alg/*.cpp" )
SET(GDALalg_SRCS ${GDALalgCpp_SRCS} ${GDALalgC_SRCS})

# ----------------- FRMTS ----------------------------------------------
 ADD_DEFINITIONS( -DFRMT_gxf  )
 ADD_DEFINITIONS( -DFRMT_gtiff ) 
 ADD_DEFINITIONS(-DFRMT_hfa ) 
 ADD_DEFINITIONS(-DFRMT_aigrid  )
 ADD_DEFINITIONS(-DFRMT_aaigrid  )
 ADD_DEFINITIONS(-DFRMT_ceos  )
 ADD_DEFINITIONS(-DFRMT_ceos2  )
 ADD_DEFINITIONS(-DFRMT_iso8211  )
 ADD_DEFINITIONS(-DFRMT_xpm  )
 ADD_DEFINITIONS(-DFRMT_sdts  )
 ADD_DEFINITIONS(-DFRMT_raw  )
 ADD_DEFINITIONS(-DFRMT_dted  )
 ADD_DEFINITIONS(-DFRMT_mem  )
 ADD_DEFINITIONS(-DFRMT_jdem  )
 ADD_DEFINITIONS(-DFRMT_envisat  )
 ADD_DEFINITIONS(-DFRMT_elas  )
 ADD_DEFINITIONS(-DFRMT_fit  )
 ADD_DEFINITIONS(-DFRMT_vrt  )
 ADD_DEFINITIONS(-DFRMT_usgsdem  )
 ADD_DEFINITIONS(-DFRMT_l1b  )
 ADD_DEFINITIONS(-DFRMT_nitf  )
 ADD_DEFINITIONS(-DFRMT_bmp  )
 ADD_DEFINITIONS(-DFRMT_pcidsk  )
 ADD_DEFINITIONS(-DFRMT_airsar  )
 ADD_DEFINITIONS(-DFRMT_rs2  )
 ADD_DEFINITIONS(-DFRMT_ilwis  )
 ADD_DEFINITIONS(-DFRMT_rmf  )
 ADD_DEFINITIONS(-DFRMT_rik  )
 ADD_DEFINITIONS(-DFRMT_leveller  )
 ADD_DEFINITIONS(-DFRMT_sgi  )
 ADD_DEFINITIONS(-DFRMT_idrisi  )
 ADD_DEFINITIONS(-DFRMT_bsb  )
 ADD_DEFINITIONS(-DFRMT_gif  )
 ADD_DEFINITIONS(-DFRMT_jpeg  )
 ADD_DEFINITIONS(-DFRMT_png  )
 ADD_DEFINITIONS(-DFRMT_pcraster)
 
 ADD_DEFINITIONS(-DGDAL_FORMATS="gxf gtiff hfa aigrid aaigrid ceos ceos2 iso8211 xpm sdts raw dted mem jdem envisat elas fit vrt usgsdem l1b nitf bmp pcidsk airsar rs2 ilwis rmf rik leveller sgi idrisi bsb gif jpeg png pcraster " )
 
SET(GDALfrmts_SRCS frmts/gdalallregister.cpp)

 ADD_DEFINITIONS(-DUSE_IN_GDAL)
 
 
# ----------------- SUBDIRS FRMTS ----------------------------------------------

SET(GDALfrmtsSubDirs_SRCS)

# ----------------- SUBDIRS FRMTS   HFA    ----------------------------------------------
FILE(GLOB GDALfrmts_hfa_SRCS "frmts/hfa/*.cpp" )
SET(GDALfrmtsSubDirs_SRCS ${GDALfrmtsSubDirs_SRCS} ${GDALfrmts_hfa_SRCS})

# ----------------- SUBDIRS FRMTS   ISO8211    ----------------------------------------------
FILE(GLOB GDALfrmts_iso8211_SRCS "frmts/iso8211/dd*.cpp" )
SET(GDALfrmtsSubDirs_SRCS ${GDALfrmtsSubDirs_SRCS} ${GDALfrmts_iso8211_SRCS})

# ----------------- SUBDIRS FRMTS   GTIFF    ----------------------------------------------
SET( GDALfrmts_gtiff_SRCS
        frmts/gtiff/geotiff.cpp
        frmts/gtiff/gt_wkt_srs.cpp
        frmts/gtiff/gt_overview.cpp
        frmts/gtiff/tif_ovrcache.c
        frmts/gtiff/tif_overview.c
        frmts/gtiff/tif_float.c
        frmts/gtiff/tifvsi.cpp
)

SET(GDALfrmtsSubDirs_SRCS ${GDALfrmtsSubDirs_SRCS} ${GDALfrmts_gtiff_SRCS})



SET( OTB_frmtsSubDirs
gxf
gtiff/libtiff
gtiff/libgeotiff
aigrid
aaigrid
ceos
ceos2
xpm
sdts
raw
dted
mem
jdem
envisat
elas
fit
vrt
usgsdem
l1b
nitf
bmp
pcidsk
airsar
rs2
ilwis

rmf
rik
leveller
sgi
idrisi
bsb
gif
jpeg
png
pcraster
pcraster/libcsf

gif/libungif
png/libpng
jpeg/libjpeg

zlib

#hfa/addtiffo_src
)

# Parcours repertoires
FOREACH(frmtsSubDirs ${OTB_frmtsSubDirs} )
        SET( LOCALfrmtsSubDirs_SRCS )
        FILE(GLOB LOCALfrmtsSubDirsCpp_SRCS "frmts/${frmtsSubDirs}/*.cpp" )
        FILE(GLOB LOCALfrmtsSubDirsC_SRCS "frmts/${frmtsSubDirs}/*.c" )
        SET (LOCALfrmtsSubDirs_SRCS ${LOCALfrmtsSubDirsCpp_SRCS} ${LOCALfrmtsSubDirsC_SRCS} )
        SET( GDALfrmtsSubDirs_SRCS ${GDALfrmtsSubDirs_SRCS} ${LOCALfrmtsSubDirs_SRCS} )
ENDFOREACH(frmtsSubDirs)




# ----------------- OGR ----------------------------------------------
SET( GDALogr_SRCS
        ogr/ogrgeometryfactory.cpp
        ogr/ogrpoint.cpp
        ogr/ogrcurve.cpp
        ogr/ogrlinestring.cpp
        ogr/ogrlinearring.cpp
        ogr/ogrpolygon.cpp
        ogr/ogrutils.cpp
        ogr/ogrgeometry.cpp
        ogr/ogrgeometrycollection.cpp
        ogr/ogrmultipolygon.cpp
        ogr/ogrsurface.cpp
        ogr/ogrmultipoint.cpp
        ogr/ogrmultilinestring.cpp
        ogr/ogr_api.cpp
        ogr/ogrfeature.cpp
        ogr/ogrfeaturedefn.cpp
        ogr/ogrfeaturequery.cpp
        ogr/ogrfeaturestyle.cpp
        ogr/ogrfielddefn.cpp
        ogr/ogrspatialreference.cpp
        ogr/ogr_srsnode.cpp
        ogr/ogr_srs_proj4.cpp
        ogr/ogr_fromepsg.cpp
        ogr/ogrct.cpp
        ogr/ogr_opt.cpp
        ogr/ogr_srs_esri.cpp
        ogr/ogr_srs_pci.cpp
        ogr/ogr_srs_usgs.cpp
        ogr/ogr_srs_dict.cpp
        ogr/ogr_srs_panorama.cpp
        ogr/swq.c
        ogr/ogr_srs_validate.cpp
        ogr/ogr_srs_xml.cpp
        ogr/ograssemblepolygon.cpp
        ogr/ogr2gmlgeometry.cpp
        ogr/gml2ogrgeometry.cpp
)

# ----------------- SUBDIRS OGR ----------------------------------------------

SET(GDALogrSubDirs_SRCS)


# ----------------- SUBDIRS OGR     DGN    ----------------------------------------------
SET( GDALogr_dgn_SRCS
        ogr/ogrsf_frmts/dgn/dgnopen.cpp
        ogr/ogrsf_frmts/dgn/dgnread.cpp 
        ogr/ogrsf_frmts/dgn/dgnfloat.cpp 
        ogr/ogrsf_frmts/dgn/dgnhelp.cpp 
        ogr/ogrsf_frmts/dgn/dgnwrite.cpp 
        ogr/ogrsf_frmts/dgn/dgnstroke.cpp 
        ogr/ogrsf_frmts/dgn/ogrdgndriver.cpp 
        ogr/ogrsf_frmts/dgn/ogrdgndatasource.cpp 
        ogr/ogrsf_frmts/dgn/ogrdgnlayer.cpp
)

SET(GDALogrSubDirs_SRCS ${GDALogrSubDirs_SRCS} ${GDALogr_dgn_SRCS})

# ----------------- SUBDIRS OGR     GML    ----------------------------------------------
SET( GDALogr_gml_SRCS
        ogr/ogrsf_frmts/gml/gmlpropertydefn.cpp
        ogr/ogrsf_frmts/gml/gmlfeatureclass.cpp 
        ogr/ogrsf_frmts/gml/gmlfeature.cpp 
        ogr/ogrsf_frmts/gml/gmlreader.cpp 
        ogr/ogrsf_frmts/gml/parsexsd.cpp 
)

SET(GDALogrSubDirs_SRCS ${GDALogrSubDirs_SRCS} ${GDALogr_gml_SRCS})

SET( OTB_ogrSubDirs
generic
shape
ntf
sdts
tiger
s57
mitab
#dgn
#gml
avc
rec
mem
vrt
csv
)

# Que pour subdir "ogrsf_frmts/generic"
#  ADD_DEFINITIONS-DSHAPE_ENABLED)
#  ADD_DEFINITIONS -DTAB_ENABLED)
#  ADD_DEFINITIONS(-DNTF_ENABLED)
#  ADD_DEFINITIONS(-DSDTS_ENABLED)
#  ADD_DEFINITIONS(-DTIGER_ENABLED)
#  ADD_DEFINITIONS(-DS57_ENABLED)
#  ADD_DEFINITIONS(-DDGN_ENABLED)
#  ADD_DEFINITIONS(-DVRT_ENABLED)
#  ADD_DEFINITIONS(-DAVCBIN_ENABLED)
#  ADD_DEFINITIONS(-DREC_ENABLED)
#  ADD_DEFINITIONS(-DMEM_ENABLED)
#  ADD_DEFINITIONS(-DCSV_ENABLED)
#  ADD_DEFINITIONS(-DGML_ENABLED)

# Parcours repertoires
FOREACH(ogrSubDirs ${OTB_ogrSubDirs} )
        SET( LOCALogrSubDirs_SRCS )
        FILE(GLOB LOCALogrSubDirsCpp_SRCS "ogr/ogrsf_frmts/${ogrSubDirs}/*.cpp" )
        FILE(GLOB LOCALogrSubDirsC_SRCS "ogr/ogrsf_frmts/${ogrSubDirs}/*.c" )
        SET (LOCALogrSubDirs_SRCS ${LOCALogrSubDirsCpp_SRCS} ${LOCALogrSubDirsC_SRCS} )
        SET( GDALogrSubDirs_SRCS ${GDALogrSubDirs_SRCS} ${LOCALogrSubDirs_SRCS} )
ENDFOREACH(ogrSubDirs)







# ----------------- LIBRARY ----------------------------------------------
ADD_LIBRARY(gdal ${GDALport_SRCS}  ${GDALgcore_SRCS} ${GDALalg_SRCS} ${GDALfrmts_SRCS} ${GDALfrmtsSubDirs_SRCS} ${GDALogr_SRCS} ${GDALogrSubDirs_SRCS})



#ADD_LIBRARY(gdal ${GDALliste_sources_SRCS} )


#ADD_EXECUTABLE(otbgdalinfo ${OTB_SOURCE_DIR}/Utilities/GDAL/apps/gdalinfo.cpp)
#TARGET_LINK_LIBRARIES(otbgdalinfo -lm gdal -lgif /usr/lib/libjpeg.so -ltiff -lpng -lz -lrt -ldl )

#ADD_EXECUTABLE(otbgdalinfo ${OTB_SOURCE_DIR}/Utilities/GDAL/apps/otbgdalinfo.cxx)
#TARGET_LINK_LIBRARIES(otbgdalinfo gdal)

INSTALL_TARGETS(/lib/otb gdal )
INSTALL_FILES(/include/otb/Utilities/GDAL "(\\.h)$")
INSTALL_FILES(/include/otb/Utilities/GDAL "(gcore/\\.h|\\.txx)$")

