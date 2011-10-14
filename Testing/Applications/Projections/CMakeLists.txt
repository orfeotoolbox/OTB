
set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )

#--- Rescale ---#



IF(OTB_DATA_USE_LARGEINPUT)

add_test(NAME apTvUtORthorectificationTest_UTM
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OrthoRectification
                 --modulePath $<TARGET_FILE_DIR:otbapp_Rescale>
                 --in ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                 --out ${TEMP}/owTvOrthorectifTest_UTM.tif
                 --dem ${INPUTDATA}/DEM/srtm_directory/
                 --outputs.uservalues 0
                 --outputs.ulx  374100.8
                 --outputs.uly  4829184.8
                 --outputs.sizex 500
                 --outputs.sizey 500
                 #         220
                 --outputs.spacingx  0.5
                 --outputs.spacingy  -0.5
                 --map utm
                 --map.utm.zone 31
                 --map.utm.hem 1
                 --rpc.ncp 100
                 #4 # Spacing of the deformation field
                 )

#add_test(NAME apTvUtReadImageInfoSpot5
#         COMMAND otbTestDriver
#                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
#                 ReadImageInfo
#                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
#                 --in ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF )



ENDIF(OTB_DATA_USE_LARGEINPUT)


