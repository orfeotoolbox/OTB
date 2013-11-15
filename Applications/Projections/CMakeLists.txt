
OTB_CREATE_APPLICATION(NAME           BundleToPerfectSensor
                       SOURCES        otbBundleToPerfectSensor.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           OrthoRectification
                       SOURCES        otbOrthoRectification.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           RigidTransformResample
                       SOURCES        otbRigidTransformResample.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Superimpose
                       SOURCES        otbSuperimpose.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ConvertCartoToGeoPoint
                       SOURCES        otbConvertCartoToGeoPoint.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ImageEnvelope
                       SOURCES        otbImageEnvelope.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ObtainUTMZoneFromGeoPoint
                       SOURCES        otbObtainUTMZoneFromGeoPoint.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ConvertSensorToGeoPoint
                       SOURCES        otbConvertSensorToGeoPoint.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           GridBasedImageResampling
                       SOURCES        otbGridBasedImageResampling.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           VectorDataReprojection
                       SOURCES        otbVectorDataReprojection.cxx
                       LINK_LIBRARIES OTBBasicFilters;OTBIO;OTBOGRAdapters;OTBProjections)

OTB_CREATE_APPLICATION(NAME           RefineSensorModel
                       SOURCES        otbRefineSensorModel.cxx
                       LINK_LIBRARIES OTBProjections)

OTB_CREATE_APPLICATION(NAME           GenerateRPCSensorModel
                       SOURCES        otbGenerateRPCSensorModel.cxx
                       LINK_LIBRARIES OTBProjections)
