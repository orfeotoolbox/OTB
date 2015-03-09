otb_module_test()
#----------- Despeckle TESTS ----------------

#----------- Smoothing TESTS ----------------
otb_test_application(NAME  apTvUtSmoothingTest_InXML
                     APP  Smoothing
                     OPTIONS -inxml ${INPUTDATA}/apTvUtSmoothingTest_OutXML.xml
               	             -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest_InXML.tif
                             -type mean
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest_InXML.tif)

otb_test_application(NAME  apTvUtSmoothingTest
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest.tif
                             -type mean
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest.tif)

otb_test_application(NAME  apTvUtSmoothingTest_OutXML
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest_OutXML.tif
                             -type mean
                             -outxml ${TEMP}/apTvUtSmoothingTest_OutXML.xml
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest_OutXML.tif)

