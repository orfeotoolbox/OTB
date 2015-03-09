set(OTBSVMLearning_SRC
  otbChangeProfileKernelFunctor.cxx
  otbExhaustiveExponentialOptimizer.cxx
  otbNonGaussianRBFKernelFunctor.cxx
  otbMixturePolyRBFKernelFunctor.cxx
  otbSpectralAngleKernelFunctor.cxx
  )

add_library(OTBSVMLearning ${OTBSVMLearning_SRC})
target_link_libraries(OTBSVMLearning 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBLibSVM_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBSVMLearning)
