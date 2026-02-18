set(otb-group "Core")

# OTBBoostAdapter needs only Thirdparty/Boost
add_subdirectory(Adapters/BoostAdapters)
# OTBCommon needs BoostAdapters
# OTBCommon tests needs OTBImageBase
add_subdirectory(Common)
# OTBCurlAdapters needs Common and OTBITK thirdparty
add_subdirectory(Adapters/CurlAdapters)
add_subdirectory(Adapters/GdalAdapters)

# OTBObjectList depends of OTBCommon and OTBITK
add_subdirectory(ObjectList)
# OTBMetadata depends of OTBObjectList
add_subdirectory(Metadata)
# OTBImageList depends of OTBMetadata
add_subdirectory(ImageList)
# OTBImageBase depends of OTBMetadata
add_subdirectory(ImageBase)
# OTBConvolution needs OTBCommon
add_subdirectory(Convolution)

# --- OTBImage base dependee
# OTBFunctor depends of OTBImageBase
add_subdirectory(Functor)
# OTBVectorDataBase needs OTBImageBase
add_subdirectory(VectorDataBase)
# OTBStreaming depends of OTBImageBase
add_subdirectory(Streaming)
# OTBInterpolation needs only OTBImageBase
add_subdirectory(Interpolation)
# IOGDAL needs OTBImageBase
add_subdirectory(IO/IOGDAL)
# IOExtendedFilename depends of OTBIOGDAL
add_subdirectory(IO/ExtendedFilename)
# ---

# OTBPath depends of OTBVectorDataBase
add_subdirectory(Path)
# OTBMoment needs OTBPath
add_subdirectory(Moments)

# OTBTransform needs OTBInterpolation
add_subdirectory(Transform)
# OTBPointSet needs OTBInterpolation
add_subdirectory(PointSet)

# OTBDensity needs OTBPointSet
add_subdirectory(Density)

# OTBImageManipulation needs OTBTransform
add_subdirectory(ImageManipulation)
# OTBVectorDataManipulation needs OTBTransform
add_subdirectory(VectorDataManipulation)

# OTBProjection needs OTBVectorDataManipulation
add_subdirectory(Projection)
# OTBLabelMap depends of OTBVectorDataManipulation
add_subdirectory(LabelMap)

# OTBConversion depends of OTBLabelMap
add_subdirectory(Conversion)

# OTBMosaic needs OTBConversion
add_subdirectory(Mosaic)

# OTBStatistics needs OTBMosaic
add_subdirectory(Statistics)
# OTBImageIO depends of OTBStatistics
add_subdirectory(IO/ImageIO)

# TestKernel depends of ImageIO
add_subdirectory(IO/TestKernel)
# OTBPansharpening needs OTBStatistics
add_subdirectory(PanSharpening)

add_subdirectory(IO/IOKML)
add_subdirectory(IO/KMZWriter)

add_subdirectory(IO/IOXML)
add_subdirectory(IO/VectorDataIO)
# OTBCarto needs OTBImageIO
add_subdirectory(IO/Carto)

add_subdirectory(ColorMap)
add_subdirectory(ComplexImage)

add_subdirectory(DEM)

add_subdirectory(Descriptors)
add_subdirectory(Edge)
add_subdirectory(Fusion/Fuzzy)
add_subdirectory(Fusion/MajorityVoting)
add_subdirectory(Indices)


add_subdirectory(OpticalCalibration)

add_subdirectory(Smoothing)
add_subdirectory(Wrappers/ApplicationEngine)
add_subdirectory(Wrappers/CommandLine)
add_subdirectory(Applications)
# NOTE TLA: uncomment when there is a function that generate qgis wrappers
# for the moment this folder is read in the main CMakeLists after all
# other folders
# add_subdirectory(Wrappers/QGIS)
add_subdirectory(Wrappers/SWIG)

# Get the list of enabled modules for this component. This list is created
# in OTBModuleEnablement.cmake
get_property(__ENABLED_MODULES GLOBAL PROPERTY ${otb-group}_MODULE_LIST)
configure_file(${CMAKE_SOURCE_DIR}/CMake/GenericGroupConfig.cmake.in ${CMAKE_BINARY_DIR}/CMakeFiles/${otb-group}Config.cmake @ONLY)
unset(__ENABLED_MODULES)

install(FILES ${CMAKE_BINARY_DIR}/CMakeFiles/${otb-group}Config.cmake
        DESTINATION ${OTB_INSTALL_PACKAGE_DIR}
        COMPONENT ${otb-group})

if (CMAKE_DEBUG)
    message(STATUS "[CMAKE_DEBUG] ${otb-group}Config.cmake generated for component \"${otb-group}\" that will be installed in \"${OTB_INSTALL_PACKAGE_DIR}\"")
endif()


# /!\ UGLY FIX, DO NOT TRY THIS AT HOME /!\
# Read the test folder of Core modules ONLY AFTER READING ALL MODULES DIRS.
# When reading the module directories, we create file OTB<ModuleName>.cmake used
# by intern mecanism to resolve dependency (in otb_module_impl)
# THIS IS A NASTY FIX needed to make tests works as THERE IS CYCLIC DEPENDENCIES
# FOR CORE TESTS. Thus it need some the OTB<ModuleName>.cmake files that are
# not already made (but only for test).
# E.G. OTBCommon tests needs OTBTestKernel, which needs... OTBCommon!! Yeeeeh!!
#
# WHEN THIS CODE IS CLEANED, also remove the if (component != Core) that
# does not read tests when we use the Core module
# in otb_module_impl in OTBModuleMacro. Do the same for the Remote Cmakelists!
#
# Instead of Nasty solution, if we have time, we can use solutions proposed at
# https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/-/issues/2530#note_118810

# First read all otb-modules files of Core
if (BUILD_TESTING)
    # part of this code was in OTBModuleEnablement
    # match all otb-module.cmake files in Core and its subdirs.
    file( GLOB_RECURSE _core_modules_files RELATIVE "${OTB_SOURCE_DIR}" ${CMAKE_CURRENT_SOURCE_DIR}/*/otb-module.cmake )
    foreach(_module_file ${_core_modules_files})
        # get parent directory
        get_filename_component(_module_dir ${_module_file} DIRECTORY)
        # check if tests exists and build them
        if (EXISTS "${OTB_SOURCE_DIR}/${_module_dir}/test/CMakeLists.txt")
            add_subdirectory(${OTB_SOURCE_DIR}/${_module_dir}/test)
        endif()
    endforeach()
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/PackageConfig.cmake)
unset(otb-group)
