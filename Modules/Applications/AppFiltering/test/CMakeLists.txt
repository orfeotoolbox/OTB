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
                             
                             
otb_test_application(NAME  apTvDespeckleLee
                     APP  Despeckle
                     OPTIONS -in ${INPUTDATA}/GomaAvant.tif
               	             -out ${TEMP}/bfFiltreLee_05_05_12_app.tif
                             -filter lee
                             -filter.lee.rad 5
                             -filter.lee.nblooks 12
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/bfFiltreLee_05_05_12.tif
                             ${TEMP}/bfFiltreLee_05_05_12_app.tif)
                             
otb_test_application(NAME  apTvDespeckleFrost
                     APP  Despeckle
                     OPTIONS -in ${INPUTDATA}/GomaAvant.tif
               	             -out ${TEMP}/bfFiltreFrost_05_05_12_app.tif
                             -filter frost
                             -filter.frost.rad 5
                             -filter.frost.deramp 0.1
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/bfFiltreLee_05_05_12.tif
                             ${TEMP}/bfFiltreLee_05_05_12_app.tif)

otb_test_application(NAME  apTvDespeckleGammaMAP
                     APP  Despeckle
                     OPTIONS -in ${INPUTDATA}/GomaAvant.tif
               	             -out ${TEMP}/bfFiltreGammaMAP_05_05_12_app.tif
                             -filter gammamap
                             -filter.gammamap.rad 5
                             -filter.gammamap.nblooks 12
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/bfFiltreGammaMAP_05_05_12.tif
                             ${TEMP}/bfFiltreGammaMAP_05_05_12_app.tif)                      
                             
otb_test_application(NAME  apTvDespeckleKuan
                     APP  Despeckle
                     OPTIONS -in ${INPUTDATA}/GomaAvant.tif
               	             -out ${TEMP}/bfFiltreKuan_05_05_12_app.tif
                             -filter kuan
                             -filter.kuan.rad 5
                             -filter.kuan.nblooks 12
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/bfFiltreKuan_05_05_12.tif
                             ${TEMP}/bfFiltreKuan_05_05_12_app.tif)
