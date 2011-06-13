
SET(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env LD_LIBRARY_PATH   "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG" 
    --add-before-env ITK_AUTOLOAD_PATH "${CMAKE_BINARY_DIR}/Example"
)

ADD_TEST( NAME luTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
                  ${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG/otbApplicationLua
                  ${CMAKE_CURRENT_SOURCE_DIR}/LuaSmoothingTest.lua
                  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
                  ${TEMP}/rbTvSmoothing_ )
