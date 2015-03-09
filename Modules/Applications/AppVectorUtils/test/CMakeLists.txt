otb_module_test()
#----------- VectorDataTransform TESTS ----------------
otb_test_application(NAME apTvUtVectorDataTransform_Rotation
                     APP VectorDataTransform
                     OPTIONS -in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
                             -vd ${INPUTDATA}/ToulouseRoad-examples.shp
                             -out ${TEMP}/apTvUtVectorDataTransform_rotation.shp
                             -transform.ro 15
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvVectorDataTransformFilter_Rotation.shp
                             ${TEMP}/apTvUtVectorDataTransform_rotation.shp)

otb_test_application(NAME apTvUtVectorDataTransform_Translation_Rotation
                     APP VectorDataTransform
                     OPTIONS -in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
                             -vd ${INPUTDATA}/ToulouseRoad-examples.shp
                             -out ${TEMP}/apTvUtVectorDataTransform_Translation_rotation.shp
                             -transform.ro 15
                             -transform.tx  5
                             -transform.ty  6
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvVectorDataTransformFilter_Translation_rotation.shp
                             ${TEMP}/apTvUtVectorDataTransform_Translation_rotation.shp)


#----------- VectorDataExtractROI TESTS ----------------
otb_test_application(NAME apTvUtVectorDataExtractROI
                     APP VectorDataExtractROI
                     OPTIONS -io.in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -io.vd LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
                             -io.out ${TEMP}/apTvUtVectorDataExtractROIApplicationTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvUtVectorDataExtractROIApplicationTest.shp
                             ${TEMP}/apTvUtVectorDataExtractROIApplicationTest.shp
                     )


#----------- VectorDataSetField TESTS ----------------
otb_test_application(NAME apTvUtVectorDataSetField
                     APP VectorDataSetField
                     OPTIONS -in ${INPUTDATA}/ToulousePoints-examples.shp
                             -out ${TEMP}/apTvUtVectorDataSetFieldTest.shp
                             -fn MyField
                             -fv MyValue
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvUtVectorDataSetFieldTest.shp
                             ${TEMP}/apTvUtVectorDataSetFieldTest.shp
                     )


#----------- OSMDownloader TESTS ----------------
otb_test_application(NAME apTvUtOSMDownloader
                     APP OSMDownloader
                     OPTIONS -support LARGEINPUT{DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF}
                             -key highway
                             -out ${TEMP}/apTvUtOSMDownloader.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/otbOSMDownloaderOutput.shp
                             ${TEMP}/apTvUtOSMDownloader.shp)


#----------- ConcatenateVectorData TESTS ----------------
otb_test_application(NAME apTvUtConcatenateVectorData
                     APP ConcatenateVectorData
                     OPTIONS -vd   ${INPUTDATA}/waterways.shp
                                    ${INPUTDATA}/france_coastline.shp
                             -out ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvConcatenateVectorData.shp
                             ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp)

