#--- ConnectedComponentSegmentation ---#

OTB_TEST_APPLICATION(NAME  apTvCcConnectedComponentSegmentationMaskMuParserShp
                     APP  ConnectedComponentSegmentation
                     OPTIONS --in ${INPUTDATA}/ROI_QB_MUL_4.tif
                             --out ${TEMP}/apTvCcConnectedComponentSegmentationMaskMuParser.shp
                             --mask "((b1>80)*intensity>95)"
                             --expr "distance<10"
                             --minsize 15
                             --obia "SHAPE_Elongation>8"
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvCcConnectedComponentSegmentationMaskMuParser.shp
                             ${TEMP}/apTvCcConnectedComponentSegmentationMaskMuParser.shp)

OTB_TEST_APPLICATION(NAME  apTvCcConnectedComponentSegmentatioNoMask
                     APP  ConnectedComponentSegmentation
                     OPTIONS --in ${INPUTDATA}/ROI_QB_MUL_4.tif
                             --out ${TEMP}/apTvCcConnectedComponentSegmentationNoMask.shp
                             --expr "distance<10"
                             --minsize 15
                             --obia "SHAPE_Elongation>8"
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvCcConnectedComponentSegmentationNoMask.shp
                             ${TEMP}/apTvCcConnectedComponentSegmentationNoMask.shp)
