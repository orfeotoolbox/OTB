message(STATUS "Importing LibKML...")
find_package(LibKML)

if(LIBKML_FOUND)
  option(OTB_USE_EXTERNAL_LIBKML "Use external LibKML library." ON)
else()
  option(OTB_USE_EXTERNAL_LIBKML "Use external LibKML library." OFF)
  set(LIBKML_LIBRARIES otbkml)
  message(STATUS "Using LibKML internal version")
endif()

# Known issue in OTB which does not compile with external ITK and internal libkml 
if (OTB_USE_EXTERNAL_ITK AND NOT OTB_USE_EXTERNAL_LIBKML)
  message(WARNING "There is a known issue which does not allow to use internal libKML with external ITK in OTB."
                  "You might consider using an external LibKML. "
                  "See http://bugs.orfeo-toolbox.org/view.php?id=896 for details")
endif()

mark_as_advanced(OTB_USE_EXTERNAL_LIBKML)
