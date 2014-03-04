message(STATUS "Importing LibKML...")
find_package(LibKML)

if(LIBKML_FOUND)
  option(OTB_USE_EXTERNAL_LIBKML "Use external LibKML library." ON)
else()
  option(OTB_USE_EXTERNAL_LIBKML "Use external LibKML library." OFF)

  #Known issue in OTB which does not compile with external ITK and internal libkml 
  #Bugs 896: http://bugs.orfeo-toolbox.org/view.php?id=896
  if (USE_EXTERNAL_ITK)
    message(WARNING "There is a known issue which does not allow to use internal libKML with external ITK in OTB. You might consider using an external LibKML.")
  endif()

  set(LIBKML_LIBRARIES otbkml)
  message(STATUS "Using LibKML internal version")
endif()
mark_as_advanced(OTB_USE_EXTERNAL_LIBKML)
