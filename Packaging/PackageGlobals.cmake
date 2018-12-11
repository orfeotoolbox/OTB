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

set(LOADER_REGEX_EOL "E")
set(DEST_BIN_DIR bin)
set(DEST_APP_DIR lib/otb/applications)

set(LIB_PREFIX lib)
set(DEST_LIB_DIR lib)
set(EXE_EXT "")
set(SCRIPT_EXT ".sh")
set(LIB_EXT ".so")
set(PYMODULE_EXT ".so")
set(STATIC_LIB_EXT ".a")

if(WIN32)
  set(LIB_PREFIX)
  set(DEST_LIB_DIR bin)
  set(EXE_EXT ".exe")
  set(LIB_EXT ".dll")
  set(SCRIPT_EXT ".bat")
  set(PYMODULE_EXT ".pyd")
  set(STATIC_LIB_EXT ".lib")
elseif(APPLE)
  set(LIB_EXT ".dylib")
endif()

set(WINDOWS_SYSTEM_DLLS
  advapi32.dll
  comdlg32.dll
  crypt32.dll
  dnsapi.dll
  dwmapi.dll
  dwrite.dll
  d2d1.dll
  d3d9.dll
  d3d11.dll
  gdi32.dll
  glu32.dll
  imm32.dll
  iphlpapi.dll
  kernel32.dll
  netapi32.dll
  normaliz.dll
  mpr.dll
  odbc32.dll
  ole32.dll
  oleaut32.dll
  opengl32.dll
  psapi.dll
  python...dll
  rpcrt4.dll
  secur32.dll
  shell32.dll
  shfolder.dll
  user32.dll
  userenv.dll
  uxtheme.dll
  version.dll
  winmm.dll
  winspool.drv
  wldap32.dll
  ws2_32.dll
  wsock32.dll
  )

set(LINUX_SYSTEM_DLLS
  libm.so
  libc.so.*
  libstdc*
  libgcc_s.so
  librt.so
  libdl.so
  libpthread.so
  libidn.so
  libgomp.so*
  ld-linux-x86-64.so*
  libX11.so*
  libXi.so*  #GLUT
  libXcursor.so* #GLFW
  libXinerama.so*  #GLFW
  libXext.so*
  libXau.so*
  libXdmcp.so*
  libXxf86vm.so*
  libdrm.so.2
  libGL.so*
  libGLU.so*
  libXrender.so*
  libSM.so*
  libICE.so*
  libXrandr.so*
  libpython*
  libxcb.so*
  libxcb-glx.so*
  libX11-xcb.so*
  libmysqlclient.so*
  libodbc.so*
  libpq.so*
  libEGL.so*
  )
  # libexpat.so.*
  # libfontconfig.so*
  # libfreetype.so*
  # libwebp.so*
  # )

# libgcc_s.*dylib and other *.framework are dragged by QT
set(APPLE_SYSTEM_DLLS
  libSystem.*dylib
  libiconv.*dylib
  libc\\+\\+.*dylib
  libstdc.*dylib
  libobjc.*dylib
  ApplicationServices.framework
  CoreFoundation.framework
  CoreServices.framework
  Security.framework
  Carbon.framework
  AppKit.framework
  Foundation.framework
  Python.framework
  AGL.framework
  OpenGL.framework
  libgcc_s.*dylib
  libcups.*dylib
  libomp.dylib
  )

if(WIN32)
  set(SYSTEM_DLLS "${WINDOWS_SYSTEM_DLLS}")
else() #case for unixes
  if(APPLE)
    set(SYSTEM_DLLS "${APPLE_SYSTEM_DLLS}")
  else()
    set(SYSTEM_DLLS "${LINUX_SYSTEM_DLLS}")
  endif()
endif(WIN32)

# Take gtk libs from system. we should fix this to take from custom
# build location
set(PKG_GTK_SEARCHDIRS
  /usr/lib64
  /lib64
  /lib/x86_64-linux-gnu/
  /usr/lib/x86_64-linux-gnu/
  )

#superbuild cannot manage build of gtk2+ just for qt gtkstyle.
# -gtkstyle option is deactivated by default in build of QT4
# So the list of requirements on building OTB with superbuild stays same.
#For our user base, we need monteverdi with a nice look and feel
# rather than simply X11 based windows. Hence we need -gtkstyle
# This forces us to have system gtk+ installed on the system.
# OTB package manager 'this script' will pick them up and put
# into the binary package. Below cmake variable controls the list of
#libraries coming from /usr/lib a.k.a system.

set(GTK_LIB_LIST_1
  libgthread-2.0.so.0
  libglib-2.0.so.0
  libgobject-2.0.so.0
  libXrender.so.1
  libpcre.so.3
  libffi.so.6
  )
# set(GTK_LIB_LIST_2
#   libz.so.1
#   libpng12.so.0
#   )


#set(ALLOWED_SYSTEM_DLLS ${GTK_LIB_LIST_1} ${GTK_LIB_LIST_2})
set(ALLOWED_SYSTEM_DLLS ${GTK_LIB_LIST_1}) # ${GTK_LIB_LIST_2})
