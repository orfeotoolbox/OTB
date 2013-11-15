message(STATUS "Importing GetText...")

option(OTB_USE_GETTEXT "Get Text Library." OFF)
mark_as_advanced(OTB_USE_GETTEXT)

if(OTB_USE_GETTEXT)

    find_package(Gettext)
    mark_as_advanced(GETTEXT_MSGMERGE_EXECUTABLE)
    mark_as_advanced(GETTEXT_MSGFMT_EXECUTABLE)

    if(GETTEXT_FOUND)
      find_library(GETTEXT_LIBRARY gettextlib DOC "GetText library")
      mark_as_advanced(GETTEXT_LIBRARY)
      if(APPLE OR WIN32)
        find_library(GETTEXT_INTL_LIBRARY intl DOC "GetText intl library")
        mark_as_advanced(GETTEXT_INTL_LIBRARY)
      endif()

      if(GETTEXT_LIBRARY)
          set(OTB_I18N 1)
          message(STATUS
                  "  GetText found, configuring internationalization")
          set(OTB_LANG $ENV{LANG} CACHE STRING "OTB internationalization (Experimental)")#might want to get the Locale from the system here
          if(NOT OTB_LANG)
            set(OTB_LANG "en_EN.UTF-8" CACHE STRING "OTB internationalization (Experimental)" FORCE)
          endif()
          set(OTB_LANG_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/I18n)
          add_subdirectory(I18n)
          find_path(GETTEXT_INCLUDE_DIR
            libintl.h
            DOC "Path to gettext include directory (where libintl.h can be found)")
          mark_as_advanced(GETTEXT_INCLUDE_DIR)

          if(GETTEXT_INCLUDE_DIR)
            include_directories(${GETTEXT_INCLUDE_DIR})
          endif()
      endif()

      #message(STATUS "  Enabling GetText support")
      message(STATUS "  GetText includes: ${GETTEXT_INCLUDE_DIR}")
      message(STATUS "  GetText library : ${GETTEXT_LIBRARY}")
      if(APPLE OR WIN32)
        message(STATUS "  GetText intl library  : ${GETTEXT_INTL_LIBRARY}")
      endif()

      if(BUILD_APPLICATIONS AND OTB_WRAP_QT)
            # Check bug 419 : conflict between gettext and Qt on some Ubuntu systems
            set(CMAKE_REQUIRED_INCLUDES "${QT_INCLUDES}")
            set(CMAKE_REQUIRED_LIBRARIES "${QT_LIBRARIES};${GETTEXT_LIBRARY}")
            set(CMAKE_REQUIRED_DEFINITIONS "${QT_DEFINITIONS}")
            file(READ ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestQtGettextConflict.cxx TestQtGettextConflict_SOURCES)
            CHECK_CXX_SOURCE_RUNS(
             "${TestQtGettextConflict_SOURCES}"
             CHECK_QT_GETTEXT_CONFLICT
             )
             if(NOT CHECK_QT_GETTEXT_CONFLICT)
                  message(FATAL_ERROR "CHECK_QT_GETTEXT_CONFLICT test failed : your platform exhibits a QT/Gettext conflict.\n"
                  "Opening OTB-applications auto generated in QT may generate a crash.\n"
                  "You might consider deactivating the support of Gettext in OTB with the OTB_USE_GETTEXT cmake option.\n")
            endif()
      endif()

    else()
      set(OTB_I18N 0)
      message(STATUS
                  "  Gettext not found, internationalization will not be available")
    endif()

else()
    message(STATUS "  Disabling GetText support")
endif()
