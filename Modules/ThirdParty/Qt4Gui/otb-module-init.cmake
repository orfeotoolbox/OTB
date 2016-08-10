# TODO:  check the QT_LIBINFIX

set(QT_QTGUI_LIBRARY_RELEASE NOTFOUND)
set(QT_QTGUI_LIBRARY_DEBUG NOTFOUND)
find_library(QT_QTGUI_LIBRARY_RELEASE
             NAMES QtGui${QT_LIBINFIX} QtGui${QT_LIBINFIX}4
             PATHS ${QT_LIBRARY_DIR}
             NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH
            )
find_library(QT_QTGUI_LIBRARY_DEBUG
             NAMES QtGui_debug QtGuid QtGuid4
             PATHS ${QT_LIBRARY_DIR}
             NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH
            )

message(STATUS "QT_QTGUI_LIBRARY_DEBUG : ${QT_QTGUI_LIBRARY_DEBUG}")

if (QT_QTGUI_LIBRARY_RELEASE)
  if(QT_QTGUI_LIBRARY_DEBUG)
    # both libraries present
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      set(QT_QTGUI_LIBRARY optimized ${QT_QTGUI_LIBRARY_RELEASE} debug ${QT_QTGUI_LIBRARY_DEBUG})
    else()
      set(QT_QTGUI_LIBRARY ${QT_QTGUI_LIBRARY_RELEASE})
    endif()
  else()
    # only release library present
    set(QT_QTGUI_LIBRARY ${QT_QTGUI_LIBRARY_RELEASE})
  endif()
else()
  if(QT_QTGUI_LIBRARY_DEBUG)
    # only debug library present
    set(QT_QTGUI_LIBRARY ${QT_QTGUI_LIBRARY_DEBUG})
  else()
    set(QT_QTGUI_LIBRARY "")
  endif()
endif()

set(QT_QTGUI_LIBRARY "${QT_QTGUI_LIBRARY}" CACHE STRING "The QtGui library" FORCE)

