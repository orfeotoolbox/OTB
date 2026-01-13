set(otb-group "Core")

# OTBBoostAdapter needs only Thirdparty/Boost
add_subdirectory(Adapters/BoostAdapters)
# OTBCommon needs BoostAdapters
add_subdirectory(Common)
# OTBCurlAdapters needs Common and OTBITK thirdparty
add_subdirectory(Adapters/CurlAdapters)
add_subdirectory(Adapters/GdalAdapters)

# OTBConvolution needs OTBCommon
add_subdirectory(Convolution)
# OTBObjectList depends of OTBCommon and OTBITK
add_subdirectory(ObjectList)
# OTBMetadata depends of OTBObjectList
add_subdirectory(Metadata)
# OTBImageList depends of OTBMetadata
add_subdirectory(ImageList)
# OTBImageBase depends of OTBMetadata
add_subdirectory(ImageBase)

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

# OTBLabelMap depends of OTBVectorDataManipulation
add_subdirectory(LabelMap)
# OTBProjection needs OTBVectorDataManipulation
add_subdirectory(Projection)

# OTBConversion depends of OTBLabelMap
add_subdirectory(Conversion)

# OTBMosaic needs OTBConversion
add_subdirectory(Mosaic)

# OTBStatistics needs OTBMosaic
add_subdirectory(Statistics)
# OTBPansharpening needs OTBStatistics
add_subdirectory(PanSharpening)


add_subdirectory(IO/ExtendedFilename)
add_subdirectory(IO/ImageIO)

if (OTB_USE_LIBKML)
    add_subdirectory(IO/IOKML)
    add_subdirectory(IO/KMZWriter)
endif()

add_subdirectory(IO/IOXML)
add_subdirectory(IO/TestKernel)
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
# NOTE TLA: uncomment when there is a function that generate qgis wrappers
# for the moment this folder is read in the main CMakeLists after all
# other folders
add_subdirectory(Wrappers/QGIS)
add_subdirectory(Wrappers/SWIG)
add_subdirectory(Applications)

# Get the list of enabled modules for this component. This list is created
# in OTBModuleEnablement.cmake
get_property(__ENABLED_MODULES GLOBAL PROPERTY ${arg}_MODULE_LIST)
configure_file(${CMAKE_SOURCE_DIR}/CMake/GenericGroupConfig.cmake.in ${CMAKE_BINARY_DIR}/CMakeFiles/${otb-group}Config.cmake @ONLY)
unset(__ENABLED_MODULES)

install(FILES ${CMAKE_BINARY_DIR}/CMakeFiles/${otb-group}Config.cmake
        DESTINATION ${OTB_INSTALL_PACKAGE_DIR}
        COMPONENT ${otb-group})

if (CMAKE_DEBUG)
    message(STATUS "[CMAKE_DEBUG] Generating ${otb-group}Config.cmake for component \"${otb-group}\" that will be installed in \"${OTB_INSTALL_PACKAGE_DIR}\"")
endif()


unset(otb-group)
