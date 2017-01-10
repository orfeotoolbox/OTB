set(WINDOWS_SYSTEM_DLLS
  user32.dll
  gdi32.dll
  shell32.dll
  kernel32.dll
  ws2_32.dll
  wldap32.dll
  ole32.dll
  comdlg32.dll
  shfolder.dll
  secur32.dll
  wsock32.dll
  advapi32.dll
  crypt32.dll
  imm32.dll
  oleaut32.dll
  winmm.dll
  opengl32.dll
  glu32.dll
  rpcrt4.dll
  winspool.drv
  normaliz.dll
  odbc32.dll
  psapi.dll
  python...dll
  )

if(MINGW)
  set(WINDOWS_SYSTEM_DLLS
    ${WINDOWS_SYSTEM_DLLS}
    msvcrt.dll
    )
endif()

if(PKG_GENERATE_XDK)
  set(WINDOWS_SYSTEM_DLLS
    ${WINDOWS_SYSTEM_DLLS}
    api-ms-win-*
    concrt140.dll
    ucrtbase.dll
    msvcp140.dll
    msvrt140.dll
    vcomp140.dll
    )
endif()

set(LINUX_SYSTEM_DLLS
  libm.so
  libc.so
  libstdc*
  libgcc_s.so
  librt.so
  libdl.so
  libpthread.so
  libidn.so
  libgomp.so*
  ld-linux-x86-64.so*
  libX11.so*
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
  libexpat.so.0
  )

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
set(ALLOWED_SYSTEM_DLLS
  libfreetype.so.6.*
  libgthread-2.0.so.0
  libglib-2.0.so.0
  libgobject-2.0.so.0
  libXrender.so.1
  libfontconfig.so.1
  libpcre.so.3
  )
