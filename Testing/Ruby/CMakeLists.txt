
set(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env LD_LIBRARY_PATH   "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG"
    --add-before-env ITK_AUTOLOAD_PATH "${CMAKE_BINARY_DIR}/Example"
)

add_test( NAME rbTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
                  ${RUBY_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/RubySmoothingTest.rb
                  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
                  ${TEMP}/rbTvSmoothing_ )
