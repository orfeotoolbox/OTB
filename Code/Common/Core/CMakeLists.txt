#############################################################################
# Source files of Common library

# Package CORE (files ordered by lexicographical order i.e. ease of search).
set( Common_Core_SOURCES
  mvdAbstractImageModel.cxx
  mvdAbstractModel.cxx
  mvdAbstractWorker.cxx
  mvdAlgorithm.cxx
  mvdBackgroundTask.cxx
  mvdCore.cxx
  mvdDatasetDescriptor.cxx
  mvdDatabaseModel.cxx
  mvdDatasetModel.cxx
  mvdHistogramModel.cxx
  mvdI18nCoreApplication.cxx
  mvdImageImporter.cxx
  mvdImagePlacenameLoader.cxx
  mvdImageProperties.cxx
  mvdMath.cxx
  mvdModifiableInterface.cxx
  mvdMyClass.cxx
  mvdQuicklookModel.cxx
  mvdSerializableInterface.cxx
  mvdSystemError.cxx
  mvdTextStream.cxx
  mvdTypes.cxx
  mvdVectorImageModel.cxx
)

#############################################################################
# Qt's MOC header files for Common library (QObject derived classes).

# Package CORE (files ordered by lexicographical order i.e. ease of search).
set( Common_Core_HEADERS_MOC
  mvdAbstractImageModel.h
  mvdAbstractModel.h
  mvdAbstractWorker.h
  # mvdAlgorithm.h
  mvdBackgroundTask.h
  # mvdCore.h
  mvdDatabaseModel.h
  mvdDatasetDescriptor.h
  mvdDatasetModel.h
  mvdHistogramModel.h
  mvdI18nCoreApplication.h
  mvdImageImporter.h
  mvdImagePlacenameLoader.h
  # mvdImageProperties.h
  # mvdMath.h
  mvdMyClass.h
  mvdQuicklookModel.h
  # mvdSystemError.h
  # mvdTypes.h
  mvdVectorImageModel.h
)

#############################################################################
# Includes
include_directories(
  ${Monteverdi2_BINARY_DIR}
  #
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  #${Monteverdi2_SOURCE_DIR}/Code/Common/Core
  #
  ${Monteverdi2_BINARY_DIR}/Code/Common
  ${Monteverdi2_BINARY_DIR}/Code/Common/Core
)

#############################################################################
# Qt4 wrapped files (meta-object generated).
qt4_wrap_cpp( Common_Core_SRC_MOC ${Common_Core_HEADERS_MOC} )

########################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Common_Core_SOURCES}
)

#############################################################################
add_library(Monteverdi2_Core
  ${Common_Core_SOURCES}
  ${Common_Core_SRC_MOC}
)

#############################################################################
target_link_libraries(Monteverdi2_Core
  OTBIO
  Qt4::QtCore
  Qt4::QtXml
)

#############################################################################
set_target_properties(Monteverdi2_Core
  PROPERTIES ${Monteverdi2_LIBRARY_PROPERTIES}
)

#############################################################################
# INSTALL Common application.
install(
  TARGETS Monteverdi2_Core
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
