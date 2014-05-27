message(STATUS "Importing LibKML...")
find_package(LibKML)

if(LIBKML_FOUND)
  option(OTB_USE_EXTERNAL_LIBKML "Use external LibKML library." ON)
else()
  option(OTB_USE_EXTERNAL_LIBKML "Use external LibKML library." OFF)
endif()
mark_as_advanced(OTB_USE_EXTERNAL_LIBKML)

if(OTB_USE_EXTERNAL_LIBKML)
  if(NOT LIBKML_FOUND)
    message(FATAL_ERROR "Can't build OTB without LibKML. Instal it "
                        "on your system, or disable the option "
                        "OTB_USE_EXTERNAL_LIBKML to use internal one")
  endif()
else()
  set(LIBKML_LIBRARIES otbkml)
  message(STATUS "  Using LibKML internal version")
  # Known issue in OTB which does not compile with external ITK and internal libkml 
  if (OTB_USE_EXTERNAL_ITK)
    message(WARNING "There is a known issue which does not allow to use internal libKML with external ITK in OTB."
                    "You might consider using an external LibKML. "
                    "See http://bugs.orfeo-toolbox.org/view.php?id=896 for details")
  endif()
endif()
