# This is the root GDAL CMakeLists file.
# $Id$
#


SUBDIRS( gcore port ogr alg apps )


SET( GDALgtiff_SRCS
        frmts/gtiff/libgeotiff/geo_free.c
        frmts/gtiff/libgeotiff/geo_get.c
        frmts/gtiff/libgeotiff/geo_names.c
        frmts/gtiff/libgeotiff/geo_new.c
        frmts/gtiff/libgeotiff/geo_print.c
        frmts/gtiff/libgeotiff/geo_set.c
        frmts/gtiff/libgeotiff/geo_tiffp.c
        frmts/gtiff/libgeotiff/geo_write.c
        frmts/gtiff/libgeotiff/geo_normalize.c
        frmts/gtiff/libgeotiff/geotiff_proj4.c
        frmts/gtiff/libgeotiff/geo_extra.c
        frmts/gtiff/libgeotiff/geo_trans.c
        frmts/gtiff/libgeotiff/xtiff.c

        frmts/gtiff/libtiff/tif_aux.c
	frmts/gtiff/libtiff/tif_close.c
	frmts/gtiff/libtiff/tif_codec.c
	frmts/gtiff/libtiff/tif_color.c
	frmts/gtiff/libtiff/tif_compress.c
	frmts/gtiff/libtiff/tif_dir.c
	frmts/gtiff/libtiff/tif_dirinfo.c
	frmts/gtiff/libtiff/tif_dirread.c
	frmts/gtiff/libtiff/tif_dirwrite.c
	frmts/gtiff/libtiff/tif_dumpmode.c
	frmts/gtiff/libtiff/tif_error.c
	frmts/gtiff/libtiff/tif_extension.c
	frmts/gtiff/libtiff/tif_fax3.c
	frmts/gtiff/libtiff/tif_fax3sm.c
	frmts/gtiff/libtiff/tif_getimage.c
	frmts/gtiff/libtiff/tif_jpeg.c
	frmts/gtiff/libtiff/tif_flush.c
	frmts/gtiff/libtiff/tif_luv.c
	frmts/gtiff/libtiff/tif_lzw.c
	frmts/gtiff/libtiff/tif_next.c
	frmts/gtiff/libtiff/tif_open.c
	frmts/gtiff/libtiff/tif_packbits.c
	frmts/gtiff/libtiff/tif_pixarlog.c
	frmts/gtiff/libtiff/tif_predict.c
	frmts/gtiff/libtiff/tif_print.c
	frmts/gtiff/libtiff/tif_read.c
	frmts/gtiff/libtiff/tif_swab.c
	frmts/gtiff/libtiff/tif_strip.c
	frmts/gtiff/libtiff/tif_thunder.c
	frmts/gtiff/libtiff/tif_tile.c
	frmts/gtiff/libtiff/tif_vsi.c
	frmts/gtiff/libtiff/tif_version.c
	frmts/gtiff/libtiff/tif_warning.c
	frmts/gtiff/libtiff/tif_write.c
	frmts/gtiff/libtiff/tif_zip.c

	frmts/gtiff/geotiff.cpp 	
        frmts/gtiff/gt_wkt_srs.cpp 	
        frmts/gtiff/gt_overview.cpp
	frmts/gtiff/tif_ovrcache.c 	
        frmts/gtiff/tif_overview.c 	
        frmts/gtiff/tif_float.c 	
        frmts/gtiff/tifvsi.cpp


)

FILE(GLOB GDALfrmts_SRCS "frmts/*.cpp" )

SET(GDALfrmtsSubDirs_SRCS)

SET( OTB_frmtsSubDirs

aaigrid
aigrid
airsar
bmp
bsb
ceos
ceos2
#dods
dted
#ecw
elas
envisat
fast
fit
#fits
#gdb
gif/libungif
gif
#grass
gxf
#hdf4
#hdf5

hfa/addtiffo_src
hfa

ilwis
#iso8211
jdem
#jp2kak
jpeg/libjpeg
jpeg
#jpeg2000
l1b
mem
#mrsid
#msg
#msgn
#netcdf
nitf
#ogdi
pcidsk
#pcraster
#pgchip
png/libpng
png
raw
rik
rmf
rs2
#sdts
usgsdem
vrt
#xpm
zlib
)

# Parcours repertoires
FOREACH(frmtsSubDirs ${OTB_frmtsSubDirs} )

        # Pour chaque répertoire, compile les fichiers sources *.cpp
        SET( REP_OK 1)
        SET( LOCALfrmtsSubDirs_SRCS )
        FILE(GLOB LOCALfrmtsSubDirsCpp_SRCS "frmts/${frmtsSubDirs}/*.cpp" )
        FILE(GLOB LOCALfrmtsSubDirsC_SRCS "frmts/${frmtsSubDirs}/*.c" )
        SET (LOCALfrmtsSubDirs_SRCS ${LOCALfrmtsSubDirsCpp_SRCS} ${LOCALfrmtsSubDirsC_SRCS} )
        SET( GDALfrmtsSubDirs_SRCS ${GDALfrmtsSubDirs_SRCS} ${LOCALfrmtsSubDirs_SRCS} )
#        FOREACH(file ${LOCALfrmtsSubDirs_SRCS} )
#                SET(RESULT)
#                TRY_COMPILE(RESULT
#                        ${GDAL_BINARY_DIR}/CMakeTmp
#                        ${file}
#                        )
#                IF(RESULT)
#                        MESSAGE(STATUS "Compile ${file} -- yes")
#                ELSE(RESULT)
#                        MESSAGE(STATUS "Compile ${file} -- no")
#                        SET( REP_OK 0)
#                ENDIF(RESULT)
#        ENDFOREACH(file)
#        
#        IF( "${REP_OK}" GREATER 0)
#                SET( GDALfrmtsSubDirs_SRCS ${GDALfrmtsSubDirs_SRCS} ${LOCALfrmtsSubDirs_SRCS} )
#                MESSAGE(STATUS "Compile directory ${frmtsSubDirs} -- yes")
#        ELSE ( "${REP_OK}" GREATER 0 )
#                MESSAGE(STATUS "Compile directory ${frmtsSubDirs} -- no")
#        ENDIF( "${REP_OK}" GREATER 0 )

ENDFOREACH(frmtsSubDirs)


FILE(GLOB GDALgcore_SRCS "gcore/*.cpp" )
#FILE(GLOB GDALport_SRCS "port/*.cpp" )

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
        port/xmlreformat.cpp
)

FILE(GLOB GDALalg_SRCS "alg/*.cpp" )
SET(GDALpymod_SRCS)
#FILE(GLOB GDALpymod_SRCS "pymod/*.c" )

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
	ogr/swq.c
	ogr/ogr_srs_validate.cpp
	ogr/ogr_srs_xml.cpp
	ogr/ograssemblepolygon.cpp
	ogr/ogr2gmlgeometry.cpp
	ogr/gml2ogrgeometry.cpp
	)

# INCLUDE_DIRECTORIES(
#   .
#   ${OTB_SOURCE_DIR}
#   ${OTB_SOURCE_DIR}/Utilities
#   ${OTB_SOURCE_DIR}/Utilities/GDAL
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/raw
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff/libtiff
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff/libgeotiff
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/msg
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/msg/PublicDecompWT/COMP/WT/Inc
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/gcore
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/ogr
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/ogr/ogrsf_frmts
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/port
#   ${OTB_SOURCE_DIR}/Utilities/GDAL/alg
# )


ADD_LIBRARY(gdal ${GDALfrmts_SRCS} ${GDALgtiff_SRCS} ${GDALfrmtsSubDirs_SRCS} ${GDALgcore_SRCS} ${GDALalg_SRCS} ${GDALport_SRCS} ${GDALogr_SRCS} ${GDALpymod_SRCS})


INSTALL_TARGETS(/lib/otb gdal )
INSTALL_FILES(/include/otb/Utilities/GDAL "(\\.h)$")
INSTALL_FILES(/include/otb/Utilities/GDAL "(gcore/\\.h|\\.txx)$")

