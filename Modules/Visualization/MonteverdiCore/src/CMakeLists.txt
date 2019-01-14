#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#############################################################################
set( OTBMonteverdiCore_SRCS
  mvdAbstractWorker.cxx
  mvdAbstractModel.cxx
  mvdAbstractImageModel.cxx
  mvdAbstractLayerModel.cxx
  mvdBackgroundTask.cxx
  mvdCore.cxx
  mvdDataStream.cxx
  mvdFilenameInterface.cxx
  mvdHistogramModel.cxx
  mvdI18nCoreApplication.cxx
  mvdImageImporter.cxx
  mvdImagePlacenameLoader.cxx
  mvdImageProperties.cxx
  mvdImageSettings.cxx
  mvdImageSettingsInterface.cxx
  mvdModifiableInterface.cxx
  mvdMyClass.cxx
  mvdOverviewBuilder.cxx
  mvdProcessObjectObserver.cxx
  mvdProgressInterface.cxx
  mvdQuicklookModel.cxx
  mvdSerializableInterface.cxx
  mvdStackedLayerModel.cxx
  mvdTextStream.cxx
  mvdTypes.cxx
  mvdVectorImageModel.cxx
  mvdVectorImageSettings.cxx
  mvdVisibleInterface.cxx
  )

set( OTBMonteverdiCore_HEADERS_MOC
  ../include/mvdAbstractImageModel.h
  ../include/mvdAbstractLayerModel.h
  ../include/mvdAbstractModel.h
  ../include/mvdAbstractWorker.h
  # ../include/mvdAlgorithm.h
  ../include/mvdBackgroundTask.h
  # ../include/mvdCore.h
  # ../include/mvdDatabaseConnection.h
  # ../include/mvdDatabaseError.h
  # ../include/mvdDatabaseModel.h
  # ../include/mvdDatabaseQueries.h
  # ../include/mvdDatasetDescriptor.h
  # ../include/mvdDatasetModel.h
  ../include/mvdHistogramModel.h
  ../include/mvdI18nCoreApplication.h
  ../include/mvdImageImporter.h
  ../include/mvdImagePlacenameLoader.h
  # ../include/mvdImageProperties.h
  # ../include/mvdMath.h
  ../include/mvdMyClass.h
  ../include/mvdOverviewBuilder.h
  ../include/mvdQuicklookModel.h
  ../include/mvdStackedLayerModel.h
  # ../include/mvdSystemError.h
  # ../include/mvdTypes.h
  ../include/mvdVectorImageModel.h
)

#############################################################################
if( OTBApplicationEngine_ENABLED )
  list( APPEND OTBMonteverdiCore_SRCS
    mvdApplicationsBrowser.cxx
  )
  list( APPEND OTBMonteverdiCore_HEADERS_MOC
    ../include/mvdApplicationsBrowser.h
  )
endif()

#############################################################################
add_to_qt_i18n_sources( ${OTBMonteverdiCore_SRCS} )
add_to_qt_i18n_headers( "../include" ${OTBMonteverdiCore_SRCS} )

#############################################################################
qt5_wrap_cpp( OTBMonteverdiCore_SRC_MOC ${OTBMonteverdiCore_HEADERS_MOC} OPTIONS -p "." )

add_library( OTBMonteverdiCore ${OTBMonteverdiCore_SRCS} ${OTBMonteverdiCore_SRC_MOC})

#############################################################################
target_link_libraries( OTBMonteverdiCore
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBCarto_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBQt_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  )

#############################################################################
otb_module_target( OTBMonteverdiCore )
