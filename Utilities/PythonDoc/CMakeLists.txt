#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

set( OTB_SPHINX_OUTPUT_DIR ${OTB_BINARY_DIR}/Documentation/PythonDoc )

set( OTB_SPHINX_OUTPUT_DIR_HTML ${OTB_SPHINX_OUTPUT_DIR}/html)

set( OTB_SPHINX_INPUT_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src )

# Copy rst sources in build dir
file( COPY ${OTB_SPHINX_INPUT_DIR} DESTINATION ${OTB_SPHINX_OUTPUT_DIR})

set ( SPHINXBUILD -msphinx -M )
add_custom_target( Documentation-sphinx
    COMMAND ${CMAKE_COMMAND} -E copy ${OTB_BINARY_DIR}/${OTB_INSTALL_PYTHON_DIR}/otbApplication.py ${OTB_SPHINX_OUTPUT_DIR}/src/
    COMMAND ${CMAKE_COMMAND} -E copy ${OTB_BINARY_DIR}/${OTB_INSTALL_PYTHON_DIR}/_otbApplication.so ${OTB_SPHINX_OUTPUT_DIR}/src/
    COMMAND python -msphinx ${OTB_SPHINX_OUTPUT_DIR}/src ${OTB_SPHINX_OUTPUT_DIR_HTML}
    DEPENDS OTBSWIGWrapper-all
    )
