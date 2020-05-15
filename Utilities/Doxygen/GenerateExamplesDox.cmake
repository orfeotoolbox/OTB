#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#
# Usage: cmake -D "PROJECT_SOURCE_DIR:PATH=${PROJECT_SOURCE_DIR}"
#              -D "OUTPUT_FILE:PATH=${PROJECT_BINARY_DIR}/Examples.dox"
#              -P GenerateExamplesDox.cmake

# This cmake script gets a lists of examples for the project and
# generates a file suitable for doxygen. The file is a list of
# "\examples filename", where doxygen will search for fileanme in it's
# example path. Doxygen should then generate an examples page with the
# referenced files.

# glob the examples for each language
file( GLOB_RECURSE EXAMPLES_LIST
  RELATIVE "${PROJECT_SOURCE_DIR}/Examples"
  "*.cxx"
  "*.py"
  "*.java"
)

# remove the file before we begin appending
file( REMOVE ${OUTPUT_FILE} )

# begin comment
file( APPEND ${OUTPUT_FILE} "/**\n")

# After updating to Doxygen 1.8.4, this new line is required for any
# of the \example to be utilized
file( APPEND ${OUTPUT_FILE} " \n")

FOREACH( f IN LISTS EXAMPLES_LIST )
  file( APPEND ${OUTPUT_FILE}  "\\example ${f}\n" )
ENDFOREACH( )

# end comment
file( APPEND ${OUTPUT_FILE}  "*/\n" )
