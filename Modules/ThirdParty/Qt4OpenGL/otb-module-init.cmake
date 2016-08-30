# TODO:  check the QT_LIBINFIX

set(QT_QTOPENGL_LIBRARY_RELEASE NOTFOUND)
set(QT_QTOPENGL_LIBRARY_DEBUG NOTFOUND)
find_library(QT_QTOPENGL_LIBRARY_RELEASE
             NAMES QtOpenGL${QT_LIBINFIX} QtOpenGL${QT_LIBINFIX}4
             PATHS ${QT_LIBRARY_DIR}
             NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH
            )
find_library(QT_QTOPENGL_LIBRARY_DEBUG
             NAMES QtOpenGL${QT_LIBINFIX}_debug QtOpenGL${QT_LIBINFIX}d QtOpenGL${QT_LIBINFIX}d4
             PATHS ${QT_LIBRARY_DIR}
             NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH
            )

if (QT_QTOPENGL_LIBRARY_RELEASE)
  if(QT_QTOPENGL_LIBRARY_DEBUG)
    # both libraries present
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      set(QT_QTOPENGL_LIBRARY optimized ${QT_QTOPENGL_LIBRARY_RELEASE} debug ${QT_QTOPENGL_LIBRARY_DEBUG})
    else()
      set(QT_QTOPENGL_LIBRARY ${QT_QTOPENGL_LIBRARY_RELEASE})
    endif()
  else()
    # only release library present
    set(QT_QTOPENGL_LIBRARY ${QT_QTOPENGL_LIBRARY_RELEASE})
  endif()
else()
  if(QT_QTOPENGL_LIBRARY_DEBUG)
    # only debug library present
    set(QT_QTOPENGL_LIBRARY ${QT_QTOPENGL_LIBRARY_DEBUG})
  else()
    set(QT_QTOPENGL_LIBRARY "")
  endif()
endif()

set(QT_QTOPENGL_LIBRARY "${QT_QTOPENGL_LIBRARY}" CACHE STRING "The QtOpenGL library" FORCE)
