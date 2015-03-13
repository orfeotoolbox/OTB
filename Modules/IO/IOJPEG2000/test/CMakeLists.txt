otb_module_test()

set(OTBIOJPEG2000Tests
otbIOJPEG2000TestDriver.cxx
otbJPEG2000ImageIOTestCanWrite.cxx
otbJPEG2000ImageIOTestCanRead.cxx
otbGenerateClassicalQLWithJPEG2000.cxx
)

add_executable(otbIOJPEG2000TestDriver ${OTBIOJPEG2000Tests})
target_link_libraries(otbIOJPEG2000TestDriver ${OTBIOJPEG2000-Test_LIBRARIES})
otb_module_target_label(otbIOJPEG2000TestDriver)

# Tests Declaration

otb_add_test(NAME ioTuJP2ImageIOCanRead COMMAND otbIOJPEG2000TestDriver
  otbJPEG2000ImageIOTestCanRead
  LARGEINPUT{JPEG2000/QB_multicomp.jpx}
  )

otb_add_test(NAME ioTuJ2KImageIOCanRead COMMAND otbIOJPEG2000TestDriver
  otbJPEG2000ImageIOTestCanRead
  ${INPUTDATA}/bretagne.j2k
  )

otb_add_test(NAME ioTvJPEG2000ImageIO_CacheSize_500 COMMAND otbIOJPEG2000TestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_500.tif
  otbGenerateClassicalQLWithJPEG2000
  ${INPUTDATA}/bretagne.j2k
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_500.tif
  500)

otb_add_test(NAME ioTvJPEG2000ImageIO_CacheSize_5000 COMMAND otbIOJPEG2000TestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_5000.tif
  otbGenerateClassicalQLWithJPEG2000
  ${INPUTDATA}/bretagne.j2k
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_5000.tif
  5000)

otb_add_test(NAME ioTvJPEG2000ImageIO_CacheSize_1000 COMMAND otbIOJPEG2000TestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_1000.tif
  otbGenerateClassicalQLWithJPEG2000
  ${INPUTDATA}/bretagne.j2k
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_1000.tif
  1000)

otb_add_test(NAME ioTvJPEG2000ImageIO_CacheSize_25000 COMMAND otbIOJPEG2000TestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_OUT.tif
  otbGenerateClassicalQLWithJPEG2000
  ${INPUTDATA}/bretagne.j2k
  ${TEMP}/ioClassicalQLJPEG2K_bretagne_OUT.tif)

if(OTB_DATA_USE_LARGEINPUT)
  set( GenericTestPHR_TESTNB 0)

foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
  list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
  string(REGEX MATCH "TIFF" PHR_TIFF_FILE ${current_type})

  if(NOT PHR_TIFF_FILE)
    # CanRead
    otb_add_test(NAME ioTuJ2KImageIOCanRead_${current_type} COMMAND otbIOJPEG2000TestDriver
             otbJPEG2000ImageIOTestCanRead
                ${current_file}
            )
  endif()

endforeach()

endif()

