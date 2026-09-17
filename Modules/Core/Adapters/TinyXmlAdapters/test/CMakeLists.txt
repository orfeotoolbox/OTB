#
# Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_executable(otbTuXmlWrapper otbTestXmlWrapper.cxx)
  target_link_libraries(
    otbTuXmlWrapper
    ${OTBTinyXMLAdapters_LIBRARIES}
    ${OTBTinyXML_LIBRARIES} # TODO: find a way to not make this dependency implicit
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

  add_test(NAME coTuXmlWrapper
           COMMAND otbTuXmlWrapper #coTuXmlWrapper
           WORKING_DIRECTORY ${TEMP})

endif()
