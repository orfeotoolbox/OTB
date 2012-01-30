MESSAGE(STATUS "Importing GetText...")

OPTION(OTB_USE_GETTEXT "Get Text Library." OFF)
MARK_AS_ADVANCED(OTB_USE_GETTEXT)

IF(OTB_USE_GETTEXT)
    
    FIND_PACKAGE(Gettext)
    MARK_AS_ADVANCED(GETTEXT_MSGMERGE_EXECUTABLE)
    MARK_AS_ADVANCED(GETTEXT_MSGFMT_EXECUTABLE)
    
    IF(GETTEXT_FOUND)
      FIND_LIBRARY(GETTEXT_LIBRARY gettextlib DOC "GetText library")
      MARK_AS_ADVANCED(GETTEXT_LIBRARY)
      IF(APPLE OR WIN32)
        FIND_LIBRARY(GETTEXT_INTL_LIBRARY intl DOC "GetText intl library")
        MARK_AS_ADVANCED(GETTEXT_INTL_LIBRARY)
      ENDIF(APPLE OR WIN32)
    
      IF(GETTEXT_LIBRARY)
          SET(OTB_I18N 1)
          MESSAGE(STATUS
                  "  GetText found, configuring internationalization")
          SET(OTB_LANG $ENV{LANG} CACHE STRING "OTB internationalization (Experimental)")#might want to get the Locale from the system here
          IF(NOT OTB_LANG)
            SET(OTB_LANG "en_EN.UTF-8" CACHE STRING "OTB internationalization (Experimental)" FORCE)
          ENDIF(NOT OTB_LANG)
          SET(OTB_LANG_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/I18n)
          ADD_SUBDIRECTORY(I18n)
          FIND_PATH(GETTEXT_INCLUDE_DIR
            libintl.h
            DOC "Path to gettext include directory (where libintl.h can be found)")
          MARK_AS_ADVANCED(GETTEXT_INCLUDE_DIR)
        
          IF(GETTEXT_INCLUDE_DIR)
            INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
          ENDIF(GETTEXT_INCLUDE_DIR)
      ENDIF(GETTEXT_LIBRARY)
      
      #MESSAGE(STATUS "  Enabling GetText support")
      MESSAGE(STATUS "  GetText includes: ${GETTEXT_INCLUDE_DIR}")
      MESSAGE(STATUS "  GetText library : ${GETTEXT_LIBRARY}")
      IF(APPLE OR WIN32)
        MESSAGE(STATUS "  GetText intl library  : ${GETTEXT_INTL_LIBRARY}")
      ENDIF(APPLE OR WIN32)
    
      IF(BUILD_APPLICATIONS AND OTB_WRAP_QT)
      			# Check bug 419 : conflict between gettext and Qt on some Ubuntu systems
            SET(CMAKE_REQUIRED_INCLUDES "${QT_INCLUDES}")
            SET(CMAKE_REQUIRED_LIBRARIES "${QT_LIBRARIES};${GETTEXT_LIBRARY}")
            SET(CMAKE_REQUIRED_DEFINITIONS "${QT_DEFINITIONS}")
            FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestQtGettextConflict.cxx TestQtGettextConflict_SOURCES)
            CHECK_CXX_SOURCE_RUNS(
             "${TestQtGettextConflict_SOURCES}"
             CHECK_QT_GETTEXT_CONFLICT
             )
             IF(NOT CHECK_QT_GETTEXT_CONFLICT)
                  MESSAGE(FATAL_ERROR "CHECK_QT_GETTEXT_CONFLICT test failed : your platform exhibits a QT/Gettext conflict.\n"
                  "Opening OTB-applications auto generated in QT may generate a crash.\n"
                  "You might consider deactivating the support of Gettext in OTB with the OTB_USE_GETTEXT cmake option.\n")
            ENDIF(NOT CHECK_QT_GETTEXT_CONFLICT)
      ENDIF(BUILD_APPLICATIONS AND OTB_WRAP_QT)
      
    ELSE(GETTEXT_FOUND)
      SET(OTB_I18N 0)
      MESSAGE(STATUS
                  "  Gettext not found, internationalization will not be available")
    ENDIF(GETTEXT_FOUND)
    
ELSE(OTB_USE_GETTEXT)
    MESSAGE(STATUS "  Disabling GetText support")
ENDIF(OTB_USE_GETTEXT)

