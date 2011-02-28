
ADD_EXECUTABLE(otbAvirisBandSelection otbAvirisBandSelection.cxx)
TARGET_LINK_LIBRARIES(otbAvirisBandSelection OTBCommon OTBIO OTBBasicFilters )

ADD_EXECUTABLE(otbEndmembersSubset otbEndmembersSubset.cxx)
TARGET_LINK_LIBRARIES(otbEndmembersSubset OTBCommon OTBIO OTBBasicFilters )

ADD_EXECUTABLE(otbHyperspectralCompare otbHyperspectralCompare.cxx)
TARGET_LINK_LIBRARIES(otbHyperspectralCompare OTBCommon OTBIO OTBBasicFilters )

ADD_EXECUTABLE(otbHyperspectralRecompose otbHyperspectralRecompose.cxx)
TARGET_LINK_LIBRARIES(otbHyperspectralRecompose OTBCommon OTBIO OTBBasicFilters )

ADD_EXECUTABLE(otbHyperspectralUnmixing otbHyperspectralUnmixing.cxx)
TARGET_LINK_LIBRARIES(otbHyperspectralUnmixing OTBCommon OTBIO OTBBasicFilters )




SET(TESTEXE_DIR ${Hyperspectral_BINARY_DIR}/bin)
SET(TEMP ${Hyperspectral_BINARY_DIR}/Testing/Temporary)
SET(DATA ${Hyperspectral_SOURCE_DIR}/Data)

#  parser->AddInputImage(); 
#  parser->AddOption( "--DimReduceMethod", "Dimensionnality reduction method (NONE,PCA,MCF)", "-dr", 1, false );
#  parser->AddOption( "--NumEndmembers", "Number of endmembers (if not given or 0, will be estimated with DimensionalityEstimationMethod)", "-ne", 1, false );
#  parser->AddOption( "--DimensionalityEstimationMethod", "Dimensionality estimation method (ELM)", "-de", 1, false );
#  parser->AddOption( "--EndmembersEstimationMethod", "Endmembers estimation method (VCA)", "-ee", 1, false );
#  parser->AddOption( "--InputEndmembers", "Input endmembers image (must have NumComponents components or fit the input image nb of components)", "-ie", 1, false );
#  parser->AddOption( "--UnmixingAlgorithm", "Unmixing algorithm (NONE, UCLS, ISRA)", "-ua", 1, false );
#  parser->AddOption( "--OutputEndmembers", "Output estimated endmembers image", "-oe", 1, false );
#  parser->AddOutputImage();

ADD_TEST(hyTvUnmixApplication
         ${TESTEXE_DIR}/otbHyperspectralUnmixing
         otbHyperspectralUnmixing
         -in ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         
         8)

         