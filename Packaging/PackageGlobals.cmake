#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
  WTSAPI32.dll
  advapi32.dll
  comdlg32.dll
  crypt32.dll
  d2d1.dll
  d3d11.dll
  d3d9.dll
  dnsapi.dll
  dwmapi.dll
  dwrite.dll
  dxgi.dll
  gdi32.dll
  imm32.dll
  iphlpapi.dll
  kernel32.dll
  mpr.dll
  netapi32.dll
  normaliz.dll
  odbc32.dll
  ole32.dll
  oleaut32.dll
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
  MSVCP140_1.dll
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
  libgssapi_krb5.so*
  libdrm.so.2
  libSM.so*
  libpython*
  libmysqlclient.so*
  libodbc.so*
  libpq.so*
  libpcre.so*
  )

set(APPLE_SYSTEM_DLLS
  libSystem.*dylib
  libiconv.*dylib
  libc\\+\\+.*dylib
  libstdc.*dylib
  libobjc.*dylib
  libcups.*dylib
  libomp.dylib
  libqcocoa.dylib
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
