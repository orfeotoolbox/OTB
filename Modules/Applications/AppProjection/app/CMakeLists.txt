set(OTBAppProjection_LINK_LIBS
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBCarto_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
)

otb_create_application(
  NAME           OrthoRectification
  SOURCES        otbOrthoRectification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConvertCartoToGeoPoint
  SOURCES        otbConvertCartoToGeoPoint.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           GenerateRPCSensorModel
  SOURCES        otbGenerateRPCSensorModel.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           GridBasedImageResampling
  SOURCES        otbGridBasedImageResampling.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorDataReprojection
  SOURCES        otbVectorDataReprojection.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ObtainUTMZoneFromGeoPoint
  SOURCES        otbObtainUTMZoneFromGeoPoint.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           RefineSensorModel
  SOURCES        otbRefineSensorModel.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           RigidTransformResample
  SOURCES        otbRigidTransformResample.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ImageEnvelope
  SOURCES        otbImageEnvelope.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConvertSensorToGeoPoint
  SOURCES        otbConvertSensorToGeoPoint.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Superimpose
  SOURCES        otbSuperimpose.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
