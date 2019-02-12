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

otb_module_test()

set(OTBIOKMLTests
otbIOKMLTestDriver.cxx
otbKMLVectorDataIOTestFileReader.cxx
otbKMLVectorDataIOTestCanWrite.cxx
otbKMLVectorDataIOTestCanRead.cxx
)

add_executable(otbIOKMLTestDriver ${OTBIOKMLTests})
target_link_libraries(otbIOKMLTestDriver ${OTBIOKML-Test_LIBRARIES})
otb_module_target_label(otbIOKMLTestDriver)

# Tests Declaration

otb_add_test(NAME ioTvKMLVectorDataIOFileReaderExample COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestFileReader
  ${INPUTDATA}/MODIS20040214212340.kml
  ${TEMP}/MODIS20040214212340.kml)

otb_add_test(NAME ioTvKMLVectorDataIOFileReaderMultiGeom COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestFileReader
  ${INPUTDATA}/multigeometry.kml
  ${TEMP}/multigeometry.kml)

otb_add_test(NAME ioTvKMLVectorDataIOFileReaderLines COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestFileReader
  ${INPUTDATA}/lines.kml
  ${TEMP}/lines.kml)

otb_add_test(NAME ioTuKMLVectorDataIOCanWrite COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestCanWrite
  ${TEMP}/MODIS20040214212340.kml)

otb_add_test(NAME ioTuKMLVectorDataIOCanRead COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestCanRead
  ${INPUTDATA}/MODIS20040214212340.kml)
