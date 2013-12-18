# $Id$

project(OTBExamples)

if(OTB_BINARY_DIR)
  # We are building inside the tree.


add_subdirectory(BasicFilters)
add_subdirectory(FeatureExtraction)
add_subdirectory(DataRepresentation)
add_subdirectory(IO)
add_subdirectory(Filtering)
add_subdirectory(ChangeDetection)
add_subdirectory(Learning)
add_subdirectory(Classification)
add_subdirectory(Segmentation)
add_subdirectory(Iterators)
add_subdirectory(MultiScale)
add_subdirectory(DisparityMap)
add_subdirectory(Projections)
add_subdirectory(Registration)
add_subdirectory(Radiometry)
#add_subdirectory(SARPolarimetry)
add_subdirectory(Fusion)
add_subdirectory(Tutorials)
add_subdirectory(Markov)
add_subdirectory(OBIA)
add_subdirectory(Simulation)
add_subdirectory(Hyperspectral)
add_subdirectory(DimensionReduction)

if(BUILD_APPLICATIONS)
		add_subdirectory(Application)
endif()

if(OTB_USE_VISU_GUI)
        add_subdirectory(Visualization)
endif()

if(OTB_USE_PATENTED)
  add_subdirectory( Patented )
endif()

#Recopie du fichier README.txt dans l'arborescence BINARY
if( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
else()
  configure_file(${OTB_SOURCE_DIR}/Examples/README.txt
    ${OTB_BINARY_DIR}/Examples/README.txt @ONLY IMMEDIATE)
endif()


else()
  # We are building as a stand-alone project.
  set(LIBRARY_OUTPUT_PATH ${OTBExamples_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
  set(EXECUTABLE_OUTPUT_PATH ${OTBExamples_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
  mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
  set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

  # Configure Dart testing support.
  set(PROJECT_SOURCE_DIR "${OTBExamples_SOURCE_DIR}/..")
  include(${CMAKE_ROOT}/Modules/Dart.cmake)
  set(PROJECT_SOURCE_DIR "${OTBExamples_SOURCE_DIR}")
  mark_as_advanced(TCL_TCLSH)
  if(BUILD_TESTING)
    enable_testing()
  endif()

  # Find OTB and load its settings.
  find_package(OTB)
  if(OTB_FOUND)
    include(${OTB_USE_FILE})
        add_subdirectory(BasicFilters)
        add_subdirectory(FeatureExtraction)
        add_subdirectory(DataRepresentation)
        add_subdirectory(IO)
        add_subdirectory(Filtering)
        add_subdirectory(ChangeDetection)
        add_subdirectory(Learning)
        add_subdirectory(Classification)
        add_subdirectory(Segmentation)
        add_subdirectory(Iterators)
        add_subdirectory(MultiScale)
        add_subdirectory(DisparityMap)
        add_subdirectory(Registration)
        add_subdirectory(Radiometry)
        #add_subdirectory(SARPolarimetry)
        add_subdirectory(Fusion)
        add_subdirectory(Tutorials)
        add_subdirectory(Markov)
        add_subdirectory(OBIA)
        add_subdirectory(Simulation)
		add_subdirectory(Hyperspectral)

	if(BUILD_APPLICATIONS)
		add_subdirectory(Application)
	endif()

    if(OTB_USE_VISU_GUI)
        add_subdirectory(Visualization)
    endif()

    if(OTB_USE_PATENTED)
        add_subdirectory(Patented)
    endif()
  else()
    message("OTB not found.  Please set OTB_DIR")
  endif()
endif()

