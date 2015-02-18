project(OTBOssimPlugins)
set(OTBOssimPlugins_THIRD_PARTY 1)

set(OTBOssimPlugins_LIBRARIES otbossimplugins)

set(OTBOssimPlugins_INCLUDE_DIRS
  ${OTBOssimPlugins_SOURCE_DIR}/src
  ${OTBOssimPlugins_SOURCE_DIR}/src/ossim
  )

otb_module_impl()
