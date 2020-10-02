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

# Configuration options for macOS

set(site_option
"OTB_USE_GLUT=OFF
OTB_USE_GLFW=OFF
CMAKE_C_COMPILER_LAUNCHER:STRING=ccache
CMAKE_CXX_COMPILER_LAUNCHER:STRING=ccache
OPENGL_INCLUDE_DIR:STRING=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/OpenGL.framework/Headers
OTB_DATA_USE_LARGEINPUT:BOOL=ON") 
# Large input path are in an environment variable on macOS
