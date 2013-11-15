message(STATUS "Importing Pqxx...")

option(OTB_USE_PQXX "Use pqxx library (EXPERIMENTAL)." OFF)
mark_as_advanced(OTB_USE_PQXX)

if(OTB_USE_PQXX)
  find_package(Pqxx)
  if(PQXX_FOUND)
    message(STATUS "Activating Pqxx support")
  else(PQXX_FOUND)
    # Generate an error if no external Pqxx is available
    message(FATAL_ERROR "Pqxx required but not found. "
                        "Turn OTB_USE_PQXX to OFF")
  endif(PQXX_FOUND)
else(OTB_USE_PQXX)
  message(STATUS "Disabling Pqxx support")
endif(OTB_USE_PQXX)
