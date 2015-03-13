otb_module_test()

#----------- KmzExport TESTS ----------------
otb_test_application(NAME apTvUtKmzExport
                     APP KmzExport
                     OPTIONS -in   ${INPUTDATA}/qb_RoadExtract.img
                             -out ${TEMP}/apTvUtKmzExport.kmz
                             -logo ${INPUTDATA}/cnes.png
                             -legend ${INPUTDATA}/otb_big.png
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory)
set_property(TEST apTvUtKmzExport PROPERTY RESOURCE_LOCK kmz_tmp_dir)

otb_test_application(NAME apTvUtKmzExport1Band
                     APP KmzExport
                     OPTIONS #-in /media/otbnas/public/Data/Saterre/data/cumulLAI.tif
                             -in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                             -out ${TEMP}/apTvUtKmzExport1Band.kmz
                             -logo ${INPUTDATA}/cnes.png
                             -legend ${INPUTDATA}/otb_big.png
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -tilesize 50) # needed to see the bug mantis #773
set_property(TEST apTvUtKmzExport1Band PROPERTY RESOURCE_LOCK kmz_tmp_dir)
