otb_module_test()
#----------- HomologousPointsExtraction TESTS ----------------
otb_test_application(NAME apTvHomologousPointsExtractionGeoBinsAnisotropic
                     APP  HomologousPointsExtraction
                     OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -algorithm surf
                             -mode geobins
                             -mode.geobins.binsize 100
                             -mode.geobins.binsizey 150
                             -mode.geobins.binstep 25
                             -mode.geobins.binstepy 10
                             -mode.geobins.margin 12
                             -mfilter 1
                             -precision 10
                             -2wgs84 1
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -elev.geoid ${INPUTDATA}/DEM/egm96.grd
                             -out ${TEMP}/apTvHomologousPointsExtractionGeoBinsAnisotropic.txt
                     VALID   --compare-ascii ${EPSILON_9}
                             ${BASELINE_FILES}/apTvHomologousPointsExtractionGeoBinsAnisotropic.txt
                             ${TEMP}/apTvHomologousPointsExtractionGeoBinsAnisotropic.txt)

otb_test_application(NAME apTvHomologousPointsExtractionFull
                     APP  HomologousPointsExtraction
                     OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -algorithm surf
                             -out ${TEMP}/apTvHomologousPointsExtractionFull.txt
                     VALID   --compare-ascii ${EPSILON_9}
                             ${BASELINE_FILES}/apTvHomologousPointsExtractionFull.txt
                             ${TEMP}/apTvHomologousPointsExtractionFull.txt)

otb_test_application(NAME apTvHomologousPointsExtractionGeoBins
                     APP  HomologousPointsExtraction
                     OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -algorithm surf
                             -mode geobins
                             -mode.geobins.binsize 100
                             -mode.geobins.binstep 25
                             -mode.geobins.margin 12
                             -mfilter 1
                             -precision 10
                             -2wgs84 1
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -elev.geoid ${INPUTDATA}/DEM/egm96.grd
                             -out ${TEMP}/apTvHomologousPointsExtractionGeoBins.txt
                     VALID   --compare-ascii ${EPSILON_9}
                             ${BASELINE_FILES}/apTvHomologousPointsExtractionGeoBins.txt
                             ${TEMP}/apTvHomologousPointsExtractionGeoBins.txt)

