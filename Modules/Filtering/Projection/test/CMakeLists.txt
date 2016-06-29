otb_module_test()

set(OTBProjectionTests
otbSensorModel.cxx
otbProjectionTestDriver.cxx
otbVectorDataProjectionFilterFromGeoToMap.cxx
otbOrthoRectificationFilter.cxx
otbGCPsToRPCSensorModelImageFilterNew.cxx
otbMapProjection.cxx
otbGenericRSResampleImageFilter.cxx
otbGeometriesProjectionFilter.cxx
otbGenericRSTransformGenericTest.cxx
otbLeastSquareAffineTransformEstimatorNew.cxx
otbVectorDataTransformFilter.cxx
otbRationalTransformToDisplacementFieldSource.cxx
otbImportGeoInformationImageFilterNew.cxx
otbVectorDataProjectionFilterFromMapToSensor.cxx
otbCompositeTransformNew.cxx
otbRationalTransform.cxx
otbGeometriesProjectionFilterFromMapToSensor.cxx
otbImageToEnvelopeVectorDataFilter.cxx
otbMapProjectionsNew.cxx
otbOrthoRectificationFilterNew.cxx
otbImageToGenericRSOutputParameters.cxx
otbGeometriesProjectionFilterFromMapToGeo.cxx
otbROIdataConversion.cxx
otbProjectionBaseNew.cxx
otbVectorDataProjectionFilterFromMapToGeo.cxx
otbPhysicalToRPCSensorModelImageFilter.cxx
otbGeometriesProjectionFilterFromGeoToMap.cxx
otbVectorDataProjectionFilterFromMapToImage.cxx
otbGenericRSTransformFromImage.cxx
otbCompositeTransform.cxx
otbLeastSquareAffineTransformEstimator.cxx
otbSpectralAngleDataNodeFeatureFunction.cxx
otbROIdataConversionNew.cxx
otbGCPsToRPCSensorModelImageFilterCheckRpcModel.cxx
otbGeographicalDistance.cxx
otbTileImageFilterRSTransformTest.cxx
otbGCPsToRPCSensorModelImageFilterAndOrtho.cxx
otbVectorDataIntoImageProjectionFilterTest.cxx
otbVectorDataExtractROIandProjection.cxx
otbGCPsToRPCSensorModelImageFilterWithoutDEM.cxx
otbGeometriesProjectionFilterFromMapToImage.cxx
otbGeometriesProjectionFilterFromMapToEPSG.cxx
otbVectorDataProjectionFilter.cxx
otbUtmMapProjection.cxx
otbVectorDataProjectionFilterNew.cxx
otbTileMapTransform.cxx
otbImportGeoInformationImageFilter.cxx
)


set(GEOMGCP
"pleiades/pleiades-1" #LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2} 
"wv2/wv2-1"           #LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF} 
"spot6/spot6-1"       #LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2}
"QB/qb-1"             #LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
"ikonos/ikonos-1"     #LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_nir_0000000.tif} 
"rapideye/rapideye-1" #LARGEINPUT{RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf}
"sentinel1/sentinel1-1" #LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
)

set(NEEDEDKW
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff" #Pleiades
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff" #World view 2
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff" #Spot 6
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff" #QB
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff" #Ikonos
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff" #Rapideye
"number_lines number_samples orbitList support_data.geom.bursts support_data.geom.gcp support_data.pulse_repetition_frequency support_data.radar_frequency support_data.range_sampling_rate support_data.slant_range_to_first_pixel" # Sentinel1
)

set(GEOTOL
  1 # Pleiades
  1 # Worldview2
  1 # Spot 6
  1 # QB
  1 # Ikonos
  1 # Rapideye
  5 # Sentinel1
  )

set( GEOM_TESTNB 0)
foreach( file ${GEOMGCP} )
  list(GET GEOMGCP   ${GEOM_TESTNB}  current_geomgcp   )
  list(GET NEEDEDKW   ${GEOM_TESTNB} current_neededkw   )
  list(GET GEOTOL     ${GEOM_TESTNB} current_geotol )
  math(EXPR GEOM_TESTNB "${GEOM_TESTNB} + 1")
  
  string(FIND ${current_geomgcp} "/" POS)
  math(EXPR POS "${POS} + 1")
  string(LENGTH ${current_geomgcp} LENGTH)
  math(EXPR SIZE "${LENGTH} - ${POS}")
  string(SUBSTRING ${current_geomgcp} ${POS} ${SIZE} TESTNAME)
  
  otb_add_test(NAME prTvSensorModel_${TESTNAME} COMMAND otbProjectionTestDriver
    otbSensorModel
    ${INPUTDATA}/${current_geomgcp}.geom
    ${INPUTDATA}/${current_geomgcp}.gcp
    ${TEMP}/${TESTNAME}.gcp # baseline (produced only if argv[7] == 1)
    ${current_neededkw}
    1 # imgTol
    ${current_geotol} # geoTol
    0 # create baseline (argv[7])
    1 # mode verbose
    1 # 0 : no keyword check  1 : check needed keywords
  )
endforeach()


#Faulty sensors
#set(GEOMGCP
#"radarsat2/gd-radarsat2-1" #LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif} 
#"terrasarx/gd-terrasarx-1" #LARGEINPUT{TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos} 
#"terrasarx/gd-terrasarx-1" #LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos} 
#"palsar/gd-palsar-1" #LARGEINPUT{PALSAR/200801280007/l1data/IMG-HH-ALPSRP037120700-H1.1__A} Ne reconnaît pas l'image
#"s1/gd-s1-A" #LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff} segfault
#)


#Other sensor : no keyword check
set(GEOMGCP
"ers2/ers2-1"       #LARGEINPUT{SAR_ERS2_SLCI_SCENE1/DAT_01.001}
"spot5/spot5-1"     #LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
"geoeye1/geoeye1-1" #LARGEINPUT{GEOEYE/MARCILLOLES/po_350134_bgrn_0000001.tif}
)


set( GEOM_TESTNB 0)
foreach( file ${GEOMGCP} )
  list(GET GEOMGCP   ${GEOM_TESTNB} current_geomgcp   )
  math(EXPR GEOM_TESTNB "${GEOM_TESTNB} + 1")
  
  string(FIND ${current_geomgcp} "/" POS)
  math(EXPR POS "${POS} + 1")
  string(LENGTH ${current_geomgcp} LENGTH)
  math(EXPR SIZE "${LENGTH} - ${POS}")
  string(SUBSTRING ${current_geomgcp} ${POS} ${SIZE} TESTNAME)
  
  otb_add_test(NAME prTvSensorModel_${TESTNAME} COMMAND otbProjectionTestDriver
    #BASELINE_FILES
    otbSensorModel
    ${INPUTDATA}/${current_geomgcp}.geom
    ${INPUTDATA}/${current_geomgcp}.gcp
    ${TEMP}/${TESTNAME}.gcp # baseline (produced only if argv[7] == 1)
    "Foo"
    1 # imgTol
    1 # geoTol
    0 # create baseline (argv[7])
    1 # mode verbose
    0 # 0 : no keyword check  1 : check needed keywords
  )
endforeach()


add_executable(otbProjectionTestDriver ${OTBProjectionTests})
target_link_libraries(otbProjectionTestDriver ${OTBProjection-Test_LIBRARIES})
otb_module_target_label(otbProjectionTestDriver)

# Tests Declaration

otb_add_test(NAME prTuVectorDataProjectionFilterFromGeoToMap COMMAND otbProjectionTestDriver
  otbVectorDataProjectionFilterFromGeoToMap
  ${INPUTDATA}/ToulousePointsWGS.sqlite
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/prTvVectorDataProjectionFilterFromGeoToMap.shp
  )


#------------------------------------------------------------------------
#-------------------- OrthoRectification tests --------------------------
#------------------------------------------------------------------------
set(IMG_TEST_ORTHO
PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2 
WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2
QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
IKONOS/BLOSSEVILLE/po_2619900_nir_0000000.tif 
RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf
)

set(IMG_TYPE
"pleiades-1"
"wv2-1" 
"spot6-1"
"qb-1"
"ikonos-1"     
"rapideye-1"
)

set(GEOMGCP
"pleiades/pleiades-1" #LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2} 
"wv2/wv2-1"           #LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF} 
"spot6/spot6-1"       #LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2}
"QB/qb-1"             #LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
"ikonos/ikonos-1"     #LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_nir_0000000.tif} 
"rapideye/rapideye-1" #LARGEINPUT{RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf}
)

set(ORIGINSX
370000
291835
425000
375000
340000
430000
)

set(ORIGINSY
4830000
4640740
4580000
4823000
5524000
6190000
)

set(UTMZONES
31
33
31
31
31
55
)

set(HEMIS
N
N
N
N
N
S
)

set(MODE
"noDEM"
"DEMSRTM"
"DEMGTIFF"
)

set(DEMDIR
"noDEM"
"${INPUTDATA}/DEM/srtm_directory"
"${INPUTDATA}/DEM/tif_directory"
)

set( IMGNB 0)
foreach(current_img ${IMG_TEST_ORTHO})

	list(GET IMG_TYPE   ${IMGNB} current_imgtype  )
	list(GET GEOMGCP   ${IMGNB} current_geomgcp   )
	list(GET ORIGINSX   ${IMGNB} current_originx  )
	list(GET ORIGINSY   ${IMGNB} current_originy  )
	list(GET UTMZONES   ${IMGNB} current_zone     )
	list(GET HEMIS   ${IMGNB} current_hemis       )
	math(EXPR IMGNB "${IMGNB} + 1")

	set( MODENB 0)
	foreach( current_mode ${MODE} )
	  list(GET DEMDIR   ${MODENB} current_demdir   )
	  math(EXPR MODENB "${MODENB} + 1")
	  
	  otb_add_test(NAME prTvOrthoRectification_${current_imgtype}_${current_mode} COMMAND otbProjectionTestDriver
		  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectification_${current_imgtype}_${current_mode}.tif
		  ${TEMP}/prTvOrthoRectification_${current_imgtype}_${current_mode}.tif
		  otbOrthoRectificationFilter
		  LARGEINPUT{${current_img}?&geom=${INPUTDATA}/${current_geomgcp}.geom}
		  ${TEMP}/prTvOrthoRectification_${current_imgtype}_${current_mode}.tif
		  ${current_originx}
		  ${current_originy}
		  500
		  500
		  0.5
		  -0.5
		  ${current_zone}
		  ${current_hemis}
		  4 # Spacing of the displacement field
		  ${MODENB}
		  ${current_demdir}
		)
			  
	endforeach()
endforeach()
#------------------------------------------------------------------------
#----------------- OrthoRectification tests (end) -----------------------
#------------------------------------------------------------------------


otb_add_test(NAME prTuGCPsToRPCSensorModelImageFilterNew COMMAND otbProjectionTestDriver
  otbGCPsToRPCSensorModelImageFilterNew)

otb_add_test(NAME prTvMapProjection COMMAND otbProjectionTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvMapProjection.txt
  ${TEMP}/prTvMapProjection.txt
  otbMapProjection
  ${TEMP}/prTvMapProjection.txt
  )

otb_add_test(NAME prTuotbGenericRSResampleImageFilterNew COMMAND otbProjectionTestDriver
  otbGenericRSResampleImageFilterNew
  )

otb_add_test(NAME prTvotbGenericRSResampleImageFilter COMMAND otbProjectionTestDriver
  --compare-image ${EPSILON_4}
  ${BASELINE}/prTvotbGenericRSResampleImageFilterOutput.tif
  ${TEMP}/prTvotbGenericRSResampleImageFilterOutput.tif
  otbGenericRSResampleImageFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  1000
  15
  0
  1
  ${TEMP}/prTvotbGenericRSResampleImageFilterOutput.tif
  )

otb_add_test(NAME prTvGeometriesProjectionFilterLines COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterLines.shp
  ${TEMP}/prTvGeometriesProjectionFilterLines.shp
  otbGeometriesProjectionFilter
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/prTvGeometriesProjectionFilterLines.shp
  )

otb_add_test(NAME prTvGeometriesProjectionFilterPoints COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterPoints.shp
  ${TEMP}/prTvGeometriesProjectionFilterPoints.shp
  otbGeometriesProjectionFilter
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/prTvGeometriesProjectionFilterPoints.shp
  )

otb_add_test(NAME prTvGeometriesProjectionFilterPolygons COMMAND otbProjectionTestDriver
  --compare-ogr  ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterPolygons.shp
  ${TEMP}/prTvGeometriesProjectionFilterPolygons.shp
  otbGeometriesProjectionFilter
  ${INPUTDATA}/Capitole-Shadows.shp
  ${TEMP}/prTvGeometriesProjectionFilterPolygons.shp
  )

otb_add_test(NAME prTvGenericRSTransformGenericTestEPSG4326toEPSG32631 COMMAND otbProjectionTestDriver
  otbGenericRSTransformGenericTest
  1.3561728980 43.4876035537 367077.72312001337 4816277.086791252 EPSG 4326 EPSG 32631 GEO 1e-3 PHYSICAL 1e-3 NOELEV)

otb_add_test(NAME prTvGenericRSTransformGenericTestWGS84toEPSG3857 COMMAND otbProjectionTestDriver
  otbGenericRSTransformGenericTest
  1.48353 43.55968 165145.804154 5397552.58726 EPSG 4326 EPSG 3857 GEO 1e-3 PHYSICAL 1e-3 NOELEV)

otb_add_test(NAME prTvGenericRSTransformGenericTestEPSG4326toEPSG4326 COMMAND otbProjectionTestDriver
  otbGenericRSTransformGenericTest
  1.35617289802566 43.4876035537 1.35617289802566 43.4876035537 EPSG 4326 EPSG 4326 GEO 1e-6 GEO 1e-6 NOELEV)

otb_add_test(NAME prTuLeastSquareAffineTransformEstimatorNew COMMAND otbProjectionTestDriver
  otbLeastSquareAffineTransformEstimatorNew)

otb_add_test(NAME prTuVectorDataTransformFilterNew COMMAND otbProjectionTestDriver
  otbVectorDataTransformFilterNew)

otb_add_test(NAME prTvVectorDataTransformFilter COMMAND otbProjectionTestDriver
  --compare-ogr ${EPSILON_4}
  ${BASELINE_FILES}/prTvVectorDataTransformFilter_Translation.shp
  ${TEMP}/prTvVectorDataTransformFilter_Translation.shp
  otbVectorDataTransformFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/prTvVectorDataTransformFilter_Translation.shp
  )

otb_add_test(NAME prTvRationalTransformToDisplacementFieldSourceTest COMMAND otbProjectionTestDriver
  --compare-image ${EPSILON_9}  ${BASELINE}/prTvRationalTransformToDisplacementFieldSourceTest.hdr
  ${TEMP}/prTvRationalTransformToDisplacementFieldSourceTest.hdr
  otbRationalTransformToDisplacementFieldSourceTest
  ${TEMP}/prTvRationalTransformToDisplacementFieldSourceTest.hdr
  )

otb_add_test(NAME bfTuImportGeoInformationImageFilterNew COMMAND otbProjectionTestDriver
  otbImportGeoInformationImageFilterNew)


otb_add_test(NAME prTvVectorDataProjectionFilterFromMapToSensor COMMAND otbProjectionTestDriver
  --compare-ogr  ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToSensor.shp
  ${TEMP}/prTvVectorDataProjectionFilterFromMapToSensor.shp
  otbVectorDataProjectionFilterFromMapToSensor
  ${INPUTDATA}/ToulousePoints-examples.shp
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/prTvVectorDataProjectionFilterFromMapToSensor.shp
  )

otb_add_test(NAME prTuCompositeTransformNew COMMAND otbProjectionTestDriver  otbCompositeTransformNew )

otb_add_test(NAME prTvRationalTransform COMMAND otbProjectionTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/otbRationalTransformOutput.txt
  ${TEMP}/otbRationalTransformOutput.txt
  otbRationalTransform
  ${TEMP}/otbRationalTransformOutput.txt
  0 0
  1 1
  10 10
  -10 -10
  )

otb_add_test(NAME prTuRationalTransformNew COMMAND otbProjectionTestDriver
  otbRationalTransformNew)

otb_add_test(NAME prTvGeometriesProjectionFilterFromMapToSensor COMMAND otbProjectionTestDriver
  --compare-ogr  ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToSensor.shp
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToSensor.shp
  otbGeometriesProjectionFilterFromMapToSensor
  ${INPUTDATA}/ToulousePoints-examples.shp
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToSensor.shp
  )

otb_add_test(NAME prTvImageToEnvelopeVectorDataFilter COMMAND otbProjectionTestDriver
  --compare-ogr ${EPSILON_6}
  ${BASELINE_FILES}/prTvImageToEnvelopeVectorDataFilterOutput.kml
  ${TEMP}/prTvImageToEnvelopeVectorDataFilterOutput.kml
  otbImageToEnvelopeVectorDataFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/prTvImageToEnvelopeVectorDataFilterOutput.kml
  )

otb_add_test(NAME prTuImageToEnvelopeVectorDataFilterNew COMMAND otbProjectionTestDriver
  otbImageToEnvelopeVectorDataFilterNew)

otb_add_test(NAME prTuMapProjectionsNew COMMAND otbProjectionTestDriver  otbMapProjectionsNew )

otb_add_test(NAME prTvMapProjectionsTest COMMAND otbProjectionTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvMapProjectionsTest.txt
  ${TEMP}/prTvMapProjectionsTest.txt
  otbMapProjectionsTest
  ${TEMP}/prTvMapProjectionsTest.txt
  )


otb_add_test(NAME prTuOrthoRectificationFilterNew COMMAND otbProjectionTestDriver
  otbOrthoRectificationFilterNew )

otb_add_test(NAME prTvImageToGenericRSOutputParameters COMMAND otbProjectionTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/prTvImageToGenericRSOutputParametersOutput.txt
  ${TEMP}/prTvImageToGenericRSOutputParametersOutput.txt
  otbImageToGenericRSOutputParameters
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/prTvImageToGenericRSOutputParametersOutput.txt
  )

otb_add_test(NAME prTuImageToGenericRSOutputParametersNew COMMAND otbProjectionTestDriver
  otbImageToGenericRSOutputParametersNew)

otb_add_test(NAME prTvGeometriesProjectionFilterFromMapToGeo COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.kml
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToGeo.kml
  otbGeometriesProjectionFilterFromMapToGeo
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToGeo.kml
  )

otb_add_test(NAME leTvROIdataConversion COMMAND otbProjectionTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvROIdataConversion.txt
  ${TEMP}/leTvROIdataConversion.txt
  otbROIdataConversion
  ${INPUTDATA}/ImageLineDir.bsq.hdr
  ${INPUTDATA}/ImageLine.bsq.hdr
  ${TEMP}/leTvROIdataConversion.txt
  )

otb_add_test(NAME prTuProjectionBaseNew COMMAND otbProjectionTestDriver  otbProjectionBaseNew )

otb_add_test(NAME prTvVectorDataProjectionFilterFromMapToGeo COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.kml
  ${TEMP}/prTvVectorDataProjectionFilterFromMapToGeo.kml
  otbVectorDataProjectionFilterFromMapToGeo
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/prTvVectorDataProjectionFilterFromMapToGeo.kml
  )

otb_add_test(NAME prTvPhysicalToRPCSensorModelImageFilter COMMAND otbProjectionTestDriver
  otbPhysicalToRPCSensorModelImageFilter
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif
  15
  ${TEMP}/prTvotbPhysicalToRPCSensorModelImageFilter.tif
  )

otb_add_test(NAME prTuPhysicalToRPCSensorModelImageFilterNew COMMAND otbProjectionTestDriver
  otbPhysicalToRPCSensorModelImageFilter
  )

otb_add_test(NAME prTuGeometriesProjectionFilterFromGeoToMap COMMAND otbProjectionTestDriver
  otbGeometriesProjectionFilterFromGeoToMap
  ${INPUTDATA}/ToulousePointsWGS.sqlite
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/prTvGeometriesProjectionFilterFromGeoToMap.shp
  )

otb_add_test(NAME prTvVectorDataProjectionFilterFromMapToImage COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToImage.shp
  ${TEMP}/prTvVectorDataProjectionFilterFromMapToImage.shp
  otbVectorDataProjectionFilterFromMapToImage
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/prTvVectorDataProjectionFilterFromMapToImage.shp
  )

otb_add_test(NAME prTvGenericRSTransformPleiadesToulouseGeodesicPointChecking COMMAND otbProjectionTestDriver
  otbGenericRSTransformImageAndMNTToWGS84ConversionChecking
  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2}
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20000 28010 # IGN reference point based inside CNES (site 31555A point c)
  1.48353 43.55968 # IGN reference point lon/lat
  192.205 # IGN reference point elevation (prec < 10 cm)
  37 # Tolerance in pixels (performances in february 2012)
  20 # Tolerance in meters (performances in february 2012)
  )

otb_add_test(NAME prTvGenericRSTransformPleiadesToulouseGeodesicPointChecking_PRE COMMAND otbProjectionTestDriver
  otbGenericRSTransformImageAndMNTToWGS84ConversionChecking
  LARGEINPUT{PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF}
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  6933 6472 # IGN reference point based inside CNES (site 31555A point c)
  1.48353 43.55968 # IGN reference point lon/lat
  192.205 # IGN reference point elevation (prec < 10 cm)
  35 # Tolerance in pixels (performances in february 2012)
  20 # Tolerance in meters (performances in february 2012)
  )

otb_add_test(NAME prTvGenericRSTransformFromImage COMMAND otbProjectionTestDriver
  otbGenericRSTransformFromImage
  ${INPUTDATA}/WithoutProjRefWithKeywordlist.tif)

otb_add_test(NAME prTvGenericRSTransformQuickbirdToulouseGeodesicPointChecking COMMAND otbProjectionTestDriver
  otbGenericRSTransformImageAndMNTToWGS84ConversionChecking
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  16271 15647 # IGN reference point based inside CNES (site 31555A point c)
  1.48353 43.55968 # IGN reference point lon/lat
  192.205 # IGN reference point elevation (prec < 10 cm)
  24 # Tolerance in pixels according to DigitalGlobe specification
  23 # Tolerance in meters according to DigitalGlobe specification
  )




otb_add_test(NAME prTvCompositeTransform COMMAND otbProjectionTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvCompositeTransform.txt
  ${TEMP}/prTvCompositeTransform.txt
  otbCompositeTransform
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/prTvCompositeTransform.txt
  )

otb_add_test(NAME prTvLeastSquareAffineTransformEstimator COMMAND otbProjectionTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/otbLeastSquareAffineTransformEstimatorOutput.txt
  ${TEMP}/otbLeastSquareAffineTransformEstimatorOutput.txt
  otbLeastSquareAffineTransformEstimator
  ${TEMP}/otbLeastSquareAffineTransformEstimatorOutput.txt
  840 394 783 7907 9925 11943
  798 911 197 5831 7738 9645
  335 768 277 5356 6737 8118
  553 477 628 6867 8526 10185
  364 513 952 9090 10920 12750
  916 635 717 8485 10754 13023
  141 606 16 2687 3451 4215
  242 137 804 6428 7612 8796
  156 400 129 2669 3355 4041
  108 998 218 5636 6961 8286
  512 839 612 8162 10126 12090
  296 637 524 6522 7980 9438
  493 972 292 6435 8193 9951
  771 526 769 8268 10335 12402
  400 891 283 5955 7530 9105
  352 807 919 10023 12102 14181
  )

otb_add_test(NAME bfTvSpectralAngleDataNodeFeatureFunction_Polygon COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Polygon.shp
  otbSpectralAngleDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Polygon.shp
  0 )

otb_add_test(NAME bfTuSpectralAngleDataNodeFeatureFunctionNew COMMAND otbProjectionTestDriver
  otbSpectralAngleDataNodeFeatureFunctionNew
  )

otb_add_test(NAME bfTvSpectralAngleDataNodeFeatureFunction_Line COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Line.shp
  otbSpectralAngleDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Line.shp
  0
  )

otb_add_test(NAME leTuROIdataConversionNew COMMAND otbProjectionTestDriver
  otbROIdataConversionNew)

otb_add_test(NAME prTvGCPsToRPCSensorModelImageCheckInputGcpPointsProjection_WithDEM COMMAND otbProjectionTestDriver
  otbGCPsToRPCSensorModelImageFilterCheckRpcModel
  -in ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  -dem ${INPUTDATA}/DEM/srtm_directory
  -gcp
  2000 2000 1.4181 43.6147 135.084
  2000 2050 1.41811 43.6135 134.185
  2000 2100 1.41812 43.6123 135.728
  2000 2150 1.41812 43.6111 140.437
  2000 2200 1.41813 43.6099 141.985
  2000 2250 1.41813 43.6087 138.257
  2000 2300 1.41814 43.6075 136.908
  2000 2350 1.41815 43.6062 129.787
  2000 2400 1.41815 43.605 126.295
  2000 2450 1.41816 43.6038 136.66
  2050 2000 1.41968 43.6147 135.682
  2050 2050 1.41969 43.6135 135
  2050 2100 1.4197 43.6123 134.7
  2050 2150 1.4197 43.6111 140.348
  2050 2200 1.41971 43.6099 143.342
  2050 2250 1.41972 43.6087 138.644
  2050 2300 1.41972 43.6075 137.985
  2050 2350 1.41973 43.6062 137.348
  2050 2400 1.41973 43.605 125.707
  2050 2450 1.41974 43.6038 130.963
  2100 2000 1.42127 43.6147 135.329
  2100 2050 1.42127 43.6135 137.984
  2100 2100 1.42128 43.6123 137.291
  2100 2150 1.42129 43.6111 138.949
  2100 2200 1.42129 43.6099 140.584
  2100 2250 1.4213 43.6087 142.13
  2100 2300 1.4213 43.6075 138.452
  2100 2350 1.42131 43.6063 137.792
  2100 2400 1.42132 43.605 136.434
  2100 2450 1.42132 43.6038 130.481
  2150 2000 1.42285 43.6147 135.332
  2150 2050 1.42286 43.6135 136.651
  2150 2100 1.42286 43.6123 136.629
  2150 2150 1.42287 43.6111 139.551
  2150 2200 1.42287 43.6099 138.204
  2150 2250 1.42288 43.6087 145.283
  2150 2300 1.42289 43.6075 137.505
  2150 2350 1.42289 43.6063 137.57
  2150 2400 1.4229 43.605 134.733
  2150 2450 1.4229 43.6038 131.314
  2200 2000 1.42443 43.6148 138.407
  2200 2050 1.42444 43.6135 139.594
  2200 2100 1.42444 43.6123 139.942
  2200 2150 1.42445 43.6111 140.982
  2200 2200 1.42446 43.6099 138.061
  2200 2250 1.42446 43.6087 143.539
  2200 2300 1.42447 43.6075 140.254
  2200 2350 1.42447 43.6063 136.862
  2200 2400 1.42448 43.605 139.257
  2200 2450 1.42449 43.6038 133.532
  2250 2000 1.42601 43.6148 140.76
  2250 2050 1.42602 43.6135 139.717
  2250 2100 1.42603 43.6123 139.331
  2250 2150 1.42603 43.6111 137.95
  2250 2200 1.42604 43.6099 136.37
  2250 2250 1.42604 43.6087 139.675
  2250 2300 1.42605 43.6075 142.744
  2250 2350 1.42606 43.6063 138.594
  2250 2400 1.42606 43.6051 142.685
  2250 2450 1.42607 43.6038 136.393
  2300 2000 1.4276 43.6148 141.979
  2300 2050 1.4276 43.6135 140.486
  2300 2100 1.42761 43.6123 142.465
  2300 2150 1.42761 43.6111 143.805
  2300 2200 1.42762 43.6099 140.161
  2300 2250 1.42762 43.6087 140.587
  2300 2300 1.42763 43.6075 142.85
  2300 2350 1.42764 43.6063 143.652
  2300 2400 1.42764 43.6051 144.587
  2300 2450 1.42765 43.6038 135.548
  2350 2000 1.42918 43.6148 139.286
  2350 2050 1.42918 43.6136 142.901
  2350 2100 1.42919 43.6123 143.92
  2350 2150 1.4292 43.6111 145.468
  2350 2200 1.4292 43.6099 142.063
  2350 2250 1.42921 43.6087 141.327
  2350 2300 1.42921 43.6075 140.21
  2350 2350 1.42922 43.6063 144.686
  2350 2400 1.42922 43.6051 142.948
  2350 2450 1.42923 43.6038 136.852
  2400 2000 1.43076 43.6148 142.428
  2400 2050 1.43077 43.6136 142.015
  2400 2100 1.43077 43.6123 141.832
  2400 2150 1.43078 43.6111 142.742
  2400 2200 1.43078 43.6099 142.89
  2400 2250 1.43079 43.6087 141.337
  2400 2300 1.43079 43.6075 140.138
  2400 2350 1.4308 43.6063 144.412
  2400 2400 1.43081 43.6051 143.224
  2400 2450 1.43081 43.6038 133.995
  2450 2000 1.43234 43.6148 142.162
  2450 2050 1.43235 43.6136 139.831
  2450 2100 1.43235 43.6123 140.736
  2450 2150 1.43236 43.6111 140.871
  2450 2200 1.43236 43.6099 142.21
  2450 2250 1.43237 43.6087 141.501
  2450 2300 1.43238 43.6075 138.911
  2450 2350 1.43238 43.6063 144.66
  2450 2400 1.43239 43.6051 144.932
  2450 2450 1.43239 43.6038 136.794
  -err 10 # Tolerance in meters, the first goal of this test
  )

otb_add_test(NAME prTvGCPsToRPCSensorModelImageCheckInputGcpPointsProjection_WithoutDEM COMMAND otbProjectionTestDriver
  otbGCPsToRPCSensorModelImageFilterCheckRpcModel
  -in ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  -gcp
  2000 2000 1.41809 43.6143 0
  2000 2050 1.4181 43.6131 0
  2000 2100 1.4181 43.6119 0
  2000 2150 1.41811 43.6107 0
  2000 2200 1.41812 43.6095 0
  2000 2250 1.41812 43.6083 0
  2000 2300 1.41813 43.6071 0
  2000 2350 1.41814 43.6058 0
  2000 2400 1.41814 43.6046 0
  2000 2450 1.41815 43.6034 0
  2050 2000 1.41967 43.6143 0
  2050 2050 1.41968 43.6131 0
  2050 2100 1.41969 43.6119 0
  2050 2150 1.41969 43.6107 0
  2050 2200 1.4197 43.6095 0
  2050 2250 1.41971 43.6083 0
  2050 2300 1.41971 43.6071 0
  2050 2350 1.41972 43.6059 0
  2050 2400 1.41973 43.6046 0
  2050 2450 1.41973 43.6034 0
  2100 2000 1.42126 43.6143 0
  2100 2050 1.42126 43.6131 0
  2100 2100 1.42127 43.6119 0
  2100 2150 1.42128 43.6107 0
  2100 2200 1.42128 43.6095 0
  2100 2250 1.42129 43.6083 0
  2100 2300 1.4213 43.6071 0
  2100 2350 1.4213 43.6059 0
  2100 2400 1.42131 43.6046 0
  2100 2450 1.42132 43.6034 0
  2150 2000 1.42284 43.6143 0
  2150 2050 1.42285 43.6131 0
  2150 2100 1.42285 43.6119 0
  2150 2150 1.42286 43.6107 0
  2150 2200 1.42286 43.6095 0
  2150 2250 1.42287 43.6083 0
  2150 2300 1.42288 43.6071 0
  2150 2350 1.42288 43.6059 0
  2150 2400 1.42289 43.6046 0
  2150 2450 1.4229 43.6034 0
  2200 2000 1.42442 43.6144 0
  2200 2050 1.42443 43.6131 0
  2200 2100 1.42443 43.6119 0
  2200 2150 1.42444 43.6107 0
  2200 2200 1.42445 43.6095 0
  2200 2250 1.42445 43.6083 0
  2200 2300 1.42446 43.6071 0
  2200 2350 1.42447 43.6059 0
  2200 2400 1.42447 43.6046 0
  2200 2450 1.42448 43.6034 0
  2250 2000 1.42601 43.6144 0
  2250 2050 1.42601 43.6131 0
  2250 2100 1.42602 43.6119 0
  2250 2150 1.42602 43.6107 0
  2250 2200 1.42603 43.6095 0
  2250 2250 1.42604 43.6083 0
  2250 2300 1.42604 43.6071 0
  2250 2350 1.42605 43.6059 0
  2250 2400 1.42605 43.6047 0
  2250 2450 1.42606 43.6034 0
  2300 2000 1.42759 43.6144 0
  2300 2050 1.42759 43.6131 0
  2300 2100 1.4276 43.6119 0
  2300 2150 1.42761 43.6107 0
  2300 2200 1.42761 43.6095 0
  2300 2250 1.42762 43.6083 0
  2300 2300 1.42762 43.6071 0
  2300 2350 1.42763 43.6059 0
  2300 2400 1.42764 43.6047 0
  2300 2450 1.42764 43.6034 0
  2350 2000 1.42917 43.6144 0
  2350 2050 1.42918 43.6131 0
  2350 2100 1.42918 43.6119 0
  2350 2150 1.42919 43.6107 0
  2350 2200 1.42919 43.6095 0
  2350 2250 1.4292 43.6083 0
  2350 2300 1.42921 43.6071 0
  2350 2350 1.42921 43.6059 0
  2350 2400 1.42922 43.6047 0
  2350 2450 1.42922 43.6034 0
  2400 2000 1.43075 43.6144 0
  2400 2050 1.43076 43.6131 0
  2400 2100 1.43076 43.6119 0
  2400 2150 1.43077 43.6107 0
  2400 2200 1.43078 43.6095 0
  2400 2250 1.43078 43.6083 0
  2400 2300 1.43079 43.6071 0
  2400 2350 1.43079 43.6059 0
  2400 2400 1.4308 43.6047 0
  2400 2450 1.43081 43.6034 0
  2450 2000 1.43234 43.6144 0
  2450 2050 1.43234 43.6132 0
  2450 2100 1.43235 43.6119 0
  2450 2150 1.43235 43.6107 0
  2450 2200 1.43236 43.6095 0
  2450 2250 1.43236 43.6083 0
  2450 2300 1.43237 43.6071 0
  2450 2350 1.43238 43.6059 0
  2450 2400 1.43238 43.6047 0
  2450 2450 1.43239 43.6034 0
  -err 10 # Tolerance in meters, the first goal of this test
  )

otb_add_test(NAME prTuGeographicalDistanceNew COMMAND otbProjectionTestDriver
  otbGeographicalDistanceNew)

otb_add_test(NAME prTvGeographicalDistance COMMAND otbProjectionTestDriver
  otbGeographicalDistance
  -1.84944 53.14722
  0.14056 52.20444
  170254.85084
  ${EPSILON_4}
  )

otb_add_test(NAME prTvTileImageFilterGenericRSTransformTest_TLSE_DIMAPv2 COMMAND otbProjectionTestDriver
  otbTileImageFilterRSTransformTest
  1 3
  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2}
  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2}
  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R3C1.JP2}
  )

otb_add_test(NAME prTvTileImageFilterGenericRSTransformTest_LASVEGAS_DIMAPv1 COMMAND otbProjectionTestDriver
  otbTileImageFilterRSTransformTest
  1 3
  LARGEINPUT{PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R1C1.JP2}
  LARGEINPUT{PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R2C1.JP2}
  LARGEINPUT{PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R3C1.JP2}
  )

otb_add_test(NAME prTvGCPsToRPCSensorModelImageFilterAndOrtho COMMAND otbProjectionTestDriver
  --compare-image ${EPSILON_4}
  ${BASELINE}/otbGCPsToRPCSensorModelImageFilterAndOrthoOutput.tif
  ${TEMP}/otbGCPsToRPCSensorModelImageFilterAndOrthoOutput.tif
  otbGCPsToRPCSensorModelImageFilterAndOrtho
  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  ${TEMP}/otbGCPsToRPCSensorModelImageFilterAndOrthoOutput.tif
  372332.21875
  4830255.50000
  507
  534
  2.5
  -2.5
  31
  N
  2000 2000 1.41809 43.6143 0
  2000 2050 1.4181 43.6131 0
  2000 2100 1.4181 43.6119 0
  2000 2150 1.41811 43.6107 0
  2000 2200 1.41812 43.6095 0
  2000 2250 1.41812 43.6083 0
  2000 2300 1.41813 43.6071 0
  2000 2350 1.41814 43.6058 0
  2000 2400 1.41814 43.6046 0
  2000 2450 1.41815 43.6034 0
  2050 2000 1.41967 43.6143 0
  2050 2050 1.41968 43.6131 0
  2050 2100 1.41969 43.6119 0
  2050 2150 1.41969 43.6107 0
  2050 2200 1.4197 43.6095 0
  2050 2250 1.41971 43.6083 0
  2050 2300 1.41971 43.6071 0
  2050 2350 1.41972 43.6059 0
  2050 2400 1.41973 43.6046 0
  2050 2450 1.41973 43.6034 0
  2100 2000 1.42126 43.6143 0
  2100 2050 1.42126 43.6131 0
  2100 2100 1.42127 43.6119 0
  2100 2150 1.42128 43.6107 0
  2100 2200 1.42128 43.6095 0
  2100 2250 1.42129 43.6083 0
  2100 2300 1.4213 43.6071 0
  2100 2350 1.4213 43.6059 0
  2100 2400 1.42131 43.6046 0
  2100 2450 1.42132 43.6034 0
  2150 2000 1.42284 43.6143 0
  2150 2050 1.42285 43.6131 0
  2150 2100 1.42285 43.6119 0
  2150 2150 1.42286 43.6107 0
  2150 2200 1.42286 43.6095 0
  2150 2250 1.42287 43.6083 0
  2150 2300 1.42288 43.6071 0
  2150 2350 1.42288 43.6059 0
  2150 2400 1.42289 43.6046 0
  2150 2450 1.4229 43.6034 0
  2200 2000 1.42442 43.6144 0
  2200 2050 1.42443 43.6131 0
  2200 2100 1.42443 43.6119 0
  2200 2150 1.42444 43.6107 0
  2200 2200 1.42445 43.6095 0
  2200 2250 1.42445 43.6083 0
  2200 2300 1.42446 43.6071 0
  2200 2350 1.42447 43.6059 0
  2200 2400 1.42447 43.6046 0
  2200 2450 1.42448 43.6034 0
  2250 2000 1.42601 43.6144 0
  2250 2050 1.42601 43.6131 0
  2250 2100 1.42602 43.6119 0
  2250 2150 1.42602 43.6107 0
  2250 2200 1.42603 43.6095 0
  2250 2250 1.42604 43.6083 0
  2250 2300 1.42604 43.6071 0
  2250 2350 1.42605 43.6059 0
  2250 2400 1.42605 43.6047 0
  2250 2450 1.42606 43.6034 0
  2300 2000 1.42759 43.6144 0
  2300 2050 1.42759 43.6131 0
  2300 2100 1.4276 43.6119 0
  2300 2150 1.42761 43.6107 0
  2300 2200 1.42761 43.6095 0
  2300 2250 1.42762 43.6083 0
  2300 2300 1.42762 43.6071 0
  2300 2350 1.42763 43.6059 0
  2300 2400 1.42764 43.6047 0
  2300 2450 1.42764 43.6034 0
  2350 2000 1.42917 43.6144 0
  2350 2050 1.42918 43.6131 0
  2350 2100 1.42918 43.6119 0
  2350 2150 1.42919 43.6107 0
  2350 2200 1.42919 43.6095 0
  2350 2250 1.4292 43.6083 0
  2350 2300 1.42921 43.6071 0
  2350 2350 1.42921 43.6059 0
  2350 2400 1.42922 43.6047 0
  2350 2450 1.42922 43.6034 0
  2400 2000 1.43075 43.6144 0
  2400 2050 1.43076 43.6131 0
  2400 2100 1.43076 43.6119 0
  2400 2150 1.43077 43.6107 0
  2400 2200 1.43078 43.6095 0
  2400 2250 1.43078 43.6083 0
  2400 2300 1.43079 43.6071 0
  2400 2350 1.43079 43.6059 0
  2400 2400 1.4308 43.6047 0
  2400 2450 1.43081 43.6034 0
  2450 2000 1.43234 43.6144 0
  2450 2050 1.43234 43.6132 0
  2450 2100 1.43235 43.6119 0
  2450 2150 1.43235 43.6107 0
  2450 2200 1.43236 43.6095 0
  2450 2250 1.43236 43.6083 0
  2450 2300 1.43237 43.6071 0
  2450 2350 1.43238 43.6059 0
  2450 2400 1.43238 43.6047 0
  2450 2450 1.43239 43.6034 0
  )

#otb_add_test(NAME prTvVectorDataExtractROIandProjection COMMAND otbProjectionTestDriver
  #otbVectorDataExtractROIandProjection
  #LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  #)

otb_add_test(NAME prTvGCPsToRPCSensorModelImageFilterWithoutDEM COMMAND otbProjectionTestDriver
  --compare-ascii ${EPSILON_4}
  ${BASELINE_FILES}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
  ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
  otbGCPsToRPCSensorModelImageFilterWithoutDEM
  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
  2000 2000 1.41809 43.6143 0
  2000 2050 1.4181 43.6131 0
  2000 2100 1.4181 43.6119 0
  2000 2150 1.41811 43.6107 0
  2000 2200 1.41812 43.6095 0
  2000 2250 1.41812 43.6083 0
  2000 2300 1.41813 43.6071 0
  2000 2350 1.41814 43.6058 0
  2000 2400 1.41814 43.6046 0
  2000 2450 1.41815 43.6034 0
  2050 2000 1.41967 43.6143 0
  2050 2050 1.41968 43.6131 0
  2050 2100 1.41969 43.6119 0
  2050 2150 1.41969 43.6107 0
  2050 2200 1.4197 43.6095 0
  2050 2250 1.41971 43.6083 0
  2050 2300 1.41971 43.6071 0
  2050 2350 1.41972 43.6059 0
  2050 2400 1.41973 43.6046 0
  2050 2450 1.41973 43.6034 0
  2100 2000 1.42126 43.6143 0
  2100 2050 1.42126 43.6131 0
  2100 2100 1.42127 43.6119 0
  2100 2150 1.42128 43.6107 0
  2100 2200 1.42128 43.6095 0
  2100 2250 1.42129 43.6083 0
  2100 2300 1.4213 43.6071 0
  2100 2350 1.4213 43.6059 0
  2100 2400 1.42131 43.6046 0
  2100 2450 1.42132 43.6034 0
  2150 2000 1.42284 43.6143 0
  2150 2050 1.42285 43.6131 0
  2150 2100 1.42285 43.6119 0
  2150 2150 1.42286 43.6107 0
  2150 2200 1.42286 43.6095 0
  2150 2250 1.42287 43.6083 0
  2150 2300 1.42288 43.6071 0
  2150 2350 1.42288 43.6059 0
  2150 2400 1.42289 43.6046 0
  2150 2450 1.4229 43.6034 0
  2200 2000 1.42442 43.6144 0
  2200 2050 1.42443 43.6131 0
  2200 2100 1.42443 43.6119 0
  2200 2150 1.42444 43.6107 0
  2200 2200 1.42445 43.6095 0
  2200 2250 1.42445 43.6083 0
  2200 2300 1.42446 43.6071 0
  2200 2350 1.42447 43.6059 0
  2200 2400 1.42447 43.6046 0
  2200 2450 1.42448 43.6034 0
  2250 2000 1.42601 43.6144 0
  2250 2050 1.42601 43.6131 0
  2250 2100 1.42602 43.6119 0
  2250 2150 1.42602 43.6107 0
  2250 2200 1.42603 43.6095 0
  2250 2250 1.42604 43.6083 0
  2250 2300 1.42604 43.6071 0
  2250 2350 1.42605 43.6059 0
  2250 2400 1.42605 43.6047 0
  2250 2450 1.42606 43.6034 0
  2300 2000 1.42759 43.6144 0
  2300 2050 1.42759 43.6131 0
  2300 2100 1.4276 43.6119 0
  2300 2150 1.42761 43.6107 0
  2300 2200 1.42761 43.6095 0
  2300 2250 1.42762 43.6083 0
  2300 2300 1.42762 43.6071 0
  2300 2350 1.42763 43.6059 0
  2300 2400 1.42764 43.6047 0
  2300 2450 1.42764 43.6034 0
  2350 2000 1.42917 43.6144 0
  2350 2050 1.42918 43.6131 0
  2350 2100 1.42918 43.6119 0
  2350 2150 1.42919 43.6107 0
  2350 2200 1.42919 43.6095 0
  2350 2250 1.4292 43.6083 0
  2350 2300 1.42921 43.6071 0
  2350 2350 1.42921 43.6059 0
  2350 2400 1.42922 43.6047 0
  2350 2450 1.42922 43.6034 0
  2400 2000 1.43075 43.6144 0
  2400 2050 1.43076 43.6131 0
  2400 2100 1.43076 43.6119 0
  2400 2150 1.43077 43.6107 0
  2400 2200 1.43078 43.6095 0
  2400 2250 1.43078 43.6083 0
  2400 2300 1.43079 43.6071 0
  2400 2350 1.43079 43.6059 0
  2400 2400 1.4308 43.6047 0
  2400 2450 1.43081 43.6034 0
  2450 2000 1.43234 43.6144 0
  2450 2050 1.43234 43.6132 0
  2450 2100 1.43235 43.6119 0
  2450 2150 1.43235 43.6107 0
  2450 2200 1.43236 43.6095 0
  2450 2250 1.43236 43.6083 0
  2450 2300 1.43237 43.6071 0
  2450 2350 1.43238 43.6059 0
  2450 2400 1.43238 43.6047 0
  2450 2450 1.43239 43.6034 0
  )

otb_add_test(NAME prTvGeometriesProjectionFilterFromMapToImage COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToImage.shp
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToImage.shp
  otbGeometriesProjectionFilterFromMapToImage
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToImage.shp
  )

otb_add_test(NAME prTvGeometriesProjectionFilterFromMapToEPSG_UTM COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterLines.shp
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_UTM.shp
  otbGeometriesProjectionFilterFromMapToEPSG
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_UTM.shp
  32631 )

otb_add_test(NAME prTvGeometriesProjectionFilterFromMapToEPSG_WGS84 COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.kml
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_WGS84.kml
  otbGeometriesProjectionFilterFromMapToEPSG
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_WGS84.kml
  4326 )

otb_add_test(NAME prTvVectorDataProjectionFilterPoints COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterPoints.shp
  ${TEMP}/prTvVectorDataProjectionFilterPoints.shp
  otbVectorDataProjectionFilter
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/prTvVectorDataProjectionFilterPoints.shp
  )

otb_add_test(NAME prTvVectorDataProjectionFilterLines COMMAND otbProjectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterLines.shp
  ${TEMP}/prTvVectorDataProjectionFilterLines.shp
  otbVectorDataProjectionFilter
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/prTvVectorDataProjectionFilterLines.shp
  )

otb_add_test(NAME prTvVectorDataProjectionFilterPolygons COMMAND otbProjectionTestDriver
  --compare-ogr  ${NOTOL}
  ${BASELINE_FILES}/prTvVectorDataProjectionFilterPolygons.shp
  ${TEMP}/prTvVectorDataProjectionFilterPolygons.shp
  otbVectorDataProjectionFilter
  ${INPUTDATA}/Capitole-Shadows.shp
  ${TEMP}/prTvVectorDataProjectionFilterPolygons.shp
  )

otb_add_test(NAME prTvUtmMapProjection COMMAND otbProjectionTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvUtmMapProjection.txt
  ${TEMP}/prTvUtmMapProjection.txt
  otbUtmMapProjection
  ${TEMP}/prTvUtmMapProjection.txt
  )

otb_add_test(NAME prTuVectorDataProjectionFilterNew COMMAND otbProjectionTestDriver  otbVectorDataProjectionFilterNew )

otb_add_test(NAME prTvTileMapTransform COMMAND otbProjectionTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvTileMapTransform.txt
  ${TEMP}/prTvTileMapTransform.txt
  otbTileMapTransform
  ${TEMP}/prTvTileMapTransform.txt
  )

otb_add_test(NAME bfTvImportGeoInformationImageFilter COMMAND otbProjectionTestDriver
  --compare-metadata ${NOTOL}
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/bfTvImportGeoInformationOutput.img
  otbImportGeoInformationImageFilter
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/bfTvImportGeoInformationOutput.img
  )


set(GenericConversionChecking_INPUTS
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  #LARGEINPUT{SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  LARGEINPUT{RADARSAT1/GOMA2/SCENE01/DAT_01.001}
  LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif}
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  #LARGEINPUT{GEOEYE/LES_ROCHES/po_350134_bgrn_0000000.tif
  #LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
  #LARGEINPUT{PALSAR/ALOS-H1_1__A-ORBIT__ALPSRP037120700.tif
  #LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
  LARGEINPUT{TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml}
  LARGEINPUT{ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1}
  #LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5
  #LARGEINPUT{MODIS/MOD13Q1/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf
  #LARGEINPUT{ONERA/spa3_0215_rad.dat
  #LARGEINPUT{RAPIDEYE/level3A/2008-12-25T005918_RE3_3A-NAC_752656_40091.tif
  )



#set(VectorDataIntoImage_FILES
    #LARGEINPUT{QB_EXTRACT/qb_extract_00_withgeom.tif}
    #LARGEINPUT{QB_EXTRACT/qb_extract_00_withgeom_ENVI.hdr}
    #LARGEINPUT{QB_EXTRACT/qb_extract_11_withgeom.tif}
    #LARGEINPUT{QB_EXTRACT/qb_extract_11_withgeom_ENVI.hdr}
    #LARGEINPUT{QB_EXTRACT/qb_utm.tif}
    #LARGEINPUT{QB_EXTRACT/qb_utm_withgeom.tif}
    #LARGEINPUT{QB_EXTRACT/qb_wgs84.tif}
    #LARGEINPUT{QB_EXTRACT/qb_wgs84_withgeom.tif}
    #LARGEINPUT{QB_EXTRACT/qb_wgs84_worldfile.tif}
    #)

  #set( VectorDataIntoImage_TESTNB 0)
  
  #foreach( file ${VectorDataIntoImage_FILES} )
    #math(EXPR VectorDataIntoImage_TESTNB "${VectorDataIntoImage_TESTNB} + 1")
    #otb_add_test(NAME prTvVectorDataIntoImageProjectionFilter_${VectorDataIntoImage_TESTNB} COMMAND otbProjectionTestDriver
      #otbVectorDataIntoImageProjectionFilterTest
      #${file}
      #${OTB_DATA_LARGEINPUT_ROOT}/VECTOR/MidiPyrenees/natural.shp
      #${INPUTDATA}/DEM/srtm_directory
      #3 )
  #endforeach()
  
  

  set(SENSOR_TYPES
    "QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF"
    "GEOEYE/LES_ROCHES/po_350134_bgrn_0000000.tif"
    "WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF"
    )
  
  foreach( file ${SENSOR_TYPES})
    # Get the sensor name
    set(sharp_regexp "([0-9A-Za-z_]*)[ ]*/[ ]*(.*)")
    string(REGEX REPLACE "${sharp_regexp}" "\\1" sensor_name "${file}")
    # Tests
    otb_add_test(NAME prTvotbGenericRSResampleImageFilterFromMap_${sensor_name} COMMAND otbProjectionTestDriver
      --compare-image ${EPSILON_4}
      ${BASELINE}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
      ${TEMP}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
      otbGenericRSResampleImageFilterFromMap
      LARGEINPUT{${file}}
      15
      0
      ${TEMP}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
      )
  endforeach()
